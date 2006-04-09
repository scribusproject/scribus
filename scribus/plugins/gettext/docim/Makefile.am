pluginsdir = $(libdir)/scribus/plugins/gettext

SUBDIRS =  

plugins_LTLIBRARIES = libdocim.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libdocim_la_LDFLAGS = -version-info 0:0:0
libdocim_la_METASOURCES = AUTO
libdocim_la_SOURCES = docim.cpp 

EXTRA_DIST = docim.cpp docim.h  

KDE_OPTIONS = qtonly

if COMPILE_DEBUG
 linkflag =
else
 linkflag = -s
endif
AM_LDFLAGS =  $(linkflag) $(LIBFREETYPE_LIBS)

AM_CPPFLAGS = \
    -I$(top_builddir)/scribus \
    -DDOCDIR=\"$(datadir)/doc/scribus-$(VERSION)/\" \
    -DICONDIR=\"$(datadir)/scribus/icons/\" \
    -DTEMPLATEDIR=\"$(datadir)/scribus/templates/\" \
    -DLIBDIR=\"$(libdir)/scribus/\" \
    -DPLUGINDIR=\"$(libdir)/scribus/plugins/\"

