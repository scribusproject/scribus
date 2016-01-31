/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          shapepalette.cpp  -  description
                             -------------------
    begin                : Sat Mar 28 2015
    copyright            : (C) 2015 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "iconmanager.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scmimedata.h"
#include "scpainter.h"
#include "scribus.h"
#include "scribusXml.h"
#include "scribusdoc.h"
#include "selection.h"
#include "shapepalette.h"
#include "ui/scmessagebox.h"
#include "util.h"
#include "util_math.h"

#include <QApplication>
#include <QByteArray>
#include <QDomDocument>
#include <QDomElement>
#include <QDrag>
#include <QFileDialog>
#include <QMimeData>
#include <QPainter>


ShapeView::ShapeView(QWidget* parent) : QListWidget(parent)
{
	shapes.clear();
	scMW = NULL;

	setDragEnabled(true);
	setViewMode(QListView::IconMode);
	setFlow(QListView::LeftToRight);
	setSortingEnabled(true);
	setWrapping(true);
	setResizeMode(QListView::Adjust);
	setAcceptDrops(false);
	setDropIndicatorShown(true);
	setDragDropMode(QAbstractItemView::DragDrop);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setContextMenuPolicy(Qt::CustomContextMenu);
	delegate = new ScListWidgetDelegate(this, this);
	delegate->setIconOnly(false);
	setItemDelegate(delegate);
	setIconSize(QSize(48, 48));

	connect(this, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(HandleContextMenu(QPoint)));
}

void ShapeView::HandleContextMenu(QPoint)
{
	QMenu *pmenu = new QMenu();
	if (this->count() != 0)
	{
		QListWidgetItem* it = currentItem();
		if (it != NULL)
		{
			QAction* delAct = pmenu->addAction( tr("Delete selected Shape"));
			connect(delAct, SIGNAL(triggered()), this, SLOT(delOne()));
		}
		QAction* delAAct = pmenu->addAction( tr("Delete all Shapes"));
		connect(delAAct, SIGNAL(triggered()), this, SLOT(deleteAll()));
		pmenu->addSeparator();
	}
	QAction* viewAct = pmenu->addAction( tr("Display Icons only"));
	viewAct->setCheckable(true);
	viewAct->setChecked(delegate->iconOnly());
	connect(viewAct, SIGNAL(triggered()), this, SLOT(changeDisplay()));
	pmenu->exec(QCursor::pos());
	delete pmenu;
}

void ShapeView::deleteAll()
{
	int t = ScMessageBox::warning(this, CommonStrings::trWarning, tr("Do you really want to clear all your shapes in this tab?"),
				QMessageBox::Yes | QMessageBox::No,
				QMessageBox::No,	// GUI default
				QMessageBox::Yes);	// batch default
	if (t == QMessageBox::No)
		return;
	shapes.clear();
	clear();
}

void ShapeView::delOne()
{
	QListWidgetItem* it = currentItem();
	if (it != NULL)
	{
		QString key = it->data(Qt::UserRole).toString();
		shapes.remove(key);
		updateShapeList();
	}
}

void ShapeView::changeDisplay()
{
	reset();
	delegate->setIconOnly(!delegate->iconOnly());
	repaint();
}

bool ShapeView::viewportEvent(QEvent *event)
{
	if (event != NULL)
	{
		if (event->type() == QEvent::ToolTip)
		{
			QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
			QListWidgetItem* it = itemAt(helpEvent->pos());
			if (it != NULL)
			{
				event->accept();
				QString tipText = it->text();
				QToolTip::showText(helpEvent->globalPos(), tipText, this);
				return true;
			}
		}
		else if (event->type() == QEvent::MouseButtonRelease)
		{
			QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
			if (mouseEvent->button() == Qt::RightButton)
			{
				emit customContextMenuRequested(mouseEvent->pos());
				return true;
			}
		}
	}
	return QListWidget::viewportEvent(event);
}

void ShapeView::keyPressEvent(QKeyEvent* e)
{
	switch (e->key())
	{
		case Qt::Key_Backspace:
		case Qt::Key_Delete:
			{
				QListWidgetItem* it = currentItem();
				if (it != NULL)
				{
					QString key = it->data(Qt::UserRole).toString();
					if (shapes.contains(key))
					{
						shapes.remove(key);
						updateShapeList();
						e->accept();
					}
				}
			}
			break;
		default:
			break;
	}
}

void ShapeView::dragEnterEvent(QDragEnterEvent *e)
{
	if (e->source() == this)
		e->ignore();
	else
		e->acceptProposedAction();
}

void ShapeView::dragMoveEvent(QDragMoveEvent *e)
{
	if (e->source() == this)
		e->ignore();
	else
		e->acceptProposedAction();
}

void ShapeView::dropEvent(QDropEvent *e)
{
	if (e->mimeData()->hasText())
	{
		e->acceptProposedAction();
		if (e->source() == this)
			return;
		QString text = e->mimeData()->text();
		if ((text.startsWith("<SCRIBUSELEM")) || (text.startsWith("SCRIBUSELEMUTF8")))
			emit objectDropped();
	}
	else
		e->ignore();
}

void ShapeView::startDrag(Qt::DropActions supportedActions)
{
	QString key = currentItem()->data(Qt::UserRole).toString();
	if (shapes.contains(key))
	{
		int w = shapes[key].width;
		int h = shapes[key].height;
		ScribusDoc *m_Doc = new ScribusDoc();
		m_Doc->setup(0, 1, 1, 1, 1, "Custom", "Custom");
		m_Doc->setPage(w, h, 0, 0, 0, 0, 0, 0, false, false);
		m_Doc->addPage(0);
		m_Doc->setGUI(false, scMW, 0);
		int z = m_Doc->itemAdd(PageItem::Polygon, PageItem::Unspecified, m_Doc->currentPage()->xOffset(), m_Doc->currentPage()->yOffset(), w, h, m_Doc->itemToolPrefs().shapeLineWidth, m_Doc->itemToolPrefs().shapeFillColor, m_Doc->itemToolPrefs().shapeLineColor);
		PageItem* ite = m_Doc->Items->at(z);
		ite->PoLine = shapes[key].path.copy();
		FPoint wh = getMaxClipF(&ite->PoLine);
		ite->setWidthHeight(wh.x(),wh.y());
		ite->setTextFlowMode(PageItem::TextFlowDisabled);
		m_Doc->adjustItemSize(ite);
		ite->OldB2 = ite->width();
		ite->OldH2 = ite->height();
		ite->updateClip();
		ite->ClipEdited = true;
		ite->FrameType = 3;
		m_Doc->m_Selection->addItem(ite, true);
		ScElemMimeData* md = ScriXmlDoc::WriteToMimeData(m_Doc, m_Doc->m_Selection);
		QDrag* dr = new QDrag(this);
		dr->setMimeData(md);
		dr->setPixmap(currentItem()->icon().pixmap(QSize(48, 48)));
		dr->exec();
		delete m_Doc;
	}
}

void ShapeView::updateShapeList()
{
	clear();
	setWordWrap(true);
	for (QHash<QString, shapeData>::Iterator it = shapes.begin(); it != shapes.end(); ++it)
	{
		int w = it.value().width + 4;
		int h = it.value().height + 4;
		QImage Ico(w, h, QImage::Format_ARGB32_Premultiplied);
		Ico.fill(0);
		ScPainter *painter = new ScPainter(&Ico, w, h);
		painter->setBrush(qRgb(0, 0, 0));
		painter->setPen(qRgb(0, 0, 0), 1.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		painter->setFillMode(ScPainter::Solid);
		painter->setStrokeMode(ScPainter::Solid);
		painter->translate(2.0, 2.0);
		painter->setupPolygon(&it.value().path);
		painter->drawPolygon();
		painter->end();
		delete painter;
		QPixmap pm;
		if (w >= h)
			pm = QPixmap::fromImage(Ico.scaledToWidth(48, Qt::SmoothTransformation));
		else
			pm = QPixmap::fromImage(Ico.scaledToHeight(48, Qt::SmoothTransformation));
		QPixmap pm2(48, 48);
		pm2.fill(palette().color(QPalette::Base));
		QPainter p;
		p.begin(&pm2);
		p.drawPixmap(24 - pm.width() / 2, 24 - pm.height() / 2, pm);
		p.end();
		QListWidgetItem *item = new QListWidgetItem(pm2, it.value().name, this);
		item->setData(Qt::UserRole, it.key());
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);
	}
}

ShapePalette::ShapePalette( QWidget* parent) : ScDockPalette( parent, "Shap", 0)
{
	setMinimumSize( QSize( 220, 240 ) );
	setObjectName(QString::fromLocal8Bit("Shap"));
	setSizePolicy( QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
	containerWidget = new QWidget(this);
	vLayout = new QVBoxLayout( containerWidget );
	vLayout->setSpacing( 0 );
	vLayout->setMargin( 0 );
	buttonLayout = new QHBoxLayout;
	buttonLayout->setSpacing( 5 );
	buttonLayout->setMargin( 0 );
	importButton = new QToolButton(this);
	importButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	importButton->setIcon(IconManager::instance()->loadIcon("16/document-open.png"));
	importButton->setIconSize(QSize(16, 16));
	buttonLayout->addWidget( importButton );
	QSpacerItem* spacer = new QSpacerItem( 1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum );
	buttonLayout->addItem( spacer );
	closeButton = new QToolButton(this);
	closeButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	closeButton->setIcon(IconManager::instance()->loadIcon("16/close.png"));
	closeButton->setIconSize(QSize(16, 16));
	buttonLayout->addWidget( closeButton );
	vLayout->addLayout( buttonLayout );
	Frame3 = new QToolBox( this );
	vLayout->addWidget(Frame3);
	setWidget(containerWidget);

	unsetDoc();
	m_scMW  = NULL;
	languageChange();
	connect(importButton, SIGNAL(clicked()), this, SLOT(Import()));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(closeTab()));
}

void ShapePalette::writeToPrefs()
{
	QString prFile = QDir::toNativeSeparators(PrefsManager::instance()->preferencesLocation()+"/scribusshapes.xml");
	QFile f(prFile);
	if(!f.open(QIODevice::WriteOnly))
		return;
	QDomDocument docu = QDomDocument("svgdoc");
	QString vo = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	QString st = "<ScribusShape></ScribusShape>";
	docu.setContent(st);
	QDomElement docElement = docu.documentElement();
	for (int a = 0; a < Frame3->count(); a++)
	{
		ShapeViewWidget = (ShapeView*)Frame3->widget(a);
		QDomElement fil = docu.createElement("file");
		fil.setAttribute("name", Frame3->itemText(a));
		for (QHash<QString, shapeData>::Iterator it = ShapeViewWidget->shapes.begin(); it != ShapeViewWidget->shapes.end(); ++it)
		{
			QDomElement shp = docu.createElement("shape");
			shp.setAttribute("width", it.value().width);
			shp.setAttribute("height", it.value().width);
			shp.setAttribute("name", it.value().name);
			shp.setAttribute("path", it.value().path.svgPath(true));
			shp.setAttribute("uuid", it.key());
			fil.appendChild(shp);
		}
		docElement.appendChild(fil);
	}
	QDataStream s(&f);
	QString wr = vo;
	wr += docu.toString();
	QByteArray utf8wr = wr.toUtf8();
	s.writeRawData(utf8wr.data(), utf8wr.length());
	f.close();
}

void ShapePalette::readFromPrefs()
{
	QString prFile = QDir::toNativeSeparators(PrefsManager::instance()->preferencesLocation()+"/scribusshapes.xml");
	QFileInfo fi(prFile);
	if (fi.exists())
	{
		QByteArray docBytes("");
		loadRawText(prFile, docBytes);
		QString docText("");
		docText = QString::fromUtf8(docBytes);
		QDomDocument docu("scridoc");
		docu.setContent(docText);
		QDomElement docElem = docu.documentElement();
		for(QDomElement drawPag = docElem.firstChildElement(); !drawPag.isNull(); drawPag = drawPag.nextSiblingElement() )
		{
			if (drawPag.tagName() == "file")
			{
				ShapeViewWidget = new ShapeView(this);
				ShapeViewWidget->scMW = m_scMW;
				Frame3->addItem(ShapeViewWidget, drawPag.attribute("name"));
				for(QDomElement dpg = drawPag.firstChildElement(); !dpg.isNull(); dpg = dpg.nextSiblingElement() )
				{
					if (dpg.tagName() == "shape")
					{
						shapeData shData;
						shData.height = dpg.attribute("height", "1").toInt();
						shData.width = dpg.attribute("width", "1").toInt();
						shData.path.parseSVG(dpg.attribute("path"));
						shData.name = dpg.attribute("name");
						ShapeViewWidget->shapes.insert(dpg.attribute("uuid"), shData);
					}
				}
				ShapeViewWidget->updateShapeList();
			}
		}
		if (Frame3->count() > 0)
			Frame3->setCurrentIndex(0);
	}
}

void ShapePalette::closeTab()
{
	int index = Frame3->currentIndex();
	ShapeViewWidget = (ShapeView*)Frame3->widget(index);
	Frame3->removeItem(index);
	delete ShapeViewWidget;
}

double ShapePalette::decodePSDfloat(uint data)
{
	double ret = 0.0;
	char man = (data & 0xFF000000) >> 24;
	if (man >= 0)
	{
		ret = (data & 0x00FFFFFF) / 16777215.0;
		ret = (ret + man);
	}
	else
	{
		ret = (~data & 0x00FFFFFF) / 16777215.0;
		ret = (ret + ~man) * -1;
	}
	return ret;
}

void ShapePalette::Import()
{
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString s = QFileDialog::getOpenFileName(this, tr("Choose a shape file to import"), dirs->get("shape_load", "."), tr("Photoshop Custom Shape (*.csh *.CSH)"));
	if (!s.isEmpty())
	{
		QFileInfo fi(s);
		ShapeViewWidget = new ShapeView(this);
		int nIndex = Frame3->addItem(ShapeViewWidget, fi.baseName());
		dirs->set("shape_load", s.left(s.lastIndexOf(QDir::toNativeSeparators("/"))));
		QFile file(s);
		if (!file.open(QFile::ReadOnly))
			return;
		QApplication::setOverrideCursor(Qt::WaitCursor);
		QDataStream ds(&file);
		ds.setByteOrder(QDataStream::BigEndian);
		QByteArray magic;
		magic.resize(4);
		ds.readRawData(magic.data(), 4);
		if (magic != "cush")
			return;
		quint32 version, count, shpCounter;
		shpCounter = 1;
		ds >> version >> count;
		while (!ds.atEnd())
		{
			QString  string = "";
			quint32  length, dummy, shpLen, paDataLen;
			ds >> length;
			for (uint i = 0; i < length; ++i)
			{
				quint16  ch;
				ds >> ch;
				if (ch > 0)
					string += char(ch);
			}
			if (length % 2 != 0)
				ds.skipRawData(2);
			ds >> dummy;
			ds >> shpLen;
			qint64 posi = ds.device()->pos();
			ds.skipRawData(1);
			QByteArray uuid;
			uuid.resize(36);
			ds.readRawData(uuid.data(), 36);
			qint32 x, y, w, h;
			ds >> y >> x >> h >> w;
			paDataLen = shpLen - 53;
			QRect bounds = QRect(QPoint(x,y), QPoint(w, h));
			bool first = false;
			bool pathOpen = false;
			FPoint firstPoint, firstControl;
			FPointArray clip2;
			short type;
			uint data;
			double frac1, frac2, frac3, frac4, frac5, frac6;
			uint offset2;
			offset2 = 0;
			clip2.resize(0);
			while (offset2 < paDataLen)
			{
				ds >> type;
				ds >> data;
				frac1 = decodePSDfloat(data) * bounds.height();
				ds >> data;
				frac2 = decodePSDfloat(data) * bounds.width();
				ds >> data;
				frac3 = decodePSDfloat(data) * bounds.height();
				ds >> data;
				frac4 = decodePSDfloat(data) * bounds.width();
				ds >> data;
				frac5 = decodePSDfloat(data) * bounds.height();
				ds >> data;
				frac6 = decodePSDfloat(data) * bounds.width();
				switch (type)
				{
					case 0:
					case 3:
						if (pathOpen)
						{
							clip2.addPoint(firstPoint);
							clip2.addPoint(firstControl);
							clip2.setMarker();
						}
						pathOpen = false;
						first = true;
						break;
					case 1:
					case 2:
					case 4:
					case 5:
						if (first)
						{
							firstControl = FPoint(frac2, frac1);
							firstPoint = FPoint(frac4, frac3);
							clip2.addPoint(FPoint(frac4, frac3));
							clip2.addPoint(FPoint(frac6, frac5));
						}
						else
						{
							clip2.addPoint(frac4, frac3);
							clip2.addPoint(frac2, frac1);
							clip2.addPoint(frac4, frac3);
							clip2.addPoint(frac6, frac5);
						}
						pathOpen = true;
						first = false;
						break;
					case 6:
						first = true;
						break;
					default:
						break;
				}
				offset2 += 26;
			}
			clip2.addPoint(firstPoint);
			clip2.addPoint(firstControl);
			shapeData shData;
			shData.height = bounds.height();
			shData.width = bounds.width();
			shData.path = clip2.copy();
			shData.name = string;
			ShapeViewWidget->shapes.insert(QString(uuid), shData);
			ds.device()->seek(posi + shpLen);
			shpCounter++;
		}
		file.close();
		Frame3->setCurrentIndex(nIndex);
		ShapeViewWidget->updateShapeList();
		ShapeViewWidget->scMW = m_scMW;
		QApplication::restoreOverrideCursor();
	}
}

void ShapePalette::setMainWindow(ScribusMainWindow *mw)
{
	m_scMW = mw;
	for (int a = 0; a < Frame3->count(); a++)
	{
		ShapeViewWidget = (ShapeView*)Frame3->widget(a);
		ShapeViewWidget->scMW = mw;
	}
}

void ShapePalette::setDoc(ScribusDoc *newDoc)
{
	if (m_scMW == NULL)
		m_doc = NULL;
	else
		m_doc = newDoc;
	if (m_doc == NULL)
		setEnabled(true);
	else
		setEnabled(!m_doc->drawAsPreview);
}

void ShapePalette::unsetDoc()
{
	m_doc = NULL;
	setEnabled(true);
}

void ShapePalette::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		ScDockPalette::changeEvent(e);
}

void ShapePalette::languageChange()
{
	setWindowTitle( tr( "Custom Shapes" ) );
	importButton->setToolTip( tr("Load Photoshop Custom Shapes"));
	closeButton->setToolTip( tr("Close current Tab"));
}
