/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          svgexplugin.cpp  -  description
                             -------------------
    begin                : Sun Aug 3 08:00:00 CEST 2002
    copyright            : (C) 2002 by Franz Schmid
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

#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <QByteArray>
#include <QBuffer>
#include <QList>
#include <QCheckBox>

#include "svgexplugin.h"

#include "scconfig.h"
#include "canvas.h"
#include "cmsettings.h"
#include "commonstrings.h"
#include "ui/customfdialog.h"
#include "scribuscore.h"
#include "page.h"
#include "scgzfile.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "ui/scmessagebox.h"
#include "scpattern.h"
#include "util.h"
#include "ui/customfdialog.h"
#include "sctextstruct.h"
#include "ui/guidemanager.h"
#include "sccolorengine.h"
#include "util_formats.h"

int svgexplugin_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* svgexplugin_getPlugin()
{
	SVGExportPlugin* plug = new SVGExportPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void svgexplugin_freePlugin(ScPlugin* plugin)
{
	SVGExportPlugin* plug = dynamic_cast<SVGExportPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

SVGExportPlugin::SVGExportPlugin() : ScActionPlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

SVGExportPlugin::~SVGExportPlugin() {};

void SVGExportPlugin::languageChange()
{
	// Note that we leave the unused members unset. They'll be initialised
	// with their default ctors during construction.
	// Action name
	m_actionInfo.name = "ExportAsSVG";
	// Action text for menu, including accel
	m_actionInfo.text = tr("Save as &SVG...");
	// Menu
	m_actionInfo.menu = "FileExport";
	m_actionInfo.enabledOnStartup = false;
	m_actionInfo.needsNumObjects = -1;
}

const QString SVGExportPlugin::fullTrName() const
{
	return QObject::tr("SVG Export");
}

const ScActionPlugin::AboutData* SVGExportPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Franz Schmid <franz@scribus.info>";
	about->shortDescription = tr("Exports SVG Files");
	about->description = tr("Exports the current page into an SVG file.");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void SVGExportPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

bool SVGExportPlugin::run(ScribusDoc* doc, QString filename)
{
	Q_ASSERT(filename.isEmpty());
	QString fileName;
	if (doc!=0)
	{
		PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("svgex");
		QString wdir = prefs->get("wdir", ".");
		CustomFDialog *openDia = new CustomFDialog(doc->scMW(), wdir, QObject::tr("Save as"), QObject::tr("%1;;All Files (*)").arg(FormatsManager::instance()->extensionsForFormat(FormatsManager::SVG)), fdHidePreviewCheckBox);
		openDia->setSelection(getFileNameByPage(doc, doc->currentPage()->pageNr(), "svg"));
		openDia->setExtension("svg");
		openDia->setZipExtension("svgz");
		QCheckBox* compress = new QCheckBox(openDia);
		compress->setText( tr("Compress File"));
		compress->setChecked(false);
		openDia->addWidgets(compress);
		QCheckBox* inlineImages = new QCheckBox(openDia);
		inlineImages->setText( tr("Save Images inline"));
		inlineImages->setToolTip( tr("Adds all Images on the Page inline to the SVG.\nCaution: this will increase the file size!"));
		inlineImages->setChecked(true);
		openDia->addWidgets(inlineImages);
		QCheckBox* exportBack = new QCheckBox(openDia);
		exportBack->setText( tr("Export Page background"));
		exportBack->setToolTip( tr("Adds the Page itself as background to the SVG."));
		exportBack->setChecked(false);
		openDia->addWidgets(exportBack);
		if (openDia->exec())
		{
			fileName = openDia->selectedFile();
			QFileInfo fi(fileName);
			QString baseDir = fi.absolutePath();
			if (compress->isChecked())
				fileName = baseDir + "/" + fi.baseName() + ".svgz";
			else
				fileName = baseDir + "/" + fi.baseName() + ".svg";
		}
		else
			return true;
		SVGOptions Options;
		Options.inlineImages = inlineImages->isChecked();
		Options.exportPageBackground = exportBack->isChecked();
		Options.compressFile = compress->isChecked();
		delete openDia;

		if (!fileName.isEmpty())
		{
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
			SVGExPlug *dia = new SVGExPlug(doc);
			dia->doExport(fileName, Options);
			delete dia;
		}
		else
			return true;
	}
	return true;
}

SVGExPlug::SVGExPlug( ScribusDoc* doc )
{
	m_Doc = doc;
	Options.inlineImages = true;
	Options.exportPageBackground = false;
	Options.compressFile = false;
	glyphNames.clear();
}

bool SVGExPlug::doExport( QString fName, SVGOptions &Opts )
{
	Options = Opts;
	QFileInfo fiBase(fName);
	baseDir = fiBase.absolutePath();
	Page *Seite;
	GradCount = 0;
	ClipCount = 0;
	PattCount = 0;
	docu = QDomDocument("svgdoc");
	QString vo = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	QString st = "<svg></svg>";
	docu.setContent(st);
	Seite = m_Doc->currentPage();
	double pageWidth  = Seite->width();
	double pageHeight = Seite->height();
	docElement = docu.documentElement();
	docElement.setAttribute("width", FToStr(pageWidth)+"pt");
	docElement.setAttribute("height", FToStr(pageHeight)+"pt");
	docElement.setAttribute("viewBox", QString("0 0 %1 %2").arg(pageWidth).arg(pageHeight));
	docElement.setAttribute("xmlns", "http://www.w3.org/2000/svg");
	docElement.setAttribute("xmlns:xlink","http://www.w3.org/1999/xlink");
	docElement.setAttribute("version","1.1");
	if (!m_Doc->documentInfo.getTitle().isEmpty())
	{
		QDomText title = docu.createTextNode(m_Doc->documentInfo.getTitle());
		QDomElement titleElem = docu.createElement("title");
		titleElem.appendChild(title);
		docElement.appendChild(titleElem);
	}
	if (!m_Doc->documentInfo.getComments().isEmpty())
	{
		QDomText desc = docu.createTextNode(m_Doc->documentInfo.getComments());
		QDomElement descElem = docu.createElement("desc");
		descElem.appendChild(desc);
		docElement.appendChild(descElem);
	}
	globalDefs = docu.createElement("defs");
	docElement.appendChild(globalDefs);
	if (Options.exportPageBackground)
	{
		QDomElement backG = docu.createElement("rect");
		backG.setAttribute("x", "0");
		backG.setAttribute("y", "0");
		backG.setAttribute("width", FToStr(pageWidth));
		backG.setAttribute("height", FToStr(pageHeight));
		backG.setAttribute("style", "fill:"+m_Doc->papColor.name()+";" + "stroke:none;");
		docElement.appendChild(backG);
	}
	Seite = m_Doc->MasterPages.at(m_Doc->MasterNames[m_Doc->currentPage()->MPageNam]);
	ProcessPage(Seite);
	Seite = m_Doc->currentPage();
	ProcessPage(Seite);
	if(Options.compressFile)
	{
		// zipped saving
		QByteArray array(docu.toString().toUtf8());
		if (!ScGzFile::writeToFile(fName, array, vo.toUtf8().data()))
			return false;
	}
	else
	{
		QFile f(fName);
		if(!f.open(QIODevice::WriteOnly))
			return false;
		QDataStream s(&f);
		QString wr = vo;
		wr += docu.toString();
		QByteArray utf8wr = wr.toUtf8();
		s.writeRawData(utf8wr.data(), utf8wr.length());
		f.close();
	}
	return true;
}

void SVGExPlug::ProcessPage(Page *Seite)
{
	int Lnr = 0;
	ScLayer ll;
	ll.isPrintable = false;
	ll.LNr = 0;
	QDomElement layerGroup;
	PageItem *Item;
	QList<PageItem*> Items;
	QStack<PageItem*> groupStack;
	QStack<QDomElement> groupStack2;
	Page* SavedAct = m_Doc->currentPage();
	if (Seite->pageName().isEmpty())
		Items = m_Doc->DocItems;
	else
		Items = m_Doc->MasterItems;
	if (Items.count() == 0)
		return;
	m_Doc->setCurrentPage(Seite);
	for (int la = 0; la < m_Doc->Layers.count(); la++)
	{
		m_Doc->Layers.levelToLayer(ll, Lnr);
		if (ll.isPrintable)
		{
			layerGroup = docu.createElement("g");
			layerGroup.setAttribute("id", ll.Name);
			if (ll.transparency != 1.0)
				layerGroup.setAttribute("opacity", FToStr(ll.transparency));
			for(int j = 0; j < Items.count(); ++j)
			{
				Item = Items.at(j);
				if (Item->LayerNr != ll.LNr)
					continue;
				if (!Item->printEnabled())
					continue;
				double x = Seite->xOffset();
				double y = Seite->yOffset();
				double w = Seite->width();
				double h = Seite->height();
				double x2 = Item->BoundingX;
				double y2 = Item->BoundingY;
				double w2 = Item->BoundingW;
				double h2 = Item->BoundingH;
				if (!( qMax( x, x2 ) <= qMin( x+w, x2+w2 ) && qMax( y, y2 ) <= qMin( y+h, y2+h2 )))
					continue;
				if (Item->isGroupControl)
				{
					groupStack.push(Item->groupsLastItem);
					groupStack2.push(layerGroup);
					layerGroup = docu.createElement("g");
					if (!Item->AutoName)
						layerGroup.setAttribute("id", Item->itemName());
					if (Item->fillTransparency() != 0)
						layerGroup.setAttribute("opacity", FToStr(1.0 - Item->fillTransparency()));
					QDomElement ob = docu.createElement("clipPath");
					ob.setAttribute("id", "Clip"+IToStr(ClipCount));
					QDomElement cl = docu.createElement("path");
					cl.setAttribute("d", SetClipPath(&Item->PoLine, true));
					QString trans = "translate("+FToStr(Item->xPos()-Seite->xOffset())+", "+FToStr(Item->yPos()-Seite->yOffset())+")";
					if (Item->rotation() != 0)
						trans += " rotate("+FToStr(Item->rotation())+")";
					cl.setAttribute("transform", trans);
					ob.appendChild(cl);
					globalDefs.appendChild(ob);
					layerGroup.setAttribute("clip-path", "url(#Clip"+IToStr(ClipCount)+")");
					ClipCount++;
					continue;
				}
				ProcessItemOnPage(Item->xPos()-Seite->xOffset(), Item->yPos()-Seite->yOffset(), Item, &layerGroup);
				if (groupStack.count() != 0)
				{
					while (Item == groupStack.top())
					{
						groupStack.pop();
						groupStack2.top().appendChild(layerGroup);
						layerGroup = groupStack2.pop();
						if (groupStack.count() == 0)
							break;
					}
				}
			}
			for(int j = 0; j < Items.count(); ++j)
			{
				Item = Items.at(j);
				if (Item->LayerNr != ll.LNr)
					continue;
				if (!Item->printEnabled())
					continue;
				double x = Seite->xOffset();
				double y = Seite->yOffset();
				double w = Seite->width();
				double h = Seite->height();
				double x2 = Item->BoundingX;
				double y2 = Item->BoundingY;
				double w2 = Item->BoundingW;
				double h2 = Item->BoundingH;
				if (!( qMax( x, x2 ) <= qMin( x+w, x2+w2 ) && qMax( y, y2 ) <= qMin( y+h, y2+h2 )))
					continue;
				if (!Item->isTableItem)
					continue;
				if ((Item->lineColor() == CommonStrings::None) || (Item->lineWidth() == 0.0))
					continue;
				if ((Item->TopLine) || (Item->RightLine) || (Item->BottomLine) || (Item->LeftLine))
				{
					QString trans = "translate("+FToStr(Item->xPos()-Seite->xOffset())+", "+FToStr(Item->yPos()-Seite->yOffset())+")";
					if (Item->rotation() != 0)
						trans += " rotate("+FToStr(Item->rotation())+")";
					QString stroke = getStrokeStyle(Item);
					QDomElement ob = docu.createElement("path");
					ob.setAttribute("transform", trans);
					ob.setAttribute("style", "fill:none; " + stroke);
					QString pathAttr = "";
					if (Item->TopLine)
						pathAttr += "M 0 0 L "+FToStr(Item->width())+" 0";
					if (Item->RightLine)
						pathAttr += " M " + FToStr(Item->width()) + "0 L "+FToStr(Item->width())+" "+FToStr(Item->height());
					if (Item->BottomLine)
						pathAttr += " M 0 " + FToStr(Item->height()) + " L "+FToStr(Item->width())+" "+FToStr(Item->height());
					if (Item->LeftLine)
						pathAttr += " M 0 0 L 0 "+FToStr(Item->height());
					ob.setAttribute("d", pathAttr);
					layerGroup.appendChild(ob);
				}
			}
			docElement.appendChild(layerGroup);
		}
		Lnr++;
	}
	m_Doc->setCurrentPage(SavedAct);
}

void SVGExPlug::ProcessItemOnPage(double xOffset, double yOffset, PageItem *Item, QDomElement *parentElem)
{
	QDomElement ob;
	QString trans = "translate("+FToStr(xOffset)+", "+FToStr(yOffset)+")";
	if (Item->rotation() != 0)
		trans += " rotate("+FToStr(Item->rotation())+")";
	QString fill = getFillStyle(Item);
	QString stroke = "stroke:none";
	if (!Item->isTableItem)
		stroke = getStrokeStyle(Item);
	switch (Item->itemType())
	{
		case PageItem::Polygon:
		case PageItem::PolyLine:
			ob = processPolyItem(Item, trans, fill, stroke);
			if ((Item->lineColor() != CommonStrings::None) && ((Item->startArrowIndex() != 0) || (Item->endArrowIndex() != 0)))
				ob = processArrows(Item, ob, trans);
			break;
		case PageItem::Line:
			ob = processLineItem(Item, trans, stroke);
			if ((Item->lineColor() != CommonStrings::None) && ((Item->startArrowIndex() != 0) || (Item->endArrowIndex() != 0)))
				ob = processArrows(Item, ob, trans);
			break;
		case PageItem::ImageFrame:
		case PageItem::LatexFrame:
			ob = processImageItem(Item, trans, fill, stroke);
			break;
		case PageItem::TextFrame:
			ob = processTextItem(Item, trans, fill, stroke);
			break;
		case PageItem::PathText:
			ob = processPathTextItem(Item, trans, stroke);
			break;
		default:
			break;
	}
	if (!Item->AutoName)
		ob.setAttribute("id", Item->itemName());
	parentElem->appendChild(ob);
}

QDomElement SVGExPlug::processPolyItem(PageItem *Item, QString trans, QString fill, QString stroke)
{
	bool closedPath;
	QDomElement ob;
	if (Item->itemType() == PageItem::Polygon)
		closedPath = true;
	else
		closedPath = false;
	if (Item->NamedLStyle.isEmpty())
	{
		ob = docu.createElement("path");
		ob.setAttribute("d", SetClipPath(&Item->PoLine, closedPath));
		ob.setAttribute("transform", trans);
		ob.setAttribute("style", fill + stroke);
	}
	else
	{
		ob = docu.createElement("g");
		ob.setAttribute("transform", trans);
		QDomElement ob2 = docu.createElement("path");
		ob2.setAttribute("d", SetClipPath(&Item->PoLine, closedPath));
		ob2.setAttribute("style", fill);
		ob.appendChild(ob2);
		multiLine ml = m_Doc->MLineStyles[Item->NamedLStyle];
		for (int it = ml.size()-1; it > -1; it--)
		{
			if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
			{
				QDomElement ob3 = docu.createElement("path");
				ob3.setAttribute("d", SetClipPath(&Item->PoLine, closedPath));
				ob3.setAttribute("style", GetMultiStroke(&ml[it], Item));
				ob.appendChild(ob3);
			}
		}
	}
	return ob;
}

QDomElement SVGExPlug::processLineItem(PageItem *Item, QString trans, QString stroke)
{
	QDomElement ob;
	if (Item->NamedLStyle.isEmpty())
	{
		ob = docu.createElement("path");
		ob.setAttribute("d", "M 0 0 L "+FToStr(Item->width())+" 0");
		ob.setAttribute("transform", trans);
		ob.setAttribute("style", stroke);
	}
	else
	{
		ob = docu.createElement("g");
		ob.setAttribute("transform", trans);
		multiLine ml = m_Doc->MLineStyles[Item->NamedLStyle];
		for (int it = ml.size()-1; it > -1; it--)
		{
			if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
			{
				QDomElement ob2 = docu.createElement("path");
				ob2.setAttribute("d", "M 0 0 L "+FToStr(Item->width())+" 0");
				ob2.setAttribute("style", GetMultiStroke(&ml[it], Item));
				ob.appendChild(ob2);
			}
		}
	}
	return ob;
}

QDomElement SVGExPlug::processImageItem(PageItem *Item, QString trans, QString fill, QString stroke)
{
	QDomElement ob;
	ob = docu.createElement("g");
	ob.setAttribute("transform", trans);
	if ((Item->fillColor() != CommonStrings::None) || (Item->GrType != 0))
	{
		QDomElement ob1 = docu.createElement("path");
		ob1.setAttribute("d", SetClipPath(&Item->PoLine, true));
		ob1.setAttribute("style", fill);
		ob.appendChild(ob1);
	}
	if ((Item->PictureIsAvailable) && (!Item->Pfile.isEmpty()))
	{
		QDomElement ob2 = docu.createElement("clipPath");
		ob2.setAttribute("id", "Clip"+IToStr(ClipCount));
		ob2.setAttribute("clipPathUnits", "userSpaceOnUse");
		ob2.setAttribute("clip-rule", "evenodd");
		QDomElement cl = docu.createElement("path");
		if (Item->imageClip.size() != 0)
			cl.setAttribute("d", SetClipPath(&Item->imageClip, true));
		else
			cl.setAttribute("d", SetClipPath(&Item->PoLine, true));
		ob2.appendChild(cl);
		globalDefs.appendChild(ob2);
		QDomElement ob3 = docu.createElement("image");
		ob3.setAttribute("clip-path", "url(#Clip"+IToStr(ClipCount)+")");
		ScImage img;
		CMSettings cms(m_Doc, Item->IProfile, Item->IRender);
		img.LoadPicture(Item->Pfile, Item->pixm.imgInfo.actualPageNumber, cms, Item->UseEmbedded, true, ScImage::RGBProof, 72);
		img.applyEffect(Item->effectsInUse, m_Doc->PageColors, true);
		if (Options.inlineImages)
		{
			QBuffer buffer;
			buffer.open(QIODevice::WriteOnly);
			img.qImage().save(&buffer, "PNG");
			QByteArray ba = buffer.buffer().toBase64();
			buffer.close();
			ob3.setAttribute("xlink:href", "data:image/png;base64,"+QString(ba));
		}
		else
		{
			QFileInfo fi = QFileInfo(Item->Pfile);
			QString imgFileName = baseDir + "/" + fi.baseName()+".png";
			QFileInfo im = QFileInfo(imgFileName);
			if (im.exists())
				imgFileName = baseDir + "/" + fi.baseName()+"_copy.png";
			img.qImage().save(imgFileName, "PNG");
			QFileInfo fi2 = QFileInfo(imgFileName);
			ob3.setAttribute("xlink:href", fi2.baseName()+".png");
		}
		ob3.setAttribute("x", FToStr(Item->imageXOffset() * Item->imageXScale()));
		ob3.setAttribute("y", FToStr(Item->imageYOffset() * Item->imageYScale()));
		ob3.setAttribute("width", FToStr(img.width() * Item->imageXScale()));
		ob3.setAttribute("height", FToStr(img.height() * Item->imageYScale()));
		QMatrix mpa;
		if (Item->imageFlippedH())
		{
			mpa.translate(Item->width(), 0);
			mpa.scale(-1, 1);
		}
		if (Item->imageFlippedV())
		{
			mpa.translate(0, Item->height());
			mpa.scale(1, -1);
		}
		ob3.setAttribute("transform", MatrixToStr(mpa));
		ClipCount++;
		ob.appendChild(ob3);
	}
	if (Item->NamedLStyle.isEmpty())
	{
		QDomElement ob4 = docu.createElement("path");
		ob4.setAttribute("d", SetClipPath(&Item->PoLine, true));
		ob4.setAttribute("style", "fill:none; "+stroke);
		ob.appendChild(ob4);
	}
	else
	{
		multiLine ml = m_Doc->MLineStyles[Item->NamedLStyle];
		for (int it = ml.size()-1; it > -1; it--)
		{
			if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
			{
				QDomElement ob5 = docu.createElement("path");
				ob5.setAttribute("d", SetClipPath(&Item->PoLine, true));
				ob5.setAttribute("style", "fill:none; "+GetMultiStroke(&ml[it], Item));
				ob.appendChild(ob5);
			}
		}
	}
	return ob;
}

QDomElement SVGExPlug::processTextItem(PageItem *Item, QString trans, QString fill, QString stroke)
{
	QDomElement ob;
	ob = docu.createElement("g");
	ob.setAttribute("transform", trans);
	if ((Item->fillColor() != CommonStrings::None) || (Item->GrType != 0))
	{
		QDomElement ob1 = docu.createElement("path");
		ob1.setAttribute("d", SetClipPath(&Item->PoLine, true));
		ob1.setAttribute("style", fill);
		ob.appendChild(ob1);
	}
	double x, y, wide;
	QString chstr;
	for (uint ll=0; ll < Item->itemText.lines(); ++ll)
	{
		LineSpec ls = Item->itemText.line(ll);
		double CurX = ls.x;
		for (int a = ls.firstItem; a <= ls.lastItem; ++a)
		{
			x = 0.0;
			y = 0.0;
			ScText * hl = Item->itemText.item(a);
			const CharStyle& charStyle(Item->itemText.charStyle(a));
			chstr = Item->itemText.text(a,1);
			if ((chstr == QChar(13)) || (chstr == QChar(29)))
			{
				if (chstr == QChar(29))
					CurX += hl->glyph.wide();
				continue;
			}
			if (chstr == QChar(30))
			{
				chstr = Item->ExpandToken(a);
				if (chstr == QChar(32))
				{
					CurX += hl->glyph.wide();
					continue;
				}
			}
			double chs = charStyle.fontSize();
			if (hl->effects() & ScStyle_SmallCaps)
			{
				if (chstr.toUpper() != chstr)
				{
					chs = qMax(static_cast<int>(hl->fontSize() * m_Doc->typographicSettings.valueSmallCaps / 100), 1);
					chstr = chstr.toUpper();
				}
			}
			else if (hl->effects() & ScStyle_AllCaps)
				chstr = chstr.toUpper();
			uint chr = chstr[0].unicode();
			QMatrix chma, chma2, chma3, chma4, chma6;
			QMatrix trafo = QMatrix( 1, 0, 0, 1, CurX, ls.y );
			if (Item->rotation() != 0)
			{
				QMatrix sca;
				sca.translate(-Item->xPos(), -Item->yPos());
				trafo *= sca;
			}
			chma.scale(hl->glyph.scaleH * charStyle.fontSize() / 100.00, hl->glyph.scaleV * charStyle.fontSize() / 100.0);
			if (Item->reversed())
			{
				if (a < Item->itemText.length()-1)
					wide = hl->font().charWidth(chstr[0], hl->fontSize(), Item->itemText.text(a+1));
				else
					wide = hl->font().charWidth(chstr[0], hl->fontSize());
				chma3.scale(-1, 1);
				chma3.translate(-wide, 0);
			}
			chma4.translate(0, Item->BaseOffs - (charStyle.fontSize() / 10.0) * hl->glyph.scaleV);
			if (charStyle.effects() & (ScStyle_Subscript | ScStyle_Superscript))
				chma6.translate(0, hl->glyph.yoffset);
			if (hl->baselineOffset() != 0)
				chma6.translate(0, (-charStyle.fontSize() / 10.0) * (charStyle.baselineOffset() / 1000.0));
			QMatrix finalMat = QMatrix(chma * chma2 * chma3 * chma4 * chma6 * trafo);
			if (Item->rotation() != 0)
			{
				QMatrix sca;
				sca.translate(Item->xPos(), Item->yPos());
				finalMat *= sca;
			}
			if ((hl->ch == SpecialChars::OBJECT) && (hl->embedded.hasItem()))
			{
				ob.appendChild(processInlineItem(CurX + hl->glyph.xoffset, ls.y + hl->glyph.yoffset, finalMat, hl, false, trans));
				InlineFrame& embedded(const_cast<InlineFrame&>(hl->embedded));
				CurX += (embedded.getItem()->gWidth + embedded.getItem()->lineWidth()) * hl->glyph.scaleH;
			}
			else
			{
				QString glName;
				if (chstr > QChar(32))
					glName = handleGlyph(chr, hl);
				if ((charStyle.effects() & ScStyle_Shadowed) && (charStyle.strokeColor() != CommonStrings::None) && (chstr > QChar(32)))
				{
					QMatrix sha = finalMat;
					QMatrix shad;
					shad.translate(charStyle.fontSize() * charStyle.shadowXOffset() / 10000.0, -charStyle.fontSize() * charStyle.shadowYOffset() / 10000.0);
					sha *= shad;
					QDomElement ob2 = docu.createElement("use");
					ob2.setAttribute("xlink:href", "#" + glName);
					ob2.setAttribute("transform", MatrixToStr(sha));
					ob2.setAttribute("style", "fill:"+SetColor(hl->strokeColor(), hl->strokeShade())+";" + "stroke:none;");
					ob.appendChild(ob2);
				}
				QChar chstc = hl->ch;
				if (((charStyle.effects() & ScStyle_Underline) && !SpecialChars::isBreak(chstc))
					|| ((charStyle.effects() & ScStyle_UnderlineWords) && !chstc.isSpace() && !SpecialChars::isBreak(chstc)))
				{
					x = CurX;
					y = ls.y;
					double Ulen = hl->glyph.xadvance;
					double Upos, lw, kern;
					if (charStyle.effects() & ScStyle_StartOfLine)
						kern = 0;
					else
						kern = charStyle.fontSize() * charStyle.tracking() / 10000.0;
					if ((charStyle.underlineOffset() != -1) || (charStyle.underlineWidth() != -1))
					{
						if (charStyle.underlineOffset() != -1)
							Upos = (charStyle.underlineOffset() / 1000.0) * (charStyle.font().descent(charStyle.fontSize() / 10.0));
						else
							Upos = charStyle.font().underlinePos(charStyle.fontSize() / 10.0);
						if (charStyle.underlineWidth() != -1)
							lw = (charStyle.underlineWidth() / 1000.0) * (charStyle.fontSize() / 10.0);
						else
							lw = qMax(charStyle.font().strokeWidth(charStyle.fontSize() / 10.0), 1.0);
					}
					else
					{
						Upos = charStyle.font().underlinePos(charStyle.fontSize() / 10.0);
						lw = qMax(charStyle.font().strokeWidth(charStyle.fontSize() / 10.0), 1.0);
					}
					if (charStyle.baselineOffset() != 0)
						Upos += (charStyle.fontSize() / 10.0) * (charStyle.baselineOffset() / 1000.0);
					QDomElement ob6 = docu.createElement("path");
					if (charStyle.effects() & ScStyle_Subscript)
						ob6.setAttribute("d", QString("M %1 %2 L%3 %4").arg(x + hl->glyph.xoffset-kern).arg(y + hl->glyph.yoffset - Upos).arg(x + hl->glyph.xoffset+Ulen).arg(y + hl->glyph.yoffset - Upos));
					else
						ob6.setAttribute("d", QString("M %1 %2 L%3 %4").arg(x + hl->glyph.xoffset-kern).arg(y - Upos).arg(x + hl->glyph.xoffset+Ulen).arg(y - Upos));
					QString sT = "stroke:none;";
					if (charStyle.fillColor() != CommonStrings::None)
					{
						sT = "stroke:"+SetColor(charStyle.fillColor(), charStyle.fillShade())+";";
						sT += " stroke-width:"+FToStr(lw)+";";
					}
					ob6.setAttribute("style", "fill:none;" + sT);
					ob.appendChild(ob6);
				}
				if (chstr > QChar(32))
				{
					QDomElement ob3 = docu.createElement("use");
					ob3.setAttribute("xlink:href", "#" + glName);
					ob3.setAttribute("transform", MatrixToStr(finalMat));
					QString fT = "fill:"+SetColor(hl->fillColor(), hl->fillShade())+";";
					QString sT = "stroke:none;";
					if (charStyle.effects() & ScStyle_Outline)
					{
						sT = "stroke:"+SetColor(hl->strokeColor(), hl->strokeShade())+";";
						sT += " stroke-width:"+FToStr(chs * hl->outlineWidth() / 10000.0)+";";
					}
					ob3.setAttribute("style", fT + sT);
					ob.appendChild(ob3);
				}
				if (charStyle.effects() & ScStyle_Strikethrough)
				{
					x = CurX;
					y = ls.y;
					double Ulen = hl->glyph.xadvance;
					double Upos, lw, kern;
					if (charStyle.effects() & ScStyle_StartOfLine)
						kern = 0;
					else
						kern = charStyle.fontSize() * charStyle.tracking() / 10000.0;
					if ((charStyle.strikethruOffset() != -1) || (charStyle.strikethruWidth() != -1))
					{
						if (charStyle.strikethruOffset() != -1)
							Upos = (charStyle.strikethruOffset() / 1000.0) * (charStyle.font().ascent(charStyle.fontSize() / 10.0));
						else
							Upos = charStyle.font().strikeoutPos(charStyle.fontSize() / 10.0);
						if (charStyle.strikethruWidth() != -1)
							lw = (charStyle.strikethruWidth() / 1000.0) * (charStyle.fontSize() / 10.0);
						else
							lw = qMax(charStyle.font().strokeWidth(charStyle.fontSize() / 10.0), 1.0);
					}
					else
					{
						Upos = charStyle.font().strikeoutPos(charStyle.fontSize() / 10.0);
						lw = qMax(charStyle.font().strokeWidth(charStyle.fontSize() / 10.0), 1.0);
					}
					if (charStyle.baselineOffset() != 0)
						Upos += (charStyle.fontSize() / 10.0) * hl->glyph.scaleV * (charStyle.baselineOffset() / 1000.0);
					QDomElement ob7 = docu.createElement("path");
					ob7.setAttribute("d", QString("M %1 %2 L%3 %4").arg(x + hl->glyph.xoffset-kern).arg(y + hl->glyph.yoffset - Upos).arg(x + hl->glyph.xoffset+Ulen).arg(y + hl->glyph.yoffset - Upos));
					QString sT = "stroke:none;";
					if (charStyle.fillColor() != CommonStrings::None)
					{
						sT = "stroke:"+SetColor(charStyle.fillColor(), charStyle.fillShade())+";";
						sT += " stroke-width:"+FToStr(lw)+";";
					}
					ob7.setAttribute("style", "fill:none;" + sT);
					ob.appendChild(ob7);
				}
				CurX += hl->glyph.wide();
			}
		}
	}
	if (Item->NamedLStyle.isEmpty())
	{
		QDomElement ob4 = docu.createElement("path");
		ob4.setAttribute("d", SetClipPath(&Item->PoLine, true));
		ob4.setAttribute("style", "fill:none; "+stroke);
		ob.appendChild(ob4);
	}
	else
	{
		multiLine ml = m_Doc->MLineStyles[Item->NamedLStyle];
		for (int it = ml.size()-1; it > -1; it--)
		{
			if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
			{
				QDomElement ob5 = docu.createElement("path");
				ob5.setAttribute("d", SetClipPath(&Item->PoLine, true));
				ob5.setAttribute("style", "fill:none; "+GetMultiStroke(&ml[it], Item));
				ob.appendChild(ob5);
			}
		}
	}
	return ob;
}

QDomElement SVGExPlug::processPathTextItem(PageItem *Item, QString trans, QString stroke)
{
	QDomElement ob;
	ob = docu.createElement("g");
	ob.setAttribute("transform", trans);
	if (Item->PoShow)
	{
		if (Item->NamedLStyle.isEmpty())
		{
			QDomElement ob4 = docu.createElement("path");
			ob4.setAttribute("d", SetClipPath(&Item->PoLine, false));
			ob4.setAttribute("style", "fill:none; "+stroke);
			ob.appendChild(ob4);
		}
		else
		{
			multiLine ml = m_Doc->MLineStyles[Item->NamedLStyle];
			for (int it = ml.size()-1; it > -1; it--)
			{
				if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
				{
					QDomElement ob5 = docu.createElement("path");
					ob5.setAttribute("d", SetClipPath(&Item->PoLine, false));
					ob5.setAttribute("style", "fill:none; "+GetMultiStroke(&ml[it], Item));
					ob.appendChild(ob5);
				}
			}
		}
	}
	double x, y, wide;
	QString chstr;
	for (int a = 0; a < Item->itemText.length(); ++a)
	{
		x = 0.0;
		y = 0.0;
		ScText *hl = Item->itemText.item(a);
		const CharStyle& charStyle(Item->itemText.charStyle(a));
		chstr = Item->itemText.text(a,1);
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
				chs = qMax(static_cast<int>(hl->fontSize() * m_Doc->typographicSettings.valueSmallCaps / 100), 1);
				chstr = chstr.toUpper();
			}
		}
		else if (hl->effects() & ScStyle_AllCaps)
			chstr = chstr.toUpper();
		uint chr = chstr[0].unicode();
		QPointF tangt = QPointF( cos(hl->PRot), sin(hl->PRot) );
		QMatrix chma, chma2, chma3, chma4, chma6;
		QMatrix trafo = QMatrix( 1, 0, 0, -1, -hl->PDx, 0 );
		if (Item->textPathFlipped)
			trafo *= QMatrix(1, 0, 0, -1, 0, 0);
		if (Item->textPathType == 0)
			trafo *= QMatrix( tangt.x(), tangt.y(), tangt.y(), -tangt.x(), hl->PtransX, hl->PtransY );
		else if (Item->textPathType == 1)
			trafo *= QMatrix(1, 0, 0, -1, hl->PtransX, hl->PtransY );
		else if (Item->textPathType == 2)
		{
			double a = 1;
			double b = -1;
			if (tangt.x() < 0)
			{
				a = -1;
				b = 1;
			}
			if (fabs(tangt.x()) > 0.1)
				trafo *= QMatrix( a, (tangt.y() / tangt.x()) * b, 0, -1, hl->PtransX, hl->PtransY ); // ID's Skew mode
			else
				trafo *= QMatrix( a, 6 * b, 0, -1, hl->PtransX, hl->PtransY );
		}
		if ((hl->ch == SpecialChars::OBJECT) && (hl->embedded.hasItem()))
		{
			QMatrix finalMat = QMatrix(chma * chma2 * chma3 * chma4 * chma6 * trafo);
			ob.appendChild(processInlineItem(0, 0, finalMat, hl, true, trans));
		}
		else
		{
			if (Item->rotation() != 0)
			{
				QMatrix sca;
				sca.translate(-Item->xPos(), -Item->yPos());
				trafo *= sca;
			}
			chma.scale(hl->glyph.scaleH * charStyle.fontSize() / 100.00, hl->glyph.scaleV * charStyle.fontSize() / 100.0);
			if (Item->reversed())
			{
				if (a < Item->itemText.length()-1)
					wide = hl->font().charWidth(chstr[0], hl->fontSize(), Item->itemText.text(a+1));
				else
					wide = hl->font().charWidth(chstr[0], hl->fontSize());
				chma3.scale(-1, 1);
				chma3.translate(-wide, 0);
			}
			chma4.translate(0, Item->BaseOffs - (charStyle.fontSize() / 10.0) * hl->glyph.scaleV);
			if (charStyle.effects() & (ScStyle_Subscript | ScStyle_Superscript))
				chma6.translate(0, hl->glyph.yoffset);
			if (hl->baselineOffset() != 0)
				chma6.translate(0, (-charStyle.fontSize() / 10.0) * (charStyle.baselineOffset() / 1000.0));
			QMatrix finalMat = QMatrix(chma * chma2 * chma3 * chma4 * chma6 * trafo);
			if (Item->rotation() != 0)
			{
				QMatrix sca;
				sca.translate(Item->xPos(), Item->yPos());
				finalMat *= sca;
			}
			QChar chstc = hl->ch;
			if (((charStyle.effects() & ScStyle_Underline) && !SpecialChars::isBreak(chstc))
				|| ((charStyle.effects() & ScStyle_UnderlineWords) && !chstc.isSpace() && !SpecialChars::isBreak(chstc)))
			{
				QMatrix stro = QMatrix(chma2 * chma3 * chma6 * trafo);
				if (Item->rotation() != 0)
				{
					QMatrix sca;
					sca.translate(Item->xPos(), Item->yPos());
					stro *= sca;
				}
				double Ulen = hl->glyph.xadvance;
				double Upos, Uwid, kern;
				if (hl->effects() & ScStyle_StartOfLine)
					kern = 0;
				else
					kern = charStyle.fontSize() * charStyle.tracking() / 10000.0;
				if ((charStyle.underlineOffset() != -1) || (charStyle.underlineWidth() != -1))
				{
					if (charStyle.underlineOffset() != -1)
						Upos = (charStyle.underlineOffset() / 1000.0) * (charStyle.font().descent(charStyle.fontSize() / 10.0));
					else
						Upos = charStyle.font().underlinePos(charStyle.fontSize() / 10.0);
					if (charStyle.underlineWidth() != -1)
						Uwid = (charStyle.underlineWidth() / 1000.0) * (charStyle.fontSize() / 10.0);
					else
						Uwid = qMax(charStyle.font().strokeWidth(charStyle.fontSize() / 10.0), 1.0);
				}
				else
				{
					Upos = charStyle.font().underlinePos(charStyle.fontSize() / 10.0);
					Uwid = qMax(charStyle.font().strokeWidth(charStyle.fontSize() / 10.0), 1.0);
				}
				if (charStyle.baselineOffset() != 0)
					Upos += (charStyle.fontSize() / 10.0) * (charStyle.baselineOffset() / 1000.0);
				QDomElement ob8 = docu.createElement("path");
				ob8.setAttribute("transform", MatrixToStr(stro));
				if (charStyle.effects() & ScStyle_Subscript)
					ob8.setAttribute("d", QString("M %1 %2 L%3 %4").arg(hl->glyph.xoffset-kern).arg(-Upos).arg(hl->glyph.xoffset+Ulen).arg(-Upos));
				else
					ob8.setAttribute("d", QString("M %1 %2 L%3 %4").arg(hl->glyph.xoffset-kern).arg(-(Upos + hl->glyph.yoffset)).arg(hl->glyph.xoffset+Ulen).arg(-(Upos + hl->glyph.yoffset)));
				QString sT = "stroke:none;";
				if (charStyle.fillColor() != CommonStrings::None)
				{
					sT = "stroke:"+SetColor(charStyle.fillColor(), charStyle.fillShade())+";";
					sT += " stroke-width:"+FToStr(Uwid)+";";
				}
				ob8.setAttribute("style", "fill:none;" + sT);
				ob.appendChild(ob8);
			}
			if (chstr > QChar(32))
			{
				QString glName = handleGlyph(chr, hl);
				if ((charStyle.effects() & ScStyle_Shadowed) && (charStyle.strokeColor() != CommonStrings::None))
				{
					QMatrix sha = finalMat;
					QMatrix shad;
					shad.translate(charStyle.fontSize() * charStyle.shadowXOffset() / 10000.0, -charStyle.fontSize() * charStyle.shadowYOffset() / 10000.0);
					sha *= shad;
					QDomElement ob2 = docu.createElement("use");
					ob2.setAttribute("xlink:href", "#" + glName);
					ob2.setAttribute("transform", MatrixToStr(sha));
					ob2.setAttribute("style", "fill:"+SetColor(hl->strokeColor(), hl->strokeShade())+";" + "stroke:none;");
					ob.appendChild(ob2);
				}
				QDomElement ob1 = docu.createElement("use");
				ob1.setAttribute("xlink:href", "#" + glName);
				ob1.setAttribute("transform", MatrixToStr(finalMat));
				QString fT = "fill:"+SetColor(hl->fillColor(), hl->fillShade())+";";
				QString sT = "stroke:none;";
				if (charStyle.effects() & ScStyle_Outline)
				{
					sT = "stroke:"+SetColor(hl->strokeColor(), hl->strokeShade())+";";
					sT += " stroke-width:"+FToStr(chs * hl->outlineWidth() / 10000.0)+";";
				}
				ob1.setAttribute("style", fT + sT);
				ob.appendChild(ob1);
			}
			if (charStyle.effects() & ScStyle_Strikethrough)
			{
				QMatrix stro = QMatrix(chma2 * chma3 * chma6 * trafo);
				if (Item->rotation() != 0)
				{
					QMatrix sca;
					sca.translate(Item->xPos(), Item->yPos());
					stro *= sca;
				}
				double Ulen = hl->glyph.xadvance;
				double Upos, Uwid, kern;
				if (hl->effects() & ScStyle_StartOfLine)
					kern = 0;
				else
					kern = charStyle.fontSize() * charStyle.tracking() / 10000.0;
				if ((charStyle.strikethruOffset() != -1) || (charStyle.strikethruWidth() != -1))
				{
					if (charStyle.strikethruOffset() != -1)
						Upos = (charStyle.strikethruOffset() / 1000.0) * (charStyle.font().ascent(charStyle.fontSize() / 10.0));
					else
						Upos = charStyle.font().strikeoutPos(charStyle.fontSize() / 10.0);
					if (charStyle.strikethruWidth() != -1)
						Uwid = (charStyle.strikethruWidth() / 1000.0) * (charStyle.fontSize() / 10.0);
					else
						Uwid = qMax(charStyle.font().strokeWidth(charStyle.fontSize() / 10.0), 1.0);
				}
				else
				{
					Upos = charStyle.font().strikeoutPos(charStyle.fontSize() / 10.0);
					Uwid = qMax(charStyle.font().strokeWidth(charStyle.fontSize() / 10.0), 1.0);
				}
				if (charStyle.baselineOffset() != 0)
					Upos += (charStyle.fontSize() / 10.0) * (charStyle.baselineOffset() / 1000.0);
				QDomElement ob7 = docu.createElement("path");
				ob7.setAttribute("transform", MatrixToStr(stro));
				ob7.setAttribute("d", QString("M %1 %2 L%3 %4").arg(hl->glyph.xoffset-kern).arg(-Upos).arg(hl->glyph.xoffset+Ulen).arg(-Upos));
				QString sT = "stroke:none;";
				if (charStyle.fillColor() != CommonStrings::None)
				{
					sT = "stroke:"+SetColor(charStyle.fillColor(), charStyle.fillShade())+";";
					sT += " stroke-width:"+FToStr(Uwid)+";";
				}
				ob7.setAttribute("style", "fill:none;" + sT);
				ob.appendChild(ob7);
			}
		}
	}
	return ob;
}

QDomElement SVGExPlug::processInlineItem(double xpos, double ypos, QMatrix &finalMat, ScText *hl, bool pathT, QString trans)
{
	const CharStyle & charStyle(*hl);
	QList<PageItem*> emG = hl->embedded.getGroupedItems();
	QStack<PageItem*> groupStack;
	QStack<QDomElement> groupStack2;
	QDomElement layerGroup = docu.createElement("g");
	if (pathT)
		layerGroup.setAttribute("transform", MatrixToStr(finalMat));
	for (int em = 0; em < emG.count(); ++em)
	{
		PageItem* embedded = emG.at(em);
		if (embedded->isGroupControl)
		{
			groupStack.push(embedded->groupsLastItem);
			groupStack2.push(layerGroup);
			layerGroup = docu.createElement("g");
			if (embedded->fillTransparency() != 0)
				layerGroup.setAttribute("opacity", FToStr(1.0 - embedded->fillTransparency()));
			QDomElement ob = docu.createElement("clipPath");
			ob.setAttribute("id", "Clip"+IToStr(ClipCount));
			QDomElement cl = docu.createElement("path");
			cl.setAttribute("d", SetClipPath(&embedded->PoLine, true));
			QMatrix mm;
			mm.translate(xpos + embedded->gXpos * (charStyle.scaleH() / 1000.0), (ypos - (embedded->gHeight * (charStyle.scaleV() / 1000.0)) + embedded->gYpos * (charStyle.scaleV() / 1000.0)));
			if (charStyle.baselineOffset() != 0)
				mm.translate(0, embedded->gHeight * (charStyle.baselineOffset() / 1000.0));
			if (charStyle.scaleH() != 1000)
				mm.scale(charStyle.scaleH() / 1000.0, 1);
			if (charStyle.scaleV() != 1000)
				mm.scale(1, charStyle.scaleV() / 1000.0);
			mm.rotate(embedded->rotation());
			cl.setAttribute("transform", MatrixToStr(mm));
			ob.appendChild(cl);
			globalDefs.appendChild(ob);
			layerGroup.setAttribute("clip-path", "url(#Clip"+IToStr(ClipCount)+")");
			ClipCount++;
			continue;
		}
		QDomElement obE;
		QString fill = getFillStyle(embedded);
		QString stroke = "stroke:none";
		if (!embedded->isTableItem)
			stroke = getStrokeStyle(embedded);
		switch (embedded->itemType())
		{
			case PageItem::Polygon:
			case PageItem::PolyLine:
				obE = processPolyItem(embedded, trans, fill, stroke);
				if ((embedded->lineColor() != CommonStrings::None) && ((embedded->startArrowIndex() != 0) || (embedded->endArrowIndex() != 0)))
					obE = processArrows(embedded, obE, trans);
				break;
			case PageItem::Line:
				obE = processLineItem(embedded, trans, stroke);
				if ((embedded->lineColor() != CommonStrings::None) && ((embedded->startArrowIndex() != 0) || (embedded->endArrowIndex() != 0)))
					obE = processArrows(embedded, obE, trans);
				break;
			case PageItem::ImageFrame:
			case PageItem::LatexFrame:
				obE = processImageItem(embedded, trans, fill, stroke);
				break;
			case PageItem::TextFrame:
				obE = processTextItem(embedded, trans, fill, stroke);
				break;
			case PageItem::PathText:
				obE = processPathTextItem(embedded, trans, stroke);
				break;
			default:
				break;
		}
		QMatrix mm;
		mm.translate(xpos + embedded->gXpos * (charStyle.scaleH() / 1000.0), (ypos - (embedded->gHeight * (charStyle.scaleV() / 1000.0)) + embedded->gYpos * (charStyle.scaleV() / 1000.0)));
		if (charStyle.baselineOffset() != 0)
			mm.translate(0, embedded->gHeight * (charStyle.baselineOffset() / 1000.0));
		if (charStyle.scaleH() != 1000)
			mm.scale(charStyle.scaleH() / 1000.0, 1);
		if (charStyle.scaleV() != 1000)
			mm.scale(1, charStyle.scaleV() / 1000.0);
		mm.rotate(embedded->rotation());
		obE.setAttribute("transform", MatrixToStr(mm));
		layerGroup.appendChild(obE);
		if (groupStack.count() != 0)
		{
			while (embedded == groupStack.top())
			{
				groupStack.pop();
				groupStack2.top().appendChild(layerGroup);
				layerGroup = groupStack2.pop();
				if (groupStack.count() == 0)
					break;
			}
		}
	}
	for (int em = 0; em < emG.count(); ++em)
	{
		PageItem* embedded = emG.at(em);
		if (!embedded->isTableItem)
			continue;
		if ((embedded->lineColor() == CommonStrings::None) || (embedded->lineWidth() == 0.0))
			continue;
		if ((embedded->TopLine) || (embedded->RightLine) || (embedded->BottomLine) || (embedded->LeftLine))
		{
			QMatrix mm;
			mm.translate(xpos + embedded->gXpos * (charStyle.scaleH() / 1000.0), (ypos - (embedded->gHeight * (charStyle.scaleV() / 1000.0)) + embedded->gYpos * (charStyle.scaleV() / 1000.0)));
			if (charStyle.baselineOffset() != 0)
				mm.translate(0, embedded->gHeight * (charStyle.baselineOffset() / 1000.0));
			if (charStyle.scaleH() != 1000)
				mm.scale(charStyle.scaleH() / 1000.0, 1);
			if (charStyle.scaleV() != 1000)
				mm.scale(1, charStyle.scaleV() / 1000.0);
			mm.rotate(embedded->rotation());
			QString stroke = getStrokeStyle(embedded);
			QDomElement obL = docu.createElement("path");
			obL.setAttribute("transform", MatrixToStr(mm));
			obL.setAttribute("style", "fill:none; " + stroke);
			QString pathAttr = "";
			if (embedded->TopLine)
				pathAttr += "M 0 0 L "+FToStr(embedded->width())+" 0";
			if (embedded->RightLine)
				pathAttr += " M " + FToStr(embedded->width()) + "0 L "+FToStr(embedded->width())+" "+FToStr(embedded->height());
			if (embedded->BottomLine)
				pathAttr += " M 0 " + FToStr(embedded->height()) + " L "+FToStr(embedded->width())+" "+FToStr(embedded->height());
			if (embedded->LeftLine)
				pathAttr += " M 0 0 L 0 "+FToStr(embedded->height());
			obL.setAttribute("d", pathAttr);
			layerGroup.appendChild(obL);
		}
	}
	return layerGroup;
}

QString SVGExPlug::handleGlyph(uint chr, ScText *hl)
{
	if (chr == 32)
		return "SPACE";
	QString glName = QString("Gl%1%2").arg(hl->font().psName().simplified().replace(QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" )).arg(chr);
	if (glyphNames.contains(glName))
		return glName;
	uint gl = hl->font().char2CMap(chr);
	FPointArray pts = hl->font().glyphOutline(gl);
	QDomElement ob = docu.createElement("path");
	ob.setAttribute("d", SetClipPath(&pts, true));
	ob.setAttribute("id", glName);
	globalDefs.appendChild(ob);
	glyphNames.append(glName);
	return glName;
}

QDomElement SVGExPlug::processArrows(PageItem *Item, QDomElement line, QString trans)
{
	QDomElement ob, gr;
	gr = docu.createElement("g");
	gr.appendChild(line);
	if (Item->startArrowIndex() != 0)
	{
		QMatrix arrowTrans;
		FPointArray arrow = m_Doc->arrowStyles.at(Item->startArrowIndex()-1).points.copy();
		if (Item->itemType() == PageItem::Line)
		{
			arrowTrans.translate(0, 0);
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
			for (uint xx = 1; xx < Item->PoLine.size(); xx += 2)
			{
				FPoint Vector = Item->PoLine.point(xx);
				if ((Start.x() != Vector.x()) || (Start.y() != Vector.y()))
				{
					double r = atan2(Start.y()-Vector.y(),Start.x()-Vector.x())*(180.0/M_PI);
					arrowTrans.translate(Start.x(), Start.y());
					arrowTrans.rotate(r);
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
		if (Item->NamedLStyle.isEmpty())
		{
			ob = docu.createElement("path");
			ob.setAttribute("d", SetClipPath(&arrow, true));
			ob.setAttribute("transform", trans);
			QString aFill = "fill:"+SetColor(Item->lineColor(), Item->lineShade())+";";
			if (Item->lineTransparency() != 0)
				aFill += " fill-opacity:"+FToStr(1.0 - Item->lineTransparency())+";";
			ob.setAttribute("style", aFill + " stroke:none;");
			gr.appendChild(ob);
		}
		else
		{
			multiLine ml = m_Doc->MLineStyles[Item->NamedLStyle];
			if (ml[0].Color != CommonStrings::None)
			{
				ob = docu.createElement("path");
				ob.setAttribute("d", SetClipPath(&arrow, true));
				ob.setAttribute("transform", trans);
				QString aFill = "fill:"+SetColor(ml[0].Color, ml[0].Shade)+";";
				ob.setAttribute("style", aFill + " stroke:none;");
				gr.appendChild(ob);
			}
			for (int it = ml.size()-1; it > 0; it--)
			{
				if (ml[it].Color != CommonStrings::None)
				{
					QDomElement ob5 = docu.createElement("path");
					ob5.setAttribute("d", SetClipPath(&arrow, true));
					ob5.setAttribute("transform", trans);
					QString stroke = "fill:none; stroke:"+SetColor(ml[it].Color, ml[it].Shade)+"; stroke-linecap:butt; stroke-linejoin:miter; stroke-dasharray:none;";
					if (ml[it].Width != 0.0)
						stroke += " stroke-width:"+FToStr(ml[it].Width)+";";
					else
						stroke += " stroke-width:1px;";
					ob5.setAttribute("style", stroke);
					gr.appendChild(ob5);
				}
			}
		}
	}
	if (Item->endArrowIndex() != 0)
	{
		QMatrix arrowTrans;
		FPointArray arrow = m_Doc->arrowStyles.at(Item->endArrowIndex()-1).points.copy();
		if (Item->itemType() == PageItem::Line)
		{
			arrowTrans.translate(Item->width(), 0);
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
		if (Item->NamedLStyle.isEmpty())
		{
			ob = docu.createElement("path");
			ob.setAttribute("d", SetClipPath(&arrow, true));
			ob.setAttribute("transform", trans);
			QString aFill = "fill:"+SetColor(Item->lineColor(), Item->lineShade())+";";
			if (Item->lineTransparency() != 0)
				aFill += " fill-opacity:"+FToStr(1.0 - Item->lineTransparency())+";";
			ob.setAttribute("style", aFill + " stroke:none;");
			gr.appendChild(ob);
		}
		else
		{
			multiLine ml = m_Doc->MLineStyles[Item->NamedLStyle];
			if (ml[0].Color != CommonStrings::None)
			{
				ob = docu.createElement("path");
				ob.setAttribute("d", SetClipPath(&arrow, true));
				ob.setAttribute("transform", trans);
				QString aFill = "fill:"+SetColor(ml[0].Color, ml[0].Shade)+";";
				ob.setAttribute("style", aFill + " stroke:none;");
				gr.appendChild(ob);
			}
			for (int it = ml.size()-1; it > 0; it--)
			{
				if (ml[it].Color != CommonStrings::None)
				{
					QDomElement ob5 = docu.createElement("path");
					ob5.setAttribute("d", SetClipPath(&arrow, true));
					ob5.setAttribute("transform", trans);
					QString stroke = "fill:none; stroke:"+SetColor(ml[it].Color, ml[it].Shade)+"; stroke-linecap:butt; stroke-linejoin:miter; stroke-dasharray:none;";
					if (ml[it].Width != 0.0)
						stroke += " stroke-width:"+FToStr(ml[it].Width)+";";
					else
						stroke += " stroke-width:1px;";
					ob5.setAttribute("style", stroke);
					gr.appendChild(ob5);
				}
			}
		}
	}
	return gr;
}

QString SVGExPlug::getFillStyle(PageItem *Item)
{
	QDomElement grad;
	QString fill;
	if (Item->asPathText())
		return "fill:none;";
	if ((Item->fillColor() != CommonStrings::None) || (Item->GrType != 0))
	{
		fill = "fill:"+SetColor(Item->fillColor(), Item->fillShade())+";";
		if (Item->GrType != 0)
		{
			if (Item->GrType == 8)
			{
				QStack<PageItem*> groupStack;
				QStack<QDomElement> groupStack2;
				QString pattID = Item->pattern()+IToStr(PattCount);
				PattCount++;
				ScPattern pa = m_Doc->docPatterns[Item->pattern()];
				QDomElement patt = docu.createElement("pattern");
				patt.setAttribute("id", pattID);
				patt.setAttribute("height", pa.height);
				patt.setAttribute("width", pa.width);
				patt.setAttribute("patternUnits", "userSpaceOnUse");
				double patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation;
				Item->patternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation);
				QMatrix mpa;
				mpa.translate(patternOffsetX, patternOffsetY);
				mpa.rotate(patternRotation);
				mpa.scale(pa.scaleX, pa.scaleY);
				mpa.scale(patternScaleX / 100.0 , patternScaleY / 100.0);
				patt.setAttribute("patternTransform", MatrixToStr(mpa));
				for (int em = 0; em < pa.items.count(); ++em)
				{
					PageItem* Item = pa.items.at(em);
					if (Item->isGroupControl)
					{
						groupStack.push(Item->groupsLastItem);
						groupStack2.push(patt);
						patt = docu.createElement("g");
						if (Item->fillTransparency() != 0)
							patt.setAttribute("opacity", FToStr(1.0 - Item->fillTransparency()));
						QDomElement ob = docu.createElement("clipPath");
						ob.setAttribute("id", "Clip"+IToStr(ClipCount));
						QDomElement cl = docu.createElement("path");
						cl.setAttribute("d", SetClipPath(&Item->PoLine, true));
						QString trans = "translate("+FToStr(Item->gXpos)+", "+FToStr(Item->gYpos)+")";
						if (Item->rotation() != 0)
							trans += " rotate("+FToStr(Item->rotation())+")";
						cl.setAttribute("transform", trans);
						ob.appendChild(cl);
						globalDefs.appendChild(ob);
						patt.setAttribute("clip-path", "url(#Clip"+IToStr(ClipCount)+")");
						ClipCount++;
						continue;
					}
					ProcessItemOnPage(Item->gXpos, Item->gYpos, Item, &patt);
					if (groupStack.count() != 0)
					{
						while (Item == groupStack.top())
						{
							groupStack.pop();
							groupStack2.top().appendChild(patt);
							patt = groupStack2.pop();
							if (groupStack.count() == 0)
								break;
						}
					}
				}
				for (int em = 0; em < pa.items.count(); ++em)
				{
					PageItem* embedded = pa.items.at(em);
					QString trans = "translate("+FToStr(embedded->gXpos)+", "+FToStr(embedded->gYpos)+")";
					if (embedded->rotation() != 0)
						trans += " rotate("+FToStr(embedded->rotation())+")";
					if (!embedded->isTableItem)
						continue;
					if ((embedded->lineColor() == CommonStrings::None) || (embedded->lineWidth() == 0.0))
						continue;
					if ((embedded->TopLine) || (embedded->RightLine) || (embedded->BottomLine) || (embedded->LeftLine))
					{
						QString stroke = getStrokeStyle(embedded);
						QDomElement obL = docu.createElement("path");
						obL.setAttribute("transform", trans);
						obL.setAttribute("style", "fill:none; " + stroke);
						QString pathAttr = "";
						if (embedded->TopLine)
							pathAttr += "M 0 0 L "+FToStr(embedded->width())+" 0";
						if (embedded->RightLine)
							pathAttr += " M " + FToStr(embedded->width()) + "0 L "+FToStr(embedded->width())+" "+FToStr(embedded->height());
						if (embedded->BottomLine)
							pathAttr += " M 0 " + FToStr(embedded->height()) + " L "+FToStr(embedded->width())+" "+FToStr(embedded->height());
						if (embedded->LeftLine)
							pathAttr += " M 0 0 L 0 "+FToStr(embedded->height());
						obL.setAttribute("d", pathAttr);
						patt.appendChild(obL);
					}
				}
				globalDefs.appendChild(patt);
				fill = "fill:url(#"+pattID+");";
			}
			else
			{
				if ((Item->GrType == 5) || (Item->GrType == 7))
					grad = docu.createElement("radialGradient");
				else
					grad = docu.createElement("linearGradient");
				grad.setAttribute("id", "Grad"+IToStr(GradCount));
				grad.setAttribute("gradientUnits", "userSpaceOnUse");
				switch (Item->GrType)
				{
					case 1:
						grad.setAttribute("x1", "0");
						grad.setAttribute("y1", FToStr(Item->height() / 2.0));
						grad.setAttribute("x2", FToStr(Item->width()));
						grad.setAttribute("y2", FToStr(Item->height() / 2.0));
						break;
					case 2:
						grad.setAttribute("x1", FToStr(Item->width()/ 2.0));
						grad.setAttribute("y1", "0");
						grad.setAttribute("x2", FToStr(Item->width()/ 2.0));
						grad.setAttribute("y2", FToStr(Item->height()));
						break;
					case 3:
						grad.setAttribute("x1", "0");
						grad.setAttribute("y1", "0");
						grad.setAttribute("x2", FToStr(Item->width()));
						grad.setAttribute("y2", FToStr(Item->height()));
						break;
					case 4:
						grad.setAttribute("x1", "0");
						grad.setAttribute("y1", FToStr(Item->height()));
						grad.setAttribute("x2", FToStr(Item->width()));
						grad.setAttribute("y2", "0");
						break;
					case 5:
						grad.setAttribute("r", FToStr(qMax(Item->width() / 2.0, Item->height() / 2.0)));
						grad.setAttribute("cx", FToStr(Item->width() / 2.0));
						grad.setAttribute("cy", FToStr(Item->height() / 2.0));
						break;
					case 6:
						grad.setAttribute("x1", FToStr(Item->GrStartX));
						grad.setAttribute("y1", FToStr(Item->GrStartY));
						grad.setAttribute("x2", FToStr(Item->GrEndX));
						grad.setAttribute("y2", FToStr(Item->GrEndY));
						break;
					case 7:
						grad.setAttribute("r", FToStr(qMax(Item->width() / 2.0, Item->height() / 2.0)));
						grad.setAttribute("cx", FToStr(Item->GrStartX));
						grad.setAttribute("cy", FToStr(Item->GrStartY));
						break;
				}
				QList<VColorStop*> cstops = Item->fill_gradient.colorStops();
				for (uint cst = 0; cst < Item->fill_gradient.Stops(); ++cst)
				{
					QDomElement itcl = docu.createElement("stop");
					itcl.setAttribute("offset", FToStr(cstops.at(cst)->rampPoint*100)+"%");
					itcl.setAttribute("stop-opacity", FToStr(cstops.at(cst)->opacity));
					itcl.setAttribute("stop-color", SetColor(cstops.at(cst)->name, cstops.at(cst)->shade));
					grad.appendChild(itcl);
				}
				globalDefs.appendChild(grad);
				fill = "fill:url(#Grad"+IToStr(GradCount)+");";
				GradCount++;
			}
		}
		if (Item->fillRule)
			fill += " fill-rule:evenodd;";
		else
			fill += " fill-rule:nonzero;";
		if (Item->fillTransparency() != 0)
			fill += " fill-opacity:"+FToStr(1.0 - Item->fillTransparency())+";";
	}
	else
		fill = "fill:none;";
	return fill;
}

QString SVGExPlug::getStrokeStyle(PageItem *Item)
{
	QString stroke = "";
	if (Item->lineColor() != CommonStrings::None)
	{
		stroke = "stroke:"+SetColor(Item->lineColor(), Item->lineShade())+";";
		if (Item->lineTransparency() != 0)
			stroke += " stroke-opacity:"+FToStr(1.0 - Item->lineTransparency())+";";
		if (Item->lineWidth() != 0.0)
			stroke += " stroke-width:"+FToStr(Item->lineWidth())+";";
		else
			stroke += " stroke-width:1px;";
		stroke += " stroke-linecap:";
		switch (Item->PLineEnd)
		{
			case Qt::FlatCap:
				stroke += "butt;";
				break;
			case Qt::SquareCap:
				stroke += "square;";
				break;
			case Qt::RoundCap:
				stroke += "round;";
				break;
			default:
				stroke += "butt;";
				break;
		}
		stroke += " stroke-linejoin:";
		switch (Item->PLineJoin)
		{
			case Qt::MiterJoin:
				stroke += "miter;";
				break;
			case Qt::BevelJoin:
				stroke += "bevel;";
				break;
			case Qt::RoundJoin:
				stroke += "round;";
				break;
			default:
				stroke += "miter;";
				break;
		}
		stroke += " stroke-dasharray:";
		if (Item->DashValues.count() != 0)
		{
			QVector<double>::iterator it;
			for ( it = Item->DashValues.begin(); it != Item->DashValues.end(); ++it )
			{
				stroke += IToStr(static_cast<int>(*it))+" ";
			}
			stroke += "; stroke-dashoffset:"+IToStr(static_cast<int>(Item->DashOffset))+";";
		}
		else
		{
			if (Item->PLineArt == Qt::SolidLine)
				stroke += "none;";
			else
			{
				QString Da = getDashString(Item->PLineArt, Item->lineWidth());
				if (Da.isEmpty())
					stroke += "none;";
				else
					stroke += Da.replace(" ", ", ")+";";
			}
		}
	}
	else
		stroke = "stroke:none;";
	return stroke;
}

QString SVGExPlug::SetClipPath(FPointArray *ite, bool closed)
{
	QString tmp = "";
	FPoint np, np1, np2, np3;
	bool nPath = true;
	if (ite->size() > 3)
	{
		for (uint poi=0; poi<ite->size()-3; poi += 4)
		{
			if (ite->point(poi).x() > 900000)
			{
				tmp += "Z ";
				nPath = true;
				continue;
			}
			if (nPath)
			{
				np = ite->point(poi);
				tmp += QString("M%1 %2 ").arg(np.x()).arg(np.y());
				nPath = false;
			}
			np = ite->point(poi);
			np1 = ite->point(poi+1);
			np2 = ite->point(poi+3);
			np3 = ite->point(poi+2);
			if ((np == np1) && (np2 == np3))
				tmp += QString("L%1 %2 ").arg(np3.x()).arg(np3.y());
			else
				tmp += QString("C%1 %2 %3 %4 %5 %6 ").arg(np1.x()).arg(np1.y()).arg(np2.x()).arg(np2.y()).arg(np3.x()).arg(np3.y());
		}
	}
	if (closed)
		tmp += "Z";
	return tmp;
}

QString SVGExPlug::FToStr(double c)
{
	QString cc;
	return cc.setNum(c);
}

QString SVGExPlug::IToStr(int c)
{
	QString cc;
	return cc.setNum(c);
}

QString SVGExPlug::MatrixToStr(QMatrix &mat)
{
	QString cc("matrix(%1 %2 %3 %4 %5 %6)");
	return  cc.arg(mat.m11()).arg(mat.m12()).arg(mat.m21()).arg(mat.m22()).arg(mat.dx()).arg(mat.dy());
}

QString SVGExPlug::SetColor(QString farbe, int shad)
{
	const ScColor& col = m_Doc->PageColors[farbe];
	return ScColorEngine::getShadeColorProof(col, m_Doc, shad).name();
}

QString SVGExPlug::GetMultiStroke(struct SingleLine *sl, PageItem *Item)
{
	QString tmp = "fill:none; ";
	tmp += "stroke:"+SetColor(sl->Color, sl->Shade)+"; ";
	if (Item->fillTransparency() != 0)
		tmp += QString(" stroke-opacity:%1; ").arg(1.0 - Item->fillTransparency());
	tmp += QString("stroke-width:%1; ").arg(sl->Width);
	tmp += "stroke-linecap:";
	switch (static_cast<Qt::PenCapStyle>(sl->LineEnd))
		{
		case Qt::FlatCap:
			tmp += "butt;";
			break;
		case Qt::SquareCap:
			tmp += "square;";
			break;
		case Qt::RoundCap:
			tmp += "round;";
			break;
		default:
			tmp += "butt;";
			break;
		}
	tmp += " stroke-linejoin:";
	switch (static_cast<Qt::PenJoinStyle>(sl->LineJoin))
		{
		case Qt::MiterJoin:
			tmp += "miter;";
			break;
		case Qt::BevelJoin:
			tmp += "bevel;";
			break;
		case Qt::RoundJoin:
			tmp += "round;";
			break;
		default:
			tmp += "miter;";
			break;
		}
	tmp += " stroke-dasharray:";
	if (static_cast<Qt::PenStyle>(sl->Dash) == Qt::SolidLine)
		tmp += "none;";
	else
	{
		QString Da = getDashString(sl->Dash, sl->Width);
		if (Da.isEmpty())
			tmp += "none;";
		else
			tmp += Da.replace(" ", ", ")+";";
	}
	return tmp;
}

SVGExPlug::~SVGExPlug()
{
}
