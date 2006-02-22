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


StencilReader::StencilReader()
{
}

QPixmap StencilReader::createPreview(QString data)
{
	double GrW, GrH, pmmax;
	QPixmap tmp = QPixmap(0, 0);
	QColor stroke = Qt::black;
	QColor fill = Qt::white;
	ScPainter *pS = NULL;
	double strokewidth = 1.0;
	QDomDocument docu("scridoc");
	docu.setContent(data);
	QDomElement elem=docu.documentElement();
	if (elem.tagName() != "KivioShapeStencil")
		return tmp;
	QDomNode DOC=elem.firstChild();
	while(!DOC.isNull())
	{
		QDomElement pg=DOC.toElement();
		if(pg.tagName()=="Dimensions")
		{
			GrW = pg.attribute("w").toDouble()+10;
			GrH = pg.attribute("h").toDouble()+10;
			pmmax = 60 / QMAX(GrW, GrH);
			tmp = QPixmap(static_cast<int>(GrW), static_cast<int>(GrH));
			pS = new ScPainter(&tmp, tmp.width(), tmp.height());
			pS->translate(5, 5);
		}
		if(pg.tagName()=="KivioShape")
		{
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
					fill.setNamedColor(pt.attribute("color"));
				}
				if(pt.tagName()=="KivioLineStyle")
				{
					stroke.setNamedColor(pt.attribute("color"));
					strokewidth = pt.attribute("width", "1").toDouble();
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
				pS->setFillMode(1);
				pS->setLineWidth(strokewidth);
				pS->setPen(stroke);
				pS->setupPolygon(&PoLine);
				pS->drawPolygon();
				pS->drawPolyLine();
			}
			else if ((typ == "Bezier") || (typ == "OpenPath") || (typ == "LineArray") || (typ == "Polyline"))
			{
				pS->setLineWidth(strokewidth);
				pS->setPen(stroke);
				pS->setupPolygon(&PoLine, false);
				pS->drawPolyLine();
			}
			if (typ == "Rectangle")
			{
				pS->setBrush(fill);
				pS->setFillMode(1);
				pS->setLineWidth(strokewidth);
				pS->setPen(stroke);
				x = pg.attribute("x").toDouble();
				y = pg.attribute("y").toDouble();
				x1 = pg.attribute("w").toDouble();
				y1 = pg.attribute("h").toDouble();
				pS->drawRect(x, y, x1, y1);
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
