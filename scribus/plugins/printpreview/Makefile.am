pluginsdir = $(libdir)/scribus/plugins
plugins_LTLIBRARIES = libpreview.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libpreview_la_LDFLAGS = -version-info 0:0:0

libpreview_la_METASOURCES = AUTO

libpreview_la_SOURCES = preview.cpp 
KDE_OPTIONS = qtonly

EXTRA_DIST = preview.cpp preview.h 

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
