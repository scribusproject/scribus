pluginsdir = $(libdir)/scribus/libs
plugins_LTLIBRARIES = libabout.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libabout_la_LDFLAGS = -version-info 0:0:0

libabout_la_METASOURCES = AUTO

libabout_la_SOURCES = about.cpp 
KDE_OPTIONS = qtonly

EXTRA_DIST = about.cpp about.h 

if COMPILE_DEBUG
 linkflag = 
else
 linkflag = -s
endif
AM_LDFLAGS =  $(linkflag) $(LIBFREETYPE_LIBS) 

AM_CPPFLAGS = \
    -DDOCDIR=\"$(datadir)/scribus/doc/\" \
    -DICONDIR=\"$(datadir)/scribus/icons/\" \
    -DSAMPLESDIR=\"$(datadir)/scribus/samples/\" \
    -DSCRIPTSDIR=\"$(datadir)/scribus/scripts/\" \
    -DTEMPLATEDIR=\"$(datadir)/scribus/templates/\" \
    -DLIBDIR=\"$(libdir)/scribus/\" \
    -DPLUGINDIR=\"$(libdir)/scribus/plugins/\"
