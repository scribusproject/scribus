pluginsdir = $(libdir)/scribus/plugins
AM_CPPFLAGS = -I$(top_builddir)/scribus
plugins_LTLIBRARIES = libcharselect.la

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
