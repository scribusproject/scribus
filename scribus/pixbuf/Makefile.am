# Don't install the gdk-pixbuf library, it's only a convenience lib
# to make conditional compilation work correctly.
noinst_LTLIBRARIES=libpixbuf.la

libpixbuf_la_SOURCES=art_kmisc.c art_render_misc.c art_render_pattern.c art_rgba_affine.c art_rgb_affine_private.c art_rgb.c art_rgb_svp.c gdk-pixbuf-xlib.c gdk-pixbuf-xlib.h gdk-pixbuf-xlib-render.c  gdk-pixbuf-xlibrgb.h gdk-pixbuf-xlib-drawable.c gdk-pixbuf-xlib-private.h  gdk-pixbuf-xlibrgb.c

EXTRA_DIST = art_kmisc.h art_render_misc.h art_render_pattern.h art_rgba_affine.h art_rgb_affine_private.h art_rgb.h gdk-pixbuf-xlib-private.h gdk-pixbuf-xlib.h gdk-pixbuf-xlibrgb.h

INCLUDES = $(LIBART_CFLAGS) $(all_includes)

libpixbuf_la_METASOURCES=AUTO
AM_LDFLAGS = $(LIBART_LIBS)
AM_CFLAGS= -I$(top_builddir)/scribus -I$(top_srcdir)/scribus
