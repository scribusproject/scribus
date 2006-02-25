/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "stencilreader.h"
#include <qtextstream.h>
#include <qdom.h>
#include "sccolor.h"
#include "scribus.h"
#include "splash.h"
#include "util.h"
#include "prefsmanager.h"
#include "scpainter.h"
#include "commonstrings.h"
#include "pageitem.h"
#include "scribusdoc.h"


StencilReader::StencilReader()
{
}

QPixmap StencilReader::createPreview(QString data)
{
	double pmmax;
	double GrW = 50.0;
	double GrH = 50.0;
	QPixmap tmp = QPixmap(0, 0);
	QColor stroke = Qt::black;
	QColor fill = Qt::white;
	Qt::PenStyle Dash = Qt::SolidLine;
	Qt::PenCapStyle LineEnd = Qt::FlatCap;
	Qt::PenJoinStyle LineJoin = Qt::MiterJoin;
	int fillStyle = 1;
	ScPainter *pS = NULL;
	double strokewidth = 1.0;
	QDomDocument docu("scridoc");
	docu.setContent(data);
	QDomElement elem=docu.documentElement();
	if (elem.tagName() != "KivioShapeStencil")
		return tmp;
	QDomNodeList list = elem.elementsByTagName("Dimensions");
	if (list.count() == 0)
		return tmp;
	QDomElement dims = list.item(0).toElement();
	GrW = dims.attribute("w","50").toDouble()+10;
	GrH = dims.attribute("h","50").toDouble()+10;
	pmmax = 60 / QMAX(GrW, GrH);
	tmp = QPixmap(static_cast<int>(GrW), static_cast<int>(GrH));
	pS = new ScPainter(&tmp, tmp.width(), tmp.height());
	pS->translate(5, 5);
	QDomNode DOC=elem.firstChild();
	while(!DOC.isNull())
	{
		QDomElement pg=DOC.toElement();
		if(pg.tagName()=="KivioShape")
		{
			stroke = Qt::black;
			fill = Qt::white;
			FPointArray PoLine;
			PoLine.resize(0);
			double x, y, x1, y1, currx, curry, startx, starty;
			QString typ = pg.attribute("type");
			QDomNode point = DOC.firstChild();
			bool first = true;
			int count = 0;
			while(!point.isNull())
			{
				QDomElement pt = point.toElement();
				if(pt.tagName()=="KivioFillStyle")
				{
					QString colnam = pt.attribute("color","#000000");
					fill.setNamedColor("#"+colnam.right(6));
					fillStyle = pt.attribute("colorStyle","1").toInt();
				}
				if(pt.tagName()=="KivioLineStyle")
				{
					QString colnam = pt.attribute("color","#FFFFFF");
					stroke.setNamedColor("#"+colnam.right(6));
					strokewidth = pt.attribute("width", "1").toDouble();
					LineJoin = Qt::PenJoinStyle(pt.attribute("joinStyle", "0").toInt());
					Dash = Qt::PenStyle(pt.attribute("pattern").toInt());
					LineEnd = Qt::PenCapStyle(pt.attribute("capStyle", "0").toInt());
				}
				if(pt.tagName()=="KivioPoint")
				{
					x = pt.attribute("x").toDouble();
					y = pt.attribute("y").toDouble();
					if (first)
					{
						currx = x;
						curry = y;
						startx = x;
						starty = y;
						first = false;
						if (pt.attribute("type") == "bezier")
							count = 0;
						else
							count = -1;
					}
					else
					{
						if (pt.attribute("type") != "bezier")
						{
							PoLine.addPoint(currx, curry);
							PoLine.addPoint(currx, curry);
							PoLine.addPoint(x, y);
							PoLine.addPoint(x, y);
							currx = x;
							curry = y;
						}
						else
						{
							if (count == -1)
							{
								if (FPoint(currx, curry) != FPoint(x, y))
								{
									PoLine.addPoint(currx, curry);
									PoLine.addPoint(currx, curry);
									PoLine.addPoint(x, y);
									PoLine.addPoint(x, y);
								}
								currx = x;
								curry = y;
								count++;
							}
							else if (count == 0)
							{
								PoLine.addPoint(currx, curry);
								PoLine.addPoint(x, y);
								count++;
							}
							else if (count == 1)
							{
								currx = x;
								curry = y;
								count++;
							}
							else if (count == 2)
							{
								PoLine.addPoint(x, y);
								PoLine.addPoint(currx, curry);
								currx = x;
								curry = y;
								count = -1;
							}
						}
					}
				}
				if(pt.tagName()=="Line")
				{
					if (!first)
						PoLine.setMarker();
					x = pt.attribute("x1").toDouble();
					y = pt.attribute("y1").toDouble();
					x1 = pt.attribute("x2").toDouble();
					y1 = pt.attribute("y2").toDouble();
					PoLine.addPoint(x, y);
					PoLine.addPoint(x, y);
					PoLine.addPoint(x1, y1);
					PoLine.addPoint(x1, y1);
					first = false;
				}
				point = point.nextSibling();
			}
			if ((typ == "Polygon") || (typ == "ClosedPath"))
			{
				pS->setBrush(fill);
				pS->setFillMode(fillStyle);
				pS->setLineWidth(strokewidth);
				pS->setPen(stroke);
				pS->setupPolygon(&PoLine);
				pS->drawPolygon();
				pS->drawPolyLine();
			}
			else if ((typ == "Bezier") || (typ == "OpenPath") || (typ == "LineArray") || (typ == "Polyline"))
			{
				pS->setPen(stroke, strokewidth, Dash, LineEnd, LineJoin);
				pS->setupPolygon(&PoLine, false);
				pS->drawPolyLine();
			}
			if (typ == "Rectangle")
			{
				pS->setBrush(fill);
				pS->setFillMode(fillStyle);
				pS->setPen(stroke, strokewidth, Dash, LineEnd, LineJoin);
				x = pg.attribute("x").toDouble();
				y = pg.attribute("y").toDouble();
				x1 = pg.attribute("w").toDouble();
				y1 = pg.attribute("h").toDouble();
				static double rect[] = {0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0,
										1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0,
										0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0};
				PoLine.resize(0);
				for (int a = 0; a < 29; a += 4)
				{
					double xa = x1 * rect[a];
					double ya = y1 * rect[a+1];
					double xb = x1 * rect[a+2];
					double yb = y1 * rect[a+3];
					PoLine.addPoint(x+xa, y+ya);
					PoLine.addPoint(x+xb, y+yb);
				}
				pS->setupPolygon(&PoLine);
				pS->drawPolygon();
				pS->drawPolyLine();
			}
			if (typ == "Ellipse")
			{
				pS->setBrush(fill);
				pS->setFillMode(fillStyle);
				pS->setPen(stroke, strokewidth, Dash, LineEnd, LineJoin);
				x = pg.attribute("x").toDouble();
				y = pg.attribute("y").toDouble();
				x1 = pg.attribute("w").toDouble();
				y1 = pg.attribute("h").toDouble();
				static double rect[] = {1.0, 0.5, 1.0, 0.77615235,0.5, 1.0, 0.77615235, 1.0,
										0.5, 1.0, 0.22385765, 1.0, 0.0, 0.5, 0.0, 0.77615235,
										0.0, 0.5, 0.0, 0.22385765, 0.5, 0.0, 0.22385765, 0.0,
										0.5, 0.0, 0.77615235, 0.0, 1.0, 0.5, 1.0, 0.22385765};
				PoLine.resize(0);
				for (int a = 0; a < 29; a += 4)
				{
					double xa = x1 * rect[a];
					double ya = y1 * rect[a+1];
					double xb = x1 * rect[a+2];
					double yb = y1 * rect[a+3];
					PoLine.addPoint(x+xa, y+ya);
					PoLine.addPoint(x+xb, y+yb);
				}
				pS->setupPolygon(&PoLine);
				pS->drawPolygon();
				pS->drawPolyLine();
			}
		}
		DOC = DOC.nextSibling();
	}
	if (pS)
	{
		pS->end();
		delete pS;
	}
	QImage tmpi1 = tmp.convertToImage();
	QImage tmpi = tmpi1.smoothScale(static_cast<int>(tmp.width()*pmmax), static_cast<int>(tmp.height()*pmmax));
	tmp.convertFromImage(tmpi);
	return tmp;
}

QString StencilReader::createObjects(QString datain)
{
	PrefsManager* prefsManager = PrefsManager::instance();
	double GrW = 50.0;
	double GrH = 50.0;
	QString tmp = "";
	QColor stroke = Qt::black;
	QColor fill = Qt::white;
	Qt::PenStyle Dash = Qt::SolidLine;
	Qt::PenCapStyle LineEnd = Qt::FlatCap;
	Qt::PenJoinStyle LineJoin = Qt::MiterJoin;
	int fillStyle = 1;
	double strokewidth = 1.0;
	QString FillCol = "FromKivio#ffffff";
	QString StrokeCol = "FromKivio#000000";
	QDomDocument docu("scridoc");
	docu.setContent(datain);
	QDomElement elem=docu.documentElement();
	if (elem.tagName() != "KivioShapeStencil")
		return "";
	QDomNodeList list = elem.elementsByTagName("Dimensions");
	if (list.count() == 0)
		return tmp;
	QDomDocument data("scribus");
	QString st="<SCRIBUSELEMUTF8></SCRIBUSELEMUTF8>";
	data.setContent(st);
	QDomElement group = data.documentElement();
	QDomNodeList listItems = elem.elementsByTagName("KivioShape");
	QDomElement dims = list.item(0).toElement();
	GrW = dims.attribute("w","50").toDouble();
	GrH = dims.attribute("h","50").toDouble();
	group.setAttribute("W", GrW);
	group.setAttribute("H", GrH);
	group.setAttribute("XP", 0.0);
	group.setAttribute("YP", 0.0);
	group.setAttribute("COUNT", listItems.count());
	group.setAttribute("Version", QString(VERSION));
	PageColors.insert("FromKivio#000000", ScColor(0, 0, 0));
	QDomElement co = data.createElement("COLOR");
	co.setAttribute("NAME","FromKivio#000000");
	co.setAttribute("RGB", "#000000");
	co.setAttribute("Spot","0");
	co.setAttribute("Register","0");
	group.appendChild(co);
	PageColors.insert("FromKivio#ffffff", ScColor(255, 255, 255));
	QDomElement co2 = data.createElement("COLOR");
	co2.setAttribute("NAME","FromKivio#ffffff");
	co2.setAttribute("RGB", "#ffffff");
	co2.setAttribute("Spot","0");
	co2.setAttribute("Register","0");
	group.appendChild(co);
	QDomNodeList listStrokes = elem.elementsByTagName("KivioLineStyle");
	for (uint st = 0; st < listStrokes.count(); st++)
	{
		QDomElement str = listStrokes.item(st).toElement();
		QString colnam = str.attribute("color","#ffffff");
		stroke.setNamedColor("#"+colnam.right(6));
		ColorList::Iterator it;
		bool found = false;
		int r, g, b;
		QColor tmpR;
		for (it = PageColors.begin(); it != PageColors.end(); ++it)
		{
			PageColors[it.key()].getRGB(&r, &g, &b);
			tmpR.setRgb(r, g, b);
			if (stroke == tmpR)
			{
				StrokeCol = it.key();
				found = true;
				break;
			}
		}
		if (!found)
		{
			ScColor tmp;
			tmp.fromQColor(stroke);
			PageColors.insert("FromKivio"+stroke.name(), tmp);
			StrokeCol = "FromKivio"+stroke.name();
			QDomElement co = data.createElement("COLOR");
			co.setAttribute("NAME",StrokeCol);
			co.setAttribute("RGB",stroke.name());
			co.setAttribute("Spot","0");
			co.setAttribute("Register","0");
			group.appendChild(co);
		}
	}
	QDomNodeList listFills = elem.elementsByTagName("KivioFillStyle");
	for (uint st = 0; st < listFills.count(); st++)
	{
		QDomElement fil = listFills.item(st).toElement();
		QString colnam = fil.attribute("color","#ffffff");
		fill.setNamedColor("#"+colnam.right(6));
		ColorList::Iterator it;
		bool found = false;
		int r, g, b;
		QColor tmpR;
		for (it = PageColors.begin(); it != PageColors.end(); ++it)
		{
			PageColors[it.key()].getRGB(&r, &g, &b);
			tmpR.setRgb(r, g, b);
			if (fill == tmpR)
			{
				FillCol = it.key();
				found = true;
				break;
			}
		}
		if (!found)
		{
			ScColor tmp;
			tmp.fromQColor(fill);
			PageColors.insert("FromKivio"+fill.name(), tmp);
			FillCol = "FromKivio"+fill.name();
			QDomElement co = data.createElement("COLOR");
			co.setAttribute("NAME",FillCol);
			co.setAttribute("RGB",fill.name());
			co.setAttribute("Spot","0");
			co.setAttribute("Register","0");
			group.appendChild(co);
		}
	}
	QDomNode DOC=elem.firstChild();
	while(!DOC.isNull())
	{
		QDomElement pg=DOC.toElement();
		if(pg.tagName()=="KivioShape")
		{
			FillCol = "FromKivio#ffffff";
			StrokeCol = "FromKivio#000000";
			FPointArray PoLine;
			PoLine.resize(0);
			double x, y, x1, y1, currx, curry, startx, starty;
			QString typ = pg.attribute("type");
			QDomNode point = DOC.firstChild();
			bool first = true;
			int count = 0;
			while(!point.isNull())
			{
				QDomElement pt = point.toElement();
				if(pt.tagName()=="KivioFillStyle")
				{
					fillStyle = pt.attribute("colorStyle","1").toInt();
					QString colnam = pt.attribute("color","#000000");
					fill.setNamedColor("#"+colnam.right(6));
					if (fillStyle == 0)
						FillCol = "None";
					else
						FillCol = "FromKivio"+fill.name();
				}
				if(pt.tagName()=="KivioLineStyle")
				{
					QString colnam = pt.attribute("color","#ffffff");
					stroke.setNamedColor("#"+colnam.right(6));
					StrokeCol = "FromKivio"+stroke.name();
					strokewidth = pt.attribute("width", "1").toDouble();
					LineJoin = Qt::PenJoinStyle(pt.attribute("joinStyle", "0").toInt());
					Dash = Qt::PenStyle(pt.attribute("pattern").toInt());
					LineEnd = Qt::PenCapStyle(pt.attribute("capStyle", "0").toInt());
				}
				if(pt.tagName()=="KivioPoint")
				{
					x = pt.attribute("x").toDouble();
					y = pt.attribute("y").toDouble();
					if (first)
					{
						currx = x;
						curry = y;
						startx = x;
						starty = y;
						first = false;
						if (pt.attribute("type") == "bezier")
							count = 0;
						else
							count = -1;
					}
					else
					{
						if (pt.attribute("type") != "bezier")
						{
							PoLine.addPoint(currx, curry);
							PoLine.addPoint(currx, curry);
							PoLine.addPoint(x, y);
							PoLine.addPoint(x, y);
							currx = x;
							curry = y;
						}
						else
						{
							if (count == -1)
							{
								if (FPoint(currx, curry) != FPoint(x, y))
								{
									PoLine.addPoint(currx, curry);
									PoLine.addPoint(currx, curry);
									PoLine.addPoint(x, y);
									PoLine.addPoint(x, y);
								}
								currx = x;
								curry = y;
								count++;
							}
							else if (count == 0)
							{
								PoLine.addPoint(currx, curry);
								PoLine.addPoint(x, y);
								count++;
							}
							else if (count == 1)
							{
								currx = x;
								curry = y;
								count++;
							}
							else if (count == 2)
							{
								PoLine.addPoint(x, y);
								PoLine.addPoint(currx, curry);
								currx = x;
								curry = y;
								count = -1;
							}
						}
					}
				}
				if(pt.tagName()=="Line")
				{
					if (!first)
						PoLine.setMarker();
					x = pt.attribute("x1").toDouble();
					y = pt.attribute("y1").toDouble();
					x1 = pt.attribute("x2").toDouble();
					y1 = pt.attribute("y2").toDouble();
					PoLine.addPoint(x, y);
					PoLine.addPoint(x, y);
					PoLine.addPoint(x1, y1);
					PoLine.addPoint(x1, y1);
					first = false;
				}
				point = point.nextSibling();
			}
			QDomElement ob = data.createElement("ITEM");
			ob.setAttribute("OwnPage", 0);
			ob.setAttribute("RADRECT", 0);
			ob.setAttribute("FRTYPE",  3);
			ob.setAttribute("CLIPEDIT", 1);
			ob.setAttribute("PWIDTH", strokewidth);
			ob.setAttribute("PCOLOR", FillCol);
			ob.setAttribute("PCOLOR2", StrokeCol);
			ob.setAttribute("TXTFILL", FillCol);
			ob.setAttribute("TXTSTROKE", StrokeCol);
			ob.setAttribute("TXTSTRSH", 100);
			ob.setAttribute("TXTFILLSH", 100);
			ob.setAttribute("TXTSCALE", 100);
			ob.setAttribute("TXTSCALEV", 100);
			ob.setAttribute("TXTBASE", 0);
			ob.setAttribute("TXTSHX", 0);
			ob.setAttribute("TXTSHY", 0);
			ob.setAttribute("TXTOUT", 0);
			ob.setAttribute("TXTULP", 0);
			ob.setAttribute("TXTULW", 0);
			ob.setAttribute("TXTSTP", 0);
			ob.setAttribute("TXTSTW", 0);
			ob.setAttribute("TXTSTYLE", 0);
			ob.setAttribute("COLUMNS", 1);
			ob.setAttribute("COLGAP", 0);
			ob.setAttribute("NAMEDLST", "");
			ob.setAttribute("SHADE", 100);
			ob.setAttribute("SHADE2", 100);
			ob.setAttribute("GRTYP", 0);
			ob.setAttribute("ROT", 0);
			ob.setAttribute("PLINEART", Dash);
			ob.setAttribute("PLINEEND", LineEnd);
			ob.setAttribute("PLINEJOIN", LineJoin);
			ob.setAttribute("LINESP", 12);
			ob.setAttribute("LINESPMode", 0);
			ob.setAttribute("TXTKERN", 0);
			ob.setAttribute("LOCALSCX", 100);
			ob.setAttribute("LOCALSCY", 100);
			ob.setAttribute("LOCALX", 0);
			ob.setAttribute("LOCALY", 0);
			ob.setAttribute("PICART",  1);
			ob.setAttribute("PLTSHOW", 0);
			ob.setAttribute("BASEOF", 0);
			ob.setAttribute("FLIPPEDH", 0);
			ob.setAttribute("FLIPPEDV", 0);
			ob.setAttribute("IFONT", prefsManager->appPrefs.toolSettings.defFont);
			ob.setAttribute("ISIZE", 12.0 );
			ob.setAttribute("SCALETYPE", 1);
			ob.setAttribute("RATIO", 0);
			ob.setAttribute("PRINTABLE", 1);
			ob.setAttribute("ALIGN", "0");
			ob.setAttribute("BOOKMARK", "0");
			ob.setAttribute("fillRule", "1");
			ob.setAttribute("ANNAME", pg.attribute("name"));
			ob.setAttribute("TEXTFLOW",  0);
			ob.setAttribute("TEXTFLOW2",  0);
			ob.setAttribute("TEXTFLOW3",  0);
			ob.setAttribute("AUTOTEXT",  0);
			ob.setAttribute("EXTRA", 1);
			ob.setAttribute("TEXTRA", 1);
			ob.setAttribute("BEXTRA", 1);
			ob.setAttribute("REXTRA", 1);
			ob.setAttribute("PFILE","");
			ob.setAttribute("PFILE2","");
			ob.setAttribute("PFILE3","");
			ob.setAttribute("PRFILE", "");
			ob.setAttribute("EPROF", "");
			ob.setAttribute("IRENDER", 1);
			ob.setAttribute("EMBEDDED", 1);
			ob.setAttribute("LOCK",  0);
			ob.setAttribute("LOCKR",  0);
			ob.setAttribute("REVERS",  0);
			ob.setAttribute("TransValue", 0);
			ob.setAttribute("TransValueS", 0);
			ob.setAttribute("isTableItem", 0);
			ob.setAttribute("TopLine", 0);
			ob.setAttribute("LeftLine", 0);
			ob.setAttribute("RightLine", 0);
			ob.setAttribute("BottomLine", 0);
			if (listItems.count() != 1)
			{
				ob.setAttribute("GROUPS", 1);
				ob.setAttribute("NUMGROUP", 1);
			}
			else
			{
				ob.setAttribute("GROUPS", "");
				ob.setAttribute("NUMGROUP", 0);
			}
			ob.setAttribute("LANGUAGE", prefsManager->appPrefs.Language);
			ob.setAttribute("startArrowIndex", 0);
			ob.setAttribute("endArrowIndex", 0);
			ob.setAttribute("NUMDASH", 0);
			ob.setAttribute("DASHS", "");
			ob.setAttribute("DASHOFF", 0);
			ob.setAttribute("NUMTEXT", 0);
			ob.setAttribute("TEXTCOOR", "");
			ob.setAttribute("BACKITEM", -1);
			ob.setAttribute("BACKPAGE", -1);
			ob.setAttribute("NEXTITEM", -1);
			ob.setAttribute("NEXTPAGE", -1);
			bool valid = false;
			if ((typ == "Polygon") || (typ == "ClosedPath"))
			{
				ob.setAttribute("PTYPE", PageItem::Polygon);
				valid = true;
			}
			else if ((typ == "Bezier") || (typ == "OpenPath") || (typ == "LineArray") || (typ == "Polyline"))
			{
				ob.setAttribute("PTYPE", PageItem::PolyLine);
				valid = true;
			}
			else if (typ == "Rectangle")
			{
				ob.setAttribute("PTYPE", PageItem::Polygon);
				x = pg.attribute("x").toDouble();
				y = pg.attribute("y").toDouble();
				x1 = pg.attribute("w").toDouble();
				y1 = pg.attribute("h").toDouble();
				static double rect[] = {0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0,
										1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0,
										0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0};
				PoLine.resize(0);
				for (int a = 0; a < 29; a += 4)
				{
					double xa = x1 * rect[a];
					double ya = y1 * rect[a+1];
					double xb = x1 * rect[a+2];
					double yb = y1 * rect[a+3];
					PoLine.addPoint(x+xa, y+ya);
					PoLine.addPoint(x+xb, y+yb);
				}
				valid = true;
			}
			else if (typ == "Ellipse")
			{
				ob.setAttribute("PTYPE", PageItem::Polygon);
				x = pg.attribute("x").toDouble();
				y = pg.attribute("y").toDouble();
				x1 = pg.attribute("w").toDouble();
				y1 = pg.attribute("h").toDouble();
				static double rect[] = {1.0, 0.5, 1.0, 0.77615235,0.5, 1.0, 0.77615235, 1.0,
										0.5, 1.0, 0.22385765, 1.0, 0.0, 0.5, 0.0, 0.77615235,
										0.0, 0.5, 0.0, 0.22385765, 0.5, 0.0, 0.22385765, 0.0,
										0.5, 0.0, 0.77615235, 0.0, 1.0, 0.5, 1.0, 0.22385765};
				PoLine.resize(0);
				for (int a = 0; a < 29; a += 4)
				{
					double xa = x1 * rect[a];
					double ya = y1 * rect[a+1];
					double xb = x1 * rect[a+2];
					double yb = y1 * rect[a+3];
					PoLine.addPoint(x+xa, y+ya);
					PoLine.addPoint(x+xb, y+yb);
				}
				valid = true;
			}
			FPoint tp2(getMinClipF(&PoLine));
			PoLine.translate(-tp2.x(), -tp2.y());
			FPoint wh = getMaxClipF(&PoLine);
			ob.setAttribute("XPOS", tp2.x());
			ob.setAttribute("YPOS",tp2.y());
			ob.setAttribute("WIDTH",wh.x());
			ob.setAttribute("HEIGHT",wh.y());
			ob.setAttribute("NUMPO", PoLine.size());
			QString polp = "";
			double xf, yf;
			QString tmpSt, tmpSt2;
			for (uint nxx=0; nxx<PoLine.size(); ++nxx)
			{
				PoLine.point(nxx, &xf, &yf);
				polp += tmpSt.setNum(xf) + " " + tmpSt2.setNum(yf) + " ";
			}
			ob.setAttribute("POCOOR", polp);
			ob.setAttribute("NUMCO", 0);
			ob.setAttribute("COCOOR", "");
			if (valid)
				group.appendChild(ob);
		}
		DOC = DOC.nextSibling();
	}
	return data.toString().utf8();
}
