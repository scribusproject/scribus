pluginsdir = $(libdir)/scribus/plugins

plugins_LTLIBRARIES = libcolorwheel.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libcolorwheel_la_LDFLAGS = -version-info 0:0:0
libcolorwheel_la_METASOURCES = AUTO
libcolorwheel_la_SOURCES = colorwheel.cpp colorwheelwidget.cpp cwdialog.cpp colorblind.cpp cwsetcolor.cpp

EXTRA_DIST = colorwheel.cpp colorwheel.h colorwheelwidget.cpp colorwheelwidget.h cwdialog.cpp cwdialog.h colorblind.cpp colorblind.h cwsetcolor.cpp cwsetcolor.h

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
    -DPLUGINDIR=\"$(libdir)/scribus/plugins/\" \
    $(GCC4_VISIBILITY_FLAGS)
