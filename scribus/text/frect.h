/****************************************************************************
** $Id$
**
** Definition of FRect class
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

#ifndef FRECT_H
#define FRECT_H

//#ifndef QT_H
#include "fsize.h"
//#endif // QT_H

#if defined(topLeft)
#error "Macro definition of topLeft conflicts with FRect"
// don't just silently undo people's defines: #undef topLeft
#endif

class Q_EXPORT FRect					// rectangle class
{
public:
    FRect()	{ x1 = y1 = 0; x2 = y2 = -1; }
    FRect( FPoint &topleft, FPoint &bottomright );
    FRect( FPoint &topleft, FSize &size );
    FRect( double left, double top, double width, double height );

    bool   isNull()	const;
    bool   isEmpty()	const;
    bool   isValid()	const;
    FRect  normalize()	const;

    double	   left()	const;
    double	   top()	const;
    double	   right()	const;
    double	   bottom()	const;

    double &rLeft();
    double &rTop();
    double &rRight();
    double &rBottom();

    double	   x()		const;
    double	   y()		const;
    void   setLeft( double pos );
    void   setTop( double pos );
    void   setRight( double pos );
    void   setBottom( double pos );
    void   setX( double x );
    void   setY( double y );

    void   setTopLeft( FPoint &p );
    void   setBottomRight( FPoint &p );
    void   setTopRight( FPoint &p );
    void   setBottomLeft( FPoint &p );

    FPoint topLeft()	 const;
    FPoint bottomRight() const;
    FPoint topRight()	 const;
    FPoint bottomLeft()	 const;
    FPoint center()	 const;

    void   rect( double *x, double *y, double *w, double *h ) const;
    void   coords( double *x1, double *y1, double *x2, double *y2 ) const;

    void   moveLeft( double pos );
    void   moveTop( double pos );
    void   moveRight( double pos );
    void   moveBottom( double pos );
    void   moveTopLeft( FPoint &p );
    void   moveBottomRight( FPoint &p );
    void   moveTopRight( FPoint &p );
    void   moveBottomLeft( FPoint &p );
    void   moveCenter( FPoint &p );
    void   moveBy( double dx, double dy );

    void   setRect( double x, double y, double w, double h );
    void   setCoords( double x1, double y1, double x2, double y2 );
    void   addCoords( double x1, double y1, double x2, double y2 );

    FSize  size()	const;
    double	   width()	const;
    double	   height()	const;
    void   setWidth( double w );
    void   setHeight( double h );
    void   setSize( const FSize &s );

    FRect  operator|(const FRect &r) const;
    FRect  operator&(const FRect &r) const;
    FRect&  operator|=(const FRect &r);
    FRect&  operator&=(const FRect &r);

    bool   contains( FPoint &p, bool proper=FALSE ) const;
    bool   contains( double x, double y ) const; // inline methods, _don't_ merge these
    bool   contains( double x, double y, bool proper ) const;
    bool   contains( const FRect &r, bool proper=FALSE ) const;
    FRect  unite( const FRect &r ) const;
    FRect  intersect( const FRect &r ) const;
    bool   intersects( const FRect &r ) const;

    friend Q_EXPORT bool operator==( const FRect &, const FRect & );
    friend Q_EXPORT bool operator!=( const FRect &, const FRect & );

private:
#if defined(Q_WS_X11) || defined(Q_OS_TEMP)
    friend void qt_setCoords( FRect *r, double xp1, double yp1, double xp2, double yp2 );
#endif

    double x1;
    double y1;
    double x2;
    double y2;

};

Q_EXPORT bool operator==( const FRect &, const FRect & );
Q_EXPORT bool operator!=( const FRect &, const FRect & );


/*****************************************************************************
  FRect stream functions
 *****************************************************************************/
// #ifndef QT_NO_DATASTREAM
// Q_EXPORT QDataStream &operator<<( QDataStream &, const FRect & );
// Q_EXPORT QDataStream &operator>>( QDataStream &, FRect & );
// #endif

/*****************************************************************************
  FRect inline member functions
 *****************************************************************************/

inline FRect::FRect( double left, double top, double width, double height )
{
    x1 = (double)left;
    y1 = (double)top;
    x2 = (double)(left+width-1);
    y2 = (double)(top+height-1);
}

inline bool FRect::isNull() const
{ return x2 == x1-1 && y2 == y1-1; }

inline bool FRect::isEmpty() const
{ return x1 > x2 || y1 > y2; }

inline bool FRect::isValid() const
{ return x1 <= x2 && y1 <= y2; }

inline double FRect::left() const
{ return x1; }

inline double FRect::top() const
{ return y1; }

inline double FRect::right() const
{ return x2; }

inline double FRect::bottom() const
{ return y2; }

inline double &FRect::rLeft()
{ return x1; }

inline double & FRect::rTop()
{ return y1; }

inline double & FRect::rRight()
{ return x2; }

inline double & FRect::rBottom()
{ return y2; }

inline double FRect::x() const
{ return x1; }

inline double FRect::y() const
{ return y1; }

inline void FRect::setLeft( double pos )
{ x1 = (double)pos; }

inline void FRect::setTop( double pos )
{ y1 = (double)pos; }

inline void FRect::setRight( double pos )
{ x2 = (double)pos; }

inline void FRect::setBottom( double pos )
{ y2 = (double)pos; }

inline void FRect::setX( double x )
{ x1 = (double)x; }

inline void FRect::setY( double y )
{ y1 = (double)y; }

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

inline double FRect::width() const
{ return  x2 - x1 + 1; }

inline double FRect::height() const
{ return  y2 - y1 + 1; }

inline FSize FRect::size() const
{ return FSize(x2-x1+1, y2-y1+1); }

inline bool FRect::contains( double x, double y, bool proper ) const
{
    if ( proper )
        return x > x1 && x < x2 &&
               y > y1 && y < y2;
    else
        return x >= x1 && x <= x2 &&
               y >= y1 && y <= y2;
}

inline bool FRect::contains( double x, double y ) const
{
    return x >= x1 && x <= x2 &&
	   y >= y1 && y <= y2;
}
// #define Q_DEFINED_QRECT
// #include "qwinexport.h"
 #endif // QRECT_H
