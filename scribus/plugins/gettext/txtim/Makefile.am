pluginsdir = $(libdir)/scribus/plugins/gettext
AM_CPPFLAGS = -I$(top_builddir)/scribus

SUBDIRS =  

plugins_LTLIBRARIES = libtxtim.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libtxtim_la_LDFLAGS = -version-info 0:0:0
libtxtim_la_METASOURCES = AUTO
libtxtim_la_SOURCES = txtim.cpp 

EXTRA_DIST = txtim.cpp txtim.h  

KDE_OPTIONS = qtonly
if COMPILE_DEBUG
 linkflag = 
else
 linkflag = -s
endif
AM_LDFLAGS =  $(linkflag) $(LIBFREETYPE_LIBS) 

