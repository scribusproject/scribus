pluginsdir = $(libdir)/scribus/plugins

plugins_LTLIBRARIES = libscribus134format.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libscribus134format_la_LDFLAGS = -version-info 0:0:0
libscribus134format_la_METASOURCES = AUTO
libscribus134format_la_SOURCES = scribus134format.cpp scribus134formatimpl.cpp

EXTRA_DIST = scribus134format.h scribus134formatimpl.h

KDE_OPTIONS = qtonly

AM_LDFLAGS = $(LIBFREETYPE_LIBS) $(MODULE_LINK_FLAGS)

AM_CPPFLAGS = \
    -I$(top_builddir)/scribus

AM_CXXFLAGS = \
    $(GCC_CXX_VISIBILITY_FLAGS)
