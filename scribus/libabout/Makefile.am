libdir = $(prefix)/lib/scribus/libs
lib_LTLIBRARIES = libabout.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libabout_la_LDFLAGS = -version-info 0:0:0

libabout_la_METASOURCES = AUTO

libabout_la_SOURCES = about.cpp 
KDE_OPTIONS = qtonly

EXTRA_DIST = about.cpp about.h 

AM_LDFLAGS =  -s $(LIBFREETYPE_LIBS)  
