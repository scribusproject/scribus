pluginsdir = $(libdir)/scribus/plugins

plugins_LTLIBRARIES = libfontpreview.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libfontpreview_la_LDFLAGS = -version-info 0:0:0
libfontpreview_la_METASOURCES = AUTO
libfontpreview_la_SOURCES = ui.cpp fontpreview.cpp

EXTRA_DIST = ui.cpp fontpreview.cpp ui.h fontpreview.h

KDE_OPTIONS = qtonly
if COMPILE_DEBUG
 linkflag = 
else
 linkflag = -s
endif
AM_LDFLAGS =  $(linkflag) $(LIBFREETYPE_LIBS) $(MODULE_LINK_FLAGS)

AM_CPPFLAGS = \
    -I$(top_builddir)/scribus \
    -DDOCDIR=\"$(datadir)/scribus/doc/\" \
    -DICONDIR=\"$(datadir)/scribus/icons/\" \
    -DSAMPLESDIR=\"$(datadir)/scribus/samples/\" \
    -DSCRIPTSDIR=\"$(datadir)/scribus/scripts/\" \
    -DTEMPLATEDIR=\"$(datadir)/scribus/templates/\" \
    -DLIBDIR=\"$(libdir)/scribus/\" \
    -DPLUGINDIR=\"$(libdir)/scribus/plugins/\"
