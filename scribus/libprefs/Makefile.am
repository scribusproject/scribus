pluginsdir = $(libdir)/scribus/libs
plugins_LTLIBRARIES = libpreferences.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libpreferences_la_LDFLAGS = -version-info 0:0:0

libpreferences_la_METASOURCES = AUTO

libpreferences_la_SOURCES = prefs.cpp 
KDE_OPTIONS = qtonly

EXTRA_DIST = prefs.cpp prefs.h 
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
