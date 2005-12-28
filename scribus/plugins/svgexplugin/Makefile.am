pluginsdir = $(libdir)/scribus/plugins
plugins_LTLIBRARIES = libsvgexplugin.la
INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)
AM_LDFLAGS = $(LIBFREETYPE_LIBS) $(MODULE_LINK_FLAGS) 
libsvgexplugin_la_LDFLAGS = -version-info 0:0:0
libsvgexplugin_la_METASOURCES = AUTO
libsvgexplugin_la_SOURCES = svgexplugin.cpp 
KDE_OPTIONS = qtonly
EXTRA_DIST = svgexplugin.cpp svgexplugin.h 

AM_CPPFLAGS = \
    -I$(top_builddir)/scribus \
    -DDOCDIR=\"$(datadir)/scribus/doc/\" \
    -DICONDIR=\"$(datadir)/scribus/icons/\" \
    -DSAMPLESDIR=\"$(datadir)/scribus/samples/\" \
    -DSCRIPTSDIR=\"$(datadir)/scribus/scripts/\" \
    -DTEMPLATEDIR=\"$(datadir)/scribus/templates/\" \
    -DLIBDIR=\"$(libdir)/scribus/\" \
    -DPLUGINDIR=\"$(libdir)/scribus/plugins/\"

AM_CXXFLAGS = \
    $(GCC_CXX_VISIBILITY_FLAGS)
