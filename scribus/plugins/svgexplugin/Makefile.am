pluginsdir = $(prefix)/lib/scribus/plugins
AM_CPPFLAGS = -I$(top_builddir)/scribus
plugins_LTLIBRARIES = libsvgexplugin.la
INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)
AM_LDFLAGS =  -s $(LIBFREETYPE_LIBS)      
libsvgexplugin_la_LDFLAGS = -version-info 0:0:0
libsvgexplugin_la_METASOURCES = AUTO
libsvgexplugin_la_SOURCES = svgexplugin.cpp 
KDE_OPTIONS = qtonly
EXTRA_DIST = svgexplugin.cpp svgexplugin.h 

