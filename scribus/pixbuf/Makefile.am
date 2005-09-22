# Don't install the gdk-pixbuf library, it's only a convenience lib
# to make conditional compilation work correctly.
noinst_LTLIBRARIES=libpixbuf.la

libpixbuf_la_SOURCES=gdk-pixbuf-xlib.c gdk-pixbuf-xlib.h gdk-pixbuf-xlib-render.c  gdk-pixbuf-xlibrgb.h gdk-pixbuf-xlib-drawable.c gdk-pixbuf-xlib-private.h  gdk-pixbuf-xlibrgb.c

EXTRA_DIST = gdk-pixbuf-xlib-private.h gdk-pixbuf-xlib.h gdk-pixbuf-xlibrgb.h

INCLUDES = $(all_includes)

libpixbuf_la_METASOURCES=AUTO
AM_LDFLAGS = 
AM_CFLAGS= -I$(top_builddir)/scribus -I$(top_srcdir)/scribus
