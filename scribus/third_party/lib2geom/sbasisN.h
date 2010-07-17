/**
 * \file
 * \brief Defines multi-dimensional S-power basis function class
 * A SBasisN<n> is a polynomial f of n variables (t0,...,tn-1), 
 * written in a particular form. Let si = ti(1-t_i). f is written as
 *
 *  f = sum_p  s^p a_{p}(t0,...,t_{n-1})
 *
 * where p=(p0,...,p_{n-1}) is a multi index (called MultiDegree<n> in the code)
 *  s^p = prod_i si^pi, and a_p is a LinearN<n>.
 * Recall a LinearN<n> is sum over all choices xi = ti or (1-ti) of terms of form
 *   a * x0*...*x_{n-1}
 *
 * Caution: degrees are expressed as degrees of s=t*(1-t). The real degree
 * (with respect to t) of the polynomial is twice that + 0 or 1 depending 
 * wether the relevant LinearN<n> coeff is constant or not.
 *
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

#ifndef SEEN_SBASISN_H
#define SEEN_SBASISN_H
#include <vector>
#include <cassert>
#include <iostream>

#include "linearN.h"
#include "linear.h" //for conversion purpose
#include "sbasis.h" //for conversion purpose
#include "interval.h"
#include "utils.h"
#include 2exception.h"


namespace Geom{

/** MultiDegree:
 *  \brief multi-degree (p0,...,p^{n-1}) of a s0^p0...s_{n-1}p^{n-1} monomial.
 *
 *  a "Multi_deg" is a sequence p={p0,...,p_n-1} representing the monomial
 *  s^p = s_0^{p_0}*...*s_{n-1}^{p_{n-1}}.
 *  Caution: the degrees are expressed with respect to si! ( in SBasis code
 *  below, si = ti*(1-ti) ).
 */

template<unsigned n>
class MultiDegree{
public:
    unsigned p[n];
    MultiDegree(){
        for (unsigned i = 0; i <n; i++) {
            p[i]=0;
        }
    }
    MultiDegree( unsigned const other_p[] ){
        p = other_p;
    }
    MultiDegree(unsigned const idx, unsigned const sizes[]){
        unsigned q = idx;
        for (unsigned i = n-1; i >0; i--) {
            div_t d = std::div(int(q), int(sizes[i]));
            p[i] = d.rem;
            q = d.quot;
        }
        p[0] = q;
    }
    unsigned operator[](const unsigned i) const {
        assert(i < n); return p[i];
    }
    unsigned& operator[](const unsigned i) {
        assert(i < n); return p[i]; 
    }

    unsigned asIdx(unsigned const sizes[]) const{
        unsigned ret = p[0];
        bool in_range = (p[0]<sizes[0]);
        for (unsigned i = 1; i < n; i++) {
            in_range = in_range && (p[i]<sizes[i]);
            ret = ret*sizes[i] + p[i];
        }
        if (in_range) return ret;
        //TODO: find a better warning than returning out of range idx!
        ret =1;
        for (unsigned i = 0; i < n; i++) {
            ret *= sizes[i];
        }
        return ret;
    }
    bool stepUp(unsigned const sizes[], unsigned frozen_mask = 0){
        unsigned i = 0;
        while ( i < n && ( (1<<i) & frozen_mask ) ) i++;
        while ( i <n && p[i] == sizes[i]-1 ) {
            i++;
            while (i<n && ( (1<<i) & frozen_mask ) ) i++;
        }
        if (i<n){
            p[i]+=1; 
            for (unsigned j = 0; j <  i; j++) {
                if ( !( (1<<j) & frozen_mask ) ) p[j] = 0;
            }
            return true; 
        }else{
            return false;
        }
    }
    bool stepDown(unsigned const sizes[], unsigned frozen_mask = 0){
        int i = n-1;
        while (i>=0 && ( (1<<i) & frozen_mask ) ) i--;
        while ( i >= 0 && p[i] == 0 ) {
            i--;
            while (i>=0 && ( (1<<i) & frozen_mask ) ) i--;
        }
        if ( i >= 0 ){
            p[i]-=1; 
            for (unsigned j = i+1; j <  n; j++) {
                if ( !( (1<<j) & frozen_mask ) ) p[j] = sizes[j]-1;
            }
            return true; 
        }else{
            return false;
        }
    }
};

/**
 * Returns the maximal degree appearing in the two arguments for each variables.
 */
template <unsigned n>
MultiDegree<n> max(MultiDegree<n> const &p, MultiDegree<n> const &q){
    MultiDegree<n> ret;
    for (unsigned i = 0; i <n; i++) {
        ret.p[i] = (p[i]>q[i] ? p[i] : q[i]);
    }
    return ret;
}

template <unsigned n>
MultiDegree<n> operator + (MultiDegree<n> const &p, MultiDegree<n> const &q){
    MultiDegree<n> ret;
    for (unsigned i = 0; i <n; i++) {
        ret.p[i] = p[i] + q[i];
    }
    return ret;
}
template <unsigned n>
MultiDegree<n> operator += (MultiDegree<n> const &p, MultiDegree<n> const &q){
    for (unsigned i = 0; i <n; i++) {
        p[i] += q[i];
    }
    return p;
}

/**
 *  \brief MultiDegree comparison.
 * A MultiDegree \param p is smaller than another \param q
 * if all it's smaller for all variables.
 *
 * In particular, p<=q and q<=p can both be false! 
 */
template<unsigned n>
bool operator<=(MultiDegree<n> const &p, MultiDegree<n> const &q){
    for (unsigned i = 0; i <n; i++) {
        if (p[i]>q[i]) return false;
    }
    return true;
}


/**
 *  \brief Polynomials of n variables, written in SBasis form.
 * An SBasisN<n> f is internaly represented as a vector of LinearN<n>.
 * It should be thought of as an n-dimensional vector: the coef of s0^p0...s_{n-1}p^{n-1}
 * is soterd in f[p0,...,p_{n-1}]. The sizes of each dimension is stored in "sizes". 
 * Note: an empty SBasis is identically 0.
 */
template<unsigned n>
class SBasisN : public std::vector<LinearN<n> >{
public:
    unsigned sizes[n];
    SBasisN() {
        for (unsigned i = 0; i < n; i++) {
            sizes[i] = 0;
        }
    }
    explicit SBasisN(double a) {
        for (unsigned i = 0; i < n; i++) {
            sizes[i] = 1;
        }
        push_back(LinearN<n>(a));
    }
    SBasisN(SBasisN<n> const & a) : std::vector<LinearN<n> >(a){
        //TODO: efficient array copy??
        for (unsigned i = 0; i < n; i++) {
            sizes[i] = a.sizes[i];
        }
    }
    SBasisN(LinearN<n> const & bo) {
        for (unsigned i = 0; i < n; i++) {
            sizes[i] = 1;
        }
        push_back(bo);
    }
    SBasisN(LinearN<n>* bo) {
        for (unsigned i = 0; i < n; i++) {
            sizes[i] = 1;
        }
        push_back(*bo); 
    }

//----------------------------------------------
//-- Degree/Sizing facilities ------------------
//----------------------------------------------
/**
 * Internal recursive function used to compute partial degrees.
 */
    bool find_non_empty_level(unsigned var, MultiDegree<n> &fixed_degrees)const{
        if (this->size()==0){
            for (unsigned i = 0; i < n; i++) {
                fixed_degrees[i] = 0;//FIXME this should be -infty!!
            }
            return false;
        }
        if ( !((*this)[fixed_degrees.asIdx(sizes)].isZero()) ) return true;

        unsigned frozen = (1<<var);
        if ( fixed_degrees.stepDown(sizes, frozen) ){
            if ( find_non_empty_level(var, fixed_degrees) ) return true;
        }
        if ( fixed_degrees[var] > 0 ){
            fixed_degrees[var] -= 1;
            for (unsigned i = 0; i < n; i++) {
                if (i!=var) fixed_degrees[i] = sizes[i]-1;
            }
            if (find_non_empty_level(var, fixed_degrees)) return true;
        }
        return false;//FIXME: this shoud return -infty in all variables!
    }

/**
 * Returns the degree of an SBasisN<n> with respect to a given variable form its sizes.
 * All terms are taken into account, even eventual trailing zeros.
 * Note: degree is expressed with respect to s = t*(1-t), not t itself.
 */
    unsigned quick_degree(unsigned var) const{
        return ( sizes[var] > 0 ? sizes[var]-1 : 0 );//this should be -infty.
     }
/**
 * Computes the multi degree of the SBasis from it's sizes.
 * All terms are taken into account, even eventual trailing zeros.
 * Note: degrees are expressed with respect to s = t*(1-t), not t itself.
 */
    MultiDegree<n> quick_multi_degree() const{
        MultiDegree<n> ret;
        if (this->size()==0) return ret;//should be -infty for all vars.
        for (unsigned i = 0; i < n; i++) {
            assert( sizes[i]>0 );
            ret.p[i] = sizes[i]-1;
       }
        return ret;
    }
/**
 * Returns the degree of an SBasisN<n> with respect to a given variable.
 * Trailing zeros are not taken into account.
 * Note: degree is expressed with respect to s = t*(1-t), not t itself.
 */
    unsigned degree(unsigned var)const{
        MultiDegree<n> degrees;
        for(unsigned i = 0; i < n; i++) {
            degrees[i] = sizes[i]-1;
        }
        if ( find_non_empty_level(var, degrees) ) return degrees[var];
        else return 0;//should be -infty.
     }
/**
 * Returns the *real* degree of an SBasisN<n> with respect to a given variable.
 * Trailing zeros are not taken into account.
 * Note: degree is expressed with respect to t itself, not s = t*(1-t).
 * In particular: real_t_degree() = 2*degree() + 0 or 1.
 */
    unsigned real_t_degree(unsigned var)const{
        unsigned deg = 0;
        bool even = true;
        bool notfound = true;
        unsigned frozen = (1<<var);
        MultiDegree<n> degrees;
        for(unsigned i = 0; i < n; i++) {
            degrees[i] = sizes[i]-1;
        }
        while( notfound ){
            if ( find_non_empty_level(var, degrees) && degrees[var]>= deg ){
                deg = degrees[var];
                even = (*this)[degrees.asIdx(sizes)].isConstant(var);
            }
            notfound =  even && degrees.stepDown(sizes, frozen);
        }
        return 2*deg + ( even ? 0 : 1 );
     }
/**
 * Returns the *real* degrees of an SBasisN<n>.
 * Trailing zeros are not taken into account.
 * Note: degree is expressed with respect to t itself, not s = t*(1-t).
 * In particular: real_t_degree() = 2*degree() + 0 or 1.
 */
    MultiDegree<n> real_t_degrees()const{
        MultiDegree<n>res;
        for(unsigned i = 0; i < n; i++) {
            res[i] = real_t_degree(i);
        }
        return res;
     }
/**
 * Computes the multi degree of the SBasis.
 * Trailing zeros are not taken into account.
 * Note: degree is expressed with respect to s = t*(1-t), not t itself.
 */
    MultiDegree<n> multi_degree() const{
        MultiDegree<n> ret;
        if (this->size()==0) return ret;//should be -infty for all vars.
        for (unsigned i = 0; i < n; i++) {
            ret[i] = this->degree(i);
        }
        return ret;
    }
/**
 * Returns the highest degree over all variables.
 * Note: degree is expressed with respect to s = t*(1-t), not t itself.
 */
    unsigned max_degree() const {
        if (this->size()==0) return 0;//should be -infty!
        unsigned d=0;
        for (unsigned i = 0; i < n; i++) {
            assert( sizes[i]>0 );
            if (d < sizes[i]-1) d = sizes[i]-1;
        }
        return d;
    }

/**
 * Resize an SBasisN<n> to match new sizes.
 *
 * Caution: if a new size is smaller, the coresponding coefficients are discarded.
 */
    void multi_resize(unsigned new_sizes[], LinearN<n> def_value = LinearN<n>(0.)){
        SBasisN<n> result;
        bool nothing_todo = true;
        unsigned tot_size = 1;
        for(unsigned i = 0; i < n; i++) {
            nothing_todo = nothing_todo && (sizes[i] == new_sizes[i]);
            result.sizes[i] = new_sizes[i];
            tot_size *= new_sizes[i];
        }
        if (nothing_todo) return;
        result.resize(tot_size, def_value);
        for(unsigned i = 0; i < tot_size; i++) {
            MultiDegree<n> d( i, result.sizes );
            unsigned j = d.asIdx(sizes);
            if ( j < this->size() ){
                result[i] = (*this)[j];
            }
        }
        *this = result;
    }

    //remove extra zeros
    void normalize() {
        MultiDegree<n> max_p = multi_degree();
        unsigned new_sizes[n];
        for (unsigned i=0; i<n; i++){
            new_sizes[i] = max_p[i]+1;
        }
        multi_resize(new_sizes);
    }

//-----------------------------
//-- Misc. --------------------
//-----------------------------

/**
 * Returns the number of variables this function takes as input: n.
 */
    unsigned input_dim(){return n;};

    //IMPL: FragmentConcept
    typedef double output_type;

    inline bool isZero() const {
        if(this->size()==0) return true;
        for(unsigned i = 0; i < this->size(); i++) {
            if(!(*this)[i].isZero()) return false;
        }
        return true;
    }
    inline bool isConstant() const {
        if (this->size()==0) return true;
        if(!(*this)[0].isConstant()) return false;
        for (unsigned i = 1; i < this->size(); i++) {
            if(!(*this)[i].isZero()) return false;
        }
        return true;
    }

    bool isFinite() const{
        for (unsigned i = 0; i < this->size(); i++) {
            if(!(*this)[i].isFinite()) return false;
        }
        return true;
    }


//------------------------------------------
//-- Evaluation methods --------------------
//------------------------------------------
/**
 * Returns the value of the SBasis at a given corner of [0,1]^n.
 * \param k describes the corner: if i-th bit is 0, ti=0, otherwise ti=1. 
 */
    inline double atCorner(unsigned k) const { 
        if(this->size()==0) return 0.;
        return (*this)[0].atCorner(k);
    }
/**
 * Returns the value of the SBasis at a given corner of [0,1]^n.
 * \param t[n] describes the corner: the values should be 0's and 1's. 
 */
    inline double atCorner(double t[]) const { 
        if(this->size()==0) return 0.;
        return (*this)[0].atCorner(t);
    }
/**
 * Returns a "slice" of the array.
 * Returns an SBasis containing all the coeff of (s-)degree \param deg in variable \param var
 */
    //TODO: move by bigger blocks (?) but they are broken into pieces in general...
    SBasisN<n> slice(unsigned const var, unsigned const deg) const{
        if (deg >= sizes[var] ) return SBasisN<n>();
        SBasisN<n> res;
        unsigned tot_size = 1;
        for (unsigned i = 0; i < n; i++) {
            res.sizes[i] = (i==var ? 1 : sizes[i]);
            tot_size *= res.sizes[i];
        }
        res.resize( tot_size, LinearN<n>(0.));
        for (unsigned i = 0; i < tot_size; i++) {
            MultiDegree<n> d(i,res.sizes);
            d.p[var] = deg;
            res[i] = (*this)[d.asIdx(sizes)];
        }
        return res;
    }
/**
 * Returns a the SBasisN<n-1> obtained by setting variable \param var to 0.
 */
    inline SBasisN<n-1> at0(unsigned var=0, unsigned deg=0) const {
        SBasisN<n> sl = slice(var,deg);
        SBasisN<n-1> res;
        res.reserve(sl.size());
        for (unsigned i = 0; i < n-1; i++) {
            res.sizes[i] = sizes[ ( i<var ? i : i+1 ) ];
        }
        for (unsigned i = 0; i < sl.size(); i++) {
            res.push_back( sl[i].at0(var) );
        }
        return res;
    }
/**
 * Returns a the SBasisN<n-1> obtained by setting variable \param var to 1.
 */
    inline SBasisN<n-1> at1(unsigned var=0, unsigned deg=0) const {
        SBasisN<n> sl = slice(var,deg);
        SBasisN<n-1> res;
        res.reserve(sl.size());
        for (unsigned i = 0; i < n-1; i++) {
            res.sizes[i] = sizes[ ( i<var ? i : i+1 ) ];
        }
        for (unsigned i = 0; i < sl.size(); i++) {
            res.push_back( sl[i].at1(var) );
        }
        return res;
    }
/**
 * Returns a the SBasisN<n-1> obtained by setting variable \param var to \param t.
 */
    inline SBasisN<n-1> partialEval(double t, unsigned var=0 ) const {
        SBasisN<n> sl;
        double s = t*(1-t);
        double si = 1;
        for (unsigned i = 0; i <sizes[var]; i++) {
            sl = sl + slice(var, i)*si;
            si *= s;
        }
        SBasisN<n-1> res;
        res.resize(sl.size(), LinearN<n-1>(0.));
        for (unsigned i = 0; i < n-1; i++) {
            res.sizes[i] = sizes[ ( i<var ? i : i+1 ) ];
        }
        for (unsigned i = 0; i < sl.size(); i++) {
            res[i] = sl[i].partialEval(t,var);
        }
        return res;
    }

/**
 * \brief Internal recursive function.
 * Replace each variable  by it's value in the 's=t*(1-t)' factor 
 * but not in the LinearN<n> coeffs. Then sum up all coefficients.
 * \param t[n]: values of the variables.
 */
    LinearN<n> sumCoefs( double t[], unsigned const k, unsigned const idx) const{
        LinearN<n> a;
        if (k == n){
            a = (*this)[idx];
            return (*this)[idx];
        }
        double s = t[k]*(1-t[k]);
        double si=1;
        for (unsigned i=0; i<sizes[k]; i++){
            a += sumCoefs(t,k+1,idx*sizes[k]+i)*si;;
            si *= s;
        }
        return a;
    }
/**
 * Evaluate at given n-dimensional point.
 * \param t[n]: values of the variables.
 */
    double valueAt(double t[]) const {
        LinearN<n> a = sumCoefs(t,0,0);
        return a.valueAt(t);
    }

    double operator()(double t[]) const {
        return valueAt(t);
    }

    //double valueAndDerivative(double t, double &der) const;
    //std::vector<double> valueAndDerivatives(double t, unsigned n) const;
    //SBasisN toSBasisN() const { return SBasisN(*this); }
    //double tailError(unsigned tail) const;


//--------------------------------------------------
//-- Coeff. manipulation ---------------------------
//--------------------------------------------------

/**
 * Accessing the SBasisN<n> coefficents.
 */
    LinearN<n> operator[](unsigned i) const {
        assert(i < this->size());
        return std::vector<LinearN<n> >::operator[](i);
    }
    LinearN<n> operator[](MultiDegree<n> const &p) const {
        unsigned i = p.asIdx(sizes);
        assert(i < this->size());        
        return std::vector<LinearN<n> >::operator[](i);
    }

//MUTATOR PRISON
    LinearN<n>& operator[](unsigned i) { return this->at(i); }
//    LinearN<n>& operator[](MultiDegree const &p) { 
//        unsigned i = p.asIdx(sizes);
//        return this->at(i); 
//    }

    void appendCoef(const SBasisN<n-1> &a, const SBasisN<n-1> &b, unsigned var=0){
        unsigned new_sizes[n];
        MultiDegree<n-1> deg_a = a.multi_degree(), deg_b = b.multi_degree();
        MultiDegree<n-1> dcoef = max( deg_a, deg_b );
        for (unsigned i=0; i<n; i++){
            if ( i == var ){
                new_sizes[var] = sizes[var] + 1;
            }else{
                unsigned coef_size = dcoef[(i<var?i:i-1)] + 1;
                new_sizes[i] = ( sizes[i]>coef_size ? sizes[i] : coef_size );
            }
        }
        multi_resize(new_sizes);
        
        MultiDegree<n> d;
        d[var] = sizes[var]-1;
        unsigned frozen_mask = (1<<var);
        do{
            for (unsigned i=0; i<n-1; i++){
                dcoef.p[i] = d.p[ ( i<var ? i : i+1) ];
            }
            LinearN<n-1> a_d,b_d;
            unsigned ia = dcoef.asIdx(a.sizes);
            if ( ia < a.size() ) a_d = a[ia];
            unsigned ib = dcoef.asIdx(b.sizes);
            if ( ib < b.size() ) b_d = b[ib];
            (*this)[d.asIdx(sizes)] = LinearN<n>(a_d,b_d);
        }while (d.stepUp(sizes,frozen_mask));
    }

//private:
    //void derive(); // in place version
};

//SBasisN<0> is a double. Specialize it out.
template<>
class SBasisN<0>{
public:
    double d;
    SBasisN () {}
    SBasisN(double d) :d(d) {}
    operator double() const { return d; }
};


//SBasisN<1> are usual SBasis. Allow conversion.
SBasis toSBasis(SBasisN<1> f){
    SBasis res(f.size(), Linear());
    for (unsigned i = 0; i < f.size(); i++) {
        res[i] = toLinear(f[i]);
    }
    return res;
}

//TODO: figure out how to stick this in linear, while not adding an sbasis dep
template<unsigned n>
inline SBasisN<n> LinearN<n>::toSBasisN() const { return SBasisN<n>(*this); }




//implemented in sbasis-roots.cpp
//OptInterval bounds_exact(SBasisN const &a);
//OptInterval bounds_fast(SBasisN const &a, int order = 0);
//OptInterval bounds_local(SBasisN const &a, const OptInterval &t, int order = 0);

/** Returns a function which reverses the domain of a.
 \param a sbasis function

useful for reversing a parameteric curve.
*/
//template<unsigned n>
//inline SBasisN<n> reverse(SBasisN<n> const &a);

//IMPL: ScalableConcept
template<unsigned n>
inline SBasisN<n> operator-(const SBasisN<n>& p) {
    if(p.isZero()) return SBasisN<n>();
    SBasisN<n> result;
    for(unsigned i = 0; i < n; i++) {
        result.sizes[i] = p.sizes[i];
    }
    result.reserve(p.size());
    for(unsigned i = 0; i < p.size(); i++) {
        result.push_back(-p[i]);
    }
    return result;
}
template<unsigned n>
SBasisN<n> operator*(SBasisN<n> const &a, double c){
    if(a.isZero()) return SBasisN<n>();
    SBasisN<n> result;
    for(unsigned i = 0; i < n; i++) {
        result.sizes[i] = a.sizes[i];
    }
    result.reserve(a.size());
    for(unsigned i = 0; i < a.size(); i++) {
        result.push_back(a[i] * c);
    }
    return result;
}
template<unsigned n>
inline SBasisN<n> operator*(double k, SBasisN<n> const &a) { return a*k; }
template<unsigned n>
inline SBasisN<n> operator/(SBasisN<n> const &a, double k) { return a*(1./k); }
template<unsigned n>
SBasisN<n>& operator*=(SBasisN<n>& a, double c){
    for(unsigned i = 0; i < a.size(); i++) a[i] *= c;
    return a;
}
template<unsigned n>
inline SBasisN<n>& operator/=(SBasisN<n>& a, double b) { return (a*=(1./b)); }

//IMPL: AddableConcept
template<unsigned n>
SBasisN<n> operator + (const SBasisN<n>& a, const SBasisN<n>& b){
    if( a.isZero() ) return b;
    if( b.isZero() ) return a;
    SBasisN<n> result;
    MultiDegree<n> deg = max(a.quick_multi_degree(),b.quick_multi_degree());
    unsigned max_size = 1;
    for(unsigned i = 0; i < n; i++) {
        result.sizes[i] = deg[i]+1;
        max_size *= result.sizes[i];
    }
    result.resize( max_size, LinearN<n>(0.) );
    for(unsigned i = 0; i < result.size(); i++) {
        MultiDegree<n> p(i,result.sizes);
        unsigned ia = p.asIdx(a.sizes);
        unsigned ib = p.asIdx(b.sizes);
        if (ia<a.size()) {
            result[i] += a[ia];
        }
        if (ib<b.size()) {
            result[i] += b[ib];
        }
    }
    return result;
}
template<unsigned n>
SBasisN<n> operator-(const SBasisN<n>& a, const SBasisN<n>& b){return a+(-b);}
template<unsigned n>
SBasisN<n>& operator+=(SBasisN<n>& a, const SBasisN<n>& b){
    if(b.isZero()) return a;
    a = a + b;
    return a;
}
template<unsigned n>
SBasisN<n>& operator-=(SBasisN<n>& a, const SBasisN<n>& b){
    a += -b;
    return a;
}

//TODO: remove?
template<unsigned n>
inline SBasisN<n> operator+(const SBasisN<n> & a, LinearN<n> const & b) {
    if(b.isZero()) return a;
    if(a.isZero()) return b;
    SBasisN<n> result(a);
    result[0] += b;
    return result;
}
template<unsigned n>
inline SBasisN<n> operator-(const SBasisN<n> & a, LinearN<n> const & b) {
    if(b.isZero()) return a;
    if(a.isZero()) return -b;
    SBasisN<n> result(a);
    result[0] -= b;
    return result;
}
template<unsigned n>
inline SBasisN<n>& operator+=(SBasisN<n>& a, const LinearN<n>& b) {
    if(a.size()==0)
        a.push_back(b);
    else
        a[0] += b;
    return a;
}
template<unsigned n>
inline SBasisN<n>& operator-=(SBasisN<n>& a, const LinearN<n>& b) {
    if(a.size()==0)
        a.push_back(-b);
    else
        a[0] -= b;
    return a;
}

//IMPL: OffsetableConcept
template<unsigned n>
inline SBasisN<n> operator+(const SBasisN<n> & a, double b) {
    if(a.isZero()) return LinearN<n>(b);
    SBasisN<n> result(a);
    result[0] += b;
    return result;
}
template<unsigned n>
inline SBasisN<n> operator-(const SBasisN<n> & a, double b) {
    if(a.isZero()) return LinearN<n>(-b);
    SBasisN<n> result(a);
    result[0] -= b;
    return result;
}
template<unsigned n>
inline SBasisN<n>& operator+=(SBasisN<n>& a, double b) {
    if(a.size()==0)
        a.push_back(LinearN<n>(b));
    else
        a[0] += b;
    return a;
}
template<unsigned n>
inline SBasisN<n>& operator-=(SBasisN<n>& a, double b) {
    if(a.size()==0)
        a.push_back(LinearN<n>(-b));
    else
        a[0] -= b;
    return a;
}

template<unsigned n>
SBasisN<n> shift(SBasisN<n> const &a, MultiDegree<n> sh){
    SBasisN<n> result;
    MultiDegree<n> deg = a.quick_multi_degree() + sh;
    for(unsigned i = 0; i < n; i++) {
        result.sizes[i] = deg[i]+1;
    }
    unsigned max_size = deg.asIdx(result.sizes);
    result.resize( max_size, LinearN<n>(0.) );
    for(unsigned i = 0; i < a.size(); i++) {
        MultiDegree<n> p(i,a.sizes);
        p+=sh;
        result[p.asIdx(result.sizes)]=a[i];
    }
    return result;
}
template<unsigned n>
SBasisN<n> shift(LinearN<n> const &a, MultiDegree<n> sh){
    SBasisN<n> result;
    for(unsigned i = 0; i < n; i++) {
        result.sizes[i] = sh[i]+1;
    }
    unsigned max_size = sh.asIdx(result.sizes);
    result.resize( max_size, LinearN<n>(0.) );
    result[max_size-1]=a;
    return result;
}
//shift only in one variable
template<unsigned n>
SBasisN<n> shift(LinearN<n> const &a, unsigned sh, unsigned var){
    assert( var < n );
    SBasisN<n> result;
    for(unsigned i = 0; i < n; i++) {
        result.sizes[i] = 1;
    }
    result.sizes[var] = sh+1;
    result.resize( sh+1, LinearN<n>(0.) );
    result[sh]=a;
    return result;
}

//truncate only in first variable
template<unsigned n>
inline SBasisN<n> truncate(SBasisN<n> const &a, unsigned first_size) {
    if ( first_size <= a.sizes[0] ) return a;
    SBasisN<n> c;
    for (unsigned i = 0; i < n; i++) {
        c.sizes[i] = a.sizes[i];
    }
    c.sizes[0] = first_size;
    unsigned tot_size = 1;
    for(unsigned i = 0; i < n; i++) {
        tot_size*=c.sizes[i];
    }
    c.insert(c.begin(), a.begin(), a.begin() + tot_size);
    return c;
}

template<unsigned n>
SBasisN<n> multiply(SBasisN<n> const &a, SBasisN<n> const &b){
    SBasisN<n> c;
    MultiDegree<n> d;
    MultiDegree<n> t_deg = a.real_t_degrees() + b.real_t_degrees();
    for(unsigned i = 0; i < n; i++) {
        d[i] = ( t_deg[i]%2 == 0 ? t_deg[i]/2 : (t_deg[i]-1)/2 ) ;
    }
    unsigned new_sizes[n], tot_size = 1;
    for(unsigned i = 0; i < n; i++) {
        //c.sizes[i] = d[i] + 1+1;//product of linears might give 1 more s in each dir!!
        new_sizes[i] = d[i] + 1;
        tot_size*=new_sizes[i];
    }
    c.resize( tot_size, LinearN<n>(0.) );
    for(unsigned i = 0; i < n; i++) {
        c.sizes[i] = new_sizes[i];
    }

    for(unsigned ai = 0; ai < a.size(); ai++) {
        for(unsigned bj = 0; bj < b.size(); bj++) {
            MultiDegree<n> di( ai, a.sizes ); 
            MultiDegree<n> dj( bj, b.sizes ); 
            //compute a[ai]*b[bj]:
            for(unsigned p = 0; p < (1<<n); p++) {
                for(unsigned q = 0; q < (1<<n); q++) {

                    //compute a[ai][p]*b[bj][q]:
                    unsigned m = p^q;//m has ones for factors s, 0 for (t-s) or ((1-t)-s).
                    for(unsigned r = 0; r < (1<<n); r++) {
                        if (!(r&m)) {// a 1 in r means take t (or (1-t)), otherwise take -s.
                            int sign = 1;
                            MultiDegree<n> dr; 
                            unsigned t0 = 0, t1 = 0;
                            for (unsigned var = 0; var < n; var++) {
                                //if var is in mask m, no choice, take s
                                if ( m & (1<<var) ){
                                    dr.p[var] = 1;
                                }//if var is in mask r, take t or (1-t)
                                else  if ( r & (1<<var) ){
                                    dr.p[var] = 0;
                                    if ( p&(1<<var) ) {
                                        t0 = t0 | (1<<var);
                                    }else{
                                        t1 = t1 | (1<<var);
                                    }
                                }//ohterwise take -s
                                else{
                                    dr.p[var] = 1;
                                    sign *= -1;
                                }
                            }
                            unsigned idx = (di+dj+dr).asIdx(c.sizes);
                            if (idx < c.size()){
                                for(unsigned s = 0; s < (1<<n); s++) {
                                    if ( (t0 & ~s) || (t1 & s) ){
                                        c[idx][s] += 0;
                                    }else{
                                        c[idx][s] += sign * a[ai][p] * b[bj][q];
                                    }
                                }
                            }
                        }
                    }//r loop: all choices have been expanded in the product a[ai][p]*b[bj][q]
                }//q loop
            }//p loop: all products a[ai][p]*b[bj][q] have been computed.
        }//bj loop
    }//ai loop: all a[ai]b[bj] have been computed.

    //TODO: normalize c, or even better, compute with the right size from scratch
    return c;
}


template<unsigned n>
inline SBasisN<n> operator*(SBasisN<n> const & a, SBasisN<n> const & b) {
    return multiply(a, b);
}

template<unsigned n>
inline SBasisN<n>& operator*=(SBasisN<n>& a, SBasisN<n> const & b) {
    a = multiply(a, b);
    return a;
}

template<unsigned m,unsigned n>
SBasisN<m> compose(LinearN<n> const &f, std::vector<SBasisN<m> > const &t, unsigned fixed=0, unsigned flags=0 ){
    assert (t.size() == n );
    if (fixed == n) {
        return SBasisN<m>(1.) * f[flags]; 
    }else{
        SBasisN<m> a0 = compose(f, t, fixed+1, flags);
        SBasisN<m> a1 = compose(f, t, fixed+1, flags|(1<<fixed));
        return (-t[fixed]+1) * a0 + t[fixed] * a1;
    }
}

template<unsigned m,unsigned n>
SBasisN<m> compose(SBasisN<n> const &f, std::vector<SBasisN<m> > const &t, unsigned const k=0, unsigned const idx = 0){
    assert (t.size() == n );
    if (k == n){
        return compose( f[idx], t);
    }
    SBasisN<m> a;
    SBasisN<m> s = multiply( t[k], (-t[k]+1.) );
    SBasisN<m> si= SBasisN<m>(1.);
    for (unsigned i=0; i<f.sizes[k]; i++){
        a += compose(f, t,k+1,idx*f.sizes[k]+i)*si;;
        si *= s;
    }
    return a;
}

template <unsigned n>
inline std::ostream &operator<< (std::ostream &out_file, const MultiDegree<n> & d) {
    out_file << "s^{";
    for(unsigned i = 0; i < n; i++) {
        out_file << d[i] << (i == n-1 ? "}" : ",");        
    }
    return out_file;
}
template <unsigned n>
inline std::ostream &operator<< (std::ostream &out_file, const SBasisN<n> & p) {
    for(unsigned i = 0; i < p.size(); i++) {
        MultiDegree<n> d(i, p.sizes);
        out_file << d << " " << p[i] << " + ";
    }
    return out_file;
}


//--------------------------------------------------
//--------------------------------------------------
//--------------------------------------------------
//--------------------------------------------------
//--------------------------------------------------

#if 0


// This performs a multiply and accumulate operation in about the same time as multiply.  return a*b + c
template<unsigned n>
SBasisN<n> multiply_add(SBasisN<n> const &a, SBasisN<n> const &b, SBasisN<n> c);

template<unsigned n>
SBasisN<n> integral(SBasisN<n> const &c);
template<unsigned n>
SBasisN<n> derivative(SBasisN<n> const &a);

template<unsigned n>
SBasisN<n> sqrt(SBasisN<n> const &a, int k);

// return a kth order approx to 1/a)
template<unsigned n>
SBasisN<n> reciprocal(LinearN<n> const &a, int k);
template<unsigned n>
SBasisN<n> divide(SBasisN<n> const &a, SBasisN<n> const &b, int k);


/** Returns the degree of the first non zero coefficient.
 \param a sbasis function
 \param tol largest abs val considered 0
 \returns first non zero coefficient
*/
template<unsigned n>
inline unsigned 
valuation(SBasisN<n> const &a, double tol=0){
    unsigned val=0;
    while( val<a.size() &&
           fabs(a[val][0])<tol &&
           fabs(a[val][1])<tol ) 
        val++;
    return val;
}

// a(b(t))
template<unsigned n>
SBasisN<n> compose(SBasisN<n> const &a, SBasisN<n> const &b);
template<unsigned n>
SBasisN<n> compose(SBasisN<n> const &a, SBasisN<n> const &b, unsigned k);
template<unsigned n>
SBasisN<n> inverse(SBasisN<n> a, int k);
//compose_inverse(f,g)=compose(f,inverse(g)), but is numerically more stable in some good cases...
//TODO: requires g(0)=0 & g(1)=1 atm. generalization should be obvious.
template<unsigned n>
SBasisN<n> compose_inverse(SBasisN<n> const &f, SBasisN<n> const &g, unsigned order=2, double tol=1e-3);

/** Returns the sbasis on domain [0,1] that was t on [from, to]
 \param a sbasis function
 \param from,to interval
 \returns sbasis

*/
template<unsigned n>
inline SBasisN<n> portion(const SBasisN<n> &t, double from, double to) { return compose(t, LinearN<n>(from, to)); }

// compute f(g)
template<unsigned n>
inline SBasisN<n>
SBasisN<n>::operator()(SBasisN<n> const & g) const {
    return compose(*this, g);
}
 
template<unsigned n>
inline std::ostream &operator<< (std::ostream &out_file, const LinearN<n> &bo) {
    out_file << "{" << bo[0] << ", " << bo[1] << "}";
    return out_file;
}

template<unsigned n>
inline std::ostream &operator<< (std::ostream &out_file, const SBasisN<n> & p) {
    for(unsigned i = 0; i < p.size(); i++) {
        out_file << p[i] << "s^" << i << " + ";
    }
    return out_file;
}

// These are deprecated, use sbasis-math.h versions if possible
template<unsigned n>
SBasisN<n> sin(LinearN<n> bo, int k);
template<unsigned n>
SBasisN<n> cos(LinearN<n> bo, int k);

template<unsigned n>
std::vector<double> roots(SBasisN<n> const & s);
template<unsigned n>
std::vector<std::vector<double> > multi_roots(SBasisN<n> const &f,
                                 std::vector<double> const &levels,
                                 double htol=1e-7,
                                 double vtol=1e-7,
                                 double a=0,
                                 double b=1);
    
#endif
//--------------------------------------------------
//--------------------------------------------------
//--------------------------------------------------
//--------------------------------------------------
//--------------------------------------------------
}

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
#endif
