pluginsdir = $(prefix)/lib/scribus/plugins/gettext
AM_CPPFLAGS = -I$(top_builddir)/scribus

SUBDIRS =  

plugins_LTLIBRARIES = libtxtim.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libtxtim_la_LDFLAGS = -version-info 0:0:0
libtxtim_la_METASOURCES = AUTO
libtxtim_la_SOURCES = txtim.cpp 

EXTRA_DIST = txtim.cpp txtim.h  

KDE_OPTIONS = qtonly
AM_LDFLAGS =	-s $(LIBFREETYPE_LIBS)

