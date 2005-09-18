/*
 * art_render_pattern.c: 
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

#include "scconfig.h"
 
#include "art_render_pattern.h"

#include <math.h>

typedef struct _ArtImageSourcePattern ArtImageSourcePattern;

struct _ArtImageSourcePattern {
  ArtImageSource super;
  const ArtPattern *pattern;
};

static void
art_render_pattern_done (ArtRenderCallback *self, ArtRender *render)
{
  art_free (self);
}

static void
art_render_pattern_render (ArtRenderCallback *self, ArtRender *render,
				   art_u8 *dest, int y)
{
  ArtImageSourcePattern *z = (ArtImageSourcePattern *)self;
  const ArtPattern *pattern = z->pattern;
  int pixstride = (render->n_chan + 1) * (render->depth >> 3);
  int n_ch = render->n_chan + 1;
  int x, j, index;
  int width = render->x1 - render->x0;
  int twidth = pattern->twidth;
  int theight = pattern->theight;
  art_u8 *bufp = render->image_buf;

  double angle = pattern->angle;
  double opacity = pattern->opacity;
  double cosangle = cos(angle);
  double sinangle = sin(angle);

  y = y - render->y0;

  for (x = 0; x < width; x++)
  {
	  int x0 = sinangle * y + cosangle * x;
	  int y0 = -sinangle * x + cosangle * y;
	  x0 = (x0 % twidth);
	  if(x0 < 0) x0 += twidth;
	  y0 = (y0 % theight);
	  if(y0 < 0) y0 += theight;
	  index = (((y0 * twidth + x0) * pixstride) % (twidth * theight * 4));
	  /*for (j = 0; j < n_ch - 1; j++)
		{*/
	      /* bgra -> rgba */
          bufp[0] = pattern->buffer[index + 2];
          bufp[1] = pattern->buffer[index + 1];
          bufp[2] = pattern->buffer[index + 0];
          bufp[3] = opacity;
		/*}*/
      bufp += pixstride;
  }
}

static void
art_render_pattern_negotiate (ArtImageSource *self, ArtRender *render,
				      ArtImageSourceFlags *p_flags,
				      int *p_buf_depth, ArtAlphaType *p_alpha)
{
  self->super.render = art_render_pattern_render;
  *p_flags = 0;
  *p_buf_depth = render->depth;
  *p_alpha = ART_ALPHA_SEPARATE;
}

void
art_render_pattern (ArtRender *render,
			    const ArtPattern *pattern,
			    ArtFilterLevel level)
{
  ArtImageSourcePattern *image_source = art_new (ArtImageSourcePattern, 1);

  image_source->super.super.render = NULL;
  image_source->super.super.done = art_render_pattern_done;
  image_source->super.negotiate = art_render_pattern_negotiate;

  image_source->pattern = pattern;

  art_render_add_image_source (render, &image_source->super);
}
