/*
 * arc-length.cpp
 *
 * Copyright 2006 Nathan Hurst <njh@mail.csse.monash.edu.au>
 * Copyright 2006 Michael G. Sloan <mgsloan@gmail.com>
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
 *
 */

#include "arc-length.h"
#include "bezier-utils.h"
#include "poly.h"
using namespace Geom;

/** Calculates the length of a cubic element through subdivision.
 *  The 'tol' parameter is the maximum error allowed.  This is used to subdivide the curve where necessary.
 */
double cubic_length_subdividing(Path::Elem const & e, double tol) {
    Point v[3];
    for(int i = 0; i < 3; i++)
        v[i] = e[i+1] - e[0];
    Point orth = v[2]; // unit normal to path line
    rot90(orth);
    orth.normalize();
    double err = fabs(dot(orth, v[1])) + fabs(dot(orth, v[0]));
    if(err < tol) {
        return distance(e.first(), e.last()); // approximately a line
    } else {
        Point mid[3];
        double result;
        for(int i = 0; i < 3; i++)
            mid[i] = lerp(0.5, e[i], e[i+1]);
        Point midmid[2];
        for(int i = 0; i < 2; i++)
            midmid[i] = lerp(0.5, mid[i], mid[i+1]);
        Point midmidmid = lerp(0.5, midmid[0], midmid[1]);
        {
            Point curve[4] = {e[0], mid[0], midmid[0], midmidmid};
            Path::Elem e0(cubicto, std::vector<Point>::const_iterator(curve), std::vector<Point>::const_iterator(curve) + 4);
            result = cubic_length_subdividing(e0, tol);
        } {
            Point curve[4] = {midmidmid, midmid[1], mid[2], e[3]};
            Path::Elem e1(cubicto, std::vector<Point>::const_iterator(curve), std::vector<Point>::const_iterator(curve) + 4);
            return result + cubic_length_subdividing(e1, tol);
        }
    }
}

/** Calculates the length of a path through iteration and subsequent subdivision.
 *  Currently handles cubic curves and lines.
 *  The 'tol' parameter is the maximum error allowed.  This is used to subdivide the curve where necessary.
 */
double arc_length_subdividing(Path const & p, double tol) {
    double result = 0;

    for(Path::const_iterator iter(p.begin()), end(p.end()); iter != end; ++iter) {
        if(dynamic_cast<LineTo *>(iter.cmd()))
            result += distance((*iter).first(), (*iter).last());
        else if(dynamic_cast<CubicTo *>(iter.cmd()))
            result += cubic_length_subdividing(*iter, tol);
        else
            ;
    }
    
    return result;
}


#ifdef HAVE_GSL
#include <gsl/gsl_integration.h>
static double poly_length_integrating(double t, void* param) {
    Poly* pc = (Poly*)param;
    return hypot(pc[0].eval(t), pc[1].eval(t));
}

/** Calculates the length of a path Element through gsl integration.
 \param pe the Element.
 \param t the parametric input 0 to 1 which specifies the amount of the curve to use.
 \param tol the maximum error allowed.
 \param result variable to be incremented with the length of the path
 \param abs_error variable to be incremented with the estimated error
*/
void arc_length_integrating(Path::Elem pe, double t, double tol, double &result, double &abs_error) {
    if(dynamic_cast<LineTo *>(iter.cmd()))
        result += distance(pe.first(), pe.last()) * t;
    else if(dynamic_cast<QuadTo *>(iter.cmd()) ||
            dynamic_cast<CubicTo *>(iter.cmd())) {
        Poly B[2] = {get_parametric_poly(pe, X), get_parametric_poly(pe, Y)};
        for(int i = 0; i < 2; i++)
            B[i] = derivative(B[i]);
        
        gsl_function F;
        gsl_integration_workspace * w 
            = gsl_integration_workspace_alloc (20);
        F.function = &poly_length_integrating;
        F.params = (void*)B;
        double quad_result, err;
        /* We could probably use the non adaptive code here if we removed any cusps first. */
        int returncode = 
            gsl_integration_qag (&F, 0, t, 0, tol, 20, 
                                 GSL_INTEG_GAUSS21, w, &quad_result, &err);
            
        abs_error += err;
        result += quad_result;
    } else
        return;
}

/** Calculates the length of a Path through gsl integration.  The parameter 'tol' is the maximum error allowed. */
double arc_length_integrating(Path const & p, double tol) {
    double result = 0, abserr = 0;

    for(Path::const_iterator iter(p.begin()), end(p.end()); iter != end; ++iter) {
        arc_length_integrating(*iter, 1.0, tol, result, abserr);
    }
    //printf("got %g with err %g\n", result, abserr);
    
    return result;
}

/** Calculates the arc length to a specific location on the path.  The parameter 'tol' is the maximum error allowed. */
double arc_length_integrating(Path const & p, Path::Location const & pl, double tol) {
    double result = 0, abserr = 0;
    ptrdiff_t offset = pl.it - p.begin();
    
    assert(offset >= 0);
    assert(offset < p.size());
    
    for(Path::const_iterator iter(p.begin()), end(p.end()); 
        (iter != pl.it); ++iter) {
        arc_length_integrating(*iter, 1.0, tol, result, abserr);
    }
    arc_length_integrating(*pl.it, pl.t, tol, result, abserr);
    
    return result;
}

/* We use a somewhat surprising result for this that s'(t) = |p'(t)| 
   Thus, we can use a derivative based root finder.
*/

#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_roots.h>
     
struct arc_length_params
{
    Path::Elem pe;
    double s,tol, result, abs_error;
    double left, right;
};

double
arc_length (double t, void *params)
{
    struct arc_length_params *p 
        = (struct arc_length_params *) params;
     
    double result = 0, abs_error = 0;
    if(t < 0) t = 0;
    if(t > 1) t = 1;
    if(!((t >= 0) && (t <= 1))) {
        printf("t = %g\n", t);
    }
    assert((t >= 0) && (t <= 1));
    arc_length_integrating(p->pe, t, p->tol, result, abs_error);
    return result - p->s ;
}
     
double
arc_length_deriv (double t, void *params)
{
    struct arc_length_params *p 
        = (struct arc_length_params *) params;
    
    Point pos, tgt, acc;
    p->pe.point_tangent_acc_at(t, pos, tgt, acc);
    return L2(tgt);
}
     
void
arc_length_fdf (double t, void *params, 
               double *y, double *dy)
{
    *y = arc_length(t, params);
    *dy = arc_length_deriv(t, params);
}

double polish_brent(double t, arc_length_params &alp) {
       int status;
       int iter = 0, max_iter = 10;
       const gsl_root_fsolver_type *T;
       gsl_root_fsolver *solver;
       double x_lo = 0.0, x_hi = 1.0;
       gsl_function F;
     
       F.function = &arc_length;
       F.params = &alp;
     
       T = gsl_root_fsolver_brent;
       solver = gsl_root_fsolver_alloc (T);
       gsl_root_fsolver_set (solver, &F, x_lo, x_hi);
     
       do
         {
           iter++;
           status = gsl_root_fsolver_iterate (solver);
           t = gsl_root_fsolver_root (solver);
           x_lo = gsl_root_fsolver_x_lower (solver);
           x_hi = gsl_root_fsolver_x_upper (solver);
           status = gsl_root_test_interval (x_lo, x_hi,
                                            0, alp.tol);
     
           //if (status == GSL_SUCCESS)
           //    printf ("Converged:\n");
     
         }
       while (status == GSL_CONTINUE && iter < max_iter);
       return t;
}

double polish (double t, arc_length_params &alp) {
    int status;
    int iter = 0, max_iter = 5;
    const gsl_root_fdfsolver_type *T;
    gsl_root_fdfsolver *solver;
    double t0;
    gsl_function_fdf FDF;
     
    FDF.f = &arc_length;
    FDF.df = &arc_length_deriv;
    FDF.fdf = &arc_length_fdf;
    FDF.params = &alp;
    
    T = gsl_root_fdfsolver_newton;
    solver = gsl_root_fdfsolver_alloc (T);
    gsl_root_fdfsolver_set (solver, &FDF, t);
     
    do
    {
        iter++;
        status = gsl_root_fdfsolver_iterate (solver);
        t0 = t;
        t = gsl_root_fdfsolver_root (solver);
        status = gsl_root_test_delta (t, t0, 0, alp.tol);
     
        if (status == GSL_SUCCESS)
            ;//printf ("Converged:\n");
     
        printf ("%5d %10.7f %+10.7f\n",
                iter, t, t - t0);
    }
    while (status == GSL_CONTINUE && iter < max_iter);
    return t;
}


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
