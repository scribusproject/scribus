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

EXTRA_DIST = AUTHORS COPYING INSTALL README TODO NEWS PACKAGING ChangeLog ChangeLogCVS scribus.lsm scribus.desktop configure config.h.in configure.in configure.in.in configure.in.bot admin stamp-h1 subdirs configure.files

# Prevent the KDE build system from piling stuff we don't want into the
# toplevel dir of the "make dist" tarballs.
KDE_OPTIONS = noautodist

dist-hook:
	cd $(top_distdir) && perl admin/am_edit -padmin
	cd $(top_distdir) && $(MAKE) -f admin/Makefile.common subdirs
