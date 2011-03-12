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

#include "util_math.h"
#include "scconfig.h"
#include "sclimits.h"
#include "fpoint.h"
#include "fpointarray.h"

using namespace std;



uint getDouble(QString in, bool raw)
{
	QByteArray bb(4, ' ');
	if (raw)
	{
		// Qt4
/*		bb[3] = static_cast<uchar>(QChar(in.at(0)));
		bb[2] = static_cast<uchar>(QChar(in.at(1)));
		bb[1] = static_cast<uchar>(QChar(in.at(2)));
		bb[0] = static_cast<uchar>(QChar(in.at(3)));*/
		bb = bb.insert(3, in.at(0));
		bb = bb.insert(2, in.at(1));
		bb = bb.insert(1, in.at(2));
		bb = bb.insert(0, in.at(3));
	}
	else
	{
		// Qt4
// 		bb[0] = static_cast<uchar>(QChar(in.at(0)));
// 		bb[1] = static_cast<uchar>(QChar(in.at(1)));
// 		bb[2] = static_cast<uchar>(QChar(in.at(2)));
// 		bb[3] = static_cast<uchar>(QChar(in.at(3)));
		bb = bb.insert(0, in.at(0));
		bb = bb.insert(1, in.at(1));
		bb = bb.insert(2, in.at(2));
		bb = bb.insert(3, in.at(3));
	}
	uint ret;
	ret = bb[0] & 0xff;
	ret |= (bb[1] << 8) & 0xff00;
	ret |= (bb[2] << 16) & 0xff0000;
	ret |= (bb[3] << 24) & 0xff000000;
	return ret;
}

QPainterPath RegularPolygonPath(double w, double h, uint c, bool star, double factor, double rota, double factor2, double innerRot, double factor3)
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
			mx = sin(sc / 180.0 * M_PI) * (wf) + (w/2.0);
			my = cos(sc / 180.0 * M_PI) * (hf) + (h/2.0);
		}
		else
		{
			mx = sin(sc / 180.0 * M_PI) * (w/2.0) + (w/2.0);
			my = cos(sc / 180.0 * M_PI) * (h/2.0) + (h/2.0);
		}
		cornerPoints.append(QPointF(mx, my));
	}
	// now calculate bezier control points if needed;
	if (star)
	{
		pts.moveTo(cornerPoints[0]);
		double mxc1, myc1, mxc2, myc2;
		for (int a = 0; a < cornerPoints.count() - 2; a++)
		{
			sc = seg * a + 180.0 + rota;
			if (a % 2 != 0)
			{
				// outer control point
				QLineF oline = QLineF(cornerPoints[a+1], cornerPoints[a-1]);
				mxc1 = oline.pointAt(factor3).x();
				myc1 = oline.pointAt(factor3).y();
				// inner control point
				mxc2 = sin((sc + 90.0) / 180.0 * M_PI) * (trueLength * factor2) + cornerPoints[a].x();
				myc2 = cos((sc + 90.0) / 180.0 * M_PI) * (trueLength * factor2) + cornerPoints[a].y();
				pts.cubicTo(QPointF(mxc2, myc2), QPointF(mxc1, myc1), cornerPoints[a+1]);
			}
			else
			{
				// outer control point
				QLineF oline = QLineF(cornerPoints[a], cornerPoints[a+2]);
				mxc1 = oline.pointAt(factor3).x();
				myc1 = oline.pointAt(factor3).y();
				// inner control point
				mxc2 = sin((sc - 90.0 + seg) / 180.0 * M_PI) * (trueLength * factor2) + cornerPoints[a+1].x();
				myc2 = cos((sc - 90.0 + seg) / 180.0 * M_PI) * (trueLength * factor2) + cornerPoints[a+1].y();
				pts.cubicTo(QPointF(mxc1, myc1), QPointF(mxc2, myc2), cornerPoints[a+1]);
			}
		}
		// outer control point
		QLineF oline = QLineF(cornerPoints[cornerPoints.count()-2], cornerPoints[0]);
		mxc1 = oline.pointAt(factor3).x();
		myc1 = oline.pointAt(factor3).y();
		// inner control point
		sc = seg * (cornerPoints.count()-1) + 180.0 + rota;
		mxc2 = sin((sc - 90.0) / 180.0 * M_PI) * (trueLength * factor2) + cornerPoints[cornerPoints.count()-1].x();
		myc2 = cos((sc - 90.0) / 180.0 * M_PI) * (trueLength * factor2) + cornerPoints[cornerPoints.count()-1].y();
		pts.cubicTo(QPointF(mxc1, myc1), QPointF(mxc2, myc2), cornerPoints[cornerPoints.count()-1]);
		oline = QLineF(cornerPoints[0], cornerPoints[cornerPoints.count()-2]);
		mxc1 = oline.pointAt(factor3).x();
		myc1 = oline.pointAt(factor3).y();
		// inner control point
		mxc2 = sin((sc + 90.0) / 180.0 * M_PI) * (trueLength * factor2) + cornerPoints[cornerPoints.count()-1].x();
		myc2 = cos((sc + 90.0) / 180.0 * M_PI) * (trueLength * factor2) + cornerPoints[cornerPoints.count()-1].y();
		pts.cubicTo(QPointF(mxc2, myc2), QPointF(mxc1, myc1), cornerPoints[0]);
		pts.closeSubpath();
	}
	else
	{
		pts.moveTo(cornerPoints[0]);
		for (int a = 1; a < cornerPoints.count(); a++)
		{
			pts.lineTo(cornerPoints[a]);
		}
		pts.closeSubpath();
	}
	return pts;
}

QPainterPath SpiralPath(double spiralWidth, double spiralHeight, double spiralStartAngle, double spiralEndAngle, double spiralFactor)
{
	if (spiralStartAngle >= spiralEndAngle)
		return QPainterPath();
	double startAngleK = spiralStartAngle;
	double endAngleK = spiralEndAngle;
	QPainterPath path, path2;
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

QList<QPainterPath> decomposePath(QPainterPath &path)
{
	QList<QPainterPath> ret;
	ret.clear();
	QPainterPath part;
	part = QPainterPath();
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
				part.cubicTo(elm.x, elm.y, path.elementAt(i+1).x, path.elementAt(i+1).y, path.elementAt(i+2).x, path.elementAt(i+2).y );
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


QPolygon FlattenPath(const FPointArray& ina, QList<uint> &Segs)
{
	QPolygon cli, outa;
	Segs.clear();
	if (ina.size() > 3)
	{
		for (uint poi=0; poi<ina.size()-3; poi += 4)
		{
			if (ina.point(poi).x() > 900000) // && cli.size() > 0)
			{
//				outa << cli.point(cli.size()-1);
				Segs.append(outa.size());
				continue;
			}
			FPoint a1 = ina.point(poi);
			FPoint a2 = ina.point(poi+1);
			FPoint a3 = ina.point(poi+3);
			FPoint a4 = ina.point(poi+2);
			QPainterPath Bez;
			Bez.moveTo(a1.x(), a1.y());
			Bez.cubicTo(a2.x(), a2.y(), a3.x(), a3.y(), a4.x(), a4.y());
			cli = Bez.toFillPolygon().toPolygon();
			if (cli.size() > 1)
				outa.putPoints(outa.size(), cli.size()-2, cli);
			else
				outa << QPoint(qRound(a4.x()), qRound(a4.y()));
		}
//		if (cli.size() > 0)
//			outa << cli.point(cli.size()-1);
	}
	return outa;
}

FPoint getMaxClipF(FPointArray* Clip)
{
	FPoint np, rp;
	double mx = 0;
	double my = 0;
	uint clipSize=Clip->size();
	for (uint c = 0; c < clipSize; ++c)
	{
		np = Clip->point(c);
		if (np.x() > 900000)
			continue;
		if (np.x() > mx)
			mx = np.x();
		if (np.y() > my)
			my = np.y();
	}
	rp.setXY(mx, my);
	return rp;
}

FPoint getMinClipF(FPointArray* Clip)
{
	FPoint np, rp;
	double mx =  std::numeric_limits<double>::max();
	double my =  std::numeric_limits<double>::max();
	uint clipSize=Clip->size();
	for (uint c = 0; c < clipSize; ++c)
	{
		np = Clip->point(c);
		if (np.x() > 900000)
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
	double newAngle=angle;
	double constrainTo=constrain;
	if (newAngle<0.0)
		newAngle+=360.0;
	newAngle=qRound(angle/constrainTo)*constrainTo;
	if (newAngle==360.0)
		newAngle=0.0;
	return newAngle;
}

double getRotationFromMatrix(QTransform& matrix, double def)
{
	double value = def;
	double norm = sqrt(fabs(matrix.det()));
	if (norm > 0.0000001)
	{
		double m11 = matrix.m11() / norm;
		double m12 = matrix.m12() / norm;
		double m21 = matrix.m21() / norm;
		double m22 = matrix.m22() / norm;
		if (fabs(m11) <= 1.0 && fabs(m12) <= 1.0 && fabs(m21) <= 1.0 && fabs(m22) <= 1.0)
		{
			QTransform mat(m11, m12, m21, m22, 0, 0);
			if (abs(mat.det()-1.0) < 0.00001 && (mat.m12() == -mat.m21()))
			{
				double ac = acos(mat.m11());
				value = (mat.m21() >= 0.0) ? ac : (-ac);
			}
		}
	}
	return value;
}

