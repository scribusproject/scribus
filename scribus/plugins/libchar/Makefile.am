pluginsdir = $(libdir)/scribus/plugins
plugins_LTLIBRARIES = libcharselect.la
AM_CPPFLAGS = -I$(top_builddir)/scribus

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libcharselect_la_LDFLAGS = -version-info 0:0:0

libcharselect_la_METASOURCES = AUTO

libcharselect_la_SOURCES = charselect.cpp 
KDE_OPTIONS = qtonly

EXTRA_DIST = charselect.cpp charselect.h 
if COMPILE_DEBUG
 linkflag = 
else
 linkflag = -s
endif
AM_LDFLAGS =  $(linkflag) $(LIBFREETYPE_LIBS) 
