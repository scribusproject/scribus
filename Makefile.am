SUBDIRS = $(TOPSUBDIRS)

$(top_srcdir)/configure.in: configure.in.in $(top_srcdir)/subdirs
	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common configure.in ;

$(top_srcdir)/subdirs:
	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common subdirs

$(top_srcdir)/acinclude.m4: $(top_srcdir)/admin/acinclude.m4.in $(top_srcdir)/admin/libtool.m4.in
	@cd $(top_srcdir) && cat admin/acinclude.m4.in admin/libtool.m4.in > acinclude.m4

MAINTAINERCLEANFILES = subdirs configure.in acinclude.m4 configure.files 

package-messages:
	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common package-messages
	$(MAKE) -C po merge

# Prevent the KDE build system from piling stuff we don't want into the
# toplevel dir of the "make dist" tarballs.
KDE_OPTIONS = noautodist

# We have to list everything we want to go into the root level of the tarball
# in EXTRA_DIST, except dirs listed in SUBDIRS.
#
# We have to include configure.in and friends because the auto-generated make
# rules to re-generate configure if its .in files are modified cause the build
# to fail if those files are absent.
#
# We also have to list anything we want included from the admin dir that the
# build magic doesn't know to include in dist tarballs automatically. In this
# case, we include enough to handle the auto-regeneration of configure after
# configure.in, aclocal.m4, or acinclude.m4 are changed, since that's done
# using rules defined here and not in the admin magic.
# DO NOT INCLUDE THE "admin" DIR, ONLY FILES WITHIN IT. Otherwise you'll get
# *everything* in admin, including the CVS dirs etc.
#

EXTRA_DIST = AUTHORS COPYING INSTALL README TODO NEWS PACKAGING BUILDING ChangeLog ChangeLogCVS \
			 scribus.lsm scribus.desktop \
			 configure config.h.in configure.in configure.in.in configure.in.bot Makefile.cvs \
			 stamp-h1 subdirs configure.files \
			 admin/acinclude.m4.in admin/libtool.m4.in admin/Makefile.common \
			 admin/cvs.sh admin/am_edit admin/detect-autoconf.sh

dist-hook:
	cd $(top_distdir) && perl admin/am_edit -padmin
	cd $(top_distdir) && $(MAKE) -f admin/Makefile.common subdirs

AM_CPPFLAGS = \
    -DDOCDIR=\"$(datadir)/scribus/doc/\" \
    -DICONDIR=\"$(datadir)/scribus/icons/\" \
    -DSAMPLESDIR=\"$(datadir)/scribus/samples/\" \
    -DSCRIPTSDIR=\"$(datadir)/scribus/scripts/\" \
    -DTEMPLATEDIR=\"$(datadir)/scribus/templates/\" \
    -DLIBDIR=\"$(libdir)/scribus/\" \
    -DPLUGINDIR=\"$(libdir)/scribus/plugins/\"
