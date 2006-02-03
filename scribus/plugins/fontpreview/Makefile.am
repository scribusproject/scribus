pluginsdir = $(libdir)/scribus/plugins

plugins_LTLIBRARIES = libfontpreview.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libfontpreview_la_LDFLAGS = -version-info 0:0:0
libfontpreview_la_METASOURCES = AUTO
libfontpreview_la_SOURCES = fontpreviewplugin.cpp fontpreview.cpp fontpreviewbase.cpp

EXTRA_DIST = fontpreviewplugin.cpp fontpreviewplugin.h fontpreview.cpp fontpreview.h fontpreviewbase.cpp fontpreviewbase.h fontpreviewbase.ui

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

AM_CXXFLAGS = \
    $(GCC_CXX_VISIBILITY_FLAGS)
