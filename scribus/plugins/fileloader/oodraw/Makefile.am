pluginsdir = $(prefix)/lib/scribus/plugins

SUBDIRS =  

plugins_LTLIBRARIES = liboodrawimp.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

liboodrawimp_la_LDFLAGS = -version-info 0:0:0
liboodrawimp_la_METASOURCES = AUTO
liboodrawimp_la_SOURCES = oodrawimp.cpp stylestack.cpp sxwunzip.cpp unzip.c ioapi.c

EXTRA_DIST = oodrawimp.cpp oodrawimp.h stylestack.cpp stylestack.h color.h sxwunzip.cpp sxwunzip.h unzip.c unzip.h ioapi.c ioapi.h

KDE_OPTIONS = qtonly

if COMPILE_DEBUG
 linkflag =
else
 linkflag = -s
endif
AM_LDFLAGS =  $(linkflag) $(LIBFREETYPE_LIBS)

AM_CPPFLAGS = \
    -I$(top_builddir)/scribus \
    -DDOCDIR=\"$(datadir)/scribus/doc/\" \
    -DICONDIR=\"$(datadir)/scribus/icons/\" \
    -DTEMPLATEDIR=\"$(datadir)/scribus/templates/\" \
    -DLIBDIR=\"$(libdir)/scribus/\" \
    -DPLUGINDIR=\"$(libdir)/scribus/plugins/\"

