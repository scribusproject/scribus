pluginsdir = $(libdir)/scribus/plugins
AM_CPPFLAGS = -I$(top_builddir)/scribus

plugins_LTLIBRARIES = libfontpreview.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libfontpreview_la_LDFLAGS = -version-info 0:0:0
libfontpreview_la_METASOURCES = AUTO
libfontpreview_la_SOURCES = ui.cpp fontpreview.cpp

EXTRA_DIST = ui.cpp fontpreview.cpp ui.h fontpreview.h

KDE_OPTIONS = qtonly
AM_LDFLAGS =    -s $(LIBFREETYPE_LIBS)

