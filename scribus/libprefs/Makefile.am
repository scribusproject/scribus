libdir = $(prefix)/lib/scribus/libs
AM_CPPFLAGS = -I$(top_builddir)/scribus
lib_LTLIBRARIES = libpreferences.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libpreferences_la_LDFLAGS = -version-info 0:0:0

libpreferences_la_METASOURCES = AUTO

libpreferences_la_SOURCES = prefs.cpp 
KDE_OPTIONS = qtonly

EXTRA_DIST = prefs.cpp prefs.h 
AM_LDFLAGS =  -s $(LIBFREETYPE_LIBS) 
