/**
 * \file
 * \brief  LinearN fragment function class
 *
 *  Authors:
 *   JF Barraud <jf.barraud@gmail.com>
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

#ifndef SEEN_LINEARN_H
#define SEEN_LINEARN_H
#include "interval.h"
#include "isnan.h"
#include "linear.h" //for convertion purpose ( + lerp() )

#include <iostream>

namespace Geom{

//TODO: define this only once!! (see linear.h)
inline double lerpppp(double t, double a, double b) { return a*(1-t) + b*t; }

template<unsigned n>
class SBasisN;

template<unsigned n>
class LinearN{
public:
    double a[1<<n];// 1<<n is 2^n
    LinearN() {
        for (unsigned i=0; i < (1<<n); i++){
            a[i] = 0.;
        }
    }
    LinearN(double aa[]) {
        for (unsigned i=0; i < (1<<n); i++){
            a[i] = aa[i];
        }
    }
    LinearN(double c) {
        for (unsigned i=0; i<(1<<n); i++){
            a[i] = c;
        }
    }
    LinearN(LinearN<n-1> const &aa, LinearN<n-1> const &b, unsigned var=0) {
//         for (unsigned i=0; i<(1<<n-1); i++){
//             a[i] = aa[i];
//             a[i+(1<<(n-1))] = b[i];
//         }
        unsigned mask = (1<<var)-1;
        for (unsigned i=0; i < (1<<(n-1)); i++){
            unsigned low_i = i & mask, high_i = i & ~mask;
            unsigned idx0 = (high_i<<1)|low_i;
            unsigned idx1 = (high_i<<1)|(1<<var)|low_i;
            a[idx0] = aa[i];
            a[idx1] = b[i];
        }

    }
    double operator[](const int i) const {
        assert( i >= 0 );
        assert( i < (1<<n) );
        return a[i];
    }
    double& operator[](const int i) {
        assert(i >= 0);
        assert(i < (1<<n) );
        return a[i];
    }

    //IMPL: FragmentConcept
    typedef double output_type;
    unsigned input_dim() const {return n;}
    inline bool isZero() const { 
        for (unsigned i=0; i < (1<<n); i++){
            if (a[i] != 0) return false;
        }
        return true; }
    inline bool isConstant() const { 
        for (unsigned i=1; i < (1<<n); i++){
            if (a[i] != a[0]) return false;
        }
        return true; }
    inline bool isConstant(unsigned var) const { 
        unsigned mask = (1<<var)-1;
        for (unsigned i=0; i < (1<<(n-1)); i++){
            unsigned low_i = i & mask, high_i = i & ~mask;
            unsigned idx0 = (high_i<<1)|low_i;
            unsigned idx1 = (high_i<<1)|(1<<var)|low_i;
            if (a[idx0] != a[idx1]) return false;
        }
        return true; 
    }
    inline bool isFinite() const {
        for (unsigned i=0; i < (1<<n); i++){
            if ( !IS_FINITE(a[i]) ) return false;
        }
        return true; }
    //value if k-th variable is set to 0.
    inline LinearN<n-1> at0(unsigned k=0) const {
        LinearN<n-1> res;
        unsigned mask = (1<<k)-1;
        for (unsigned i=0; i < (1<<(n-1)); i++){
            unsigned low_i = i & mask, high_i = i & ~mask;
            unsigned idx = (high_i<<1)|low_i;
            res[i] = a[idx];
        }
        return res;
    }
    //value if k-th variable is set to 1.
    inline LinearN<n-1> at1(unsigned k=0) const {
        LinearN<n-1> res;
        for (unsigned i=0; i < (1<<(n-1)); i++){
            unsigned mask = (1<<k)-1;
            unsigned low_i = i & mask, high_i = i & ~mask;
            unsigned idx = (high_i<<1)|(1<<k)|low_i;
            res[i] = a[idx];
        }
        return res;
    }
    inline double atCorner(unsigned k) const {
        assert( k < (1<<n) );
        return a[k];
    }
    inline double atCorner(double t[]) const { 
        unsigned k=0;
        for(unsigned i=0; i<n; i++){
            if (t[i] == 1.) k = k | (1<<i);
            else assert( t[i] == 0. );
        }
        return atCorner(k);
    }
    inline LinearN<n-1> partialEval(double t, unsigned var=0 ) const {
        LinearN<n-1> res;
        res = at0(var)*(1-t) + at1(var)*t;
        return res;
    }
    
    //fixed and flags are used for recursion.
    inline double valueAt(double t[], unsigned fixed=0, unsigned flags=0 ) const {
        if (fixed == n) {
            return a[flags]; 
        }else{
            double a0 = valueAt(t, fixed+1, flags);
            double a1 = valueAt(t, fixed+1, flags|(1<<fixed));
            return lerpppp( t[fixed], a0, a1 );
        }
    }
    inline double operator()(double t[]) const { return valueAt(t); }

    //defined in sbasisN.h
    inline SBasisN<n> toSBasisN() const;

    inline OptInterval bounds_exact() const {
        double min=a[0], max=a[0];
        for (unsigned i=1; i < (1<<n); i++){
            if (a[i] < min) min = a[i];
            if (a[i] > max) max = a[i];
        }
        return Interval(min, max); 
    }
    inline OptInterval bounds_fast() const { return bounds_exact(); }
    //inline OptInterval bounds_local(double u, double v) const { return Interval(valueAt(u), valueAt(v)); }
};

//LinearN<0> are doubles. Specialize them out.
template<>
class LinearN<0>{
public:
    double d;
    LinearN () {}
    LinearN(double d) :d(d) {}
    operator double() const { return d; }
    double operator[](const int i) const {assert (i==0); return d;}
    double& operator[](const int i) {assert (i==0); return d;}
    typedef double output_type;
    unsigned input_dim() const {return 0;}
    inline bool isZero() const { return d==0; }
    inline bool isConstant() const { return true; }
    inline bool isFinite() const { return IS_FINITE(d); }
};

//LinearN<1> are usual Linear. Allow conversion.
Linear toLinear(LinearN<1> f){
    return Linear(f[0],f[1]);
}



//inline Linear reverse(Linear const &a) { return Linear(a[1], a[0]); }

//IMPL: AddableConcept
template<unsigned n>
inline LinearN<n> operator+(LinearN<n> const & a, LinearN<n> const & b) {
    LinearN<n> res;
    for (unsigned i=0; i < (1<<n); i++){
        res[i] = a[i] + b[i];
    }
    return res;
}
template<unsigned n>
inline LinearN<n> operator-(LinearN<n> const & a, LinearN<n> const & b) {
    LinearN<n> res;
    for (unsigned i=0; i < (1<<n); i++){
        res[i] = a[i] - b[i];
    }
    return res;
}
template<unsigned n>
inline LinearN<n>& operator+=(LinearN<n> & a, LinearN<n> const & b) {
    for (unsigned i=0; i < (1<<n); i++){
        a[i] += b[i];
    }
    return a;
}
template<unsigned n>
inline LinearN<n>& operator-=(LinearN<n> & a, LinearN<n> const & b) {
    for (unsigned i=0; i < (1<<n); i++){
        a[i] -= b[i];
    }
    return a;
}
//IMPL: OffsetableConcept
template<unsigned n>
inline LinearN<n> operator+(LinearN<n> const & a, double b) {
    LinearN<n> res;
    for (unsigned i=0; i < (1<<n); i++){
        res[i] = a[i] + b;
    }
    return res;
}
template<unsigned n>
inline LinearN<n> operator-(LinearN<n> const & a, double b) {
    LinearN<n> res;
    for (unsigned i=0; i < (1<<n); i++){
        res[i] = a[i] - b;
    }
    return res;
}
template<unsigned n>
inline LinearN<n>& operator+=(LinearN<n> & a, double b) {
    for (unsigned i=0; i < (1<<n); i++){
        a[i] += b;
    }
    return a;
}
template<unsigned n>
inline LinearN<n>& operator-=(LinearN<n> & a, double b) {
    for (unsigned i=0; i < (1<<n); i++){
        a[i] -= b;
    }
    return a;
}
//IMPL: boost::EqualityComparableConcept
template<unsigned n>
inline bool operator==(LinearN<n> const & a, LinearN<n> const & b) {
    for (unsigned i=0; i < (1<<n); i++){
        if (a[i] != b[i]) return false;
    }
    return true;
}
template<unsigned n>
inline bool operator!=(LinearN<n> const & a, LinearN<n> const & b) {
    return !(a==b);
}
//IMPL: ScalableConcept
template<unsigned n>
inline LinearN<n> operator-(LinearN<n> const &a) {
    LinearN<n> res;
    for (unsigned i=0; i < (1<<n); i++){
        res[i] = -a[i];
    }
    return res;
}
template<unsigned n>
inline LinearN<n> operator*(LinearN<n> const & a, double b) {
    LinearN<n> res;
    for (unsigned i=0; i < (1<<n); i++){
        res[i] = a[i] * b;
    }
    return res;
}
template<unsigned n>
inline LinearN<n> operator/(LinearN<n> const & a, double b) {
    LinearN<n> res;
    for (unsigned i=0; i < (1<<n); i++){
        res[i] = a[i] / b;
    }
    return res;
}
template<unsigned n>
inline LinearN<n> operator*=(LinearN<n> & a, double b) {
    for (unsigned i=0; i < (1<<n); i++){
        a[i] *= b;
    }
    return a;
}
template<unsigned n>
inline LinearN<n> operator/=(LinearN<n> & a, double b) {
    for (unsigned i=0; i < (1<<n); i++){
        a[i] /= b;
    }
    return a;
}

template<unsigned n>
void setToVariable(LinearN<n> &x, unsigned k){;
    x = LinearN<n>(0.);
    unsigned mask = 1<<k;
    for (unsigned i=0; i < (1<<n); i++){
        if ( i & mask ) x[i] = 1;
    }
}

template<unsigned n>
inline std::ostream &operator<< (std::ostream &out_file, const LinearN<n> &bo) {
    out_file << "{";
    for (unsigned i=0; i < (1<<n); i++){
        out_file << bo[i]<<(i == (1<<n)-1 ? "}" : ",");
    }
    return out_file;
}


}
#endif //SEEN_LINEAR_H

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
