pluginsdir = $(libdir)/scribus/plugins/gettext
AM_CPPFLAGS = -I$(top_builddir)/scribus

SUBDIRS = 

plugins_LTLIBRARIES = libtextfilterplugin.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libtextfilterplugin_la_LDFLAGS = -version-info 0:0:0
libtextfilterplugin_la_METASOURCES = AUTO
libtextfilterplugin_la_SOURCES = textfilter.cpp tfdia.cpp tffilter.cpp

EXTRA_DIST = textfilter.cpp textfilter.h tfdia.cpp tfdia.h tffilter.cpp tffilter.h

KDE_OPTIONS = qtonly

if COMPILE_DEBUG
 linkflag =
else
 linkflag = -s
endif
AM_LDFLAGS =  $(linkflag) $(LIBFREETYPE_LIBS)
