pluginsdir = $(libdir)/scribus/plugins/gettext

SUBDIRS =  

plugins_LTLIBRARIES = libhtmlim.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(XML_CFLAGS) $(all_includes)

libhtmlim_la_LDFLAGS = -version-info 0:0:0
libhtmlim_la_METASOURCES = AUTO
libhtmlim_la_SOURCES = htmlim.cpp htmlreader.cpp 

EXTRA_DIST = htmlim.cpp htmlim.h htmlreader.cpp htmlreader.h

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
