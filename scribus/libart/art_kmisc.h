/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/* Libart_LGPL - library of basic graphic primitives
 * Copyright (C) 1998 Raph Levien
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __KSVG_ART_H__
#define __KSVG_ART_H__

#include <libart_lgpl/art_misc.h>
#include <libart_lgpl/art_bpath.h>
#include <libart_lgpl/art_vpath.h>
#include <libart_lgpl/art_alphagamma.h>
#include <libart_lgpl/art_filterlevel.h>
#include <libart_lgpl/art_svp.h>

#define ART_END2 10

#ifdef __cplusplus
extern "C" {
#endif

	 void ksvg_art_vpath_render_bez (ArtVpath **p_vpath, int *pn, int *pn_max,
				double x0, double y0,
				double x1, double y1,
				double x2, double y2,
				double x3, double y3,
				double flatness);

	 ArtVpath *ksvg_art_bez_path_to_vec(const ArtBpath *bez, double flatness);
	
	 void ksvg_art_rgb_affine_run (int *p_x0, int *p_x1, int y,
			int src_width, int src_height,
			const double affine[6]);

	 void ksvg_art_rgb_affine (art_u8 *dst, int x0, int y0, int x1, int y1, int dst_rowstride,
			const art_u8 *src,
			int src_width, int src_height, int src_rowstride,
			const double affine[6],
			ArtFilterLevel level,
			ArtAlphaGamma *alphagamma,
			int alpha);

	void ksvg_art_rgb_affine_clip(const ArtSVP *svp, art_u8 *dst, int x0, int y0, int x1, int y1, int dst_rowstride, int dst_channels,
			const art_u8 *src,
			int src_width, int src_height, int src_rowstride,
			const double affine[6],
			int alpha, const art_u8 *mask);
	
#ifdef __cplusplus
}
#endif

#endif
