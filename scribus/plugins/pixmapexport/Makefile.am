pluginsdir = $(libdir)/scribus/plugins
AM_CPPFLAGS = -I$(top_builddir)/scribus

SUBDIRS =

plugins_LTLIBRARIES = libscribusexportpixmap.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libscribusexportpixmap_la_LDFLAGS = -version-info 0:0:0
libscribusexportpixmap_la_METASOURCES = AUTO
libscribusexportpixmap_la_SOURCES = export.cpp dialog.cpp

EXTRA_DIST = dialog.cpp export.cpp dialog.h export.h

KDE_OPTIONS = qtonly
if COMPILE_DEBUG
 linkflag = 
else
 linkflag = -s
endif
AM_LDFLAGS =  $(linkflag) $(LIBFREETYPE_LIBS) 
