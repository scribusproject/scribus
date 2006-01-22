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

#ifndef __ART_RGB_AFFINE_H__
#define __ART_RGB_AFFINE_H__

/* This module handles compositing of affine-transformed rgb images
   over rgb pixel buffers. */

#ifdef LIBART_COMPILATION
#include "art_filterlevel.h"
#include "art_alphagamma.h"
#else
#include <libart_lgpl/art_filterlevel.h>
#include <libart_lgpl/art_alphagamma.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

void
art_rgba_affine (art_u8 *dst, int x0, int y0, int x1, int y1, int dst_rowstride,
		const art_u8 *src,
		int src_width, int src_height, int src_rowstride,
		const double affine[6],
		ArtFilterLevel level,
		ArtAlphaGamma *alphagamma);

#ifdef __cplusplus
}
#endif

#endif
