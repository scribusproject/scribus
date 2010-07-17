
/*
 * Find intersecions between two Bezier curves.
 * The intersection points are found by using Bezier clipping.
 *
 * Authors:
 *      Marco Cecchetti <mrcekets at gmail.com>
 *
 * Copyright 2008  authors
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





#include <basic-intersection.h>
#include <bezier.h>
#include <interval.h>
#include <convex-cover.h>


#include <vector>
#include <utility>
#include <iomanip>


namespace Geom {

namespace detail { namespace bezier_clipping {


////////////////////////////////////////////////////////////////////////////////
// for debugging
//

inline
void print(std::vector<Point> const& cp)
{
    for (size_t i = 0; i < cp.size(); ++i)
        std::cerr << i << " : " << cp[i] << std::endl;
}

template< class charT >
inline
std::basic_ostream<charT> &
operator<< (std::basic_ostream<charT> & os, const Interval & I)
{
    os << "[" << I.min() << ", " << I.max() << "]";
    return os;
}

inline
double angle (std::vector<Point> const& A)
{
    size_t n = A.size() -1;
    double a = std::atan2(A[n][Y] - A[0][Y], A[n][X] - A[0][X]);
    return (180 * a / M_PI);
}

inline
size_t get_precision(Interval const& I)
{
    double d = I.extent();
    double e = 1, p = 1;
    size_t n = 0;
    while (n < 16 && (are_near(d, 0, e)))
    {
        p *= 10;
        e = 1 /p;
        ++n;
    }
    return n;
}

////////////////////////////////////////////////////////////////////////////////


/*
 * return true if all the Bezier curve control points are near,
 * false otherwise
 */
inline
bool is_constant(std::vector<Point> const& A, double precision = EPSILON)
{
    for (unsigned int i = 1; i < A.size(); ++i)
    {
        if(!are_near(A[i], A[0], precision))
            return false;
    }
    return true;
}

/*
 *  Make up an orientation line using the control points c[i] and c[j]
 *  the line is returned in the output parameter "l" in the form of a 3 element
 *  vector : l[0] * x + l[1] * y + l[2] == 0; the line is normalized.
 */
inline
void orientation_line (std::vector<double> & l,
                       std::vector<Point> const& c,
                       size_t i, size_t j)
{
    l[0] = c[j][Y] - c[i][Y];
    l[1] = c[i][X] - c[j][X];
    l[2] = cross(c[i], c[j]);
    double length = std::sqrt(l[0] * l[0] + l[1] * l[1]);
    assert (length != 0);
    l[0] /= length;
    l[1] /= length;
    l[2] /= length;
}

/*
 * Pick up an orientation line for the Bezier curve "c" and return it in
 * the output parameter "l"
 */
inline
void pick_orientation_line (std::vector<double> & l,
                            std::vector<Point> const& c)
{
    size_t i = c.size();
    while (--i > 0 && are_near(c[0], c[i]))
    {}
    if (i == 0)
    {
        // this should never happen because when a new curve portion is created
        // we check that it is not constant;
        // however this requires that the precision used in the is_constant
        // routine has to be the same used here in the are_near test
        assert(i != 0);
    }
    orientation_line(l, c, 0, i);
    //std::cerr << "i = " << i << std::endl;
}

/*
 *  Compute the signed distance of the point "P" from the normalized line l
 */
inline
double distance (Point const& P, std::vector<double> const& l)
{
    return l[X] * P[X] + l[Y] * P[Y] + l[2];
}

/*
 * Compute the min and max distance of the control points of the Bezier
 * curve "c" from the normalized orientation line "l".
 * This bounds are returned through the output Interval parameter"bound".
 */
inline
void fat_line_bounds (Interval& bound,
                      std::vector<Point> const& c,
                      std::vector<double> const& l)
{
    bound[0] = 0;
    bound[1] = 0;
    double d;
    for (size_t i = 0; i < c.size(); ++i)
    {
        d = distance(c[i], l);
        if (bound[0] > d)  bound[0] = d;
        if (bound[1] < d)  bound[1] = d;
    }
}

/*
 * return the x component of the intersection point between the line
 * passing through points p1, p2 and the line Y = "y"
 */
inline
double intersect (Point const& p1, Point const& p2, double y)
{
    // we are sure that p2[Y] != p1[Y] because this routine is called
    // only when the lower or the upper bound is crossed
    double dy = (p2[Y] - p1[Y]);
    double s = (y - p1[Y]) / dy;
    return (p2[X]-p1[X])*s + p1[X];
}

/*
 * Clip the Bezier curve "B" wrt the fat line defined by the orientation
 * line "l" and the interval range "bound", the new parameter interval for
 * the clipped curve is returned through the output parameter "dom"
 */
void clip (Interval& dom,
           std::vector<Point> const& B,
           std::vector<double> const& l,
           Interval const& bound)
{
    double n = B.size() - 1;  // number of sub-intervals
    std::vector<Point> D;     // distance curve control points
    D.reserve (B.size());
    double d;
    for (size_t i = 0; i < B.size(); ++i)
    {
        d = distance (B[i], l);
        D.push_back (Point(i/n, d));
    }
    //print(D);
    ConvexHull chD(D);
    std::vector<Point> & p = chD.boundary; // convex hull vertices

    //print(p);

    bool plower, phigher;
    bool clower, chigher;
    double t, tmin = 1, tmax = 0;
    //std::cerr << "bound : " << bound << std::endl;

    plower = (p[0][Y] < bound.min());
    phigher = (p[0][Y] > bound.max());
    if (!(plower || phigher))  // inside the fat line
    {
        if (tmin > p[0][X])  tmin = p[0][X];
        if (tmax < p[0][X])  tmax = p[0][X];
        //std::cerr << "0 : inside " << p[0]
        //          << " : tmin = " << tmin << ", tmax = " << tmax << std::endl;
    }

    for (size_t i = 1; i < p.size(); ++i)
    {
        clower = (p[i][Y] < bound.min());
        chigher = (p[i][Y] > bound.max());
        if (!(clower || chigher))  // inside the fat line
        {
            if (tmin > p[i][X])  tmin = p[i][X];
            if (tmax < p[i][X])  tmax = p[i][X];
            //std::cerr << i << " : inside " << p[i]
            //          << " : tmin = " << tmin << ", tmax = " << tmax
            //          << std::endl;
        }
        if (clower != plower)  // cross the lower bound
        {
            t = intersect(p[i-1], p[i], bound.min());
            if (tmin > t)  tmin = t;
            if (tmax < t)  tmax = t;
            plower = clower;
            //std::cerr << i << " : lower " << p[i]
            //          << " : tmin = " << tmin << ", tmax = " << tmax
            //          << std::endl;
        }
        if (chigher != phigher)  // cross the upper bound
        {
            t = intersect(p[i-1], p[i], bound.max());
            if (tmin > t)  tmin = t;
            if (tmax < t)  tmax = t;
            phigher = chigher;
            //std::cerr << i << " : higher " << p[i]
            //          << " : tmin = " << tmin << ", tmax = " << tmax
            //          << std::endl;
        }
    }

    // we have to test the closing segment for intersection
    size_t last = p.size() - 1;
    clower = (p[0][Y] < bound.min());
    chigher = (p[0][Y] > bound.max());
    if (clower != plower)  // cross the lower bound
    {
        t = intersect(p[last], p[0], bound.min());
        if (tmin > t)  tmin = t;
        if (tmax < t)  tmax = t;
        //std::cerr << "0 : lower " << p[0]
        //          << " : tmin = " << tmin << ", tmax = " << tmax << std::endl;
    }
    if (chigher != phigher)  // cross the upper bound
    {
        t = intersect(p[last], p[0], bound.max());
        if (tmin > t)  tmin = t;
        if (tmax < t)  tmax = t;
        //std::cerr << "0 : higher " << p[0]
        //          << " : tmin = " << tmin << ", tmax = " << tmax << std::endl;
    }

    dom[0] = tmin;
    dom[1] = tmax;
}

/*
 *  Compute the portion of the Bezier curve "B" wrt the interval "I"
 */
void portion (std::vector<Point> & B, Interval const& I)
{
    Bezier::Order bo(B.size()-1);
    Bezier Bx(bo), By(bo);
    for (size_t i = 0; i < B.size(); ++i)
    {
        Bx[i] = B[i][X];
        By[i] = B[i][Y];
    }
    Bx = portion(Bx, I.min(), I.max());
    By = portion(By, I.min(), I.max());
    assert (Bx.size() == By.size());
    B.resize(Bx.size());
    for (size_t i = 0; i < Bx.size(); ++i)
    {
        B[i][X] = Bx[i];
        B[i][Y] = By[i];
    }
}

/*
 * Map the sub-interval I in [0,1] into the interval J and assign it to J
 */
inline
void map_to(Interval & J, Interval const& I)
{
    double length = J.extent();
    J[1] = I.max() * length + J[0];
    J[0] = I.min() * length + J[0];
}

/*
 * The interval [1,0] is used to represent the empty interval, this routine
 * is just an helper function for creating such an interval
 */
inline
Interval make_empty_interval()
{
    Interval I(0);
    I[0] = 1;
    return I;
}




const double MAX_PRECISION = 1e-8;
const double MIN_CLIPPED_SIZE_THRESHOLD = 0.8;
const Interval UNIT_INTERVAL(0,1);
const Interval EMPTY_INTERVAL = make_empty_interval();
const Interval H1_INTERVAL(0, 0.5);
const Interval H2_INTERVAL(0.5 + MAX_PRECISION, 1.0);

/*
 * intersection
 *
 * input:
 * A, B: control point sets of two bezier curves
 * domA, domB: real parameter intervals of the two curves
 * precision: required computational precision of the returned parameter ranges
 * output:
 * domsA, domsB: sets of parameter intervals describing an intersection point
 *
 * The parameter intervals are computed by using a Bezier clipping algorithm,
 * in case the clipping doesn't shrink the initial interval more than 20%,
 * a subdivision step is performed.
 * If during the computation one of the two curve interval length becomes less
 * than MAX_PRECISION the routine exits indipendently by the precision reached
 * in the computation of the other curve interval.
 */
void intersection (std::vector<Interval>& domsA,
                   std::vector<Interval>& domsB,
                   std::vector<Point> const& A,
                   std::vector<Point> const& B,
                   Interval const& domA,
                   Interval const& domB,
                   double precision)
{
//    std::cerr << ">> curve subdision performed <<" << std::endl;
//    std::cerr << "dom(A) : " << domA << std::endl;
//    std::cerr << "dom(B) : " << domB << std::endl;
//    std::cerr << "angle(A) : " << angle(A) << std::endl;
//    std::cerr << "angle(B) : " << angle(B) << std::endl;


    if (precision < MAX_PRECISION)
        precision = MAX_PRECISION;

    std::vector<Point> pA = A;
    std::vector<Point> pB = B;
    std::vector<Point>* C1 = &pA;
    std::vector<Point>* C2 = &pB;

    Interval dompA = domA;
    Interval dompB = domB;
    Interval* dom1 = &dompA;
    Interval* dom2 = &dompB;

    std::vector<double> bl(3);
    Interval bound, dom;


    size_t iter = 0;
    while (++iter < 100
            && (dompA.extent() >= precision || dompB.extent() >= precision))
    {
//        std::cerr << "iter: " << iter << std::endl;

        pick_orientation_line(bl, *C1);
        fat_line_bounds(bound, *C1, bl);
        clip(dom, *C2, bl, bound);

        // [1,0] is utilized to represent an empty interval
        if (dom == EMPTY_INTERVAL)
        {
//            std::cerr << "dom: empty" << std::endl;
            return;
        }
//        std::cerr << "dom : " << dom << std::endl;

        // all other cases where dom[0] > dom[1] are invalid
        if (dom.min() >  dom.max())
        {
            assert(dom.min() <  dom.max());
        }

        map_to(*dom2, dom);

        // it's better to stop before losing computational precision
        if (dom2->extent() <= MAX_PRECISION)
        {
//            std::cerr << "beyond max precision limit" << std::endl;
            break;
        }

        portion(*C2, dom);
        if (is_constant(*C2))
        {
//            std::cerr << "new curve portion is constant" << std::endl;
            break;
        }
        // if we have clipped less than 20% than we need to subdive the curve
        // with the largest domain into two sub-curves
        if (dom.extent() > MIN_CLIPPED_SIZE_THRESHOLD)
        {
//            std::cerr << "clipped less than 20% : " << dom.extent() << std::endl;
//            std::cerr << "angle(pA) : " << angle(pA) << std::endl;
//            std::cerr << "angle(pB) : " << angle(pB) << std::endl;

            std::vector<Point> pC1, pC2;
            Interval dompC1, dompC2;
            if (dompA.extent() > dompB.extent())
            {
                if ((dompA.extent() / 2) < MAX_PRECISION)
                {
                    break;
                }
                pC1 = pC2 = pA;
                portion(pC1, H1_INTERVAL);
                portion(pC2, H2_INTERVAL);
                dompC1 = dompC2 = dompA;
                map_to(dompC1, H1_INTERVAL);
                map_to(dompC2, H2_INTERVAL);
                intersection(domsA, domsB, pC1, pB, dompC1, dompB, precision);
                intersection(domsA, domsB, pC2, pB, dompC2, dompB, precision);
            }
            else
            {
                if ((dompB.extent() / 2) < MAX_PRECISION)
                {
                    break;
                }
                pC1 = pC2 = pB;
                portion(pC1, H1_INTERVAL);
                portion(pC2, H2_INTERVAL);
                dompC1 = dompC2 = dompB;
                map_to(dompC1, H1_INTERVAL);
                map_to(dompC2, H2_INTERVAL);
                intersection(domsB, domsA, pC1, pA, dompC1, dompA, precision);
                intersection(domsB, domsA, pC2, pA, dompC2, dompA, precision);
            }
            return;
        }

        std::swap(C1, C2);
        std::swap(dom1, dom2);
//        std::cerr << "dom(pA) : " << dompA << std::endl;
//        std::cerr << "dom(pB) : " << dompB << std::endl;
    }
    domsA.push_back(dompA);
    domsB.push_back(dompB);
}

} /* end namespace bezier_clipping */ } /* end namespace detail */


/*
 * find_intersection
 *
 *  input: A, B       - set of control points of two Bezier curve
 *  input: precision  - required precision of computation
 *  output: xs        - set of pairs of parameter values
 *                      at which crossing happens
 *
 *  This routine is based on the Bezier Clipping Algorithm,
 *  see: Sederberg - Computer Aided Geometric Design
 */
void find_intersections (std::vector< std::pair<double, double> > & xs,
                         std::vector<Point> const& A,
                         std::vector<Point> const& B,
                         double precision)
{
    std::cout << "find_intersections: intersection-by-clipping.cpp version\n";
//    std::cerr << std::fixed << std::setprecision(16);

    using detail::bezier_clipping::get_precision;
    using detail::bezier_clipping::operator<<;
    using detail::bezier_clipping::intersection;
    using detail::bezier_clipping::UNIT_INTERVAL;

    std::pair<double, double> ci;
    std::vector<Interval> domsA, domsB;
    intersection (domsA, domsB, A, B, UNIT_INTERVAL, UNIT_INTERVAL, precision);
    if (domsA.size() != domsB.size())
    {
        assert (domsA.size() == domsB.size());
    }
    xs.clear();
    xs.reserve(domsA.size());
    for (size_t i = 0; i < domsA.size(); ++i)
    {
//        std::cerr << i << " : domA : " << domsA[i] << std::endl;
//        std::cerr << "precision A: " << get_precision(domsA[i]) << std::endl;
//        std::cerr << i << " : domB : " << domsB[i] << std::endl;
//        std::cerr << "precision B: " << get_precision(domsB[i]) << std::endl;

        ci.first = domsA[i].middle();
        ci.second = domsB[i].middle();
        xs.push_back(ci);
    }
}

}  // end namespace Geom


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
