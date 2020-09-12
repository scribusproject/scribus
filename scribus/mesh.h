/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          mesh.h  -  description
                             -------------------
    begin                : Mit Apr 21 2010
    copyright            : (C) 2010 by Franz Schmid
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

#ifndef MESH_H
#define MESH_H

#include <QString>
#include <QColor>
#include <QTransform>
#include "fpoint.h"
#include "scribusapi.h"

class SCRIBUS_API MeshPoint
{
	public:
		MeshPoint() {};
		void moveRel(double dx, double dy);
		void moveAbs(double x, double y);
		void transform(const QTransform& t);
		void resetTo(const FPoint& p);
		bool operator ==(const MeshPoint& p);
		FPoint gridPoint;
		FPoint controlTop;
		FPoint controlBottom;
		FPoint controlLeft;
		FPoint controlRight;
		FPoint controlColor;
		double transparency {1.0};
		int shade {100};
		QString colorName;
		QColor color;
};

struct meshGradientPatch
{
	MeshPoint TL;
	MeshPoint TR;
	MeshPoint BL;
	MeshPoint BR;
};

#endif
