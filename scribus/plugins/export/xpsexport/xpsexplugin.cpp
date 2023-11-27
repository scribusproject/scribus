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
#include "pageitem_table.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "scpage.h"
#include "scpattern.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "sctextstruct.h"
#include "tableutils.h"
#include "text/textlayoutpainter.h"
#include "util.h"
#include "ui/customfdialog.h"
#include "ui/guidemanager.h"
#include "ui/scmessagebox.h"
#include "sccolorengine.h"
#include "util_formats.h"
#include "util_math.h"
#include "third_party/zip/scribus_zip.h"
#include "text/boxes.h"

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
	XPSExportPlugin* plug = qobject_cast<XPSExportPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

using namespace TableUtils;

XPSExportPlugin::XPSExportPlugin()
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
	m_actionInfo.exportPlugin = true;
	m_actionInfo.needsNumObjects = -1;
}

QString XPSExportPlugin::fullTrName() const
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

bool XPSExportPlugin::run(ScribusDoc* doc, const QString& filename)
{
	Q_ASSERT(filename.isEmpty());
	QString fileName;
	if (doc != nullptr)
	{
		PrefsContext* prefs = PrefsManager::instance().prefsFile->getPluginContext("xpsex");
		QString wdir = prefs->get("wdir", ".");
		QScopedPointer<CustomFDialog> openDia( new CustomFDialog(doc->scMW(), wdir, QObject::tr("Save as"), QObject::tr("Microsoft XPS (*.xps *.XPS);;All Files (*)"), fdHidePreviewCheckBox) );

		QFrame *Layout = new QFrame(openDia.data());
		QHBoxLayout *Layout1 = new QHBoxLayout(Layout);
		Layout1->setSpacing(6);
		Layout1->setContentsMargins(0, 0, 0, 0);
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
			QFileInfo fi(doc->documentFileName());
			QString completeBaseName = fi.completeBaseName();
			if (completeBaseName.endsWith(".xps", Qt::CaseInsensitive))
				completeBaseName.chop(4);
			wdir = QDir::fromNativeSeparators( fi.path() );
			fna  = QDir::fromNativeSeparators( fi.path() + "/" + completeBaseName + ".xps" );
		}
		else
		{
			wdir = QDir::fromNativeSeparators( wdir );
			if (wdir.right(1) != "/")
				fna = wdir + "/";
			else
				fna = wdir;
			fna += doc->documentFileName() + ".xps";
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
			int exit = ScMessageBox::warning(doc->scMW(), CommonStrings::trWarning,
				QObject::tr("Do you really want to overwrite the file:\n%1 ?").arg(fileName),
				QMessageBox::Yes | QMessageBox::No,
				QMessageBox::NoButton,	// GUI default
				QMessageBox::Yes);	// batch default
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

bool XPSExPlug::doExport(const QString& fName)
{
	ScZipHandler zip(true);
	if (!zip.open(fName))
		return false;

	QTemporaryDir dir;
	if (!dir.isValid())
	{
		zip.close();
		QFile::remove(fName);
		return false;
	}

	imageCounter = 0;
	fontCounter = 0;
	xps_fontMap.clear();
	baseDir = dir.path();
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
	QImage thumb = m_Doc->view()->PageToPixmap(0, 256, Pixmap_DrawBackground);
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
	
	bool written = zip.write(baseDir);
	zip.close();
	if (!written)
		QFile::remove(fName);
	return written;
}

void XPSExPlug::writePages(QDomElement &root)
{
	for (int i = 0; i < m_Doc->Pages->count(); ++i)
	{
		ScPage* Page = m_Doc->Pages->at(i);

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
		xps_fontRel.clear();

		writePage(droot, rroot, Page);

		QFile ft(baseDir + QString("/Documents/1/Pages/%1.fpage").arg(i + 1));
		if (ft.open(QIODevice::WriteOnly))
		{
			QString vo = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
			QDataStream s(&ft);
			vo += p_docu.toString();
			QByteArray utf8wr = vo.toUtf8();
			s.writeRawData(utf8wr.data(), utf8wr.length());
			ft.close();
		}

		QFile ftr(baseDir + QString("/Documents/1/Pages/_rels/%1.fpage.rels").arg(i + 1));
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
		rel1.setAttribute("Source", QString("Pages/%1.fpage").arg(i + 1));
		root.appendChild(rel1);
		p_docu.clear();
		r_docu.clear();
	}
}

void XPSExPlug::writePage(QDomElement &doc_root, QDomElement &rel_root, ScPage *Page)
{
	ScLayer ll;
	ll.isPrintable = false;
	for (int i = 0; i < m_Doc->Layers.count(); i++)
	{
		m_Doc->Layers.levelToLayer(ll, i);
		if (ll.isPrintable)
		{
			ScPage *mpage = m_Doc->MasterPages.at(m_Doc->MasterNames[Page->masterPageName()]);
			writePageLayer(doc_root, rel_root, mpage, ll);
			writePageLayer(doc_root, rel_root, Page, ll);
		}
	}
}

void XPSExPlug::writePageLayer(QDomElement &doc_root, QDomElement &rel_root, ScPage *page, ScLayer& layer)
{
	PageItem *item;
	QList<PageItem*> items;
	ScPage* SavedAct = m_Doc->currentPage();
	if (page->pageNameEmpty())
		items = m_Doc->DocItems;
	else
		items = m_Doc->MasterItems;
	if (items.count() == 0)
		return;
	if (!layer.isPrintable)
		return;
	m_Doc->setCurrentPage(page);
	QDomElement layerGroup = p_docu.createElement("Canvas");
	if (layer.transparency != 1.0)
		layerGroup.setAttribute("Opacity", layer.transparency);
	for (int j = 0; j < items.count(); ++j)
	{
		item = items.at(j);
		if (item->m_layerID != layer.ID)
			continue;
		if (!item->printEnabled())
			continue;
		double x = page->xOffset();
		double y = page->yOffset();
		double w = page->width();
		double h = page->height();
		double lw = item->visualLineWidth();
		double x2 = item->BoundingX - lw / 2.0;
		double y2 = item->BoundingY - lw / 2.0;
		double w2 = item->BoundingW + lw;
		double h2 = item->BoundingH + lw;
		if (!QRectF(x2, y2, w2, h2).intersects(QRectF(x, y, w, h)))
			continue;
		if ((!page->pageNameEmpty()) && (item->OwnPage != static_cast<int>(page->pageNr())) && (item->OwnPage != -1))
			continue;
		writeItemOnPage(item->xPos() - page->xOffset(), item->yPos() - page->yOffset(), item, layerGroup, rel_root);
	}
	doc_root.appendChild(layerGroup);
	m_Doc->setCurrentPage(SavedAct);
}

void XPSExPlug::writeItemOnPage(double xOffset, double yOffset, PageItem *item, QDomElement &parentElem, QDomElement &rel_root)
{
	switch (item->itemType())
	{
		case PageItem::Arc:
		case PageItem::Polygon:
		case PageItem::PolyLine:
		case PageItem::RegularPolygon:
		case PageItem::Spiral:
			if (checkForFallback(item))
				handleImageFallBack(item, parentElem, rel_root);
			else
			{
				processPolyItem(xOffset, yOffset, item, parentElem, rel_root);
				if ((item->lineColor() != CommonStrings::None) && ((item->startArrowIndex() != 0) || (item->endArrowIndex() != 0)))
					processArrows(xOffset, yOffset, item, parentElem, rel_root);
			}
			break;
		case PageItem::Line:
			if (checkForFallback(item))
				handleImageFallBack(item, parentElem, rel_root);
			else
			{
				processLineItem(xOffset, yOffset, item, parentElem, rel_root);
				if ((item->lineColor() != CommonStrings::None) && ((item->startArrowIndex() != 0) || (item->endArrowIndex() != 0)))
					processArrows(xOffset, yOffset, item, parentElem, rel_root);
			}
			break;
		case PageItem::ImageFrame:
		case PageItem::LatexFrame:
			if (checkForFallback(item))
				handleImageFallBack(item, parentElem, rel_root);
			else
				processImageItem(xOffset, yOffset, item, parentElem, rel_root);
			break;
		case PageItem::PathText:
		case PageItem::TextFrame:
			if (checkForFallback(item))
				handleImageFallBack(item, parentElem, rel_root);
			else
				processTextItem(xOffset, yOffset, item, parentElem, rel_root);
			break;
		case PageItem::Table:
			if (checkForFallback(item))
				handleImageFallBack(item, parentElem, rel_root);
			else
				processTableItem(xOffset, yOffset, item, parentElem, rel_root);
			break;
		case PageItem::Symbol:
			if (checkForFallback(item))
				handleImageFallBack(item, parentElem, rel_root);
			else
				processSymbolItem(xOffset, yOffset, item, parentElem, rel_root);
			break;
		case PageItem::Group:
			if (item->groupItemList.count() > 0)
			{
				if (checkForFallback(item))
					handleImageFallBack(item, parentElem, rel_root);
				else
				{
					QDomElement ob = p_docu.createElement("Canvas");
					if (item->GrMask > 0)
						handleMask(1, item, ob, rel_root, xOffset, yOffset);
					else
					{
						if (item->fillTransparency() != 0)
							ob.setAttribute("Opacity", FToStr(1.0 - item->fillTransparency()));
					}
					if (item->groupClipping())
					{
						FPointArray path = item->PoLine.copy();
						path.scale(conversionFactor, conversionFactor);
						path.scale(item->groupWidth / item->width(), item->groupHeight / item->height());
						setClipAttr(ob, &path, item->fillRule);
					}
					QTransform mpx;
					mpx.translate(xOffset * conversionFactor, yOffset * conversionFactor);
					mpx.scale(item->width() / item->groupWidth, item->height() / item->groupHeight);
					if ((item->rotation() != 0.0) || item->imageFlippedH() || item->imageFlippedV())
					{
						mpx.rotate(item->rotation());
						if (item->imageFlippedH())
						{
							mpx.translate(item->width() * conversionFactor, 0);
							mpx.scale(-1, 1);
						}
						if (item->imageFlippedV())
						{
							mpx.translate(0, item->height() * conversionFactor);
							mpx.scale(1, -1);
						}
					}
					ob.setAttribute("RenderTransform", MatrixToStr(mpx));
					for (int em = 0; em < item->groupItemList.count(); ++em)
					{
						PageItem* embed = item->groupItemList.at(em);
						writeItemOnPage(embed->gXpos, embed->gYpos, embed, ob, rel_root);
					}
					parentElem.appendChild(ob);
				}
			}
			break;
		default:
			handleImageFallBack(item, parentElem, rel_root);
			break;
	}
}

void XPSExPlug::handleImageFallBack(PageItem *item, QDomElement &parentElem, QDomElement &rel_root)
{
	QDomElement ob = p_docu.createElement("Path");
	double maxAdd = 0;
	if (item->hasSoftShadow())
		maxAdd = qMax(fabs(item->softShadowXOffset()), fabs(item->softShadowYOffset())) + item->softShadowBlurRadius();
	QRectF bounds = item->getVisualBoundingRect().adjusted(-maxAdd, -maxAdd, maxAdd, maxAdd);
	QPainterPath path;
	path.moveTo(0, 0);
	path.lineTo(bounds.width(), 0);
	path.lineTo(bounds.width(), bounds.height());
	path.lineTo(0, bounds.height());
	path.closeSubpath();
	QTransform mpp;
	mpp.translate((item->visualXPos() - m_Doc->currentPage()->xOffset() - maxAdd) * conversionFactor, (item->visualYPos() - m_Doc->currentPage()->yOffset() - maxAdd) * conversionFactor);
	mpp.scale(conversionFactor, conversionFactor);
	path = mpp.map(path);
	FPointArray fPath;
	fPath.fromQPainterPath(path, true);
	QString pa = setClipPath(&fPath, true);
	if (item->fillRule)
		pa.prepend("F 0 ");
	else
		pa.prepend("F 1 ");
	ob.setAttribute("Data", pa);
	QDomElement obf = p_docu.createElement("Path.Fill");
	QDomElement gr = p_docu.createElement("ImageBrush");
	double maxSize = qMax(bounds.width(), bounds.height());
	maxSize = qMin(3000.0, maxSize * (m_dpi / 72.0));
	QImage tmpImg = item->DrawObj_toImage(maxSize);
	tmpImg.save(baseDir + "/Resources/Images/" + QString("%1.png").arg(imageCounter), "PNG");
	gr.setAttribute("TileMode", "None");
	gr.setAttribute("ViewboxUnits", "Absolute");
	gr.setAttribute("ViewportUnits", "Absolute");
	gr.setAttribute("Viewport", "0,0,1,1");
	gr.setAttribute("Viewbox", QString("0, 0, %1, %2").arg(tmpImg.width()).arg(tmpImg.height()));
	gr.setAttribute("Viewport", QString("%1, %2, %3, %4").arg((item->visualXPos() - m_Doc->currentPage()->xOffset() - maxAdd) * conversionFactor).arg((item->visualYPos() - m_Doc->currentPage()->yOffset() - maxAdd) * conversionFactor).arg(bounds.width() * conversionFactor).arg(bounds.height() * conversionFactor));
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

void XPSExPlug::processPolyItem(double xOffset, double yOffset, PageItem *item, QDomElement &parentElem, QDomElement &rel_root)
{
	if (((item->GrType == 0) && (item->fillColor() == CommonStrings::None)) && ((item->GrTypeStroke == 0) && (item->lineColor() == CommonStrings::None) && item->NamedLStyle.isEmpty()))
		return;

	if (item->GrType == Gradient_Hatch)
		processHatchFill(xOffset, yOffset, item, parentElem, rel_root);
	bool closedPath;
	closedPath = ((item->itemType() == PageItem::Polygon) || (item->itemType() == PageItem::RegularPolygon) || (item->itemType() == PageItem::Arc));

	QDomElement ob = p_docu.createElement("Path");
	FPointArray path = item->PoLine.copy();
	QTransform mpx;
	if (item->rotation() != 0.0)
	{
		mpx.translate(xOffset * conversionFactor, yOffset * conversionFactor);
		mpx.rotate(item->rotation());
		mpx.translate(-xOffset * conversionFactor, -yOffset * conversionFactor);
	}
	path.translate(xOffset, yOffset);
	path.scale(conversionFactor, conversionFactor);
	QString pa = setClipPath(&path, closedPath);
	if (item->fillRule)
		pa.prepend("F 0 ");
	else
		pa.prepend("F 1 ");
	ob.setAttribute("Data", pa);
	if (item->GrType != Gradient_Hatch)
	{
		if (item->GrMask > 0)
			handleMask(3, item, ob, rel_root, xOffset, yOffset);
		getFillStyle(item, ob, rel_root, xOffset, yOffset);
	}
	if (item->NamedLStyle.isEmpty())
	{
		if ((!item->strokePattern().isEmpty()) && (item->patternStrokePath))
		{
			processSymbolStroke(xOffset, yOffset, item, parentElem, rel_root);
		}
		else
		{
			getStrokeStyle(item, ob, rel_root, xOffset, yOffset);
			if (item->rotation() != 0.0)
				ob.setAttribute("RenderTransform", MatrixToStr(mpx));
			parentElem.appendChild(ob);
		}
	}
	else
	{
		QDomElement grp2 = p_docu.createElement("Canvas");
		multiLine ml = m_Doc->docLineStyles[item->NamedLStyle];
		for (int it = ml.size()-1; it > -1; it--)
		{
			if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
			{
				QDomElement ob3 = p_docu.createElement("Path");
				ob3.setAttribute("Data", pa);
				getMultiStroke(&ml[it], ob3);
				grp2.appendChild(ob3);
			}
		}
		if (item->lineTransparency() != 0)
			grp2.setAttribute("Opacity", FToStr(1.0 - item->lineTransparency()));
		if (item->rotation() != 0.0)
			grp2.setAttribute("RenderTransform", MatrixToStr(mpx));
		parentElem.appendChild(grp2);
	}
}

void XPSExPlug::processLineItem(double xOffset, double yOffset, PageItem *item, QDomElement &parentElem, QDomElement &rel_root)
{
	if ((item->GrTypeStroke != 0) || (item->lineColor() != CommonStrings::None) || !item->NamedLStyle.isEmpty())
	{
		QDomElement ob;
		double x1 = xOffset * conversionFactor;
		double y1 = yOffset * conversionFactor;
		double x2 = (item->width() + xOffset) * conversionFactor;
		double y2 = yOffset * conversionFactor;
		QLineF line = QLineF(x1, y1, x2, y2);
		line.setAngle(-item->rotation());
		if (item->NamedLStyle.isEmpty())
		{
			ob = p_docu.createElement("Path");
			ob.setAttribute("Data", QString("M %1, %2 L %3, %4").arg(line.p1().x()).arg(line.p1().y()).arg(line.p2().x()).arg(line.p2().y()));
			getStrokeStyle(item, ob, rel_root, xOffset, yOffset);
		}
		else
		{
			ob = p_docu.createElement("Canvas");
			multiLine ml = m_Doc->docLineStyles[item->NamedLStyle];
			for (int it = ml.size()-1; it > -1; it--)
			{
				if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
				{
					QDomElement ob2 = p_docu.createElement("Path");
					ob2.setAttribute("Data", QString("M %1, %2 L %3, %4").arg(line.p1().x()).arg(line.p1().y()).arg(line.p2().x()).arg(line.p2().y()));
					getMultiStroke(&ml[it], ob2);
					ob.appendChild(ob2);
				}
			}
			if (item->lineTransparency() != 0)
				ob.setAttribute("Opacity", FToStr(1.0 - item->lineTransparency()));
		}
		parentElem.appendChild(ob);
	}
}

void XPSExPlug::processImageItem(double xOffset, double yOffset, PageItem *item, QDomElement &parentElem, QDomElement &rel_root)
{
	if (item->GrType == Gradient_Hatch)
		processHatchFill(xOffset, yOffset, item, parentElem, rel_root);
	FPointArray path = item->PoLine.copy();
	path.translate(xOffset, yOffset);
	path.scale(conversionFactor, conversionFactor);
	QString pa = setClipPath(&path, true);
	if (item->fillRule)
		pa.prepend("F 0 ");
	else
		pa.prepend("F 1 ");
	QDomElement grp = p_docu.createElement("Canvas");
	QTransform mpx;
	if (item->rotation() != 0.0)
	{
		mpx.translate(xOffset * conversionFactor, yOffset * conversionFactor);
		mpx.rotate(item->rotation());
		mpx.translate(-xOffset * conversionFactor, -yOffset * conversionFactor);
		grp.setAttribute("RenderTransform", MatrixToStr(mpx));
	}
	if (item->GrType != Gradient_Hatch)
	{
		if (item->GrMask > 0)
			handleMask(1, item, grp, rel_root, xOffset, yOffset);
		else
		{
			if (item->fillTransparency() != 0)
				grp.setAttribute("Opacity", FToStr(1.0 - item->fillTransparency()));
		}
		if ((item->GrType != 0) || (item->fillColor() != CommonStrings::None))
		{
			QDomElement ob = p_docu.createElement("Path");
			ob.setAttribute("Data", pa);
			getFillStyle(item, ob, rel_root, xOffset, yOffset, false);
			grp.appendChild(ob);
		}
	}
	if ((item->imageIsAvailable) && (!item->Pfile.isEmpty()))
	{
		QDomElement ob2 = p_docu.createElement("Path");
		ob2.setAttribute("Data", pa);
		ob2.setAttribute("Clip", pa);
		QDomElement obf = p_docu.createElement("Path.Fill");
		QDomElement gr = p_docu.createElement("ImageBrush");
		ScImage img;
		CMSettings cms(m_Doc, item->ImageProfile, item->ImageIntent);
		cms.setUseEmbeddedProfile(item->UseEmbedded);
		cms.allowSoftProofing(true);
		img.loadPicture(item->Pfile, item->pixm.imgInfo.actualPageNumber, cms, ScImage::RGBData, 96);
		img.applyEffect(item->effectsInUse, m_Doc->PageColors, true);
		img.qImagePtr()->setDotsPerMeterX(3780);
		img.qImagePtr()->setDotsPerMeterY(3780);
		img.qImage().save(baseDir + "/Resources/Images/" + QString("%1.png").arg(imageCounter), "PNG");
		gr.setAttribute("TileMode", "None");
		gr.setAttribute("ViewboxUnits", "Absolute");
		gr.setAttribute("ViewportUnits", "Absolute");
		gr.setAttribute("Viewport", "0,0,1,1");
		gr.setAttribute("Viewbox", QString("0, 0, %1, %2").arg(img.width()).arg(img.height()));
		QTransform mpx;
		double xpos = item->imageXOffset() * item->imageXScale();
		double ypos = item->imageYOffset() * item->imageYScale();
		mpx.translate((xOffset + xpos) * conversionFactor, (yOffset + ypos) * conversionFactor);
		mpx.scale(img.width() * item->imageXScale() * conversionFactor, img.height() * item->imageYScale() * conversionFactor);
		if (item->imageFlippedH())
		{
			mpx.translate(item->width() / (img.width() * item->imageXScale()), 0);
			mpx.scale(-1, 1);
		}
		if (item->imageFlippedV())
		{
			mpx.translate(0, item->height() / (img.height() * item->imageYScale()));
			mpx.scale(1, -1);
		}
		mpx.rotate(item->imageRotation());
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
	if ((item->GrTypeStroke != 0) || (item->lineColor() != CommonStrings::None) || !item->NamedLStyle.isEmpty())
	{
		if (item->NamedLStyle.isEmpty())
		{
			if ((!item->strokePattern().isEmpty()) && (item->patternStrokePath))
			{
				processSymbolStroke(xOffset, yOffset, item, parentElem, rel_root);
			}
			else
			{
				QDomElement ob3 = p_docu.createElement("Path");
				ob3.setAttribute("Data", pa);
				getStrokeStyle(item, ob3, rel_root, xOffset, yOffset);
				if (item->rotation() != 0.0)
					ob3.setAttribute("RenderTransform", MatrixToStr(mpx));
				parentElem.appendChild(ob3);
			}
		}
		else
		{
			QDomElement grp2 = p_docu.createElement("Canvas");
			multiLine ml = m_Doc->docLineStyles[item->NamedLStyle];
			for (int it = ml.size()-1; it > -1; it--)
			{
				if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
				{
					QDomElement ob3 = p_docu.createElement("Path");
					ob3.setAttribute("Data", pa);
					getMultiStroke(&ml[it], ob3);
					grp2.appendChild(ob3);
				}
			}
			if (item->lineTransparency() != 0)
				grp2.setAttribute("Opacity", FToStr(1.0 - item->lineTransparency()));
			if (item->rotation() != 0.0)
				grp2.setAttribute("RenderTransform", MatrixToStr(mpx));
			parentElem.appendChild(grp2);
		}
	}
}

class XPSPainter: public TextLayoutPainter
{
	//PageItem *m_item;
	QDomElement m_group;
	XPSExPlug *m_xps;
	QMap<QString, XPSResourceInfo> &m_fontMap;
	QSet<QString> &m_fontRel;
	QDomElement &m_relRoot;

	bool    m_restart { true };
	double  m_current_x { 0.0 };
	double  m_current_y { 0.0 };
	double  m_fontSize { 0.0 };
	QString m_fontUri;
	TextLayoutColor m_fillColor;
	QTransform  m_transform;
	QDomElement m_glyphElem;

public:
	XPSPainter(PageItem *item, QDomElement &group, XPSExPlug *xps, QMap<QString, XPSResourceInfo> &xpsFontMap, QSet<QString> &xpsFontRel, QDomElement &rel_root):
//		m_item(item),
		m_group(group),
		m_xps(xps),
		m_fontMap(xpsFontMap),
		m_fontRel(xpsFontRel),
		m_relRoot(rel_root)
	{ }

	void drawGlyph(const GlyphCluster& gc) override
	{
		if (gc.isControlGlyphs() || gc.isEmpty())
			return;

		XPSResourceInfo fontInfo;
		QString replacementName = font().replacementName();
		if (!m_fontMap.contains(replacementName))
			m_fontMap.insert(replacementName, m_xps->embedFont(font(), m_relRoot));
		fontInfo = m_fontMap.value(replacementName);

		if (!m_fontRel.contains(replacementName))
		{
			m_xps->addFontRelationship(m_relRoot, fontInfo);
			m_fontRel.insert(replacementName);
		}

		QTransform transform = matrix();
		double size = fontSize() * qMax(gc.scaleV(), gc.scaleH()) * m_xps->conversionFactor;
		QString fontUri = fontInfo.uri;

		if (m_restart || (size != m_fontSize) || (m_fillColor != fillColor()) || (m_fontUri != fontUri) ||
			(qAbs(m_current_x - x()) > 1e-6) || (m_current_y != y()) || (m_transform != transform))
		{
			m_glyphElem = m_xps->p_docu.createElement("Glyphs");
			m_glyphElem.setAttribute("RenderTransform", m_xps->MatrixToStr(transform, m_xps->conversionFactor));
			m_glyphElem.setAttribute("BidiLevel", "0");
			m_glyphElem.setAttribute("StyleSimulations", "None");
			m_glyphElem.setAttribute("FontRenderingEmSize", m_xps->FToStr(size));
			m_glyphElem.setAttribute("FontUri", fontUri);
			m_glyphElem.setAttribute("Fill", m_xps->setColor(fillColor().color,fillColor().shade, 0));
			m_glyphElem.setAttribute("OriginX", m_xps->FToStr(x() * m_xps->conversionFactor));
			m_glyphElem.setAttribute("OriginY", m_xps->FToStr(y() * m_xps->conversionFactor));
			m_glyphElem.setAttribute("UnicodeString", QString());
			m_group.appendChild(m_glyphElem);
		}

		QString unicodeString = m_glyphElem.attribute("UnicodeString");
		unicodeString += gc.getText();
		m_glyphElem.setAttribute("UnicodeString", unicodeString);

		QString gcMap;
		QString indices, allIndices = m_glyphElem.attribute("Indices");

		int gcTextSize = gc.getText().size();
		int gcGlyphCount = gc.glyphs().size();
		if (gcTextSize > 1 || gcGlyphCount > 1)
			gcMap = QString("(%1:%2)").arg(gcTextSize).arg(gcGlyphCount);

		double current_x = 0.0;
		double clusterWidth = gc.width();
		const auto& glyphs = gc.glyphs();

		for (int i = 0; i < glyphs.count(); ++i) 
		{
			const GlyphLayout& gl = glyphs.at(i);
			if (gl.glyph >= ScFace::CONTROL_GLYPHS)
			{
				current_x += gl.xadvance * gl.scaleH;
				continue;
			}

			double glWidth = gl.xadvance * gl.scaleH;
			if (i == glyphs.count() - 1) // Clusters may have some extra width
				glWidth = clusterWidth - current_x;

			indices += QString("%1,%2,%3,%4;").arg(gl.glyph)
					.arg((glWidth * m_xps->conversionFactor) / size * 100)
					.arg((-gl.xoffset * m_xps->conversionFactor) / size * 100)
					.arg((-gl.yoffset * m_xps->conversionFactor) / size * 100);
			current_x += glWidth;
		}
		indices.chop(1);
		
		if (!allIndices.isEmpty())
			allIndices += ";";
		allIndices += QString("%1%2").arg(gcMap, indices);
		m_glyphElem.setAttribute("Indices", allIndices);

		m_restart = false;
		m_current_x = x() + clusterWidth;
		m_current_y = y();
		m_fontSize = size;
		m_fontUri = fontUri;
		m_fillColor = fillColor();
		m_transform = transform;
	}

	void drawGlyphOutline(const GlyphCluster& gc, bool fill) override
	{
		if (gc.isControlGlyphs())
			return;

		double current_x = 0.0;
		for (const GlyphLayout& gl : gc.glyphs())
		{
			if (gl.glyph >= ScFace::CONTROL_GLYPHS)
			{
				current_x += gl.xadvance * gl.scaleH;
				continue;
			}

			FPointArray outline = font().glyphOutline(gl.glyph);
			if (outline.size() >= 4)
			{
				QTransform transform = matrix();
				transform.scale((fontSize() * gc.scaleH()) / 10.0, (fontSize() * gc.scaleV()) / 10.0);
				outline.map(transform);
				outline.translate(gl.xoffset + current_x, -(fontSize() * gl.scaleV) + gl.yoffset);
				outline.translate(x(), y());
				outline.scale(m_xps->conversionFactor, m_xps->conversionFactor);
				QString pathData = m_xps->setClipPath(&outline, true);
				QDomElement glyph = m_xps->p_docu.createElement("Path");
				glyph.setAttribute("Data", pathData);
				if (!fill)
					glyph.setAttribute("Fill", m_xps->setColor("None", fillColor().shade, 0));
				else
					glyph.setAttribute("Fill", m_xps->setColor(fillColor().color, fillColor().shade, 0));
				glyph.setAttribute("StrokeThickness", m_xps->FToStr(strokeWidth() * m_xps->conversionFactor));
				glyph.setAttribute("Stroke", m_xps->setColor(strokeColor().color, strokeColor().shade, 0));
				m_group.appendChild(glyph);
				//qDebug() << "StrokeWidth XPS" << strokeWidth();
			}
			current_x += gl.xadvance * gl.scaleH;
		}

		m_restart = true;
	}

	void drawLine(const QPointF& start, const QPointF& end) override
	{
		QTransform transform = matrix();
		QDomElement path = m_xps->p_docu.createElement("Path");
		path.setAttribute("RenderTransform", m_xps->MatrixToStr(transform, m_xps->conversionFactor));
		path.setAttribute("Data", QString("M%1,%2 L%3,%4").arg((x() + start.x()) * m_xps->conversionFactor).arg((y() + end.y()) * m_xps->conversionFactor).arg((x() + start.x() + end.x()) * m_xps->conversionFactor).arg((y() + end.y()) * m_xps->conversionFactor));
		path.setAttribute("Stroke", m_xps->setColor(strokeColor().color, strokeColor().shade, 0));
		path.setAttribute("StrokeThickness", m_xps->FToStr(strokeWidth() * m_xps->conversionFactor));
		m_group.appendChild(path);
		m_restart = true;
	}

	void drawRect(const QRectF& rect) override
	{
		QTransform transform = matrix();
		double rx = (x() + rect.x()) * m_xps->conversionFactor;
		double ry = (y() + rect.y()) * m_xps->conversionFactor;
		double rw = rx + rect.width() * m_xps->conversionFactor;
		double rh = ry + rect.height() * m_xps->conversionFactor;
		QString paS = QString("M%1,%2 ").arg(rx).arg(ry);
		paS += QString("L%1,%2 ").arg(rw).arg(ry);
		paS += QString("L%1,%2 ").arg(rw).arg(rh);
		paS += QString("L%1,%2 ").arg(rx).arg(rh);
		paS += "Z";
		QDomElement path = m_xps->p_docu.createElement("Path");
		path.setAttribute("RenderTransform", m_xps->MatrixToStr(transform, m_xps->conversionFactor));
		path.setAttribute("Data", paS);
		path.setAttribute("Fill", m_xps->setColor(fillColor().color, fillColor().shade, 0));
		path.setAttribute("StrokeThickness", m_xps->FToStr(strokeWidth() * m_xps->conversionFactor));
		path.setAttribute("Stroke", m_xps->setColor(strokeColor().color, strokeColor().shade, 0));
		m_group.appendChild(path);
		m_restart = true;
	}

	void drawObject(PageItem* item) override
	{
		QDomElement canvas = m_xps->p_docu.createElement("Canvas");
		QTransform matrix;
		matrix.translate(x() * m_xps->conversionFactor, (y() - (item->height() * (scaleV() / 1000.0))) * m_xps->conversionFactor);
		if (scaleH() != 1.0)
			matrix.scale(scaleH(), 1);
		if (scaleV() != 1.0)
			matrix.scale(1, scaleV());
		canvas.setAttribute("RenderTransform", m_xps->MatrixToStr(matrix));
		m_xps->writeItemOnPage(0.0, 0.0, item, canvas, m_relRoot);
		m_group.appendChild(canvas);
		m_restart = true;
	}

	void drawObjectDecoration(PageItem* item) override {}
};

void XPSExPlug::processTextItem(double xOffset, double yOffset, PageItem *item, QDomElement &parentElem, QDomElement &rel_root)
{
	if (item->isAnnotation())
		return;
	if (item->GrType == Gradient_Hatch)
		processHatchFill(xOffset, yOffset, item, parentElem, rel_root);
	FPointArray path = item->PoLine.copy();
	path.scale(conversionFactor, conversionFactor);
	QString pa = setClipPath(&path, true);
	if (item->fillRule)
		pa.prepend("F 0 ");
	else
		pa.prepend("F 1 ");
	QDomElement grp = p_docu.createElement("Canvas");
	QTransform mpx;
	QTransform mpl;
	mpl.translate(xOffset * conversionFactor, yOffset * conversionFactor);
	mpx.translate(xOffset * conversionFactor, yOffset * conversionFactor);
	if ((item->rotation() != 0.0) || item->imageFlippedH() || item->imageFlippedV())
	{
		mpx.rotate(item->rotation());
		mpl.rotate(item->rotation());
		if (item->imageFlippedH())
		{
			mpx.translate(item->width() * conversionFactor, 0);
			mpx.scale(-1, 1);
		}
		if (item->imageFlippedV())
		{
			mpx.translate(0, item->height() * conversionFactor);
			mpx.scale(1, -1);
		}
	}
	grp.setAttribute("RenderTransform", MatrixToStr(mpx));
	if (item->isBookmark)
		grp.setAttribute("Name", item->itemName());
	if (item->GrType != Gradient_Hatch)
	{
		if (item->GrMask > 0)
			handleMask(1, item, grp, rel_root, xOffset, yOffset);
		else
		{
			if (item->fillTransparency() != 0)
				grp.setAttribute("Opacity", FToStr(1.0 - item->fillTransparency()));
		}
		if ((item->GrType != 0) || (item->fillColor() != CommonStrings::None))
		{
			FPointArray pathi = item->PoLine.copy();
			if (item->imageFlippedH() || item->imageFlippedV())
			{
				QTransform mpi;
				if (item->imageFlippedH())
				{
					mpi.translate(item->width(), 0);
					mpi.scale(-1, 1);
				}
				if (item->imageFlippedV())
				{
					mpi.translate(0, item->height());
					mpi.scale(1, -1);
				}
				pathi.map(mpi);
			}
			pathi.scale(conversionFactor, conversionFactor);
			QString pai = setClipPath(&pathi, true);
			if (item->fillRule)
				pai.prepend("F 0 ");
			else
				pai.prepend("F 1 ");
			QDomElement ob = p_docu.createElement("Path");
			ob.setAttribute("Data", pai);
			getFillStyle(item, ob, rel_root, xOffset, yOffset, false);
			grp.appendChild(ob);
		}
	}
	if (item->isPathText())
	{
		if ((item->PoShow) && (item->lineColor() != CommonStrings::None))
		{
			QDomElement ob = p_docu.createElement("Path");
			FPointArray path = item->PoLine.copy();
			path.scale(conversionFactor, conversionFactor);
			QString pa = setClipPath(&path, false);
			ob.setAttribute("Data", pa);
			if (item->NamedLStyle.isEmpty())
			{
				if ((!item->strokePattern().isEmpty()) && (item->patternStrokePath))
				{
					processSymbolStroke(xOffset, yOffset, item, parentElem, rel_root);
				}
				else
				{
					getStrokeStyle(item, ob, rel_root, xOffset, yOffset);
					grp.appendChild(ob);
				}
			}
			else
			{
				QDomElement grp2 = p_docu.createElement("Canvas");
				multiLine ml = m_Doc->docLineStyles[item->NamedLStyle];
				for (int it = ml.size()-1; it > -1; it--)
				{
					if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
					{
						QDomElement ob3 = p_docu.createElement("Path");
						ob3.setAttribute("Data", pa);
						getMultiStroke(&ml[it], ob3);
						grp2.appendChild(ob3);
					}
				}
				if (item->lineTransparency() != 0)
					grp2.setAttribute("Opacity", FToStr(1.0 - item->lineTransparency()));
				grp.appendChild(grp2);
			}
		}
	}

	if (grp.hasChildNodes())
		parentElem.appendChild(grp);

	if (item->itemText.length() != 0)
	{
		QDomElement grp2 = p_docu.createElement("Canvas");
		if (grp.hasAttribute("RenderTransform"))
			grp2.setAttribute("RenderTransform", grp.attribute("RenderTransform"));
		if (grp.hasAttribute("Name"))
			grp2.setAttribute("Name", grp.attribute("Name"));
		if (grp.hasAttribute("Opacity"))
			grp2.setAttribute("Opacity", grp.attribute("Opacity"));
		XPSPainter p(item, grp2, this, xps_fontMap, xps_fontRel, rel_root);
		item->textLayout.renderBackground(&p);
		item->textLayout.render(&p);
		parentElem.appendChild(grp2);
	}
	if (item->isTextFrame())
	{
		if ((item->GrTypeStroke != 0) || (item->lineColor() != CommonStrings::None) || !item->NamedLStyle.isEmpty())
		{
			if (item->NamedLStyle.isEmpty())
			{
				if ((!item->strokePattern().isEmpty()) && (item->patternStrokePath))
				{
					processSymbolStroke(xOffset, yOffset, item, parentElem, rel_root);
				}
				else
				{
					QDomElement ob3 = p_docu.createElement("Path");
					ob3.setAttribute("Data", pa);
					getStrokeStyle(item, ob3, rel_root, xOffset, yOffset);
					ob3.setAttribute("RenderTransform", MatrixToStr(mpl));
					parentElem.appendChild(ob3);
				}
			}
			else
			{
				QDomElement grp2 = p_docu.createElement("Canvas");
				multiLine ml = m_Doc->docLineStyles[item->NamedLStyle];
				for (int it = ml.size()-1; it > -1; it--)
				{
					if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
					{
						QDomElement ob3 = p_docu.createElement("Path");
						ob3.setAttribute("Data", pa);
						getMultiStroke(&ml[it], ob3);
						grp2.appendChild(ob3);
					}
				}
				if (item->lineTransparency() != 0)
					grp2.setAttribute("Opacity", FToStr(1.0 - item->lineTransparency()));
				grp2.setAttribute("RenderTransform", MatrixToStr(mpl));
				parentElem.appendChild(grp2);
			}
		}
	}
}

void XPSExPlug::processSymbolItem(double xOffset, double yOffset, PageItem *item, QDomElement &parentElem, QDomElement &rel_root)
{
	QDomElement ob = p_docu.createElement("Canvas");
	FPointArray path = item->PoLine.copy();
	ScPattern pat = m_Doc->docPatterns[item->pattern()];
	path.scale(conversionFactor, conversionFactor);
	path.scale(pat.width / item->width(), pat.height / item->height());
	setClipAttr(ob, &path, item->fillRule);
	QTransform mpx;
	mpx.translate(xOffset * conversionFactor, yOffset * conversionFactor);
	mpx.scale(item->width() / pat.width, item->height() / pat.height);
	if ((item->rotation() != 0.0) || item->imageFlippedH() || item->imageFlippedV())
	{
		mpx.rotate(item->rotation());
		if (item->imageFlippedH())
		{
			mpx.translate(item->width() * conversionFactor, 0);
			mpx.scale(-1, 1);
		}
		if (item->imageFlippedV())
		{
			mpx.translate(0, item->height() * conversionFactor);
			mpx.scale(1, -1);
		}
	}
	ob.setAttribute("RenderTransform", MatrixToStr(mpx));
	if (item->GrMask > 0)
		handleMask(1, item, ob, rel_root, xOffset, yOffset);
	else
	{
		if (item->fillTransparency() != 0)
			ob.setAttribute("Opacity", FToStr(1.0 - item->fillTransparency()));
	}
	for (int em = 0; em < pat.items.count(); ++em)
	{
		PageItem* embed = pat.items.at(em);
		writeItemOnPage(embed->gXpos, embed->gYpos, embed, ob, rel_root);
	}
	parentElem.appendChild(ob);
}

void XPSExPlug::processTableItem(double xOffset, double yOffset, PageItem *item, QDomElement &parentElem, QDomElement &rel_root)
{
	QDomElement ob = p_docu.createElement("Canvas");
	QTransform mpx;
	mpx.translate(xOffset * conversionFactor, yOffset * conversionFactor);
	if (item->rotation() != 0.0)
		mpx.rotate(item->rotation());
	mpx.translate(item->asTable()->gridOffset().x() * conversionFactor, item->asTable()->gridOffset().y() * conversionFactor);
	ob.setAttribute("RenderTransform", MatrixToStr(mpx));
	// Paint table fill.
	if (item->asTable()->fillColor() != CommonStrings::None)
	{
		int lastCol = item->asTable()->columns() - 1;
		int lastRow = item->asTable()->rows() - 1;
		double x = item->asTable()->columnPosition(0);
		double y = item->asTable()->rowPosition(0);
		double width = item->asTable()->columnPosition(lastCol) + item->asTable()->columnWidth(lastCol) - x;
		double height = item->asTable()->rowPosition(lastRow) + item->asTable()->rowHeight(lastRow) - y;
		FPointArray path;
		path.svgInit();
		path.svgMoveTo(0, 0);
		path.svgLineTo(width, 0);
		path.svgLineTo(width, height);
		path.svgLineTo(0, height);
		path.svgClosePath();
		path.scale(conversionFactor, conversionFactor);
		QString pa = setClipPath(&path, true);
		QDomElement cl = p_docu.createElement("Path");
		cl.setAttribute("Data", pa);
		cl.setAttribute("Fill", setColor(item->asTable()->fillColor(), item->asTable()->fillShade(), 0));
		ob.appendChild(cl);
	}
	// Pass 1: Paint cell fills.
	for (int row = 0; row < item->asTable()->rows(); ++row)
	{
		int colSpan = 0;
		for (int col = 0; col < item->asTable()->columns(); col += colSpan)
		{
			TableCell cell = item->asTable()->cellAt(row, col);
			if (row == cell.row())
			{
				QString colorName = cell.fillColor();
				if (colorName != CommonStrings::None)
				{
					int row = cell.row();
					int col = cell.column();
					int lastRow = row + cell.rowSpan() - 1;
					int lastCol = col + cell.columnSpan() - 1;
					double x = item->asTable()->columnPosition(col);
					double y = item->asTable()->rowPosition(row);
					double width = item->asTable()->columnPosition(lastCol) + item->asTable()->columnWidth(lastCol) - x;
					double height = item->asTable()->rowPosition(lastRow) + item->asTable()->rowHeight(lastRow) - y;
					FPointArray path;
					path.svgInit();
					path.svgMoveTo(x, y);
					path.svgLineTo(x + width, y);
					path.svgLineTo(x + width, y + height);
					path.svgLineTo(x, y + height);
					path.svgClosePath();
					path.scale(conversionFactor, conversionFactor);
					QString pa = setClipPath(&path, true);
					QDomElement cl = p_docu.createElement("Path");
					cl.setAttribute("Data", pa);
					cl.setAttribute("Fill", setColor(colorName, cell.fillShade(), 0));
					ob.appendChild(cl);
				}
			}
			colSpan = cell.columnSpan();
		}
	}
	// Pass 2: Paint vertical borders.
	for (int row = 0; row < item->asTable()->rows(); ++row)
	{
		int colSpan = 0;
		for (int col = 0; col < item->asTable()->columns(); col += colSpan)
		{
			TableCell cell = item->asTable()->cellAt(row, col);
			if (row == cell.row())
			{
				const int lastRow = cell.row() + cell.rowSpan() - 1;
				const int lastCol = cell.column() + cell.columnSpan() - 1;
				const double borderX = item->asTable()->columnPosition(lastCol) + item->asTable()->columnWidth(lastCol);
				QPointF start(borderX, 0.0);
				QPointF end(borderX, 0.0);
				QPointF startOffsetFactors, endOffsetFactors;
				int startRow, endRow;
				for (int row = cell.row(); row <= lastRow; row += endRow - startRow + 1)
				{
					TableCell rightCell = item->asTable()->cellAt(row, lastCol + 1);
					startRow = qMax(cell.row(), rightCell.row());
					endRow = qMin(lastRow, rightCell.isValid() ? rightCell.row() + rightCell.rowSpan() - 1 : lastRow);
					TableCell topLeftCell = item->asTable()->cellAt(startRow - 1, lastCol);
					TableCell topRightCell = item->asTable()->cellAt(startRow - 1, lastCol + 1);
					TableCell bottomRightCell = item->asTable()->cellAt(endRow + 1, lastCol + 1);
					TableCell bottomLeftCell = item->asTable()->cellAt(endRow + 1, lastCol);
					TableBorder topLeft, top, topRight, border, bottomLeft, bottom, bottomRight;
					resolveBordersVertical(topLeftCell, topRightCell, cell, rightCell, bottomLeftCell, bottomRightCell,
										   &topLeft, &top, &topRight, &border, &bottomLeft, &bottom, &bottomRight, item->asTable());
					if (border.isNull())
						continue; // Quit early if the border to paint is null.
					start.setY(item->asTable()->rowPosition(startRow));
					end.setY((item->asTable()->rowPosition(endRow) + item->asTable()->rowHeight(endRow)));
					joinVertical(border, topLeft, top, topRight, bottomLeft, bottom, bottomRight, &start, &end, &startOffsetFactors, &endOffsetFactors);
					paintBorder(border, start, end, startOffsetFactors, endOffsetFactors, ob);
				}
				if (col == 0)
				{
					const int lastRow = cell.row() + cell.rowSpan() - 1;
					const int firstCol = cell.column();
					const double borderX = item->asTable()->columnPosition(firstCol);
					QPointF start(borderX, 0.0);
					QPointF end(borderX, 0.0);
					QPointF startOffsetFactors, endOffsetFactors;
					int startRow, endRow;
					for (int row = cell.row(); row <= lastRow; row += endRow - startRow + 1)
					{
						TableCell leftCell = item->asTable()->cellAt(row, firstCol - 1);
						startRow = qMax(cell.row(), leftCell.row());
						endRow = qMin(lastRow, leftCell.isValid() ? leftCell.row() + leftCell.rowSpan() - 1 : lastRow);
						TableCell topLeftCell = item->asTable()->cellAt(startRow - 1, firstCol - 1);
						TableCell topRightCell = item->asTable()->cellAt(startRow - 1, firstCol);
						TableCell bottomRightCell = item->asTable()->cellAt(lastRow + 1, firstCol);
						TableCell bottomLeftCell = item->asTable()->cellAt(lastRow + 1, firstCol - 1);
						TableBorder topLeft, top, topRight, border, bottomLeft, bottom, bottomRight;
						resolveBordersVertical(topLeftCell, topRightCell, leftCell, cell, bottomLeftCell, bottomRightCell,
											   &topLeft, &top, &topRight, &border, &bottomLeft, &bottom, &bottomRight, item->asTable());
						if (border.isNull())
							continue; // Quit early if the border to paint is null.
						start.setY(item->asTable()->rowPosition(startRow));
						end.setY((item->asTable()->rowPosition(endRow) + item->asTable()->rowHeight(endRow)));
						joinVertical(border, topLeft, top, topRight, bottomLeft, bottom, bottomRight, &start, &end, &startOffsetFactors, &endOffsetFactors);
						paintBorder(border, start, end, startOffsetFactors, endOffsetFactors, ob);
					}
				}
			}
			colSpan = cell.columnSpan();
		}
	}
	// Pass 3: Paint horizontal borders.
	for (int row = 0; row < item->asTable()->rows(); ++row)
	{
		int colSpan = 0;
		for (int col = 0; col < item->asTable()->columns(); col += colSpan)
		{
			TableCell cell = item->asTable()->cellAt(row, col);
			if (row == cell.row())
			{
				const int lastRow = cell.row() + cell.rowSpan() - 1;
				const int lastCol = cell.column() + cell.columnSpan() - 1;
				const double borderY = (item->asTable()->rowPosition(lastRow) + item->asTable()->rowHeight(lastRow));
				QPointF start(0.0, borderY);
				QPointF end(0.0, borderY);
				QPointF startOffsetFactors, endOffsetFactors;
				int startCol, endCol;
				for (int col = cell.column(); col <= lastCol; col += endCol - startCol + 1)
				{
					TableCell bottomCell = item->asTable()->cellAt(lastRow + 1, col);
					startCol = qMax(cell.column(), bottomCell.column());
					endCol = qMin(lastCol, bottomCell.isValid() ? bottomCell.column() + bottomCell.columnSpan() - 1 : lastCol);
					TableCell topLeftCell = item->asTable()->cellAt(lastRow, startCol - 1);
					TableCell topRightCell = item->asTable()->cellAt(lastRow, endCol + 1);
					TableCell bottomRightCell = item->asTable()->cellAt(lastRow + 1, endCol + 1);
					TableCell bottomLeftCell = item->asTable()->cellAt(lastRow + 1, startCol - 1);
					TableBorder topLeft, left, bottomLeft, border, topRight, right, bottomRight;
					resolveBordersHorizontal(topLeftCell, cell, topRightCell, bottomLeftCell, bottomCell,
											 bottomRightCell, &topLeft, &left, &bottomLeft, &border, &topRight, &right, &bottomRight, item->asTable());
					if (border.isNull())
						continue; // Quit early if the border is null.
					start.setX(item->asTable()->columnPosition(startCol));
					end.setX(item->asTable()->columnPosition(endCol) + item->asTable()->columnWidth(endCol));
					joinHorizontal(border, topLeft, left, bottomLeft, topRight, right, bottomRight, &start, &end, &startOffsetFactors, &endOffsetFactors);
					paintBorder(border, start, end, startOffsetFactors, endOffsetFactors, ob);
				}
				if (row == 0)
				{
					const int firstRow = cell.row();
					const int lastCol = cell.column() + cell.columnSpan() - 1;
					const double borderY = item->asTable()->rowPosition(firstRow);
					QPointF start(0.0, borderY);
					QPointF end(0.0, borderY);
					QPointF startOffsetFactors, endOffsetFactors;
					int startCol, endCol;
					for (int col = cell.column(); col <= lastCol; col += endCol - startCol + 1)
					{
						TableCell topCell = item->asTable()->cellAt(firstRow - 1, col);
						startCol = qMax(cell.column(), topCell.column());
						endCol = qMin(lastCol, topCell.isValid() ? topCell.column() + topCell.columnSpan() - 1 : lastCol);
						TableCell topLeftCell = item->asTable()->cellAt(firstRow - 1, startCol - 1);
						TableCell topRightCell = item->asTable()->cellAt(firstRow - 1, endCol + 1);
						TableCell bottomRightCell = item->asTable()->cellAt(firstRow, endCol + 1);
						TableCell bottomLeftCell = item->asTable()->cellAt(firstRow, startCol - 1);
						TableBorder topLeft, left, bottomLeft, border, topRight, right, bottomRight;
						resolveBordersHorizontal(topLeftCell, topCell, topRightCell, bottomLeftCell, cell,
												 bottomRightCell, &topLeft, &left, &bottomLeft, &border, &topRight, &right, &bottomRight, item->asTable());
						if (border.isNull())
							continue; // Quit early if the border is null.
						start.setX(item->asTable()->columnPosition(startCol));
						end.setX(item->asTable()->columnPosition(endCol) + item->asTable()->columnWidth(endCol));
						joinHorizontal(border, topLeft, left, bottomLeft, topRight, right, bottomRight, &start, &end, &startOffsetFactors, &endOffsetFactors);
						paintBorder(border, start, end, startOffsetFactors, endOffsetFactors, ob);
					}
				}
			}
			colSpan = cell.columnSpan();
		}
	}
	// Pass 4: Paint cell content.
	for (int row = 0; row < item->asTable()->rows(); ++row)
	{
		for (int col = 0; col < item->asTable()->columns(); col ++)
		{
			TableCell cell = item->asTable()->cellAt(row, col);
			if (cell.row() == row && cell.column() == col)
			{
				PageItem* textFrame = cell.textFrame();
				processTextItem(cell.contentRect().x(), cell.contentRect().y(), textFrame, ob, rel_root);
			}
		}
	}
	if (item->GrMask > 0)
		handleMask(1, item, ob, rel_root, xOffset, yOffset);
	else
	{
		if (item->fillTransparency() != 0)
			ob.setAttribute("Opacity", FToStr(1.0 - item->fillTransparency()));
	}
	parentElem.appendChild(ob);
}

void XPSExPlug::paintBorder(const TableBorder& border, const QPointF& start, const QPointF& end, const QPointF& startOffsetFactors, const QPointF& endOffsetFactors, QDomElement &ob)
{
	QPointF lineStart, lineEnd;
	for (const TableBorderLine& line : border.borderLines())
	{
		lineStart.setX(start.x() + line.width() * startOffsetFactors.x());
		lineStart.setY(start.y() + line.width() * startOffsetFactors.y());
		lineEnd.setX(end.x() + line.width() * endOffsetFactors.x());
		lineEnd.setY(end.y() + line.width() * endOffsetFactors.y());
		QDomElement cl = p_docu.createElement("Path");
		cl.setAttribute("Data", "M" + FToStr(lineStart.x() * conversionFactor) + "," + FToStr(lineStart.y() * conversionFactor) + " L" + FToStr(lineEnd.x() * conversionFactor) + " " + FToStr(lineEnd.y() * conversionFactor));
		QString dashVals;
		if (line.style() != Qt::SolidLine)
			dashVals = getDashString(line.style(), qMax(line.width(), 1.0));
		if (!dashVals.isEmpty())
			cl.setAttribute("StrokeDashArray", dashVals);
		if (line.color() != CommonStrings::None)
			cl.setAttribute("Stroke", setColor(line.color(), line.shade(), 0));
		if (line.width() != 0.0)
			cl.setAttribute("StrokeThickness", FToStr(line.width() * conversionFactor));
		else
			cl.setAttribute("StrokeThickness", FToStr(1.0 * conversionFactor));
		ob.appendChild(cl);
	}
}

void XPSExPlug::processHatchFill(double xOffset, double yOffset, PageItem *item, QDomElement &parentElem, QDomElement &rel_root)
{
	QDomElement obC = p_docu.createElement("Canvas");
	FPointArray path = item->PoLine.copy();
	path.scale(conversionFactor, conversionFactor);
	setClipAttr(obC, &path, item->fillRule);
	if (item->GrMask > 0)
		handleMask(1, item, obC, rel_root, xOffset, yOffset);
	else
	{
		if (item->fillTransparency() != 0)
			obC.setAttribute("Opacity", FToStr(1.0 - item->fillTransparency()));
	}
	QTransform mpo;
	mpo.translate(xOffset * conversionFactor, yOffset * conversionFactor);
	if (item->rotation() != 0.0)
		mpo.rotate(item->rotation());
	obC.setAttribute("RenderTransform", MatrixToStr(mpo));
	if (item->hatchUseBackground)
	{
		FPointArray path;
		path.svgInit();
		path.svgMoveTo(0, 0);
		path.svgLineTo(item->width(), 0);
		path.svgLineTo(item->width(), item->height());
		path.svgLineTo(0, item->height());
		path.svgClosePath();
		path.scale(conversionFactor, conversionFactor);
		QString pa = setClipPath(&path, true);
		QDomElement cl = p_docu.createElement("Path");
		cl.setAttribute("Data", pa);
		cl.setAttribute("Fill", setColor(item->hatchBackground, 100, 0));
		obC.appendChild(cl);
	}
	double lineLen = sqrt((item->width() / 2.0) * (item->width() / 2.0) + (item->height() / 2.0) * (item->height() / 2.0)) * conversionFactor;
	double dist = 0.0;
	while (dist < lineLen)
	{
		QTransform mpx;
		mpx.translate((item->width() / 2.0) * conversionFactor, (item->height() / 2.0) * conversionFactor);
		if (item->hatchAngle != 0.0)
			mpx.rotate(-item->hatchAngle);
		QDomElement ob = p_docu.createElement("Path");
		ob.setAttribute("StrokeThickness", FToStr(conversionFactor));
		ob.setAttribute("StrokeDashCap", "Flat");
		ob.setAttribute("StrokeEndLineCap", "Flat");
		ob.setAttribute("StrokeStartLineCap", "Flat");
		ob.setAttribute("StrokeLineJoin", "Miter");
		ob.setAttribute("Stroke", setColor(item->hatchForeground, 100, 0));
		ob.setAttribute("Data", QString("M %1, %2 L %3, %4").arg(-lineLen).arg(dist).arg(lineLen).arg(dist));
		ob.setAttribute("RenderTransform", MatrixToStr(mpx));
		obC.appendChild(ob);
		if (dist > 0)
		{
			QDomElement ob = p_docu.createElement("Path");
			ob.setAttribute("StrokeThickness", FToStr(conversionFactor));
			ob.setAttribute("StrokeDashCap", "Flat");
			ob.setAttribute("StrokeEndLineCap", "Flat");
			ob.setAttribute("StrokeStartLineCap", "Flat");
			ob.setAttribute("StrokeLineJoin", "Miter");
			ob.setAttribute("Stroke", setColor(item->hatchForeground, 100, 0));
			ob.setAttribute("Data", QString("M %1, %2 L %3, %4").arg(-lineLen).arg(-dist).arg(lineLen).arg(-dist));
			ob.setAttribute("RenderTransform", MatrixToStr(mpx));
			obC.appendChild(ob);
		}
		dist += item->hatchDistance * conversionFactor;
	}
	if ((item->hatchType == 1) || (item->hatchType == 2))
	{
		dist = 0.0;
		while (dist < lineLen)
		{
			QTransform mpx;
			mpx.translate((item->width() / 2.0) * conversionFactor, (item->height() / 2.0) * conversionFactor);
			if (item->hatchAngle != 0.0)
				mpx.rotate(-item->hatchAngle + 90);
			QDomElement ob = p_docu.createElement("Path");
			ob.setAttribute("StrokeThickness", FToStr(conversionFactor));
			ob.setAttribute("StrokeDashCap", "Flat");
			ob.setAttribute("StrokeEndLineCap", "Flat");
			ob.setAttribute("StrokeStartLineCap", "Flat");
			ob.setAttribute("StrokeLineJoin", "Miter");
			ob.setAttribute("Stroke", setColor(item->hatchForeground, 100, 0));
			ob.setAttribute("Data", QString("M %1, %2 L %3, %4").arg(-lineLen).arg(dist).arg(lineLen).arg(dist));
			ob.setAttribute("RenderTransform", MatrixToStr(mpx));
			obC.appendChild(ob);
			if (dist > 0)
			{
				QDomElement ob = p_docu.createElement("Path");
				ob.setAttribute("StrokeThickness", FToStr(conversionFactor));
				ob.setAttribute("StrokeDashCap", "Flat");
				ob.setAttribute("StrokeEndLineCap", "Flat");
				ob.setAttribute("StrokeStartLineCap", "Flat");
				ob.setAttribute("StrokeLineJoin", "Miter");
				ob.setAttribute("Stroke", setColor(item->hatchForeground, 100, 0));
				ob.setAttribute("Data", QString("M %1, %2 L %3, %4").arg(-lineLen).arg(-dist).arg(lineLen).arg(-dist));
				ob.setAttribute("RenderTransform", MatrixToStr(mpx));
				obC.appendChild(ob);
			}
			dist += item->hatchDistance * conversionFactor;
		}
	}
	if (item->hatchType == 2)
	{
		dist = 0.0;
		while (dist < lineLen)
		{
			double dDist = dist * sqrt(2.0);
			QTransform mpx;
			mpx.translate((item->width() / 2.0) * conversionFactor, (item->height() / 2.0) * conversionFactor);
			if (item->hatchAngle != 0.0)
				mpx.rotate(-item->hatchAngle + 45);
			QDomElement ob = p_docu.createElement("Path");
			ob.setAttribute("StrokeThickness", FToStr(conversionFactor));
			ob.setAttribute("StrokeDashCap", "Flat");
			ob.setAttribute("StrokeEndLineCap", "Flat");
			ob.setAttribute("StrokeStartLineCap", "Flat");
			ob.setAttribute("StrokeLineJoin", "Miter");
			ob.setAttribute("Stroke", setColor(item->hatchForeground, 100, 0));
			ob.setAttribute("Data", QString("M %1, %2 L %3, %4").arg(-lineLen).arg(dDist).arg(lineLen).arg(dDist));
			ob.setAttribute("RenderTransform", MatrixToStr(mpx));
			obC.appendChild(ob);
			if (dist > 0)
			{
				QDomElement ob = p_docu.createElement("Path");
				ob.setAttribute("StrokeThickness", FToStr(conversionFactor));
				ob.setAttribute("StrokeDashCap", "Flat");
				ob.setAttribute("StrokeEndLineCap", "Flat");
				ob.setAttribute("StrokeStartLineCap", "Flat");
				ob.setAttribute("StrokeLineJoin", "Miter");
				ob.setAttribute("Stroke", setColor(item->hatchForeground, 100, 0));
				ob.setAttribute("Data", QString("M %1, %2 L %3, %4").arg(-lineLen).arg(-dDist).arg(lineLen).arg(-dDist));
				ob.setAttribute("RenderTransform", MatrixToStr(mpx));
				obC.appendChild(ob);
			}
			dist += item->hatchDistance * conversionFactor;
		}
	}
	parentElem.appendChild(obC);
}

void XPSExPlug::processSymbolStroke(double xOffset, double yOffset, PageItem *item, QDomElement &parentElem, QDomElement &rel_root)
{
	QDomElement ob = p_docu.createElement("Canvas");
	QTransform mpx;
	mpx.translate(xOffset * conversionFactor, yOffset * conversionFactor);
	ob.setAttribute("RenderTransform", MatrixToStr(mpx));
	QPainterPath path = item->PoLine.toQPainterPath(false);
	ScPattern pat = m_Doc->docPatterns[item->strokePattern()];
	double pLen = path.length() - ((pat.width / 2.0) * item->patternStrokeTransfrm.scaleX);
	double adv = pat.width * item->patternStrokeTransfrm.scaleX * item->patternStrokeTransfrm.space;
	double xpos = item->patternStrokeTransfrm.offsetX * item->patternStrokeTransfrm.scaleX;
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
		trans.translate(0.0, item->patternStrokeTransfrm.offsetY);
		trans.rotate(-item->patternStrokeTransfrm.rotation);
		trans.shear(item->patternStrokeTransfrm.skewX, -item->patternStrokeTransfrm.skewY);
		trans.scale(item->patternStrokeTransfrm.scaleX, item->patternStrokeTransfrm.scaleY);
		trans.translate(-pat.width / 2.0, -pat.height / 2.0);
		if (item->patternStrokeMirrorX)
		{
			trans.translate(pat.width, 0);
			trans.scale(-1, 1);
		}
		if (item->patternStrokeMirrorY)
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
}

void XPSExPlug::processArrows(double xOffset, double yOffset, PageItem *item, QDomElement &parentElem, QDomElement &rel_root)
{
	if (item->startArrowIndex() != 0)
	{
		QTransform arrowTrans;
		FPointArray arrow = m_Doc->arrowStyles().at(item->startArrowIndex()-1).points.copy();
		if (item->itemType() == PageItem::Line)
		{
			arrowTrans.translate(0, 0);
			arrowTrans.scale(item->startArrowScale() / 100.0, item->startArrowScale() / 100.0);
			if (item->NamedLStyle.isEmpty())
			{
				if (item->lineWidth() != 0.0)
					arrowTrans.scale(item->lineWidth(), item->lineWidth());
			}
			else
			{
				multiLine ml = m_Doc->docLineStyles[item->NamedLStyle];
				if (ml[ml.size()-1].Width != 0.0)
					arrowTrans.scale(ml[ml.size()-1].Width, ml[ml.size()-1].Width);
			}
			arrowTrans.scale(-1,1);
		}
		else
		{
			FPoint Start = item->PoLine.point(0);
			for (int xx = 1; xx < item->PoLine.size(); xx += 2)
			{
				FPoint Vector = item->PoLine.point(xx);
				if ((Start.x() != Vector.x()) || (Start.y() != Vector.y()))
				{
					double r = atan2(Start.y()-Vector.y(),Start.x()-Vector.x())*(180.0/M_PI);
					arrowTrans.translate(Start.x(), Start.y());
					arrowTrans.rotate(r);
					arrowTrans.scale(item->startArrowScale() / 100.0, item->startArrowScale() / 100.0);
					if (item->NamedLStyle.isEmpty())
					{
						if (item->lineWidth() != 0.0)
							arrowTrans.scale(item->lineWidth(), item->lineWidth());
					}
					else
					{
						multiLine ml = m_Doc->docLineStyles[item->NamedLStyle];
						if (ml[ml.size()-1].Width != 0.0)
							arrowTrans.scale(ml[ml.size()-1].Width, ml[ml.size()-1].Width);
					}
					break;
				}
			}
		}
		arrow.map(arrowTrans);
		drawArrow(xOffset, yOffset, item, parentElem, rel_root, arrow);
	}
	if (item->endArrowIndex() != 0)
	{
		QTransform arrowTrans;
		FPointArray arrow = m_Doc->arrowStyles().at(item->endArrowIndex()-1).points.copy();
		if (item->itemType() == PageItem::Line)
		{
			arrowTrans.translate(item->width(), 0);
			arrowTrans.scale(item->endArrowScale() / 100.0, item->endArrowScale() / 100.0);
			if (item->NamedLStyle.isEmpty())
			{
				if (item->lineWidth() != 0.0)
					arrowTrans.scale(item->lineWidth(), item->lineWidth());
			}
			else
			{
				multiLine ml = m_Doc->docLineStyles[item->NamedLStyle];
				if (ml[ml.size()-1].Width != 0.0)
					arrowTrans.scale(ml[ml.size()-1].Width, ml[ml.size()-1].Width);
			}
		}
		else
		{
			FPoint End = item->PoLine.point(item->PoLine.size()-2);
			for (uint xx = item->PoLine.size()-1; xx > 0; xx -= 2)
			{
				FPoint Vector = item->PoLine.point(xx);
				if ((End.x() != Vector.x()) || (End.y() != Vector.y()))
				{
					double r = atan2(End.y()-Vector.y(),End.x()-Vector.x())*(180.0/M_PI);
					arrowTrans.translate(End.x(), End.y());
					arrowTrans.rotate(r);
					arrowTrans.scale(item->endArrowScale() / 100.0, item->endArrowScale() / 100.0);
					if (item->NamedLStyle.isEmpty())
					{
						if (item->lineWidth() != 0.0)
							arrowTrans.scale(item->lineWidth(), item->lineWidth());
					}
					else
					{
						multiLine ml = m_Doc->docLineStyles[item->NamedLStyle];
						if (ml[ml.size()-1].Width != 0.0)
							arrowTrans.scale(ml[ml.size()-1].Width, ml[ml.size()-1].Width);
					}
					break;
				}
			}
		}
		arrow.map(arrowTrans);
		drawArrow(xOffset, yOffset, item, parentElem, rel_root, arrow);
	}
}

void XPSExPlug::drawArrow(double xOffset, double yOffset, PageItem *item, QDomElement &parentElem, QDomElement &rel_root, FPointArray &arrow)
{
	QTransform mpx;
	if (item->rotation() != 0.0)
	{
		mpx.translate(xOffset * conversionFactor, yOffset * conversionFactor);
		mpx.rotate(item->rotation());
		mpx.translate(-xOffset * conversionFactor, -yOffset * conversionFactor);
	}
	arrow.translate(xOffset, yOffset);
	arrow.scale(conversionFactor, conversionFactor);
	QString pa = setClipPath(&arrow, true);
	if (item->NamedLStyle.isEmpty())
	{
		QDomElement ob = p_docu.createElement("Path");
		ob.setAttribute("Data", pa);
		ob.setAttribute("RenderTransform", MatrixToStr(mpx));
		getStrokeStyle(item, ob, rel_root, xOffset, yOffset, true);
		parentElem.appendChild(ob);
	}
	else
	{
		QDomElement grp2 = p_docu.createElement("Canvas");
		grp2.setAttribute("RenderTransform", MatrixToStr(mpx));
		multiLine ml = m_Doc->docLineStyles[item->NamedLStyle];
		if (ml[0].Color != CommonStrings::None)
		{
			QDomElement ob3 = p_docu.createElement("Path");
			ob3.setAttribute("Data", pa);
			ob3.setAttribute("Fill", setColor(ml[0].Color, ml[0].Shade, 0));
			getMultiStroke(&ml[0], ob3);
			grp2.appendChild(ob3);
		}
		for (int it = ml.size()-1; it > 0; it--)
		{
			if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
			{
				QDomElement ob3 = p_docu.createElement("Path");
				ob3.setAttribute("Data", pa);
				getMultiStroke(&ml[it], ob3);
				grp2.appendChild(ob3);
			}
		}
		parentElem.appendChild(grp2);
	}
}

void XPSExPlug::addFontRelationship(QDomElement &rel_root, const XPSResourceInfo& fontInfo)
{
	QDomElement rel = r_docu.createElement("Relationship");
	rel.setAttribute("Id", fontInfo.id);
	rel.setAttribute("Type", "http://schemas.microsoft.com/xps/2005/06/required-resource");
	rel.setAttribute("Target", fontInfo.uri);
	rel_root.appendChild(rel);
}

XPSResourceInfo XPSExPlug::embedFont(const ScFace& font, QDomElement &rel_root)
{
	QByteArray fontData;
	loadRawText(font.fontFilePath(), fontData);
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

	XPSResourceInfo rsrcInfo;
	rsrcInfo.id =  QString("rIDf%1").arg(fontCounter);
	rsrcInfo.uri = "/Resources/Fonts/" + guidString + ".odttf";
	fontCounter++;

	return rsrcInfo;
}

void XPSExPlug::getMultiStroke(struct SingleLine *sl, QDomElement &parentElem)
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
	QString dashVals;
	if (static_cast<Qt::PenStyle>(sl->Dash) != Qt::SolidLine)
		dashVals = getDashString(sl->Dash, 1);
	if (!dashVals.isEmpty())
		parentElem.setAttribute("StrokeDashArray", dashVals);
	if (sl->Color != CommonStrings::None)
		parentElem.setAttribute("Stroke", setColor(sl->Color, sl->Shade, 0));
}

void XPSExPlug::getStrokeStyle(PageItem *item, QDomElement &parentElem, QDomElement &rel_root, double xOffset, double yOffset, bool forArrow)
{
	parentElem.setAttribute("StrokeThickness", FToStr(item->lineWidth() * conversionFactor));
	switch (item->PLineEnd)
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
	switch (item->PLineJoin)
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
	QString dashVals;
	if (item->DashValues.count() != 0)
	{
		for (auto it = item->DashValues.cbegin(); it != item->DashValues.cend(); ++it)
			dashVals += FToStr((*it) / item->lineWidth()) + " ";
	}
	else
	{
		if (item->PLineArt != Qt::SolidLine)
			dashVals = getDashString(item->PLineArt, 1);
	}
	if (!dashVals.isEmpty())
	{
		parentElem.setAttribute("StrokeDashArray", dashVals);
		if (item->DashValues.count() != 0)
			parentElem.setAttribute("StrokeDashOffset", FToStr(item->DashOffset));
	}
	if (item->GrTypeStroke == 0)
	{
		if (item->lineColor() != CommonStrings::None)
		{
			if (forArrow)
				parentElem.setAttribute("Fill", setColor(item->lineColor(), item->lineShade(), item->lineTransparency()));
			else
				parentElem.setAttribute("Stroke", setColor(item->lineColor(), item->lineShade(), item->lineTransparency()));
			return;
		}
	}
	else
	{
		if ((!item->strokePattern().isEmpty()) && (!item->patternStrokePath))
		{
			ScPattern pa = m_Doc->docPatterns[item->strokePattern()];
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
			ScStrokePatternTransform strokePatTrans = item->strokePatternTransform();
			double lw2 = item->lineWidth() / 2.0;
			gr.setAttribute("Viewport", QString("%1, %2, %3, %4").arg((xOffset + strokePatTrans.offsetX - lw2) * conversionFactor).arg((yOffset + strokePatTrans.offsetY - lw2) * conversionFactor).arg((pa.width * strokePatTrans.scaleX) * conversionFactor).arg((pa.height * strokePatTrans.scaleY) * conversionFactor));
			bool mirrorX, mirrorY;
			item->strokePatternFlip(mirrorX, mirrorY);
			if ((strokePatTrans.rotation != 0) || (strokePatTrans.skewX != 0) || (strokePatTrans.skewY != 0) || mirrorX || mirrorY)
			{
				QTransform mpa;
				mpa.rotate(strokePatTrans.rotation);
				mpa.shear(-strokePatTrans.skewX, strokePatTrans.skewY);
				mpa.scale(pa.scaleX, pa.scaleY);
				if (mirrorX)
					mpa.scale(-1, 1);
				if (mirrorY)
					mpa.scale(1, -1);
				gr.setAttribute("Transform", MatrixToStr(mpa));
			}
			if (item->lineTransparency() != 0)
				gr.setAttribute("Opacity", FToStr(1.0 - item->lineTransparency()));
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
		else if ((item->GrTypeStroke == Gradient_Linear) || (item->GrTypeStroke == Gradient_Radial))
		{
			QDomElement ob;
			if (forArrow)
				ob = p_docu.createElement("Path.Fill");
			else
				ob = p_docu.createElement("Path.Stroke");
			QDomElement gr;
			double GrStartX = (item->GrStrokeStartX + xOffset) * conversionFactor;
			double GrStartY = (item->GrStrokeStartY + yOffset) * conversionFactor;
			double GrFocalX = (item->GrStrokeFocalX + xOffset) * conversionFactor;
			double GrFocalY = (item->GrStrokeFocalY + yOffset) * conversionFactor;
			double GrEndX = (item->GrStrokeEndX + xOffset) * conversionFactor;
			double GrEndY = (item->GrStrokeEndY + yOffset) * conversionFactor;
			if (item->GrTypeStroke == Gradient_Linear)
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
			if (item->GrStrokeSkew == 90)
				gradientSkew = 1;
			else if (item->GrStrokeSkew == 180)
				gradientSkew = 0;
			else if (item->GrStrokeSkew == 270)
				gradientSkew = -1;
			else if (item->GrStrokeSkew == 390)
				gradientSkew = 0;
			else
				gradientSkew = tan(M_PI / 180.0 * item->GrStrokeSkew);
			QTransform qmatrix;
			if (item->GrTypeStroke == Gradient_Linear)
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
				qmatrix.translate(0, GrStartY * (1.0 - item->GrStrokeScale));
				qmatrix.translate(-GrStartX, -GrStartY);
				qmatrix.scale(1, item->GrStrokeScale);
			}
			gr.setAttribute("Transform", MatrixToStr(qmatrix));
			if (item->lineTransparency() != 0)
				gr.setAttribute("Opacity", FToStr(1.0 - item->lineTransparency()));
			QDomElement grs;
			if (item->GrTypeStroke == Gradient_Linear)
				grs = p_docu.createElement("LinearGradientBrush.GradientStops");
			else
				grs = p_docu.createElement("RadialGradientBrush.GradientStops");
			bool   isFirst = true;
			double actualStop = 0.0, lastStop = 0.0;
			QList<VColorStop*> cstops = item->stroke_gradient.colorStops();
			for (int cst = 0; cst < item->stroke_gradient.stops(); ++cst)
			{
				actualStop = cstops.at(cst)->rampPoint;
				if ((actualStop != lastStop) || (isFirst))
				{
					QDomElement itcl = p_docu.createElement("GradientStop");
					itcl.setAttribute("Offset", FToStr(cstops.at(cst)->rampPoint));
					itcl.setAttribute("Color", setColor(cstops.at(cst)->name, cstops.at(cst)->shade, 1.0 - cstops.at(cst)->opacity));
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

void XPSExPlug::getFillStyle(PageItem *item, QDomElement &parentElem, QDomElement &rel_root, double xOffset, double yOffset, bool withTransparency)
{
	if (item->GrType == 0)
	{
		if (item->fillColor() != CommonStrings::None)
		{
			if ((withTransparency) || (item->GrMask == 0))
				parentElem.setAttribute("Fill", setColor(item->fillColor(), item->fillShade(), item->fillTransparency()));
			else
				parentElem.setAttribute("Fill", setColor(item->fillColor(), item->fillShade(), 0));
		}
		return;
	}

	if ((item->GrType == Gradient_Linear) || (item->GrType == Gradient_Radial))
	{
		QDomElement ob = p_docu.createElement("Path.Fill");
		QDomElement gr;
		double GrStartX = (item->GrStartX + xOffset) * conversionFactor;
		double GrStartY = (item->GrStartY + yOffset) * conversionFactor;
		double GrFocalX = (item->GrFocalX + xOffset) * conversionFactor;
		double GrFocalY = (item->GrFocalY + yOffset) * conversionFactor;
		double GrEndX = (item->GrEndX + xOffset) * conversionFactor;
		double GrEndY = (item->GrEndY + yOffset) * conversionFactor;
		if (item->GrType == Gradient_Linear)
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
		if (item->GrSkew == 90)
			gradientSkew = 1;
		else if (item->GrSkew == 180)
			gradientSkew = 0;
		else if (item->GrSkew == 270)
			gradientSkew = -1;
		else if (item->GrSkew == 390)
			gradientSkew = 0;
		else
			gradientSkew = tan(M_PI / 180.0 * item->GrSkew);
		QTransform qmatrix;
		if (item->GrType == Gradient_Linear)
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
			qmatrix.translate(0, GrStartY * (1.0 - item->GrScale));
			qmatrix.translate(-GrStartX, -GrStartY);
			qmatrix.scale(1, item->GrScale);
		}
		gr.setAttribute("Transform", MatrixToStr(qmatrix));
		if ((item->fillTransparency() != 0) && ((withTransparency) || (item->GrMask == 0)))
			gr.setAttribute("Opacity", FToStr(1.0 - item->fillTransparency()));
		QDomElement grs;
		if (item->GrType == Gradient_Linear)
			grs = p_docu.createElement("LinearGradientBrush.GradientStops");
		else
			grs = p_docu.createElement("RadialGradientBrush.GradientStops");
		bool   isFirst = true;
		double actualStop = 0.0, lastStop = 0.0;
		QList<VColorStop*> cstops = item->fill_gradient.colorStops();
		for (int cst = 0; cst < item->fill_gradient.stops(); ++cst)
		{
			actualStop = cstops.at(cst)->rampPoint;
			if ((actualStop != lastStop) || (isFirst))
			{
				QDomElement itcl = p_docu.createElement("GradientStop");
				itcl.setAttribute("Offset", FToStr(cstops.at(cst)->rampPoint));
				itcl.setAttribute("Color", setColor(cstops.at(cst)->name, cstops.at(cst)->shade, 1.0 - cstops.at(cst)->opacity));
				grs.appendChild(itcl);
				lastStop = actualStop;
				isFirst  = false;
			}
		}
		gr.appendChild(grs);
		ob.appendChild(gr);
		parentElem.appendChild(ob);
	}
	else if (item->GrType == 8)
	{
		ScPattern pa = m_Doc->docPatterns[item->pattern()];
		QDomElement ob = p_docu.createElement("Path.Fill");
		QDomElement gr = p_docu.createElement("VisualBrush");
		gr.setAttribute("TileMode", "Tile");
		gr.setAttribute("ViewboxUnits", "Absolute");
		gr.setAttribute("ViewportUnits", "Absolute");
		gr.setAttribute("Viewbox", QString("0, 0, %1, %2").arg(pa.width * conversionFactor).arg(pa.height * conversionFactor));
		ScPatternTransform patternTrans = item->patternTransform();
		gr.setAttribute("Viewport", QString("%1, %2, %3, %4").arg((xOffset + patternTrans.offsetX) * conversionFactor).arg((yOffset + patternTrans.offsetY) * conversionFactor).arg((pa.width * patternTrans.scaleX) * conversionFactor).arg((pa.height * patternTrans.scaleY) * conversionFactor));
		bool mirrorX, mirrorY;
		item->patternFlip(mirrorX, mirrorY);
		if ((patternTrans.rotation != 0) || (patternTrans.skewX != 0) || (patternTrans.skewY != 0) || mirrorX || mirrorY)
		{
			QTransform mpa;
			mpa.rotate(patternTrans.rotation);
			mpa.shear(-patternTrans.skewX, patternTrans.skewY);
			mpa.scale(pa.scaleX, pa.scaleY);
			if (mirrorX)
				mpa.scale(-1, 1);
			if (mirrorY)
				mpa.scale(1, -1);
			gr.setAttribute("Transform", MatrixToStr(mpa));
		}
		if ((item->fillTransparency() != 0) && ((withTransparency) || (item->GrMask == 0)))
			gr.setAttribute("Opacity", FToStr(1.0 - item->fillTransparency()));
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

void XPSExPlug::handleMask(int type, PageItem *item, QDomElement &parentElem, QDomElement &rel_root, double xOffset, double yOffset)
{
	QDomElement ob;
	if (type == 1)
		ob = p_docu.createElement("Canvas.OpacityMask");
	else if (type == 2)
		ob = p_docu.createElement("Glyph.OpacityMask");
	else
		ob = p_docu.createElement("Path.OpacityMask");
	if ((item->GrMask == GradMask_Linear) || (item->GrMask == GradMask_Radial))
	{
		QDomElement gr;
		double GrStartX = (item->GrMaskStartX + xOffset) * conversionFactor;
		double GrStartY = (item->GrMaskStartY + yOffset) * conversionFactor;
		double GrFocalX = (item->GrMaskFocalX + xOffset) * conversionFactor;
		double GrFocalY = (item->GrMaskFocalY + yOffset) * conversionFactor;
		double GrEndX = (item->GrMaskEndX + xOffset) * conversionFactor;
		double GrEndY = (item->GrMaskEndY + yOffset) * conversionFactor;
		if ((item->GrMask == GradMask_Linear) || (item->GrMask == GradMask_LinearLumAlpha))
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
		if (item->GrMaskSkew == 90)
			gradientSkew = 1;
		else if (item->GrMaskSkew == 180)
			gradientSkew = 0;
		else if (item->GrMaskSkew == 270)
			gradientSkew = -1;
		else if (item->GrMaskSkew == 390)
			gradientSkew = 0;
		else
			gradientSkew = tan(M_PI / 180.0 * item->GrMaskSkew);
		QTransform qmatrix;
		if (item->GrMask == GradMask_Linear)
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
			qmatrix.translate(0, GrStartY * (1.0 - item->GrMaskScale));
			qmatrix.translate(-GrStartX, -GrStartY);
			qmatrix.scale(1, item->GrMaskScale);
		}
		gr.setAttribute("Transform", MatrixToStr(qmatrix));
		QDomElement grs;
		if (item->GrMask == GradMask_Linear)
			grs = p_docu.createElement("LinearGradientBrush.GradientStops");
		else
			grs = p_docu.createElement("RadialGradientBrush.GradientStops");
		bool   isFirst = true;
		double actualStop = 0.0, lastStop = 0.0;
		QList<VColorStop*> cstops = item->mask_gradient.colorStops();
		for (int cst = 0; cst < item->mask_gradient.stops(); ++cst)
		{
			actualStop = cstops.at(cst)->rampPoint;
			if ((actualStop != lastStop) || (isFirst))
			{
				QDomElement itcl = p_docu.createElement("GradientStop");
				itcl.setAttribute("Offset", FToStr(cstops.at(cst)->rampPoint));
				itcl.setAttribute("Color", setColor(cstops.at(cst)->name, cstops.at(cst)->shade, 1.0 - cstops.at(cst)->opacity));
				grs.appendChild(itcl);
				lastStop = actualStop;
				isFirst  = false;
			}
		}
		gr.appendChild(grs);
		ob.appendChild(gr);
	}
	else if (item->GrMask == GradMask_Pattern)
	{
		ScPattern pa = m_Doc->docPatterns[item->patternMask()];
		QDomElement gr = p_docu.createElement("VisualBrush");
		gr.setAttribute("TileMode", "Tile");
		gr.setAttribute("ViewboxUnits", "Absolute");
		gr.setAttribute("ViewportUnits", "Absolute");
		gr.setAttribute("Viewbox", QString("0, 0, %1, %2").arg(pa.width * conversionFactor).arg(pa.height * conversionFactor));
		ScMaskTransform maskTrans = item->maskTransform();
		gr.setAttribute("Viewport", QString("%1, %2, %3, %4").arg((xOffset + maskTrans.offsetX) * conversionFactor).arg((yOffset + maskTrans.offsetY) * conversionFactor).arg((pa.width * maskTrans.scaleX) * conversionFactor).arg((pa.height * maskTrans.scaleY) * conversionFactor));
		bool mirrorX, mirrorY;
		item->maskFlip(mirrorX, mirrorY);
		if ((maskTrans.rotation != 0) || (maskTrans.skewX != 0) || (maskTrans.skewY != 0) || mirrorX || mirrorY)
		{
			QTransform mpa;
			mpa.rotate(maskTrans.rotation);
			mpa.shear(-maskTrans.skewX, maskTrans.skewY);
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

QString XPSExPlug::setColor(const QString& farbe, int shad, double transparency)
{
	if (farbe == CommonStrings::None)
		return "#00FFFFFF";
	const ScColor& col = m_Doc->PageColors[farbe];
	QString color =  ScColorEngine::getShadeColorProof(col, m_Doc, shad).name().mid(1);
	color = color.toUpper();
	QString alpha;
	alpha.setNum(qRound((1.0 - transparency) * 255), 16);
	alpha = alpha.toUpper();
	if (alpha.length() == 1)
		alpha.prepend("0");
	return "#" + alpha + color;
}

void XPSExPlug::setClipAttr(QDomElement &elem, FPointArray *ite, bool fillRule)
{
	QString pathStr = setClipPath(ite, true);
	if (pathStr.length() > 0)
	{
		if (fillRule)
			pathStr.prepend("F 0 ");
		else
			pathStr.prepend("F 1 ");
		elem.setAttribute("Clip", pathStr);
	}
}

QString XPSExPlug::setClipPath(FPointArray *ite, bool closed)
{
	QString tmp;
	FPoint np, np1, np2, np3, np4, firstP;
	bool nPath = true;
	bool first = true;
	if (ite->size() <= 3)
		return tmp;

	for (int poi=0; poi<ite->size()-3; poi += 4)
	{
		if (ite->isMarker(poi))
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

QString XPSExPlug::MatrixToStr(QTransform &mat, double factor)
{
	QString cc("%1, %2, %3, %4, %5, %6");
	return  cc.arg(mat.m11()).arg(mat.m12()).arg(mat.m21()).arg(mat.m22()).arg(mat.dx() * factor).arg(mat.dy() * factor);
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

bool XPSExPlug::checkForFallback(PageItem *item)
{
	bool ret = false;
	int GrType = item->GrType;
	int GrMask = item->GrMask;
	if ((GrType == Gradient_4Colors) || (GrType == Gradient_Diamond) || (GrType == Gradient_Mesh) || (GrType == Gradient_PatchMesh) || (GrType == Gradient_Conical))
		ret = true;
	if ((GrMask == GradMask_LinearLumAlpha) || (GrMask == GradMask_RadialLumAlpha) || (GrMask == GradMask_PatternLumAlpha) || (GrMask == GradMask_PatternLumAlphaInverted) || (GrMask == GradMask_PatternInverted))
		ret = true;
	if (item->fillBlendmode() != 0)
		ret = true;
	if (item->lineBlendmode() != 0)
		ret = true;
	if (item->hasSoftShadow())
		ret = true;
	return ret;
}

XPSExPlug::~XPSExPlug()
{
}
