pluginsdir = $(libdir)/scribus/plugins
AM_CPPFLAGS = -I$(top_builddir)/scribus

SUBDIRS =  

plugins_LTLIBRARIES = libsaveastemplateplugin.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libsaveastemplateplugin_la_LDFLAGS = -version-info 0:0:0
libsaveastemplateplugin_la_METASOURCES = AUTO
libsaveastemplateplugin_la_SOURCES = satemplate.cpp satdialog.cpp 

EXTRA_DIST = satemplate.cpp satemplate.h satdialog.cpp satdialog.h

KDE_OPTIONS = qtonly
if COMPILE_DEBUG
 linkflag = 
else
 linkflag = -s
endif
AM_LDFLAGS =  $(linkflag) $(LIBFREETYPE_LIBS) 

