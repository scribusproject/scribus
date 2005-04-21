pluginsdir = $(libdir)/scribus/plugins

SUBDIRS =

plugins_LTLIBRARIES = libscribusexportpixmap.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libscribusexportpixmap_la_LDFLAGS = -version-info 0:0:0
libscribusexportpixmap_la_METASOURCES = AUTO
libscribusexportpixmap_la_SOURCES = export.cpp dialog.cpp

EXTRA_DIST = dialog.cpp export.cpp dialog.h export.h

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
    -DPLUGINDIR=\"$(libdir)/scribus/plugins/\"
