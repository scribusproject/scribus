pluginsdir = $(libdir)/scribus/plugins/gettext

SUBDIRS =  

plugins_LTLIBRARIES = libpdbim.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libpdbim_la_LDFLAGS = -version-info 0:0:0
libpdbim_la_METASOURCES = AUTO
libpdbim_la_SOURCES = pdbim.cpp

EXTRA_DIST = pdbim.cpp pdbim.h  

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
