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
#include "svgexplugin.h"
#include "svgexplugin.moc"

#if (_MSC_VER >= 1200)
 #include "win-config.h"
#else
 #include "config.h"
#endif

#include "customfdialog.h"
#include <qfile.h>
#include <qtextstream.h>
#ifdef HAVE_LIBZ
#include <zlib.h>
#endif
extern void Level2Layer(ScribusDoc *doc, struct Layer *ll, int Level);
extern QString Path2Relative(QString Path);
extern QImage LoadPict(QString fn, bool *gray = 0);

/*!
 \fn QString Name()
 \author Franz Schmid
 \date
 \brief Returns name of plugin
 \param None
 \retval QString containing name of plugin: Save Page as SVG...
 */
QString Name()
{
  return QObject::tr("Save Page as SVG...");
}

/*!
 \fn int Type()
 \author Franz Schmid
 \date
 \brief Returns type of plugin
 \param None
 \retval int containing type of plugin (1: Extra, 2: Import, 3: Export, 4: )
 */
int Type()
{
	return 3;
}

/*!
 \fn void Run(QWidget *d, ScribusApp *plug)
 \author Franz Schmid
 \date
 \brief Run the SVG export
 \param d QWidget *
 \param plug ScribusApp *
 \retval None
 */
void Run(QWidget *d, ScribusApp *plug)
{
	if (plug->HaveDoc)
		{
#ifdef HAVE_LIBZ
		QString fileName = plug->CFileDialog(QObject::tr("Save as"), QObject::tr("SVG-Images (*.svg *.svgz);;All Files (*)"),"", false, false, true);
#else
		QString fileName = plug->CFileDialog(QObject::tr("Save as"), QObject::tr("SVG-Images (*.svg);;All Files (*)"),"", false, false);
#endif
		if (!fileName.isEmpty())
			{
  		QFile f(fileName);
  		if (f.exists())
  			{
  			int exit=QMessageBox::warning(d, QObject::tr("Warning"),
  																		QObject::tr("Do you really want to overwrite the File:\n%1 ?").arg(fileName),
                                			QObject::tr("Yes"),
                                			QObject::tr("No"),
                                			0, 0, 1);
  			if (exit != 0)
  				return;
  			}
  		SVGExPlug *dia = new SVGExPlug(d, plug, fileName);
  		delete dia;
			}
		else
			return;
  	}
}

/*!
 \fn SVGExPlug::SVGExPlug( QWidget* parent, ScribusApp *plug, QString fName )
 \author Franz Schmid
 \date
 \brief Create the SVG exporter window
 \param parent QWidget *
 \param plug ScribusApp *
 \param fName QString
 \retval SVGExPlug plugin
 */
SVGExPlug::SVGExPlug( QWidget* parent, ScribusApp *plug, QString fName )
{
	QDomDocument docu("svgdoc");
	QString vo = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	QString st = "<svg></svg>";
	docu.setContent(st);
	QDomElement elem = docu.documentElement();
	elem.setAttribute("width", plug->doc->PageB);
	elem.setAttribute("height", plug->doc->PageH);
	elem.setAttribute("xmlns", "http://www.w3.org/2000/svg");
	elem.setAttribute("xmlns:xlink","http://www.w3.org/1999/xlink"); 
	Page *Seite;
	GradCount = 0;
	ClipCount = 0;
	Seite = plug->view->MasterPages.at(plug->view->MasterNames[plug->doc->ActPage->MPageNam]);
	ProcessPage(plug, Seite, &docu, &elem);
	Seite = plug->doc->ActPage;
	ProcessPage(plug, Seite, &docu, &elem);
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
		wr += docu.toString().utf8();
		s.writeRawBytes(wr, wr.length());
		f.close();
		}
#else
	QFile f(fName);
	if(!f.open(IO_WriteOnly))
		return;
	QTextStream s(&f);
	QString wr = vo;
	wr += docu.toString().utf8();
	s.writeRawBytes(wr, wr.length());
	f.close();
#endif
}

/*!
 \fn void SVGExPlug::ProcessPage(ScribusApp *plug, Page *Seite, QDomDocument *docu, QDomElement *elem)
 \author Franz Schmid
 \date
 \brief Process a page to export to SVG format
 \param plug ScribusApp
 \param Seite Page *
 \param docu QDomDocument * 
 \param elem QDomElement *
 \retval None
 */
void SVGExPlug::ProcessPage(ScribusApp *plug, Page *Seite, QDomDocument *docu, QDomElement *elem)
{
	QString tmp, trans, fill, stroke, strokeW, strokeLC, strokeLJ, strokeDA, gradi, Clipi, chx;
	uint d;
	struct Pti *hl;
	int Lnr = 0;
	struct Layer ll;
	ll.Drucken = false;
	ll.LNr = 0;
	QDomElement ob, gr, tp, tp2, defi, grad;
	QDomText tp1;
	PageItem *Item;
	gradi = "Grad";
	Clipi = "Clip";
	for (uint la = 0; la < plug->doc->Layers.count(); la++)
		{
		Level2Layer(plug->doc, &ll, Lnr);
		if (ll.Drucken)
			{
			for(uint j = 0; j < Seite->Items.count();j++)
				{
				Item = Seite->Items.at(j);
				if (Item->LayerNr != ll.LNr)
					continue;
				if ((Item->Pcolor != "None") || (Item->GrType != 0))
					{
					fill = "fill:"+SetFarbe(Item->Pcolor, Item->Shade, plug)+";";
					if (Item->GrType != 0)
						{
						defi = docu->createElement("defs");
						if (Item->GrType == 5)
							grad = docu->createElement("radialGradient");
						else
							grad = docu->createElement("linearGradient");
						grad.setAttribute("id", gradi+IToStr(GradCount));
						grad.setAttribute("gradientUnits", "userSpaceOnUse");
						switch (Item->GrType)
							{
							case 1:
								grad.setAttribute("x1", "0");
								grad.setAttribute("y1", FToStr(Item->Height / 2));
								grad.setAttribute("x2", FToStr(Item->Width));
								grad.setAttribute("y2", FToStr(Item->Height / 2));
								break;
							case 2:
								grad.setAttribute("x1", FToStr(Item->Width / 2));
								grad.setAttribute("y1", "0");
								grad.setAttribute("x2", FToStr(Item->Width/ 2));
								grad.setAttribute("y2", FToStr(Item->Height));
								break;
							case 3:
								grad.setAttribute("x1", "0");
								grad.setAttribute("y1", "0");
								grad.setAttribute("x2", FToStr(Item->Width));
								grad.setAttribute("y2", FToStr(Item->Height));
								break;
							case 4:
								grad.setAttribute("x1", FToStr(Item->Width));
								grad.setAttribute("y1", "0");
								grad.setAttribute("x2", "0");
								grad.setAttribute("y2", FToStr(Item->Height));
								break;
							case 5:
								grad.setAttribute("r", FToStr(QMIN(Item->Width / 2, Item->Height / 2)));
								grad.setAttribute("cx", FToStr(Item->Width / 2));
								grad.setAttribute("cy", FToStr(Item->Height / 2));
								break;
							}
						QDomElement s1 = docu->createElement("stop");
						s1.setAttribute("offset","0%");
						if (Item->GrType == 5)
							s1.setAttribute("stop-color",SetFarbe(Item->GrColor, Item->GrShade, plug));
						else
							s1.setAttribute("stop-color",SetFarbe(Item->GrColor2, Item->GrShade2, plug));
						grad.appendChild(s1);
						QDomElement s2 = docu->createElement("stop");
						s2.setAttribute("offset","100%");
						if (Item->GrType == 5)
							s2.setAttribute("stop-color",SetFarbe(Item->GrColor2, Item->GrShade2, plug));
						else
							s2.setAttribute("stop-color",SetFarbe(Item->GrColor, Item->GrShade, plug));
						grad.appendChild(s2);
						defi.appendChild(grad);
						fill = "fill:url(#"+gradi+IToStr(GradCount)+");";
						GradCount++;
						}
					fill += " fill-rule:evenodd;";
					if (Item->Transparency != 0)
						fill += " fill-opacity:"+FToStr(1.0 - Item->Transparency)+";"; 
					}
				else
					fill = "fill:none;";
				if (Item->Pcolor2 != "None")
					{
					stroke = "stroke:"+SetFarbe(Item->Pcolor2, Item->Shade2, plug)+";";
					if (Item->TranspStroke != 0)
						stroke += " stroke-opacity:"+FToStr(1.0 - Item->TranspStroke)+";";
					} 
				else
					stroke = "stroke:none;";
				trans = "translate("+FToStr(Item->Xpos)+", "+FToStr(Item->Ypos)+")";
				if (Item->Rot != 0)
					trans += " rotate("+FToStr(Item->Rot)+")";
				strokeW = "stroke-width:"+FToStr(Item->Pwidth)+";";
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
				QString Dt = FToStr(QMAX(Item->Pwidth, 1));
				QString Da = FToStr(QMAX(3*Item->Pwidth, 1));
				switch (Item->PLineArt)
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
				gr = docu->createElement("g");
				gr.setAttribute("transform", trans);
				if (Item->PType != 4)
					{
					if (Item->NamedLStyle == "")
						{
						if ((Item->PType == 5) || (Item->PType == 7) || (Item->PType == 8))
							gr.setAttribute("style", "fill:none; "+stroke+" "+strokeW+" "+strokeLC+" "+strokeLJ+" "+strokeDA);
						else
							gr.setAttribute("style", fill+" "+stroke+" "+strokeW+" "+strokeLC+" "+strokeLJ+" "+strokeDA);
						}
					}
				switch (Item->PType)
					{
					case 1:
					case 3:
					case 6:
						if (Item->NamedLStyle == "")
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
							multiLine ml = plug->doc->MLineStyles[Item->NamedLStyle];
							for (int it = ml.size()-1; it > -1; it--)
								{
								ob = docu->createElement("path");
								ob.setAttribute("d", SetClipPath(Item)+"Z");
								ob.setAttribute("style", GetMultiStroke(plug, &ml[it], Item));
								gr.appendChild(ob);
								}
							}
						break;
					case 2:
						if ((Item->Pcolor != "None") || (Item->GrType != 0))
							{
							ob = docu->createElement("path");
							ob.setAttribute("d", SetClipPath(Item)+"Z");
							ob.setAttribute("style", fill);
							gr.appendChild(ob);
							}
						if ((Item->PicAvail) && (Item->Pfile != ""))
							{
							ob = docu->createElement("clipPath");
							ob.setAttribute("id", Clipi+IToStr(ClipCount));
							ob.setAttribute("clipPathUnits", "userSpaceOnUse");
							ob.setAttribute("clip-rule", "evenodd");
							QDomElement cl = docu->createElement("path");
							cl.setAttribute("d", SetClipPath(Item)+"Z");
							ob.appendChild(cl);
							gr.appendChild(ob);
							QImage img = LoadPict(Item->Pfile);
							QFileInfo fi = QFileInfo(Item->Pfile);
							img.save(fi.baseName()+".png", "PNG");
							ob = docu->createElement("image");
							ob.setAttribute("clip-path", "url(#"+Clipi+IToStr(ClipCount)+")");
							ob.setAttribute("xlink:href", fi.baseName()+".png");
							ob.setAttribute("x", "0");
							ob.setAttribute("y", "0");
							ob.setAttribute("width", FToStr(Item->Width));
							ob.setAttribute("height", FToStr(Item->Height));
							ClipCount++;
							gr.appendChild(ob);
							}
						if (Item->NamedLStyle == "")
							{
							ob = docu->createElement("path");
							ob.setAttribute("d", SetClipPath(Item)+"Z");
							ob.setAttribute("style", "fill:none; "+stroke+" "+strokeW+" "+strokeLC+" "+strokeLJ+" "+strokeDA);
							}
						else
							{
							multiLine ml = plug->doc->MLineStyles[Item->NamedLStyle];
							for (int it = ml.size()-1; it > -1; it--)
								{
								ob = docu->createElement("path");
								ob.setAttribute("d", SetClipPath(Item)+"Z");
								ob.setAttribute("style", "fill:none; "+GetMultiStroke(plug, &ml[it], Item));
								gr.appendChild(ob);
								}
							}
						break;
					case 7:
						if (Item->NamedLStyle == "")
							{
							ob = docu->createElement("path");
							ob.setAttribute("d", SetClipPath(Item));
							}
						else
							{
							multiLine ml = plug->doc->MLineStyles[Item->NamedLStyle];
							for (int it = ml.size()-1; it > -1; it--)
								{
								ob = docu->createElement("path");
								ob.setAttribute("d", SetClipPath(Item));
								ob.setAttribute("style", GetMultiStroke(plug, &ml[it], Item));
								gr.appendChild(ob);
								}
							}
						break;
					case 4:
						if ((Item->Pcolor != "None") || (Item->GrType != 0))
							{
							ob = docu->createElement("path");
							ob.setAttribute("d", SetClipPath(Item)+"Z");
							ob.setAttribute("style", fill);
							gr.appendChild(ob);
							}
						ob = docu->createElement("text");
						for (d = 0; d < Item->MaxChars; d++)
							{
							hl = Item->Ptext.at(d);
							if ((hl->ch == QChar(13)) || (hl->ch == QChar(10)) || (hl->ch == QChar(9)))
								continue;
							if (hl->yp == 0)
								break;
							if (hl->ch == QChar(29))
								chx = " ";
							else
								chx = hl->ch;
							tp = docu->createElement("tspan");
							tp.setAttribute("x", hl->xp);
							tp.setAttribute("y", hl->yp);
							SetTextProps(&tp, hl, plug);
							tp1 = docu->createTextNode(chx);
							tp.appendChild(tp1);
							ob.appendChild(tp);
							}
						break;
					case 5:
						if (Item->NamedLStyle == "")
							{
							ob = docu->createElement("path");
							ob.setAttribute("d", "M 0 0 L "+FToStr(Item->Width)+" 0");
							}
						else
							{
							multiLine ml = plug->doc->MLineStyles[Item->NamedLStyle];
							for (int it = ml.size()-1; it > -1; it--)
								{
								ob = docu->createElement("path");
								ob.setAttribute("d", "M 0 0 L "+FToStr(Item->Width)+" 0");
								ob.setAttribute("style", GetMultiStroke(plug, &ml[it], Item));
								gr.appendChild(ob);
								}
							}
						break;
					case 8:
						if (Item->PoShow)
							{
							if (Item->NamedLStyle == "")
								{
								ob = docu->createElement("path");
								ob.setAttribute("d", SetClipPath(Item));
								gr.appendChild(ob);
								}
							else
								{
								multiLine ml = plug->doc->MLineStyles[Item->NamedLStyle];
								for (int it = ml.size()-1; it > -1; it--)
									{
									ob = docu->createElement("path");
									ob.setAttribute("d", SetClipPath(Item));
									ob.setAttribute("style", GetMultiStroke(plug, &ml[it], Item));
									gr.appendChild(ob);
									}
								}
							}
						ob = docu->createElement("text");
						for (d = 0; d < Item->MaxChars; d++)
							{
							hl = Item->Ptext.at(d);
							if ((hl->ch == QChar(13)) || (hl->ch == QChar(10)) || (hl->ch == QChar(9)))
								continue;
							if (hl->ch == QChar(29))
								chx = " ";
							else
								chx = hl->ch;
							tp = docu->createElement("tspan");
							tp.setAttribute("x", hl->PtransX);
							tp.setAttribute("y", hl->PtransY);
							tp.setAttribute("rotate", hl->PRot);
							tp2 = docu->createElement("tspan");
							tp2.setAttribute("dx", hl->xp);
							tp2.setAttribute("dy", hl->yp);
							SetTextProps(&tp2, hl, plug);
							tp1 = docu->createTextNode(chx);
							tp2.appendChild(tp1);
							tp.appendChild(tp2);
							ob.appendChild(tp);
							}
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
}

/*!
 \fn QString SVGExPlug::SetClipPath(PageItem *ite)
 \author Franz Schmid
 \date
 \brief
 \param ite PageItem *
 \retval QString Clipping Path
 */
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

/*!
 \fn QString SVGExPlug::FToStr(double c)
 \author Franz Schmid
 \date
 \brief Converts double to string
 \param c double
 \retval QString
 */
QString SVGExPlug::FToStr(double c)
{
	QString cc;
	return cc.setNum(c);
}

/*!
 \fn QString SVGExPlug::IToStr(int c)
 \author Franz Schmid
 \date
 \brief Converts integer to QString
 \param c int
 \retval QString representation of value
 */
QString SVGExPlug::IToStr(int c)
{
	QString cc;
	return cc.setNum(c);
}

/*!
 \fn void SVGExPlug::SetTextProps(QDomElement *tp, struct Pti *hl, ScribusApp *plug)
 \author Franz Schmid
 \date
 \brief Set text properties
 \param tp QDomElement *
 \param hl struct Pti *
 \param plug ScribusApp *
 \retval None
 */
void SVGExPlug::SetTextProps(QDomElement *tp, struct Pti *hl, ScribusApp *plug)
{
	int chst = hl->cstyle & 127;
	if (hl->ccolor != "None")
		tp->setAttribute("fill", SetFarbe(hl->ccolor, hl->cshade, plug));
	else
		tp->setAttribute("fill", "none");
	if ((hl->cstroke != "None") && (chst & 4))
		{
		tp->setAttribute("stroke", SetFarbe(hl->cstroke, hl->cshade2, plug));
		tp->setAttribute("stroke-width", (*plug->doc->AllFonts)[hl->cfont]->strokeWidth * (hl->csize / 10.0));
		}
	else
		tp->setAttribute("stroke", "none");
	tp->setAttribute("font-size", (hl->csize / 10.0));
	tp->setAttribute("font-family", (*plug->doc->AllFonts)[hl->cfont]->Family);
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

/*!
 \fn QString SVGExPlug::SetFarbe(QString farbe, int shad, ScribusApp *plug)
 \author Franz Schmid
 \date
 \brief
 \param farbe QString
 \param shad int
 \param plug ScribusApp *
 \retval QString Colour settings
 */
QString SVGExPlug::SetFarbe(QString farbe, int shad, ScribusApp *plug)
{
	int h, s, v, sneu;
	QColor tmp;
	plug->doc->PageColors[farbe].getRGBColor().rgb(&h, &s, &v);
	if ((h == s) && (s == v))
		{
		plug->doc->PageColors[farbe].getRGBColor().hsv(&h, &s, &v);
		sneu = 255 - ((255 - v) * shad / 100);
		tmp.setHsv(h, s, sneu);
		}
	else
		{
		plug->doc->PageColors[farbe].getRGBColor().hsv(&h, &s, &v);
		sneu = s * shad / 100;
		tmp.setHsv(h, sneu, v);
		}
	return tmp.name();
}

/*!
 \fn QString SVGExPlug::GetMultiStroke(ScribusApp *plug, struct singleLine *sl, PageItem *Item)
 \author Franz Schmid
 \date
 \brief
 \param plug ScribusApp *
 \param sl struct singleLine *
 \param Item PageItem *
 \retval QString Stroke settings
 */
QString SVGExPlug::GetMultiStroke(ScribusApp *plug, struct singleLine *sl, PageItem *Item)
{
	QString tmp = "fill:none; ";
	tmp += "stroke:"+SetFarbe(sl->Color, sl->Shade, plug)+"; ";
	if (Item->Transparency != 0)
		tmp += " stroke-opacity:"+FToStr(1.0 - Item->Transparency)+"; ";
	tmp += "stroke-width:"+FToStr(sl->Width)+"; ";
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
	QString Dt = FToStr(QMAX(sl->Width, 1));
	QString Da = FToStr(QMAX(3*sl->Width, 1));
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

/*!
 \fn SVGExPlug::~SVGExPlug()
 \author Franz Schmid
 \date
 \brief Destructore
 \param None
 \retval None
 */
SVGExPlug::~SVGExPlug()
{
}

