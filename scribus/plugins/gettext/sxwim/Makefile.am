pluginsdir = $(libdir)/scribus/plugins/gettext

SUBDIRS =  

plugins_LTLIBRARIES = libsxwimplugin.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(XML_CFLAGS) $(all_includes)

libsxwimplugin_la_LDFLAGS = -version-info 0:0:0
libsxwimplugin_la_METASOURCES = AUTO
libsxwimplugin_la_SOURCES = sxwim.cpp sxwdia.cpp stylereader.cpp contentreader.cpp

EXTRA_DIST = sxwim.cpp sxwim.h sxwunzip.cpp sxwdia.cpp sxwdia.h stylereader.cpp stylereader.h contentreader.cpp contentreader.h

KDE_OPTIONS = qtonly
if COMPILE_DEBUG
 linkflag = 
else
 linkflag = -s
endif
AM_LDFLAGS =  $(linkflag) $(LIBFREETYPE_LIBS) $(XML_LIBS)

AM_CPPFLAGS = \
    -I$(top_builddir)/scribus \
    -DDOCDIR=\"$(datadir)/scribus/doc/\" \
    -DICONDIR=\"$(datadir)/scribus/icons/\" \
    -DSAMPLESDIR=\"$(datadir)/scribus/samples/\" \
    -DSCRIPTSDIR=\"$(datadir)/scribus/scripts/\" \
    -DTEMPLATEDIR=\"$(datadir)/scribus/templates/\" \
    -DLIBDIR=\"$(libdir)/scribus/\" \
    -DPLUGINDIR=\"$(libdir)/scribus/plugins/\"
