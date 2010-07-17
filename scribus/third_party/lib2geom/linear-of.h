/**
 * \file
 * \brief  Linear fragment function class
 *
 *  Authors:
 *   Nathan Hurst <njh@mail.csse.monash.edu.au>
 *   Michael Sloan <mgsloan@gmail.com>
 *
 * Copyright (C) 2006-2007 authors
 *
 * This library is free software; you can redistribute it and/or
 * modify it either under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * (the "LGPL") or, at your option, under the terms of the Mozilla
 * Public License Version 1.1 (the "MPL"). If you do not alter this
 * notice, a recipient may use your version of this file under either
 * the MPL or the LGPL.
 *
 * You should have received a copy of the LGPL along with this library
 * in the file COPYING-LGPL-2.1; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * You should have received a copy of the MPL along with this library
 * in the file COPYING-MPL-1.1
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY
 * OF ANY KIND, either express or implied. See the LGPL or the MPL for
 * the specific language governing rights and limitations.
 */

#ifndef SEEN_LINEAR_OF_H
#define SEEN_LINEAR_OF_H
#include "interval.h"
#include "isnan.h"

namespace Geom{

template <typename T>
inline T lerp(double t, T a, T b) { return a*(1-t) + b*t; }

template <typename T>
class SBasisOf;

template <typename T>
class HatOf{
public:
    HatOf () {}
    HatOf(T d) :d(d) {}
    operator T() const { return d; }
    T d;
};

template <typename T>
class TriOf{
public:
    TriOf () {}
    TriOf(double d) :d(d) {}
    operator T() const { return d; }
    T d;
};


//--------------------------------------------------------------------------
#ifdef USE_SBASIS_OF
template <typename T>
class LinearOf;
typedef Geom::LinearOf<double> Linear;
#endif
//--------------------------------------------------------------------------

template <typename T>
class LinearOf{
public:
    T a[2];
    LinearOf() {}
    LinearOf(T aa, T b) {a[0] = aa; a[1] = b;}
    //LinearOf(double aa, double b) {a[0] = T(aa); a[1] = T(b);}
    LinearOf(HatOf<T> h, TriOf<T> t) {
        a[0] = T(h) - T(t)/2; 
        a[1] = T(h) + T(t)/2;
    }

    LinearOf(HatOf<T> h) {
        a[0] = T(h); 
        a[1] = T(h);
    }

    unsigned input_dim(){return T::input_dim() + 1;}

    T operator[](const int i) const {
        assert(i >= 0);
        assert(i < 2);
        return a[i];
    }
    T& operator[](const int i) {
        assert(i >= 0);
        assert(i < 2);
        return a[i];
    }

    //IMPL: FragmentConcept
    typedef T output_type;
    inline bool isZero() const { return a[0].isZero() && a[1].isZero(); }
    inline bool isConstant() const { return a[0] == a[1]; }
    inline bool isFinite() const { return IS_FINITE(a[0]) && IS_FINITE(a[1]); }

    inline T at0() const { return a[0]; }
    inline T at1() const { return a[1]; }

    inline T valueAt(double t) const { return lerp(t, a[0], a[1]); }
    inline T operator()(double t) const { return valueAt(t); }

    //defined in sbasis.h
    inline SBasisOf<T> toSBasis() const;

//This is specific for T=double!!
    inline OptInterval bounds_exact() const { return Interval(a[0], a[1]); }
    inline OptInterval bounds_fast() const { return bounds_exact(); }
    inline OptInterval bounds_local(double u, double v) const { return Interval(valueAt(u), valueAt(v)); }

    operator TriOf<T>() const {
        return a[1] - a[0];
    }
    operator HatOf<T>() const {
        return (a[1] + a[0])/2;
    }
};

template <>
unsigned LinearOf<double>::input_dim(){return 1;}
template <>
inline OptInterval LinearOf<double>::bounds_exact() const { return Interval(a[0], a[1]); }
template <>
inline OptInterval LinearOf<double>::bounds_fast() const { return bounds_exact(); }
template <>
inline OptInterval LinearOf<double>::bounds_local(double u, double v) const { return Interval(valueAt(u), valueAt(v)); }
template <>
inline bool LinearOf<double>::isZero() const { return a[0]==0 && a[1]==0; }

template <typename T>
inline LinearOf<T> reverse(LinearOf<T> const &a) { return LinearOf<T>(a[1], a[0]); }

//IMPL: AddableConcept
template <typename T>
inline LinearOf<T> operator+(LinearOf<T> const & a, LinearOf<T> const & b) {
    return LinearOf<T>(a[0] + b[0], a[1] + b[1]);
}
template <typename T>
inline LinearOf<T> operator-(LinearOf<T> const & a, LinearOf<T> const & b) {
    return LinearOf<T>(a[0] - b[0], a[1] - b[1]);
}
template <typename T>
inline LinearOf<T>& operator+=(LinearOf<T> & a, LinearOf<T> const & b) {
    a[0] += b[0]; a[1] += b[1];
    return a;
}
template <typename T>
inline LinearOf<T>& operator-=(LinearOf<T> & a, LinearOf<T> const & b) {
    a[0] -= b[0]; a[1] -= b[1];
    return a;
}
//IMPL: OffsetableConcept
template <typename T>
inline LinearOf<T> operator+(LinearOf<T> const & a, double b) {
    return LinearOf<T>(a[0] + b, a[1] + b);
}
template <typename T>
inline LinearOf<T> operator-(LinearOf<T> const & a, double b) {
    return LinearOf<T>(a[0] - b, a[1] - b);
}
template <typename T>
inline LinearOf<T>& operator+=(LinearOf<T> & a, double b) {
    a[0] += b; a[1] += b;
    return a;
}
template <typename T>
inline LinearOf<T>& operator-=(LinearOf<T> & a, double b) {
    a[0] -= b; a[1] -= b;
    return a;
}
/*
//We can in fact offset in coeff ring T...
template <typename T>
inline LinearOf<T> operator+(LinearOf<T> const & a, T b) {
    return LinearOf<T>(a[0] + b, a[1] + b);
}
template <typename T>
inline LinearOf<T> operator-(LinearOf<T> const & a, T b) {
    return LinearOf<T>(a[0] - b, a[1] - b);
}
template <typename T>
inline LinearOf<T>& operator+=(LinearOf<T> & a, T b) {
    a[0] += b; a[1] += b;
    return a;
}
template <typename T>
inline LinearOf<T>& operator-=(LinearOf<T> & a, T b) {
    a[0] -= b; a[1] -= b;
    return a;
}
*/

//IMPL: boost::EqualityComparableConcept
template <typename T>
inline bool operator==(LinearOf<T> const & a, LinearOf<T> const & b) {
    return a[0] == b[0] && a[1] == b[1];
}
template <typename T>
inline bool operator!=(LinearOf<T> const & a, LinearOf<T> const & b) {
    return a[0] != b[0] || a[1] != b[1];
}
//IMPL: ScalableConcept
template <typename T>
inline LinearOf<T> operator-(LinearOf<T> const &a) {
    return LinearOf<T>(-a[0], -a[1]);
}
template <typename T>
inline LinearOf<T> operator*(LinearOf<T> const & a, double b) {
    return LinearOf<T>(a[0]*b, a[1]*b);
}
template <typename T>
inline LinearOf<T> operator/(LinearOf<T> const & a, double b) {
    return LinearOf<T>(a[0]/b, a[1]/b);
}
template <typename T>
inline LinearOf<T> operator*=(LinearOf<T> & a, double b) {
    a[0] *= b; a[1] *= b;
    return a;
}
template <typename T>
inline LinearOf<T> operator/=(LinearOf<T> & a, double b) {
    a[0] /= b; a[1] /= b;
    return a;
}
/*
//We can in fact rescale in coeff ring T... (but not devide!)
template <typename T>
inline LinearOf<T> operator*(LinearOf<T> const & a, T b) {
    return LinearOf<T>(a[0]*b, a[1]*b);
}
template <typename T>
inline LinearOf<T> operator/(LinearOf<T> const & a, T b) {
    return LinearOf<T>(a[0]/b, a[1]/b);
}
template <typename T>
inline LinearOf<T> operator*=(LinearOf<T> & a, T b) {
    a[0] *= b; a[1] *= b;
    return a;
}
*/

};

#endif //SEEN_LINEAR_OF_H

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0)(inline-open . 0)(case-label . +))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:encoding=utf-8:textwidth=99 :
