/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "stencilreader.h"

#include <QPixmap>
#include <QRegExp>
#include "commonstrings.h"
#include "pageitem.h"
#include "prefsmanager.h"
#include "sccolor.h"
#include "scpainter.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "splash.h"
#include "units.h"
#include "util_math.h"


StencilReader::StencilReader()
{
}

QString StencilReader::createShape(QString datain)
{
	QString tmp = "";
	double GrW = 50.0;
	double GrH = 50.0;
	double Dx = 0.0;
	double Dy = 0.0;
	QDomDocument docu("scridoc");
	docu.setContent(datain);
	QDomElement elem=docu.documentElement();
	if (elem.tagName() != "shape")
		return tmp;
	QDomNodeList list = elem.elementsByTagName("name");
	if (list.count() == 0)
		return tmp;
	QString name = list.item(0).toElement().text();
	QDomDocument data("scribus");
	QString st="<SCRIBUSELEMUTF8></SCRIBUSELEMUTF8>";
	data.setContent(st);
	QDomElement group = data.documentElement();
	QDomNodeList list2 = elem.elementsByTagName("svg:svg");
	if (list2.count() == 0)
		return tmp;
	QDomElement svg = list2.item(0).toElement();
	Conversion = 1.0;

	group.setAttribute("XP", 0.0);
	group.setAttribute("YP", 0.0);
	group.setAttribute("Version", QString(VERSION));
	PageColors.insert("Black", ScColor(0, 0, 0, 255));
	QDomElement co = data.createElement("COLOR");
	co.setAttribute("NAME","Black");
	co.setAttribute("CMYK", "#000000FF");
	co.setAttribute("Spot","0");
	co.setAttribute("Register","0");
	group.appendChild(co);
	PageColors.insert("White", ScColor(0, 0, 0, 0));
	QDomElement co2 = data.createElement("COLOR");
	co2.setAttribute("NAME","White");
	co2.setAttribute("CMYK", "#00000000");
	co2.setAttribute("Spot","0");
	co2.setAttribute("Register","0");
	group.appendChild(co2);
	QDomNode DOC = svg.firstChild();
	double minXCoor = 0.0;
	double minYCoor = 0.0;
	double maxXCoor = 0.0;
	double maxYCoor = 0.0;
	int groupElemCounter = 0;
	Conversion = 1.0;
	bool firstCheck = true;
	parseGroupProperties(data, group, DOC, groupElemCounter, minXCoor, minYCoor, maxXCoor, maxYCoor, firstCheck);
	GrW = maxXCoor - minXCoor;
	GrH = maxYCoor - minYCoor;
	Conversion = 100.0 / qMax(GrW, GrH);
	GrW *= Conversion;
	GrH *= Conversion;
	Dx = minXCoor * Conversion;
	Dy = minYCoor* Conversion;
	QDomElement obGroup;
	if (groupElemCounter != 1)
	{
		group.setAttribute("COUNT", groupElemCounter+1);
		obGroup = data.createElement("ITEM");
		writeDefaults(obGroup);
		obGroup.setAttribute("PWIDTH", 0);
		obGroup.setAttribute("PCOLOR", "None");
		obGroup.setAttribute("PCOLOR2", "None");
		obGroup.setAttribute("PLINEART", Qt::SolidLine);
		obGroup.setAttribute("PLINEEND", Qt::FlatCap);
		obGroup.setAttribute("PLINEJOIN", Qt::MiterJoin);
		obGroup.setAttribute("ANNAME", name.simplified().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" ));
		obGroup.setAttribute("GROUPS", 1);
		obGroup.setAttribute("NUMGROUP", 1);
		obGroup.setAttribute("isGroupControl", 1);
		obGroup.setAttribute("PTYPE", PageItem::Polygon);
		group.appendChild(obGroup);
	}
	else
		group.setAttribute("COUNT", groupElemCounter);
	int groupElemCounter2 = 0;
	DOC = svg.firstChild();
	parseGroup(data, group, DOC, groupElemCounter, groupElemCounter2, Dx, Dy);
	if (groupElemCounter > 1)
	{
		obGroup.setAttribute("groupsLastItem", groupElemCounter2);
		static double rect[] = {0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0,
											1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0,
											0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0};
		FPointArray PoLine;
		PoLine.resize(0);
		for (int a = 0; a < 29; a += 4)
		{
			double xa = GrW * rect[a];
			double ya = GrH * rect[a+1];
			double xb = GrW * rect[a+2];
			double yb = GrH * rect[a+3];
			PoLine.addPoint(0+xa, 0+ya);
			PoLine.addPoint(0+xb, 0+yb);
		}
		obGroup.setAttribute("XPOS", 0);
		obGroup.setAttribute("YPOS",0);
		obGroup.setAttribute("WIDTH",GrW);
		obGroup.setAttribute("HEIGHT",GrH);
		obGroup.setAttribute("NUMPO", PoLine.size());
		QString polp = "";
		double xf, yf;
		QString tmpSt, tmpSt2;
		for (uint nxx=0; nxx<PoLine.size(); ++nxx)
		{
			PoLine.point(nxx, &xf, &yf);
			polp += tmpSt.setNum(xf) + " " + tmpSt2.setNum(yf) + " ";
		}
		obGroup.setAttribute("POCOOR", polp);
	}
	group.setAttribute("W", GrW);
	group.setAttribute("H", GrH);
	return data.toString();
}

void StencilReader::parseGroup(QDomDocument &data, QDomElement &group, QDomNode &DOC, int groupElemCounter, int &groupElemCounter2, double Dx, double Dy)
{
	QString tmp = "";
	QString FillCol = "White";
	QString StrokeCol = "Black";
	QString defFillCol = "White";
	QString defStrokeCol = "Black";
	QColor stroke = Qt::black;
	QColor fill = Qt::white;
	Qt::PenStyle Dash = Qt::SolidLine;
	Qt::PenCapStyle LineEnd = Qt::FlatCap;
	Qt::PenJoinStyle LineJoin = Qt::MiterJoin;
	int fillStyle = 1;
	double strokewidth = 0.1;
	bool poly = false;
	while(!DOC.isNull())
	{
		double x1, y1, x2, y2;
		StrokeCol = defStrokeCol;
		FillCol = defFillCol;
		stroke = Qt::black;
		fill = Qt::white;
		fillStyle = 1;
		strokewidth = 1.0;
		Dash = Qt::SolidLine;
		LineEnd = Qt::FlatCap;
		LineJoin = Qt::MiterJoin;
		FPointArray PoLine;
		PoLine.resize(0);
		QDomElement pg=DOC.toElement();
		QString STag = pg.tagName();
		QString style = pg.attribute( "style", "" ).simplified();
		if (style.isEmpty())
			style = pg.attribute( "svg:style", "" ).simplified();
		if (!style.isEmpty())
		{
			FillCol = "None";
			strokewidth = 0.1;
			QStringList substyles = style.split(';', QString::SkipEmptyParts);
			for( QStringList::Iterator it = substyles.begin(); it != substyles.end(); ++it )
			{
				QStringList substyle = (*it).split(':', QString::SkipEmptyParts);
				QString command(substyle[0].trimmed());
				QString params(substyle[1].trimmed());
				if (command == "fill")
				{
					if ((params == "foreground") || (params == "fg") || (params == "inverse"))
						FillCol = defStrokeCol;
					else if ((params == "background") || (params == "bg"))
						FillCol = defFillCol;
					else if  (params == "default")
						FillCol = "None";
					else if (params == "none")
						FillCol = "None";
					else
					{
						fill.setNamedColor( params );
						if (fill == Qt::white)
							FillCol = "White";
						else if (fill == Qt::black)
							FillCol = "Black";
						else
							FillCol = "FromDia"+fill.name();
					}
				}
				else if (command == "stroke")
				{
					if ((params == "foreground") || (params == "fg"))
						StrokeCol = defStrokeCol;
					else if ((params == "background") || (params == "bg") || (params == "inverse"))
						StrokeCol = defFillCol;
					else if  (params == "default")
						StrokeCol = defStrokeCol;
					else if (params == "none")
						StrokeCol = "None";
					else
					{
						stroke.setNamedColor( params );
						if (stroke == Qt::white)
							StrokeCol = "White";
						else if (stroke == Qt::black)
							StrokeCol = "Black";
						else
							StrokeCol = "FromDia"+stroke.name();
					}
				}
				else if (command == "stroke-width")
					strokewidth = params.toDouble();
				else if( command == "stroke-linejoin" )
				{
					if( params == "miter" )
						LineJoin = Qt::MiterJoin;
					else if( params == "round" )
						LineJoin = Qt::RoundJoin;
					else if( params == "bevel" )
						LineJoin = Qt::BevelJoin;
				}
				else if( command == "stroke-linecap" )
				{
					if( params == "butt" )
						LineEnd = Qt::FlatCap;
					else if( params == "round" )
						LineEnd = Qt::RoundCap;
					else if( params == "square" )
						LineEnd = Qt::SquareCap;
				}
			}
		}
		if (STag == "svg:line")
		{
			x1 = pg.attribute("x1").toDouble() * Conversion;
			y1 = pg.attribute("y1").toDouble() * Conversion;
			x2 = pg.attribute("x2").toDouble() * Conversion;
			y2 = pg.attribute("y2").toDouble() * Conversion;
			PoLine.addPoint(x1, y1);
			PoLine.addPoint(x1, y1);
			PoLine.addPoint(x2, y2);
			PoLine.addPoint(x2, y2);
		}
		else if (STag == "svg:rect")
		{
			x1 = pg.attribute("x").toDouble() * Conversion;
			y1 = pg.attribute("y").toDouble() * Conversion;
			x2 = pg.attribute("width").toDouble() * Conversion;
			y2 = pg.attribute("height").toDouble() * Conversion;
			static double rect[] = {0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0,
									1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0,
									0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0};
			for (int a = 0; a < 29; a += 4)
			{
				double xa = x2 * rect[a];
				double ya = y2 * rect[a+1];
				double xb = x2 * rect[a+2];
				double yb = y2 * rect[a+3];
				PoLine.addPoint(x1+xa, y1+ya);
				PoLine.addPoint(x1+xb, y1+yb);
			}
		}
		else if ((STag == "svg:polygon") || (STag == "svg:polyline"))
		{
			bool bFirst = true;
			double x = 0.0;
			double y = 0.0;
			QString points = pg.attribute( "points" ).simplified().replace(',', " ");
			QStringList pointList = points.split(' ', QString::SkipEmptyParts);
			FirstM = true;
			for( QStringList::Iterator it = pointList.begin(); it != pointList.end(); it++ )
			{
				if( bFirst )
				{
					x = (*(it++)).toDouble();
					y = (*it).toDouble();
					svgMoveTo(x * Conversion, y * Conversion);
					bFirst = false;
					WasM = true;
				}
				else
				{
					x = (*(it++)).toDouble();
					y = (*it).toDouble();
					svgLineTo(&PoLine, x * Conversion, y * Conversion);
				}
			}
			if (STag == "svg:polygon")
				svgClosePath(&PoLine);
			if (PoLine.size() < 4)
			{
				DOC = DOC.nextSibling();
				continue;
			}
		}
		else if (STag == "svg:circle")
		{
			x1 = pg.attribute("r").toDouble() * Conversion;
			y1 = pg.attribute("r").toDouble() * Conversion;
			x2 = pg.attribute("cx").toDouble() * Conversion - x1;
			y2 = pg.attribute("cy").toDouble() * Conversion - y1;
			x1 *= 2.0;
			y1 *= 2.0;
			static double rect[] = {1.0, 0.5, 1.0, 0.77615235,0.5, 1.0, 0.77615235, 1.0,
									0.5, 1.0, 0.22385765, 1.0, 0.0, 0.5, 0.0, 0.77615235,
									0.0, 0.5, 0.0, 0.22385765, 0.5, 0.0, 0.22385765, 0.0,
									0.5, 0.0, 0.77615235, 0.0, 1.0, 0.5, 1.0, 0.22385765};
			for (int a = 0; a < 29; a += 4)
			{
				double xa = x1 * rect[a];
				double ya = y1 * rect[a+1];
				double xb = x1 * rect[a+2];
				double yb = y1 * rect[a+3];
				PoLine.addPoint(x2+xa, y2+ya);
				PoLine.addPoint(x2+xb, y2+yb);
			}
		}
		else if (STag == "svg:ellipse")
		{
			x1 = pg.attribute("rx").toDouble() * Conversion;
			y1 = pg.attribute("ry").toDouble() * Conversion;
			x2 = pg.attribute("cx").toDouble() * Conversion - x1;
			y2 = pg.attribute("cy").toDouble() * Conversion - y1;
			x1 *= 2.0;
			y1 *= 2.0;
			static double rect[] = {1.0, 0.5, 1.0, 0.77615235,0.5, 1.0, 0.77615235, 1.0,
									0.5, 1.0, 0.22385765, 1.0, 0.0, 0.5, 0.0, 0.77615235,
									0.0, 0.5, 0.0, 0.22385765, 0.5, 0.0, 0.22385765, 0.0,
									0.5, 0.0, 0.77615235, 0.0, 1.0, 0.5, 1.0, 0.22385765};
			for (int a = 0; a < 29; a += 4)
			{
				double xa = x1 * rect[a];
				double ya = y1 * rect[a+1];
				double xb = x1 * rect[a+2];
				double yb = y1 * rect[a+3];
				PoLine.addPoint(x2+xa, y2+ya);
				PoLine.addPoint(x2+xb, y2+yb);
			}
		}
		else if (STag == "svg:path")
		{
			poly = parseSVG( pg.attribute( "d" ), &PoLine );
			if (PoLine.size() < 4)
			{
				DOC = DOC.nextSibling();
				continue;
			}
		}
		else if (STag == "svg:g")
		{
			QDomNode child = DOC.firstChild();
			parseGroup(data, group, child, groupElemCounter, groupElemCounter2, Dx, Dy);
		}
		if (PoLine.size() < 4)
		{
			DOC = DOC.nextSibling();
			continue;
		}
		QDomElement ob = data.createElement("ITEM");
		ob.setAttribute("PWIDTH", strokewidth);
		ob.setAttribute("PCOLOR2", StrokeCol);
		writeDefaults(ob);
		ob.setAttribute("PLINEART", Dash);
		ob.setAttribute("PLINEEND", LineEnd);
		ob.setAttribute("PLINEJOIN", LineJoin);
		if (groupElemCounter != 1)
		{
			ob.setAttribute("GROUPS", 1);
			ob.setAttribute("NUMGROUP", 1);
		}
		else
		{
			ob.setAttribute("GROUPS", "");
			ob.setAttribute("NUMGROUP", 0);
		}
		if ((STag == "svg:rect") || (STag == "svg:polygon") || (STag == "svg:circle") || (STag == "svg:ellipse"))
		{
			ob.setAttribute("PTYPE", PageItem::Polygon);
			ob.setAttribute("PCOLOR", FillCol);
		}
		else if ((STag == "svg:line") || (STag == "svg:polyline"))
		{
			ob.setAttribute("PTYPE", PageItem::PolyLine);
			ob.setAttribute("PCOLOR", "None");
		}
		else if (STag == "svg:path")
		{
			if (poly)
			{
				ob.setAttribute("PTYPE", PageItem::PolyLine);
				ob.setAttribute("PCOLOR", "None");
			}
			else
			{
				ob.setAttribute("PTYPE", PageItem::Polygon);
				ob.setAttribute("PCOLOR", FillCol);
			}
		}
		FPoint tp2(getMinClipF(&PoLine));
		PoLine.translate(-tp2.x(), -tp2.y());
		FPoint wh = getMaxClipF(&PoLine);
		ob.setAttribute("XPOS", tp2.x() - Dx);
		ob.setAttribute("YPOS",tp2.y() - Dy);
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
		group.appendChild(ob);
		groupElemCounter2++;
		DOC = DOC.nextSibling();
	}
}

void StencilReader::parseGroupProperties(QDomDocument &data, QDomElement &group, QDomNode &DOC, int &groupElemCounter, double &minXCoor, double &minYCoor, double &maxXCoor, double &maxYCoor, bool &firstCheck)
{
	QString FillCol = "White";
	QString StrokeCol = "Black";
	QColor stroke = Qt::black;
	QColor fill = Qt::white;
	while(!DOC.isNull())
	{
		double x1, y1, x2, y2;
		FPointArray PoLine;
		PoLine.resize(0);
		QDomElement pg = DOC.toElement();
		QString STag = pg.tagName();
		QString style = pg.attribute( "style", "" ).simplified();
		if (style.isEmpty())
			style = pg.attribute( "svg:style", "" ).simplified();
		QStringList substyles = style.split(';', QString::SkipEmptyParts);
		for( QStringList::Iterator it = substyles.begin(); it != substyles.end(); ++it )
		{
			QStringList substyle = (*it).split(':', QString::SkipEmptyParts);
			QString command(substyle[0].trimmed());
			QString params(substyle[1].trimmed());
			if (command == "fill")
			{
				if (!((params == "foreground") || (params == "background") || (params == "fg") || (params == "bg") || (params == "none") || (params == "default") || (params == "inverse")))
				{
					fill.setNamedColor( params );
					FillCol = "FromDia"+fill.name();
				}
			}
			else if (command == "stroke")
			{
				if (!((params == "foreground") || (params == "background") || (params == "fg") || (params == "bg") || (params == "none") || (params == "default")) || (params == "inverse"))
				{
					fill.setNamedColor( params );
					FillCol = "FromDia"+fill.name();
				}
			}
			ColorList::Iterator itc;
			bool found = false;
			int r, g, b;
			QColor tmpR;
			if ((fill == Qt::white) || (fill == Qt::black))
				continue;
			for (itc = PageColors.begin(); itc != PageColors.end(); ++itc)
			{
				if (itc.value().getColorModel() == colorModelRGB)
				{
					itc.value().getRGB(&r, &g, &b);
					tmpR.setRgb(r, g, b);
					if (fill == tmpR)
					{
						FillCol = itc.key();
						found = true;
						break;
					}
				}
			}
			if (!found)
			{
				ScColor tmp;
				tmp.fromQColor(fill);
				tmp.setSpotColor(false);
				tmp.setRegistrationColor(false);
				PageColors.insert(FillCol, tmp);
				QDomElement co = data.createElement("COLOR");
				co.setAttribute("NAME",FillCol);
				co.setAttribute("RGB",fill.name());
				co.setAttribute("Spot","0");
				co.setAttribute("Register","0");
				group.appendChild(co);
			}
		}
		if (STag == "svg:line")
		{
			x1 = pg.attribute("x1").toDouble() * Conversion;
			y1 = pg.attribute("y1").toDouble() * Conversion;
			x2 = pg.attribute("x2").toDouble() * Conversion;
			y2 = pg.attribute("y2").toDouble() * Conversion;
			PoLine.addPoint(x1, y1);
			PoLine.addPoint(x1, y1);
			PoLine.addPoint(x2, y2);
			PoLine.addPoint(x2, y2);
		}
		else if (STag == "svg:rect")
		{
			x1 = pg.attribute("x").toDouble() * Conversion;
			y1 = pg.attribute("y").toDouble() * Conversion;
			x2 = pg.attribute("width").toDouble() * Conversion;
			y2 = pg.attribute("height").toDouble() * Conversion;
			static double rect[] = {0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0,
									1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0,
									0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0};
			for (int a = 0; a < 29; a += 4)
			{
				double xa = x2 * rect[a];
				double ya = y2 * rect[a+1];
				double xb = x2 * rect[a+2];
				double yb = y2 * rect[a+3];
				PoLine.addPoint(x1+xa, y1+ya);
				PoLine.addPoint(x1+xb, y1+yb);
			}
		}
		else if ((STag == "svg:polygon") || (STag == "svg:polyline"))
		{
			bool bFirst = true;
			double x = 0.0;
			double y = 0.0;
			QString points = pg.attribute( "points" ).simplified().replace(',', " ");
			QStringList pointList = points.split(' ', QString::SkipEmptyParts);
			FirstM = true;
			for( QStringList::Iterator it1 = pointList.begin(); it1 != pointList.end(); it1++ )
			{
				if( bFirst )
				{
					x = (*(it1++)).toDouble();
					y = (*it1).toDouble();
					svgMoveTo(x * Conversion, y * Conversion);
					bFirst = false;
					WasM = true;
				}
				else
				{
					x = (*(it1++)).toDouble();
					y = (*it1).toDouble();
					svgLineTo(&PoLine, x * Conversion, y * Conversion);
				}
			}
			if (STag == "svg:polygon")
				svgClosePath(&PoLine);
			if (PoLine.size() < 4)
			{
				DOC = DOC.nextSibling();
				continue;
			}
		}
		else if (STag == "svg:circle")
		{
			x1 = pg.attribute("r").toDouble() * Conversion;
			y1 = pg.attribute("r").toDouble() * Conversion;
			x2 = pg.attribute("cx").toDouble() * Conversion - x1;
			y2 = pg.attribute("cy").toDouble() * Conversion - y1;
			x1 *= 2.0;
			y1 *= 2.0;
			static double rect[] = {1.0, 0.5, 1.0, 0.77615235,0.5, 1.0, 0.77615235, 1.0,
									0.5, 1.0, 0.22385765, 1.0, 0.0, 0.5, 0.0, 0.77615235,
									0.0, 0.5, 0.0, 0.22385765, 0.5, 0.0, 0.22385765, 0.0,
									0.5, 0.0, 0.77615235, 0.0, 1.0, 0.5, 1.0, 0.22385765};
			for (int a = 0; a < 29; a += 4)
			{
				double xa = x1 * rect[a];
				double ya = y1 * rect[a+1];
				double xb = x1 * rect[a+2];
				double yb = y1 * rect[a+3];
				PoLine.addPoint(x2+xa, y2+ya);
				PoLine.addPoint(x2+xb, y2+yb);
			}
		}
		else if (STag == "svg:ellipse")
		{
			x1 = pg.attribute("rx").toDouble() * Conversion;
			y1 = pg.attribute("ry").toDouble() * Conversion;
			x2 = pg.attribute("cx").toDouble() * Conversion - x1;
			y2 = pg.attribute("cy").toDouble() * Conversion - y1;
			x1 *= 2.0;
			y1 *= 2.0;
			static double rect[] = {1.0, 0.5, 1.0, 0.77615235,0.5, 1.0, 0.77615235, 1.0,
									0.5, 1.0, 0.22385765, 1.0, 0.0, 0.5, 0.0, 0.77615235,
									0.0, 0.5, 0.0, 0.22385765, 0.5, 0.0, 0.22385765, 0.0,
									0.5, 0.0, 0.77615235, 0.0, 1.0, 0.5, 1.0, 0.22385765};
			for (int a = 0; a < 29; a += 4)
			{
				double xa = x1 * rect[a];
				double ya = y1 * rect[a+1];
				double xb = x1 * rect[a+2];
				double yb = y1 * rect[a+3];
				PoLine.addPoint(x2+xa, y2+ya);
				PoLine.addPoint(x2+xb, y2+yb);
			}
		}
		else if (STag == "svg:path")
		{
			parseSVG( pg.attribute( "d" ), &PoLine );
			if (PoLine.size() < 4)
			{
				DOC = DOC.nextSibling();
				continue;
			}
		}
		else if (STag == "svg:g")
		{
			QDomNode child = DOC.firstChild();
			parseGroupProperties(data, group, child, groupElemCounter, minXCoor, minYCoor, maxXCoor, maxYCoor, firstCheck);
		}
		if (PoLine.size() < 4)
		{
			DOC = DOC.nextSibling();
			continue;
		}
		groupElemCounter++;
		FPoint tp2(getMinClipF(&PoLine));
		PoLine.translate(-tp2.x(), -tp2.y());
		FPoint wh(getMaxClipF(&PoLine));
		if (firstCheck)
		{
			minXCoor = tp2.x();
			minYCoor = tp2.y();
			maxXCoor = tp2.x() + wh.x();
			maxYCoor = tp2.y() + wh.y();
			firstCheck = false;
		}
		else
		{
			minXCoor = qMin(minXCoor, tp2.x());
			minYCoor = qMin(minYCoor, tp2.y());
			maxXCoor = qMax(maxXCoor, tp2.x() + wh.x());
			maxYCoor = qMax(maxYCoor, tp2.y() + wh.y());
		}
		DOC = DOC.nextSibling();
	}
}

double StencilReader::parseUnit(const QString &unit)
{
	QString sCM(unitGetUntranslatedStrFromIndex(SC_CM));
	QString sMM(unitGetUntranslatedStrFromIndex(SC_MM));
	QString sIN(unitGetUntranslatedStrFromIndex(SC_IN));
	QString sPT(unitGetUntranslatedStrFromIndex(SC_PT));
	QString sPX("px");
	
	bool noUnit = false;
	QString unitval(unit);
	if( unit.right( 2 ) == sPT )
		unitval.replace( sPT, "" );
	else if( unit.right( 2 ) == sCM )
		unitval.replace( sCM, "" );
	else if( unit.right( 2 ) == sMM )
		unitval.replace( sMM , "" );
	else if( unit.right( 2 ) == sIN )
		unitval.replace( sIN, "" );
	else if( unit.right( 2 ) == sPX )
		unitval.replace( sPX, "" );
	if (unitval == unit)
		noUnit = true;
	double value = unitval.toDouble();
	if( unit.right( 2 ) == sPT )
		value = value;
	else if( unit.right( 2 ) == sCM )
	{
		value = cm2pts(value);
		Conversion = 1/unitGetRatioFromIndex(SC_CM);
	}
	else if( unit.right( 2 ) == sMM )
	{
		value = mm2pts(value);
		Conversion = 1/unitGetRatioFromIndex(SC_MM);
	}
	else if( unit.right( 2 ) == sIN )
	{
		value = in2pts(value);
		Conversion = 1/unitGetRatioFromIndex(SC_IN);
	}
	else if( unit.right( 2 ) == sPX )
	{
		value = value * 0.8;
		Conversion = 0.8;
	}
	else if(noUnit)
		value = value;
	return value;
}

const char * StencilReader::getCoord( const char *ptr, double &number )
{
	int integer, exponent;
	double decimal, frac;
	int sign, expsign;

	exponent = 0;
	integer = 0;
	frac = 1.0;
	decimal = 0;
	sign = 1;
	expsign = 1;

	// read the sign
	if(*ptr == '+')
		ptr++;
	else if(*ptr == '-')
	{
		ptr++;
		sign = -1;
	}

	// read the integer part
	while(*ptr != '\0' && *ptr >= '0' && *ptr <= '9')
		integer = (integer * 10) + *(ptr++) - '0';
	if(*ptr == '.') // read the decimals
	{
		ptr++;
		while(*ptr != '\0' && *ptr >= '0' && *ptr <= '9')
			decimal += (*(ptr++) - '0') * (frac *= 0.1);
	}

	if(*ptr == 'e' || *ptr == 'E') // read the exponent part
	{
		ptr++;

		// read the sign of the exponent
		if(*ptr == '+')
			ptr++;
		else if(*ptr == '-')
		{
			ptr++;
			expsign = -1;
		}

		exponent = 0;
		while(*ptr != '\0' && *ptr >= '0' && *ptr <= '9')
		{
			exponent *= 10;
			exponent += *ptr - '0';
			ptr++;
		}
	}
	number = integer + decimal;
	number *= sign * pow( static_cast<double>(10), static_cast<double>( expsign * exponent ) );
	// skip the following space
	if(*ptr == ' ')
		ptr++;

	return ptr;
}

bool StencilReader::parseSVG( const QString &s, FPointArray *ite )
{
	QString d = s;
	d = d.replace( QRegExp( "," ), " ");
	bool ret = false;
	if( !d.isEmpty() )
	{
		d = d.simplified();
		QByteArray pathData = d.toLatin1();
		const char *ptr = pathData.constData();
		const char *end = pathData.constData() + pathData.length() + 1;
		double contrlx, contrly, curx, cury, subpathx, subpathy, tox, toy, x1, y1, x2, y2, xc, yc;
		double px1, py1, px2, py2, px3, py3;
		bool relative;
		FirstM = true;
		char command = *(ptr++), lastCommand = ' ';
		subpathx = subpathy = curx = cury = contrlx = contrly = 0.0;
		while( ptr < end )
		{
			if( *ptr == ' ' )
				ptr++;
			relative = false;
			switch( command )
			{
			case 'm':
				relative = true;
			case 'M':
				{
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					tox *= Conversion;
					toy *= Conversion;
					WasM = true;
					subpathx = curx = relative ? curx + tox : tox;
					subpathy = cury = relative ? cury + toy : toy;
					svgMoveTo(curx, cury );
					break;
				}
			case 'l':
				relative = true;
			case 'L':
				{
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					tox *= Conversion;
					toy *= Conversion;
					curx = relative ? curx + tox : tox;
					cury = relative ? cury + toy : toy;
					svgLineTo(ite, curx, cury );
					break;
				}
			case 'h':
				{
					ptr = getCoord( ptr, tox );
					tox *= Conversion;
					curx = curx + tox;
					svgLineTo(ite, curx, cury );
					break;
				}
			case 'H':
				{
					ptr = getCoord( ptr, tox );
					tox *= Conversion;
					curx = tox;
					svgLineTo(ite, curx, cury );
					break;
				}
			case 'v':
				{
					ptr = getCoord( ptr, toy );
					toy *= Conversion;
					cury = cury + toy;
					svgLineTo(ite, curx, cury );
					break;
				}
			case 'V':
				{
					ptr = getCoord( ptr, toy );
					toy *= Conversion;
					cury = toy;
					svgLineTo(ite,  curx, cury );
					break;
				}
			case 'z':
			case 'Z':
				{
					curx = subpathx;
					cury = subpathy;
					svgClosePath(ite);
					break;
				}
			case 'c':
				relative = true;
			case 'C':
				{
					ptr = getCoord( ptr, x1 );
					ptr = getCoord( ptr, y1 );
					ptr = getCoord( ptr, x2 );
					ptr = getCoord( ptr, y2 );
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					tox *= Conversion;
					toy *= Conversion;
					x1 *= Conversion;
					y1 *= Conversion;
					x2 *= Conversion;
					y2 *= Conversion;
					px1 = relative ? curx + x1 : x1;
					py1 = relative ? cury + y1 : y1;
					px2 = relative ? curx + x2 : x2;
					py2 = relative ? cury + y2 : y2;
					px3 = relative ? curx + tox : tox;
					py3 = relative ? cury + toy : toy;
					svgCurveToCubic(ite, px1, py1, px2, py2, px3, py3 );
					contrlx = relative ? curx + x2 : x2;
					contrly = relative ? cury + y2 : y2;
					curx = relative ? curx + tox : tox;
					cury = relative ? cury + toy : toy;
					break;
				}
			case 's':
				relative = true;
			case 'S':
				{
					ptr = getCoord( ptr, x2 );
					ptr = getCoord( ptr, y2 );
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					tox *= Conversion;
					toy *= Conversion;
					x2 *= Conversion;
					y2 *= Conversion;
					px1 = 2 * curx - contrlx;
					py1 = 2 * cury - contrly;
					px2 = relative ? curx + x2 : x2;
					py2 = relative ? cury + y2 : y2;
					px3 = relative ? curx + tox : tox;
					py3 = relative ? cury + toy : toy;
					svgCurveToCubic(ite, px1, py1, px2, py2, px3, py3 );
					contrlx = relative ? curx + x2 : x2;
					contrly = relative ? cury + y2 : y2;
					curx = relative ? curx + tox : tox;
					cury = relative ? cury + toy : toy;
					break;
				}
			case 'q':
				relative = true;
			case 'Q':
				{
					ptr = getCoord( ptr, x1 );
					ptr = getCoord( ptr, y1 );
					ptr = getCoord( ptr, tox );
					ptr = getCoord( ptr, toy );
					tox *= Conversion;
					toy *= Conversion;
					x1 *= Conversion;
					y1 *= Conversion;
					px1 = relative ? (curx + 2 * (x1 + curx)) * (1.0 / 3.0) : (curx + 2 * x1) * (1.0 / 3.0);
					py1 = relative ? (cury + 2 * (y1 + cury)) * (1.0 / 3.0) : (cury + 2 * y1) * (1.0 / 3.0);
					px2 = relative ? ((curx + tox) + 2 * (x1 + curx)) * (1.0 / 3.0) : (tox + 2 * x1) * (1.0 / 3.0);
					py2 = relative ? ((cury + toy) + 2 * (y1 + cury)) * (1.0 / 3.0) : (toy + 2 * y1) * (1.0 / 3.0);
					px3 = relative ? curx + tox : tox;
					py3 = relative ? cury + toy : toy;
					svgCurveToCubic(ite, px1, py1, px2, py2, px3, py3 );
					contrlx = relative ? curx + x1 : (tox + 2 * x1) * (1.0 / 3.0);
					contrly = relative ? cury + y1 : (toy + 2 * y1) * (1.0 / 3.0);
					curx = relative ? curx + tox : tox;
					cury = relative ? cury + toy : toy;
					break;
				}
			case 't':
				relative = true;
			case 'T':
				{
					ptr = getCoord(ptr, tox);
					ptr = getCoord(ptr, toy);
					tox *= Conversion;
					toy *= Conversion;
					xc = 2 * curx - contrlx;
					yc = 2 * cury - contrly;
					px1 = relative ? (curx + 2 * xc) * (1.0 / 3.0) : (curx + 2 * xc) * (1.0 / 3.0);
					py1 = relative ? (cury + 2 * yc) * (1.0 / 3.0) : (cury + 2 * yc) * (1.0 / 3.0);
					px2 = relative ? ((curx + tox) + 2 * xc) * (1.0 / 3.0) : (tox + 2 * xc) * (1.0 / 3.0);
					py2 = relative ? ((cury + toy) + 2 * yc) * (1.0 / 3.0) : (toy + 2 * yc) * (1.0 / 3.0);
					px3 = relative ? curx + tox : tox;
					py3 = relative ? cury + toy : toy;
					svgCurveToCubic(ite, px1, py1, px2, py2, px3, py3 );
					contrlx = xc;
					contrly = yc;
					curx = relative ? curx + tox : tox;
					cury = relative ? cury + toy : toy;
					break;
				}
			}
			lastCommand = command;
			if(*ptr == '+' || *ptr == '-' || (*ptr >= '0' && *ptr <= '9'))
			{
				// there are still coords in this command
				if(command == 'M')
					command = 'L';
				else if(command == 'm')
					command = 'l';
			}
			else
				command = *(ptr++);

			if( lastCommand != 'C' && lastCommand != 'c' &&
			        lastCommand != 'S' && lastCommand != 's' &&
			        lastCommand != 'Q' && lastCommand != 'q' &&
			        lastCommand != 'T' && lastCommand != 't')
			{
				contrlx = curx;
				contrly = cury;
			}
		}
		if ((lastCommand != 'z') && (lastCommand != 'Z'))
			ret = true;
		if (ite->size() > 2)
		{
			if ((ite->point(0).x() == ite->point(ite->size()-2).x()) && (ite->point(0).y() == ite->point(ite->size()-2).y()))
				ret = false;
		}
	}
	return ret;
}

void StencilReader::svgMoveTo(double x1, double y1)
{
	CurrX = x1;
	CurrY = y1;
	StartX = x1;
	StartY = y1;
	PathLen = 0;
}

void StencilReader::svgLineTo(FPointArray *i, double x1, double y1)
{
	if ((!FirstM) && (WasM))
	{
		i->setMarker();
		PathLen += 4;
	}
	FirstM = false;
	WasM = false;
	if (i->size() > 3)
	{
		FPoint b1 = i->point(i->size()-4);
		FPoint b2 = i->point(i->size()-3);
		FPoint b3 = i->point(i->size()-2);
		FPoint b4 = i->point(i->size()-1);
		FPoint n1 = FPoint(CurrX, CurrY);
		FPoint n2 = FPoint(x1, y1);
		if ((b1 == n1) && (b2 == n1) && (b3 == n2) && (b4 == n2))
			return;
	}
	i->addPoint(FPoint(CurrX, CurrY));
	i->addPoint(FPoint(CurrX, CurrY));
	i->addPoint(FPoint(x1, y1));
	i->addPoint(FPoint(x1, y1));
	CurrX = x1;
	CurrY = y1;
	PathLen += 4;
}

void StencilReader::svgCurveToCubic(FPointArray *i, double x1, double y1, double x2, double y2, double x3, double y3)
{
	if ((!FirstM) && (WasM))
	{
		i->setMarker();
		PathLen += 4;
	}
	FirstM = false;
	WasM = false;
	if (PathLen > 3)
	{
		FPoint b1 = i->point(i->size()-4);
		FPoint b2 = i->point(i->size()-3);
		FPoint b3 = i->point(i->size()-2);
		FPoint b4 = i->point(i->size()-1);
		FPoint n1 = FPoint(CurrX, CurrY);
		FPoint n2 = FPoint(x1, y1);
		FPoint n3 = FPoint(x3, y3);
		FPoint n4 = FPoint(x2, y2);
		if ((b1 == n1) && (b2 == n2) && (b3 == n3) && (b4 == n4))
			return;
	}
	i->addPoint(FPoint(CurrX, CurrY));
	i->addPoint(FPoint(x1, y1));
	i->addPoint(FPoint(x3, y3));
	i->addPoint(FPoint(x2, y2));
	CurrX = x3;
	CurrY = y3;
	PathLen += 4;
}

void StencilReader::svgClosePath(FPointArray *i)
{
	if (PathLen > 2)
	{
		if ((PathLen == 4) || (i->point(i->size()-2).x() != StartX) || (i->point(i->size()-2).y() != StartY))
		{
			i->addPoint(i->point(i->size()-2));
			i->addPoint(i->point(i->size()-3));
			i->addPoint(FPoint(StartX, StartY));
			i->addPoint(FPoint(StartX, StartY));
		}
	}
}

QPixmap StencilReader::createPreview(QString data)
{
	double pmmax;
	double GrW = 50.0;
	double GrH = 50.0;
	QImage tmp = QImage(0, 0);
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
		return QPixmap();
	QDomNodeList list = elem.elementsByTagName("Dimensions");
	if (list.count() == 0)
		return QPixmap();
	QDomElement dims = list.item(0).toElement();
	GrW = dims.attribute("w","50").toDouble()+10;
	GrH = dims.attribute("h","50").toDouble()+10;
	pmmax = 60 / qMax(GrW, GrH);
	tmp = QImage(static_cast<int>(GrW), static_cast<int>(GrH), QImage::Format_ARGB32);
	pS = new ScPainter(&tmp, tmp.width(), tmp.height());
	pS->clear();
	pS->translate(5, 5);
	QDomNode DOC=elem.firstChild();
	while(!DOC.isNull())
	{
		QDomElement pg=DOC.toElement();
		if(pg.tagName()=="KivioShape")
		{
			stroke = Qt::black;
			fill = Qt::white;
			fillStyle = 1;
			strokewidth = 1.0;
			Dash = Qt::SolidLine;
			LineEnd = Qt::FlatCap;
			LineJoin = Qt::MiterJoin;
			FPointArray PoLine;
			PoLine.resize(0);
			double x = 0.0;
			double y = 0.0;
			double x1 = 0.0;
			double y1 = 0.0;
			double currx = 0.0;
			double curry = 0.0;
			double startx = 0.0;
			double starty = 0.0;
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
					Dash = Qt::PenStyle(pt.attribute("pattern", "1").toInt());
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
	QImage tmpi = tmp.scaled(static_cast<int>(tmp.width()*pmmax), static_cast<int>(tmp.height()*pmmax), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	QPixmap tmpil;
	tmpil=QPixmap::fromImage(tmpi);
	return tmpil;
}

void StencilReader::writeDefaults(QDomElement &ob)
{
	PrefsManager* prefsManager = PrefsManager::instance();
	ob.setAttribute("OwnPage", 0);
	ob.setAttribute("RADRECT", 0);
	ob.setAttribute("FRTYPE",  3);
	ob.setAttribute("CLIPEDIT", 1);
	ob.setAttribute("TXTFILL", prefsManager->appPrefs.toolSettings.dPenText);
	ob.setAttribute("TXTSTROKE", prefsManager->appPrefs.toolSettings.dStrokeText);
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
	ob.setAttribute("ISIZE", prefsManager->appPrefs.toolSettings.defSize / 10.0);
	ob.setAttribute("SCALETYPE", 1);
	ob.setAttribute("RATIO", 0);
	ob.setAttribute("PRINTABLE", 1);
	ob.setAttribute("ALIGN", "0");
	ob.setAttribute("BOOKMARK", "0");
	ob.setAttribute("fillRule", "1");
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
	ob.setAttribute("NUMCO", 0);
	ob.setAttribute("COCOOR", "");
}

QString StencilReader::createObjects(QString datain)
{
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
	QString FillCol = "White";
	QString StrokeCol = "Black";
	QDomDocument docu("scridoc");
	docu.setContent(datain);
	QDomElement elem=docu.documentElement();
	if (elem.tagName() != "KivioShapeStencil")
		return "";
	QDomNodeList list = elem.elementsByTagName("Dimensions");
	if (list.count() == 0)
		return tmp;
	QString name = "KivioGroup";
	QDomNodeList descList = elem.elementsByTagName("Title");
	if (descList.count() != 0)
	{
		QDomElement namElem = descList.item(0).toElement();
		name = namElem.attribute("data", "");
	}
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
	group.setAttribute("Version", QString(VERSION));

	PageColors.insert("Black", ScColor(0, 0, 0, 255));
	QDomElement co = data.createElement("COLOR");
	co.setAttribute("NAME","Black");
	co.setAttribute("CMYK", "#000000FF");
	co.setAttribute("Spot","0");
	co.setAttribute("Register","0");
	group.appendChild(co);
	PageColors.insert("White", ScColor(0, 0, 0, 0));
	QDomElement co2 = data.createElement("COLOR");
	co2.setAttribute("NAME","White");
	co2.setAttribute("CMYK", "#00000000");
	co2.setAttribute("Spot","0");
	co2.setAttribute("Register","0");
	group.appendChild(co2);
	QDomNodeList listStrokes = elem.elementsByTagName("KivioLineStyle");
	for (int st = 0; st < listStrokes.count(); st++)
	{
		QDomElement str = listStrokes.item(st).toElement();
		QString colnam = str.attribute("color","#ffffff");
		stroke.setNamedColor("#"+colnam.right(6));
		if ((stroke == Qt::white) || (stroke == Qt::black))
			continue;
		ColorList::Iterator it;
		bool found = false;
		int r, g, b;
		QColor tmpR;
		for (it = PageColors.begin(); it != PageColors.end(); ++it)
		{
			if (it.value().getColorModel() == colorModelRGB)
			{
				it.value().getRGB(&r, &g, &b);
				tmpR.setRgb(r, g, b);
				if (stroke == tmpR)
				{
					StrokeCol = it.key();
					found = true;
					break;
				}
			}
		}
		if (!found)
		{
			ScColor tmp;
			tmp.fromQColor(stroke);
			tmp.setSpotColor(false);
			tmp.setRegistrationColor(false);
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
	for (int st = 0; st < listFills.count(); st++)
	{
		QDomElement fil = listFills.item(st).toElement();
		QString colnam = fil.attribute("color","#ffffff");
		fill.setNamedColor("#"+colnam.right(6));
		ColorList::Iterator it;
		bool found = false;
		int r, g, b;
		QColor tmpR;
		if ((fill == Qt::white) || (fill == Qt::black))
			continue;
		for (it = PageColors.begin(); it != PageColors.end(); ++it)
		{
			if (it.value().getColorModel() == colorModelRGB)
			{
				it.value().getRGB(&r, &g, &b);
				tmpR.setRgb(r, g, b);
				if (fill == tmpR)
				{
					FillCol = it.key();
					found = true;
					break;
				}
			}
		}
		if (!found)
		{
			ScColor tmp;
			tmp.fromQColor(fill);
			tmp.setSpotColor(false);
			tmp.setRegistrationColor(false);
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
	QDomElement obGroup;
	int groupElemCounter = 0;
	if (listItems.count() != 1)
	{
		group.setAttribute("COUNT", listItems.count()+1);
		obGroup = data.createElement("ITEM");
		writeDefaults(obGroup);
		obGroup.setAttribute("PWIDTH", 0);
		obGroup.setAttribute("PCOLOR", "None");
		obGroup.setAttribute("PCOLOR2", "None");
		obGroup.setAttribute("PLINEART", Dash);
		obGroup.setAttribute("PLINEEND", LineEnd);
		obGroup.setAttribute("PLINEJOIN", LineJoin);
		obGroup.setAttribute("ANNAME", name.simplified().replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" ));
		obGroup.setAttribute("GROUPS", 1);
		obGroup.setAttribute("NUMGROUP", 1);
		obGroup.setAttribute("isGroupControl", 1);
		obGroup.setAttribute("PTYPE", PageItem::Polygon);
		static double rect[] = {0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0,
											1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0,
											0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0};
		FPointArray PoLine;
		PoLine.resize(0);
		for (int a = 0; a < 29; a += 4)
		{
			double xa = GrW * rect[a];
			double ya = GrH * rect[a+1];
			double xb = GrW * rect[a+2];
			double yb = GrH * rect[a+3];
			PoLine.addPoint(0+xa, 0+ya);
			PoLine.addPoint(0+xb, 0+yb);
		}
		obGroup.setAttribute("XPOS", 0);
		obGroup.setAttribute("YPOS",0);
		obGroup.setAttribute("WIDTH",GrW);
		obGroup.setAttribute("HEIGHT",GrH);
		obGroup.setAttribute("NUMPO", PoLine.size());
		QString polp = "";
		double xf, yf;
		QString tmpSt, tmpSt2;
		for (uint nxx=0; nxx<PoLine.size(); ++nxx)
		{
			PoLine.point(nxx, &xf, &yf);
			polp += tmpSt.setNum(xf) + " " + tmpSt2.setNum(yf) + " ";
		}
		obGroup.setAttribute("POCOOR", polp);
		group.appendChild(obGroup);
	}
	else
		group.setAttribute("COUNT", listItems.count());
	QDomNode DOC=elem.firstChild();
	while(!DOC.isNull())
	{
		QDomElement pg=DOC.toElement();
		if(pg.tagName()=="KivioShape")
		{
			FillCol = "White";
			StrokeCol = "Black";
			fillStyle = 1;
			strokewidth = 1.0;
			Dash = Qt::SolidLine;
			LineEnd = Qt::FlatCap;
			LineJoin = Qt::MiterJoin;
			FPointArray PoLine;
			PoLine.resize(0);
			double x = 0.0;
			double y = 0.0;
			double x1 = 0.0;
			double y1 = 0.0;
			double currx = 0.0;
			double curry = 0.0;
			double startx = 0.0;
			double starty = 0.0;
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
						FillCol = CommonStrings::None;
					else
					{
						if (fill == Qt::white)
							FillCol = "White";
						else if (fill == Qt::black)
							FillCol = "Black";
						else
							FillCol = "FromKivio"+fill.name();
					}
				}
				if(pt.tagName()=="KivioLineStyle")
				{
					QString colnam = pt.attribute("color","#ffffff");
					stroke.setNamedColor("#"+colnam.right(6));
					if (stroke == Qt::white)
						StrokeCol = "White";
					else if (stroke == Qt::black)
						StrokeCol = "Black";
					else
						StrokeCol = "FromKivio"+stroke.name();
					strokewidth = pt.attribute("width", "1").toDouble();
					LineJoin = Qt::PenJoinStyle(pt.attribute("joinStyle", "0").toInt());
					Dash = Qt::PenStyle(pt.attribute("pattern", "1").toInt());
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
			writeDefaults(ob);
			ob.setAttribute("PWIDTH", strokewidth);
			if (typ == "TextBox")
			{
				ob.setAttribute("PCOLOR", CommonStrings::None);
				ob.setAttribute("PCOLOR2", CommonStrings::None);
			}
			else
			{
				ob.setAttribute("PCOLOR", FillCol);
				ob.setAttribute("PCOLOR2", StrokeCol);
			}
			ob.setAttribute("PLINEART", Dash);
			ob.setAttribute("PLINEEND", LineEnd);
			ob.setAttribute("PLINEJOIN", LineJoin);
			ob.setAttribute("ANNAME", pg.attribute("name"));
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
			else if ((typ == "Rectangle") || (typ == "TextBox"))
			{
				if (typ == "Rectangle")
					ob.setAttribute("PTYPE", PageItem::Polygon);
				else
					ob.setAttribute("PTYPE", PageItem::TextFrame);
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
			if (valid)
			{
				group.appendChild(ob);
				groupElemCounter++;
			}
		}
		DOC = DOC.nextSibling();
	}
	if (listItems.count() != 1)
		obGroup.setAttribute("groupsLastItem", groupElemCounter);
	return data.toString();
}
