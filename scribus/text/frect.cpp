/****************************************************************************
** $Id$
**
** Implementation of FRect class
**
** Created : 931028
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
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
**   information about Qt Commercial License Agreements.
** See http://www.trolltech.com/qpl/ for QPL licensing information.
** See http://www.trolltech.com/gpl/ for GPL licensing information.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#define	 QRECT_C
#include "frect.h"
//#include "qdatastream.h"

/*!
    \class FRect
    \brief The FRect class defines a rectangle in the plane.

    \ingroup images
    \ingroup graphics
    \mainclass

    A rectangle is internally represented as an upper-left corner and
    a bottom-right corner, but it is normally expressed as an
    upper-left corner and a size.

    The coordinate type is qreal (defined in \c qwindowdefs.h as \c
    int). The minimum value of qreal is qreal_MIN (-2147483648) and
    the maximum value is  qreal_MAX (2147483647).

    Note that the size (width and height) of a rectangle might be
    different from what you are used to. If the top-left corner and
    the bottom-right corner are the same, the height and the width of
    the rectangle will both be 1.

    Generally, \e{width = right - left + 1} and \e{height = bottom -
    top + 1}. We designed it this way to make it correspond to
    rectangular spaces used by drawing functions in which the width
    and height denote a number of pixels. For example, drawing a
    rectangle with width and height 1 draws a single pixel.

    The default coordinate system has origin (0, 0) in the top-left
    corner. The positive direction of the y axis is down, and the
    positive x axis is from left to right.

    A FRect can be constructed with a set of left, top, width and
    height integers, from two FPoints or from a FPoint and a FSize.
    After creation the dimensions can be changed, e.g. with setLeft(),
    setRight(), setTop() and setBottom(), or by setting sizes, e.g.
    setWidth(), setHeight() and setSize(). The dimensions can also be
    changed with the move functions, e.g. moveBy(), moveCenter(),
    moveBottomRight(), etc. You can also add coordinates to a
    rectangle with addCoords().

    You can test to see if a FRect contains a specific point with
    contains(). You can also test to see if two FRects intersect with
    intersects() (see also intersect()). To get the bounding rectangle
    of two FRects use unite().

    \sa FPoint, FSize
*/


/*****************************************************************************
  FRect member functions
 *****************************************************************************/

/*!
    \fn FRect::FRect()

    Constructs an invalid rectangle.
*/

/*!
    Constructs a rectangle with \a topLeft as the top-left corner and
    \a bottomRight as the bottom-right corner.
*/

FRect::FRect( FPoint &topLeft, FPoint &bottomRight )
{
    x1 = (qreal)topLeft.x();
    y1 = (qreal)topLeft.y();
    x2 = (qreal)bottomRight.x();
    y2 = (qreal)bottomRight.y();
}

/*!
    Constructs a rectangle with \a topLeft as the top-left corner and
    \a size as the rectangle size.
*/

FRect::FRect( FPoint &topLeft, FSize &size )
{
    x1 = (qreal)topLeft.x();
    y1 = (qreal)topLeft.y();
    x2 = (qreal)(x1+size.width());
    y2 = (qreal)(y1+size.height());
}

/*!
    \fn FRect::FRect( qreal left, qreal top, qreal width, qreal height )

    Constructs a rectangle with the \a top, \a left corner and \a
    width and \a height.

    Example (creates three identical rectangles):
    \code
	FRect r1( FPoint(100,200), FPoint(110,215) );
	FRect r2( FPoint(100,200), FSize(11,16) );
	FRect r3( 100, 200, 11, 16 );
    \endcode
*/


/*!
    \fn bool FRect::isNull() const

    Returns TRUE if the rectangle is a null rectangle; otherwise
    returns FALSE.

    A null rectangle has both the width and the height set to 0, that
    is right() == left() - 1 and bottom() == top() - 1.

    Note that if right() == left() and bottom() == top(), then the
    rectangle has width 1 and height 1.

    A null rectangle is also empty.

    A null rectangle is not valid.

    \sa isEmpty(), isValid()
*/

/*!
    \fn bool FRect::isEmpty() const

    Returns TRUE if the rectangle is empty; otherwise returns FALSE.

    An empty rectangle has a left() \> right() or top() \> bottom().

    An empty rectangle is not valid. \c{isEmpty() == !isValid()}

    \sa isNull(), isValid(), normalize()
*/

/*!
    \fn bool FRect::isValid() const

    Returns TRUE if the rectangle is valid; otherwise returns FALSE.

    A valid rectangle has a left() \<= right() and top() \<= bottom().

    Note that non-trivial operations like intersections are not defined
    for invalid rectangles.

    \c{isValid() == !isEmpty()}

    \sa isNull(), isEmpty(), normalize()
*/


/*!
    Returns a normalized rectangle, i.e. a rectangle that has a
    non-negative width and height.

    It swaps left and right if left() \> right(), and swaps top and
    bottom if top() \> bottom().

    \sa isValid()
*/

FRect FRect::normalize() const
{
    FRect r;
    if ( x2 < x1 ) {				// swap bad x values
	r.x1 = x2;
	r.x2 = x1;
    } else {
	r.x1 = x1;
	r.x2 = x2;
    }
    if ( y2 < y1 ) {				// swap bad y values
	r.y1 = y2;
	r.y2 = y1;
    } else {
	r.y1 = y1;
	r.y2 = y2;
    }
    return r;
}


/*!
    \fn qreal FRect::left() const

    Returns the left coordinate of the rectangle. Identical to x().

    \sa setLeft(), right(), topLeft(), bottomLeft()
*/

/*!
    \fn qreal FRect::top() const

    Returns the top coordinate of the rectangle. Identical to y().

    \sa setTop(), bottom(), topLeft(), topRight()
*/

/*!
    \fn qreal FRect::right() const

    Returns the right coordinate of the rectangle.

    \sa setRight(), left(), topRight(), bottomRight()
*/

/*!
    \fn qreal FRect::bottom() const

    Returns the bottom coordinate of the rectangle.

    \sa setBottom(), top(), bottomLeft(), bottomRight()
*/

/*!
    \fn qreal &FRect::rLeft()

    Returns a reference to the left coordinate of the rectangle.

    \sa rTop(), rRight(), rBottom()
*/

/*!
    \fn qreal &FRect::rTop()

    Returns a reference to the top coordinate of the rectangle.

    \sa rLeft(),  rRight(), rBottom()
*/

/*!
    \fn qreal &FRect::rRight()

    Returns a reference to the right coordinate of the rectangle.

    \sa rLeft(), rTop(), rBottom()
*/

/*!
    \fn qreal &FRect::rBottom()

    Returns a reference to the bottom coordinate of the rectangle.

    \sa rLeft(), rTop(), rRight()
*/

/*!
    \fn qreal FRect::x() const

    Returns the left coordinate of the rectangle. Identical to left().

    \sa left(), y(), setX()
*/

/*!
    \fn qreal FRect::y() const

    Returns the top coordinate of the rectangle. Identical to top().

    \sa top(), x(), setY()
*/

/*!
    \fn void FRect::setLeft( qreal pos )

    Sets the left edge of the rectangle to \a pos. May change the
    width, but will never change the right edge of the rectangle.

    Identical to setX().

    \sa left(), setTop(), setWidth()
*/

/*!
    \fn void FRect::setTop( qreal pos )

    Sets the top edge of the rectangle to \a pos. May change the
    height, but will never change the bottom edge of the rectangle.

    Identical to setY().

    \sa top(), setBottom(), setHeight()
*/

/*!
    \fn void FRect::setRight( qreal pos )

    Sets the right edge of the rectangle to \a pos. May change the
    width, but will never change the left edge of the rectangle.

    \sa right(), setLeft(), setWidth()
*/

/*!
    \fn void FRect::setBottom( qreal pos )

    Sets the bottom edge of the rectangle to \a pos. May change the
    height, but will never change the top edge of the rectangle.

    \sa bottom(), setTop(), setHeight()
*/

/*!
    \fn void FRect::setX( qreal x )

    Sets the x position of the rectangle (its left end) to \a x. May
    change the width, but will never change the right edge of the
    rectangle.

    Identical to setLeft().

    \sa x(), setY()
*/

/*!
    \fn void FRect::setY( qreal y )

    Sets the y position of the rectangle (its top) to \a y. May change
    the height, but will never change the bottom edge of the
    rectangle.

    Identical to setTop().

    \sa y(), setX()
*/

/*!
    Set the top-left corner of the rectangle to \a p. May change
    the size, but will the never change the bottom-right corner of
    the rectangle.

    \sa topLeft(), moveTopLeft(), setBottomRight(), setTopRight(), setBottomLeft()
*/
void FRect::setTopLeft( FPoint &p )
{
    setLeft( p.x() );
    setTop( p.y() );
}

/*!
    Set the bottom-right corner of the rectangle to \a p. May change
    the size, but will the never change the top-left corner of
    the rectangle.

    \sa bottomRight(), moveBottomRight(), setTopLeft(), setTopRight(), setBottomLeft()
*/
void FRect::setBottomRight( FPoint &p )
{
    setRight( p.x() );
    setBottom( p.y() );
}

/*!
    Set the top-right corner of the rectangle to \a p. May change
    the size, but will the never change the bottom-left corner of
    the rectangle.

    \sa topRight(), moveTopRight(), setTopLeft(), setBottomRight(), setBottomLeft()
*/
void FRect::setTopRight( FPoint &p )
{
    setRight( p.x() );
    setTop( p.y() );
}

/*!
    Set the bottom-left corner of the rectangle to \a p. May change
    the size, but will the never change the top-right corner of
    the rectangle.

    \sa bottomLeft(), moveBottomLeft(), setTopLeft(), setBottomRight(), setTopRight()
*/
void FRect::setBottomLeft( FPoint &p )
{
    setLeft( p.x() );
    setBottom( p.y() );
}

/*!
    \fn FPoint FRect::topLeft() const

    Returns the top-left position of the rectangle.

    \sa setTopLeft(), moveTopLeft(), bottomRight(), left(), top()
*/

/*!
    \fn FPoint FRect::bottomRight() const

    Returns the bottom-right position of the rectangle.

    \sa setBottomRight(), moveBottomRight(), topLeft(), right(), bottom()
*/

/*!
    \fn FPoint FRect::topRight() const

    Returns the top-right position of the rectangle.

    \sa setTopRight(), moveTopRight(), bottomLeft(), top(), right()
*/

/*!
    \fn FPoint FRect::bottomLeft() const

    Returns the bottom-left position of the rectangle.

    \sa setBottomLeft(), moveBottomLeft(), topRight(), bottom(), left()
*/

/*!
    \fn FPoint FRect::center() const

    Returns the center point of the rectangle.

    \sa moveCenter(), topLeft(), bottomRight(), topRight(), bottomLeft()
*/


/*!
    Extracts the rectangle parameters as the position \a *x, \a *y and
    width \a *w and height \a *h.

    \sa setRect(), coords()
*/

void FRect::rect( qreal *x, qreal *y, qreal *w, qreal *h ) const
{
    *x = x1;
    *y = y1;
    *w = x2-x1;
    *h = y2-y1;
}

/*!
    Extracts the rectangle parameters as the top-left point \a *xp1,
    \a *yp1 and the bottom-right point \a *xp2, \a *yp2.

    \sa setCoords(), rect()
*/

void FRect::coords( qreal *xp1, qreal *yp1, qreal *xp2, qreal *yp2 ) const
{
    *xp1 = x1;
    *yp1 = y1;
    *xp2 = x2;
    *yp2 = y2;
}


/*!
    Sets the left position of the rectangle to \a pos, leaving the
    size unchanged.

    \sa left(), setLeft(), moveTop(), moveRight(), moveBottom()
*/
void FRect::moveLeft( qreal pos )
{
    x2 += (qreal)(pos - x1);
    x1 = (qreal)pos;
}

/*!
    Sets the top position of the rectangle to \a pos, leaving the
    size unchanged.

    \sa top(), setTop(), moveLeft(), moveRight(), moveBottom()
*/

void FRect::moveTop( qreal pos )
{
    y2 += (qreal)(pos - y1);
    y1 = (qreal)pos;
}

/*!
    Sets the right position of the rectangle to \a pos, leaving the
    size unchanged.

    \sa right(), setRight(), moveLeft(), moveTop(), moveBottom()
*/

void FRect::moveRight( qreal pos )
{
    x1 += (qreal)(pos - x2);
    x2 = (qreal)pos;
}

/*!
    Sets the bottom position of the rectangle to \a pos, leaving the
    size unchanged.

    \sa bottom(), setBottom(), moveLeft(), moveTop(), moveRight()
*/

void FRect::moveBottom( qreal pos )
{
    y1 += (qreal)(pos - y2);
    y2 = (qreal)pos;
}

/*!
    Sets the top-left position of the rectangle to \a p, leaving the
    size unchanged.

    \sa topLeft(), setTopLeft(), moveBottomRight(), moveTopRight(), moveBottomLeft()
*/

void FRect::moveTopLeft( FPoint &p )
{
    moveLeft( p.x() );
    moveTop( p.y() );
}

/*!
    Sets the bottom-right position of the rectangle to \a p, leaving
    the size unchanged.

    \sa bottomRight(), setBottomRight(), moveTopLeft(), moveTopRight(), moveBottomLeft()
*/

void FRect::moveBottomRight( FPoint &p )
{
    moveRight( p.x() );
    moveBottom( p.y() );
}

/*!
    Sets the top-right position of the rectangle to \a p, leaving the
    size unchanged.

    \sa topRight(), setTopRight(), moveTopLeft(), moveBottomRight(), moveBottomLeft()
*/

void FRect::moveTopRight( FPoint &p )
{
    moveRight( p.x() );
    moveTop( p.y() );
}

/*!
    Sets the bottom-left position of the rectangle to \a p, leaving
    the size unchanged.

    \sa bottomLeft(), setBottomLeft(), moveTopLeft(), moveBottomRight(), moveTopRight()
*/

void FRect::moveBottomLeft( FPoint &p )
{
    moveLeft( p.x() );
    moveBottom( p.y() );
}


/*!
    Sets the center point of the rectangle to \a p, leaving the size
    unchanged.

    \sa center(), moveTopLeft(), moveBottomRight(), moveTopRight(), moveBottomLeft()
*/

void FRect::moveCenter( FPoint &p )
{
    qreal w = x2 - x1;
    qreal h = y2 - y1;
    x1 = (qreal)(p.x() - w/2);
    y1 = (qreal)(p.y() - h/2);
    x2 = x1 + w;
    y2 = y1 + h;
}


/*!
    Moves the rectangle \a dx along the x axis and \a dy along the y
    axis, relative to the current position. Positive values move the
    rectangle to the right and down.

    \sa moveTopLeft()
*/

void FRect::moveBy( qreal dx, qreal dy )
{
    x1 += (qreal)dx;
    y1 -= (qreal)dy;
    x2 += (qreal)dx;
    y2 -= (qreal)dy;
}

/*!
    Sets the coordinates of the rectangle's top-left corner to \a (x,
    y), and its size to \a (w, h).

    \sa rect(), setCoords()
*/

void FRect::setRect( qreal x, qreal y, qreal w, qreal h )
{
    x1 = (qreal)x;
    y1 = (qreal)y;
    x2 = (qreal)(x+w);
    y2 = (qreal)(y+h);
}

/*!
    Sets the coordinates of the rectangle's top-left corner to \a
    (xp1, yp1), and the coordinates of its bottom-right corner to \a
    (xp2, yp2).

    \sa coords(), setRect()
*/

void FRect::setCoords( qreal xp1, qreal yp1, qreal xp2, qreal yp2 )
{
    x1 = (qreal)xp1;
    y1 = (qreal)yp1;
    x2 = (qreal)xp2;
    y2 = (qreal)yp2;
}

/*!
    Adds \a xp1, \a yp1, \a xp2 and \a yp2 respectively to the
    existing coordinates of the rectangle.
*/

void FRect::addCoords( qreal xp1, qreal yp1, qreal xp2, qreal yp2 )
{
    x1 += (qreal)xp1;
    y1 += (qreal)yp1;
    x2 += (qreal)xp2;
    y2 += (qreal)yp2;
}

/*!
    \fn FSize FRect::size() const

    Returns the size of the rectangle.

    \sa width(), height()
*/

/*!
    \fn qreal FRect::width() const

    Returns the width of the rectangle. The width includes both the
    left and right edges, i.e. width = right - left + 1.

    \sa height(), size(), setHeight()
*/

/*!
    \fn qreal FRect::height() const

    Returns the height of the rectangle. The height includes both the
    top and bottom edges, i.e. height = bottom - top + 1.

    \sa width(), size(), setHeight()
*/

/*!
    Sets the width of the rectangle to \a w. The right edge is
    changed, but not the left edge.

    \sa width(), setLeft(), setRight(), setSize()
*/

void FRect::setWidth( qreal w )
{
    x2 = (qreal)(x1 + w);
}

/*!
    Sets the height of the rectangle to \a h. The top edge is not
    moved, but the bottom edge may be moved.

    \sa height(), setTop(), setBottom(), setSize()
*/

void FRect::setHeight( qreal h )
{
    y2 = (qreal)(y1 + h);
}

/*!
    Sets the size of the rectangle to \a s. The top-left corner is not
    moved.

    \sa size(), setWidth(), setHeight()
*/

void FRect::setSize( const FSize &s )
{
    x2 = (qreal)(s.width() +x1);
    y2 = (qreal)(s.height()+y1);
}

/*!
    Returns TRUE if the point \a p is inside or on the edge of the
    rectangle; otherwise returns FALSE.

    If \a proper is TRUE, this function returns TRUE only if \a p is
    inside (not on the edge).
*/

bool FRect::contains( FPoint &p, bool proper ) const
{
    if ( proper )
	return p.x() > x1 && p.x() < x2 &&
	       p.y() > y1 && p.y() < y2;
    else
	return p.x() >= x1 && p.x() <= x2 &&
	       p.y() >= y1 && p.y() <= y2;
}

/*!
    \overload bool FRect::contains( qreal x, qreal y, bool proper ) const

    Returns TRUE if the point \a x, \a y is inside this rectangle;
    otherwise returns FALSE.

    If \a proper is TRUE, this function returns TRUE only if the point
    is entirely inside (not on the edge).
*/

/*!
    \overload bool FRect::contains( qreal x, qreal y ) const

    Returns TRUE if the point \a x, \a y is inside this rectangle;
    otherwise returns FALSE.
*/

/*!
    \overload

    Returns TRUE if the rectangle \a r is inside this rectangle;
    otherwise returns FALSE.

    If \a proper is TRUE, this function returns TRUE only if \a r is
    entirely inside (not on the edge).

    \sa unite(), intersect(), intersects()
*/

bool FRect::contains( const FRect &r, bool proper ) const
{
    if ( proper )
	return r.x1 > x1 && r.x2 < x2 && r.y1 > y1 && r.y2 < y2;
    else
	return r.x1 >= x1 && r.x2 <= x2 && r.y1 >= y1 && r.y2 <= y2;
}

/*!
    Unites this rectangle with rectangle \a r.
*/
FRect& FRect::operator|=(const FRect &r)
{
    *this = *this | r;
    return *this;
}

/*!
    Intersects this rectangle with rectangle \a r.
*/
FRect& FRect::operator&=(const FRect &r)
{
    *this = *this & r;
    return *this;
}


/*!
    Returns the bounding rectangle of this rectangle and rectangle \a
    r.

    The bounding rectangle of a nonempty rectangle and an empty or
    invalid rectangle is defined to be the nonempty rectangle.

    \sa operator|=(), operator&(), intersects(), contains()
*/

FRect FRect::operator|(const FRect &r) const
{
    if ( isValid() ) {
	if ( r.isValid() ) {
	    FRect tmp;
	    tmp.setLeft(   qMin( x1, r.x1 ) );
	    tmp.setRight(  qMax( x2, r.x2 ) );
	    tmp.setTop(	   qMin( y1, r.y1 ) );
	    tmp.setBottom( qMax( y2, r.y2 ) );
	    return tmp;
	} else {
	    return *this;
	}
    } else {
	return r;
    }
}

/*!
    Returns the bounding rectangle of this rectangle and rectangle \a
    r. \c{r.unite(s)} is equivalent to \c{r|s}.
*/
FRect FRect::unite( const FRect &r ) const
{
    return *this | r;
}


/*!
    Returns the intersection of this rectangle and rectangle \a r.

    Returns an empty rectangle if there is no intersection.

    \sa operator&=(), operator|(), isEmpty(), intersects(), contains()
*/

FRect FRect::operator&( const FRect &r ) const
{
    FRect tmp;
    tmp.x1 = qMax( x1, r.x1 );
    tmp.x2 = qMin( x2, r.x2 );
    tmp.y1 = qMax( y1, r.y1 );
    tmp.y2 = qMin( y2, r.y2 );
    return tmp;
}

/*!
    Returns the intersection of this rectangle and rectangle \a r.
    \c{r.intersect(s)} is equivalent to \c{r&s}.
*/
FRect FRect::intersect( const FRect &r ) const
{
    return *this & r;
}

/*!
    Returns TRUE if this rectangle intersects with rectangle \a r
    (there is at least one pixel that is within both rectangles);
    otherwise returns FALSE.

    \sa intersect(), contains()
*/

bool FRect::intersects( const FRect &r ) const
{
    return ( qMax( x1, r.x1 ) <= qMin( x2, r.x2 ) &&
	     qMax( y1, r.y1 ) <= qMin( y2, r.y2 ) );
}


/*!
    \relates FRect

    Returns TRUE if \a r1 and \a r2 are equal; otherwise returns FALSE.
*/

bool operator==( const FRect &r1, const FRect &r2 )
{
    return r1.x1==r2.x1 && r1.x2==r2.x2 && r1.y1==r2.y1 && r1.y2==r2.y2;
}

/*!
    \relates FRect

    Returns TRUE if \a r1 and \a r2 are different; otherwise returns FALSE.
*/

bool operator!=( const FRect &r1, const FRect &r2 )
{
    return r1.x1!=r2.x1 || r1.x2!=r2.x2 || r1.y1!=r2.y1 || r1.y2!=r2.y2;
}


// /*****************************************************************************
//   FRect stream functions
//  *****************************************************************************/
// #ifndef QT_NO_DATASTREAM
// /*!
//     \relates FRect
//
//     Writes the FRect, \a r, to the stream \a s, and returns a
//     reference to the stream.
//
//     \sa \link datastreamformat.html Format of the QDataStream operators \endlink
// */
//
// QDataStream &operator<<( QDataStream &s, const FRect &r )
// {
//     if ( s.version() == 1 )
// 	s << (Q_INT16)r.left() << (Q_INT16)r.top()
// 	  << (Q_INT16)r.right() << (Q_INT16)r.bottom();
//     else
// 	s << (Q_INT32)r.left() << (Q_INT32)r.top()
// 	  << (Q_INT32)r.right() << (Q_INT32)r.bottom();
//     return s;
// }
//
// /*!
//     \relates FRect
//
//     Reads a FRect from the stream \a s into rect \a r and returns a
//     reference to the stream.
//
//     \sa \link datastreamformat.html Format of the QDataStream operators \endlink
// */
//
// QDataStream &operator>>( QDataStream &s, FRect &r )
// {
//     if ( s.version() == 1 ) {
// 	Q_INT16 x1, y1, x2, y2;
// 	s >> x1; s >> y1; s >> x2; s >> y2;
// 	r.setCoords( x1, y1, x2, y2 );
//     }
//     else {
// 	Q_INT32 x1, y1, x2, y2;
// 	s >> x1; s >> y1; s >> x2; s >> y2;
// 	r.setCoords( x1, y1, x2, y2 );
//     }
//     return s;
// }
// #endif // QT_NO_DATASTREAM
