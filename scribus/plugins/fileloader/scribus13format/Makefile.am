pluginsdir = $(libdir)/scribus/plugins

plugins_LTLIBRARIES = libscribus13format.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libscribus13format_la_LDFLAGS = -version-info 0:0:0
libscribus13format_la_METASOURCES = AUTO
libscribus13format_la_SOURCES = scribus13format.cpp scribus13formatimpl.cpp

EXTRA_DIST = scribus13format.h scribus13formatimpl.h CMakeLists.txt

KDE_OPTIONS = qtonly

AM_LDFLAGS = $(LIBFREETYPE_LIBS) $(MODULE_LINK_FLAGS)

AM_CPPFLAGS = \
    -I$(top_builddir)/scribus

AM_CXXFLAGS = \
    $(GCC_CXX_VISIBILITY_FLAGS)
