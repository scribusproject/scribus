/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          xpsexport.cpp  -  description
                             -------------------
    begin                : Sun Nov 24 08:00:00 CEST 2013
    copyright            : (C) 2013 by Franz Schmid
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

#include <QBuffer>
#include <QByteArray>
#include <QComboBox>
#include <QDataStream>
#include <QFile>
#include <QList>
#include <QMessageBox>
#include <QScopedPointer>
#include <QTemporaryDir>
#include <QTextStream>
#include <QUuid>

#include "xpsexplugin.h"

#include "scconfig.h"
#include "canvas.h"
#include "cmsettings.h"
#include "commonstrings.h"
#include "pageitem_pathtext.h"
#include "pageitem_table.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "scpage.h"
#include "scpattern.h"
#include "scribuscore.h"
#include "sctextstruct.h"
#include "tableutils.h"
#include "util.h"
#include "ui/customfdialog.h"
#include "ui/guidemanager.h"
#include "sccolorengine.h"
#include "util_formats.h"
#include "util_math.h"
#include "third_party/zip/scribus_zip.h"

int xpsexplugin_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* xpsexplugin_getPlugin()
{
	XPSExportPlugin* plug = new XPSExportPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void xpsexplugin_freePlugin(ScPlugin* plugin)
{
	XPSExportPlugin* plug = dynamic_cast<XPSExportPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

using namespace TableUtils;

XPSExportPlugin::XPSExportPlugin() : ScActionPlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

XPSExportPlugin::~XPSExportPlugin() {};

void XPSExportPlugin::languageChange()
{
	// Note that we leave the unused members unset. They'll be initialised
	// with their default ctors during construction.
	// Action name
	m_actionInfo.name = "ExportAsXPS";
	// Action text for menu, including accel
	m_actionInfo.text = tr("Save as XPS...");
	// Menu
	m_actionInfo.menu = "FileExport";
	m_actionInfo.enabledOnStartup = false;
	m_actionInfo.needsNumObjects = -1;
}

const QString XPSExportPlugin::fullTrName() const
{
	return QObject::tr("XPS Export");
}

const ScActionPlugin::AboutData* XPSExportPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Franz Schmid <franz@scribus.info>";
	about->shortDescription = tr("Exports XPS Files");
	about->description = tr("Exports the current document into an XPS file.");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void XPSExportPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

bool XPSExportPlugin::run(ScribusDoc* doc, QString filename)
{
	Q_ASSERT(filename.isEmpty());
	QString fileName;
	if (doc!=0)
	{
		PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("xpsex");
		QString wdir = prefs->get("wdir", ".");
		QScopedPointer<CustomFDialog> openDia( new CustomFDialog(doc->scMW(), wdir, QObject::tr("Save as"), QObject::tr("Microsoft XPS (*.xps *.XPS);;All Files (*)"), fdHidePreviewCheckBox) );

		QFrame *Layout = new QFrame(openDia.data());
		QHBoxLayout *Layout1 = new QHBoxLayout(Layout);
		Layout1->setSpacing( 5 );
		QLabel *text = new QLabel(QObject::tr("Output Settings:"), Layout);
		Layout1->addWidget(text);
		QComboBox* compress = new QComboBox(Layout);
		compress->addItem(QObject::tr("Low Resolution"));
		compress->addItem(QObject::tr("Medium Resolution"));
		compress->addItem(QObject::tr("High Resolution"));
		Layout1->addWidget(compress);
		QSpacerItem* spacer = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
		Layout1->addItem( spacer );
		compress->setCurrentIndex(1);
		openDia->addWidgets(Layout);
		QString fna;
		if (doc->hasName)
		{
			QFileInfo fi(doc->DocName);
			QString completeBaseName = fi.completeBaseName();
			if (completeBaseName.endsWith(".xps", Qt::CaseInsensitive))
				completeBaseName.chop(4);
			wdir = QDir::fromNativeSeparators( fi.path() );
			fna  = QDir::fromNativeSeparators( fi.path()+"/"+completeBaseName+".xps" );
		}
		else
		{
			wdir = QDir::fromNativeSeparators( wdir );
			if (wdir.right(1) != "/")
				fna = wdir + "/";
			else
				fna = wdir;
			fna += doc->DocName + ".xps";
		}
		openDia->setSelection(fna);
		openDia->setExtension("xps");

		if (!openDia->exec())
			return true;
		fileName = openDia->selectedFile();
		QFileInfo fi(fileName);
		QString baseDir = fi.absolutePath();
		fileName = baseDir + "/" + fi.baseName() + ".xps";
		if (fileName.isEmpty())
			return true;
		prefs->set("wdir", fileName.left(fileName.lastIndexOf("/")));
		QFile f(fileName);
		if (f.exists())
		{
			int exit = QMessageBox::warning(doc->scMW(), CommonStrings::trWarning,
				QObject::tr("Do you really want to overwrite the file:\n%1 ?").arg(fileName),
				QMessageBox::Yes | QMessageBox::No);
			if (exit == QMessageBox::No)
				return true;
		}
		XPSExPlug *dia = new XPSExPlug(doc, compress->currentIndex());
		dia->doExport(fileName);
		delete dia;
	}
	return true;
}

XPSExPlug::XPSExPlug(ScribusDoc* doc, int output_res)
{
	m_Doc = doc;
	conversionFactor = 96.0 / 72.0;
	m_dpi = 96.0;
	if (output_res == 0)
		m_dpi = 72.0;
	else if (output_res == 1)
		m_dpi = 150.0;
	else if (output_res == 2)
		m_dpi = 300.0;
}

bool XPSExPlug::doExport(QString fName)
{
	zip = new ScZipHandler(true);
	if (!zip->open(fName))
	{
		delete zip;
		return false;
	}
	dir = new QTemporaryDir();
	if (dir->isValid())
	{
		imageCounter = 0;
		fontCounter = 0;
		xps_fontMap.clear();
		baseDir = dir->path();
		// Create directory tree
		QDir outDir(baseDir);
		outDir.mkdir("_rels");
		outDir.mkdir("docProps");
		outDir.mkdir("Documents");
		outDir.cd("Documents");
		outDir.mkdir("1");
		outDir.cd("1");
		outDir.mkdir("_rels");
		outDir.mkdir("Pages");
		outDir.cd("Pages");
		outDir.mkdir("_rels");
		outDir.cdUp();
		outDir.mkdir("Structure");
		outDir.cdUp();
		outDir.cdUp();
		outDir.mkdir("Resources");
		outDir.cd("Resources");
		outDir.mkdir("Images");
		outDir.mkdir("Fonts");
		outDir.cdUp();
		writeBaseRel();
		writeContentType();
		writeCore();
		writeDocRels();
		// Write Thumbnail
		QImage thumb = m_Doc->view()->PageToPixmap(0, 256, false);
		thumb.save(baseDir + "/docProps/thumbnail.jpeg", "JPG");
		// Write required DocStructure.struct
		QFile fts(baseDir + "/Documents/1/Structure/DocStructure.struct");
		if (fts.open(QIODevice::WriteOnly))
		{
			fts.write(QByteArray("<DocumentStructure xmlns=\"http://schemas.microsoft.com/xps/2005/06/documentstructure\">\n</DocumentStructure>"));
			fts.close();
		}
		// Write required FixedDocSeq.fdseq
		QFile ft(baseDir + "/FixedDocSeq.fdseq");
		if (ft.open(QIODevice::WriteOnly))
		{
			ft.write(QByteArray("<FixedDocumentSequence xmlns=\"http://schemas.microsoft.com/xps/2005/06\">\n\t<DocumentReference Source=\"/Documents/1/FixedDoc.fdoc\"/>\n</FixedDocumentSequence>"));
			ft.close();
		}
		// Write required FixedDoc.fdoc
		f_docu = QDomDocument("xpsdoc");
		QString st = "<FixedDocument></FixedDocument>";
		f_docu.setContent(st);
		QDomElement root  = f_docu.documentElement();
		root.setAttribute("xmlns", "http://schemas.microsoft.com/xps/2005/06");
		f_docu.appendChild(root);
		writePages(root);
		QFile fdo(baseDir + "/Documents/1/FixedDoc.fdoc");
		if (fdo.open(QIODevice::WriteOnly))
		{
			QString vo = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
			QDataStream s(&fdo);
			vo += f_docu.toString();
			QByteArray utf8wr = vo.toUtf8();
			s.writeRawData(utf8wr.data(), utf8wr.length());
			fdo.close();
		}
		zip->write(baseDir);
	}
	zip->close();
	delete zip;
	delete dir;
	return true;
}

void XPSExPlug::writePages(QDomElement &root)
{
	for (int a = 0; a < m_Doc->Pages->count(); a++)
	{
		ScPage* Page = m_Doc->Pages->at(a);
		p_docu.setContent(QString("<FixedPage></FixedPage>"));
		QDomElement droot  = p_docu.documentElement();
		droot.setAttribute("xmlns", "http://schemas.microsoft.com/xps/2005/06");
		droot.setAttribute("Width", QString("%1").arg(Page->width() * conversionFactor));
		droot.setAttribute("Height", QString("%1").arg(Page->height() * conversionFactor));
		QString lang = QLocale::system().name();
		lang.replace("_", "-");
		droot.setAttribute("xml:lang", lang);
		r_docu.setContent(QString("<Relationships></Relationships>"));
		QDomElement rroot  = r_docu.documentElement();
		rroot.setAttribute("xmlns", "http://schemas.openxmlformats.org/package/2006/relationships");
		writePage(droot, rroot, Page);
		p_docu.appendChild(droot);
		r_docu.appendChild(rroot);
		QFile ft(baseDir + QString("/Documents/1/Pages/%1.fpage").arg(a+1));
		if (ft.open(QIODevice::WriteOnly))
		{
			QString vo = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
			QDataStream s(&ft);
			vo += p_docu.toString();
			QByteArray utf8wr = vo.toUtf8();
			s.writeRawData(utf8wr.data(), utf8wr.length());
			ft.close();
		}
		QFile ftr(baseDir + QString("/Documents/1/Pages/_rels/%1.fpage.rels").arg(a+1));
		if (ftr.open(QIODevice::WriteOnly))
		{
			QString vo = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
			QDataStream s(&ftr);
			vo += r_docu.toString();
			QByteArray utf8wr = vo.toUtf8();
			s.writeRawData(utf8wr.data(), utf8wr.length());
			ftr.close();
		}
		QDomElement rel1 = f_docu.createElement("PageContent");
		rel1.setAttribute("Source", QString("Pages/%1.fpage").arg(a+1));
		root.appendChild(rel1);
		p_docu.clear();
		r_docu.clear();
	}
}

void XPSExPlug::writePage(QDomElement &doc_root, QDomElement &rel_root, ScPage *Page)
{
	ScLayer ll;
	ll.isPrintable = false;
	for (int la = 0; la < m_Doc->Layers.count(); la++)
	{
		m_Doc->Layers.levelToLayer(ll, la);
		if (ll.isPrintable)
		{
			ScPage *mpage = m_Doc->MasterPages.at(m_Doc->MasterNames[Page->MPageNam]);
			writePageLayer(doc_root, rel_root, mpage, ll);
			writePageLayer(doc_root, rel_root, Page, ll);
		}
	}
}

void XPSExPlug::writePageLayer(QDomElement &doc_root, QDomElement &rel_root, ScPage *page, ScLayer& layer)
{
	PageItem *Item;
	QList<PageItem*> Items;
	ScPage* SavedAct = m_Doc->currentPage();
	if (page->pageName().isEmpty())
		Items = m_Doc->DocItems;
	else
		Items = m_Doc->MasterItems;
	if (Items.count() == 0)
		return;
	if (!layer.isPrintable)
		return;
	m_Doc->setCurrentPage(page);
	QDomElement layerGroup = p_docu.createElement("Canvas");
	if (layer.transparency != 1.0)
		layerGroup.setAttribute("Opacity", layer.transparency);
	for(int j = 0; j < Items.count(); ++j)
	{
		Item = Items.at(j);
		if (Item->LayerID != layer.ID)
			continue;
		if (!Item->printEnabled())
			continue;
		double x = page->xOffset();
		double y = page->yOffset();
		double w = page->width();
		double h = page->height();
		double x2 = Item->BoundingX;
		double y2 = Item->BoundingY;
		double w2 = Item->BoundingW;
		double h2 = Item->BoundingH;
		if (!( qMax( x, x2 ) <= qMin( x+w, x2+w2 ) && qMax( y, y2 ) <= qMin( y+h, y2+h2 )))
			continue;
		if ((!page->pageName().isEmpty()) && (Item->OwnPage != static_cast<int>(page->pageNr())) && (Item->OwnPage != -1))
			continue;
		writeItemOnPage(Item->xPos() - page->xOffset(), Item->yPos() - page->yOffset(), Item, layerGroup, rel_root);
	}
	doc_root.appendChild(layerGroup);
	m_Doc->setCurrentPage(SavedAct);
}

void XPSExPlug::writeItemOnPage(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root)
{
	switch (Item->itemType())
	{
		case PageItem::Arc:
		case PageItem::Polygon:
		case PageItem::PolyLine:
		case PageItem::RegularPolygon:
		case PageItem::Spiral:
			if (checkForFallback(Item))
				handleImageFallBack(xOffset, yOffset, Item, parentElem, rel_root);
			else
			{
				processPolyItem(xOffset, yOffset, Item, parentElem, rel_root);
				if ((Item->lineColor() != CommonStrings::None) && ((Item->startArrowIndex() != 0) || (Item->endArrowIndex() != 0)))
					processArrows(xOffset, yOffset, Item, parentElem, rel_root);
			}
			break;
		case PageItem::Line:
			if (checkForFallback(Item))
				handleImageFallBack(xOffset, yOffset, Item, parentElem, rel_root);
			else
			{
				processLineItem(xOffset, yOffset, Item, parentElem, rel_root);
				if ((Item->lineColor() != CommonStrings::None) && ((Item->startArrowIndex() != 0) || (Item->endArrowIndex() != 0)))
					processArrows(xOffset, yOffset, Item, parentElem, rel_root);
			}
			break;
		case PageItem::ImageFrame:
		case PageItem::LatexFrame:
			if (checkForFallback(Item))
				handleImageFallBack(xOffset, yOffset, Item, parentElem, rel_root);
			else
				processImageItem(xOffset, yOffset, Item, parentElem, rel_root);
			break;
		case PageItem::TextFrame:
			if (checkForFallback(Item))
				handleImageFallBack(xOffset, yOffset, Item, parentElem, rel_root);
			else
				processTextItem(xOffset, yOffset, Item, parentElem, rel_root);
			break;
		case PageItem::PathText:
			if (checkForFallback(Item))
				handleImageFallBack(xOffset, yOffset, Item, parentElem, rel_root);
			else
				processPathTextItem(xOffset, yOffset, Item, parentElem, rel_root);
			break;
		case PageItem::Table:
			if (checkForFallback(Item))
				handleImageFallBack(xOffset, yOffset, Item, parentElem, rel_root);
			else
				processTableItem(xOffset, yOffset, Item, parentElem, rel_root);
			break;
		case PageItem::Symbol:
			if (checkForFallback(Item))
				handleImageFallBack(xOffset, yOffset, Item, parentElem, rel_root);
			else
				processSymbolItem(xOffset, yOffset, Item, parentElem, rel_root);
			break;
		case PageItem::Group:
			if (Item->groupItemList.count() > 0)
			{
				if (checkForFallback(Item))
					handleImageFallBack(xOffset, yOffset, Item, parentElem, rel_root);
				else
				{
					QDomElement ob = p_docu.createElement("Canvas");
					if (Item->GrMask > 0)
						handleMask(1, Item, ob, rel_root, xOffset, yOffset);
					else
					{
						if (Item->fillTransparency() != 0)
							ob.setAttribute("Opacity", FToStr(1.0 - Item->fillTransparency()));
					}
					FPointArray path = Item->PoLine.copy();
					path.scale(conversionFactor, conversionFactor);
					QString pa = SetClipPath(&path, true);
					if (Item->fillRule)
						pa.prepend("F 0 ");
					else
						pa.prepend("F 1 ");
					ob.setAttribute("Clip", pa);
					QTransform mpx;
					mpx.translate(xOffset * conversionFactor, yOffset * conversionFactor);
					mpx.scale(Item->width() / Item->groupWidth, Item->height() / Item->groupHeight);
					if ((Item->rotation() != 0.0) || Item->imageFlippedH() || Item->imageFlippedV())
					{
						mpx.rotate(Item->rotation());
						if (Item->imageFlippedH())
						{
							mpx.translate(Item->width() * conversionFactor, 0);
							mpx.scale(-1, 1);
						}
						if (Item->imageFlippedV())
						{
							mpx.translate(0, Item->height() * conversionFactor);
							mpx.scale(1, -1);
						}
					}
					ob.setAttribute("RenderTransform", MatrixToStr(mpx));
					for (int em = 0; em < Item->groupItemList.count(); ++em)
					{
						PageItem* embed = Item->groupItemList.at(em);
						writeItemOnPage(embed->gXpos, embed->gYpos, embed, ob, rel_root);
					}
					parentElem.appendChild(ob);
				}
			}
			break;
		default:
			handleImageFallBack(xOffset, yOffset, Item, parentElem, rel_root);
			break;
	}
}

void XPSExPlug::handleImageFallBack(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root)
{
	QDomElement ob = p_docu.createElement("Path");
	QRectF bounds = Item->getVisualBoundingRect();
	QPainterPath path;
	path.moveTo(0, 0);
	path.lineTo(bounds.width(), 0);
	path.lineTo(bounds.width(), bounds.height());
	path.lineTo(0, bounds.height());
	path.closeSubpath();
	QTransform mpx;
	if (Item->rotation() != 0.0)
	{
		mpx.translate(xOffset * conversionFactor, yOffset * conversionFactor);
		mpx.rotate(Item->rotation());
		mpx.translate(-xOffset * conversionFactor, -yOffset * conversionFactor);
	}
	QTransform mpp;
	mpp.translate((Item->visualXPos() - m_Doc->currentPage()->xOffset()) * conversionFactor, (Item->visualYPos() - m_Doc->currentPage()->yOffset()) * conversionFactor);
	mpp.scale(conversionFactor, conversionFactor);
	path = mpp.map(path);
	FPointArray fPath;
	fPath.fromQPainterPath(path, true);
	QString pa = SetClipPath(&fPath, true);
	if (Item->fillRule)
		pa.prepend("F 0 ");
	else
		pa.prepend("F 1 ");
	ob.setAttribute("Data", pa);
	if (Item->rotation() != 0.0)
		ob.setAttribute("RenderTransform", MatrixToStr(mpx));
	QDomElement obf = p_docu.createElement("Path.Fill");
	QDomElement gr = p_docu.createElement("ImageBrush");
	double maxSize = qMax(Item->visualWidth(), Item->visualHeight());
	maxSize = qMin(3000.0, maxSize * (m_dpi / 72.0));
	QImage tmpImg = Item->DrawObj_toImage(maxSize);
	tmpImg.save(baseDir + "/Resources/Images/" + QString("%1.png").arg(imageCounter), "PNG");
	gr.setAttribute("TileMode", "None");
	gr.setAttribute("ViewboxUnits", "Absolute");
	gr.setAttribute("ViewportUnits", "Absolute");
	gr.setAttribute("Viewport", "0,0,1,1");
	gr.setAttribute("Viewbox", QString("0, 0, %1, %2").arg(tmpImg.width()).arg(tmpImg.height()));
	gr.setAttribute("Viewport", QString("%1, %2, %3, %4").arg((Item->visualXPos() - m_Doc->currentPage()->xOffset()) * conversionFactor).arg((Item->visualYPos() - m_Doc->currentPage()->yOffset()) * conversionFactor).arg(bounds.width() * conversionFactor).arg(bounds.height() * conversionFactor));
	gr.setAttribute("ImageSource", "/Resources/Images/" + QString("%1.png").arg(imageCounter));
	QDomElement rel = r_docu.createElement("Relationship");
	rel.setAttribute("Id", QString("rIDi%1").arg(imageCounter));
	rel.setAttribute("Type", "http://schemas.microsoft.com/xps/2005/06/required-resource");
	rel.setAttribute("Target", "/Resources/Images/" + QString("%1.png").arg(imageCounter));
	rel_root.appendChild(rel);
	imageCounter++;
	obf.appendChild(gr);
	ob.appendChild(obf);
	parentElem.appendChild(ob);
}

void XPSExPlug::processPolyItem(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root)
{
	if (((Item->GrType != 0) || (Item->fillColor() != CommonStrings::None)) || ((Item->GrTypeStroke != 0) || (Item->lineColor() != CommonStrings::None) || !Item->NamedLStyle.isEmpty()))
	{
		bool closedPath;
		if ((Item->itemType() == PageItem::Polygon) || (Item->itemType() == PageItem::RegularPolygon) || (Item->itemType() == PageItem::Arc))
			closedPath = true;
		else
			closedPath = false;
		QDomElement ob = p_docu.createElement("Path");
		FPointArray path = Item->PoLine.copy();
		QTransform mpx;
		if (Item->rotation() != 0.0)
		{
			mpx.translate(xOffset * conversionFactor, yOffset * conversionFactor);
			mpx.rotate(Item->rotation());
			mpx.translate(-xOffset * conversionFactor, -yOffset * conversionFactor);
		}
		path.translate(xOffset, yOffset);
		path.scale(conversionFactor, conversionFactor);
		QString pa = SetClipPath(&path, closedPath);
		if (Item->fillRule)
			pa.prepend("F 0 ");
		else
			pa.prepend("F 1 ");
		ob.setAttribute("Data", pa);
		if (Item->GrMask > 0)
			handleMask(3, Item, ob, rel_root, xOffset, yOffset);
		getFillStyle(Item, ob, rel_root, xOffset, yOffset);
		if (Item->NamedLStyle.isEmpty())
		{
			if ((!Item->strokePattern().isEmpty()) && (Item->patternStrokePath))
			{
				processSymbolStroke(xOffset, yOffset, Item, parentElem, rel_root);
			}
			else
			{
				getStrokeStyle(Item, ob, rel_root, xOffset, yOffset);
				if (Item->rotation() != 0.0)
					ob.setAttribute("RenderTransform", MatrixToStr(mpx));
				parentElem.appendChild(ob);
			}
		}
		else
		{
			QDomElement grp2 = p_docu.createElement("Canvas");
			multiLine ml = m_Doc->MLineStyles[Item->NamedLStyle];
			for (int it = ml.size()-1; it > -1; it--)
			{
				if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
				{
					QDomElement ob3 = p_docu.createElement("Path");
					ob3.setAttribute("Data", pa);
					GetMultiStroke(&ml[it], ob3);
					grp2.appendChild(ob3);
				}
			}
			if (Item->lineTransparency() != 0)
				grp2.setAttribute("Opacity", FToStr(1.0 - Item->lineTransparency()));
			if (Item->rotation() != 0.0)
				grp2.setAttribute("RenderTransform", MatrixToStr(mpx));
			parentElem.appendChild(grp2);
		}
	}
	return;
}

void XPSExPlug::processLineItem(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root)
{
	if ((Item->GrTypeStroke != 0) || (Item->lineColor() != CommonStrings::None) || !Item->NamedLStyle.isEmpty())
	{
		QDomElement ob;
		double x1 = xOffset * conversionFactor;
		double y1 = yOffset * conversionFactor;
		double x2 = (Item->width() + xOffset) * conversionFactor;
		double y2 = yOffset * conversionFactor;
		QLineF line = QLineF(x1, y1, x2, y2);
		line.setAngle(-Item->rotation());
		if (Item->NamedLStyle.isEmpty())
		{
			ob = p_docu.createElement("Path");
			ob.setAttribute("Data", QString("M %1, %2 L %3, %4").arg(line.p1().x()).arg(line.p1().y()).arg(line.p2().x()).arg(line.p2().y()));
			getStrokeStyle(Item, ob, rel_root, xOffset, yOffset);
		}
		else
		{
			ob = p_docu.createElement("Canvas");
			multiLine ml = m_Doc->MLineStyles[Item->NamedLStyle];
			for (int it = ml.size()-1; it > -1; it--)
			{
				if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
				{
					QDomElement ob2 = p_docu.createElement("Path");
					ob2.setAttribute("Data", QString("M %1, %2 L %3, %4").arg(line.p1().x()).arg(line.p1().y()).arg(line.p2().x()).arg(line.p2().y()));
					GetMultiStroke(&ml[it], ob2);
					ob.appendChild(ob2);
				}
			}
			if (Item->lineTransparency() != 0)
				ob.setAttribute("Opacity", FToStr(1.0 - Item->lineTransparency()));
		}
		parentElem.appendChild(ob);
	}
	return;
}

void XPSExPlug::processImageItem(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root)
{
	FPointArray path = Item->PoLine.copy();
	path.translate(xOffset, yOffset);
	path.scale(conversionFactor, conversionFactor);
	QString pa = SetClipPath(&path, true);
	if (Item->fillRule)
		pa.prepend("F 0 ");
	else
		pa.prepend("F 1 ");
	QDomElement grp = p_docu.createElement("Canvas");
	QTransform mpx;
	if (Item->rotation() != 0.0)
	{
		mpx.translate(xOffset * conversionFactor, yOffset * conversionFactor);
		mpx.rotate(Item->rotation());
		mpx.translate(-xOffset * conversionFactor, -yOffset * conversionFactor);
		grp.setAttribute("RenderTransform", MatrixToStr(mpx));
	}
	if (Item->GrMask > 0)
		handleMask(1, Item, grp, rel_root, xOffset, yOffset);
	else
	{
		if (Item->fillTransparency() != 0)
			grp.setAttribute("Opacity", FToStr(1.0 - Item->fillTransparency()));
	}
	if ((Item->GrType != 0) || (Item->fillColor() != CommonStrings::None))
	{
		QDomElement ob = p_docu.createElement("Path");
		ob.setAttribute("Data", pa);
		getFillStyle(Item, ob, rel_root, xOffset, yOffset, false);
		grp.appendChild(ob);
	}
	if ((Item->PictureIsAvailable) && (!Item->Pfile.isEmpty()))
	{
		QDomElement ob2 = p_docu.createElement("Path");
		ob2.setAttribute("Data", pa);
		ob2.setAttribute("Clip", pa);
		QDomElement obf = p_docu.createElement("Path.Fill");
		QDomElement gr = p_docu.createElement("ImageBrush");
		ScImage img;
		CMSettings cms(m_Doc, Item->IProfile, Item->IRender);
		cms.setUseEmbeddedProfile(Item->UseEmbedded);
		cms.allowSoftProofing(true);
		img.loadPicture(Item->Pfile, Item->pixm.imgInfo.actualPageNumber, cms, ScImage::RGBData, 96);
		img.applyEffect(Item->effectsInUse, m_Doc->PageColors, true);
		img.qImagePtr()->setDotsPerMeterX(3780);
		img.qImagePtr()->setDotsPerMeterY(3780);
		img.qImage().save(baseDir + "/Resources/Images/" + QString("%1.png").arg(imageCounter), "PNG");
		gr.setAttribute("TileMode", "None");
		gr.setAttribute("ViewboxUnits", "Absolute");
		gr.setAttribute("ViewportUnits", "Absolute");
		gr.setAttribute("Viewport", "0,0,1,1");
		gr.setAttribute("Viewbox", QString("0, 0, %1, %2").arg(img.width()).arg(img.height()));
		QTransform mpx;
		double xpos = Item->imageXOffset() * Item->imageXScale();
		double ypos = Item->imageYOffset() * Item->imageYScale();
		mpx.translate((xOffset + xpos) * conversionFactor, (yOffset + ypos) * conversionFactor);
		mpx.scale(img.width() * Item->imageXScale() * conversionFactor, img.height() * Item->imageYScale() * conversionFactor);
		if (Item->imageFlippedH())
		{
			mpx.translate(Item->width() / (img.width() * Item->imageXScale()), 0);
			mpx.scale(-1, 1);
		}
		if (Item->imageFlippedV())
		{
			mpx.translate(0, Item->height() / (img.height() * Item->imageYScale()));
			mpx.scale(1, -1);
		}
		mpx.rotate(Item->imageRotation());
		gr.setAttribute("Transform", MatrixToStr(mpx));
		gr.setAttribute("ImageSource", "/Resources/Images/" + QString("%1.png").arg(imageCounter));
		QDomElement rel = r_docu.createElement("Relationship");
		rel.setAttribute("Id", QString("rIDi%1").arg(imageCounter));
		rel.setAttribute("Type", "http://schemas.microsoft.com/xps/2005/06/required-resource");
		rel.setAttribute("Target", "/Resources/Images/" + QString("%1.png").arg(imageCounter));
		rel_root.appendChild(rel);
		imageCounter++;
		obf.appendChild(gr);
		ob2.appendChild(obf);
		grp.appendChild(ob2);
	}
	parentElem.appendChild(grp);
	if ((Item->GrTypeStroke != 0) || (Item->lineColor() != CommonStrings::None) || !Item->NamedLStyle.isEmpty())
	{
		if (Item->NamedLStyle.isEmpty())
		{
			if ((!Item->strokePattern().isEmpty()) && (Item->patternStrokePath))
			{
				processSymbolStroke(xOffset, yOffset, Item, parentElem, rel_root);
			}
			else
			{
				QDomElement ob3 = p_docu.createElement("Path");
				ob3.setAttribute("Data", pa);
				getStrokeStyle(Item, ob3, rel_root, xOffset, yOffset);
				if (Item->rotation() != 0.0)
					ob3.setAttribute("RenderTransform", MatrixToStr(mpx));
				parentElem.appendChild(ob3);
			}
		}
		else
		{
			QDomElement grp2 = p_docu.createElement("Canvas");
			multiLine ml = m_Doc->MLineStyles[Item->NamedLStyle];
			for (int it = ml.size()-1; it > -1; it--)
			{
				if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
				{
					QDomElement ob3 = p_docu.createElement("Path");
					ob3.setAttribute("Data", pa);
					GetMultiStroke(&ml[it], ob3);
					grp2.appendChild(ob3);
				}
			}
			if (Item->lineTransparency() != 0)
				grp2.setAttribute("Opacity", FToStr(1.0 - Item->lineTransparency()));
			if (Item->rotation() != 0.0)
				grp2.setAttribute("RenderTransform", MatrixToStr(mpx));
			parentElem.appendChild(grp2);
		}
	}
	return;
}

void XPSExPlug::processTextItem(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root)
{
	if (Item->isAnnotation())
		return;
	FPointArray path = Item->PoLine.copy();
	path.scale(conversionFactor, conversionFactor);
	QString pa = SetClipPath(&path, true);
	if (Item->fillRule)
		pa.prepend("F 0 ");
	else
		pa.prepend("F 1 ");
	QDomElement grp = p_docu.createElement("Canvas");
	QTransform mpx;
	QTransform mpl;
	mpl.translate(xOffset * conversionFactor, yOffset * conversionFactor);
	mpx.translate(xOffset * conversionFactor, yOffset * conversionFactor);
	if ((Item->rotation() != 0.0) || Item->imageFlippedH() || Item->imageFlippedV())
	{
		mpx.rotate(Item->rotation());
		mpl.rotate(Item->rotation());
		if (Item->imageFlippedH())
		{
			mpx.translate(Item->width() * conversionFactor, 0);
			mpx.scale(-1, 1);
		}
		if (Item->imageFlippedV())
		{
			mpx.translate(0, Item->height() * conversionFactor);
			mpx.scale(1, -1);
		}
	}
	grp.setAttribute("RenderTransform", MatrixToStr(mpx));
	if (Item->isBookmark)
		grp.setAttribute("Name", Item->itemName());
	if (Item->GrMask > 0)
		handleMask(1, Item, grp, rel_root, xOffset, yOffset);
	else
	{
		if (Item->fillTransparency() != 0)
			grp.setAttribute("Opacity", FToStr(1.0 - Item->fillTransparency()));
	}
	if ((Item->GrType != 0) || (Item->fillColor() != CommonStrings::None))
	{
		FPointArray pathi = Item->PoLine.copy();
		if (Item->imageFlippedH() || Item->imageFlippedV())
		{
			QTransform mpi;
			if (Item->imageFlippedH())
			{
				mpi.translate(Item->width(), 0);
				mpi.scale(-1, 1);
			}
			if (Item->imageFlippedV())
			{
				mpi.translate(0, Item->height());
				mpi.scale(1, -1);
			}
			pathi.map(mpi);
		}
		pathi.scale(conversionFactor, conversionFactor);
		QString pai = SetClipPath(&pathi, true);
		if (Item->fillRule)
			pai.prepend("F 0 ");
		else
			pai.prepend("F 1 ");
		QDomElement ob = p_docu.createElement("Path");
		ob.setAttribute("Data", pai);
		getFillStyle(Item, ob, rel_root, xOffset, yOffset, false);
		grp.appendChild(ob);
	}
	for (uint ll=0; ll < Item->itemText.lines(); ++ll)
	{
		LineSpec ls = Item->itemText.line(ll);
		QList<txtRunItem> current_run;
		QList<QList<txtRunItem> > textRuns;
		QList<txtRunItem> specialText;
		double CurX = ls.x;
		for (int a = ls.firstItem; a <= ls.lastItem; ++a)
		{
			ScText *hl = Item->itemText.item_p(a);
			CharStyle charStyle = Item->itemText.charStyle(a);
			const ScFace* font = &charStyle.font();
			txtRunItem txItem;
			txItem.hl = hl;
			txItem.style = charStyle;
			txItem.index = a;
			txItem.CurX = CurX;
			if (!((font->type() == ScFace::TTF) || (font->type() == ScFace::OTF)))
			{
				specialText.append(txItem);
				textRuns.append(current_run);
				current_run.clear();
				CurX += txItem.hl->glyph.wide();
				continue;
			}
			if (SpecialChars::isBreak(hl->ch, true) || (hl->ch == QChar(10)))
			{
				continue;
			}
			if (charStyle.effects() & ScLayout_SuppressSpace)
			{
				continue;
			}
			if ((hl->ch == SpecialChars::OLD_NBSPACE) || (hl->ch == SpecialChars::OBJECT) || (hl->effects() & ScStyle_UserStyles))
			{
				specialText.append(txItem);
				textRuns.append(current_run);
				current_run.clear();
				CurX += txItem.hl->glyph.wide();
				continue;
			}
			QString guidFont;
			if (!xps_fontMap.contains(font->replacementName()))
			{
				guidFont = embedFont(font, rel_root);
				xps_fontMap.insert(font->replacementName(), guidFont);
			}
			else
				guidFont = xps_fontMap[font->replacementName()];
			if (current_run.isEmpty())
				current_run.append(txItem);
			else
			{
				txtRunItem txItemL = current_run.last();
				double chl = (txItemL.style.fontSize() / 10.0) * qMax(txItemL.hl->glyph.scaleV, txItemL.hl->glyph.scaleH) * conversionFactor;
				const ScFace* fontL = &txItemL.style.font();
				QString guidFontL = xps_fontMap[fontL->replacementName()];
				StyleFlag old_sty = txItemL.style.effects();
				int old_shade = txItemL.style.fillShade();
				QString old_fill = txItemL.style.fillColor();
				double chs = (charStyle.fontSize() / 10.0) * qMax(hl->glyph.scaleV, hl->glyph.scaleH) * conversionFactor;
				if ((chs != chl) || (guidFont != guidFontL) || (charStyle.effects() != old_sty) || (charStyle.fillColor() != old_fill) || (charStyle.fillShade() != old_shade))
				{
					textRuns.append(current_run);
					current_run.clear();
				}
				current_run.append(txItem);
			}
			CurX += txItem.hl->glyph.wide();
		}
		textRuns.append(current_run);
		for (int txr = 0; txr < textRuns.count(); txr++)
		{
			QString uniString = "";
			QString indString = "";
			current_run = textRuns[txr];
			if (current_run.isEmpty())
				continue;
			double StartX = current_run[0].CurX;
			CurX = current_run[0].CurX;
			double chs = (current_run[0].style.fontSize() / 10.0) * qMax(current_run[0].hl->glyph.scaleV, current_run[0].hl->glyph.scaleH) * conversionFactor;
			for (int cr = 0; cr < current_run.count(); cr++)
			{
				txtRunItem txItem = current_run[cr];
				if ((txItem.hl->ch == SpecialChars::PAGENUMBER) || (txItem.hl->ch == SpecialChars::PAGECOUNT))
				{
					QString exString = Item->ExpandToken(txItem.index);
					for (int ex = 0; ex < exString.count(); ex++)
					{
						indString += ";";
					}
					uniString += exString;
				}
				else if ((SpecialChars::isBreakingSpace(txItem.hl->ch)) || (SpecialChars::isExpandingSpace(txItem.hl->ch)) || (txItem.hl->ch.isSpace()))
				{
					indString += QString(",%1;").arg((txItem.hl->glyph.wide() * conversionFactor) / chs * 100);
					uniString += " ";
				}
				else
				{
					uniString += txItem.hl->ch;
					if ((txItem.hl->glyph.xoffset != 0) || (txItem.hl->glyph.yoffset != 0))
						indString += QString(",%1,%2,%3;").arg((txItem.hl->glyph.wide() * conversionFactor) / chs * 100).arg((txItem.hl->glyph.xoffset * conversionFactor) / chs * 100).arg((-txItem.hl->glyph.yoffset * conversionFactor) / chs * 100);
					else
						indString += QString(",%1;").arg((txItem.hl->glyph.wide() * conversionFactor) / chs * 100);
				}
				CurX += txItem.hl->glyph.wide();
			}
			if (!indString.isEmpty())
				indString.chop(1);
			QDomElement gly = p_docu.createElement("Glyphs");
			gly.setAttribute("BidiLevel", "0");
			gly.setAttribute("StyleSimulations", "None");
			gly.setAttribute("FontRenderingEmSize", FToStr(chs));
			const ScFace* fontL = &current_run[0].style.font();
			gly.setAttribute("FontUri", xps_fontMap[fontL->replacementName()]);
			gly.setAttribute("Fill", SetColor(current_run[0].style.fillColor(), current_run[0].style.fillShade(), 0));
			gly.setAttribute("UnicodeString", uniString);
			if (current_run.count() == 1)
			{
				gly.setAttribute("OriginX", FToStr((StartX + current_run[0].hl->glyph.xoffset) * conversionFactor));
				gly.setAttribute("OriginY", FToStr((ls.y + current_run[0].hl->glyph.yoffset) * conversionFactor));
			}
			else
			{
				gly.setAttribute("OriginX", FToStr(StartX * conversionFactor));
				gly.setAttribute("OriginY", FToStr(ls.y * conversionFactor));
				gly.setAttribute("Indices", indString);
			}
			grp.appendChild(gly);
		}
		for (int cr = 0; cr < specialText.count(); cr++)
		{
			txtRunItem txItem = specialText[cr];
			CurX = txItem.CurX;
			if (txItem.hl->ch == SpecialChars::OBJECT)
			{
				if (txItem.hl->getItem(m_Doc))
				{
					QDomElement obO = p_docu.createElement("Canvas");
					QTransform mm;
					mm.translate(CurX * conversionFactor, (ls.y - (txItem.hl->getItem(m_Doc)->height() * (txItem.style.scaleV() / 1000.0))) * conversionFactor);
					if (txItem.style.baselineOffset() != 0)
						mm.translate(0, (txItem.style.baselineOffset() / 1000.0) * conversionFactor);
					if (txItem.style.scaleH() != 1000)
						mm.scale(txItem.style.scaleH() / 1000.0, 1);
					if (txItem.style.scaleV() != 1000)
						mm.scale(1, txItem.style.scaleV() / 1000.0);
					obO.setAttribute("RenderTransform", MatrixToStr(mm));
					QList<PageItem*> emG = txItem.hl->getGroupedItems(m_Doc);
					for (int em = 0; em < emG.count(); ++em)
					{
						PageItem* embed = emG.at(em);
						writeItemOnPage(embed->gXpos, embed->gYpos, embed, obO, rel_root);
					}
					grp.appendChild(obO);
				}
				continue;
			}
			QString chstr = txItem.hl->ch;
			if ((txItem.hl->ch == SpecialChars::PAGENUMBER) || (txItem.hl->ch == SpecialChars::PAGECOUNT))
				chstr = Item->ExpandToken(txItem.index);
			double chs = txItem.style.fontSize();
			if (txItem.hl->effects() & ScStyle_SmallCaps)
			{
				if (chstr.toUpper() != chstr)
				{
					chs = qMax(static_cast<int>(txItem.hl->fontSize() * m_Doc->typographicPrefs().valueSmallCaps / 100), 1);
					chstr = chstr.toUpper();
				}
			}
			else if (txItem.hl->effects() & ScStyle_AllCaps)
				chstr = chstr.toUpper();
			for (int cx = 0; cx < chstr.count(); cx++)
			{
				FPointArray pts;
				QChar chstrex = chstr[cx];
				uint chr = chstrex.unicode();
				QTransform chma;
				uint gl = txItem.style.font().char2CMap(chr);
				if (!chstrex.isSpace())
				{
					pts = txItem.style.font().glyphOutline(gl);
					if (pts.size() < 4)
						continue;
					chma = QTransform();
					chma.scale(txItem.hl->glyph.scaleH * txItem.style.fontSize() / 100.00, txItem.hl->glyph.scaleV * txItem.style.fontSize() / 100.0);
					pts.map(chma);
					if (txItem.hl->effects() & (ScStyle_Subscript | ScStyle_Superscript))
						pts.translate(0, -(chs / 10.0 * txItem.hl->glyph.scaleV));
					else
						pts.translate(0, -(chs / 10.0));
					pts.translate(CurX, ls.y);
					if (txItem.hl->effects() & (ScStyle_Subscript | ScStyle_Superscript))
						pts.translate(0, txItem.hl->glyph.yoffset);
					if ((txItem.style.effects() & ScStyle_Shadowed) && (txItem.style.strokeColor() != CommonStrings::None))
					{
						FPointArray ptsS = pts.copy();
						ptsS.translate(txItem.style.fontSize() * txItem.style.shadowXOffset() / 10000.0, -txItem.style.fontSize() * txItem.style.shadowYOffset() / 10000.0);
						ptsS.scale(conversionFactor, conversionFactor);
						QString paS = SetClipPath(&ptsS, true);
						QDomElement glyS = p_docu.createElement("Path");
						glyS.setAttribute("Data", paS);
						glyS.setAttribute("Fill", SetColor(txItem.style.strokeColor(), txItem.style.strokeShade(), 0));
						grp.appendChild(glyS);
					}
					pts.scale(conversionFactor, conversionFactor);
					QString pa = SetClipPath(&pts, true);
					QDomElement gly = p_docu.createElement("Path");
					gly.setAttribute("Data", pa);
					gly.setAttribute("Fill", SetColor(txItem.style.fillColor(), txItem.style.fillShade(), 0));
					if (txItem.hl->effects() & ScStyle_Outline)
					{
						gly.setAttribute("StrokeThickness", FToStr((chs * txItem.style.outlineWidth() / 10000.0) * conversionFactor));
						gly.setAttribute("Stroke", SetColor(txItem.style.strokeColor(), txItem.style.strokeShade(), 0));
					}
					grp.appendChild(gly);
				}
				if ((txItem.style.effects() & ScStyle_Underline) || ((txItem.style.effects() & ScStyle_UnderlineWords)  && chstr.toUInt() != txItem.style.font().char2CMap(QChar(' '))))
				{
					double st, lw;
					if ((txItem.style.underlineOffset() != -1) || (txItem.style.underlineWidth() != -1))
					{
						if (txItem.style.underlineOffset() != -1)
							st = (txItem.style.underlineOffset() / 1000.0) * (txItem.style.font().descent(txItem.style.fontSize() / 10.0));
						else
							st = txItem.style.font().underlinePos(txItem.style.fontSize() / 10.0);
						if (txItem.style.underlineWidth() != -1)
							lw = (txItem.style.underlineWidth() / 1000.0) * (txItem.style.fontSize() / 10.0);
						else
							lw = qMax(txItem.style.font().strokeWidth(txItem.style.fontSize() / 10.0), 1.0);
					}
					else
					{
						st = txItem.style.font().underlinePos(txItem.style.fontSize() / 10.0);
						lw = qMax(txItem.style.font().strokeWidth(txItem.style.fontSize() / 10.0), 1.0);
					}
					if (txItem.style.baselineOffset() != 0)
						st += (txItem.style.fontSize() / 10.0) * txItem.hl->glyph.scaleV * (txItem.style.baselineOffset() / 1000.0);
					QDomElement gly = p_docu.createElement("Path");
					if (txItem.style.effects() & ScStyle_Subscript)
						gly.setAttribute("Data", QString("M%1,%2 L%3,%4").arg((CurX + txItem.hl->glyph.xoffset) * conversionFactor).arg((ls.y + txItem.hl->glyph.yoffset - st) * conversionFactor).arg((CurX + txItem.hl->glyph.xoffset + txItem.hl->glyph.xadvance) * conversionFactor).arg((ls.y + txItem.hl->glyph.yoffset - st) * conversionFactor));
					else
						gly.setAttribute("Data", QString("M%1,%2 L%3,%4").arg((CurX + txItem.hl->glyph.xoffset) * conversionFactor).arg((ls.y - st) * conversionFactor).arg((CurX + txItem.hl->glyph.xoffset + txItem.hl->glyph.xadvance) * conversionFactor).arg((ls.y - st) * conversionFactor));
					gly.setAttribute("Stroke", SetColor(txItem.style.fillColor(), txItem.style.fillShade(), 0));
					gly.setAttribute("StrokeThickness", FToStr(lw * conversionFactor));
					grp.appendChild(gly);
				}
				if (txItem.style.effects() & ScStyle_Strikethrough)
				{
					double st, lw;
					if ((txItem.style.strikethruOffset() != -1) || (txItem.style.underlineWidth() != -1))
					{
						if (txItem.style.strikethruOffset() != -1)
							st = (txItem.style.strikethruOffset() / 1000.0) * (txItem.style.font().ascent(txItem.style.fontSize() / 10.0));
						else
							st = txItem.style.font().strikeoutPos(txItem.style.fontSize() / 10.0);
						if (txItem.style.strikethruWidth() != -1)
							lw = (txItem.style.strikethruWidth() / 1000.0) * (txItem.style.fontSize() / 10.0);
						else
							lw = qMax(txItem.style.font().strokeWidth(txItem.style.fontSize() / 10.0), 1.0);
					}
					else
					{
						st = txItem.style.font().strikeoutPos(txItem.style.fontSize() / 10.0);
						lw = qMax(txItem.style.font().strokeWidth(txItem.style.fontSize() / 10.0), 1.0);
					}
					if (txItem.style.baselineOffset() != 0)
						st += (txItem.style.fontSize() / 10.0) * txItem.hl->glyph.scaleV * (txItem.style.baselineOffset() / 1000.0);
					QDomElement gly = p_docu.createElement("Path");
					gly.setAttribute("Data", QString("M%1,%2 L%3,%4").arg((CurX + txItem.hl->glyph.xoffset) * conversionFactor).arg((ls.y - st) * conversionFactor).arg((CurX + txItem.hl->glyph.xoffset + txItem.hl->glyph.xadvance) * conversionFactor).arg((ls.y - st) * conversionFactor));
					gly.setAttribute("Stroke", SetColor(txItem.style.fillColor(), txItem.style.fillShade(), 0));
					gly.setAttribute("StrokeThickness", FToStr(lw * conversionFactor));
					grp.appendChild(gly);
				}
			}
		}
	}
	parentElem.appendChild(grp);
	if ((Item->GrTypeStroke != 0) || (Item->lineColor() != CommonStrings::None) || !Item->NamedLStyle.isEmpty())
	{
		if (Item->NamedLStyle.isEmpty())
		{
			if ((!Item->strokePattern().isEmpty()) && (Item->patternStrokePath))
			{
				processSymbolStroke(xOffset, yOffset, Item, parentElem, rel_root);
			}
			else
			{
				QDomElement ob3 = p_docu.createElement("Path");
				ob3.setAttribute("Data", pa);
				getStrokeStyle(Item, ob3, rel_root, xOffset, yOffset);
				ob3.setAttribute("RenderTransform", MatrixToStr(mpl));
				parentElem.appendChild(ob3);
			}
		}
		else
		{
			QDomElement grp2 = p_docu.createElement("Canvas");
			multiLine ml = m_Doc->MLineStyles[Item->NamedLStyle];
			for (int it = ml.size()-1; it > -1; it--)
			{
				if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
				{
					QDomElement ob3 = p_docu.createElement("Path");
					ob3.setAttribute("Data", pa);
					GetMultiStroke(&ml[it], ob3);
					grp2.appendChild(ob3);
				}
			}
			if (Item->lineTransparency() != 0)
				grp2.setAttribute("Opacity", FToStr(1.0 - Item->lineTransparency()));
			grp2.setAttribute("RenderTransform", MatrixToStr(mpl));
			parentElem.appendChild(grp2);
		}
	}
}

void XPSExPlug::processPathTextItem(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root)
{
	QDomElement grp = p_docu.createElement("Canvas");
	QTransform mpx;
	mpx.translate(xOffset * conversionFactor, yOffset * conversionFactor);
	if ((Item->rotation() != 0.0) || Item->imageFlippedH() || Item->imageFlippedV())
	{
		mpx.rotate(Item->rotation());
		if (Item->imageFlippedH())
		{
			mpx.translate(Item->width() * conversionFactor, 0);
			mpx.scale(-1, 1);
		}
		if (Item->imageFlippedV())
		{
			mpx.translate(0, Item->height() * conversionFactor);
			mpx.scale(1, -1);
		}
	}
	grp.setAttribute("RenderTransform", MatrixToStr(mpx));
	if ((Item->PoShow) && (Item->lineColor() != CommonStrings::None))
	{
		QDomElement ob = p_docu.createElement("Path");
		FPointArray path = Item->PoLine.copy();
		path.scale(conversionFactor, conversionFactor);
		QString pa = SetClipPath(&path, false);
		ob.setAttribute("Data", pa);
		if (Item->NamedLStyle.isEmpty())
		{
			if ((!Item->strokePattern().isEmpty()) && (Item->patternStrokePath))
			{
				processSymbolStroke(xOffset, yOffset, Item, parentElem, rel_root);
			}
			else
			{
				getStrokeStyle(Item, ob, rel_root, xOffset, yOffset);
				grp.appendChild(ob);
			}
		}
		else
		{
			QDomElement grp2 = p_docu.createElement("Canvas");
			multiLine ml = m_Doc->MLineStyles[Item->NamedLStyle];
			for (int it = ml.size()-1; it > -1; it--)
			{
				if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
				{
					QDomElement ob3 = p_docu.createElement("Path");
					ob3.setAttribute("Data", pa);
					GetMultiStroke(&ml[it], ob3);
					grp2.appendChild(ob3);
				}
			}
			if (Item->lineTransparency() != 0)
				grp2.setAttribute("Opacity", FToStr(1.0 - Item->lineTransparency()));
			grp.appendChild(grp2);
		}
	}
	QString chstr;
	for (int a = 0; a < Item->asPathText()->itemRenderText.length(); ++a)
	{
		ScText *hl = Item->asPathText()->itemRenderText.item_p(a);
		const CharStyle& charStyle(Item->asPathText()->itemRenderText.charStyle(a));
		chstr = Item->asPathText()->itemRenderText.text(a,1);
		if ((chstr == QChar(13)) || (chstr == QChar(29)))
			continue;
		if (chstr == QChar(30))
		{
			chstr = Item->ExpandToken(a);
			if (chstr == QChar(32))
				continue;
		}
		double chs = charStyle.fontSize();
		if (hl->effects() & ScStyle_SmallCaps)
		{
			if (chstr.toUpper() != chstr)
			{
				chs = qMax(static_cast<int>(hl->fontSize() * m_Doc->typographicPrefs().valueSmallCaps / 100), 1);
				chstr = chstr.toUpper();
			}
		}
		else if (hl->effects() & ScStyle_AllCaps)
			chstr = chstr.toUpper();
		uint chr = chstr[0].unicode();
		QPointF tangt = QPointF( cos(hl->PRot), sin(hl->PRot) );
		QTransform trafo = QTransform( 1, 0, 0, -1, -hl->PDx * conversionFactor, 0 );
		if (Item->textPathFlipped)
			trafo *= QTransform(1, 0, 0, -1, 0, 0);
		if (Item->textPathType == 0)
			trafo *= QTransform( tangt.x(), tangt.y(), tangt.y(), -tangt.x(), hl->PtransX * conversionFactor, hl->PtransY * conversionFactor );
		else if (Item->textPathType == 1)
			trafo *= QTransform(1, 0, 0, -1, hl->PtransX * conversionFactor, hl->PtransY * conversionFactor );
		else if (Item->textPathType == 2)
		{
			double a = 1;
			if (tangt.x() < 0)
				a = -1;
			if (fabs(tangt.x()) > 0.1)
				trafo *= QTransform( a, (tangt.y() / tangt.x()) * a, 0, -1, hl->PtransX * conversionFactor, hl->PtransY * conversionFactor ); // ID's Skew mode
			else
				trafo *= QTransform( a, 4 * a, 0, -1, hl->PtransX * conversionFactor, hl->PtransY * conversionFactor );
		}
		if (hl->baselineOffset() != 0)
			trafo.translate(0, (-charStyle.fontSize() / 10.0) * (charStyle.baselineOffset() / 1000.0));
		trafo.translate(0, Item->BaseOffs);
		QTransform finalMat = trafo;
		if (hl->ch == SpecialChars::OBJECT)
		{
			if (hl->getItem(m_Doc))
			{
				QDomElement obO = p_docu.createElement("Canvas");
				QTransform mm = finalMat;
				mm.translate(0, (-(hl->getItem(m_Doc)->height() * (charStyle.scaleV() / 1000.0))) * conversionFactor);
				if (charStyle.scaleH() != 1000)
					mm.scale(charStyle.scaleH() / 1000.0, 1);
				if (charStyle.scaleV() != 1000)
					mm.scale(1, charStyle.scaleV() / 1000.0);
				obO.setAttribute("RenderTransform", MatrixToStr(mm));
				QList<PageItem*> emG = hl->getGroupedItems(m_Doc);
				for (int em = 0; em < emG.count(); ++em)
				{
					PageItem* embed = emG.at(em);
					writeItemOnPage(embed->gXpos, embed->gYpos, embed, obO, rel_root);
				}
				grp.appendChild(obO);
			}
			continue;
		}
		finalMat.scale(conversionFactor, conversionFactor);
		FPointArray pts;
		QTransform chma;
		uint gl = charStyle.font().char2CMap(chr);
		pts = charStyle.font().glyphOutline(gl);
		if (pts.size() < 4)
			continue;
		chma = QTransform();
		chma.scale(hl->glyph.scaleH * charStyle.fontSize() / 100.00, hl->glyph.scaleV * charStyle.fontSize() / 100.0);
		pts.map(chma);
		if (hl->effects() & (ScStyle_Subscript | ScStyle_Superscript))
			pts.translate(0, -(chs / 10.0 * hl->glyph.scaleV));
		else
			pts.translate(0, -(chs / 10.0));
		if (hl->effects() & (ScStyle_Subscript | ScStyle_Superscript))
			pts.translate(0, hl->glyph.yoffset);
		if ((charStyle.effects() & ScStyle_Shadowed) && (charStyle.strokeColor() != CommonStrings::None))
		{
			FPointArray ptsS = pts.copy();
			ptsS.translate(charStyle.fontSize() * charStyle.shadowXOffset() / 10000.0, -charStyle.fontSize() * charStyle.shadowYOffset() / 10000.0);
			ptsS.map(finalMat);
			QString paS = SetClipPath(&ptsS, true);
			QDomElement glyS = p_docu.createElement("Path");
			glyS.setAttribute("Data", paS);
			glyS.setAttribute("Fill", SetColor(charStyle.strokeColor(), charStyle.strokeShade(), 0));
			grp.appendChild(glyS);
		}
		pts.map(finalMat);
		QString pa = SetClipPath(&pts, true);
		QDomElement gly = p_docu.createElement("Path");
		gly.setAttribute("Data", pa);
		gly.setAttribute("Fill", SetColor(charStyle.fillColor(), charStyle.fillShade(), 0));
		if (hl->effects() & ScStyle_Outline)
		{
			gly.setAttribute("StrokeThickness", FToStr((chs * charStyle.outlineWidth() / 10000.0) * conversionFactor));
			gly.setAttribute("Stroke", SetColor(charStyle.strokeColor(), charStyle.strokeShade(), 0));
		}
		grp.appendChild(gly);
		if ((charStyle.effects() & ScStyle_Underline) || ((charStyle.effects() & ScStyle_UnderlineWords)  && chstr.toUInt() != charStyle.font().char2CMap(QChar(' '))))
		{
			double st, lw;
			if ((charStyle.underlineOffset() != -1) || (charStyle.underlineWidth() != -1))
			{
				if (charStyle.underlineOffset() != -1)
					st = (charStyle.underlineOffset() / 1000.0) * (charStyle.font().descent(charStyle.fontSize() / 10.0));
				else
					st = charStyle.font().underlinePos(charStyle.fontSize() / 10.0);
				if (charStyle.underlineWidth() != -1)
					lw = (charStyle.underlineWidth() / 1000.0) * (charStyle.fontSize() / 10.0);
				else
					lw = qMax(charStyle.font().strokeWidth(charStyle.fontSize() / 10.0), 1.0);
			}
			else
			{
				st = charStyle.font().underlinePos(charStyle.fontSize() / 10.0);
				lw = qMax(charStyle.font().strokeWidth(charStyle.fontSize() / 10.0), 1.0);
			}
			if (charStyle.baselineOffset() != 0)
				st += (charStyle.fontSize() / 10.0) * hl->glyph.scaleV * (charStyle.baselineOffset() / 1000.0);
			FPointArray ptsS;
			ptsS.svgInit();
			if (charStyle.effects() & ScStyle_Subscript)
			{
				ptsS.svgMoveTo(hl->glyph.xoffset, hl->glyph.yoffset - st);
				ptsS.svgLineTo(hl->glyph.xoffset + hl->glyph.xadvance, hl->glyph.yoffset - st);
			}
			else
			{
				ptsS.svgMoveTo(hl->glyph.xoffset, -st);
				ptsS.svgLineTo(hl->glyph.xoffset + hl->glyph.xadvance, -st);
			}
			ptsS.map(finalMat);
			QString paS = SetClipPath(&ptsS, true);
			QDomElement gly = p_docu.createElement("Path");
			gly.setAttribute("Data", paS);
			gly.setAttribute("Stroke", SetColor(charStyle.fillColor(), charStyle.fillShade(), 0));
			gly.setAttribute("StrokeThickness", FToStr(lw * conversionFactor));
			grp.appendChild(gly);
		}
		if (charStyle.effects() & ScStyle_Strikethrough)
		{
			double st, lw;
			if ((charStyle.strikethruOffset() != -1) || (charStyle.underlineWidth() != -1))
			{
				if (charStyle.strikethruOffset() != -1)
					st = (charStyle.strikethruOffset() / 1000.0) * (charStyle.font().ascent(charStyle.fontSize() / 10.0));
				else
					st = charStyle.font().strikeoutPos(charStyle.fontSize() / 10.0);
				if (charStyle.strikethruWidth() != -1)
					lw = (charStyle.strikethruWidth() / 1000.0) * (charStyle.fontSize() / 10.0);
				else
					lw = qMax(charStyle.font().strokeWidth(charStyle.fontSize() / 10.0), 1.0);
			}
			else
			{
				st = charStyle.font().strikeoutPos(charStyle.fontSize() / 10.0);
				lw = qMax(charStyle.font().strokeWidth(charStyle.fontSize() / 10.0), 1.0);
			}
			if (charStyle.baselineOffset() != 0)
				st += (charStyle.fontSize() / 10.0) * hl->glyph.scaleV * (charStyle.baselineOffset() / 1000.0);
			FPointArray ptsS;
			ptsS.svgInit();
			ptsS.svgMoveTo(hl->glyph.xoffset, -st);
			ptsS.svgLineTo(hl->glyph.xoffset + hl->glyph.xadvance, -st);
			ptsS.map(finalMat);
			QString paS = SetClipPath(&ptsS, true);
			QDomElement gly = p_docu.createElement("Path");
			gly.setAttribute("Data", paS);
			gly.setAttribute("Stroke", SetColor(charStyle.fillColor(), charStyle.fillShade(), 0));
			gly.setAttribute("StrokeThickness", FToStr(lw * conversionFactor));
			grp.appendChild(gly);
		}
	}
	parentElem.appendChild(grp);
	return;
}

void XPSExPlug::processSymbolItem(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root)
{
	QDomElement ob = p_docu.createElement("Canvas");
	FPointArray path = Item->PoLine.copy();
	path.scale(conversionFactor, conversionFactor);
	QString pa = SetClipPath(&path, true);
	if (Item->fillRule)
		pa.prepend("F 0 ");
	else
		pa.prepend("F 1 ");
	ob.setAttribute("Clip", pa);
	ScPattern pat = m_Doc->docPatterns[Item->pattern()];
	QTransform mpx;
	mpx.translate(xOffset * conversionFactor, yOffset * conversionFactor);
	mpx.scale(Item->width() / pat.width, Item->height() / pat.height);
	if ((Item->rotation() != 0.0) || Item->imageFlippedH() || Item->imageFlippedV())
	{
		mpx.rotate(Item->rotation());
		if (Item->imageFlippedH())
		{
			mpx.translate(Item->width() * conversionFactor, 0);
			mpx.scale(-1, 1);
		}
		if (Item->imageFlippedV())
		{
			mpx.translate(0, Item->height() * conversionFactor);
			mpx.scale(1, -1);
		}
	}
	ob.setAttribute("RenderTransform", MatrixToStr(mpx));
	if (Item->GrMask > 0)
		handleMask(1, Item, ob, rel_root, xOffset, yOffset);
	else
	{
		if (Item->fillTransparency() != 0)
			ob.setAttribute("Opacity", FToStr(1.0 - Item->fillTransparency()));
	}
	for (int em = 0; em < pat.items.count(); ++em)
	{
		PageItem* embed = pat.items.at(em);
		writeItemOnPage(embed->gXpos, embed->gYpos, embed, ob, rel_root);
	}
	parentElem.appendChild(ob);
	return;
}

void XPSExPlug::processTableItem(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root)
{
	QDomElement ob = p_docu.createElement("Canvas");
	QTransform mpx;
	mpx.translate(xOffset * conversionFactor, yOffset * conversionFactor);
	if (Item->rotation() != 0.0)
		mpx.rotate(Item->rotation());
	mpx.translate(Item->asTable()->gridOffset().x() * conversionFactor, Item->asTable()->gridOffset().y() * conversionFactor);
	ob.setAttribute("RenderTransform", MatrixToStr(mpx));
	// Paint table fill.
	if (Item->asTable()->fillColor() != CommonStrings::None)
	{
		int lastCol = Item->asTable()->columns() - 1;
		int lastRow = Item->asTable()->rows() - 1;
		double x = Item->asTable()->columnPosition(0);
		double y = Item->asTable()->rowPosition(0);
		double width = Item->asTable()->columnPosition(lastCol) + Item->asTable()->columnWidth(lastCol) - x;
		double height = Item->asTable()->rowPosition(lastRow) + Item->asTable()->rowHeight(lastRow) - y;
		FPointArray path;
		path.svgInit();
		path.svgMoveTo(0, 0);
		path.svgLineTo(width, 0);
		path.svgLineTo(width, height);
		path.svgLineTo(0, height);
		path.svgClosePath();
		path.scale(conversionFactor, conversionFactor);
		QString pa = SetClipPath(&path, true);
		QDomElement cl = p_docu.createElement("Path");
		cl.setAttribute("Data", pa);
		cl.setAttribute("Fill", SetColor(Item->asTable()->fillColor(), Item->asTable()->fillShade(), 0));
		ob.appendChild(cl);
	}
	// Pass 1: Paint cell fills.
	for (int row = 0; row < Item->asTable()->rows(); ++row)
	{
		int colSpan = 0;
		for (int col = 0; col < Item->asTable()->columns(); col += colSpan)
		{
			TableCell cell = Item->asTable()->cellAt(row, col);
			if (row == cell.row())
			{
				QString colorName = cell.fillColor();
				if (colorName != CommonStrings::None)
				{
					int row = cell.row();
					int col = cell.column();
					int lastRow = row + cell.rowSpan() - 1;
					int lastCol = col + cell.columnSpan() - 1;
					double x = Item->asTable()->columnPosition(col);
					double y = Item->asTable()->rowPosition(row);
					double width = Item->asTable()->columnPosition(lastCol) + Item->asTable()->columnWidth(lastCol) - x;
					double height = Item->asTable()->rowPosition(lastRow) + Item->asTable()->rowHeight(lastRow) - y;
					FPointArray path;
					path.svgInit();
					path.svgMoveTo(x, y);
					path.svgLineTo(x + width, y);
					path.svgLineTo(x + width, y + height);
					path.svgLineTo(x, y + height);
					path.svgClosePath();
					path.scale(conversionFactor, conversionFactor);
					QString pa = SetClipPath(&path, true);
					QDomElement cl = p_docu.createElement("Path");
					cl.setAttribute("Data", pa);
					cl.setAttribute("Fill", SetColor(colorName, cell.fillShade(), 0));
					ob.appendChild(cl);
				}
			}
			colSpan = cell.columnSpan();
		}
	}
	// Pass 2: Paint vertical borders.
	for (int row = 0; row < Item->asTable()->rows(); ++row)
	{
		int colSpan = 0;
		for (int col = 0; col < Item->asTable()->columns(); col += colSpan)
		{
			TableCell cell = Item->asTable()->cellAt(row, col);
			if (row == cell.row())
			{
				const int lastRow = cell.row() + cell.rowSpan() - 1;
				const int lastCol = cell.column() + cell.columnSpan() - 1;
				const double borderX = Item->asTable()->columnPosition(lastCol) + Item->asTable()->columnWidth(lastCol);
				QPointF start(borderX, 0.0);
				QPointF end(borderX, 0.0);
				QPointF startOffsetFactors, endOffsetFactors;
				int startRow, endRow;
				for (int row = cell.row(); row <= lastRow; row += endRow - startRow + 1)
				{
					TableCell rightCell = Item->asTable()->cellAt(row, lastCol + 1);
					startRow = qMax(cell.row(), rightCell.row());
					endRow = qMin(lastRow, rightCell.isValid() ? rightCell.row() + rightCell.rowSpan() - 1 : lastRow);
					TableCell topLeftCell = Item->asTable()->cellAt(startRow - 1, lastCol);
					TableCell topRightCell = Item->asTable()->cellAt(startRow - 1, lastCol + 1);
					TableCell bottomRightCell = Item->asTable()->cellAt(endRow + 1, lastCol + 1);
					TableCell bottomLeftCell = Item->asTable()->cellAt(endRow + 1, lastCol);
					TableBorder topLeft, top, topRight, border, bottomLeft, bottom, bottomRight;
					resolveBordersVertical(topLeftCell, topRightCell, cell, rightCell, bottomLeftCell, bottomRightCell,
										   &topLeft, &top, &topRight, &border, &bottomLeft, &bottom, &bottomRight, Item->asTable());
					if (border.isNull())
						continue; // Quit early if the border to paint is null.
					start.setY(Item->asTable()->rowPosition(startRow));
					end.setY((Item->asTable()->rowPosition(endRow) + Item->asTable()->rowHeight(endRow)));
					joinVertical(border, topLeft, top, topRight, bottomLeft, bottom, bottomRight, &start, &end, &startOffsetFactors, &endOffsetFactors);
					paintBorder(border, start, end, startOffsetFactors, endOffsetFactors, ob);
				}
				if (col == 0)
				{
					const int lastRow = cell.row() + cell.rowSpan() - 1;
					const int firstCol = cell.column();
					const double borderX = Item->asTable()->columnPosition(firstCol);
					QPointF start(borderX, 0.0);
					QPointF end(borderX, 0.0);
					QPointF startOffsetFactors, endOffsetFactors;
					int startRow, endRow;
					for (int row = cell.row(); row <= lastRow; row += endRow - startRow + 1)
					{
						TableCell leftCell = Item->asTable()->cellAt(row, firstCol - 1);
						startRow = qMax(cell.row(), leftCell.row());
						endRow = qMin(lastRow, leftCell.isValid() ? leftCell.row() + leftCell.rowSpan() - 1 : lastRow);
						TableCell topLeftCell = Item->asTable()->cellAt(startRow - 1, firstCol - 1);
						TableCell topRightCell = Item->asTable()->cellAt(startRow - 1, firstCol);
						TableCell bottomRightCell = Item->asTable()->cellAt(lastRow + 1, firstCol);
						TableCell bottomLeftCell = Item->asTable()->cellAt(lastRow + 1, firstCol - 1);
						TableBorder topLeft, top, topRight, border, bottomLeft, bottom, bottomRight;
						resolveBordersVertical(topLeftCell, topRightCell, leftCell, cell, bottomLeftCell, bottomRightCell,
											   &topLeft, &top, &topRight, &border, &bottomLeft, &bottom, &bottomRight, Item->asTable());
						if (border.isNull())
							continue; // Quit early if the border to paint is null.
						start.setY(Item->asTable()->rowPosition(startRow));
						end.setY((Item->asTable()->rowPosition(endRow) + Item->asTable()->rowHeight(endRow)));
						joinVertical(border, topLeft, top, topRight, bottomLeft, bottom, bottomRight, &start, &end, &startOffsetFactors, &endOffsetFactors);
						paintBorder(border, start, end, startOffsetFactors, endOffsetFactors, ob);
					}
				}
			}
			colSpan = cell.columnSpan();
		}
	}
	// Pass 3: Paint horizontal borders.
	for (int row = 0; row < Item->asTable()->rows(); ++row)
	{
		int colSpan = 0;
		for (int col = 0; col < Item->asTable()->columns(); col += colSpan)
		{
			TableCell cell = Item->asTable()->cellAt(row, col);
			if (row == cell.row())
			{
				const int lastRow = cell.row() + cell.rowSpan() - 1;
				const int lastCol = cell.column() + cell.columnSpan() - 1;
				const double borderY = (Item->asTable()->rowPosition(lastRow) + Item->asTable()->rowHeight(lastRow));
				QPointF start(0.0, borderY);
				QPointF end(0.0, borderY);
				QPointF startOffsetFactors, endOffsetFactors;
				int startCol, endCol;
				for (int col = cell.column(); col <= lastCol; col += endCol - startCol + 1)
				{
					TableCell bottomCell = Item->asTable()->cellAt(lastRow + 1, col);
					startCol = qMax(cell.column(), bottomCell.column());
					endCol = qMin(lastCol, bottomCell.isValid() ? bottomCell.column() + bottomCell.columnSpan() - 1 : lastCol);
					TableCell topLeftCell = Item->asTable()->cellAt(lastRow, startCol - 1);
					TableCell topRightCell = Item->asTable()->cellAt(lastRow, endCol + 1);
					TableCell bottomRightCell = Item->asTable()->cellAt(lastRow + 1, endCol + 1);
					TableCell bottomLeftCell = Item->asTable()->cellAt(lastRow + 1, startCol - 1);
					TableBorder topLeft, left, bottomLeft, border, topRight, right, bottomRight;
					resolveBordersHorizontal(topLeftCell, cell, topRightCell, bottomLeftCell, bottomCell,
											 bottomRightCell, &topLeft, &left, &bottomLeft, &border, &topRight, &right, &bottomRight, Item->asTable());
					if (border.isNull())
						continue; // Quit early if the border is null.
					start.setX(Item->asTable()->columnPosition(startCol));
					end.setX(Item->asTable()->columnPosition(endCol) + Item->asTable()->columnWidth(endCol));
					joinHorizontal(border, topLeft, left, bottomLeft, topRight, right, bottomRight, &start, &end, &startOffsetFactors, &endOffsetFactors);
					paintBorder(border, start, end, startOffsetFactors, endOffsetFactors, ob);
				}
				if (row == 0)
				{
					const int firstRow = cell.row();
					const int lastCol = cell.column() + cell.columnSpan() - 1;
					const double borderY = Item->asTable()->rowPosition(firstRow);
					QPointF start(0.0, borderY);
					QPointF end(0.0, borderY);
					QPointF startOffsetFactors, endOffsetFactors;
					int startCol, endCol;
					for (int col = cell.column(); col <= lastCol; col += endCol - startCol + 1)
					{
						TableCell topCell = Item->asTable()->cellAt(firstRow - 1, col);
						startCol = qMax(cell.column(), topCell.column());
						endCol = qMin(lastCol, topCell.isValid() ? topCell.column() + topCell.columnSpan() - 1 : lastCol);
						TableCell topLeftCell = Item->asTable()->cellAt(firstRow - 1, startCol - 1);
						TableCell topRightCell = Item->asTable()->cellAt(firstRow - 1, endCol + 1);
						TableCell bottomRightCell = Item->asTable()->cellAt(firstRow, endCol + 1);
						TableCell bottomLeftCell = Item->asTable()->cellAt(firstRow, startCol - 1);
						TableBorder topLeft, left, bottomLeft, border, topRight, right, bottomRight;
						resolveBordersHorizontal(topLeftCell, topCell, topRightCell, bottomLeftCell, cell,
												 bottomRightCell, &topLeft, &left, &bottomLeft, &border, &topRight, &right, &bottomRight, Item->asTable());
						if (border.isNull())
							continue; // Quit early if the border is null.
						start.setX(Item->asTable()->columnPosition(startCol));
						end.setX(Item->asTable()->columnPosition(endCol) + Item->asTable()->columnWidth(endCol));
						joinHorizontal(border, topLeft, left, bottomLeft, topRight, right, bottomRight, &start, &end, &startOffsetFactors, &endOffsetFactors);
						paintBorder(border, start, end, startOffsetFactors, endOffsetFactors, ob);
					}
				}
			}
			colSpan = cell.columnSpan();
		}
	}
	// Pass 4: Paint cell content.
	for (int row = 0; row < Item->asTable()->rows(); ++row)
	{
		for (int col = 0; col < Item->asTable()->columns(); col ++)
		{
			TableCell cell = Item->asTable()->cellAt(row, col);
			if (cell.row() == row && cell.column() == col)
			{
				PageItem* textFrame = cell.textFrame();
				processTextItem(cell.contentRect().x(), cell.contentRect().y(), textFrame, ob, rel_root);
			}
		}
	}
	if (Item->GrMask > 0)
		handleMask(1, Item, ob, rel_root, xOffset, yOffset);
	else
	{
		if (Item->fillTransparency() != 0)
			ob.setAttribute("Opacity", FToStr(1.0 - Item->fillTransparency()));
	}
	parentElem.appendChild(ob);
}

void XPSExPlug::paintBorder(const TableBorder& border, const QPointF& start, const QPointF& end, const QPointF& startOffsetFactors, const QPointF& endOffsetFactors, QDomElement &ob)
{
	QPointF lineStart, lineEnd;
	foreach (const TableBorderLine& line, border.borderLines())
	{
		lineStart.setX(start.x() + line.width() * startOffsetFactors.x());
		lineStart.setY(start.y() + line.width() * startOffsetFactors.y());
		lineEnd.setX(end.x() + line.width() * endOffsetFactors.x());
		lineEnd.setY(end.y() + line.width() * endOffsetFactors.y());
		QDomElement cl = p_docu.createElement("Path");
		cl.setAttribute("Data", "M"+FToStr(lineStart.x() * conversionFactor)+","+FToStr(lineStart.y() * conversionFactor)+" L"+FToStr(lineEnd.x() * conversionFactor)+" "+FToStr(lineEnd.y() * conversionFactor));
		QString dashVals = "";
		if (line.style() != Qt::SolidLine)
			dashVals = getDashString(line.style(), qMax(line.width(), 1.0));
		if (!dashVals.isEmpty())
			cl.setAttribute("StrokeDashArray", dashVals);
		if (line.color() != CommonStrings::None)
			cl.setAttribute("Stroke", SetColor(line.color(), line.shade(), 0));
		if (line.width() != 0.0)
			cl.setAttribute("StrokeThickness", FToStr(line.width() * conversionFactor));
		else
			cl.setAttribute("StrokeThickness", FToStr(1.0 * conversionFactor));
		ob.appendChild(cl);
	}
}

void XPSExPlug::processSymbolStroke(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root)
{
	QDomElement ob = p_docu.createElement("Canvas");
	QTransform mpx;
	mpx.translate(xOffset * conversionFactor, yOffset * conversionFactor);
	ob.setAttribute("RenderTransform", MatrixToStr(mpx));
	QPainterPath path = Item->PoLine.toQPainterPath(false);
	ScPattern pat = m_Doc->docPatterns[Item->strokePattern()];
	double pLen = path.length() - ((pat.width / 2.0) * (Item->patternStrokeScaleX / 100.0));
	double adv = pat.width * Item->patternStrokeScaleX / 100.0 * Item->patternStrokeSpace;
	double xpos = Item->patternStrokeOffsetX * Item->patternStrokeScaleX / 100.0;
	while (xpos < pLen)
	{
		double currPerc = path.percentAtLength(xpos);
		double currAngle = path.angleAtPercent(currPerc);
		if (currAngle <= 180.0)
			currAngle *= -1.0;
		else
			currAngle = 360.0 - currAngle;
		QPointF currPoint = path.pointAtPercent(currPerc);
		QTransform trans;
		trans.translate(currPoint.x() * conversionFactor, currPoint.y() * conversionFactor);
		trans.rotate(currAngle);
		trans.translate(0.0, Item->patternStrokeOffsetY);
		trans.rotate(-Item->patternStrokeRotation);
		trans.shear(Item->patternStrokeSkewX, -Item->patternStrokeSkewY);
		trans.scale(Item->patternStrokeScaleX / 100.0, Item->patternStrokeScaleY / 100.0);
		trans.translate(-pat.width / 2.0, -pat.height / 2.0);
		if (Item->patternStrokeMirrorX)
		{
			trans.translate(pat.width, 0);
			trans.scale(-1, 1);
		}
		if (Item->patternStrokeMirrorY)
		{
			trans.translate(0, pat.height);
			trans.scale(1, -1);
		}
		QDomElement obS = p_docu.createElement("Canvas");
		obS.setAttribute("RenderTransform", MatrixToStr(trans));
		for (int em = 0; em < pat.items.count(); ++em)
		{
			PageItem* embed = pat.items.at(em);
			writeItemOnPage(embed->gXpos, embed->gYpos, embed, obS, rel_root);
		}
		ob.appendChild(obS);
		xpos += adv;
	}
	parentElem.appendChild(ob);
	return;
}

void XPSExPlug::processArrows(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root)
{
	if (Item->startArrowIndex() != 0)
	{
		QTransform arrowTrans;
		FPointArray arrow = m_Doc->arrowStyles().at(Item->startArrowIndex()-1).points.copy();
		if (Item->itemType() == PageItem::Line)
		{
			arrowTrans.translate(0, 0);
			arrowTrans.scale(Item->startArrowScale() / 100.0, Item->startArrowScale() / 100.0);
			if (Item->NamedLStyle.isEmpty())
			{
				if (Item->lineWidth() != 0.0)
					arrowTrans.scale(Item->lineWidth(), Item->lineWidth());
			}
			else
			{
				multiLine ml = m_Doc->MLineStyles[Item->NamedLStyle];
				if (ml[ml.size()-1].Width != 0.0)
					arrowTrans.scale(ml[ml.size()-1].Width, ml[ml.size()-1].Width);
			}
			arrowTrans.scale(-1,1);
		}
		else
		{
			FPoint Start = Item->PoLine.point(0);
			for (int xx = 1; xx < Item->PoLine.size(); xx += 2)
			{
				FPoint Vector = Item->PoLine.point(xx);
				if ((Start.x() != Vector.x()) || (Start.y() != Vector.y()))
				{
					double r = atan2(Start.y()-Vector.y(),Start.x()-Vector.x())*(180.0/M_PI);
					arrowTrans.translate(Start.x(), Start.y());
					arrowTrans.rotate(r);
					arrowTrans.scale(Item->startArrowScale() / 100.0, Item->startArrowScale() / 100.0);
					if (Item->NamedLStyle.isEmpty())
					{
						if (Item->lineWidth() != 0.0)
							arrowTrans.scale(Item->lineWidth(), Item->lineWidth());
					}
					else
					{
						multiLine ml = m_Doc->MLineStyles[Item->NamedLStyle];
						if (ml[ml.size()-1].Width != 0.0)
							arrowTrans.scale(ml[ml.size()-1].Width, ml[ml.size()-1].Width);
					}
					break;
				}
			}
		}
		arrow.map(arrowTrans);
		drawArrow(xOffset, yOffset, Item, parentElem, rel_root, arrow);
	}
	if (Item->endArrowIndex() != 0)
	{
		QTransform arrowTrans;
		FPointArray arrow = m_Doc->arrowStyles().at(Item->endArrowIndex()-1).points.copy();
		if (Item->itemType() == PageItem::Line)
		{
			arrowTrans.translate(Item->width(), 0);
			arrowTrans.scale(Item->endArrowScale() / 100.0, Item->endArrowScale() / 100.0);
			if (Item->NamedLStyle.isEmpty())
			{
				if (Item->lineWidth() != 0.0)
					arrowTrans.scale(Item->lineWidth(), Item->lineWidth());
			}
			else
			{
				multiLine ml = m_Doc->MLineStyles[Item->NamedLStyle];
				if (ml[ml.size()-1].Width != 0.0)
					arrowTrans.scale(ml[ml.size()-1].Width, ml[ml.size()-1].Width);
			}
		}
		else
		{
			FPoint End = Item->PoLine.point(Item->PoLine.size()-2);
			for (uint xx = Item->PoLine.size()-1; xx > 0; xx -= 2)
			{
				FPoint Vector = Item->PoLine.point(xx);
				if ((End.x() != Vector.x()) || (End.y() != Vector.y()))
				{
					double r = atan2(End.y()-Vector.y(),End.x()-Vector.x())*(180.0/M_PI);
					arrowTrans.translate(End.x(), End.y());
					arrowTrans.rotate(r);
					arrowTrans.scale(Item->endArrowScale() / 100.0, Item->endArrowScale() / 100.0);
					if (Item->NamedLStyle.isEmpty())
					{
						if (Item->lineWidth() != 0.0)
							arrowTrans.scale(Item->lineWidth(), Item->lineWidth());
					}
					else
					{
						multiLine ml = m_Doc->MLineStyles[Item->NamedLStyle];
						if (ml[ml.size()-1].Width != 0.0)
							arrowTrans.scale(ml[ml.size()-1].Width, ml[ml.size()-1].Width);
					}
					break;
				}
			}
		}
		arrow.map(arrowTrans);
		drawArrow(xOffset, yOffset, Item, parentElem, rel_root, arrow);
	}
	return;
}

void XPSExPlug::drawArrow(double xOffset, double yOffset, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root, FPointArray &arrow)
{
	QTransform mpx;
	if (Item->rotation() != 0.0)
	{
		mpx.translate(xOffset * conversionFactor, yOffset * conversionFactor);
		mpx.rotate(Item->rotation());
		mpx.translate(-xOffset * conversionFactor, -yOffset * conversionFactor);
	}
	arrow.translate(xOffset, yOffset);
	arrow.scale(conversionFactor, conversionFactor);
	QString pa = SetClipPath(&arrow, true);
	if (Item->NamedLStyle.isEmpty())
	{
		QDomElement ob = p_docu.createElement("Path");
		ob.setAttribute("Data", pa);
		ob.setAttribute("RenderTransform", MatrixToStr(mpx));
		getStrokeStyle(Item, ob, rel_root, xOffset, yOffset, true);
		parentElem.appendChild(ob);
	}
	else
	{
		QDomElement grp2 = p_docu.createElement("Canvas");
		grp2.setAttribute("RenderTransform", MatrixToStr(mpx));
		multiLine ml = m_Doc->MLineStyles[Item->NamedLStyle];
		if (ml[0].Color != CommonStrings::None)
		{
			QDomElement ob3 = p_docu.createElement("Path");
			ob3.setAttribute("Data", pa);
			ob3.setAttribute("Fill", SetColor(ml[0].Color, ml[0].Shade, 0));
			GetMultiStroke(&ml[0], ob3);
			grp2.appendChild(ob3);
		}
		for (int it = ml.size()-1; it > 0; it--)
		{
			if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
			{
				QDomElement ob3 = p_docu.createElement("Path");
				ob3.setAttribute("Data", pa);
				GetMultiStroke(&ml[it], ob3);
				grp2.appendChild(ob3);
			}
		}
		parentElem.appendChild(grp2);
	}
}

QString XPSExPlug::embedFont(const ScFace *font, QDomElement &rel_root)
{
	QByteArray fontData;
	loadRawText(font->fontFilePath(), fontData);
	QUuid id = QUuid::createUuid();
	QString guidString = id.toString();
	guidString = guidString.toUpper();
	guidString.remove("{");
	guidString.remove("}");
	unsigned short guid[16];
	const static int indexes[] = {6, 4, 2, 0, 11, 9, 16, 14, 19, 21, 24, 26, 28, 30, 32, 34};
	for (int i = 0; i < 16; i++)
	{
		int hex1 = hex2int(guidString[indexes[i]].cell());
		int hex2 = hex2int(guidString[indexes[i]+1].cell());
		guid[i] = hex1 * 16 + hex2;
	}
	// Obfuscation - xor bytes in font binary with bytes from guid (font's filename)
	const static int mapping[] = {15, 14, 13, 12, 11, 10, 9, 8, 6, 7, 4, 5, 0, 1, 2, 3};
	for (int i = 0; i < 16; i++)
	{
		fontData[i] = fontData[i] ^ guid[mapping[i]];
		fontData[i+16] = fontData[i+16] ^ guid[mapping[i]];
	}
	QFile ft(baseDir + "/Resources/Fonts/" + guidString + ".odttf");
	if (ft.open(QIODevice::WriteOnly))
	{
		ft.write(fontData);
		ft.close();
	}
	QDomElement rel = r_docu.createElement("Relationship");
	rel.setAttribute("Id", QString("rIDf%1").arg(fontCounter));
	rel.setAttribute("Type", "http://schemas.microsoft.com/xps/2005/06/required-resource");
	rel.setAttribute("Target", "/Resources/Fonts/" + guidString + ".odttf");
	rel_root.appendChild(rel);
	fontCounter++;
	return "/Resources/Fonts/" + guidString + ".odttf";
}

void XPSExPlug::GetMultiStroke(struct SingleLine *sl, QDomElement &parentElem)
{
	parentElem.setAttribute("StrokeThickness", FToStr(sl->Width * conversionFactor));
	switch (static_cast<Qt::PenCapStyle>(sl->LineEnd))
	{
		case Qt::FlatCap:
			parentElem.setAttribute("StrokeDashCap", "Flat");
			parentElem.setAttribute("StrokeEndLineCap", "Flat");
			parentElem.setAttribute("StrokeStartLineCap", "Flat");
			break;
		case Qt::SquareCap:
			parentElem.setAttribute("StrokeDashCap", "Square");
			parentElem.setAttribute("StrokeEndLineCap", "Square");
			parentElem.setAttribute("StrokeStartLineCap", "Square");
			break;
		case Qt::RoundCap:
			parentElem.setAttribute("StrokeDashCap", "Round");
			parentElem.setAttribute("StrokeEndLineCap", "Round");
			parentElem.setAttribute("StrokeStartLineCap", "Round");
			break;
		default:
			parentElem.setAttribute("StrokeDashCap", "Flat");
			parentElem.setAttribute("StrokeEndLineCap", "Flat");
			parentElem.setAttribute("StrokeStartLineCap", "Flat");
			break;
	}
	switch (static_cast<Qt::PenJoinStyle>(sl->LineJoin))
	{
		case Qt::MiterJoin:
			parentElem.setAttribute("StrokeLineJoin", "Miter");
			break;
		case Qt::BevelJoin:
			parentElem.setAttribute("StrokeLineJoin", "Bevel");
			break;
		case Qt::RoundJoin:
			parentElem.setAttribute("StrokeLineJoin", "Round");
			break;
		default:
			parentElem.setAttribute("StrokeLineJoin", "Miter");
			break;
	}
	QString dashVals = "";
	if (static_cast<Qt::PenStyle>(sl->Dash) != Qt::SolidLine)
		dashVals = getDashString(sl->Dash, 1);
	if (!dashVals.isEmpty())
		parentElem.setAttribute("StrokeDashArray", dashVals);
	if (sl->Color != CommonStrings::None)
		parentElem.setAttribute("Stroke", SetColor(sl->Color, sl->Shade, 0));
}

void XPSExPlug::getStrokeStyle(PageItem *Item, QDomElement &parentElem, QDomElement &rel_root, double xOffset, double yOffset, bool forArrow)
{
	parentElem.setAttribute("StrokeThickness", FToStr(Item->lineWidth() * conversionFactor));
	switch (Item->PLineEnd)
	{
		case Qt::FlatCap:
			parentElem.setAttribute("StrokeDashCap", "Flat");
			parentElem.setAttribute("StrokeEndLineCap", "Flat");
			parentElem.setAttribute("StrokeStartLineCap", "Flat");
			break;
		case Qt::SquareCap:
			parentElem.setAttribute("StrokeDashCap", "Square");
			parentElem.setAttribute("StrokeEndLineCap", "Square");
			parentElem.setAttribute("StrokeStartLineCap", "Square");
			break;
		case Qt::RoundCap:
			parentElem.setAttribute("StrokeDashCap", "Round");
			parentElem.setAttribute("StrokeEndLineCap", "Round");
			parentElem.setAttribute("StrokeStartLineCap", "Round");
			break;
		default:
			parentElem.setAttribute("StrokeDashCap", "Flat");
			parentElem.setAttribute("StrokeEndLineCap", "Flat");
			parentElem.setAttribute("StrokeStartLineCap", "Flat");
			break;
	}
	switch (Item->PLineJoin)
	{
		case Qt::MiterJoin:
			parentElem.setAttribute("StrokeLineJoin", "Miter");
			break;
		case Qt::BevelJoin:
			parentElem.setAttribute("StrokeLineJoin", "Bevel");
			break;
		case Qt::RoundJoin:
			parentElem.setAttribute("StrokeLineJoin", "Round");
			break;
		default:
			parentElem.setAttribute("StrokeLineJoin", "Miter");
			break;
	}
	QString dashVals = "";
	if (Item->DashValues.count() != 0)
	{
		QVector<double>::iterator it;
		for ( it = Item->DashValues.begin(); it != Item->DashValues.end(); ++it )
		{
			dashVals += FToStr((*it) / Item->lineWidth())+" ";
		}
	}
	else
	{
		if (Item->PLineArt != Qt::SolidLine)
			dashVals = getDashString(Item->PLineArt, 1);
	}
	if (!dashVals.isEmpty())
	{
		parentElem.setAttribute("StrokeDashArray", dashVals);
		if (Item->DashValues.count() != 0)
			parentElem.setAttribute("StrokeDashOffset", FToStr(Item->DashOffset));
	}
	if (Item->GrTypeStroke == 0)
	{
		if (Item->lineColor() != CommonStrings::None)
		{
			if (forArrow)
				parentElem.setAttribute("Fill", SetColor(Item->lineColor(), Item->lineShade(), Item->lineTransparency()));
			else
				parentElem.setAttribute("Stroke", SetColor(Item->lineColor(), Item->lineShade(), Item->lineTransparency()));
			return;
		}
	}
	else
	{
		if ((!Item->strokePattern().isEmpty()) && (!Item->patternStrokePath))
		{
			ScPattern pa = m_Doc->docPatterns[Item->strokePattern()];
			QDomElement ob;
			if (forArrow)
				ob = p_docu.createElement("Path.Fill");
			else
				ob = p_docu.createElement("Path.Stroke");
			QDomElement gr = p_docu.createElement("VisualBrush");
			gr.setAttribute("TileMode", "Tile");
			gr.setAttribute("ViewboxUnits", "Absolute");
			gr.setAttribute("ViewportUnits", "Absolute");
			gr.setAttribute("Viewbox", QString("0, 0, %1, %2").arg(pa.width * conversionFactor).arg(pa.height * conversionFactor));
			double patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY, patternSpace;
			Item->strokePatternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY, patternSpace);
			patternScaleX /= 100.0;
			patternScaleY /= 100.0;
			double lw2 = Item->lineWidth() / 2.0;
			gr.setAttribute("Viewport", QString("%1, %2, %3, %4").arg((xOffset + patternOffsetX - lw2) * conversionFactor).arg((yOffset + patternOffsetY - lw2) * conversionFactor).arg((pa.width * patternScaleX) * conversionFactor).arg((pa.height * patternScaleY) * conversionFactor));
			bool mirrorX, mirrorY;
			Item->strokePatternFlip(mirrorX, mirrorY);
			if ((patternRotation != 0) || (patternSkewX != 0) || (patternSkewY != 0) || mirrorX || mirrorY)
			{
				QTransform mpa;
				mpa.rotate(patternRotation);
				mpa.shear(-patternSkewX, patternSkewY);
				mpa.scale(pa.scaleX, pa.scaleY);
				if (mirrorX)
					mpa.scale(-1, 1);
				if (mirrorY)
					mpa.scale(1, -1);
				gr.setAttribute("Transform", MatrixToStr(mpa));
			}
			if (Item->lineTransparency() != 0)
				gr.setAttribute("Opacity", FToStr(1.0 - Item->lineTransparency()));
			QDomElement grp = p_docu.createElement("VisualBrush.Visual");
			for (int em = 0; em < pa.items.count(); ++em)
			{
				PageItem* embed = pa.items.at(em);
				writeItemOnPage(embed->gXpos, embed->gYpos, embed, grp, rel_root);
			}
			gr.appendChild(grp);
			ob.appendChild(gr);
			parentElem.appendChild(ob);
		}
		else if ((Item->GrTypeStroke == 6) || (Item->GrTypeStroke == 7))
		{
			QDomElement ob;
			if (forArrow)
				ob = p_docu.createElement("Path.Fill");
			else
				ob = p_docu.createElement("Path.Stroke");
			QDomElement gr;
			double GrStartX = (Item->GrStrokeStartX + xOffset) * conversionFactor;
			double GrStartY = (Item->GrStrokeStartY + yOffset) * conversionFactor;
			double GrFocalX = (Item->GrStrokeFocalX + xOffset) * conversionFactor;
			double GrFocalY = (Item->GrStrokeFocalY + yOffset) * conversionFactor;
			double GrEndX = (Item->GrStrokeEndX + xOffset) * conversionFactor;
			double GrEndY = (Item->GrStrokeEndY + yOffset) * conversionFactor;
			if (Item->GrTypeStroke == 6)
			{
				gr = p_docu.createElement("LinearGradientBrush");
				gr.setAttribute("MappingMode", "Absolute");
				gr.setAttribute("StartPoint", FToStr(GrStartX) + ", " + FToStr(GrStartY));
				gr.setAttribute("EndPoint", FToStr(GrEndX) + ", " + FToStr(GrEndY));
			}
			else
			{
				gr = p_docu.createElement("RadialGradientBrush");
				double rad = sqrt(pow(GrEndX - GrStartX, 2) + pow(GrEndY - GrStartY,2));
				gr.setAttribute("MappingMode", "Absolute");
				gr.setAttribute("RadiusX", FToStr(rad));
				gr.setAttribute("RadiusY", FToStr(rad));
				gr.setAttribute("Center", FToStr(GrStartX) + ", " + FToStr(GrStartY));
				gr.setAttribute("GradientOrigin", FToStr(GrFocalX) + ", " + FToStr(GrFocalY));
			}
			double gradientSkew;
			if (Item->GrStrokeSkew == 90)
				gradientSkew = 1;
			else if (Item->GrStrokeSkew == 180)
				gradientSkew = 0;
			else if (Item->GrStrokeSkew == 270)
				gradientSkew = -1;
			else if (Item->GrStrokeSkew == 390)
				gradientSkew = 0;
			else
				gradientSkew = tan(M_PI / 180.0 * Item->GrStrokeSkew);
			QTransform qmatrix;
			if (Item->GrTypeStroke == 6)
			{
				qmatrix.translate(GrStartX, GrStartY);
				qmatrix.shear(-gradientSkew, 0);
				qmatrix.translate(-GrStartX, -GrStartY);
			}
			else
			{
				double rotEnd = xy2Deg(GrEndX - GrStartX, GrEndY - GrStartY);
				qmatrix.translate(GrStartX, GrStartY);
				qmatrix.rotate(rotEnd);
				qmatrix.shear(gradientSkew, 0);
				qmatrix.translate(0, GrStartY * (1.0 - Item->GrStrokeScale));
				qmatrix.translate(-GrStartX, -GrStartY);
				qmatrix.scale(1, Item->GrStrokeScale);
			}
			gr.setAttribute("Transform", MatrixToStr(qmatrix));
			if (Item->lineTransparency() != 0)
				gr.setAttribute("Opacity", FToStr(1.0 - Item->lineTransparency()));
			QDomElement grs;
			if (Item->GrTypeStroke == 6)
				grs = p_docu.createElement("LinearGradientBrush.GradientStops");
			else
				grs = p_docu.createElement("RadialGradientBrush.GradientStops");
			bool   isFirst = true;
			double actualStop = 0.0, lastStop = 0.0;
			QList<VColorStop*> cstops = Item->stroke_gradient.colorStops();
			for (uint cst = 0; cst < Item->stroke_gradient.Stops(); ++cst)
			{
				actualStop = cstops.at(cst)->rampPoint;
				if ((actualStop != lastStop) || (isFirst))
				{
					QDomElement itcl = p_docu.createElement("GradientStop");
					itcl.setAttribute("Offset", FToStr(cstops.at(cst)->rampPoint));
					itcl.setAttribute("Color", SetColor(cstops.at(cst)->name, cstops.at(cst)->shade, 1.0 - cstops.at(cst)->opacity));
					grs.appendChild(itcl);
					lastStop = actualStop;
					isFirst  = false;
				}
			}
			gr.appendChild(grs);
			ob.appendChild(gr);
			parentElem.appendChild(ob);
		}
	}
}

void XPSExPlug::getFillStyle(PageItem *Item, QDomElement &parentElem, QDomElement &rel_root, double xOffset, double yOffset, bool withTransparency)
{
	if (Item->GrType == 0)
	{
		if (Item->fillColor() != CommonStrings::None)
		{
			if ((withTransparency) || (Item->GrMask == 0))
				parentElem.setAttribute("Fill", SetColor(Item->fillColor(), Item->fillShade(), Item->fillTransparency()));
			else
				parentElem.setAttribute("Fill", SetColor(Item->fillColor(), Item->fillShade(), 0));
		}
		return;
	}
	else
	{
		if ((Item->GrType == 6) || (Item->GrType == 7))
		{
			QDomElement ob = p_docu.createElement("Path.Fill");
			QDomElement gr;
			double GrStartX = (Item->GrStartX + xOffset) * conversionFactor;
			double GrStartY = (Item->GrStartY + yOffset) * conversionFactor;
			double GrFocalX = (Item->GrFocalX + xOffset) * conversionFactor;
			double GrFocalY = (Item->GrFocalY + yOffset) * conversionFactor;
			double GrEndX = (Item->GrEndX + xOffset) * conversionFactor;
			double GrEndY = (Item->GrEndY + yOffset) * conversionFactor;
			if (Item->GrType == 6)
			{
				gr = p_docu.createElement("LinearGradientBrush");
				gr.setAttribute("MappingMode", "Absolute");
				gr.setAttribute("StartPoint", FToStr(GrStartX) + ", " + FToStr(GrStartY));
				gr.setAttribute("EndPoint", FToStr(GrEndX) + ", " + FToStr(GrEndY));
			}
			else
			{
				gr = p_docu.createElement("RadialGradientBrush");
				double rad = sqrt(pow(GrEndX - GrStartX, 2) + pow(GrEndY - GrStartY,2));
				gr.setAttribute("MappingMode", "Absolute");
				gr.setAttribute("RadiusX", FToStr(rad));
				gr.setAttribute("RadiusY", FToStr(rad));
				gr.setAttribute("Center", FToStr(GrStartX) + ", " + FToStr(GrStartY));
				gr.setAttribute("GradientOrigin", FToStr(GrFocalX) + ", " + FToStr(GrFocalY));
			}
			double gradientSkew;
			if (Item->GrSkew == 90)
				gradientSkew = 1;
			else if (Item->GrSkew == 180)
				gradientSkew = 0;
			else if (Item->GrSkew == 270)
				gradientSkew = -1;
			else if (Item->GrSkew == 390)
				gradientSkew = 0;
			else
				gradientSkew = tan(M_PI / 180.0 * Item->GrSkew);
			QTransform qmatrix;
			if (Item->GrType == 6)
			{
				qmatrix.translate(GrStartX, GrStartY);
				qmatrix.shear(-gradientSkew, 0);
				qmatrix.translate(-GrStartX, -GrStartY);
			}
			else
			{
				double rotEnd = xy2Deg(GrEndX - GrStartX, GrEndY - GrStartY);
				qmatrix.translate(GrStartX, GrStartY);
				qmatrix.rotate(rotEnd);
				qmatrix.shear(gradientSkew, 0);
				qmatrix.translate(0, GrStartY * (1.0 - Item->GrScale));
				qmatrix.translate(-GrStartX, -GrStartY);
				qmatrix.scale(1, Item->GrScale);
			}
			gr.setAttribute("Transform", MatrixToStr(qmatrix));
			if ((Item->fillTransparency() != 0) && ((withTransparency) || (Item->GrMask == 0)))
				gr.setAttribute("Opacity", FToStr(1.0 - Item->fillTransparency()));
			QDomElement grs;
			if (Item->GrType == 6)
				grs = p_docu.createElement("LinearGradientBrush.GradientStops");
			else
				grs = p_docu.createElement("RadialGradientBrush.GradientStops");
			bool   isFirst = true;
			double actualStop = 0.0, lastStop = 0.0;
			QList<VColorStop*> cstops = Item->fill_gradient.colorStops();
			for (uint cst = 0; cst < Item->fill_gradient.Stops(); ++cst)
			{
				actualStop = cstops.at(cst)->rampPoint;
				if ((actualStop != lastStop) || (isFirst))
				{
					QDomElement itcl = p_docu.createElement("GradientStop");
					itcl.setAttribute("Offset", FToStr(cstops.at(cst)->rampPoint));
					itcl.setAttribute("Color", SetColor(cstops.at(cst)->name, cstops.at(cst)->shade, 1.0 - cstops.at(cst)->opacity));
					grs.appendChild(itcl);
					lastStop = actualStop;
					isFirst  = false;
				}
			}
			gr.appendChild(grs);
			ob.appendChild(gr);
			parentElem.appendChild(ob);
		}
		else if (Item->GrType == 8)
		{
			ScPattern pa = m_Doc->docPatterns[Item->pattern()];
			QDomElement ob = p_docu.createElement("Path.Fill");
			QDomElement gr = p_docu.createElement("VisualBrush");
			gr.setAttribute("TileMode", "Tile");
			gr.setAttribute("ViewboxUnits", "Absolute");
			gr.setAttribute("ViewportUnits", "Absolute");
			gr.setAttribute("Viewbox", QString("0, 0, %1, %2").arg(pa.width * conversionFactor).arg(pa.height * conversionFactor));
			double patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY;
			Item->patternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY);
			patternScaleX /= 100.0;
			patternScaleY /= 100.0;
			gr.setAttribute("Viewport", QString("%1, %2, %3, %4").arg((xOffset + patternOffsetX) * conversionFactor).arg((yOffset + patternOffsetY) * conversionFactor).arg((pa.width * patternScaleX) * conversionFactor).arg((pa.height * patternScaleY) * conversionFactor));
			bool mirrorX, mirrorY;
			Item->patternFlip(mirrorX, mirrorY);
			if ((patternRotation != 0) || (patternSkewX != 0) || (patternSkewY != 0) || mirrorX || mirrorY)
			{
				QTransform mpa;
				mpa.rotate(patternRotation);
				mpa.shear(-patternSkewX, patternSkewY);
				mpa.scale(pa.scaleX, pa.scaleY);
				if (mirrorX)
					mpa.scale(-1, 1);
				if (mirrorY)
					mpa.scale(1, -1);
				gr.setAttribute("Transform", MatrixToStr(mpa));
			}
			if ((Item->fillTransparency() != 0) && ((withTransparency) || (Item->GrMask == 0)))
				gr.setAttribute("Opacity", FToStr(1.0 - Item->fillTransparency()));
			QDomElement grp = p_docu.createElement("VisualBrush.Visual");
			for (int em = 0; em < pa.items.count(); ++em)
			{
				PageItem* embed = pa.items.at(em);
				writeItemOnPage(embed->gXpos, embed->gYpos, embed, grp, rel_root);
			}
			gr.appendChild(grp);
			ob.appendChild(gr);
			parentElem.appendChild(ob);
		}
	}
}

void XPSExPlug::handleMask(int type, PageItem *Item, QDomElement &parentElem, QDomElement &rel_root, double xOffset, double yOffset)
{
	QDomElement ob;
	if (type == 1)
		ob = p_docu.createElement("Canvas.OpacityMask");
	else if (type == 2)
		ob = p_docu.createElement("Glyph.OpacityMask");
	else
		ob = p_docu.createElement("Path.OpacityMask");
	if ((Item->GrMask == 1) || (Item->GrMask == 2))
	{
		QDomElement gr;
		double GrStartX = (Item->GrMaskStartX + xOffset) * conversionFactor;
		double GrStartY = (Item->GrMaskStartY + yOffset) * conversionFactor;
		double GrFocalX = (Item->GrMaskFocalX + xOffset) * conversionFactor;
		double GrFocalY = (Item->GrMaskFocalY + yOffset) * conversionFactor;
		double GrEndX = (Item->GrMaskEndX + xOffset) * conversionFactor;
		double GrEndY = (Item->GrMaskEndY + yOffset) * conversionFactor;
		if ((Item->GrMask == 1) || (Item->GrMask == 4))
		{
			gr = p_docu.createElement("LinearGradientBrush");
			gr.setAttribute("MappingMode", "Absolute");
			gr.setAttribute("StartPoint", FToStr(GrStartX) + ", " + FToStr(GrStartY));
			gr.setAttribute("EndPoint", FToStr(GrEndX) + ", " + FToStr(GrEndY));
		}
		else
		{
			gr = p_docu.createElement("RadialGradientBrush");
			double rad = sqrt(pow(GrEndX - GrStartX, 2) + pow(GrEndY - GrStartY,2));
			gr.setAttribute("MappingMode", "Absolute");
			gr.setAttribute("RadiusX", FToStr(rad));
			gr.setAttribute("RadiusY", FToStr(rad));
			gr.setAttribute("Center", FToStr(GrStartX) + ", " + FToStr(GrStartY));
			gr.setAttribute("GradientOrigin", FToStr(GrFocalX) + ", " + FToStr(GrFocalY));
		}
		double gradientSkew;
		if (Item->GrMaskSkew == 90)
			gradientSkew = 1;
			else if (Item->GrMaskSkew == 180)
			gradientSkew = 0;
		else if (Item->GrMaskSkew == 270)
			gradientSkew = -1;
		else if (Item->GrMaskSkew == 390)
			gradientSkew = 0;
		else
			gradientSkew = tan(M_PI / 180.0 * Item->GrMaskSkew);
		QTransform qmatrix;
		if (Item->GrMask == 1)
		{
			qmatrix.translate(GrStartX, GrStartY);
			qmatrix.shear(-gradientSkew, 0);
			qmatrix.translate(-GrStartX, -GrStartY);
		}
		else
		{
			double rotEnd = xy2Deg(GrEndX - GrStartX, GrEndY - GrStartY);
			qmatrix.translate(GrStartX, GrStartY);
			qmatrix.rotate(rotEnd);
			qmatrix.shear(gradientSkew, 0);
			qmatrix.translate(0, GrStartY * (1.0 - Item->GrMaskScale));
			qmatrix.translate(-GrStartX, -GrStartY);
			qmatrix.scale(1, Item->GrMaskScale);
		}
		gr.setAttribute("Transform", MatrixToStr(qmatrix));
		QDomElement grs;
		if (Item->GrMask == 1)
			grs = p_docu.createElement("LinearGradientBrush.GradientStops");
		else
			grs = p_docu.createElement("RadialGradientBrush.GradientStops");
		bool   isFirst = true;
		double actualStop = 0.0, lastStop = 0.0;
		QList<VColorStop*> cstops = Item->mask_gradient.colorStops();
		for (uint cst = 0; cst < Item->mask_gradient.Stops(); ++cst)
		{
			actualStop = cstops.at(cst)->rampPoint;
			if ((actualStop != lastStop) || (isFirst))
			{
				QDomElement itcl = p_docu.createElement("GradientStop");
				itcl.setAttribute("Offset", FToStr(cstops.at(cst)->rampPoint));
				itcl.setAttribute("Color", SetColor(cstops.at(cst)->name, cstops.at(cst)->shade, 1.0 - cstops.at(cst)->opacity));
				grs.appendChild(itcl);
				lastStop = actualStop;
				isFirst  = false;
			}
		}
		gr.appendChild(grs);
		ob.appendChild(gr);
	}
	else if (Item->GrMask == 3)
	{
		ScPattern pa = m_Doc->docPatterns[Item->patternMask()];
		QDomElement gr = p_docu.createElement("VisualBrush");
		gr.setAttribute("TileMode", "Tile");
		gr.setAttribute("ViewboxUnits", "Absolute");
		gr.setAttribute("ViewportUnits", "Absolute");
		gr.setAttribute("Viewbox", QString("0, 0, %1, %2").arg(pa.width * conversionFactor).arg(pa.height * conversionFactor));
		double patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY;
		Item->maskTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY);
		patternScaleX /= 100.0;
		patternScaleY /= 100.0;
		gr.setAttribute("Viewport", QString("%1, %2, %3, %4").arg((xOffset + patternOffsetX) * conversionFactor).arg((yOffset + patternOffsetY) * conversionFactor).arg((pa.width * patternScaleX) * conversionFactor).arg((pa.height * patternScaleY) * conversionFactor));
		bool mirrorX, mirrorY;
		Item->maskFlip(mirrorX, mirrorY);
		if ((patternRotation != 0) || (patternSkewX != 0) || (patternSkewY != 0) || mirrorX || mirrorY)
		{
			QTransform mpa;
			mpa.rotate(patternRotation);
			mpa.shear(-patternSkewX, patternSkewY);
			mpa.scale(pa.scaleX, pa.scaleY);
			if (mirrorX)
				mpa.scale(-1, 1);
			if (mirrorY)
				mpa.scale(1, -1);
			gr.setAttribute("Transform", MatrixToStr(mpa));
		}
		QDomElement grp = p_docu.createElement("VisualBrush.Visual");
		for (int em = 0; em < pa.items.count(); ++em)
		{
			PageItem* embed = pa.items.at(em);
			writeItemOnPage(embed->gXpos, embed->gYpos, embed, grp, rel_root);
		}
		gr.appendChild(grp);
		ob.appendChild(gr);
	}
	parentElem.appendChild(ob);
}

QString XPSExPlug::SetColor(QString farbe, int shad, double transparency)
{
	if (farbe == CommonStrings::None)
		return "#00FFFFFF";
	const ScColor& col = m_Doc->PageColors[farbe];
	QString color =  ScColorEngine::getShadeColorProof(col, m_Doc, shad).name().mid(1);
	color = color.toUpper();
	QString alpha = "";
	alpha.setNum(qRound((1.0 - transparency) * 255), 16);
	alpha = alpha.toUpper();
	if (alpha.length() == 1)
		alpha.prepend("0");
	return "#" + alpha + color;
}

QString XPSExPlug::SetClipPath(FPointArray *ite, bool closed)
{
	QString tmp = "";
	FPoint np, np1, np2, np3, np4, firstP;
	bool nPath = true;
	bool first = true;
	if (ite->size() > 3)
	{
		for (int poi=0; poi<ite->size()-3; poi += 4)
		{
			if (ite->point(poi).x() > 900000)
			{
				nPath = true;
				continue;
			}
			if (nPath)
			{
				np = ite->point(poi);
				if ((!first) && (closed) && (np4 == firstP))
					tmp += "Z ";
				tmp += QString("M%1,%2 ").arg(np.x()).arg(np.y());
				nPath = false;
				first = false;
				firstP = np;
				np4 = np;
			}
			np = ite->point(poi);
			np1 = ite->point(poi+1);
			np2 = ite->point(poi+3);
			np3 = ite->point(poi+2);
			if ((np == np1) && (np2 == np3))
				tmp += QString("L%1,%2 ").arg(np3.x()).arg(np3.y());
			else
				tmp += QString("C%1,%2 %3,%4 %5,%6 ").arg(np1.x()).arg(np1.y()).arg(np2.x()).arg(np2.y()).arg(np3.x()).arg(np3.y());
			np4 = np3;
		}
	}
	if (closed)
		tmp += "Z";
	return tmp;
}

void XPSExPlug::writeDocRels()
{
	// Create and write required "Documents/1/_rels/FixedDoc.fdoc.rels" file
	QDomDocument doc("rels");
	QString st = "<Relationships></Relationships>";
	doc.setContent(st);
	QDomElement root  = doc.documentElement();
	root.setAttribute("xmlns", "http://schemas.openxmlformats.org/package/2006/relationships");
	doc.appendChild(root);
	QFile ft(baseDir + "/Documents/1/_rels/FixedDoc.fdoc.rels");
	if (ft.open(QIODevice::WriteOnly))
	{
		QString vo = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
		QDataStream s(&ft);
		vo += doc.toString();
		QByteArray utf8wr = vo.toUtf8();
		s.writeRawData(utf8wr.data(), utf8wr.length());
		ft.close();
	}
}

void XPSExPlug::writeCore()
{
	// Create and write required "docProps/core.xml" file
	QDomDocument doc("rels");
	QString st = "<cp:coreProperties></cp:coreProperties>";
	doc.setContent(st);
	QDomElement root  = doc.documentElement();
	root.setAttribute("xmlns:cp", "http://schemas.openxmlformats.org/package/2006/metadata/core-properties");
	root.setAttribute("xmlns:dc", "http://purl.org/dc/elements/1.1/");
	root.setAttribute("xmlns:dcterms", "http://purl.org/dc/terms/");
	root.setAttribute("xmlns:dcmitype", "http://purl.org/dc/dcmitype/");
	root.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	QDomElement rel1 = doc.createElement("dc:creator");
	rel1.setNodeValue("");
	root.appendChild(rel1);
	QDomElement rel2 = doc.createElement("dcterms:created");
	rel2.setAttribute("xsi:type", "dcterms:W3CDTF");
	rel2.setNodeValue("");
	root.appendChild(rel2);
	QDomElement rel3 = doc.createElement("dcterms:modified");
	rel3.setNodeValue("");
	rel3.setAttribute("xsi:type", "dcterms:W3CDTF");
	root.appendChild(rel3);
	doc.appendChild(root);
	QFile ft(baseDir + "/docProps/core.xml");
	if (ft.open(QIODevice::WriteOnly))
	{
		QString vo = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
		QDataStream s(&ft);
		vo += doc.toString();
		QByteArray utf8wr = vo.toUtf8();
		s.writeRawData(utf8wr.data(), utf8wr.length());
		ft.close();
	}
}

void XPSExPlug::writeContentType()
{
	// Create and write required "[Content_Type].xml" file
	QDomDocument doc("Content_Type");
	QString st = "<Types></Types>";
	doc.setContent(st);
	QDomElement root  = doc.documentElement();
	root.setAttribute("xmlns", "http://schemas.openxmlformats.org/package/2006/content-types");
	QDomElement rel1 = doc.createElement("Default");
	rel1.setAttribute("Extension", "png");
	rel1.setAttribute("ContentType", "image/png");
	root.appendChild(rel1);
	QDomElement rel2 = doc.createElement("Default");
	rel2.setAttribute("Extension", "jpeg");
	rel2.setAttribute("ContentType", "image/jpeg");
	root.appendChild(rel2);
	QDomElement rel3 = doc.createElement("Default");
	rel3.setAttribute("Extension", "jpg");
	rel3.setAttribute("ContentType", "image/jpeg");
	root.appendChild(rel3);
	QDomElement rel4 = doc.createElement("Default");
	rel4.setAttribute("Extension", "rels");
	rel4.setAttribute("ContentType", "application/vnd.openxmlformats-package.relationships+xml");
	root.appendChild(rel4);
	QDomElement rel5 = doc.createElement("Default");
	rel5.setAttribute("Extension", "xml");
	rel5.setAttribute("ContentType", "application/xml");
	root.appendChild(rel5);
	QDomElement rel6 = doc.createElement("Default");
	rel6.setAttribute("Extension", "fdseq");
	rel6.setAttribute("ContentType", "application/vnd.ms-package.xps-fixeddocumentsequence+xml");
	root.appendChild(rel6);
	QDomElement rel7 = doc.createElement("Default");
	rel7.setAttribute("Extension", "fpage");
	rel7.setAttribute("ContentType", "application/vnd.ms-package.xps-fixedpage+xml");
	root.appendChild(rel7);
	QDomElement rel8 = doc.createElement("Default");
	rel8.setAttribute("Extension", "struct");
	rel8.setAttribute("ContentType", "application/vnd.ms-package.xps-documentstructure+xml");
	root.appendChild(rel8);
	QDomElement rel9 = doc.createElement("Default");
	rel9.setAttribute("Extension", "fdoc");
	rel9.setAttribute("ContentType", "application/vnd.ms-package.xps-fixeddocument+xml");
	root.appendChild(rel9);
	QDomElement rel10 = doc.createElement("Default");
	rel10.setAttribute("Extension", "odttf");
	rel10.setAttribute("ContentType", "application/vnd.ms-package.obfuscated-opentype");
	root.appendChild(rel10);
	QDomElement rel11 = doc.createElement("Default");
	rel11.setAttribute("Extension", "dict");
	rel11.setAttribute("ContentType", "application/vnd.ms-package.xps-resourcedictionary+xml");
	root.appendChild(rel11);
	QDomElement rel12 = doc.createElement("Override");
	rel12.setAttribute("PartName", "/docProps/core.xml");
	rel12.setAttribute("ContentType", "application/vnd.openxmlformats-package.core-properties+xml");
	root.appendChild(rel12);
	doc.appendChild(root);
	QFile ft(baseDir + "/[Content_Types].xml");
	if (ft.open(QIODevice::WriteOnly))
	{
		QString vo = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
		QDataStream s(&ft);
		vo += doc.toString();
		QByteArray utf8wr = vo.toUtf8();
		s.writeRawData(utf8wr.data(), utf8wr.length());
		ft.close();
	}
}

void XPSExPlug::writeBaseRel()
{
	// Create and write required "_rels/.rels" file
	QDomDocument doc("rels");
	QString st = "<Relationships></Relationships>";
	doc.setContent(st);
	QDomElement root  = doc.documentElement();
	root.setAttribute("xmlns", "http://schemas.openxmlformats.org/package/2006/relationships");
	QDomElement rel1 = doc.createElement("Relationship");
	rel1.setAttribute("Id", "rID1");
	rel1.setAttribute("Type", "http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties");
	rel1.setAttribute("Target", "docProps/core.xml");
	root.appendChild(rel1);
	QDomElement rel2 = doc.createElement("Relationship");
	rel2.setAttribute("Id", "rID2");
	rel2.setAttribute("Type", "http://schemas.openxmlformats.org/package/2006/relationships/metadata/thumbnail");
	rel2.setAttribute("Target", "docProps/thumbnail.jpeg");
	root.appendChild(rel2);
	QDomElement rel3 = doc.createElement("Relationship");
	rel3.setAttribute("Id", "rID3");
	rel3.setAttribute("Type", "http://schemas.microsoft.com/xps/2005/06/fixedrepresentation");
	rel3.setAttribute("Target", "FixedDocSeq.fdseq");
	root.appendChild(rel3);
	doc.appendChild(root);
	QFile ft(baseDir + "/_rels/.rels");
	if (ft.open(QIODevice::WriteOnly))
	{
		QString vo = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
		QDataStream s(&ft);
		vo += doc.toString();
		QByteArray utf8wr = vo.toUtf8();
		s.writeRawData(utf8wr.data(), utf8wr.length());
		ft.close();
	}
}

QString XPSExPlug::FToStr(double c)
{
	QString cc;
	return cc.setNum(c);
}

QString XPSExPlug::IToStr(int c)
{
	QString cc;
	return cc.setNum(c);
}

QString XPSExPlug::MatrixToStr(QTransform &mat)
{
	QString cc("%1, %2, %3, %4, %5, %6");
	return  cc.arg(mat.m11()).arg(mat.m12()).arg(mat.m21()).arg(mat.m22()).arg(mat.dx()).arg(mat.dy());
}

int XPSExPlug::hex2int(char hex)
{
	QChar hexchar = QLatin1Char(hex);
	int v;
	if (hexchar.isDigit())
		v = hexchar.digitValue();
	else if (hexchar >= QLatin1Char('A') && hexchar <= QLatin1Char('F'))
		v = hexchar.cell() - 'A' + 10;
	else if (hexchar >= QLatin1Char('a') && hexchar <= QLatin1Char('f'))
		v = hexchar.cell() - 'a' + 10;
	else
		v = -1;
	return v;
}

bool XPSExPlug::checkForFallback(PageItem *Item)
{
	bool ret = false;
	int GrType = Item->GrType;
	int GrMask = Item->GrMask;
	if ((GrType == 9) || (GrType == 10) || (GrType == 11) || (GrType == 12) || (GrType == 13))
		ret = true;
	if ((GrMask == 4) || (GrMask == 5) || (GrMask == 6) || (GrMask == 7) || (GrMask == 8))
		ret = true;
	if (Item->fillBlendmode() != 0)
		ret = true;
	if (Item->lineBlendmode() != 0)
		ret = true;
	return ret;
}

XPSExPlug::~XPSExPlug()
{
}
