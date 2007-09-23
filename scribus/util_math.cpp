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
#include <QBitmap>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QCheckBox>
#include <QWidget>
#include <QPixmap>
#include <QList>
#include <QByteArray>
#include <QPolygon>
#include <QImageReader>

#include <cmath>
#include <algorithm>
#include "scconfig.h"

#include "commonstrings.h"
#include <QProcess>
#include "scmessagebox.h"
#include "scpixmapcache.h"
#include "scpaths.h"

extern "C"
{
#define XMD_H           // shut JPEGlib up
#if defined(Q_OS_UNIXWARE)
#  define HAVE_BOOLEAN  // libjpeg under Unixware seems to need this
#endif
#include <jpeglib.h>
#include <jerror.h>
#undef HAVE_STDLIB_H
#ifdef const
#  undef const          // remove crazy C hackery in jconfig.h
#endif
}

#include "pageitem.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
/*#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_GLYPH_H
*/
#include <zlib.h>


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

QPolygon RegularPolygon(double w, double h, uint c, bool star, double factor, double rota)
{
	uint cx = star ? c * 2 : c;
	double seg = 360.0 / cx;
	double sc = rota + 180.0;
	double di = factor;
	int mx = 0;
	int my = 0;
	QPolygon pts(cx);
	for (uint x = 0; x < cx; ++x)
	{
		sc = seg * x + 180.0 + rota;
		if (star)
		{
			double wf = x % 2 == 0 ? w / 2.0 : w / 2.0 * di;
			double hf = x % 2 == 0 ? h / 2.0 : h / 2.0 * di;
			mx = qRound(sin(sc / 180.0 * M_PI) * (wf) + (w/2.0));
			my = qRound(cos(sc / 180.0 * M_PI) * (hf) + (h/2.0));
		}
		else
		{
			mx = qRound(sin(sc / 180.0 * M_PI) * (w/2.0) + (w/2.0));
			my = qRound(cos(sc / 180.0 * M_PI) * (h/2.0) + (h/2.0));
		}
		pts.setPoint(x, mx, my);
	}
	return pts;
}

FPointArray RegularPolygonF(double w, double h, uint c, bool star, double factor, double rota)
{
	uint cx = star ? c * 2 : c;
	double seg = 360.0 / cx;
	double sc = rota + 180.0;
	double di = factor;
	double mx = 0;
	double my = 0;
	FPointArray pts(cx);
	for (uint x = 0; x < cx; ++x)
	{
		sc = seg * x + 180.0 + rota;
		if (star)
		{
			double wf = x % 2 == 0 ? w / 2.0 : w / 2.0 * di;
			double hf = x % 2 == 0 ? h / 2.0 : h / 2.0 * di;
			mx = sin(sc / 180.0 * M_PI) * (wf) + (w/2.0);
			my = cos(sc / 180.0 * M_PI) * (hf) + (h/2.0);
		}
		else
		{
			mx = sin(sc / 180.0 * M_PI) * (w/2.0) + (w/2.0);
			my = cos(sc / 180.0 * M_PI) * (h/2.0) + (h/2.0);
		}
		pts.setPoint(x, mx, my);
	}
	return pts;
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

QPolygon FlattenPath(FPointArray ina, QList<uint> &Segs)
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

double xy2Deg(double x, double y)
{
	return (atan2(y,x)*(180.0/M_PI));
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
	double mx = 99999;
	double my = 99999;
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

inline double square(double x)
{
	return x*x;
}

inline double distance(double x, double y)
{
	return sqrt(x*x+y*y);
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

double getRotationFromMatrix(QMatrix& matrix, double def)
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
			QMatrix mat(m11, m12, m21, m22, 0, 0);
			if (abs(mat.det()-1.0) < 0.00001 && (mat.m12() == -mat.m21()))
			{
				double ac = acos(mat.m11());
				value = (mat.m21() >= 0.0) ? ac : (-ac);
			}
		}
	}
	return value;
}

