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
#include "config.h"
#include "customfdialog.h"
#include <qfile.h>
#include <qtextstream.h>
extern void Level2Layer(ScribusDoc *doc, struct Layer *ll, int Level);
extern QString Path2Relative(QString Path);
extern QImage LoadPict(QString fn);

QString Name()
{
  return QObject::tr("Save Page as SVG...");
}

int Type()
{
	return 3;
}

void Run(QWidget *d, ScribusApp *plug)
{
	if (plug->HaveDoc)
		{
		QString fileName = plug->CFileDialog(QObject::tr("Save as"), QObject::tr("SVG-Images (*.svg);; All Files (*)"),"", false, false);
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

SVGExPlug::SVGExPlug( QWidget* parent, ScribusApp *plug, QString fName )
{
	QDomDocument docu("svgdoc");
	QString vo = "<?xml version=\"1.0\" standalone=\"yes\"?>\n";
	QString st = "<svg></svg>";
	docu.setContent(st);
	QDomElement elem = docu.documentElement();
	elem.setAttribute("width", plug->doc->PageB);
	elem.setAttribute("height", plug->doc->PageH);
	Page *Seite;
	GradCount = 0;
	ClipCount = 0;
	Seite = plug->view->MasterPages.at(plug->view->MasterNames[plug->doc->ActPage->MPageNam]);
	ProcessPage(plug, Seite, &docu, &elem);
	Seite = plug->doc->ActPage;
	ProcessPage(plug, Seite, &docu, &elem);
	QFile f(fName);
	if(!f.open(IO_WriteOnly))
		return;
	QTextStream s(&f);
	s.setEncoding( QTextStream::UnicodeUTF8 );
	s<<vo;
	s<<docu.toString();
	f.close();
}


void SVGExPlug::ProcessPage(ScribusApp *plug, Page *Seite, QDomDocument *docu, QDomElement *elem)
{
	QString tmp, trans, fill, stroke, strokeW, strokeLC, strokeLJ, strokeDA, gradi, Clipi;
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
				if (Item->Pcolor != "None")
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
						s1.setAttribute("stop-color",SetFarbe(Item->GrColor2, Item->GrShade2, plug));
						grad.appendChild(s1);
						QDomElement s2 = docu->createElement("stop");
						s2.setAttribute("offset","100%");
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
					if (Item->Transparency != 0)
						stroke += " stroke-opacity:"+FToStr(1.0 - Item->Transparency)+";";
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
					if ((Item->PType == 5) || (Item->PType == 7) || (Item->PType == 8))
						gr.setAttribute("style", "fill:none; "+stroke+" "+strokeW+" "+strokeLC+" "+strokeLJ+" "+strokeDA);
					else
						gr.setAttribute("style", fill+" "+stroke+" "+strokeW+" "+strokeLC+" "+strokeLJ+" "+strokeDA);
					}
				switch (Item->PType)
					{
					case 1:
					case 3:
					case 6:
						ob = docu->createElement("path");
						ob.setAttribute("d", SetClipPath(Item)+"Z");
						break;
					case 2:
						if (Item->Pcolor != "None")
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
							ob.setAttribute("transform", "scale("+FToStr(Item->LocalScX)+", "+FToStr(Item->LocalScY)+") translate("+FToStr(Item->LocalX)+", "+FToStr(Item->LocalY)+")");
							ob.setAttribute("xlink:href", fi.baseName()+".png");
							ClipCount++;
							}
						break;
					case 7:
						ob = docu->createElement("path");
						ob.setAttribute("d", SetClipPath(Item));
						break;
					case 4:
						if (Item->Pcolor != "None")
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
							if ((hl->ch == QChar(13)) || (hl->ch == QChar(10)))
								continue;
							if (hl->yp == 0)
								break;
							tp = docu->createElement("tspan");
							tp.setAttribute("x", hl->xp);
							tp.setAttribute("y", hl->yp);
							SetTextProps(&tp, hl, plug);
							tp1 = docu->createTextNode(hl->ch);
							tp.appendChild(tp1);
							ob.appendChild(tp);
							}
						break;
					case 5:
						ob = docu->createElement("path");
						ob.setAttribute("d", "M 0 0 L "+FToStr(Item->Width)+" 0");
						break;
					case 8:
						if (Item->PoShow)
							{
							ob = docu->createElement("path");
							ob.setAttribute("d", SetClipPath(Item));
							gr.appendChild(ob);
							}
						ob = docu->createElement("text");
						for (d = 0; d < Item->MaxChars; d++)
							{
							hl = Item->Ptext.at(d);
							if ((hl->ch == QChar(13)) || (hl->ch == QChar(10)))
								continue;
							tp = docu->createElement("tspan");
							tp.setAttribute("x", hl->PtransX);
							tp.setAttribute("y", hl->PtransY);
							tp.setAttribute("rotate", hl->PRot);
							tp2 = docu->createElement("tspan");
							tp2.setAttribute("dx", hl->xp);
							tp2.setAttribute("dy", hl->yp);
							SetTextProps(&tp2, hl, plug);
							tp1 = docu->createTextNode(hl->ch);
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

QString SVGExPlug::FToStr(float c)
{
	QString cc;
	return cc.setNum(c);
}

QString SVGExPlug::IToStr(int c)
{
	QString cc;
	return cc.setNum(c);
}

void SVGExPlug::SetTextProps(QDomElement *tp, struct Pti *hl, ScribusApp *plug)
{
	int chst;
	if (hl->ccolor != "None")
		tp->setAttribute("fill", SetFarbe(hl->ccolor, hl->cshade, plug));
	else
		tp->setAttribute("fill", "none");
	tp->setAttribute("font-size", hl->csize);
	tp->setAttribute("font-family", plug->doc->UsedFonts[hl->cfont].family());
	chst = hl->cstyle & 127;
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
		if (chst & 4)
			tp->setAttribute("font-style", "italic");
		}
}

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

SVGExPlug::~SVGExPlug()
{
}

