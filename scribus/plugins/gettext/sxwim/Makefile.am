pluginsdir = $(prefix)/lib/scribus/plugins/gettext
AM_CPPFLAGS = -I$(top_builddir)/scribus

SUBDIRS =  

plugins_LTLIBRARIES = libsxwimplugin.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(XML_CFLAGS) $(all_includes)

libsxwimplugin_la_LDFLAGS = -version-info 0:0:0
libsxwimplugin_la_METASOURCES = AUTO
libsxwimplugin_la_SOURCES = sxwim.cpp sxwunzip.cpp sxwdia.cpp stylereader.cpp contentreader.cpp unzip.c ioapi.c

EXTRA_DIST = sxwim.cpp sxwim.h sxwunzip.cpp sxwunzip.h sxwdia.cpp sxwdia.h stylereader.cpp stylereader.h contentreader.cpp contentreader.h unzip.c unzip.h ioapi.c ioapi.h

KDE_OPTIONS = qtonly
AM_LDFLAGS =	-s $(LIBFREETYPE_LIBS) -s $(XML_LIBS)
