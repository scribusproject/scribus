/* GdkPixbuf library - Xlib header file
 *
 * Authors: John Harper <john@dcs.warwick.ac.uk>
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

#ifndef GDK_PIXBUF_XLIB_PRIVATE_H
#define GDK_PIXBUF_XLIB_PRIVATE_H

#include <config.h>
#include "gdk-pixbuf-xlib.h"
#include <X11/Xlib.h>

extern Display *gdk_pixbuf_dpy;
extern int gdk_pixbuf_screen;

#define KSVG_LITTLE_ENDIAN 1
#define KSVG_BIG_ENDIAN 2

#ifdef WORDS_BIGENDIAN
#define KSVG_BYTE_ORDER KSVG_BIG_ENDIAN
#else
#define KSVG_BYTE_ORDER KSVG_LITTLE_ENDIAN
#endif 
#endif
