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
