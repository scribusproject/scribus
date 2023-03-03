Compiling and Installing Scribus On Mac OS
==========================================

This document outlines the build process for Scribus 1.3.5 or higher from subversion on Mac OS. Note there are not significant differences apart from dependency versions as you go up Scribus versions. This does not currently outline the process for packaging and bundling Scribus (yet).
What you need:

    MacPorts installer (renamed from darwinports in version since 1.5) from www.macports.org
	Qt 4.5.x for Scribus 1.3, Qt 5.14.0 or higher for Scribus 1.5/1.6, and Qt 6.2.0 for 1.7/1.8.
	XCode from Apple, from the App store, or on really old mac OSes (OSX), either from the OS X install DVD or downloaded from http://developer.apple.com
	Scribus SVN

Building and Installing
=======================

Steps to build and install Scribus 1.5+ on macOS
================================================

    Install XCode from the App store
	Install MacPorts (downloadable from www.macports.org)
	Install Qt 5.14.0+ for macOS for Scribus 1.5/1.6, or Qt 6.2.0+ for macOS for Scribus 1.7/1.8 (downloadable from www.qt.io)

Installing Dependencies
=======================

Install a few ports from MacPorts. Any other required dependencies will be installed automatically if they are not already. If you have already installed macports for other software, then you should run sudo port selfupdate and sudo port upgrade installed in order that the newest versions of your current ports and libraries are installed.

    Install all the dependencies (sudo port install cmake subversion harfbuzz-icu hunspell hunspell-en_GB_ise hunspell-en_US libcdr-0.1 libetonyek libfreehand libmspub libpagemaker libqxp libvisio-0.1 libzmf podofo poppler boost)
	Optionally (recommended)sudo port install boost
	Optionally (recommended)sudo port install ghostscript
	Optionally (recommended)sudo port install graphicsmagick
	Make a directory where you want to download to (eg, in Terminal, mkdir -p ~/scribus/source)
	Change to this directory (cd ~/scribus/source)

Check out Scribus from SVN
==========================

    svn co svn://scribus.info/Scribus/trunk/
	Make a build directory mkdir builddir
	Change to the build directory (cd builddir)
	Run the cmake command: cmake -DBUILD_OSX_BUNDLE=1 -DWANT_CAIRO=1 -DCMAKE_INSTALL_PREFIX:PATH=/Users/username/Applications/Scribus.App/Contents/ ../trunk/Scribus/
	make
	make install

Run Scribus
===========

Now you can run Scribus from macOS if all has gone well.
