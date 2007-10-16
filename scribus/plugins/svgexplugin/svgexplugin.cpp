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

#include <qfile.h>
#include <qtextstream.h>

#include "svgexplugin.h"

#include "scconfig.h"
#include "commonstrings.h"
#include "customfdialog.h"
#include "scribus.h"
#include "page.h"
#ifdef HAVE_LIBZ
#include <zlib.h>
#endif
#include "prefsmanager.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "scmessagebox.h"
#include "util.h"
#include "customfdialog.h"
#include "sctextstruct.h"


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
	m_actionInfo.text = tr("Save Page as &SVG...");
	// Menu
	m_actionInfo.menu = "FileExport";
	m_actionInfo.enabledOnStartup = true;
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

bool SVGExportPlugin::run(QString filename)
{
	Q_ASSERT(filename.isEmpty());
	QString fileName;

	if (ScMW->HaveDoc)
	{
		PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("svgex");
		QString wdir = prefs->get("wdir", ".");
#ifdef HAVE_LIBZ
		CustomFDialog *openDia = new CustomFDialog(ScMW, wdir, QObject::tr("Save as"), QObject::tr("SVG-Images (*.svg *.svgz);;All Files (*)"), false, false, true, false, false);
#else
		CustomFDialog *openDia = new CustomFDialog(ScMW, wdir, QObject::tr("Save as"), QObject::tr("SVG-Images (*.svg);;All Files (*)"), false, false, false, false, false);
#endif
		openDia->setSelection(getFileNameByPage(ScMW->doc->currentPage->pageNr(), "svg"));
		openDia->setExtension("svg");
		openDia->setZipExtension("svgz");
		if (openDia->exec())
		{
#ifdef HAVE_LIBZ
			if (openDia->SaveZip->isChecked())
				openDia->handleCompress();
#endif
			fileName = openDia->selectedFile();
		}
		delete openDia;

		if (!fileName.isEmpty())
		{
			prefs->set("wdir", fileName.left(fileName.findRev("/")));
			QFile f(fileName);
			if (f.exists())
			{
				int exit=ScMessageBox::warning(ScMW, QObject::tr("Warning"),
					QObject::tr("Do you really want to overwrite the File:\n%1 ?").arg(fileName),
					QObject::tr("Yes"),
					QObject::tr("No"),
					0, 0, 1);
				if (exit != 0)
					return true;
			}
			SVGExPlug *dia = new SVGExPlug(fileName);
			delete dia;
		}
		else
			return true;
	}
	return true;
}

SVGExPlug::SVGExPlug( QString fName )
{
	QDomDocument docu("svgdoc");
	QString vo = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	QString st = "<svg></svg>";
	docu.setContent(st);
	QDomElement elem = docu.documentElement();
	elem.setAttribute("width", FToStr(ScMW->doc->pageWidth)+"pt");
	elem.setAttribute("height", FToStr(ScMW->doc->pageHeight)+"pt");
	elem.setAttribute("xmlns", "http://www.w3.org/2000/svg");
	elem.setAttribute("xmlns:xlink","http://www.w3.org/1999/xlink");
	Page *Seite;
	GradCount = 0;
	ClipCount = 0;
	Seite = ScMW->doc->MasterPages.at(ScMW->doc->MasterNames[ScMW->doc->currentPage->MPageNam]);
	ProcessPage(Seite, &docu, &elem);
	Seite = ScMW->doc->currentPage;
	ProcessPage(Seite, &docu, &elem);
#ifdef HAVE_LIBZ
	if(fName.right(2) == "gz")
	{
// zipped saving
		gzFile gzDoc = gzopen(fName.latin1(),"wb");
		if(gzDoc == NULL)
			return;
		gzputs(gzDoc, vo);
		gzputs(gzDoc, docu.toString().utf8());
		gzclose(gzDoc);
	}
	else
	{
		QFile f(fName);
		if(!f.open(IO_WriteOnly))
			return;
		QTextStream s(&f);
		QString wr = vo;
		wr += docu.toString();
		QCString utf8wr = wr.utf8();
		s.writeRawBytes(utf8wr.data(), utf8wr.length());
		f.close();
	}
#else
	QFile f(fName);
	if(!f.open(IO_WriteOnly))
		return;
	QTextStream s(&f);
	wr += docu.toString();
	QCString utf8wr = wr.utf8();
	s.writeRawBytes(utf8wr.data(), utf8wr.length());
	f.close();
#endif
}

void SVGExPlug::ProcessPage(Page *Seite, QDomDocument *docu, QDomElement *elem)
{
	int Lnr = 0;
	struct Layer ll;
	ll.isPrintable = false;
	ll.LNr = 0;
	PageItem *Item;
	QPtrList<PageItem> Items;
	Page* SavedAct = ScMW->doc->currentPage;
	ScMW->doc->currentPage = Seite;
	if (Seite->PageNam.isEmpty())
		Items = ScMW->doc->DocItems;
	else
		Items = ScMW->doc->MasterItems;
	for (uint la = 0; la < ScMW->doc->Layers.count(); la++)
	{
		Level2Layer(ScMW->doc, &ll, Lnr);
		if (ll.isPrintable)
		{
			for(uint j = 0; j < Items.count(); ++j)
			{
				Item = Items.at(j);
				if (Item->LayerNr != ll.LNr)
					continue;
				double x = Seite->xOffset();
				double y = Seite->yOffset();
				double w = Seite->width();
				double h = Seite->height();
				double x2 = Item->BoundingX;
				double y2 = Item->BoundingY;
				double w2 = Item->BoundingW;
				double h2 = Item->BoundingH;
				if (!( QMAX( x, x2 ) <= QMIN( x+w, x2+w2 ) && QMAX( y, y2 ) <= QMIN( y+h, y2+h2 )))
					continue;
				if (Item->asImageFrame())
					ProcessItem_ImageFrame(Item, Seite, docu, elem);
				else if (Item->asLine())
					ProcessItem_Line(Item, Seite, docu, elem);
				else if (Item->asPathText())
					ProcessItem_PathText(Item, Seite, docu, elem);
				else if (Item->asPolygon())
					ProcessItem_Polygon(Item, Seite, docu, elem);
				else if (Item->asPolyLine())
					ProcessItem_PolyLine(Item, Seite, docu, elem);
				else if (Item->asTextFrame())
					ProcessItem_TextFrame(Item, Seite, docu, elem);
			}
		}
		Lnr++;
	}
	ScMW->doc->currentPage = SavedAct;
}

void SVGExPlug::ProcessItem_ImageFrame(PageItem* item, Page* page, QDomDocument *docu, QDomElement *elem)
{
	QString clipId   = "Clip";
	QString fill     = ProcessFill(item, docu, elem);
	QString stroke   = ProcessStroke(item);
	QString strokeLC = ProcessStrokeLineCap(item);
	QString strokeLJ = ProcessStrokeLineJoin(item);
	QString strokeDA = ProcessStrokeDashArray(item);
	QString strokeW  = ProcessStrokeWidth(item);
	QString trans    = ProcessTransform(item, page);
	QDomElement ob;
	QDomElement gr = docu->createElement("g");
	gr.setAttribute("transform", trans);
	if (item->NamedLStyle.isEmpty())
		gr.setAttribute("style", fill+" "+stroke+" "+strokeW+" "+strokeLC+" "+strokeLJ+" "+strokeDA);
	if ((item->fillColor() != CommonStrings::None) || (item->GrType != 0))
	{
		ob = docu->createElement("path");
		ob.setAttribute("d", SetClipPath(item)+"Z");
		ob.setAttribute("style", fill);
		gr.appendChild(ob);
	}
	if ((item->PicAvail) && (!item->Pfile.isEmpty()))
	{
		ob = docu->createElement("clipPath");
		ob.setAttribute("id", clipId+IToStr(ClipCount));
		ob.setAttribute("clipPathUnits", "userSpaceOnUse");
		ob.setAttribute("clip-rule", "evenodd");
		QDomElement cl = docu->createElement("path");
		if (item->imageClip.size() != 0)
			cl.setAttribute("d", SetClipPathImage(item)+"Z");
		else
			cl.setAttribute("d", SetClipPath(item)+"Z");
		ob.appendChild(cl);
		gr.appendChild(ob);
		ScImage img;
		img.LoadPicture(item->Pfile, item->IProfile, item->IRender, item->UseEmbedded, true, ScImage::RGBProof, 72);
		img.applyEffect(item->effectsInUse, item->document()->PageColors, true);
		QFileInfo fi = QFileInfo(item->Pfile);
		img.save(fi.baseName()+".png", "PNG");
		ob = docu->createElement("image");
		ob.setAttribute("clip-path", "url(#"+clipId+IToStr(ClipCount)+")");
		ob.setAttribute("xlink:href", fi.baseName()+".png");
		ob.setAttribute("x", "0pt");
		ob.setAttribute("y", "0pt");
		ob.setAttribute("width", FToStr(item->width())+"pt");
		ob.setAttribute("height", FToStr(item->height())+"pt");
		ClipCount++;
		gr.appendChild(ob);
	}
	if (item->NamedLStyle.isEmpty())
	{
		ob = docu->createElement("path");
		ob.setAttribute("d", SetClipPath(item)+"Z");
		ob.setAttribute("style", "fill:none; "+stroke+" "+strokeW+" "+strokeLC+" "+strokeLJ+" "+strokeDA);
		gr.appendChild(ob);
	}
	else
	{
		multiLine ml = ScMW->doc->MLineStyles[item->NamedLStyle];
		for (int it = ml.size()-1; it > -1; it--)
		{
			if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
			{
				ob = docu->createElement("path");
				ob.setAttribute("d", SetClipPath(item)+"Z");
				ob.setAttribute("style", "fill:none; "+GetMultiStroke(&ml[it], item));
				gr.appendChild(ob);
			}
		}
	}
	elem->appendChild(gr);
}

void SVGExPlug::ProcessItem_Line(PageItem* item, Page* page, QDomDocument *docu, QDomElement *elem)
{
	QString fill     = ProcessFill(item, docu, elem);
	QString stroke   = ProcessStroke(item);
	QString strokeLC = ProcessStrokeLineCap(item);
	QString strokeLJ = ProcessStrokeLineJoin(item);
	QString strokeDA = ProcessStrokeDashArray(item);
	QString strokeW  = ProcessStrokeWidth(item);
	QString trans    = ProcessTransform(item, page);
	QDomElement ob;
	QDomElement gr = docu->createElement("g");
	gr.setAttribute("transform", trans);
	if (item->NamedLStyle.isEmpty())
	{
		gr.setAttribute("style", fill+" "+stroke+" "+strokeW+" "+strokeLC+" "+strokeLJ+" "+strokeDA);
		ob = docu->createElement("path");
		ob.setAttribute("d", "M 0 0 L "+FToStr(item->width())+" 0");
		gr.appendChild(ob);
	}
	else
	{
		multiLine ml = ScMW->doc->MLineStyles[item->NamedLStyle];
		for (int it = ml.size()-1; it > -1; it--)
		{
			if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
			{
				ob = docu->createElement("path");
				ob.setAttribute("d", "M 0 0 L "+FToStr(item->width())+" 0");
				ob.setAttribute("style", GetMultiStroke(&ml[it], item));
				gr.appendChild(ob);
			}
		}
	}
	elem->appendChild(gr);
}

void SVGExPlug::ProcessItem_PathText(PageItem* item, Page* page, QDomDocument *docu, QDomElement *elem)
{
	QString chx;
	ScText *hl;
	QString stroke   = ProcessStroke(item);
	QString strokeLC = ProcessStrokeLineCap(item);
	QString strokeLJ = ProcessStrokeLineJoin(item);
	QString strokeDA = ProcessStrokeDashArray(item);
	QString strokeW  = ProcessStrokeWidth(item);
	QString trans    = ProcessTransform(item, page);
	QDomElement ob, tp, tp2;
	QDomText tp1;
	QDomElement gr = docu->createElement("g");
	gr.setAttribute("transform", trans);
	if (item->NamedLStyle.isEmpty())
		gr.setAttribute("style", "fill:none; "+stroke+" "+strokeW+" "+strokeLC+" "+strokeLJ+" "+strokeDA);
	if (item->PoShow)
	{
		if (item->NamedLStyle.isEmpty())
		{
			ob = docu->createElement("path");
			ob.setAttribute("d", SetClipPath(item));
			gr.appendChild(ob);
		}
		else
		{
			multiLine ml = ScMW->doc->MLineStyles[item->NamedLStyle];
			for (int it = ml.size()-1; it > -1; it--)
			{
				if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
				{
					ob = docu->createElement("path");
					ob.setAttribute("d", SetClipPath(item));
					ob.setAttribute("style", GetMultiStroke(&ml[it], item));
					gr.appendChild(ob);
				}
			}
		}
	}
	ob = docu->createElement("text");
	for (uint d = 0; d < item->MaxChars; d++)
	{
		hl = item->itemText.at(d);
		if ((hl->ch == QChar(13)) || (hl->ch == QChar(10)) || (hl->ch == QChar(9)) || (hl->ch == QChar(25)) || (hl->ch == QChar(28)))
			continue;
		if (hl->ch == QChar(29))
			chx = " ";
		else
			chx = hl->ch;
		tp = docu->createElement("tspan");
		tp.setAttribute("x", FToStr(hl->PtransX)+"pt");
		tp.setAttribute("y", FToStr(hl->PtransY)+"pt");
		tp.setAttribute("rotate", hl->PRot);
		tp2 = docu->createElement("tspan");
		tp2.setAttribute("dx", FToStr(hl->xp)+"pt");
		tp2.setAttribute("dy", FToStr(hl->yp)+"pt");
		SetTextProps(&tp2, hl);
		tp1 = docu->createTextNode(chx);
		tp2.appendChild(tp1);
		tp.appendChild(tp2);
		ob.appendChild(tp);
	}
	gr.appendChild(ob);
	elem->appendChild(gr);
}

void SVGExPlug::ProcessItem_Polygon(PageItem* item, Page* page, QDomDocument *docu, QDomElement *elem)
{
	QString fill     = ProcessFill(item, docu, elem);
	QString stroke   = ProcessStroke(item);
	QString strokeLC = ProcessStrokeLineCap(item);
	QString strokeLJ = ProcessStrokeLineJoin(item);
	QString strokeDA = ProcessStrokeDashArray(item);
	QString strokeW  = ProcessStrokeWidth(item);
	QString trans    = ProcessTransform(item, page);
	QDomElement ob;
	QDomElement gr = docu->createElement("g");
	gr.setAttribute("transform", trans);
	if (item->NamedLStyle.isEmpty())
	{
		gr.setAttribute("style", fill+" "+stroke+" "+strokeW+" "+strokeLC+" "+strokeLJ+" "+strokeDA);
		ob = docu->createElement("path");
		ob.setAttribute("d", SetClipPath(item)+"Z");
		gr.appendChild(ob);
	}
	else
	{
		ob = docu->createElement("path");
		ob.setAttribute("d", SetClipPath(item)+"Z");
		ob.setAttribute("style", fill);
		gr.appendChild(ob);
		multiLine ml = ScMW->doc->MLineStyles[item->NamedLStyle];
		for (int it = ml.size()-1; it > -1; it--)
		{
			if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
			{
				ob = docu->createElement("path");
				ob.setAttribute("d", SetClipPath(item)+"Z");
				ob.setAttribute("style", GetMultiStroke(&ml[it], item));
				gr.appendChild(ob);
			}
		}
	}
	elem->appendChild(gr);
}

void SVGExPlug::ProcessItem_PolyLine(PageItem* item, Page* page, QDomDocument *docu, QDomElement *elem)
{
	QString fill     = ProcessFill(item, docu, elem);
	QString stroke   = ProcessStroke(item);
	QString strokeLC = ProcessStrokeLineCap(item);
	QString strokeLJ = ProcessStrokeLineJoin(item);
	QString strokeDA = ProcessStrokeDashArray(item);
	QString strokeW  = ProcessStrokeWidth(item);
	QString trans    = ProcessTransform(item, page);
	QDomElement ob;
	QDomElement gr = docu->createElement("g");
	gr.setAttribute("transform", trans);
	if (item->NamedLStyle.isEmpty())
	{
		gr.setAttribute("style", fill+" "+stroke+" "+strokeW+" "+strokeLC+" "+strokeLJ+" "+strokeDA);
		ob = docu->createElement("path");
		ob.setAttribute("d", SetClipPath(item));
		gr.appendChild(ob);
	}
	else
	{
		multiLine ml = ScMW->doc->MLineStyles[item->NamedLStyle];
		for (int it = ml.size()-1; it > -1; it--)
		{
			if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
			{
				ob = docu->createElement("path");
				ob.setAttribute("d", SetClipPath(item));
				ob.setAttribute("style", GetMultiStroke(&ml[it], item));
				gr.appendChild(ob);
			}
		}
	}
	elem->appendChild(gr);
}

void SVGExPlug::ProcessItem_TextFrame(PageItem* item, Page* page, QDomDocument *docu, QDomElement *elem)
{
	QString chx;
	ScText *hl;
	QDomText tp1;
	QString fill     = ProcessFill(item, docu, elem);
	QString stroke   = ProcessStroke(item);
	QString strokeLC = ProcessStrokeLineCap(item);
	QString strokeLJ = ProcessStrokeLineJoin(item);
	QString strokeDA = ProcessStrokeDashArray(item);
	QString strokeW  = ProcessStrokeWidth(item);
	QString trans    = ProcessTransform(item, page);
	QDomElement ob, tp;
	QDomElement gr = docu->createElement("g");
	gr.setAttribute("transform", trans);
	if ((item->fillColor() != CommonStrings::None) || (item->GrType != 0))
	{
		ob = docu->createElement("path");
		ob.setAttribute("d", SetClipPath(item)+"Z");
		ob.setAttribute("style", fill);
		gr.appendChild(ob);
	}
	ob = docu->createElement("text");
	for (uint d = 0; d < item->MaxChars; d++)
	{
		hl = item->itemText.at(d);
		if ((hl->ch == QChar(13)) || (hl->ch == QChar(10)) || (hl->ch == QChar(9)) || (hl->ch == QChar(28)))
			continue;
		if (hl->yp == 0)
			break;
		if (hl->ch == QChar(29))
			chx = " ";
		else
			chx = hl->ch;
		tp = docu->createElement("tspan");
		tp.setAttribute("x", FToStr(hl->xp)+"pt");
		tp.setAttribute("y", FToStr(hl->yp)+"pt");
		SetTextProps(&tp, hl);
		tp1 = docu->createTextNode(chx);
		tp.appendChild(tp1);
		ob.appendChild(tp);
	}
	gr.appendChild(ob);
	if (item->NamedLStyle.isEmpty())
	{
		ob = docu->createElement("path");
		ob.setAttribute("d", SetClipPath(item)+"Z");
		ob.setAttribute("style", "fill:none; "+stroke+" "+strokeW+" "+strokeLC+" "+strokeLJ+" "+strokeDA);
		gr.appendChild(ob);
	}
	else
	{
		multiLine ml = ScMW->doc->MLineStyles[item->NamedLStyle];
		for (int it = ml.size()-1; it > -1; it--)
		{
			if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
			{
				ob = docu->createElement("path");
				ob.setAttribute("d", SetClipPath(item)+"Z");
				ob.setAttribute("style", "fill:none; "+GetMultiStroke(&ml[it], item));
				gr.appendChild(ob);
			}
		}
	}
	elem->appendChild(gr);
}

QString SVGExPlug::ProcessFill(PageItem* item, QDomDocument *docu, QDomElement *elem)
{
	QString fill;
	QString gradientID = QString("Grad%1").arg(GradCount);
	if ((item->fillColor() != CommonStrings::None) || (item->GrType != 0))
	{
		fill = "fill:"+SetFarbe(item->fillColor(), item->fillShade())+";";
		if (item->GrType != 0)
		{
			QDomElement grad;
			QDomElement defi = docu->createElement("defs");
			if ((item->GrType == 5) || (item->GrType == 7))
				grad = docu->createElement("radialGradient");
			else
				grad = docu->createElement("linearGradient");
			grad.setAttribute("id", gradientID);
			grad.setAttribute("gradientUnits", "userSpaceOnUse");
			switch (item->GrType)
			{
				case 1:
					grad.setAttribute("x1", "0");
					grad.setAttribute("y1", FToStr(item->height() / 2));
					grad.setAttribute("x2", FToStr(item->width()));
					grad.setAttribute("y2", FToStr(item->height() / 2));
					break;
				case 2:
					grad.setAttribute("x1", FToStr(item->width()/ 2));
					grad.setAttribute("y1", "0");
					grad.setAttribute("x2", FToStr(item->width()/ 2));
					grad.setAttribute("y2", FToStr(item->height()));
					break;
				case 3:
					grad.setAttribute("x1", "0");
					grad.setAttribute("y1", "0");
					grad.setAttribute("x2", FToStr(item->width()));
					grad.setAttribute("y2", FToStr(item->height()));
					break;
				case 4:
					grad.setAttribute("x1", "0");
					grad.setAttribute("y1", FToStr(item->height()));
					grad.setAttribute("x2", FToStr(item->width()));
					grad.setAttribute("y2", "0");
					break;
				case 5:
					grad.setAttribute("r", FToStr(QMAX(item->width() / 2, item->height() / 2)));
					grad.setAttribute("cx", FToStr(item->width() / 2));
					grad.setAttribute("cy", FToStr(item->height() / 2));
					break;
				case 6:
					grad.setAttribute("x1", FToStr(item->GrStartX));
					grad.setAttribute("y1", FToStr(item->GrStartY));
					grad.setAttribute("x2", FToStr(item->GrEndX));
					grad.setAttribute("y2", FToStr(item->GrEndY));
					break;
				case 7:
					grad.setAttribute("r", FToStr(QMAX(item->width() / 2, item->height() / 2)));
					grad.setAttribute("cx", FToStr(item->GrStartX));
					grad.setAttribute("cy", FToStr(item->GrStartY));
					break;
			}
			QPtrVector<VColorStop> cstops = item->fill_gradient.colorStops();
			for (uint cst = 0; cst < item->fill_gradient.Stops(); ++cst)
			{
				QDomElement itcl = docu->createElement("stop");
				itcl.setAttribute("offset", FToStr(cstops.at(cst)->rampPoint*100)+"%");
				itcl.setAttribute("stop-opacity", FToStr(cstops.at(cst)->opacity));
				itcl.setAttribute("stop-color", SetFarbe(cstops.at(cst)->name, cstops.at(cst)->shade));
				grad.appendChild(itcl);
			}
			defi.appendChild(grad);
			elem->appendChild(defi);
			fill = "fill:url(#"+gradientID+");";
			GradCount++;
		}
		if (item->fillRule)
			fill += " fill-rule:evenodd;";
		else
			fill += " fill-rule:nonzero;";
		if (item->fillTransparency() != 0)
			fill += " fill-opacity:"+FToStr(1.0 - item->fillTransparency())+";";
	}
	else
		fill = "fill:none;";
	return fill;
}

QString SVGExPlug::ProcessStroke(PageItem* item)
{
	QString stroke;
	if (item->lineColor() != CommonStrings::None)
	{
		stroke = "stroke:"+SetFarbe(item->lineColor(), item->lineShade())+";";
		if (item->lineTransparency() != 0)
			stroke += " stroke-opacity:"+FToStr(1.0 - item->lineTransparency())+";";
	}
	else
		stroke = "stroke:none;";
	return stroke;
}

QString SVGExPlug::ProcessStrokeLineCap(PageItem* item)
{
	QString strokeLC = "stroke-linecap:";
	switch (item->PLineEnd)
	{
		case Qt::FlatCap:
			strokeLC += "butt;";
			break;
		case Qt::SquareCap:
			strokeLC += "square;";
			break;
		case Qt::RoundCap:
			strokeLC += "round;";
			break;
		default:
			strokeLC += "butt;";
			break;
	}
	return strokeLC;
}

QString SVGExPlug::ProcessStrokeLineJoin(PageItem* item)
{
	QString strokeLJ = "stroke-linejoin:";
	switch (item->PLineJoin)
	{
		case Qt::MiterJoin:
			strokeLJ += "miter;";
			break;
		case Qt::BevelJoin:
			strokeLJ += "bevel;";
			break;
		case Qt::RoundJoin:
			strokeLJ += "round;";
			break;
		default:
			strokeLJ += "miter;";
			break;
	}
	return strokeLJ;
}

QString SVGExPlug::ProcessStrokeDashArray(PageItem* item)
{
	QString strokeDA = "stroke-dasharray:";
	if (item->DashValues.count() != 0)
	{
		QValueList<double>::iterator it;
		for ( it = item->DashValues.begin(); it != item->DashValues.end(); ++it )
		{
			strokeDA += IToStr(static_cast<int>(*it))+" ";
		}
		strokeDA += "; stroke-dashoffset:"+IToStr(static_cast<int>(item->DashOffset))+";";
	}
	else
	{
		QString Dt = FToStr(QMAX(2*item->lineWidth(), 1));
		QString Da = FToStr(QMAX(6*item->lineWidth(), 1));
		switch (item->PLineArt)
		{
			case Qt::SolidLine:
				strokeDA += "none;";
				break;
			case Qt::DashLine:
				strokeDA += Da+","+Dt+";";
				break;
			case Qt::DotLine:
				strokeDA += Dt+";";
				break;
			case Qt::DashDotLine:
				strokeDA += Da+","+Dt+","+Dt+","+Dt+";";
				break;
			case Qt::DashDotDotLine:
				strokeDA += Da+","+Dt+","+Dt+","+Dt+","+Dt+","+Dt+";";
				break;
			default:
				strokeDA += "none;";
				break;
		}
	}
	return strokeDA;
}

QString SVGExPlug::ProcessStrokeWidth(PageItem* item)
{
	QString strokeW = "stroke-width:"+FToStr(item->lineWidth())+"pt;";
	return strokeW;
}

QString SVGExPlug::ProcessTransform(PageItem* item, Page* page)
{
	QString trans = "translate("+FToStr(item->xPos()-page->xOffset())+", "+FToStr(item->yPos()-page->yOffset())+")";
	if (item->rotation() != 0)
		trans += " rotate("+FToStr(item->rotation())+")";
	return trans;
}

QString SVGExPlug::SetClipPath(PageItem *ite)
{
	QString tmp = "";
	FPoint np, np1, np2;
	bool nPath = true;
	if (ite->PoLine.size() > 3)
	{
		for (uint poi=0; poi<ite->PoLine.size()-3; poi += 4)
		{
			if (ite->PoLine.point(poi).x() > 900000)
			{
				tmp += "Z ";
				nPath = true;
				continue;
			}
			if (nPath)
			{
				np = ite->PoLine.point(poi);
				tmp += "M"+FToStr(np.x())+" "+FToStr(np.y())+" ";
				nPath = false;
			}
			np = ite->PoLine.point(poi+1);
			tmp += "C"+FToStr(np.x())+" "+FToStr(np.y())+" ";
			np1 = ite->PoLine.point(poi+3);
			tmp += FToStr(np1.x())+" "+FToStr(np1.y())+" ";
			np2 = ite->PoLine.point(poi+2);
			tmp += FToStr(np2.x())+" "+FToStr(np2.y())+" ";
		}
	}
	return tmp;
}

QString SVGExPlug::SetClipPathImage(PageItem *ite)
{
	QString tmp = "";
	FPoint np, np1, np2;
	bool nPath = true;
	if (ite->imageClip.size() > 3)
	{
		for (uint poi=0; poi<ite->imageClip.size()-3; poi += 4)
		{
			if (ite->imageClip.point(poi).x() > 900000)
			{
				tmp += "Z ";
				nPath = true;
				continue;
			}
			if (nPath)
			{
				np = ite->imageClip.point(poi);
				tmp += "M"+FToStr(np.x())+" "+FToStr(np.y())+" ";
				nPath = false;
			}
			np = ite->imageClip.point(poi+1);
			tmp += "C"+FToStr(np.x())+" "+FToStr(np.y())+" ";
			np1 = ite->imageClip.point(poi+3);
			tmp += FToStr(np1.x())+" "+FToStr(np1.y())+" ";
			np2 = ite->imageClip.point(poi+2);
			tmp += FToStr(np2.x())+" "+FToStr(np2.y())+" ";
		}
	}
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

void SVGExPlug::SetTextProps(QDomElement *tp, ScText *hl)
{
	int chst = hl->cstyle & 127;
	if (hl->ccolor != CommonStrings::None)
		tp->setAttribute("fill", SetFarbe(hl->ccolor, hl->cshade));
	else
		tp->setAttribute("fill", "none");
	if ((hl->cstroke != CommonStrings::None) && (chst & 4))
		{
		tp->setAttribute("stroke", SetFarbe(hl->cstroke, hl->cshade2));
		tp->setAttribute("stroke-width", FToStr((*ScMW->doc->AllFonts)[hl->cfont->scName()]->strokeWidth * (hl->csize / 10.0))+"pt");
		}
	else
		tp->setAttribute("stroke", "none");
	tp->setAttribute("font-size", (hl->csize / 10.0));
	tp->setAttribute("font-family", (*ScMW->doc->AllFonts)[hl->cfont->scName()]->family());
	if (chst != 0)
		{
		if (chst & 64)
			tp->setAttribute("font-variant", "small-caps");
		if (chst & 32)
			tp->setAttribute("font-weight", "bold");
		if (chst & 16)
			tp->setAttribute("text-decoration", "line-through");
		if (chst & 8)
			tp->setAttribute("text-decoration", "underline");
		}
}

QString SVGExPlug::SetFarbe(QString farbe, int shad)
{
	return ScMW->doc->PageColors[farbe].getShadeColorProof(shad).name();
}

QString SVGExPlug::GetMultiStroke(struct SingleLine *sl, PageItem *Item)
{
	QString tmp = "fill:none; ";
	tmp += "stroke:"+SetFarbe(sl->Color, sl->Shade)+"; ";
	if (Item->fillTransparency() != 0)
		tmp += " stroke-opacity:"+FToStr(1.0 - Item->fillTransparency())+"; ";
	tmp += "stroke-width:"+FToStr(sl->Width)+"pt; ";
	tmp += "stroke-linecap:";
	switch (static_cast<PenCapStyle>(sl->LineEnd))
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
	switch (static_cast<PenJoinStyle>(sl->LineJoin))
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
	QString Dt = FToStr(QMAX(2*sl->Width, 1));
	QString Da = FToStr(QMAX(6*sl->Width, 1));
	switch (static_cast<PenStyle>(sl->Dash))
		{
		case Qt::SolidLine:
			tmp += "none;";
			break;
		case Qt::DashLine:
			tmp += Da+","+Dt+";";
			break;
		case Qt::DotLine:
			tmp += Dt+";";
			break;
		case Qt::DashDotLine:
			tmp += Da+","+Dt+","+Dt+","+Dt+";";
			break;
		case Qt::DashDotDotLine:
			tmp += Da+","+Dt+","+Dt+","+Dt+","+Dt+","+Dt+";";
			break;
		default:
			tmp += "none;";
			break;
		}
	return tmp;
}

SVGExPlug::~SVGExPlug()
{
}

#include "svgexplugin.moc"
