/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          util.cpp  -  description
                             -------------------
    begin                : Fri Sep 14 2001
    copyright            : (C) 2001 by Franz Schmid
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

#include <QRegion> 
#include "util_math.h"
#include "scconfig.h"
#include "sclimits.h"
#include "fpoint.h"
#include "fpointarray.h"

using namespace std;

uint getDouble(const QByteArray& in, bool raw)
{
	QByteArray bb(4, ' ');
	if (raw)
	{
		bb[3] = in.at(0);
		bb[2] = in.at(1);
		bb[1] = in.at(2);
		bb[0] = in.at(3);
	}
	else
	{
		bb[0] = in.at(0);
		bb[1] = in.at(1);
		bb[2] = in.at(2);
		bb[3] = in.at(3);
	}
	uint ret;
	ret = bb[0] & 0xff;
	ret |= (bb[1] << 8) & 0xff00;
	ret |= (bb[2] << 16) & 0xff0000;
	ret |= (bb[3] << 24) & 0xff000000;
	return ret;
}

QPainterPath regularPolygonPath(double w, double h, uint c, bool star, double factor, double rota, double factor2, double innerRot, double factor3)
{
	uint cx = star ? c * 2 : c;
	double seg = 360.0 / cx;
	double sc = rota + 180.0;
	double di = factor;
	double mx = 0;
	double my = 0;
	double trueLength = sqrt(pow(sin(seg / 180.0 * M_PI) * (w / 2.0), 2) + pow(cos(seg / 180.0 * M_PI) * (h / 2.0) + (h/2.0) - h, 2));
	QPainterPath pts;
	// calculate corner Points first
	QList<QPointF> cornerPoints;
	cornerPoints.reserve(cx);
	for (uint x = 0; x < cx; ++x)
	{
		sc = seg * x + 180.0 + rota;
		if (star)
		{
			double wf = w / 2.0;
			double hf = h / 2.0;
			if (x % 2 != 0)
			{
				wf *= di;
				hf *= di;
				sc += innerRot;
			}
			mx = sin(sc / 180.0 * M_PI) * wf + (w / 2.0);
			my = cos(sc / 180.0 * M_PI) * hf + (h / 2.0);
		}
		else
		{
			mx = sin(sc / 180.0 * M_PI) * (w / 2.0) + (w / 2.0);
			my = cos(sc / 180.0 * M_PI) * (h / 2.0) + (h / 2.0);
		}
		cornerPoints.append(QPointF(mx, my));
	}
	// now calculate bezier control points if needed
	if (star)
	{
		pts.moveTo(cornerPoints[0]);
		double mxc1 = 0.0;
		double myc1 = 0.0;
		double mxc2 = 0.0;
		double myc2 = 0.0;
		for (int i = 0; i < cornerPoints.count() - 2; i++)
		{
			sc = seg * i + 180.0 + rota;
			if (i % 2 != 0)
			{
				// outer control point
				QLineF oline = QLineF(cornerPoints[i + 1], cornerPoints[i - 1]);
				mxc1 = oline.pointAt(factor3).x();
				myc1 = oline.pointAt(factor3).y();
				// inner control point
				mxc2 = sin((sc + 90.0) / 180.0 * M_PI) * (trueLength * factor2) + cornerPoints[i].x();
				myc2 = cos((sc + 90.0) / 180.0 * M_PI) * (trueLength * factor2) + cornerPoints[i].y();
				pts.cubicTo(QPointF(mxc2, myc2), QPointF(mxc1, myc1), cornerPoints[i+1]);
			}
			else
			{
				// outer control point
				QLineF oline = QLineF(cornerPoints[i], cornerPoints[i + 2]);
				mxc1 = oline.pointAt(factor3).x();
				myc1 = oline.pointAt(factor3).y();
				// inner control point
				mxc2 = sin((sc - 90.0 + seg) / 180.0 * M_PI) * (trueLength * factor2) + cornerPoints[i + 1].x();
				myc2 = cos((sc - 90.0 + seg) / 180.0 * M_PI) * (trueLength * factor2) + cornerPoints[i + 1].y();
				pts.cubicTo(QPointF(mxc1, myc1), QPointF(mxc2, myc2), cornerPoints[i + 1]);
			}
		}
		// outer control point
		QLineF oline = QLineF(cornerPoints[cornerPoints.count() - 2], cornerPoints[0]);
		mxc1 = oline.pointAt(factor3).x();
		myc1 = oline.pointAt(factor3).y();
		// inner control point
		sc = seg * (cornerPoints.count() - 1) + 180.0 + rota;
		mxc2 = sin((sc - 90.0) / 180.0 * M_PI) * (trueLength * factor2) + cornerPoints.last().x();
		myc2 = cos((sc - 90.0) / 180.0 * M_PI) * (trueLength * factor2) + cornerPoints.last().y();
		pts.cubicTo(QPointF(mxc1, myc1), QPointF(mxc2, myc2), cornerPoints.last());
		oline = QLineF(cornerPoints[0], cornerPoints[cornerPoints.count() - 2]);
		mxc1 = oline.pointAt(factor3).x();
		myc1 = oline.pointAt(factor3).y();
		// inner control point
		mxc2 = sin((sc + 90.0) / 180.0 * M_PI) * (trueLength * factor2) + cornerPoints.last().x();
		myc2 = cos((sc + 90.0) / 180.0 * M_PI) * (trueLength * factor2) + cornerPoints.last().y();
		pts.cubicTo(QPointF(mxc2, myc2), QPointF(mxc1, myc1), cornerPoints[0]);
		pts.closeSubpath();
	}
	else
	{
		pts.moveTo(cornerPoints[0]);
		for (int i = 1; i < cornerPoints.count(); ++i)
			pts.lineTo(cornerPoints[i]);
		pts.closeSubpath();
	}
	return pts;
}

QPainterPath spiralPath(double spiralWidth, double spiralHeight, double spiralStartAngle, double spiralEndAngle, double spiralFactor)
{
	if (spiralStartAngle >= spiralEndAngle)
		return QPainterPath();
	double startAngleK = spiralStartAngle;
	double endAngleK = spiralEndAngle;
	QPainterPath path;
	QPainterPath path2;
	double sh = spiralHeight / (spiralFactor + 1.0);
	double sw = 0.0;
	double ww = spiralWidth;
	double hh = spiralHeight - sh;
	double segStart = 0.0;
	double segEnd = 180.0;
	double spanAngle = 180.0;
	double startAngle = 0.0;
	bool segPart = true;
	bool draw = false;
	QPointF tp;
	path2.moveTo(sw, sh);
	while (segStart < endAngleK)
	{
		if (startAngleK >= segEnd)
		{
			tp = path2.currentPosition();
			if (segPart)
			{
				sw = tp.x();
				sh = spiralHeight / (spiralFactor + 1.0);
				path2.arcTo(sw, sh - hh, ww, hh * 2, 180, 180);
			}
			else
			{
				sw = tp.x() - ww;
				sh = spiralHeight / (spiralFactor + 1.0) - hh;
				path2.arcTo(sw, sh, ww, hh * 2, 0, 180);
			}
			segPart = !segPart;
			ww /= spiralFactor;
			hh /= spiralFactor;
			segStart += 180.0;
			segEnd += 180.0;
			spanAngle = 180.0;
			continue;
		}
		if ((startAngleK >= segStart) && (startAngleK <= segEnd))
		{
			startAngle = startAngleK + 180;
			spanAngle = segEnd - startAngleK;
			if ((endAngleK >= segStart) && (endAngleK <= segEnd))
				spanAngle -= segEnd - endAngleK;
			if (segPart)
			{
				sw = tp.x();
				sh = spiralHeight / (spiralFactor + 1.0);
				path.arcMoveTo(sw, sh - hh, ww, hh * 2, startAngle);
				path.arcTo(sw, sh - hh, ww, hh * 2, startAngle, spanAngle);
				startAngle = 0.0;
			}
			else
			{
				sw = tp.x() - ww;
				sh = spiralHeight / (spiralFactor + 1.0) - hh;
				path.arcMoveTo(sw, sh, ww, hh * 2, startAngle);
				path.arcTo(sw, sh, ww, hh * 2, startAngle, spanAngle);
				startAngle = 180.0;
			}
			draw = true;
			segPart = !segPart;
			ww /= spiralFactor;
			hh /= spiralFactor;
			if ((endAngleK >= segStart) && (endAngleK <= segEnd))
				break;
			segStart += 180.0;
			segEnd += 180.0;
			spanAngle = 180.0;
			continue;
		}
		if ((endAngleK >= segStart) && (endAngleK <= segEnd))
			spanAngle -= segEnd - endAngleK;
		tp = path.currentPosition();
		if (segPart)
		{
			sw = tp.x();
			sh = tp.y();
			if (draw)
				path.arcTo(sw, sh - hh, ww, hh * 2, startAngle, spanAngle);
			startAngle = 0.0;
		}
		else
		{
			sw = tp.x() - ww;
			sh = tp.y() - hh;
			if (draw)
				path.arcTo(sw, sh, ww, hh * 2, startAngle, spanAngle);
			startAngle = 180.0;
		}
		segPart = !segPart;
		ww /= spiralFactor;
		hh /= spiralFactor;
		if ((endAngleK >= segStart) && (endAngleK <= segEnd))
			break;
		segStart += 180.0;
		segEnd += 180.0;
		spanAngle = 180.0;
	}
	return path;
}

QList<QPainterPath> decomposePath(const QPainterPath &path)
{
	QList<QPainterPath> ret;
	QPainterPath part;
	QPainterPath::Element element1;
	QPainterPath::Element element2;

	bool first = true;
	for (int i = 0; i < path.elementCount(); ++i)
	{
		const QPainterPath::Element &elm = path.elementAt(i);
		if ((first) && (elm.type != QPainterPath::MoveToElement))
			part.moveTo(elm.x, elm.y);
		switch (elm.type)
		{
			case QPainterPath::MoveToElement:
				if (!first)
				{
					ret.append(part);
					part = QPainterPath();
				}
				first = false;
				part.moveTo(elm.x, elm.y);
				break;
			case QPainterPath::LineToElement:
				part.lineTo(elm.x, elm.y);
				break;
			case QPainterPath::CurveToElement:
				element1 = path.elementAt(i + 1);
				element2 = path.elementAt(i + 2);
				part.cubicTo(elm.x, elm.y, element1.x, element1.y, element2.x, element2.y );
				break;
			default:
				break;
		}
	}
	if (!part.isEmpty())
		ret.append(part);
	return ret;
}

FPoint projectPointOnLine(FPoint p, QPointF lineStart, QPointF lineEnd)
{
	if (lineStart == lineEnd)
		return FPoint(lineStart.x(), lineStart.y());
	
	// move lineStart to Origin
	p -= FPoint(lineStart.x(), lineStart.y());
	lineEnd -= lineStart;
	// calc dot product
	double lineLengthSquare = lineEnd.x() * lineEnd.x() + lineEnd.y() * lineEnd.y();
	double partOfLine = p.x() * lineEnd.x() + p.y() * lineEnd.y() / lineLengthSquare;
	// return point on line
	return FPoint(lineStart.x() + partOfLine * lineEnd.x(), lineStart.y() + partOfLine * lineEnd.y());
}

bool regionContainsRect(const QRegion& shape, QRect rect)
{
	/*bool oldResult = QRegion(rect).subtracted(shape).isEmpty();*/

	// Code adapted from Qt RectInRegion (cf. qregion.cpp) to detect
	// if a specific rect is strictly contained in a specific region
	bool partIn(false);
	bool partOut(false);

	const QRect *prect = &rect;
	int rx = rect.left();
	int ry = rect.top();

	int rectCount = shape.rectCount();
	QRect boundingRect = shape.boundingRect();
	if (rectCount == 0 || !boundingRect.contains(rect))
		return false;

	/* can stop when both partOut and partIn are true, or we reach prect->y2 */
	const QRect* pbox = (rectCount == 1) ? &boundingRect : shape.cbegin();
	const QRect* pboxEnd = (rectCount == 1) ? (&boundingRect + 1) : shape.cend();
	for (; pbox < pboxEnd; ++pbox)
	{
		if (pbox->bottom() < ry)
			continue;

		if (pbox->top() > ry)
		{
			partOut = true;
			if (partIn || pbox->top() > prect->bottom())
				break;
			ry = pbox->top();
		}

		if (pbox->right() < rx)
			continue;            /* not far enough over yet */

		if (pbox->left() > rx)
		{
			partOut = true;      /* missed part of rectangle to left */
			if (partIn)
				break;
		}

		if (pbox->left() <= prect->right())
		{
			partIn = true;      /* definitely overlap */
			if (partOut)
				break;
		}

		if (pbox->right() >= prect->right())
		{
			ry = pbox->bottom() + 1;     /* finished with this band */
			if (ry > prect->bottom())
				break;
			rx = prect->left();  /* reset x out to left again */
		}
		else
		{
			/*
			 * Because boxes in a band are maximal width, if the first box
			 * to overlap the rectangle doesn't completely cover it in that
			 * band, the rectangle must be partially out, since some of it
			 * will be uncovered in that band. partIn will have been set true
			 * by now...
			 */
			break;
		}
	}
	/*bool newResult = partIn ? ((ry <= prect->bottom()) ? false : true) : false;
	if (oldResult != newResult)
		int test = 0;*/
	return partIn ? (ry > prect->bottom()) : false;
}

QPolygon flattenPath(const FPointArray& ina, QList<uint> &segments)
{
	QPolygon cli;
	QPolygon outa;

	segments.clear();
	if (ina.size() <= 3)
		return outa;

	int limit = ina.size() - 3;
	for (int i = 0; i < limit; i += 4)
	{
		if (ina.isMarker(i)) // && cli.size() > 0)
		{
			segments.append(outa.size());
			continue;
		}
		const FPoint& a1 = ina.point(i);
		const FPoint& a2 = ina.point(i + 1);
		const FPoint& a3 = ina.point(i + 3);
		const FPoint& a4 = ina.point(i + 2);
		QPainterPath bez;
		bez.moveTo(a1.x(), a1.y());
		bez.cubicTo(a2.x(), a2.y(), a3.x(), a3.y(), a4.x(), a4.y());
		cli = bez.toFillPolygon().toPolygon();
		if (cli.size() > 1)
			outa.putPoints(outa.size(), cli.size() - 2, cli);
		else
			outa << QPoint(qRound(a4.x()), qRound(a4.y()));
	}

	return outa;
}

FPoint getMaxClipF(const FPointArray* clip)
{
	FPoint np;
	FPoint rp;
	double mx = 0;
	double my = 0;
	int clipSize = clip->size();
	for (int i = 0; i < clipSize; ++i)
	{
		np = clip->point(i);
		if (clip->isMarker(i))
			continue;
		if (np.x() > mx)
			mx = np.x();
		if (np.y() > my)
			my = np.y();
	}
	rp.setXY(mx, my);
	return rp;
}

FPoint getMinClipF(const FPointArray* clip)
{
	FPoint np;
	FPoint rp;
	double mx = std::numeric_limits<double>::max();
	double my = std::numeric_limits<double>::max();
	int clipSize = clip->size();
	for (int i = 0; i < clipSize; ++i)
	{
		np = clip->point(i);
		if (clip->isMarker(i))
			continue;
		if (np.x() < mx)
			mx = np.x();
		if (np.y() < my)
			my = np.y();
	}
	rp.setXY(mx, my);
	return rp;
}


bool compareDouble(double a, double b)
{
	if(a > -21473 && b > -21473 && a < 21474 && b < 21474)
	{
		long al = static_cast<long>(10000 * a);
		long bl = static_cast<long>(10000 * b);
		return al == bl;
	}
	return a == b;
}


double constrainAngle(double angle, double constrain)
{
	double newAngle = angle;
	double constrainTo = constrain;
	if (newAngle < 0.0)
		newAngle += 360.0;
	newAngle = qRound(newAngle / constrainTo) * constrainTo;
	if (newAngle == 360.0)
		newAngle = 0.0;
	return newAngle;
}

double getRotationFromMatrix(const QTransform& matrix, double def)
{
	double value = def;
	double norm = sqrt(fabs(matrix.determinant()));
	if (norm > 0.0000001)
	{
		double m11 = matrix.m11() / norm;
		double m12 = matrix.m12() / norm;
		double m21 = matrix.m21() / norm;
		double m22 = matrix.m22() / norm;
		if (fabs(m11) <= 1.0 && fabs(m12) <= 1.0 && fabs(m21) <= 1.0 && fabs(m22) <= 1.0)
		{
			QTransform mat(m11, m12, m21, m22, 0, 0);
			if (abs(mat.determinant()-1.0) < 0.00001 && (mat.m12() == -mat.m21()))
			{
				double ac = acos(mat.m11());
				value = (mat.m21() >= 0.0) ? ac : (-ac);
			}
		}
	}
	return value;
}

double getRotationDFromMatrix(const QTransform& matrix)
{
	QLineF line(0.0, 0.0, 1.0, 0.0);
	line = matrix.map(line);
	return line.angle();
}

void getScaleFromMatrix(const QTransform &matrix, double &scX, double &scY)
{
	QLineF lineX(0.0, 0.0, 1.0, 0.0);
	QLineF lineY(0.0, 0.0, 0.0, 1.0);
	lineX = matrix.map(lineX);
	lineY = matrix.map(lineY);
	scX = lineX.length();
	scY = lineY.length();
}

void getTransformValuesFromMatrix(const QTransform &matrix, double &scX, double &scY, double &rot, double &dx, double &dy)
{
	QLineF lineX(0.0, 0.0, 1.0, 0.0);
	QLineF lineY(0.0, 0.0, 0.0, 1.0);
	lineX = matrix.map(lineX);
	lineY = matrix.map(lineY);
	scX = lineX.length();
	scY = lineY.length();
	rot = lineX.angle();
	dx = lineX.x1();
	dy = lineX.y1();
}
