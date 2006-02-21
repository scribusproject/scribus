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
	ScPainter *pS;
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
			GrW = pg.attribute("w").toDouble();
			GrH = pg.attribute("h").toDouble();
			pmmax = 60 / QMAX(GrW, GrH);
			tmp = QPixmap(static_cast<int>(GrW), static_cast<int>(GrH));
			pS = new ScPainter(&tmp, tmp.width(), tmp.height());
		}
		if(pg.tagName()=="KivioShape")
		{
			FPointArray PoLine;
			PoLine.resize(0);
			double x, y, currx, curry, startx, starty;
			QString typ = pg.attribute("type");
			QDomNode point = DOC.firstChild();
			bool first = true;
			bool first2 = true;
			bool second = true;
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
						if (pt.hasAttribute("bezier"))
							PoLine.addPoint(x, y);
					}
					else
					{
						if (!pt.hasAttribute("bezier"))
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
							if (first2)
							{
								currx = x;
								curry = y;
								first2 = false;
							}
							else
							{
								if (second)
								{
									PoLine.addPoint(x, y);
									second = false;
								}
								else
								{
									PoLine.addPoint(currx, curry);
									PoLine.addPoint(currx, curry);
									PoLine.addPoint(x, y);
									PoLine.addPoint(x, y);
									first = true;
									first2 = true;
									second = true;
								}
							}
						}
					}
				}
				point = point.nextSibling();
			}
			if ((typ == "Polygon") || (typ == "ClosedPath"))
			{
				pS->setBrush(fill);
				pS->setFillMode(1);
				pS->setLineWidth(strokewidth);
				pS->setupPolygon(&PoLine);
				pS->drawPolygon();
				pS->drawPolyLine();
			}
		}
		DOC = DOC.nextSibling();
	}
	pS->end();
	QImage tmpi1 = tmp.convertToImage();
	QImage tmpi = tmpi1.smoothScale(static_cast<int>(tmp.width()*pmmax), static_cast<int>(tmp.height()*pmmax));
	tmp.convertFromImage(tmpi);
	return tmp;
}
