pluginsdir = $(prefix)/lib/scribus/plugins
AM_CPPFLAGS = -I$(top_builddir)/scribus
plugins_LTLIBRARIES = libpreview.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libpreview_la_LDFLAGS = -version-info 0:0:0

libpreview_la_METASOURCES = AUTO

libpreview_la_SOURCES = preview.cpp 
KDE_OPTIONS = qtonly

EXTRA_DIST = preview.cpp preview.h 

AM_LDFLAGS =  -s $(LIBFREETYPE_LIBS) 
