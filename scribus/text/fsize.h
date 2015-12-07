/****************************************************************************
** $Id$
**
** Definition of FSize class
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

#ifndef FSIZE_H
#define FSIZE_H

// #ifndef QT_H
 #include "../fpoint.h" // ### change to qwindowdefs.h?
// #endif // QT_H

class FSize
// ### Make FSize inherit Qt in Qt 4.0
{
public:
    // ### Move this enum to qnamespace.h in Qt 4.0
    enum ScaleMode {
	ScaleFree,
	ScaleMin,
	ScaleMax
    };

    FSize();
    FSize( qreal w, qreal h );

    bool isNull() const;
    bool isEmpty() const;
    bool isValid() const;

    qreal width() const;
    qreal height() const;
    void setWidth( qreal w );
    void setHeight( qreal h );
    void transpose();

    void scale( qreal w, qreal h, Qt::AspectRatioMode mode );
    void scale( const FSize &s, Qt::AspectRatioMode mode );

    FSize expandedTo( const FSize & ) const;
    FSize boundedTo( const FSize & ) const;

    qreal &rwidth();
    qreal &rheight();

    FSize &operator+=( const FSize & );
    FSize &operator-=( const FSize & );
    FSize &operator*=( int c );
    FSize &operator*=( qreal c );
    FSize &operator/=( int c );
    FSize &operator/=( qreal c );

    friend inline bool operator==( const FSize &, const FSize & );
    friend inline bool operator!=( const FSize &, const FSize & );
    friend inline const FSize operator+( const FSize &, const FSize & );
    friend inline const FSize operator-( const FSize &, const FSize & );
    friend inline const FSize operator*( const FSize &, int );
    friend inline const FSize operator*( int, const FSize & );
    friend inline const FSize operator*( const FSize &, qreal );
    friend inline const FSize operator*( qreal, const FSize & );
    friend inline const FSize operator/( const FSize &, int );
    friend inline const FSize operator/( const FSize &, qreal );

private:
    static void warningDivByZero();

	qreal m_wd;
	qreal m_ht;
};


/*****************************************************************************
  FSize stream functions
 *****************************************************************************/

// QDataStream &operator<<( QDataStream &, const FSize & );
// QDataStream &operator>>( QDataStream &, FSize & );


/*****************************************************************************
  FSize inline functions
 *****************************************************************************/

inline FSize::FSize()
{ m_wd = m_ht = -1; }

inline FSize::FSize( qreal w, qreal h )
{ m_wd=(qreal)w; m_ht=(qreal)h; }

inline bool FSize::isNull() const
{ return m_wd==0 && m_ht==0; }

inline bool FSize::isEmpty() const
{ return m_wd<1 || m_ht<1; }

inline bool FSize::isValid() const
{ return m_wd>=0 && m_ht>=0; }

inline qreal FSize::width() const
{ return m_wd; }

inline qreal FSize::height() const
{ return m_ht; }

inline void FSize::setWidth( qreal w )
{ m_wd=(qreal)w; }

inline void FSize::setHeight( qreal h )
{ m_ht=(qreal)h; }

inline qreal &FSize::rwidth()
{ return m_wd; }

inline qreal &FSize::rheight()
{ return m_ht; }

inline FSize &FSize::operator+=( const FSize &s )
{ m_wd+=s.m_wd; m_ht+=s.m_ht; return *this; }

inline FSize &FSize::operator-=( const FSize &s )
{ m_wd-=s.m_wd; m_ht-=s.m_ht; return *this; }

inline FSize &FSize::operator*=( int c )
{ m_wd*=(qreal)c; m_ht*=(qreal)c; return *this; }

inline FSize &FSize::operator*=( qreal c )
{ m_wd=(qreal)(m_wd*c); m_ht=(qreal)(m_ht*c); return *this; }

inline bool operator==( const FSize &s1, const FSize &s2 )
{ return s1.m_wd == s2.m_wd && s1.m_ht == s2.m_ht; }

inline bool operator!=( const FSize &s1, const FSize &s2 )
{ return s1.m_wd != s2.m_wd || s1.m_ht != s2.m_ht; }

inline const FSize operator+( const FSize & s1, const FSize & s2 )
{ return FSize(s1.m_wd+s2.m_wd, s1.m_ht+s2.m_ht); }

inline const FSize operator-( const FSize &s1, const FSize &s2 )
{ return FSize(s1.m_wd-s2.m_wd, s1.m_ht-s2.m_ht); }

inline const FSize operator*( const FSize &s, int c )
{ return FSize(s.m_wd*c, s.m_ht*c); }

inline const FSize operator*( int c, const FSize &s )
{  return FSize(s.m_wd*c, s.m_ht*c); }

inline const FSize operator*( const FSize &s, qreal c )
{ return FSize((qreal)(s.m_wd*c), (qreal)(s.m_ht*c)); }

inline const FSize operator*( qreal c, const FSize &s )
{ return FSize((qreal)(s.m_wd*c), (qreal)(s.m_ht*c)); }

inline FSize &FSize::operator/=( int c )
{
#if defined(QT_CHECK_MATH)
    if ( c == 0 )
	warningDivByZero();
#endif
	m_wd/=(qreal)c; m_ht/=(qreal)c;
    return *this;
}

inline FSize &FSize::operator/=( qreal c )
{
#if defined(QT_CHECK_MATH)
    if ( c == 0.0 )
	warningDivByZero();
#endif
	m_wd=(qreal)(m_wd/c); m_ht=(qreal)(m_ht/c);
    return *this;
}

inline const FSize operator/( const FSize &s, int c )
{
#if defined(QT_CHECK_MATH)
    if ( c == 0 )
	FSize::warningDivByZero();
#endif
	return FSize(s.m_wd/c, s.m_ht/c);
}

inline const FSize operator/( const FSize &s, qreal c )
{
#if defined(QT_CHECK_MATH)
    if ( c == 0.0 )
	FSize::warningDivByZero();
#endif
	return FSize((qreal)(s.m_wd/c), (qreal)(s.m_ht/c));
}

inline FSize FSize::expandedTo( const FSize & otherSize ) const
{
	return FSize( qMax(m_wd,otherSize.m_wd), qMax(m_ht,otherSize.m_ht) );
}

inline FSize FSize::boundedTo( const FSize & otherSize ) const
{
	return FSize( qMin(m_wd,otherSize.m_wd), qMin(m_ht,otherSize.m_ht) );
}


#endif // QSIZE_H
