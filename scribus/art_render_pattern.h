/*
 * art_render_pattern.h: 
 *
 * Libart_LGPL - library of basic graphic primitives
 * Copyright (C) 2000 Raph Levien
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
 *
 */

#ifndef __ART_RENDER_PATTERN_H__
#define __ART_RENDER_PATTERN_H__

#ifdef LIBART_COMPILATION
#include "art_filterlevel.h"
#include "art_render.h"
#else
#include <libart_lgpl/art_filterlevel.h>
#include <libart_lgpl/art_render.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _ArtPattern ArtPattern;

struct _ArtPattern {
  unsigned int twidth;		/* width of tile */
  unsigned int theight;		/* height of tile */
  art_u8 opacity; 			/* opacity level */
  double angle;
  art_u8 *buffer;			/* image source */
};

void
art_render_pattern (ArtRender *render,
			    const ArtPattern *pattern,
			    ArtFilterLevel level);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ART_RENDER_PATTERN_H__ */
