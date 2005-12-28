pluginsdir = $(libdir)/scribus/plugins/gettext

SUBDIRS = 

plugins_LTLIBRARIES = libtextfilterplugin.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libtextfilterplugin_la_LDFLAGS = -version-info 0:0:0
libtextfilterplugin_la_METASOURCES = AUTO
libtextfilterplugin_la_SOURCES = textfilter.cpp tfdia.cpp tffilter.cpp

EXTRA_DIST = textfilter.cpp textfilter.h tfdia.cpp tfdia.h tffilter.cpp tffilter.h

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
