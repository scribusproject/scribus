pluginsdir = $(prefix)/lib/scribus/plugins/gettext
AM_CPPFLAGS = -I$(top_builddir)/scribus

SUBDIRS =  

plugins_LTLIBRARIES = libhtmlim.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libhtmlim_la_LDFLAGS = -version-info 0:0:0
libhtmlim_la_METASOURCES = AUTO
libhtmlim_la_SOURCES = htmlim.cpp htmlreader.cpp 

EXTRA_DIST = htmlim.cpp htmlim.h htmlreader.cpp htmlreader.h

KDE_OPTIONS = qtonly
AM_LDFLAGS =	-s $(LIBFREETYPE_LIBS)

