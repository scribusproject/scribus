pluginsdir = $(libdir)/scribus/plugins/gettext

SUBDIRS =  

plugins_LTLIBRARIES = libcsvim.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libcsvim_la_LDFLAGS = -version-info 0:0:0
libcsvim_la_METASOURCES = AUTO
libcsvim_la_SOURCES = csvdia.cpp csvim.cpp

EXTRA_DIST = csvdia.cpp csvdia.h csvim.cpp csvim.h  

KDE_OPTIONS = qtonly
if COMPILE_DEBUG
 linkflag = 
else
 linkflag = -s
endif
AM_LDFLAGS =  $(linkflag) $(LIBFREETYPE_LIBS) 

AM_CPPFLAGS = \
    -I$(top_builddir)/scribus \
    -DDOCDIR=\"$(datadir)/scribus/doc/\" \
    -DICONDIR=\"$(datadir)/scribus/icons/\" \
    -DSAMPLESDIR=\"$(datadir)/scribus/samples/\" \
    -DSCRIPTSDIR=\"$(datadir)/scribus/scripts/\" \
    -DTEMPLATEDIR=\"$(datadir)/scribus/templates/\" \
    -DLIBDIR=\"$(libdir)/scribus/\" \
    -DPLUGINDIR=\"$(libdir)/scribus/plugins/\"
