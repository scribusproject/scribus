pluginsdir = $(libdir)/scribus/plugins
AM_CPPFLAGS = -I$(top_builddir)/scribus
plugins_LTLIBRARIES = libsvgexplugin.la
INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)
if COMPILE_DEBUG
 linkflag =
else
 linkflag = -s
endif
AM_LDFLAGS =  $(linkflag) $(LIBFREETYPE_LIBS)
libsvgexplugin_la_LDFLAGS = -version-info 0:0:0
libsvgexplugin_la_METASOURCES = AUTO
libsvgexplugin_la_SOURCES = svgexplugin.cpp 
KDE_OPTIONS = qtonly
EXTRA_DIST = svgexplugin.cpp svgexplugin.h 

