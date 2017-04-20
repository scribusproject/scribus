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
#if QT_VERSION >= 0x050300
#include <QMarginsF>
#endif
/** \brief Pagemargins and bleeds*/
class MarginStruct
{
	public:
		MarginStruct() : m_top(0), m_left(0), m_bottom(0), m_right(0) {}
		MarginStruct(double top, double left, double bottom, double right) :
			m_top(top), m_left(left), m_bottom(bottom), m_right(right) {}
//unneeded? default compiler generated should suffice
//		MarginStruct(const MarginStruct& rhs) {m_top=rhs.m_top;m_bottom=rhs.m_bottom;m_left=rhs.m_left;m_right=rhs.m_right;}
//		MarginStruct& operator=(const MarginStruct& rhs) {m_top=rhs.m_top;m_bottom=rhs.m_bottom;m_left=rhs.m_left;m_right=rhs.m_right;return *this;}
		void set(double top, double left, double bottom, double right) {m_top=top;m_bottom=bottom;m_left=left;m_right=right;}
		void resetToZero() {m_top=0.0;m_bottom=0.0;m_left=0.0;m_right=0.0;}
		bool isNull() const {return qFuzzyIsNull(m_left) && qFuzzyIsNull(m_top) && qFuzzyIsNull(m_right) && qFuzzyIsNull(m_bottom);}
		//bool hasNonZeroValue() const { return m_top!=0.0 || m_bottom!=0.0 || m_left!=0.0 || m_right!=0.0;}
		void print() const {qDebug() << m_top << m_left << m_bottom << m_right;}
		inline double left() const { return m_left; }
		inline double top() const { return m_top; }
		inline double bottom() const { return m_bottom; }
		inline double right() const { return m_right; }

		inline void setLeft(double aleft) { m_left = aleft; }
		inline void setTop(double atop) { m_top = atop; }
		inline void setRight(double aright) { m_right = aright; }
		inline void setBottom(double abottom) { m_bottom = abottom; }


	protected:
		double m_top;
		double m_left;
		double m_bottom;
		double m_right;
};

#if QT_VERSION >= 0x050300
class ScMargins : public QMarginsF
{
	public:
		ScMargins(qreal left, qreal top, qreal right, qreal bottom) : QMarginsF(left, top, right, bottom) {};
		void set(qreal left, qreal top, qreal right, qreal bottom)
		{
			setTop(top);
			setBottom(bottom);
			setLeft(left);
			setRight(right);
		}
};
#endif

#endif // MARGINS_H
