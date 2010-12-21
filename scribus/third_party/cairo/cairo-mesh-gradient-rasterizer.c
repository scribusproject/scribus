/* -*- Mode: c; tab-width: 8; c-basic-offset: 4; indent-tabs-mode: t; -*- */
/* cairo - a vector graphics library with display and print output
 *
 * Copyright © 2009 Andrea Canciani
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
 * The Original Code is the cairo graphics library.
 *
 * The Initial Developer of the Original Code is Andrea Canciani.
 *
 * Contributor(s):
 *	Andrea Canciani <ranma42@gmail.com>
 */

#include "cairoint.h"

/*
  Make sure that errors are less that 1 in fixed point math if you change this values.
  Using 8.24 for first, second and third difference 256 is the maximum allowed number of steps
*/
#define VSTEPS 256.0
#define USTEPS 256.0

/*
  If the surface/curve is only partially visible, tessellate it to a finer resolution to get
  higher chances to clip (part of) it.
*/
#define VPARSTEPS 64.0
#define UPARSTEPS 64.0

#if defined (_WIN32)
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif
#endif


/* Utils */
static inline double
sqlen (double x, double y)
{
    return x*x + y*y;
}

static inline double
max (double x, double y)
{
    return x>y?x:y;
}

static inline double
min (double x, double y)
{
    return x<y?x:y;
}

static inline uint16_t
_color_delta_to_shifted_short (int32_t from, int32_t to, int shift)
{
    int32_t delta = to - from;
    if (delta >= 0)
	return delta >> shift;
    else
	return -((-delta) >> shift);
}

/*
  Given a certain (minimum) number of steps, compute the first exponent x
  such that 2**(x) > steps
  NOTE: actually the input is the square of the minimum number of steps
*/
static inline int
sqsteps2shift (double steps_sq)
{
    int r;
    frexp (max (1.0, steps_sq), &r);
    return (r + 1) >> 1;
}

/* AFD functions */
static inline void
init (double x, double y, double z, double w, double f[4])
{
    f[0] = x;
    f[1] = w - x;
    f[2] = 6 * (w - 2 * z + y);
    f[3] = 6 * (w - 3 * z + 3 * y - x);
}

static inline void
down (double f[4])
{
    f[3] /= 8;
    f[2] = f[2] / 4 - f[3];
    f[1] = (f[1] - f[2]) / 2;
}

static inline void
fwd (double f[4])
{
    f[0] += f[1];
    f[1] += f[2];
    f[2] += f[3];
}

static inline void
fixed_fd (double d[4], int32_t i[4])
{
    i[0] = _cairo_fixed_16_16_from_double (d[0]);
    i[1] = _cairo_fixed_16_16_from_double (256 * d[1]);
    i[2] = _cairo_fixed_16_16_from_double (256 * d[2]);
    i[3] = _cairo_fixed_16_16_from_double (256 * d[3]);
}

static inline void
fixed_fwd (int32_t f[4])
{
    f[0] += f[1] >> 8;
    f[1] += f[2];
    f[2] += f[3];
}

/*
  Compute the minimum number of steps needed to walk over a curve not to
  leave any hole in the surface that contains the curve.
  NOTE: the value returned is actually the square of the minimum number of steps
*/
static inline double
bezier_steps_sq (double p[4][2])
{
    double tmp = sqlen (p[0][0] - p[1][0], p[0][1] - p[1][1]);
    tmp = max (tmp, sqlen (p[2][0] - p[3][0], p[2][1] - p[3][1]));
    tmp = max (tmp, sqlen (p[0][0] - p[2][0], p[0][1] - p[2][1]) / 4);
    tmp = max (tmp, sqlen (p[1][0] - p[3][0], p[1][1] - p[3][1]) / 4);
    return 18.0 * tmp;
}

/*
  Tessellate a bezier curve using de Casteljau's algorithm
  p[0..3] are the control points of the input curve
  subp0[0..3] and subp1[0..3] are the control points of the
  first half and of the second half of the curve, respectively
*/
static void
bezier_tess (double p[4][2], double subp0[4][2], double subp1[4][2])
{
    double t;
    int j;

    for (j = 0; j < 2; ++j) {
	subp0[0][j] = p[0][j];
	subp1[3][j] = p[3][j];

	subp0[1][j] = 0.5 * (p[0][j] + p[1][j]);
	t = 0.5 * (p[1][j] + p[2][j]);
	subp1[2][j] = 0.5 * (p[2][j] + p[3][j]);

	subp0[2][j] = 0.5 * (subp0[1][j] + t);
	subp1[1][j] = 0.5 * (t + subp1[2][j]);

	subp0[3][j] = subp1[0][j] = 0.5 * (subp0[2][j] + subp1[1][j]);
    }
}

/*
  Check if an interval if inside another.
  Returns
  -1 iff [a,b) intersection [c,d) = [a,b)
  0  iff [a,b) intersection [c,d) = {}
  1  otherwise

  NOTE:
  Bitwise-anding the results along each component gives the expected result for
  [a,b) x [A,B) AND [c,d) x [C,D)
*/
static inline int
interval_inside (double a, double b, double c, double d)
{
    if (c <= a && b <= d)
	return -1;
    else if (a >= d || b <= c)
	return 0;
    else
	return 1;
}


/*
  Forward-rasterize a cubic Bezier curve using (adaptive) forward differences.
*/
static inline void
draw_bezier_curve (unsigned char * data, int width, int height, int stride, int ushift,
		   int32_t xu[4], int32_t yu[4],
		   uint16_t r0, uint16_t g0, uint16_t b0, uint16_t alpha0,
		   uint16_t r3, uint16_t g3, uint16_t b3, uint16_t alpha3)
{
    int u, usteps = 1 << ushift, oldx = -1, oldy = -1;

    uint16_t r = r0, g = g0, b = b0, alpha = alpha0;
    uint16_t dr = _color_delta_to_shifted_short (r0, r3, ushift);
    uint16_t dg = _color_delta_to_shifted_short (g0, g3, ushift);
    uint16_t db = _color_delta_to_shifted_short (b0, b3, ushift);
    uint16_t dalpha = _color_delta_to_shifted_short (alpha0, alpha3, ushift);

    for (u = 0; u <= usteps; ++u) {
	int x = xu[0] >> 16;
	int y = yu[0] >> 16;
	if ((oldx != x || oldy != y) && 0 <= x && 0 <= y && x < width && y < height) {
	    uint8_t a = alpha >> 8;
	    uint16_t tg  = g >> 8;
	    uint32_t trb = (((uint32_t) r & 0xff00) << 8) | (b >> 8);
	    tg = tg * a + 0x80;
	    trb = trb * a + 0x800080;
	    tg += tg >> 8;
	    trb += (trb >> 8) & 0xff00ff;
	    *((uint32_t*) (data + y*stride + 4*x))  = (a << 24) | ((trb >> 8) & 0xff00ff) | (tg & 0xff00);
	}
	oldx = x;
	oldy = y;

	fixed_fwd (xu);
	fixed_fwd (yu);
	r += dr;
	g += dg;
	b += db;
	alpha += dalpha;
    }
}

/*
  Clip, tessellate and draw a Bezier curve
*/
static void
bezier_curve (unsigned char * data, int width, int height, int stride,
	      double p[4][2], double c0[4], double c3[4])
{
    double steps_sq;
    int v = interval_inside (min (min (p[0][1], p[1][1]), min (p[2][1], p[3][1])),
			     max (max (p[0][1], p[1][1]), max (p[2][1], p[3][1])), 0, height);
    if (!v)
	return;

    v &= interval_inside (min (min (p[0][0], p[1][0]), min (p[2][0], p[3][0])),
			  max (max (p[0][0], p[1][0]), max (p[2][0], p[3][0])), 0, width);
    if (!v)
	return;

    steps_sq = bezier_steps_sq (p);
    if (steps_sq >= (v == -1 ? USTEPS * USTEPS : UPARSTEPS * UPARSTEPS)) {
	double subp[2][4][2], subc[4];
	bezier_tess (p, subp[0], subp[1]);
	subc[0] = (c0[0] + c3[0]) * 0.5;
	subc[1] = (c0[1] + c3[1]) * 0.5;
	subc[2] = (c0[2] + c3[2]) * 0.5;
	subc[3] = (c0[3] + c3[3]) * 0.5;
	bezier_curve (data, width, height, stride, subp[0], c0, subc);
	bezier_curve (data, width, height, stride, subp[1], subc, c3);
    } else {
	int ushift = sqsteps2shift (steps_sq), k;

	double dxu[4], dyu[4];
	int32_t xu[4], yu[4];
	init (p[0][0], p[1][0], p[2][0], p[3][0], dxu);
	init (p[0][1], p[1][1], p[2][1], p[3][1], dyu);

	for (k = 0; k < ushift; ++k) {
	    down (dxu);
	    down (dyu);
	}
	fixed_fd (dxu, xu);
	fixed_fd (dyu, yu);

	draw_bezier_curve (data, width, height, stride, ushift, xu, yu,
			   _cairo_color_double_to_short (c0[0]),
			   _cairo_color_double_to_short (c0[1]),
			   _cairo_color_double_to_short (c0[2]),
			   _cairo_color_double_to_short (c0[3]),
			   _cairo_color_double_to_short (c3[0]),
			   _cairo_color_double_to_short (c3[1]),
			   _cairo_color_double_to_short (c3[2]),
			   _cairo_color_double_to_short (c3[3]));
    }
}

/*
  Forward-rasterize a Bezier cubic surface using (adaptive) forward differences.

  This implementation is based on techniques derived from several papers:

  Lien, Shantz and Pratt "Adaptive Forward Differencing for Rendering Curves and Surfaces"
  (discussion of the AFD technique, bound of 1/sqrt (2) on step length without proof)
  Popescu and Rosen, "Forward rasterization"
  (description of forward rasterization, proof of the previoul bound)
  Klassen, "Integer Forward Differencing of Cubic Polynomials: Analysis and Algorithms"
  Klassen, "Exact Integer Hybrid Forward Differencing Subdivision and of Cubics"
  (improving the bound on the minimum number of steps)
  Chang, Shantz and Rocchetti, "Rendering Cubic Curves and Surfaces with Integer Adaptive Forward Differencing"
  (analysis of forward differencing applied to surfaces)
  All these papers are available from ACM.

  NOTES:
  - Poor performance expected in degenerate cases
  - Patches mostly outside the drawing area are drawn completely (and clipped), wasting time
  - Both previous problems are greatly reduced by tessellating to a reasonably small size and clipping the new tiles:
  execution time is quadratic in the convex-hull diameter instead than linear to the painted area
  splitting in tiles doesn't change the painted area but (usually) reduces the bounding box area
  (bbox area can remain the same after tessellation, but cannot grow)
*/
static inline void
draw_bezier_surface (unsigned char *data, int width, int height, int stride, int vshift,
		     double p[4][4][2], double col[4][4])
{
    double pv[4][2][4], c[2][4], dc[2][4];
    int vsteps, v, i, j, k;

    vsteps = 1 << vshift;

    for (i = 0; i < 4; ++i)
	for (j = 0; j < 2; ++j) {
	    init (p[i][0][j], p[i][1][j], p[i][2][j], p[i][3][j], pv[i][j]);
	    for (k = 0; k < vshift; ++k)
		down (pv[i][j]);
	    c[j][i] = col[j][i];
	    dc[j][i] = (col[2+j][i] - col[j][i]) / vsteps;
	}

    for (v = 0; v <= vsteps; ++v) {
	double p[4][2];
	for (i = 0; i < 4; ++i)
	    for (j = 0; j < 2; ++j)
		p[i][j] = pv[i][j][0];

	bezier_curve (data, width, height, stride, p, c[0], c[1]);

	for (i = 0; i < 4; ++i)
	    for (j = 0; j < 2; ++j) {
		fwd (pv[i][j]);
		c[j][i] += dc[j][i];
	    }
    }
}

/*
  Clip, tessellate (only in the v-direction) and rasterize a Bezier cubic surface
*/
static void
bezier_surface (unsigned char *data, int width, int height, int stride,
		double p[4][4][2], double c[4][4])
{
    double minp, maxp, steps_sq;
    int i, j, v;

    minp = maxp = p[0][0][1];

    for (i = 0; i < 4; ++i)
	for (j= 0; j < 4; ++j) {
	    minp = min (minp, p[i][j][1]);
	    maxp = max (maxp, p[i][j][1]);
	}
    v = interval_inside (minp, maxp, 0, height);
    if (!v)
	return;

    minp = maxp = p[0][0][0];
    for (i = 0; i < 4; ++i)
	for (j= 0; j < 4; ++j) {
	    minp = min (minp, p[i][j][0]);
	    maxp = max (maxp, p[i][j][0]);
	}
    v &= interval_inside (minp, maxp, 0, width);
    if (!v)
	return;

    steps_sq = 0;
    for (i = 0; i < 4; ++i)
	steps_sq = max (steps_sq, bezier_steps_sq (p[i]));

    if (steps_sq >= (v == -1 ? VSTEPS * VSTEPS : VPARSTEPS * VPARSTEPS)) {
	double subp[2][4][4][2], subc[4][4];

	for (i = 0; i < 4; ++i)
	    bezier_tess (p[i], subp[0][i], subp[1][i]);

	for (i = 0; i < 4; ++i) {
	    subc[0][i] = c[0][i];
	    subc[1][i] = c[1][i];
	    subc[2][i] = 0.5 * (c[0][i] + c[2][i]);
	    subc[3][i] = 0.5 * (c[1][i] + c[3][i]);
	}

	bezier_surface (data, width, height, stride, subp[0], subc);

	for (i = 0; i < 4; ++i) {
	    subc[0][i] = subc[2][i];
	    subc[1][i] = subc[3][i];
	    subc[2][i] = c[2][i];
	    subc[3][i] = c[3][i];
	}
	bezier_surface (data, width, height, stride, subp[1], subc);
    } else
	draw_bezier_surface (data, width, height, stride, sqsteps2shift (steps_sq), p, c);
}

/*
  Draw a tensor product shading pattern tile.
  0     - >    1
  0  p00 p01 p02 p03
  |  p10 p11 p12 p13
  v  p20 p21 p22 p23
  1  p30 p31 p32 p33

  c[0..3] are the rgba colors in p00, p30, p03, p33 respectively
  See PDF Reference for a formal definition
  http://www.adobe.com/devnet/pdf/pdf_reference.html
  NOTE:
  following specifications p00 < p30 < p03 < p33 (< means "can be covered")
  i.e. rows represent v coordinate, columns u coordinate and points can be compared as such
  (u, v) < (u', v') iff v < v' || ((v==v') && (u < u'))
*/
static inline void
gradient_tile (void *data, int width, int height, int stride, double p[4][4][2], double c[4][4])
{
    bezier_surface (data, width, height, stride, p, c);
}

cairo_status_t
_cairo_rasterize_gradient_mesh (cairo_mesh_pattern_t *mesh,
				cairo_surface_t      *image,
				unsigned int	      width,
				unsigned int	      height)
{
    unsigned char *data;
    int stride, i, j, k;
    double points[4][4][2];
    double colors[4][4];
    cairo_matrix_t p2u;
    cairo_status_t status;
    cairo_color_t *c;

    p2u = mesh->base.matrix;
    status = cairo_matrix_invert (&p2u);
    if (status)
	return status;

    data = cairo_image_surface_get_data (image);
    stride = cairo_image_surface_get_stride (image);
    for (i = 0; i < _cairo_array_num_elements (&mesh->patches); i++) {
	cairo_gradient_patch_t *patch = _cairo_array_index (&mesh->patches, i);

	for (j = 0; j < 4; j++) {
	    for (k = 0; k < 4; k++) {
		points[j][k][0] = patch->points[j][k][0];
		points[j][k][1] = patch->points[j][k][1];
		cairo_matrix_transform_point (&p2u, &points[j][k][0], &points[j][k][1]);
	    }
	}

	c = &patch->colors[0];
	colors[0][0] = c->red;
	colors[0][1] = c->green;
	colors[0][2] = c->blue;
	colors[0][3] = c->alpha;

	c = &patch->colors[3];
	colors[1][0] = c->red;
	colors[1][1] = c->green;
	colors[1][2] = c->blue;
	colors[1][3] = c->alpha;

	c = &patch->colors[1];
	colors[2][0] = c->red;
	colors[2][1] = c->green;
	colors[2][2] = c->blue;
	colors[2][3] = c->alpha;

	c = &patch->colors[2];
	colors[3][0] = c->red;
	colors[3][1] = c->green;
	colors[3][2] = c->blue;
	colors[3][3] = c->alpha;

	gradient_tile (data, width, height, stride, points, colors);
    }

    return CAIRO_STATUS_SUCCESS;
}
