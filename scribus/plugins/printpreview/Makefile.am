pluginsdir = $(libdir)/scribus/plugins
AM_CPPFLAGS = -I$(top_builddir)/scribus
plugins_LTLIBRARIES = libpreview.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libpreview_la_LDFLAGS = -version-info 0:0:0

libpreview_la_METASOURCES = AUTO

libpreview_la_SOURCES = preview.cpp 
KDE_OPTIONS = qtonly

EXTRA_DIST = preview.cpp preview.h 

if COMPILE_DEBUG
 linkflag =
else
 linkflag = -s
endif
AM_LDFLAGS =  $(linkflag) $(LIBFREETYPE_LIBS)
