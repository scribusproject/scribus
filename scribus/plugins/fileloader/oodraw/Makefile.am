pluginsdir = $(libdir)/scribus/plugins

SUBDIRS =  

plugins_LTLIBRARIES = liboodrawimp.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

liboodrawimp_la_LDFLAGS = -version-info 0:0:0
liboodrawimp_la_METASOURCES = AUTO
liboodrawimp_la_SOURCES = oodrawimp.cpp stylestack.cpp

EXTRA_DIST = oodrawimp.cpp oodrawimp.h stylestack.cpp stylestack.h color.h

KDE_OPTIONS = qtonly
AM_LDFLAGS = $(LIBFREETYPE_LIBS) $(MODULE_LINK_FLAGS) 

AM_CPPFLAGS = \
    -I$(top_builddir)/scribus \
    -DDOCDIR=\"$(datadir)/scribus/doc/\" \
    -DICONDIR=\"$(datadir)/scribus/icons/\" \
    -DSAMPLESDIR=\"$(datadir)/scribus/samples/\" \
    -DSCRIPTSDIR=\"$(datadir)/scribus/scripts/\" \
    -DTEMPLATEDIR=\"$(datadir)/scribus/templates/\" \
    -DLIBDIR=\"$(libdir)/scribus/\" \
    -DPLUGINDIR=\"$(libdir)/scribus/plugins/\" \
    $(GCC4_VISIBILITY_FLAGS)

