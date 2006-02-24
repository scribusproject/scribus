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
#include "scribusXml.h"
#include "selection.h"


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
	QDomNode DOC=elem.firstChild();
	while(!DOC.isNull())
	{
		QDomElement pg=DOC.toElement();
		if(pg.tagName()=="Dimensions")
		{
			GrW = pg.attribute("w","50").toDouble()+10;
			GrH = pg.attribute("h","50").toDouble()+10;
			pmmax = 60 / QMAX(GrW, GrH);
			tmp = QPixmap(static_cast<int>(GrW), static_cast<int>(GrH));
			pS = new ScPainter(&tmp, tmp.width(), tmp.height());
			pS->translate(5, 5);
		}
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

QString StencilReader::createObjects(QString data)
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
	if (!ScMW->HaveDoc)
		return tmp;
	currDoc = ScMW->doc;
	view = ScMW->view;
	if (!currDoc->PageColors.contains("FromKivio_Black"))
		currDoc->PageColors.insert("FromKivio_Black", ScColor(0, 0, 0, 255));
	if (!currDoc->PageColors.contains("FromKivio_White"))
		currDoc->PageColors.insert("FromKivio_White", ScColor(0, 0, 0, 0));
	QString FillCol = "FromKivio_White";
	QString StrokeCol = "FromKivio_Black";
	FPoint minSize = currDoc->minCanvasCoordinate;
	FPoint maxSize = currDoc->maxCanvasCoordinate;
	double BaseX = currDoc->currentPage->xOffset();
	double BaseY = currDoc->currentPage->yOffset();
	view->Deselect();
	Elements.clear();
	currDoc->setLoading(true);
	currDoc->DoDrawing = false;
	view->setUpdatesEnabled(false);
	ScMW->ScriptRunning = true;
	QDomDocument docu("scridoc");
	docu.setContent(data);
	QDomElement elem=docu.documentElement();
	if (elem.tagName() != "KivioShapeStencil")
		return "";
	QDomNode DOC=elem.firstChild();
	while(!DOC.isNull())
	{
		QDomElement pg=DOC.toElement();
		if(pg.tagName()=="Dimensions")
		{
			GrW = pg.attribute("w","50").toDouble();
			GrH = pg.attribute("h","50").toDouble();
		}
		if(pg.tagName()=="KivioShape")
		{
			FillCol = "FromKivio_White";
			StrokeCol = "FromKivio_Black";
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
					ColorList::Iterator it;
					bool found = false;
					int r, g, b;
					QColor tmpR;
					for (it = currDoc->PageColors.begin(); it != currDoc->PageColors.end(); ++it)
					{
						currDoc->PageColors[it.key()].getRGB(&r, &g, &b);
						tmpR.setRgb(r, g, b);
						if (fill == tmpR && currDoc->PageColors[it.key()].getColorModel() == colorModelRGB)
						{
							FillCol = it.key();
							found = true;
						}
					}
					if (!found)
					{
						ScColor tmp;
						tmp.fromQColor(fill);
						currDoc->PageColors.insert("FromKivio"+fill.name(), tmp);
						FillCol = "FromKivio"+fill.name();
					}
					if (fillStyle == 0)
						FillCol = "None";
				}
				if(pt.tagName()=="KivioLineStyle")
				{
					QString colnam = pt.attribute("color","#FFFFFF");
					stroke.setNamedColor("#"+colnam.right(6));
					ColorList::Iterator it;
					bool found = false;
					int r, g, b;
					QColor tmpR;
					for (it = currDoc->PageColors.begin(); it != currDoc->PageColors.end(); ++it)
					{
						currDoc->PageColors[it.key()].getRGB(&r, &g, &b);
						tmpR.setRgb(r, g, b);
						if (stroke == tmpR && currDoc->PageColors[it.key()].getColorModel() == colorModelRGB)
						{
							StrokeCol = it.key();
							found = true;
						}
					}
					if (!found)
					{
						ScColor tmp;
						tmp.fromQColor(stroke);
						currDoc->PageColors.insert("FromKivio"+stroke.name(), tmp);
						StrokeCol = "FromKivio"+stroke.name();
					}
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
				int z = currDoc->itemAdd(PageItem::Polygon, PageItem::Unspecified, BaseX, BaseY, GrW, GrH, strokewidth, FillCol, StrokeCol, true);
				PageItem* ite = currDoc->Items->at(z);
				ite->PoLine.resize(0);
				ite->PoLine = PoLine.copy();
				ite->PLineEnd = LineEnd;
				ite->PLineJoin = LineJoin;
				ite->setTextFlowsAroundFrame(false);
				ite->ClipEdited = true;
				ite->FrameType = 3;
				FPoint wh = getMaxClipF(&ite->PoLine);
				ite->setWidthHeight(wh.x(), wh.y());
				ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
				view->AdjustItemSize(ite);
				Elements.append(ite);
			}
			else if ((typ == "Bezier") || (typ == "OpenPath") || (typ == "LineArray") || (typ == "Polyline"))
			{
				int z = currDoc->itemAdd(PageItem::PolyLine, PageItem::Unspecified, BaseX, BaseY, GrW, GrH, strokewidth, FillCol, StrokeCol, true);
				PageItem* ite = currDoc->Items->at(z);
				ite->PoLine.resize(0);
				ite->PoLine = PoLine.copy();
				ite->PLineEnd = LineEnd;
				ite->PLineJoin = LineJoin;
				ite->setTextFlowsAroundFrame(false);
				ite->ClipEdited = true;
				ite->FrameType = 3;
				FPoint wh = getMaxClipF(&ite->PoLine);
				ite->setWidthHeight(wh.x(), wh.y());
				ite->Clip = FlattenPath(ite->PoLine, ite->Segments);
				view->AdjustItemSize(ite);
				Elements.append(ite);
			}
			if (typ == "Rectangle")
			{
				x = pg.attribute("x").toDouble();
				y = pg.attribute("y").toDouble();
				x1 = pg.attribute("w").toDouble();
				y1 = pg.attribute("h").toDouble();
				int z = currDoc->itemAdd(PageItem::Polygon, PageItem::Rectangle, BaseX+x, BaseY+y, x1, y1, strokewidth, FillCol, StrokeCol, true);
				PageItem* ite = currDoc->Items->at(z);
				ite->PLineEnd = LineEnd;
				ite->PLineJoin = LineJoin;
				ite->setTextFlowsAroundFrame(false);
				Elements.append(ite);
			}
			if (typ == "Ellipse")
			{
				x = pg.attribute("x").toDouble();
				y = pg.attribute("y").toDouble();
				x1 = pg.attribute("w").toDouble();
				y1 = pg.attribute("h").toDouble();
				int z = currDoc->itemAdd(PageItem::Polygon, PageItem::Ellipse, BaseX+x, BaseY+y, x1, y1, strokewidth, FillCol, StrokeCol, true);
				PageItem* ite = currDoc->Items->at(z);
				ite->PLineEnd = LineEnd;
				ite->PLineJoin = LineJoin;
				ite->setTextFlowsAroundFrame(false);
				Elements.append(ite);
			}
		}
		DOC = DOC.nextSibling();
	}
	currDoc->selection->clear();
	if (Elements.count() > 1)
	{
		for (uint a = 0; a < Elements.count(); ++a)
		{
			Elements.at(a)->Groups.push(currDoc->GroupCounter);
		}
		currDoc->GroupCounter++;
	}
	currDoc->setLoading(false);
	if (Elements.count() > 0)
	{
		for (uint dre=0; dre<Elements.count(); ++dre)
		{
			currDoc->selection->addItem(Elements.at(dre));
		}
		ScriXmlDoc *ss = new ScriXmlDoc();
		view->setGroupRect();
		tmp = ss->WriteElem(currDoc, view, currDoc->selection);
		currDoc->itemSelection_DeleteItem();
		view->resizeContents(qRound((maxSize.x() - minSize.x()) * view->scale()), qRound((maxSize.y() - minSize.y()) * view->scale()));
		view->scrollBy(qRound((currDoc->minCanvasCoordinate.x() - minSize.x()) * view->scale()), qRound((currDoc->minCanvasCoordinate.y() - minSize.y()) * view->scale()));
		currDoc->minCanvasCoordinate = minSize;
		currDoc->maxCanvasCoordinate = maxSize;
		delete ss;
	}
	ScMW->ScriptRunning = false;
	currDoc->DoDrawing = true;
	view->setUpdatesEnabled(true);
	return tmp;
}
