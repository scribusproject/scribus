pluginsdir = $(libdir)/scribus/plugins

# To get your plugin to build and install, change the noinst_ prefix to plugins_
noinst_LTLIBRARIES = libmyplugin.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libmyplugin_la_LDFLAGS = -version-info 0:0:0
libmyplugin_la_METASOURCES = AUTO
libmyplugin_la_SOURCES = myplugin.cpp mypluginimpl.cpp

EXTRA_DIST = cwdialogbase.ui myplugin.cpp myplugin.h mypluginwidget.cpp mypluginwidget.h cwdialog.cpp cwdialog.h CMakeLists.txt

KDE_OPTIONS = qtonly
AM_LDFLAGS = $(LIBFREETYPE_LIBS) $(MODULE_LINK_FLAGS)

AM_CPPFLAGS = \
    -I$(top_builddir)/scribus \
    -DDOCDIR=\"$(datadir)/doc/scribus-$(VERSION)/\" \
    -DICONDIR=\"$(datadir)/scribus/icons/\" \
    -DSAMPLESDIR=\"$(datadir)/scribus/samples/\" \
    -DSCRIPTSDIR=\"$(datadir)/scribus/scripts/\" \
    -DTEMPLATEDIR=\"$(datadir)/scribus/templates/\" \
    -DLIBDIR=\"$(libdir)/scribus/\" \
    -DPLUGINDIR=\"$(libdir)/scribus/plugins/\"

AM_CXXFLAGS = \
    $(GCC_CXX_VISIBILITY_FLAGS)
