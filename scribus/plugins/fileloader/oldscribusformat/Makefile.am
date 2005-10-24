pluginsdir = $(libdir)/scribus/plugins

plugins_LTLIBRARIES = liboldscribusformat.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

liboldscribusformat_la_LDFLAGS = -version-info 0:0:0
liboldscribusformat_la_METASOURCES = AUTO
liboldscribusformat_la_SOURCES = oldscribusformat.cpp oldscribusformatimpl.cpp

EXTRA_DIST = oldscribusformat.h oldscribusformatimpl.h

KDE_OPTIONS = qtonly

AM_LDFLAGS = $(LIBFREETYPE_LIBS) $(MODULE_LINK_FLAGS)

AM_CPPFLAGS = \
    -I$(top_builddir)/scribus \
    $(GCC4_VISIBILITY_FLAGS)
