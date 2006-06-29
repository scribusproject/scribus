#
# spec file for package scribus (Version 1.2.4)
#
# Copyright (c) 2005 SUSE LINUX Products GmbH, Nuernberg, Germany.
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.
#
# Please submit bugfixes or comments via http://www.suse.de/feedback/
#

# norootforbuild
# neededforbuild  cups cups-devel glib2 gnome-filesystem kde3-devel-packages libxml2 libxml2-devel python python-devel shared-mime-info

BuildRequires: aaa_base acl attr bash bind-utils bison bzip2 coreutils cpio cpp cracklib cvs cyrus-sasl db devs diffutils e2fsprogs file filesystem fillup findutils flex gawk gdbm-devel gettext-devel glibc glibc-devel glibc-locale gpm grep groff gzip info insserv klogd less libacl libattr libcom_err libgcc libnscd libselinux libstdc++ libxcrypt libzio m4 make man mktemp module-init-tools ncurses ncurses-devel net-tools netcfg openldap2-client openssl pam pam-modules patch permissions popt procinfo procps psmisc pwdutils rcs readline sed strace sysvinit tar tcpd texinfo timezone unzip util-linux vim zlib zlib-devel arts arts-devel autoconf automake binutils cups cups-devel expat fam fam-devel fontconfig fontconfig-devel freeglut freeglut-devel freetype2 freetype2-devel gcc gcc-c++ gdbm gettext glib2 glib2-devel gnome-filesystem jack jack-devel kdelibs3 kdelibs3-devel kdelibs3-doc libacl-devel libart_lgpl libart_lgpl-devel libattr-devel libgcrypt libgcrypt-devel libgpg-error libgpg-error-devel libidn libidn-devel libjpeg libjpeg-devel liblcms liblcms-devel libmng libmng-devel libpng libpng-devel libstdc++-devel libtiff libtiff-devel libtool libxml2 libxml2-devel libxslt libxslt-devel openssl-devel pcre pcre-devel perl pkgconfig python python-devel qt3 qt3-devel rpm shared-mime-info unsermake update-desktop-files xorg-x11-Mesa xorg-x11-Mesa-devel xorg-x11-devel xorg-x11-libs

Name:         scribus
License:      GPL
Group:        Productivity/Publishing/Other
Autoreqprov:  on
Requires:     tk python-imaging
PreReq:       shared-mime-info
Provides:     scribus-short-words
Obsoletes:    scribus-i18n-de scribus-i18n-en scribus-i18n-fr scribus-short-words
Version:      1.2.4
Release:      1.1
Source:       %{name}-%{version}.tar.bz2
URL:          http://www.scribus.net/
BuildRoot:    %{_tmppath}/%{name}-%{version}-build
Summary:      Desktop Publishing program (DTP)

%description
Scribus is an desktop open source page layout program with the aim of producing 
commerical grade output in PDF and Postscript, primarily, though not exclusively
for Linux.

While the goals of the program are for ease of use and simple easy to
understand tools, Scribus offers support for professional publishing
features, such as CMYK color, easy PDF creation, Encapsulated Postscript
import/export and creation of color separations.


Authors:
--------
    Franz Schmid <Franz.Schmid@altmuehlnet.de>
    Paul F. Johnson <paul@all-the-johnsons.co.uk>
    Craig Bradney <cbradney@zip.com.au>
    Peter Linnell <mrdocs@scribus.info>
    Petr Van&#283;k <petr@yarpen.cz>
    Riku Leino <tsoots@welho.com>
    Craig Ringer <craig@postnewspapers.com.au>
    Alessandro Rimoldi http://ideale.ch/contact
    Jean-Jacques Sarton <jj.sarton@t-online.de>
    Christian T�p <mr-ct@gmx.de>
    Alastair Robinson <blackfive@fakenhamweb.co.uk>
    Thomas Zastrow <webmaster@thomas-zastrow.de>
    Steve Callcott <stotte@firstwish.co.uk>

%debug_package
%package devel
Summary:      Desktop Publishing program (DTP)
Group:        Productivity/Publishing/Other
Requires:     %{name} = %{version} freetype2-devel zlib-devel libstdc++-devel glibc-devel
Autoreqprov:  on

%description devel
Scribus is an desktop open source page layout program with the aim of producing 
commerical grade output in PDF and Postscript, primarily, though not exclusively
for Linux.

While the goals of the program are for ease of use and simple easy to
understand tools, Scribus offers support for professional publishing
features, such as CMYK color, easy PDF creation, Encapsulated Postscript
import/export and creation of color separations.

Authors:
--------
    Franz Schmid <Franz.Schmid@altmuehlnet.de>
    Paul F. Johnson <paul@all-the-johnsons.co.uk>
    Craig Bradney <cbradney@zip.com.au>
    Peter Linnell <mrdocs@scribus.info>
    Petr Vaněk <petr@yarpen.cz>
    Riku Leino <tsoots@welho.com>
    Craig Ringer <craig@postnewspapers.com.au>
    Alessandro Rimoldi http://ideale.ch/contact
    Jean-Jacques Sarton <jj.sarton@t-online.de>
    Christian Töpp <mr-ct@gmx.de>
    Alastair Robinson <blackfive@fakenhamweb.co.uk>
    Thomas Zastrow <webmaster@thomas-zastrow.de>
    Steve Callcott

%define prefix   /usr
%prep
%setup -q

%build
#. /etc/opt/kde3/common_options
#update_admin
export CFLAGS="$RPM_OPT_FLAGS"
export CXXFLAGS="$RPM_OPT_FLAGS"
./configure \
                --prefix=%{prefix} \
                --libdir=%{_libdir} \
                --mandir=%{_mandir} \
                --infodir=%{_infodir} \
                --sysconfdir=%{_sysconfdir} \
                --with-qt-libraries=/usr/%_lib/qt3/%_lib \
                --with-docdir=%{prefix}/share/doc/packages/scribus \
%ifarch x86_64 ppc64 s390x
		--enable-libsuffix=64 \
%endif
                --with-pythondir=%{_libdir}/python \
                --with-xinerama
make

%install
#/etc/opt/kde3/common_options
make DESTDIR=$RPM_BUILD_ROOT install
mkdir -p $RPM_BUILD_ROOT/usr/share/doc/packages/scribus
cp AUTHORS COPYING ChangeLog NEWS README PACKAGING TODO $RPM_BUILD_ROOT/usr/share/doc/packages/scribus/
%suse_update_desktop_file -i %name Office Page Layout
#rm $RPM_BUILD_ROOT%{_libdir}/scribus/plugins/*.*a $RPM_BUILD_ROOT%{_libdir}/scribus/plugins/*/*.*a
install -d $RPM_BUILD_ROOT%{_datadir}/mime/packages/
install scribus.xml $RPM_BUILD_ROOT%{_datadir}/mime/packages/scribus.xml
## one way to nuke .la files from my Fedora spec file which works
#find $RPM_BUILD_ROOT -type f -name "*.la" -exec rm -f {} ';'

%post
umask 022
[ ! -x /usr/bin/update-desktop-database ] || /usr/bin/update-desktop-database >/dev/null 2>&1 ||:
[ ! -x /usr/bin/update-mime-database ] || /usr/bin/update-mime-database %{_datadir}/mime >/dev/null 2>&1 ||:

%postun 
umask 022
[ ! -x /usr/bin/update-desktop-database ] || /usr/bin/update-desktop-database >/dev/null 2>&1
[ ! -x /usr/bin/update-mime-database ] || /usr/bin/update-mime-database %{_datadir}/mime >/dev/null 2>&1 ||:




%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-, root, root)
%doc /usr/share/doc/packages/scribus
%{_bindir}/*
/usr/share/applications/*.desktop
/usr/share/man/man?/*.*
/usr/share/man/*/man?/*.*
/usr/share/pixmaps/*
%dir %{_libdir}/scribus
%{_libdir}/scribus/*.prolog
%{_libdir}/scribus/*.txt
%{_libdir}/scribus/*.qm
%{_libdir}/scribus/dicts
%dir %{_libdir}/scribus/libs
%{_libdir}/scribus/plugins
%{_libdir}/scribus/profiles
%{_libdir}/scribus/libs/*.so.*
%{_libdir}/scribus/libs/*.so
%{_libdir}/scribus/libs/*.la
%{_prefix}/share/mime/packages/*.xml
%{_prefix}/share/scribus
# This should be owned by filesystem or man, but there are only scribus files:
%dir /usr/share/man/pl
%dir /usr/share/man/pl/man1

%files devel
%defattr(-, root, root)
#%{_libdir}/scribus/libs/*.*a
%{_prefix}/include/scribus

%changelog -n scribus
* Tue Jan 4 2006 - mrdocs@scribus.info
- don't try to remove the static files it might be breaking plugins
* Tue Jan 3 2006 - mrdocs@scribus.info
- 1.2.4 release
* Mon Nov 28 2005 - mrdocs@scribus.info
- rebuild with updated po/ts files and some fixes
* Sun Nov 20 2005 - mrdocs@scribus.info
- *.so files belong in the main package
- drop the .cs patch its fixed upstream
- update the description and Authors
* Tue Oct 25 2005 - sbrabec@suse.cz
- Updated to version 1.2.3.
- Install MIME info files.
* Wed Jul 27 2005 - sbrabec@suse.cz
- Provide and obsolete scribus-short-words.
* Wed Jul 13 2005 - sbrabec@suse.cz
- Updated to version 1.2.2.1.
- Removed unneeded static modules and .la files for modules.
* Wed Jan 26 2005 - sbrabec@suse.cz
- Obsoleted old documentation packages.
* Tue Jan 11 2005 - sbrabec@suse.cz
- Updated to version 1.2.1.
* Thu Dec 16 2004 - coolo@suse.de
- fix unsermake install
* Wed Sep 15 2004 - sbrabec@suse.cz
- Worked-around bi-arch bugs by forcing /usr/lib (#44961).
  http://bugs.scribus.net/bug_view_advanced_page.php?bug_id=0001057
* Sun Sep 05 2004 - coolo@suse.de
- fixing build requires
* Tue Aug 31 2004 - sbrabec@suse.cz
- Updated to version 1.2.
* Fri Aug 27 2004 - sbrabec@suse.cz
- Updated to version 1.2RC1.
* Thu Apr 01 2004 - ro@suse.de
- run update_admin to get current libtool macros
- some more configure hackery to adapt to updated kde-macros
  to finally find python on lib64 platforms
* Wed Mar 31 2004 - sf@suse.de
- add scribus-1.1.5-c++.diff (#37208)
* Tue Mar 30 2004 - sbrabec@suse.cz
- Updated to version 1.1.6.
* Thu Mar 04 2004 - sbrabec@suse.cz
- Updated to version 1.1.5.
* Mon Feb 23 2004 - sbrabec@suse.cz
- Fixed and turned on CUPS support.
* Sat Jan 10 2004 - adrian@suse.de
- build as user
* Fri Aug 22 2003 - sbrabec@suse.cz
- Updated to version 1.0.1.
* Fri Aug 15 2003 - adrian@suse.de
- install desktop file
* Thu Aug 14 2003 - ro@suse.de
- run update_admin (autoreconf in kde-style)
- change prefix/lib to libdir in all Makefile.am
* Tue Aug 12 2003 - sbrabec@suse.cz
- Updated to version 1.0.
* Thu Jun 12 2003 - ro@suse.de
- add doc-dir to filelist
* Mon Mar 31 2003 - ro@suse.de
- fix build on lib64
* Mon Feb 10 2003 - sbrabec@suse.cz
- Fix Qt path for biarch.
* Mon Feb 10 2003 - sbrabec@suse.cz
- Updated to version 0.9.7.
* Wed Nov 20 2002 - sbrabec@suse.cz
- Added new package.
- FHS fixes.
- Aufoconf fixes.
- KDE autoconf fixes.
