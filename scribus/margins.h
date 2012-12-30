/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef MARGINS_H
#define MARGINS_H

#include <QDebug>

/** \brief Pagemargins and bleeds*/
class MarginStruct
{
	public:
		MarginStruct() : Top(0), Left(0), Bottom(0), Right(0) {}
		MarginStruct(double top, double left, double bottom, double right) :
			Top(top), Left(left), Bottom(bottom), Right(right) {}
		MarginStruct(const MarginStruct& rhs) {Top=rhs.Top;Bottom=rhs.Bottom;Left=rhs.Left;Right=rhs.Right;}
		void set(double top, double left, double bottom, double right) {Top=top;Bottom=bottom;Left=left;Right=right;}
		void resetToZero() {Top=0.0;Bottom=0.0;Left=0.0;Right=0.0;}
		bool hasNonZeroValue() const { return Top!=0.0 || Bottom!=0.0 || Left!=0.0 || Right!=0.0;}
		void print() const {qDebug() << Top << Left << Bottom << Right;}
		double leftMargin() const { return Left; }
		double topMargin() const { return Top; }
		double bottomMargin() const { return Bottom; }
		double rightMargin() const { return Right; }
		double Top;
		double Left;
		double Bottom;
		double Right;
};

#endif // MARGINS_H
