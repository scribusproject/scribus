pluginsdir = $(libdir)/scribus/plugins/gettext

SUBDIRS =

plugins_LTLIBRARIES = libodtimplugin.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(XML_CFLAGS) $(all_includes)

libodtimplugin_la_LDFLAGS = -version-info 0:0:0
libodtimplugin_la_METASOURCES = AUTO
libodtimplugin_la_SOURCES = odtim.cpp odtdia.cpp stylereader.cpp contentreader.cpp

EXTRA_DIST = odtim.cpp odtim.h odtdia.cpp odtdia.h stylereader.cpp stylereader.h contentreader.cpp contentreader.h

KDE_OPTIONS = qtonly
AM_LDFLAGS = $(LIBFREETYPE_LIBS) $(XML_LIBS) $(MODULE_LINK_FLAGS)

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
