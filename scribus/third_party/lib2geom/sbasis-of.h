/**
 * \file
 * \brief Defines S-power basis function class 
 * with coefficient in arbitrary ring
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

#ifndef SEEN_SBASIS_OF_H
#define SEEN_SBASIS_OF_H
#include <vector>
#include <cassert>
#include <iostream>

#include "linear-of.h"
#include "interval.h"
#include "utils.h"
#include 2exception.h"

namespace Geom {

template<typename T>
class SBasisOf;

#ifdef USE_SBASIS_OF
typedef Geom::SBasisOf<double> SBasis;
#endif

/*** An empty SBasisOf<T> is identically 0. */
template<typename T>
class SBasisOf : public std::vector<LinearOf<T> >{
public:
    SBasisOf() {}
    explicit SBasisOf(T a) {
        push_back(LinearOf<T>(a,a));
    }
    SBasisOf(SBasisOf<T> const & a) :
        std::vector<LinearOf<T> >(a)
    {}
    SBasisOf(LinearOf<T> const & bo) {
        push_back(bo);
    }
    SBasisOf(LinearOf<T>* bo) {
        push_back(*bo);
    }
    //static unsigned input_dim(){return T::input_dim()+1;}

    //IMPL: FragmentConcept
    typedef T output_type;
    inline bool isZero() const {
        if(this->empty()) return true;
        for(unsigned i = 0; i < this->size(); i++) {
            if(!(*this)[i].isZero()) return false;
        }
        return true;
    }
    inline bool isConstant() const {
        if (this->empty()) return true;
        for (unsigned i = 0; i < this->size(); i++) {
            if(!(*this)[i].isConstant()) return false;
        }
        return true;
    }

    //TODO: code this...
    bool isFinite() const;

    inline T at0() const { 
        if(this->empty()) return T(0); else return (*this)[0][0];
    }
    inline T at1() const{
        if(this->empty()) return T(0); else return (*this)[0][1];
    }

    T valueAt(double t) const {
        double s = t*(1-t);
        T p0 = T(0.), p1 = T(0.);
        for(unsigned k = this->size(); k > 0; k--) {
            const LinearOf<T> &lin = (*this)[k-1];
            p0 = p0*s + lin[0];
            p1 = p1*s + lin[1];
        }
        return p0*(1-t) + p1*t;
    }

    T operator()(double t) const {
        return valueAt(t);
    }

    /**
    *  The size of the returned vector equals n+1.
    */
    std::vector<T> valueAndDerivatives(double t, unsigned n) const{
        std::vector<T> ret(n+1);
        ret[0] = valueAt(t);
        SBasisOf<T> tmp = *this;
        for(unsigned i = 0; i < n; i++) {
            tmp.derive();
            ret[i+1] = tmp.valueAt(t);
        }
        return ret;
    }

    //The following lines only makes sens if T=double!
    SBasisOf<T> toSBasis() const { return SBasisOf<T>(*this); }
    double tailError(unsigned tail) const{
        Interval bs = *bounds_fast(*this, tail);
        return std::max(fabs(bs.min()),fabs(bs.max()));
    }

// compute f(g)
    SBasisOf<T> operator()(SBasisOf<T> const & g) const;

    LinearOf<T> operator[](unsigned i) const {
        assert(i < this->size());
        return std::vector<LinearOf<T> >::operator[](i);
    }

//MUTATOR PRISON
    LinearOf<T>& operator[](unsigned i) { return this->at(i); }

    //remove extra zeros
    void normalize() {
        while(!this->empty() && this->back().isZero())
            this->pop_back();
    }

    void truncate(unsigned k) { if(k < this->size()) this->resize(k); }
private:
    void derive(); // in place version
    unsigned dim;
};

//template<>
//inline unsigned SBasisOf<double>::input_dim() { return 1; }

//--------------------------------------------------------------------------
#ifdef USE_SBASIS_OF

//implemented in sbasis-roots.cpp
OptInterval bounds_exact(SBasis const &a);
OptInterval bounds_fast(SBasis const &a, int order = 0);
OptInterval bounds_local(SBasis const &a, const OptInterval &t, int order = 0);

std::vector<double> roots(SBasis const & s);
std::vector<std::vector<double> > multi_roots(SBasis const &f,
                                 std::vector<double> const &levels,
                                 double htol=1e-7,
                                 double vtol=1e-7,
                                 double a=0,
                                 double b=1);
#endif
//--------------------------------------------------------------------------


//TODO: figure out how to stick this in linear, while not adding an sbasis dep
template<typename T>
inline SBasisOf<T> LinearOf<T>::toSBasis() const { return SBasisOf<T>(*this); }

template<typename T>
inline SBasisOf<T> reverse(SBasisOf<T> const &a) {
    SBasisOf<T> result;
    result.reserve(a.size());
    for(unsigned k = 0; k < a.size(); k++)
       result.push_back(reverse(a[k]));
    return result;
}

//IMPL: ScalableConcept
template<typename T>
inline SBasisOf<T> operator-(const SBasisOf<T>& p) {
    if(p.isZero()) return SBasisOf<T>();
    SBasisOf<T> result;
    result.reserve(p.size());
        
    for(unsigned i = 0; i < p.size(); i++) {
        result.push_back(-p[i]);
    }
    return result;
}

template<typename T>
SBasisOf<T> operator*(SBasisOf<T> const &a, double k){
    SBasisOf<T> c;
    //TODO: what does this mean for vectors of vectors??
    //c.reserve(a.size());
    for(unsigned i = 0; i < a.size(); i++)
        c.push_back(a[i] * k);
    return c;
}

template<typename T>
inline SBasisOf<T> operator*(double k, SBasisOf<T> const &a) { return a*k; }
template<typename T>
inline SBasisOf<T> operator/(SBasisOf<T> const &a, double k) { return a*(1./k); }
template<typename T>
SBasisOf<T>& operator*=(SBasisOf<T>& a, double b){
    if (a.isZero()) return a;
    if (b == 0)
        a.clear();
    else
        for(unsigned i = 0; i < a.size(); i++)
            a[i] *= b;
    return a;
}

template<typename T>
inline SBasisOf<T>& operator/=(SBasisOf<T>& a, double b) { return (a*=(1./b)); }

/*
//We can also multiply by element of ring coeff T: 
template<typename T>
SBasisOf<T> operator*(SBasisOf<T> const &a, T k){
    SBasisOf<T> c;
    //TODO: what does this mean for vectors of vectors??
    //c.reserve(a.size());
    for(unsigned i = 0; i < a.size(); i++)
        c.push_back(a[i] * k);
    return c;
}

template<typename T>
inline SBasisOf<T> operator*(T k, SBasisOf<T> const &a) { return a*k; }
template<typename T>
SBasisOf<T>& operator*=(SBasisOf<T>& a, T b){
    if (a.isZero()) return a;
    if (b == 0)
        a.clear();
    else
        for(unsigned i = 0; i < a.size(); i++)
            a[i] *= b;
    return a;
}
*/

//IMPL: AddableConcept
template<typename T>
inline SBasisOf<T> operator+(const SBasisOf<T>& a, const SBasisOf<T>& b){
    SBasisOf<T> result;
    const unsigned out_size = std::max(a.size(), b.size());
    const unsigned min_size = std::min(a.size(), b.size());
    //TODO: what does this mean for vector<vector>;
    //result.reserve(out_size);

    for(unsigned i = 0; i < min_size; i++) {
        result.push_back(a[i] + b[i]);
    }
    for(unsigned i = min_size; i < a.size(); i++)
        result.push_back(a[i]);
    for(unsigned i = min_size; i < b.size(); i++)
        result.push_back(b[i]);

    assert(result.size() == out_size);
    return result;
}

template<typename T>
SBasisOf<T> operator-(const SBasisOf<T>& a, const SBasisOf<T>& b){
    SBasisOf<T> result;
    const unsigned out_size = std::max(a.size(), b.size());
    const unsigned min_size = std::min(a.size(), b.size());
    //TODO: what does this mean for vector<vector>;
    //result.reserve(out_size);

    for(unsigned i = 0; i < min_size; i++) {
        result.push_back(a[i] - b[i]);
    }
    for(unsigned i = min_size; i < a.size(); i++)
        result.push_back(a[i]);
    for(unsigned i = min_size; i < b.size(); i++)
        result.push_back(-b[i]);

    assert(result.size() == out_size);
    return result;
}

template<typename T>
SBasisOf<T>& operator+=(SBasisOf<T>& a, const SBasisOf<T>& b){
    const unsigned out_size = std::max(a.size(), b.size());
    const unsigned min_size = std::min(a.size(), b.size());
    //TODO: what does this mean for vectors of vectors
    //a.reserve(out_size);
    for(unsigned i = 0; i < min_size; i++)
        a[i] += b[i];
    for(unsigned i = min_size; i < b.size(); i++)
        a.push_back(b[i]);

    assert(a.size() == out_size);
    return a;
}

template<typename T>
SBasisOf<T>& operator-=(SBasisOf<T>& a, const SBasisOf<T>& b){
    const unsigned out_size = std::max(a.size(), b.size());
    const unsigned min_size = std::min(a.size(), b.size());
    //TODO: what does this mean for vectors of vectors
    //a.reserve(out_size);
    for(unsigned i = 0; i < min_size; i++)
        a[i] -= b[i];
    for(unsigned i = min_size; i < b.size(); i++)
        a.push_back(-b[i]);

    assert(a.size() == out_size);
    return a;
}

//TODO: remove?
template<typename T>
inline SBasisOf<T> operator+(const SBasisOf<T> & a, LinearOf<T> const & b) {
    if(b.isZero()) return a;
    if(a.isZero()) return b;
    SBasisOf<T> result(a);
    result[0] += b;
    return result;
}
template<typename T>
inline SBasisOf<T> operator-(const SBasisOf<T> & a, LinearOf<T> const & b) {
    if(b.isZero()) return a;
    SBasisOf<T> result(a);
    result[0] -= b;
    return result;
}
template<typename T>
inline SBasisOf<T>& operator+=(SBasisOf<T>& a, const LinearOf<T>& b) {
    if(a.isZero())
        a.push_back(b);
    else
        a[0] += b;
    return a;
}
template<typename T>
inline SBasisOf<T>& operator-=(SBasisOf<T>& a, const LinearOf<T>& b) {
    if(a.isZero())
        a.push_back(-b);
    else
        a[0] -= b;
    return a;
}

//IMPL: OffsetableConcept
/*
template<typename T>
inline SBasisOf<T> operator+(const SBasisOf<T> & a, double b) {
    if(a.isZero()) return LinearOf<T>(b, b);
    SBasisOf<T> result(a);
    result[0] += b;
    return result;
}
template<typename T>
inline SBasisOf<T> operator-(const SBasisOf<T> & a, double b) {
    if(a.isZero()) return LinearOf<T>(-b, -b);
    SBasisOf<T> result(a);
    result[0] -= b;
    return result;
}
template<typename T>
inline SBasisOf<T>& operator+=(SBasisOf<T>& a, double b) {
    if(a.isZero())
        a.push_back(LinearOf<T>(b,b));
    else
        a[0] += b;
    return a;
}
template<typename T>
inline SBasisOf<T>& operator-=(SBasisOf<T>& a, double b) {
    if(a.isZero())
        a.push_back(LinearOf<T>(-b,-b));
    else
        a[0] -= b;
    return a;
}
*/
//We can also offset by elements of coeff ring T
template<typename T>
inline SBasisOf<T> operator+(const SBasisOf<T> & a, T b) {
    if(a.isZero()) return LinearOf<T>(b, b);
    SBasisOf<T> result(a);
    result[0] += b;
    return result;
}
template<typename T>
inline SBasisOf<T> operator-(const SBasisOf<T> & a, T b) {
    if(a.isZero()) return LinearOf<T>(-b, -b);
    SBasisOf<T> result(a);
    result[0] -= b;
    return result;
}
template<typename T>
inline SBasisOf<T>& operator+=(SBasisOf<T>& a, T b) {
    if(a.isZero())
        a.push_back(LinearOf<T>(b,b));
    else
        a[0] += b;
    return a;
}
template<typename T>
inline SBasisOf<T>& operator-=(SBasisOf<T>& a, T b) {
    if(a.isZero())
        a.push_back(LinearOf<T>(-b,-b));
    else
        a[0] -= b;
    return a;
}


template<typename T>
SBasisOf<T> shift(SBasisOf<T> const &a, int sh){
    SBasisOf<T> c = a;
    if(sh > 0) {
        c.insert(c.begin(), sh, LinearOf<T>(0,0));
    } else {
        //TODO: truncate
    }
    return c;
}

template<typename T>
SBasisOf<T> shift(LinearOf<T> const &a, int sh) {
    SBasisOf<T> c;
    if(sh > 0) {
        c.insert(c.begin(), sh, LinearOf<T>(0,0));
        c.push_back(a);
    }
    return c;
}

template<typename T>
inline SBasisOf<T> truncate(SBasisOf<T> const &a, unsigned terms) {
    SBasisOf<T> c;
    c.insert(c.begin(), a.begin(), a.begin() + std::min(terms, (unsigned)a.size()));
    return c;
}

template<typename T>
SBasisOf<T> multiply_add(SBasisOf<T> const &a, SBasisOf<T> const &b, SBasisOf<T> c) {
    if(a.isZero() || b.isZero())
        return c;
    c.resize(a.size() + b.size(), LinearOf<T>(T(0.),T(0.)));
    for(unsigned j = 0; j < b.size(); j++) {
        for(unsigned i = j; i < a.size()+j; i++) {
            T tri = (b[j][1]-b[j][0])*(a[i-j][1]-a[i-j][0]);
            c[i+1/*shift*/] += LinearOf<T>(-tri);
        }
    }
    for(unsigned j = 0; j < b.size(); j++) {
        for(unsigned i = j; i < a.size()+j; i++) {
            for(unsigned dim = 0; dim < 2; dim++)
                c[i][dim] += b[j][dim]*a[i-j][dim];
        }
    }
    c.normalize();
    //assert(!(0 == c.back()[0] && 0 == c.back()[1]));
    return c;
}

template<typename T>
SBasisOf<T> multiply(SBasisOf<T> const &a, SBasisOf<T> const &b) {
    SBasisOf<T> c;
    if(a.isZero() || b.isZero())
        return c;
    return multiply_add(a, b, c);
}

template<typename T>
SBasisOf<T> integral(SBasisOf<T> const &c){
    SBasisOf<T> a;
    T aTri = T(0.);
    for(int k = c.size()-1; k >= 0; k--) {
        aTri = (HatOf<T>(c[k]).d + (k+1)*aTri/2)/(2*k+1);
        a[k][0] -= aTri/2;
        a[k][1] += aTri/2;
    }
    a.normalize();
    return a;
}

template<typename T>
SBasisOf<T> derivative(SBasisOf<T> const &a){
    SBasisOf<T> c;
    c.resize(a.size(), LinearOf<T>());
    if(a.isZero())
        return c;

    for(unsigned k = 0; k < a.size()-1; k++) {
        T d = (2*k+1)*(a[k][1] - a[k][0]);
        
        c[k][0] = d + (k+1)*a[k+1][0];
        c[k][1] = d - (k+1)*a[k+1][1];
    }
    int k = a.size()-1;
    T d = (2*k+1)*(a[k][1] - a[k][0]);
    //TODO: do a real test to know if d==0!
    if(d == T(0.0))
        c.pop_back();
    else {
        c[k][0] = d;
        c[k][1] = d;
    }

    return c;
}

template<typename T>
void SBasisOf<T>::derive() { // in place version
    if(isZero()) return;
    for(unsigned k = 0; k < this->size()-1; k++) {
        T d = (2*k+1)*((*this)[k][1] - (*this)[k][0]);
        
        (*this)[k][0] = d + (k+1)*(*this)[k+1][0];
        (*this)[k][1] = d - (k+1)*(*this)[k+1][1];
    }
    int k = this->size()-1;
    T d = (2*k+1)*((*this)[k][1] - (*this)[k][0]);
    if(d == 0)//TODO: give this a meaning for general coeff ring.
        this->pop_back();
    else {
        (*this)[k][0] = d;
        (*this)[k][1] = d;
    }
}


template<typename T>
inline SBasisOf<T> operator*(SBasisOf<T> const & a, SBasisOf<T> const & b) {
    return multiply(a, b);
}

template<typename T>
inline SBasisOf<T>& operator*=(SBasisOf<T>& a, SBasisOf<T> const & b) {
    a = multiply(a, b);
    return a;
}

// a(b(t))
//TODO: compose all compatibles types!
template<typename T>
SBasisOf<T> compose(SBasisOf<T> const &a, SBasisOf<T> const &b){
    SBasisOf<double> s = multiply((SBasisOf<T>(LinearOf<T>(1,1))-b), b);
    SBasisOf<T> r;

    for(int i = a.size()-1; i >= 0; i--) {
        r = multiply_add(r, s, SBasisOf<T>(LinearOf<T>(HatOf<T>(a[i][0]))) - b*a[i][0] + b*a[i][1]);
    }
    return r;
}

template<typename T>
SBasisOf<T> compose(SBasisOf<T> const &a, SBasisOf<T> const &b, unsigned k){
    SBasisOf<T> s = multiply((SBasisOf<T>(LinearOf<T>(1,1))-b), b);
    SBasisOf<T> r;

    for(int i = a.size()-1; i >= 0; i--) {
        r = multiply_add(r, s, SBasisOf<T>(LinearOf<T>(HatOf<T>(a[i][0]))) - b*a[i][0] + b*a[i][1]);
    }
    r.truncate(k);
    return r;
}
template<typename T>
SBasisOf<T> compose(LinearOf<T> const &a, SBasisOf<T> const &b){
    return compose(SBasisOf<T>(a),b);
}
template<typename T>
SBasisOf<T> compose(SBasisOf<T> const &a, LinearOf<T> const &b){
    return compose(a,SBasisOf<T>(b));
}
template<typename T>//TODO: don't be so lazy!!
SBasisOf<T> compose(LinearOf<T> const &a, LinearOf<T> const &b){
    return compose(SBasisOf<T>(a),SBasisOf<T>(b));
}



template<typename T>
inline SBasisOf<T> portion(const SBasisOf<T> &t, double from, double to) { return compose(t, LinearOf<T>(from, to)); }

// compute f(g)
template<typename T>
inline SBasisOf<T>
SBasisOf<T>::operator()(SBasisOf<T> const & g) const {
    return compose(*this, g);
}
 
template<typename T>
inline std::ostream &operator<< (std::ostream &out_file, const LinearOf<T> &bo) {
    out_file << "{" << bo[0] << ", " << bo[1] << "}";
    return out_file;
}

template<typename T>
inline std::ostream &operator<< (std::ostream &out_file, const SBasisOf<T> & p) {
    for(unsigned i = 0; i < p.size(); i++) {
        out_file << p[i] << "s^" << i << " + ";
    }
    return out_file;
}

};
#endif


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
