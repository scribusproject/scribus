pluginsdir = $(libdir)/scribus/plugins

# Disable installation of this plug-in. You must remove this
# for your own plug-ins.
noinst_LTLIBRARIES = libmyplugin.la

# Enable this instead:
# plugins_LTLIBRARIES = libmyplugin.la

INCLUDES = $(LIBFREETYPE_CFLAGS) $(all_includes)

libmyplugin_la_LDFLAGS = -version-info 0:0:0
libmyplugin_la_METASOURCES = AUTO
libmyplugin_la_SOURCES = myplugin.cpp mypluginimpl.cpp

EXTRA_DIST = myplugin.h mypluginimpl.h

KDE_OPTIONS = qtonly

AM_LDFLAGS = $(LIBFREETYPE_LIBS) $(MODULE_LINK_FLAGS)

AM_CPPFLAGS = \
    -I$(top_builddir)/scribus

AM_CXXFLAGS = \
    $(GCC_CXX_VISIBILITY_FLAGS)
