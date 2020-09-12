/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          mesh.cpp  -  description
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

#include "mesh.h"
#include "fpointarray.h"

void MeshPoint::moveRel(double dx, double dy)
{
	gridPoint += FPoint(dx, dy);
	controlTop += FPoint(dx, dy);
	controlBottom += FPoint(dx, dy);
	controlLeft += FPoint(dx, dy);
	controlRight += FPoint(dx, dy);
	controlColor += FPoint(dx, dy);
}

void MeshPoint::moveAbs(double x, double y)
{
	FPoint delta = gridPoint - FPoint(x, y);
	gridPoint = FPoint(x, y);
	controlTop -= delta;
	controlBottom -= delta;
	controlLeft -= delta;
	controlRight -= delta;
	controlColor -= delta;
}

void MeshPoint::transform(const QTransform& t)
{
	FPointArray gr;
	gr.addPoint(gridPoint);
	gr.addPoint(controlTop);
	gr.addPoint(controlBottom);
	gr.addPoint(controlLeft);
	gr.addPoint(controlRight);
	gr.addPoint(controlColor);
	gr.map(t);
	gridPoint = gr.point(0);
	controlTop = gr.point(1);
	controlBottom = gr.point(2);
	controlLeft = gr.point(3);
	controlRight = gr.point(4);
	controlColor = gr.point(5);
}

void MeshPoint::resetTo(const FPoint& p)
{
	gridPoint = p;
	controlLeft = gridPoint;
	controlRight = gridPoint;
	controlTop = gridPoint;
	controlBottom = gridPoint;
	controlColor = gridPoint;
}

bool MeshPoint::operator ==(const MeshPoint& p)
{
	return ((gridPoint == p.gridPoint) &&
			(controlTop == p.controlTop) &&
			(controlBottom == p.controlBottom) &&
			(controlLeft == p.controlLeft) &&
			(controlRight == p.controlRight) &&
			(controlColor == p.controlColor) &&
			(color == p.color) &&
			(colorName == p.colorName) &&
			(transparency == p.transparency) &&
			(shade == p.shade));
}

