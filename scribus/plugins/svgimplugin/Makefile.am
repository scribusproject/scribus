pluginsdir = $(libdir)/scribus/plugins
plugins_LTLIBRARIES = libsvgimplugin.la
INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)
AM_LDFLAGS =  $(LIBFREETYPE_LIBS) $(MODULE_LINK_FLAGS) 
libsvgimplugin_la_LDFLAGS = -version-info 0:0:0
libsvgimplugin_la_METASOURCES = AUTO
libsvgimplugin_la_SOURCES = svgplugin.cpp 
KDE_OPTIONS = qtonly
EXTRA_DIST = color.h svgplugin.cpp svgplugin.h 

AM_CPPFLAGS = \
    -I$(top_builddir)/scribus \
    -DDOCDIR=\"$(datadir)/scribus/doc/\" \
    -DICONDIR=\"$(datadir)/scribus/icons/\" \
    -DSAMPLESDIR=\"$(datadir)/scribus/samples/\" \
    -DSCRIPTSDIR=\"$(datadir)/scribus/scripts/\" \
    -DTEMPLATEDIR=\"$(datadir)/scribus/templates/\" \
    -DLIBDIR=\"$(libdir)/scribus/\" \
    -DPLUGINDIR=\"$(libdir)/scribus/plugins/\"
