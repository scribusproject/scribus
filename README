SCRIBUS 1.4.2
----------------------------------------------

1.4.2 is the new stable Scribus version, released January 2013.

While 1.4.2 is considered to be usable in production, if you have documents made in 1.3.3.x, you may want to keep the latest 1.3.3.14 as a fallback option for older documents due to some changes in layout. 
Both versions can be installed in parallel. Please be aware of the fact that files saved with 1.3.4 or any later version cannot be opened by 1.3.3.x!

If you think you have found a bug in 1.4.2, you can use the Scribus bugtracker (http://bugs.scribus.net) to make us aware of it. Please make sure that the bug hasn't been reported earlier. The bugtracker's search feature can help you to check for issues that have already been submitted.

Compiling from Source
=====================

The Scribus Team endevours to provide pre-built packages for most recent major Linux distributions including Debian, Fedora, Mandriva and openSUSE, along with builds for Windows and OSX. If you need to compile from source please read the BUILDING file in the sources, as it has detailed requirements and hints.


Web Resources:
==============

The Scribus home page is:

	http://www.scribus.net

The official documentation can be found after the installation of Scribus via Help > Scribus Manual (F1) or (a rather old and incomplete copy) at:

	http://docs.scribus.net

Additional documentation, as well as additional scripts, all contributed by the lively Scribus community, can be found on the Scribus wiki:

	http://wiki.scribus.net

The latest stable and development versions of Scribus (source tarballs, DMGs for OS X, Installers [*.exe] for Windows and OS/2) are available from Sourceforge:

	http://sourceforge.net/projects/scribus/files/

LANGUAGE SUPPORT
================

Scribus will usually read the language setting of your operating system/your graphical user interface. If, for some reason, the detection of the "locale" setting fails or if you want to use a different language for the Scribus user interface, you can easily change it via File > Preferences > General. You can also start Scribus from the command line via "scribus -l <lang>" or "scribus --lang <lang>" (without quotes), where <lang> has to be replaced with a locale (or language) definition. For instance, "scribus -l fr" will start Scribus with a user interface in French. To find out which languages are supported, you can use either the drop-down list in the Preferences tab mentioned above or use "scribus -la"/"scribus --langs-available" on the command line. If you choose to change the UI language via the Preferences, the change will take effect immediately. Please note that the documentation included in the Help Browser is currently only available in English, French and German. If you choose any other language for the user interface, Scribus will use the English version.


Dependencies:
=============

To install Scribus on Linux and UNIX systems, a number of dependencies needs to be resolved, some of which are optional.

See the BUILDING file and online help for further details. In general, the package manager of your distribution will resolve these dependencies for you.

If you want to build Scribus from source, you will also need the development versions of the required libraries. Please read the BUILDING file for further information.

Recommended Tools and Libraries:
================================

GhostScript
-----------

One of the more crucial tools Scribus depends on is Ghostscript (gs). While it is not required for compilation, Scribus must be able to find a recent version of Ghostscript at runtime for all features to be present and fully working. In particular, the print preview and EPS import into image frames will not be functional without Ghostscript. It is recommended to always use the latest Ghostcript version.

Support for versions older than 8.6x is untested and Scribus may not operate reliably using such an old version of gs.

You can find out which version of GhostScript Scribus is using in the dialog Help > About Scribus. If you have a newer version installed somewhere else you can specify its location under File > Preferences > External Tools.

External Tools for Render Frames
--------------------------------

Since version 1.3.5 Scribus provides a unique type of frame, called "Render Frame." The purpose of this kind of item is to use external programs to render content to a (temporary) PS, EPS, PDF or PNG file and then use the output in a Scribus frame. Currently Scribus is pre-configured for the use of LaTeX, Lilypond, gnuplot, Graphviz and POVray. In order to use any of these preconfigurations, you have to install the respective programs. For download links see: http://docs.scribus.net/index.php?lang=en&page=renderframes. This web page also provides information on how to configure Render Frames for use with other programs.

Other Recommended Tools
-----------------------

A good collection of small tools and full-fledged programs for use with Scribus is described in the toolbox section of the online help.

ICC Profiles for Color Management:
===================================

Activating Color Management *requires* installation of CMYK and RGB ICC profiles. Please see the included help files and FAQs for details. Scribus ships with 2 ICC profiles, which are installed only to enable color management functionality. (This is not true for some distribution packages, where licensing rules prevent the inclusion of these profiles). The profiles *may not* and probably *will not* provide meaningful color management for printing. Additional profiles, covering most of the printing industry's standard profiles, as well as test images and related documentation can be downloaded from http://www.colormanagement.org/index_en.html. Scribus 1.4. will probably be shipped with these (and other) profiles, since they have been released under a Creative Commons license.


Happy publishing with Scribus

The Scribus Team
