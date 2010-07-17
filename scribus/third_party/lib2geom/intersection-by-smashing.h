/*
 * Diffeomorphism-based intersector: given two curves
 *  M(t)=(x(t),y(t)) and N(u)=(X(u),Y(u))
 * and supposing M is a graph over the x-axis, we compute y(x) and the
 * transformation:
 *  X <- X
 *  Y <- Y - y(X)
 * smashes M on the x axis. The intersections are then given by the roots of:
 *  Y(u) - y(X(u)) = 0
 *
 * Authors:
 * 		J.-F. Barraud    <jfbarraud at gmail.com>
 * Copyright 2010  authors
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


#include "d2.h"
#include "sbasis.h"
#include "sbasis-geometric.h"
#include <cstdlib>
#include <vector>
#include <algorithm>


namespace Geom{

struct Intersection{
	Rect times;
	Rect bbox;
};

std::vector<Intersection> smash_intersect( D2<SBasis> const &a, D2<SBasis> const &b, double tol);
std::vector<Intersection> monotonic_smash_intersect( D2<SBasis> const &a, D2<SBasis> const &b, double tol);
//std::vector<Intersection> monotonic_smash_intersect( Curve const &a, double a_from, double a_to,
//			                                         Curve const &b, double b_from, double b_to, double tol);

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
