pluginsdir = $(prefix)/lib/scribus/plugins/gettext
AM_CPPFLAGS = -I$(top_builddir)/scribus

SUBDIRS =  

plugins_LTLIBRARIES = libhtmlim.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(XML_CFLAGS) $(all_includes)

libhtmlim_la_LDFLAGS = -version-info 0:0:0
libhtmlim_la_METASOURCES = AUTO
libhtmlim_la_SOURCES = htmlim.cpp htmlreader.cpp 

EXTRA_DIST = htmlim.cpp htmlim.h htmlreader.cpp htmlreader.h

KDE_OPTIONS = qtonly
if COMPILE_DEBUG
 linkflag = 
else
 linkflag = -s
endif
AM_LDFLAGS =  $(linkflag) $(LIBFREETYPE_LIBS) $(XML_LIBS)

