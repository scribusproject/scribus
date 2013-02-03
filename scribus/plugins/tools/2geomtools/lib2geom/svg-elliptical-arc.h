/*
 * SVG Elliptical Path Support Class
 *
 * Copyright 2008  Marco Cecchetti <mrcekets at gmail.com>
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
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


#ifndef _SVG_ELLIPTICAL_ARC_H_
#define _SVG_ELLIPTICAL_ARC_H_


#include "angle.h"
#include "matrix.h"
#include "sbasis.h"
#include "d2.h"


namespace Geom
{

class EllipticalArc
{
  public:
    EllipticalArc( Point _initial_point, Point _final_point,
                   double _rx,         double _ry,
                   bool _large_arc,    bool _sweep,
                   double _rot_angle = 0.0
                 )
        : m_initial_point(_initial_point), m_final_point(_final_point),
          m_rx(_rx), m_ry(_ry), m_rot_angle(_rot_angle),
          m_large_arc(_large_arc), m_sweep(_sweep)
    {
        assert( (ray(X) >= 0) && (ray(Y) >= 0) );
        if ( are_near(initialPoint(), finalPoint()) )
        {
            m_start_angle = m_end_angle = 0;
            m_center = initialPoint();
        }
        else
        {
            calculate_center_and_extreme_angles();
        }

        std::cerr << "start_angle: " << decimal_round(rad_to_deg(m_start_angle),2) << " ( " << m_start_angle << " )" << std::endl
                  << "end_angle: " << decimal_round(rad_to_deg(m_end_angle),2) << " ( " << m_end_angle << " )" << std::endl 
                  << "center: " << m_center << std::endl;
    }

  public:
    double center(Geom::Dim2 i) const
    {
        return m_center[i];
    }

    Point center() const
    {
        return m_center;
    }

    Point initialPoint() const
    {
        return m_initial_point;
    }

    Point finalPoint() const
    {
        return m_final_point;
    }

    double start_angle() const
    {
        return m_start_angle;
    }

    double end_angle() const
    {
        return m_end_angle;
    }

    double ray(Geom::Dim2 i) const
    {
        return (i == 0) ? m_rx : m_ry;
    }

    bool large_arc_flag() const
    {
        return m_large_arc;
    }

//    void large_arc_flag(bool v)
//    {
//        m_large_arc = v;
//    }

    bool sweep_flag() const
    {
        return m_sweep;
    }

//    void sweep_flag(bool v)
//    {
//        m_sweep = v;
//    }

    double rotation_angle() const
    {
        return m_rot_angle;
    }

    void setInitial( const Point _point)
    {
        m_initial_point = _point;
        calculate_center_and_extreme_angles();
    }

    void setFinal( const Point _point)
    {
        m_final_point = _point;
        calculate_center_and_extreme_angles();
    }

    void setExtremes( const Point& _initial_point, const Point& _final_point )
    {
        m_initial_point = _initial_point;
        m_final_point = _final_point;
        calculate_center_and_extreme_angles();
    }

    bool isDegenerate() const
    {
        return are_near(initialPoint(), finalPoint());
    }

    double valueAt(Coord t, Dim2 d) const
    {
        Coord tt = from_01_to_02PI(t);
        double sin_rot_angle = std::sin(rotation_angle());
        double cos_rot_angle = std::cos(rotation_angle());
        if ( d == X )
        {
            return    ray(X) * cos_rot_angle * std::cos(tt) 
                    - ray(Y) * sin_rot_angle * std::sin(tt) 
                    + center(X);
        }
        else
        {
            return    ray(X) * sin_rot_angle * std::cos(tt) 
                    + ray(Y) * cos_rot_angle * std::sin(tt) 
                    + center(X);
        }
    }

    Point pointAt(Coord t) const
    {
        Coord tt = from_01_to_02PI(t);
        double sin_rot_angle = std::sin(rotation_angle());
        double cos_rot_angle = std::cos(rotation_angle());
        Matrix m( ray(X) * cos_rot_angle, ray(X) * sin_rot_angle,
                 -ray(Y) * sin_rot_angle, ray(Y) * cos_rot_angle,
                  center(X),              center(Y) );
        Point p( std::cos(tt), std::sin(tt) );
        return p * m;
    }

    D2<SBasis> toSBasis() const
    {
        // the interval of parametrization has to be [0,1]
        Coord et = start_angle() + ( sweep_flag() ? sweep_angle() : -sweep_angle() );
        Linear param(start_angle(), et);
        // std::cerr << "param : " << param << std::endl;
        Coord cos_rot_angle = std::cos(rotation_angle());
        Coord sin_rot_angle = std::sin(rotation_angle());
        // order = 4 seems to be enough to get perfect looking elliptical arc
        // should it be choosen in function of the arc length anyway ?
        // a user settable parameter: toSBasis(unsigned int order) ?
        SBasis arc_x = ray(X) * cos(param,4);
        SBasis arc_y = ray(Y) * sin(param,4);
        D2<SBasis> arc;
        arc[0] = arc_x * cos_rot_angle - arc_y * sin_rot_angle + Linear(center(X),center(X));
        arc[1] = arc_x * sin_rot_angle + arc_y * cos_rot_angle + Linear(center(Y),center(Y));
        return arc;
    }

    std::pair<EllipticalArc, EllipticalArc>
    subdivide(Coord t) const
    {
        EllipticalArc* arc1 = portion(0, t);
        EllipticalArc* arc2 = portion(t, 1);
        assert( arc1 != NULL && arc2 != NULL);
        std::pair<EllipticalArc, EllipticalArc> arc_pair(*arc1, *arc2);        
        delete arc1;
        delete arc2;
        return arc_pair;
    }

    EllipticalArc* portion(double f, double t) const 
    {
        static const double M_2PI = 2*M_PI;
        EllipticalArc* arc = new EllipticalArc( *this );
        arc->m_initial_point = pointAt(f);
        arc->m_final_point = pointAt(t);
        //std::cerr << "initial point: " << arc->m_initial_point << std::endl;
        //std::cerr << "final point: " << arc->m_final_point << std::endl;
        double sa = sweep_angle();
        //std::cerr << "sa: " << sa << std::endl;
        arc->m_start_angle = m_start_angle + sa * f;
        if ( arc->m_start_angle > M_2PI || are_near(arc->m_start_angle, M_2PI) )
            arc->m_start_angle -= M_2PI;
        arc->m_end_angle = m_start_angle + sa * t;
        if ( arc->m_end_angle > M_2PI || are_near(arc->m_end_angle, M_2PI) )
            arc->m_end_angle -= M_2PI;
        //std::cerr << "start angle: " << arc->m_start_angle << std::endl;
        //std::cerr << "end angle: " << arc->m_end_angle << std::endl;
        //std::cerr << "sweep angle: " << arc->sweep_angle() << std::endl;
        if (f > t) arc->m_sweep = !m_sweep;
        if ( m_large_arc && (arc->sweep_angle() < M_PI) )
            arc->m_large_arc = false;
        return arc;
    }

    // the arc is the same but traversed in the opposite direction
    EllipticalArc* reverse() const
    {
        EllipticalArc* rarc = new EllipticalArc( *this );
        rarc->m_sweep = !m_sweep;
        rarc->m_initial_point = m_final_point;
        rarc->m_final_point = m_initial_point;
        rarc->m_start_angle = m_end_angle;
        rarc->m_end_angle = m_start_angle;
        return rarc;
    }

  private:

    double sweep_angle() const
    {
        Coord d = end_angle() - start_angle();
        if ( !sweep_flag() ) d = -d;
        if ( d < 0 || are_near(d, 0) )
            d += 2*M_PI;
        return d;
    }

    Coord from_01_to_02PI(Coord t) const
    {
        if ( sweep_flag() )
        {
            Coord angle = start_angle() + sweep_angle() * t;
            if ( (angle > 2*M_PI) || are_near(angle, 2*M_PI) )
                angle -= 2*M_PI;
            return angle;
        }
        else
        {
            Coord angle = start_angle() - sweep_angle() * t;
            if ( angle < 0 ) angle += 2*M_PI;
            return angle;
        }
    }

    // NOTE: doesn't work with 360 deg arcs
    void calculate_center_and_extreme_angles()
    {
        const double M_HALF_PI = M_PI/2;
        const double M_2PI = 2*M_PI;

        double sin_rot_angle = std::sin(rotation_angle());
        double cos_rot_angle = std::cos(rotation_angle());

        Point sp = sweep_flag() ? initialPoint() : finalPoint();
        Point ep = sweep_flag() ? finalPoint() : initialPoint();

        Matrix m( ray(X) * cos_rot_angle, ray(X) * sin_rot_angle,
                 -ray(Y) * sin_rot_angle, ray(Y) * cos_rot_angle,
                  0,                      0 );
        Matrix im = m.inverse();
        Point sol = (ep - sp) * im;
        std::cerr << "sol : " << sol << std::endl;
        double half_sum_angle = std::atan2(-sol[X], sol[Y]);
        double half_diff_angle;
        if ( are_near(std::fabs(half_sum_angle), M_HALF_PI) )
        {
            double anti_sgn_hsa = (half_sum_angle > 0) ? -1 : 1;
            double arg = anti_sgn_hsa * sol[X] / 2;
            // if |arg| is a little bit > 1 acos returns nan
            if ( are_near(arg, 1) )
                half_diff_angle = 0;
            else if ( are_near(arg, -1) )
                half_diff_angle = M_PI;
            else
            {
                assert( -1 < arg && arg < 1 );
                //  if it fails => there is no ellipse that satisfies the given constraints
                half_diff_angle = std::acos( arg );
            }

            half_diff_angle = M_HALF_PI - half_diff_angle;
        }
        else
        {
            double  arg = sol[Y] / ( 2 * std::cos(half_sum_angle) );
            // if |arg| is a little bit > 1 asin returns nan
            if ( are_near(arg, 1) ) 
                half_diff_angle = M_HALF_PI;
            else if ( are_near(arg, -1) )
                half_diff_angle = -M_HALF_PI;
            else
            {
                assert( -1 < arg && arg < 1 );  
                // if it fails => there is no ellipse that satisfies the given constraints
                half_diff_angle = std::asin( arg );
            }
        }
        std::cerr << "half_sum_angle : " << decimal_round(rad_to_deg(half_sum_angle),2) << " ( " << half_sum_angle << " )" << std::endl;
        std::cerr << "half_diff_angle : " << decimal_round(rad_to_deg(half_diff_angle),2) << " ( " << half_diff_angle << " )" << std::endl;
        //std::cerr << "cos(half_sum_angle) : " << std::cos(half_sum_angle) << std::endl;
        //std::cerr << "sol[Y] / ( 2 * std::cos(half_sum_angle) ) : " << sol[Y] / ( 2 * std::cos(half_sum_angle) ) << std::endl;        

        if (   ( m_large_arc && half_diff_angle > 0 ) 
            || (!m_large_arc && half_diff_angle < 0 ) )
        {
            half_diff_angle = -half_diff_angle;
        }
        if ( half_sum_angle < 0 ) half_sum_angle += M_2PI;
        if ( half_diff_angle < 0 ) half_diff_angle += M_PI;
        std::cerr << "half_sum_angle : " << decimal_round(rad_to_deg(half_sum_angle),2) << " ( " << half_sum_angle << " )" << std::endl;
        std::cerr << "half_diff_angle : " << decimal_round(rad_to_deg(half_diff_angle),2) << " ( " << half_diff_angle << " )" << std::endl;
        
        m_start_angle = half_sum_angle - half_diff_angle;
        m_end_angle =  half_sum_angle + half_diff_angle;
        // 0 <= m_start_angle, m_end_angle < 2PI
        if ( m_start_angle < 0 ) m_start_angle += M_2PI;
        if ( m_end_angle > M_2PI || are_near(m_end_angle, M_2PI) ) m_end_angle -= M_2PI;
        sol[0] = std::cos(m_start_angle);
        sol[1] = std::sin(m_start_angle);
        m_center = sp - sol * m;
        if ( !sweep_flag() )
        {
            double angle = m_start_angle;
            m_start_angle = m_end_angle;
            m_end_angle = angle;
        }
    }

  private:
    Point m_initial_point, m_final_point;
    double m_rx, m_ry, m_rot_angle;
    bool m_large_arc, m_sweep;

    double m_start_angle, m_end_angle;
    Point m_center;
};


}


#endif /*_SVG_ELLIPTICAL_ARC_H_*/


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
