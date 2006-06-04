pluginsdir = $(libdir)/scribus/plugins

plugins_LTLIBRARIES = libscribus12format.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libscribus12format_la_LDFLAGS = -version-info 0:0:0
libscribus12format_la_METASOURCES = AUTO
libscribus12format_la_SOURCES = scribus12format.cpp scribus12formatimpl.cpp

EXTRA_DIST = scribus12format.h scribus12formatimpl.h

KDE_OPTIONS = qtonly

AM_LDFLAGS = $(LIBFREETYPE_LIBS) $(MODULE_LINK_FLAGS)

AM_CPPFLAGS = \
    -I$(top_builddir)/scribus

AM_CXXFLAGS = \
    $(GCC_CXX_VISIBILITY_FLAGS)
