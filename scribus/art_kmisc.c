#include <libart_lgpl/art_vpath.h>
#include <libart_lgpl/art_bpath.h>
#include <libart_lgpl/art_misc.h>
#include <libart_lgpl/art_affine.h>

#include "config.h"
#include "art_kmisc.h"

extern double ceil(double x);
extern double floor(double x);

/**
 * art_vpath_render_bez: Render a bezier segment into the vpath.
 * @p_vpath: Where the pointer to the #ArtVpath structure is stored.
 * @pn_points: Pointer to the number of points in *@p_vpath.
 * @pn_points_max: Pointer to the number of points allocated.
 * @x0: X coordinate of starting bezier point.
 * @y0: Y coordinate of starting bezier point.
 * @x1: X coordinate of first bezier control point.
 * @y1: Y coordinate of first bezier control point.
 * @x2: X coordinate of second bezier control point.
 * @y2: Y coordinate of second bezier control point.
 * @x3: X coordinate of ending bezier point.
 * @y3: Y coordinate of ending bezier point.
 * @flatness: Flatness control.
 *
 * Renders a bezier segment into the vector path, reallocating and
 * updating *@p_vpath and *@pn_vpath_max as necessary. *@pn_vpath is
 * incremented by the number of vector points added.
 *
 * This step includes (@x0, @y0) but not (@x3, @y3).
 *
 * The @flatness argument guides the amount of subdivision. The Adobe
 * PostScript reference manual defines flatness as the maximum
 * deviation between the any point on the vpath approximation and the
 * corresponding point on the "true" curve, and we follow this
 * definition here. A value of 0.25 should ensure high quality for aa


 * rendering.
 **/
 void
ksvg_art_vpath_render_bez (ArtVpath **p_vpath, int *pn, int *pn_max,
		double x0, double y0,
		double x1, double y1,
		double x2, double y2,
		double x3, double y3,
		double flatness)
{
	double x3_0, y3_0;
	double z3_0_dot;
	double z1_dot, z2_dot;
	double z1_perp, z2_perp;
	double max_perp_sq;

	double x_m, y_m;
	double xa1, ya1;
	double xa2, ya2;
	double xb1, yb1;
	double xb2, yb2;

	/* It's possible to optimize this routine a fair amount.

	   First, once the _dot conditions are met, they will also be met in
	   all further subdivisions. So we might recurse to a different
	   routine that only checks the _perp conditions.

	   Second, the distance _should_ decrease according to fairly
	   predictable rules (a factor of 4 with each subdivision). So it might
	   be possible to note that the distance is within a factor of 4 of
	   acceptable, and subdivide once. But proving this might be hard.

	   Third, at the last subdivision, x_m and y_m can be computed more
	   expeditiously (as in the routine above).

	   Finally, if we were able to subdivide by, say 2 or 3, this would
	   allow considerably finer-grain control, i.e. fewer points for the
	   same flatness tolerance. This would speed things up downstream.

	   In any case, this routine is unlikely to be the bottleneck. It's
	   just that I have this undying quest for more speed...

*/

	x3_0 = x3 - x0;
	y3_0 = y3 - y0;

	/* z3_0_dot is dist z0-z3 squared */
	z3_0_dot = x3_0 * x3_0 + y3_0 * y3_0;

	/* todo: this test is far from satisfactory. */
	if (z3_0_dot < 0.001)
		goto nosubdivide;

	/* we can avoid subdivision if:

	   z1 has distance no more than flatness from the z0-z3 line

	   z1 is no more z0'ward than flatness past z0-z3

	   z1 is more z0'ward than z3'ward on the line traversing z0-z3

	   and correspondingly for z2 */

	/* perp is distance from line, multiplied by dist z0-z3 */
	max_perp_sq = flatness * flatness * z3_0_dot;
	z1_perp = (y1 - y0) * x3_0 - (x1 - x0) * y3_0;
	if (z1_perp * z1_perp > max_perp_sq)
		goto subdivide;

	z2_perp = (y3 - y2) * x3_0 - (x3 - x2) * y3_0;
	if (z2_perp * z2_perp > max_perp_sq)
		goto subdivide;

	z1_dot = (x1 - x0) * x3_0 + (y1 - y0) * y3_0;
	if (z1_dot < 0 && z1_dot * z1_dot > max_perp_sq)
		goto subdivide;

	z2_dot = (x3 - x2) * x3_0 + (y3 - y2) * y3_0;
	if (z2_dot < 0 && z2_dot * z2_dot > max_perp_sq)
		goto subdivide;

	if (z1_dot + z1_dot > z3_0_dot)
		goto subdivide;

	if (z2_dot + z2_dot > z3_0_dot)
		goto subdivide;

nosubdivide:
	/* don't subdivide */
	art_vpath_add_point (p_vpath, pn, pn_max,
			ART_LINETO, x3, y3);
	return;

subdivide:

	xa1 = (x0 + x1) * 0.5;
	ya1 = (y0 + y1) * 0.5;
	xa2 = (x0 + 2 * x1 + x2) * 0.25;
	ya2 = (y0 + 2 * y1 + y2) * 0.25;
	xb1 = (x1 + 2 * x2 + x3) * 0.25;
	yb1 = (y1 + 2 * y2 + y3) * 0.25;
	xb2 = (x2 + x3) * 0.5;
	yb2 = (y2 + y3) * 0.5;
	x_m = (xa2 + xb1) * 0.5;
	y_m = (ya2 + yb1) * 0.5;
#ifdef VERBOSE
	printf ("%g,%g %g,%g %g,%g %g,%g\n", xa1, ya1, xa2, ya2,
			xb1, yb1, xb2, yb2);
#endif
	ksvg_art_vpath_render_bez (p_vpath, pn, pn_max,
			x0, y0, xa1, ya1, xa2, ya2, x_m, y_m, flatness);
	ksvg_art_vpath_render_bez (p_vpath, pn, pn_max,
			x_m, y_m, xb1, yb1, xb2, yb2, x3, y3, flatness);
}

#define RENDER_LEVEL 4
#define RENDER_SIZE (1 << (RENDER_LEVEL))

/**
 * ksvg_art_bez_path_to_vec: Create vpath from bezier path.
 * @bez: Bezier path.
 * @flatness: Flatness control.
 *
 * Creates a vector path closely approximating the bezier path defined by
 * @bez. The @flatness argument controls the amount of subdivision. In
 * general, the resulting vpath deviates by at most @flatness pixels
 * from the "ideal" path described by @bez.
 *
 * Return value: Newly allocated vpath.
 **/
 ArtVpath *
ksvg_art_bez_path_to_vec(const ArtBpath *bez, double flatness)
{
	ArtVpath *vec;
	int vec_n, vec_n_max;
	int bez_index;
	double x, y;

	vec_n = 0;
	vec_n_max = RENDER_SIZE;
	vec = art_new (ArtVpath, vec_n_max);

	/* Initialization is unnecessary because of the precondition that the
	   bezier path does not begin with LINETO or CURVETO, but is here
	   to make the code warning-free. */
	x = 0;
	y = 0;

	bez_index = 0;
	do
	{
#ifdef VERBOSE
		printf ("%s %g %g\n",
				bez[bez_index].code == ART_CURVETO ? "curveto" :
				bez[bez_index].code == ART_LINETO ? "lineto" :
				bez[bez_index].code == ART_MOVETO ? "moveto" :
				bez[bez_index].code == ART_MOVETO_OPEN ? "moveto-open" :
				"end", bez[bez_index].x3, bez[bez_index].y3);
#endif
		/* make sure space for at least one more code */
		if (vec_n >= vec_n_max)
			art_expand (vec, ArtVpath, vec_n_max);
		switch (bez[bez_index].code)
		{
			case ART_MOVETO_OPEN:
			case ART_MOVETO:
			case ART_LINETO:
				x = bez[bez_index].x3;
				y = bez[bez_index].y3;
				vec[vec_n].code = bez[bez_index].code;
				vec[vec_n].x = x;
				vec[vec_n].y = y;
				vec_n++;
				break;
			case ART_END:
				vec[vec_n].code = ART_END;
				vec[vec_n].x = 0;
				vec[vec_n].y = 0;
				vec_n++;
				break;
			case ART_END2:
				vec[vec_n].code = (ArtPathcode)ART_END2;
				vec[vec_n].x = bez[bez_index].x3;
				vec[vec_n].y = bez[bez_index].y3;
				vec_n++;
				break;
			case ART_CURVETO:
#ifdef VERBOSE
				printf ("%g,%g %g,%g %g,%g %g,%g\n", x, y,
						bez[bez_index].x1, bez[bez_index].y1,
						bez[bez_index].x2, bez[bez_index].y2,
						bez[bez_index].x3, bez[bez_index].y3);
#endif
				ksvg_art_vpath_render_bez (&vec, &vec_n, &vec_n_max,
						x, y,
						bez[bez_index].x1, bez[bez_index].y1,
						bez[bez_index].x2, bez[bez_index].y2,
						bez[bez_index].x3, bez[bez_index].y3,
						flatness);
				x = bez[bez_index].x3;
				y = bez[bez_index].y3;
				break;
		}
	}
	while (bez[bez_index++].code != ART_END);
	return vec;
}

/* Private functions for the rgb affine image compositors - primarily,
*    the determination of runs, eliminating the need for source image
*       bbox calculation in the inner loop. */

/* Determine a "run", such that the inverse affine of all pixels from
*    (x0, y) inclusive to (x1, y) exclusive fit within the bounds
*       of the source image.
*
*          Initial values of x0, x1, and result values stored in first two
*             pointer arguments.
*             */

#define EPSILON 1e-6

 void ksvg_art_rgb_affine_run (int *p_x0, int *p_x1, int y,
		int src_width, int src_height,
		const double affine[6])
{
	int x0, x1;
	double z;
	double x_intercept;
	int xi;

	x0 = *p_x0;
	x1 = *p_x1;

	/* do left and right edges */
	if (affine[0] > EPSILON)
	{
		z = affine[2] * (y + 0.5) + affine[4];
		x_intercept = -z / affine[0];
		xi = ceil (x_intercept + EPSILON - 0.5);
		if (xi > x0)
			x0 = xi;
		x_intercept = (-z + src_width) / affine[0];
		xi = ceil (x_intercept - EPSILON - 0.5);
		if (xi < x1)
			x1 = xi;
	}
	else if (affine[0] < -EPSILON)
	{
		z = affine[2] * (y + 0.5) + affine[4];
		x_intercept = (-z + src_width) / affine[0];
		xi = ceil (x_intercept + EPSILON - 0.5);
		if (xi > x0)
			x0 = xi;
		x_intercept = -z / affine[0];
		xi = ceil (x_intercept - EPSILON - 0.5);
		if (xi < x1)
			x1 = xi;
	}
	else
	{
		z = affine[2] * (y + 0.5) + affine[4];
		if (z < 0 || z >= src_width)
		{
			*p_x1 = *p_x0;
			return;
		}
	}
	/* do top and bottom edges */
	if (affine[1] > EPSILON)
	{
		z = affine[3] * (y + 0.5) + affine[5];
		x_intercept = -z / affine[1];
		xi = ceil (x_intercept + EPSILON - 0.5);
		if (xi > x0)
			x0 = xi;
		x_intercept = (-z + src_height) / affine[1];
		xi = ceil (x_intercept - EPSILON - 0.5);
		if (xi < x1)
			x1 = xi;
	}
	else if (affine[1] < -EPSILON)
	{
		z = affine[3] * (y + 0.5) + affine[5];
		x_intercept = (-z + src_height) / affine[1];
		xi = ceil (x_intercept + EPSILON - 0.5);
		if (xi > x0)
			x0 = xi;
		x_intercept = -z / affine[1];
		xi = ceil (x_intercept - EPSILON - 0.5);
		if (xi < x1)
			x1 = xi;
	}
	else
	{
		z = affine[3] * (y + 0.5) + affine[5];
		if (z < 0 || z >= src_height)
		{
			*p_x1 = *p_x0;

			return;
		}
	}


	*p_x0 = x0;
	*p_x1 = x1;
}

/**
 * ksvg_art_rgb_affine: Affine transform source RGB image and composite.
 * @dst: Destination image RGB buffer.
 * @x0: Left coordinate of destination rectangle.
 * @y0: Top coordinate of destination rectangle.
 * @x1: Right coordinate of destination rectangle.
 * @y1: Bottom coordinate of destination rectangle.
 * @dst_rowstride: Rowstride of @dst buffer.
 * @src: Source image RGB buffer.
 * @src_width: Width of source image.
 * @src_height: Height of source image.
 * @src_rowstride: Rowstride of @src buffer.
 * @affine: Affine transform.
 * @level: Filter level.
 * @alphagamma: #ArtAlphaGamma for gamma-correcting the compositing.
 * @alpha: Alpha, range 0..256.
 *
 * Affine transform the source image stored in @src, compositing over
 * the area of destination image @dst specified by the rectangle
 * (@x0, @y0) - (@x1, @y1). As usual in libart, the left and top edges
 * of this rectangle are included, and the right and bottom edges are
 * excluded.
 *
 * The @alphagamma parameter specifies that the alpha compositing be done
 * in a gamma-corrected color space. Since the source image is opaque RGB,
 * this argument only affects the edges. In the current implementation,
 * it is ignored.
 *
 * The @level parameter specifies the speed/quality tradeoff of the
 * image interpolation. Currently, only ART_FILTER_NEAREST is
 * implemented.
 *
 * KSVG additions : we have changed this function to support an alpha level as well.
*                  also we made sure compositing an rgba image over an rgb buffer works.
**/
 void ksvg_art_rgb_affine (art_u8 *dst, int x0, int y0, int x1, int y1, int dst_rowstride,
		const art_u8 *src,
		int src_width, int src_height, int src_rowstride,
		const double affine[6],
		ArtFilterLevel level,
		ArtAlphaGamma *alphagamma,
		int alpha)
{
	/* Note: this is a slow implementation, and is missing all filter
	   levels other than NEAREST. It is here for clarity of presentation
	   and to establish the interface. */
	int x, y;
	double inv[6];
	art_u8 *dst_p, *dst_linestart;
	const art_u8 *src_p;
	ArtPoint pt, src_pt;
	int src_x, src_y;
	int run_x0, run_x1;

	dst_linestart = dst;
	art_affine_invert (inv, affine);

	if(alpha == 255)
		for (y = y0; y < y1; y++)
		{
			pt.y = y + 0.5;
			run_x0 = x0;
			run_x1 = x1;
			ksvg_art_rgb_affine_run (&run_x0, &run_x1, y, src_width, src_height,
					inv);
			dst_p = dst_linestart + (run_x0 - x0) * 4;
			for (x = run_x0; x < run_x1; x++)
			{
				pt.x = x + 0.5;
				art_affine_point (&src_pt, &pt, inv);
				src_x = floor (src_pt.x);
				src_y = floor (src_pt.y);
				src_p = src + (src_y * src_rowstride) + src_x * 4;
#ifdef WORDS_BIGENDIAN
				dst_p[0] = dst_p[0] + (((src_p[1] - dst_p[0]) * src_p[0] + 0x80) >> 8);
        dst_p[1] = dst_p[1] + (((src_p[2] - dst_p[1]) * src_p[0] + 0x80) >> 8);
        dst_p[2] = dst_p[2] + (((src_p[3] - dst_p[2]) * src_p[0] + 0x80) >> 8);
#else
 				dst_p[0] = dst_p[0] + (((src_p[2] - dst_p[0]) * src_p[3] + 0x80) >> 8);
 				dst_p[1] = dst_p[1] + (((src_p[1] - dst_p[1]) * src_p[3] + 0x80) >> 8);
 				dst_p[2] = dst_p[2] + (((src_p[0] - dst_p[2]) * src_p[3] + 0x80) >> 8);
#endif
				dst_p += 4;
			}
			dst_linestart += dst_rowstride;
		}
	else
		for (y = y0; y < y1; y++)
		{
			pt.y = y + 0.5;
			run_x0 = x0;
			run_x1 = x1;
			ksvg_art_rgb_affine_run (&run_x0, &run_x1, y, src_width, src_height,
					inv);
			dst_p = dst_linestart + (run_x0 - x0) * 4;
			for (x = run_x0; x < run_x1; x++)
			{
				pt.x = x + 0.5;
				art_affine_point (&src_pt, &pt, inv);
				src_x = floor (src_pt.x);
				src_y = floor (src_pt.y);
				src_p = src + (src_y * src_rowstride) + src_x * 4;
				if (src_p[3] != 0)
					{
					dst_p[0] = dst_p[0] + (((src_p[2] - dst_p[0]) * alpha + 0x80) >> 8);
					dst_p[1] = dst_p[1] + (((src_p[1] - dst_p[1]) * alpha + 0x80) >> 8);
					dst_p[2] = dst_p[2] + (((src_p[0] - dst_p[2]) * alpha + 0x80) >> 8);
					}
				dst_p += 4;
			}
			dst_linestart += dst_rowstride;
		}
}

