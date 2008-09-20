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
#include <QList>

#include "svgexplugin.h"

#include "scconfig.h"
#include "canvas.h"
#include "cmsettings.h"
#include "commonstrings.h"
#include "customfdialog.h"
#include "scribuscore.h"
#include "page.h"
#include "scgzfile.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "scmessagebox.h"
#include "util.h"
#include "customfdialog.h"
#include "sctextstruct.h"
#include "guidemanager.h"
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
		CustomFDialog *openDia = new CustomFDialog(doc->scMW(), wdir, QObject::tr("Save as"), QObject::tr("%1;;All Files (*)").arg(FormatsManager::instance()->extensionsForFormat(FormatsManager::SVG)), fdHidePreviewCheckBox | fdCompressFile);
		openDia->setSelection(getFileNameByPage(doc, doc->currentPage()->pageNr(), "svg"));
		openDia->setExtension("svg");
		openDia->setZipExtension("svgz");
		if (openDia->exec())
		{
			if (openDia->SaveZip->isChecked())
				openDia->handleCompress();
			fileName = openDia->selectedFile();
		}
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
			dia->doExport(fileName);
			delete dia;
		}
		else
			return true;
	}
	return true;
}

SVGExPlug::SVGExPlug( ScribusDoc* doc )
{
	m_Doc=doc;
	m_View=m_Doc->view();
	m_ScMW=m_Doc->scMW();
}

bool SVGExPlug::doExport( QString fName )
{
#ifdef USECAIRO
	Page *Seite = m_Doc->currentPage();
	int clipx = static_cast<int>(Seite->xOffset());
	int clipy = static_cast<int>(Seite->yOffset());
	int clipw = qRound(Seite->width());
	int cliph = qRound(Seite->height());
	double sca = m_View->scale();
	double cx = m_Doc->minCanvasCoordinate.x();
	double cy = m_Doc->minCanvasCoordinate.y();
	m_Doc->minCanvasCoordinate = FPoint(0, 0);
	bool frs = m_Doc->guidesSettings.framesShown;
	bool ctrls = m_Doc->guidesSettings.showControls;
	m_Doc->guidesSettings.framesShown = false;
	m_Doc->guidesSettings.showControls = false;
	m_View->setScale(1.0);
	m_View->m_canvas->setPreviewMode(true);
	ScPainter *painter = new ScPainter(fName, clipw, cliph, 1.0, 0);
	painter->clear(m_Doc->papColor);
	painter->translate(-clipx, -clipy);
	painter->setFillMode(ScPainter::Solid);
	m_View->m_canvas->DrawMasterItems(painter, Seite, QRect(clipx, clipy, clipw, cliph));
	m_View->m_canvas->DrawPageItems(painter, QRect(clipx, clipy, clipw, cliph));
	painter->end();
	m_Doc->guidesSettings.framesShown = frs;
	m_Doc->guidesSettings.showControls = ctrls;
	m_View->setScale(sca);
	delete painter;
	m_View->m_canvas->setPreviewMode(false);
	m_Doc->minCanvasCoordinate = FPoint(cx, cy);
#else
	QDomDocument docu("svgdoc");
	QString vo = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	QString st = "<svg></svg>";
	docu.setContent(st);
	double pageWidth  = m_Doc->pageWidth;
	double pageHeight = m_Doc->pageHeight;
	QDomElement elem = docu.documentElement();
	elem.setAttribute("width", FToStr(m_Doc->pageWidth)+"pt");
	elem.setAttribute("height", FToStr(m_Doc->pageHeight)+"pt");
	elem.setAttribute("viewBox", QString("0 0 %1 %2").arg(pageWidth).arg(pageHeight));
	elem.setAttribute("xmlns", "http://www.w3.org/2000/svg");
	elem.setAttribute("xmlns:xlink","http://www.w3.org/1999/xlink");
	Page *Seite;
	GradCount = 0;
	ClipCount = 0;
	Seite = m_Doc->MasterPages.at(m_Doc->MasterNames[m_Doc->currentPage()->MPageNam]);
	ProcessPage(Seite, &docu, &elem);
	Seite = m_Doc->currentPage();
	ProcessPage(Seite, &docu, &elem);
	if(fName.right(2) == "gz")
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
#endif
	return true;
}

#ifndef USECAIRO
void SVGExPlug::ProcessPage(Page *Seite, QDomDocument *docu, QDomElement *elem)
{
	QString tmp, trans, fill, stroke, strokeW, strokeLC, strokeLJ, strokeDA, gradi, Clipi, chx;
	int d;
	ScText *hl;
	int Lnr = 0;
	ScLayer ll;
	ll.isPrintable = false;
	ll.LNr = 0;
	QDomElement ob, gr, tp, tp2, defi, grad;
	QDomText tp1;
	PageItem *Item;
	gradi = "Grad";
	Clipi = "Clip";
	QList<PageItem*> Items;
	Page* SavedAct = m_Doc->currentPage();
	m_Doc->setCurrentPage(Seite);
	if (Seite->pageName().isEmpty())
		Items = m_Doc->DocItems;
	else
		Items = m_Doc->MasterItems;
	for (int la = 0; la < m_Doc->Layers.count(); la++)
	{
		m_Doc->Layers.levelToLayer(ll, Lnr);
		if (ll.isPrintable)
		{
			for(int j = 0; j < Items.count(); ++j)
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
				if (!( qMax( x, x2 ) <= qMin( x+w, x2+w2 ) && qMax( y, y2 ) <= qMin( y+h, y2+h2 )))
					continue;
				if ((Item->fillColor() != CommonStrings::None) || (Item->GrType != 0))
				{
					fill = "fill:"+SetColor(Item->fillColor(), Item->fillShade())+";";
					if (Item->GrType != 0)
					{
						defi = docu->createElement("defs");
						if ((Item->GrType == 5) || (Item->GrType == 7))
							grad = docu->createElement("radialGradient");
						else
							grad = docu->createElement("linearGradient");
						grad.setAttribute("id", gradi+IToStr(GradCount));
						grad.setAttribute("gradientUnits", "userSpaceOnUse");
						switch (Item->GrType)
						{
							case 1:
								grad.setAttribute("x1", "0");
								grad.setAttribute("y1", FToStr(Item->height() / 2));
								grad.setAttribute("x2", FToStr(Item->width()));
								grad.setAttribute("y2", FToStr(Item->height() / 2));
								break;
							case 2:
								grad.setAttribute("x1", FToStr(Item->width()/ 2));
								grad.setAttribute("y1", "0");
								grad.setAttribute("x2", FToStr(Item->width()/ 2));
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
								grad.setAttribute("r", FToStr(qMax(Item->width() / 2, Item->height() / 2)));
								grad.setAttribute("cx", FToStr(Item->width() / 2));
								grad.setAttribute("cy", FToStr(Item->height() / 2));
								break;
							case 6:
								grad.setAttribute("x1", FToStr(Item->GrStartX));
								grad.setAttribute("y1", FToStr(Item->GrStartY));
								grad.setAttribute("x2", FToStr(Item->GrEndX));
								grad.setAttribute("y2", FToStr(Item->GrEndY));
								break;
							case 7:
								grad.setAttribute("r", FToStr(qMax(Item->width() / 2, Item->height() / 2)));
								grad.setAttribute("cx", FToStr(Item->GrStartX));
								grad.setAttribute("cy", FToStr(Item->GrStartY));
								break;
						}
						QList<VColorStop*> cstops = Item->fill_gradient.colorStops();
						for (int cst = 0; cst < Item->fill_gradient.Stops(); ++cst)
						{
							QDomElement itcl = docu->createElement("stop");
							itcl.setAttribute("offset", FToStr(cstops.at(cst)->rampPoint*100)+"%");
							itcl.setAttribute("stop-opacity", FToStr(cstops.at(cst)->opacity));
							itcl.setAttribute("stop-color", SetColor(cstops.at(cst)->name, cstops.at(cst)->shade));
							grad.appendChild(itcl);
						}
						defi.appendChild(grad);
						fill = "fill:url(#"+gradi+IToStr(GradCount)+");";
						GradCount++;
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
				if (Item->lineColor() != CommonStrings::None)
				{
					stroke = "stroke:"+SetColor(Item->lineColor(), Item->lineShade())+";";
					if (Item->lineTransparency() != 0)
						stroke += " stroke-opacity:"+FToStr(1.0 - Item->lineTransparency())+";";
				}
				else
					stroke = "stroke:none;";
				trans = "translate("+FToStr(Item->xPos()-Seite->xOffset())+", "+FToStr(Item->yPos()-Seite->yOffset())+")";
				if (Item->rotation() != 0)
					trans += " rotate("+FToStr(Item->rotation())+")";
				strokeW = "stroke-width:"+FToStr(Item->lineWidth())+";";
				strokeLC = "stroke-linecap:";
				switch (Item->PLineEnd)
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
				strokeLJ = "stroke-linejoin:";
				switch (Item->PLineJoin)
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
				strokeDA = "stroke-dasharray:";
				if (Item->DashValues.count() != 0)
				{
					QVector<double>::iterator it;
					for ( it = Item->DashValues.begin(); it != Item->DashValues.end(); ++it )
					{
						strokeDA += IToStr(static_cast<int>(*it))+" ";
					}
					strokeDA += "; stroke-dashoffset:"+IToStr(static_cast<int>(Item->DashOffset))+";";
				}
				else
				{
					if (Item->PLineArt == Qt::SolidLine)
						strokeDA += "none;";
					else
					{
						QString Da = getDashString(Item->PLineArt, Item->lineWidth());
						if (Da.isEmpty())
							strokeDA += "none;";
						else
							strokeDA += Da.replace(" ", ", ")+";";
					}
				}
				gr = docu->createElement("g");
				gr.setAttribute("transform", trans);
				if (!Item->asTextFrame())
				{
					if (Item->NamedLStyle.isEmpty())
					{
						if (Item->asPathText())
							gr.setAttribute("style", "fill:none; "+stroke+" "+strokeW+" "+strokeLC+" "+strokeLJ+" "+strokeDA);
						else
							gr.setAttribute("style", fill+" "+stroke+" "+strokeW+" "+strokeLC+" "+strokeLJ+" "+strokeDA);
					}
				}
				switch (Item->itemType())
				{
				case PageItem::Polygon:
					if (Item->NamedLStyle.isEmpty())
					{
						ob = docu->createElement("path");
						ob.setAttribute("d", SetClipPath(Item)+"Z");
					}
					else
					{
						ob = docu->createElement("path");
						ob.setAttribute("d", SetClipPath(Item)+"Z");
						ob.setAttribute("style", fill);
						gr.appendChild(ob);
						multiLine ml = m_Doc->MLineStyles[Item->NamedLStyle];
						for (int it = ml.size()-1; it > -1; it--)
						{
							if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
							{
								ob = docu->createElement("path");
								ob.setAttribute("d", SetClipPath(Item)+"Z");
								ob.setAttribute("style", GetMultiStroke(&ml[it], Item));
								gr.appendChild(ob);
							}
						}
					}
					break;
				case PageItem::ImageFrame:
					if ((Item->fillColor() != CommonStrings::None) || (Item->GrType != 0))
					{
						ob = docu->createElement("path");
						ob.setAttribute("d", SetClipPath(Item)+"Z");
						ob.setAttribute("style", fill);
						gr.appendChild(ob);
					}
					if ((Item->PicAvail) && (!Item->Pfile.isEmpty()))
					{
						ob = docu->createElement("clipPath");
						ob.setAttribute("id", Clipi+IToStr(ClipCount));
						ob.setAttribute("clipPathUnits", "userSpaceOnUse");
						ob.setAttribute("clip-rule", "evenodd");
						QDomElement cl = docu->createElement("path");
						if (Item->imageClip.size() != 0)
							cl.setAttribute("d", SetClipPathImage(Item)+"Z");
						else
							cl.setAttribute("d", SetClipPath(Item)+"Z");
						ob.appendChild(cl);
						gr.appendChild(ob);
						ScImage img;
						CMSettings cms(m_Doc, Item->IProfile, Item->IRender);
						img.LoadPicture(Item->Pfile, Item->pixm.imgInfo.actualPageNumber, cms, Item->UseEmbedded, true, ScImage::RGBProof, 72);
						img.applyEffect(Item->effectsInUse, m_Doc->PageColors, true);
						QFileInfo fi = QFileInfo(Item->Pfile);
						img.qImage().save(fi.baseName()+".png", "PNG");
						ob = docu->createElement("image");
						ob.setAttribute("clip-path", "url(#"+Clipi+IToStr(ClipCount)+")");
						ob.setAttribute("xlink:href", fi.baseName()+".png");
						ob.setAttribute("x", "0pt");
						ob.setAttribute("y", "0pt");
						ob.setAttribute("width", FToStr(Item->width()));
						ob.setAttribute("height", FToStr(Item->height()));
						ClipCount++;
						gr.appendChild(ob);
					}
					if (Item->NamedLStyle.isEmpty())
					{
						ob = docu->createElement("path");
						ob.setAttribute("d", SetClipPath(Item)+"Z");
						ob.setAttribute("style", "fill:none; "+stroke+" "+strokeW+" "+strokeLC+" "+strokeLJ+" "+strokeDA);
					}
					else
					{
						multiLine ml = m_Doc->MLineStyles[Item->NamedLStyle];
						for (int it = ml.size()-1; it > -1; it--)
						{
							if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
							{
								ob = docu->createElement("path");
								ob.setAttribute("d", SetClipPath(Item)+"Z");
								ob.setAttribute("style", "fill:none; "+GetMultiStroke(&ml[it], Item));
								gr.appendChild(ob);
							}
						}
					}
					break;
				case PageItem::PolyLine:
					if (Item->NamedLStyle.isEmpty())
					{
						ob = docu->createElement("path");
						ob.setAttribute("d", SetClipPath(Item));
					}
					else
					{
						multiLine ml = m_Doc->MLineStyles[Item->NamedLStyle];
						for (int it = ml.size()-1; it > -1; it--)
						{
							if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
							{
								ob = docu->createElement("path");
								ob.setAttribute("d", SetClipPath(Item));
								ob.setAttribute("style", GetMultiStroke(&ml[it], Item));
								gr.appendChild(ob);
							}
						}
					}
					break;
				case PageItem::TextFrame:
					if ((Item->fillColor() != CommonStrings::None) || (Item->GrType != 0))
					{
						ob = docu->createElement("path");
						ob.setAttribute("d", SetClipPath(Item)+"Z");
						ob.setAttribute("style", fill);
						gr.appendChild(ob);
					}
					ob = docu->createElement("text");
					for (d = 0; d < Item->itemText.length() && ! Item->frameDisplays(d); ++d)
						;
					for (; d < Item->itemText.length() && Item->frameDisplays(d); ++d)
					{
						hl = Item->itemText.item(d);
						if ((hl->ch == QChar(13)) || (hl->ch == QChar(10)) || (hl->ch == QChar(9)) || (hl->ch == QChar(28)))
							continue;
						if (hl->glyph.yoffset == 0)
							break;
						if (hl->ch == QChar(29))
							chx = " ";
						else
							chx = hl->ch;
						tp = docu->createElement("tspan");
						tp.setAttribute("x", FToStr(hl->glyph.xoffset));
						tp.setAttribute("y", FToStr(hl->glyph.yoffset));
						SetTextProps(&tp, hl);
						tp1 = docu->createTextNode(chx);
						tp.appendChild(tp1);
						ob.appendChild(tp);
					}
					if (Item->NamedLStyle.isEmpty())
					{
						ob = docu->createElement("path");
						ob.setAttribute("d", SetClipPath(Item)+"Z");
						ob.setAttribute("style", "fill:none; "+stroke+" "+strokeW+" "+strokeLC+" "+strokeLJ+" "+strokeDA);
					}
					else
					{
						multiLine ml = m_Doc->MLineStyles[Item->NamedLStyle];
						for (int it = ml.size()-1; it > -1; it--)
						{
							if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
							{
								ob = docu->createElement("path");
								ob.setAttribute("d", SetClipPath(Item)+"Z");
								ob.setAttribute("style", "fill:none; "+GetMultiStroke(&ml[it], Item));
								gr.appendChild(ob);
							}
						}
					}
					break;
				case PageItem::Line:
					if (Item->NamedLStyle.isEmpty())
					{
						ob = docu->createElement("path");
						ob.setAttribute("d", "M 0 0 L "+FToStr(Item->width())+" 0");
					}
					else
					{
						multiLine ml = m_Doc->MLineStyles[Item->NamedLStyle];
						for (int it = ml.size()-1; it > -1; it--)
						{
							if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
							{
								ob = docu->createElement("path");
								ob.setAttribute("d", "M 0 0 L "+FToStr(Item->width())+" 0");
								ob.setAttribute("style", GetMultiStroke(&ml[it], Item));
								gr.appendChild(ob);
							}
						}
					}
					break;
				case PageItem::PathText:
					if (Item->PoShow)
					{
						if (Item->NamedLStyle.isEmpty())
						{
							ob = docu->createElement("path");
							ob.setAttribute("d", SetClipPath(Item));
							gr.appendChild(ob);
						}
						else
						{
							multiLine ml = m_Doc->MLineStyles[Item->NamedLStyle];
							for (int it = ml.size()-1; it > -1; it--)
							{
								if ((ml[it].Color != CommonStrings::None) && (ml[it].Width != 0))
								{
									ob = docu->createElement("path");
									ob.setAttribute("d", SetClipPath(Item));
									ob.setAttribute("style", GetMultiStroke(&ml[it], Item));
									gr.appendChild(ob);
								}
							}
						}
					}
					ob = docu->createElement("text");
					for (d = 0; d < Item->itemText.length() && !Item->frameDisplays(d); ++d)
						;
					for (; d < Item->itemText.length() && Item->frameDisplays(d); ++d)
					{
						hl = Item->itemText.item(d);
						if ((hl->ch == QChar(13)) || (hl->ch == QChar(10)) || (hl->ch == QChar(9)) || (hl->ch == QChar(25)) || (hl->ch == QChar(28)))
							continue;
						if (hl->ch == QChar(29))
							chx = " ";
						else
							chx = hl->ch;
						tp = docu->createElement("tspan");
						tp.setAttribute("x", FToStr(hl->PtransX));
						tp.setAttribute("y", FToStr(hl->PtransY));
						tp.setAttribute("rotate", hl->PRot);
						tp2 = docu->createElement("tspan");
						tp2.setAttribute("dx", FToStr(hl->glyph.xoffset));
						tp2.setAttribute("dy", FToStr(hl->glyph.yoffset));
						SetTextProps(&tp2, hl);
						tp1 = docu->createTextNode(chx);
						tp2.appendChild(tp1);
						tp.appendChild(tp2);
						ob.appendChild(tp);
					}
					break;
				default:
					break;
				}
				if (Item->GrType != 0)
					elem->appendChild(defi);
				gr.appendChild(ob);
				elem->appendChild(gr);
			}
		}
		Lnr++;
	}
	m_Doc->setCurrentPage(SavedAct);
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
	int chst = hl->effects() & 127;
	if (hl->fillColor() != CommonStrings::None)
		tp->setAttribute("fill", SetColor(hl->fillColor(), hl->fillShade()));
	else
		tp->setAttribute("fill", "none");
	if ((hl->strokeColor() != CommonStrings::None) && (chst & 4))
	{
		tp->setAttribute("stroke", SetColor(hl->strokeColor(), hl->strokeShade()));
		tp->setAttribute("stroke-width", FToStr(hl->font().strokeWidth(hl->fontSize() / 10.0)));
	}
	else
		tp->setAttribute("stroke", "none");
	tp->setAttribute("font-size", (hl->fontSize() / 10.0));
	tp->setAttribute("font-family", hl->font().family());
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
		tmp += " stroke-opacity:"+FToStr(1.0 - Item->fillTransparency())+"; ";
	tmp += "stroke-width:"+FToStr(sl->Width)+"; ";
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
#endif

SVGExPlug::~SVGExPlug()
{
}

//#include "svgexplugin.moc"
