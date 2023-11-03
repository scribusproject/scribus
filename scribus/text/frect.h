/****************************************************************************
** $Id$
**
** Definition of FRect class
**
** Created : 931028
**
** Copyright (C) 1992-2000 Trolltech AS. All rights reserved.
**
** This file is part of the kernel module of the Qt GUI Toolkit.
**
** This file may be distributed under the terms of the Q Public License
** as defined by Trolltech AS of Norway and appearing in the file
** LICENSE.QPL included in the packaging of this file.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid Qt Enterprise Edition or Qt Professional Edition
** licenses may use this file in accordance with the Qt Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.trolltech.com/pricing.html or email sales@trolltech.com for
** information about Qt Commercial License Agreements.
** See http://www.trolltech.com/qpl/ for QPL licensing information.
** See http://www.trolltech.com/gpl/ for GPL licensing information.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#ifndef FRECT_H
#define FRECT_H

#include "fsize.h"

#if defined(topLeft)
#error "Macro definition of topLeft conflicts with FRect"
// don't just silently undo people's defines: #undef topLeft
#endif

class FRect // rectangle class
{
	public:
		FRect() {}
		FRect(FPoint &topleft, FPoint &bottomright);
		FRect(FPoint &topleft, FSize &size);
		FRect(qreal left, qreal top, qreal width, qreal height);

		bool isNull() const;
		bool isEmpty() const;
		bool isValid() const;
		FRect normalize() const;

		qreal left() const;
		qreal top() const;
		qreal right() const;
		qreal bottom() const;

		qreal &rLeft();
		qreal &rTop();
		qreal &rRight();
		qreal &rBottom();

		qreal x() const;
		qreal y() const;
		void setLeft(qreal pos);
		void setTop(qreal pos);
		void setRight(qreal pos);
		void setBottom(qreal pos);
		void setX(qreal x);
		void setY(qreal y);

		void setTopLeft(const FPoint &p);
		void setBottomRight(const FPoint &p);
		void setTopRight(const FPoint &p);
		void setBottomLeft(const FPoint &p);

		FPoint topLeft() const;
		FPoint bottomRight() const;
		FPoint topRight() const;
		FPoint bottomLeft() const;
		FPoint center() const;

		void rect(qreal *x, qreal *y, qreal *w, qreal *h) const;
		void coords(qreal *xp1, qreal *yp1, qreal *xp2, qreal *yp2) const;

		void moveLeft(qreal pos);
		void moveTop(qreal pos);
		void moveRight(qreal pos);
		void moveBottom(qreal pos);
		void moveTopLeft(const FPoint &p);
		void moveBottomRight(const FPoint &p);
		void moveTopRight(const FPoint &p);
		void moveBottomLeft(const FPoint &p);
		void moveCenter(const FPoint &p);
		void moveBy(qreal dx, qreal dy);

		void setRect(qreal x, qreal y, qreal w, qreal h);
		void setCoords(qreal xp1, qreal yp1, qreal xp2, qreal yp);
		void addCoords(qreal xp1, qreal yp1, qreal xp2, qreal yp2);

		FSize size() const;
		qreal width() const;
		qreal height() const;
		void setWidth(qreal w);
		void setHeight(qreal h);
		void setSize(const FSize &s);

		FRect operator|(const FRect &r) const;
		FRect operator&(const FRect &r) const;
		FRect& operator|=(const FRect &r);
		FRect& operator&=(const FRect &r);

		bool contains(FPoint& p, bool proper = false) const;
		bool contains(qreal x, qreal y) const; // inline methods, _don't_ merge these
		bool contains(qreal x, qreal y, bool proper) const;
		bool contains(const FRect& r, bool proper = false) const;
		FRect unite(const FRect& r) const;
		FRect intersect(const FRect& r) const;
		bool intersects(const FRect& r) const;

		friend bool operator==(const FRect &, const FRect &);
		friend bool operator!=(const FRect &, const FRect &);

	private:
	#if defined(Q_OS_LINUX) || defined(Q_OS_TEMP)
		friend void qt_setCoords(FRect *r, qreal xp1, qreal yp1, qreal xp2, qreal yp2);
	#endif
		qreal x1 {0.0};
		qreal y1 {0.0};
		qreal x2 {-1.0};
		qreal y2 {-1.0};
};

bool operator==(const FRect &, const FRect &);
bool operator!=(const FRect &, const FRect &);


/*****************************************************************************
 FRect stream functions
 *****************************************************************************/
// #ifndef QT_NO_DATASTREAM
// Q_EXPORT QDataStream &operator<<(QDataStream &, const FRect &);
// Q_EXPORT QDataStream &operator>>(QDataStream &, FRect &);
// #endif

/*****************************************************************************
 FRect inline member functions
 *****************************************************************************/

inline FRect::FRect(qreal left, qreal top, qreal width, qreal height)
{
	x1 = left;
	y1 = top;
	x2 = (left + width - 1);
	y2 = (top + height -1);
}

inline bool FRect::isNull() const
{ return x2 == x1 - 1 && y2 == y1 - 1; }

inline bool FRect::isEmpty() const
{ return x1 > x2 || y1 > y2; }

inline bool FRect::isValid() const
{ return x1 <= x2 && y1 <= y2; }

inline qreal FRect::left() const
{ return x1; }

inline qreal FRect::top() const
{ return y1; }

inline qreal FRect::right() const
{ return x2; }

inline qreal FRect::bottom() const
{ return y2; }

inline qreal &FRect::rLeft()
{ return x1; }

inline qreal & FRect::rTop()
{ return y1; }

inline qreal & FRect::rRight()
{ return x2; }

inline qreal & FRect::rBottom()
{ return y2; }

inline qreal FRect::x() const
{ return x1; }

inline qreal FRect::y() const
{ return y1; }

inline void FRect::setLeft(qreal pos)
{ x1 = pos; }

inline void FRect::setTop(qreal pos)
{ y1 = pos; }

inline void FRect::setRight(qreal pos)
{ x2 = pos; }

inline void FRect::setBottom(qreal pos)
{ y2 = pos; }

inline void FRect::setX(qreal x)
{ x1 = x; }

inline void FRect::setY(qreal y)
{ y1 = (qreal)y; }

inline FPoint FRect::topLeft() const
{ return FPoint(x1, y1); }

inline FPoint FRect::bottomRight() const
{ return FPoint(x2, y2); }

inline FPoint FRect::topRight() const
{ return FPoint(x2, y1); }

inline FPoint FRect::bottomLeft() const
{ return FPoint(x1, y2); }

inline FPoint FRect::center() const
{ return FPoint((x1+x2)/2, (y1+y2)/2); }

inline qreal FRect::width() const
{ return x2 - x1 + 1; }

inline qreal FRect::height() const
{ return y2 - y1 + 1; }

inline FSize FRect::size() const
{ return FSize(x2-x1+1, y2-y1+1); }

inline bool FRect::contains(qreal x, qreal y, bool proper) const
{
	if (proper)
		return x > x1 && x < x2 &&
			 y > y1 && y < y2;
	else
		return x >= x1 && x <= x2 &&
			 y >= y1 && y <= y2;
}

inline bool FRect::contains(qreal x, qreal y) const
{
	return x >= x1 && x <= x2 &&
	 y >= y1 && y <= y2;
}

 #endif // FRECT_H
