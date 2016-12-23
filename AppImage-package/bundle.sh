#!/bin/bash

########################################################################
# Package the binaries built on Travis-CI as an AppImage
# By Simon Peter 2016
# For more information, see http://appimage.org/
########################################################################

export ARCH=$(arch)

APP=Scribus
LOWERAPP=${APP,,}

GIT_REV=$(git rev-parse --short HEAD)
echo $GIT_REV
make install DESTDIR=/home/travis/$APP/$APP.AppDir

cd $HOME/$APP/

wget -q https://github.com/probonopd/AppImages/raw/master/functions.sh -O ./functions.sh
. ./functions.sh

cd $APP.AppDir

########################################################################
# Copy desktop and icon file to AppDir for AppRun to pick them up
########################################################################

get_apprun

find . -name *desktop -exec cp {} $LOWERAPP.desktop \;

cp ./.local/share/scribus/icons/1_5_1/scribus.png .
rm -rf .local/share/icons/hicolor/48x48/

########################################################################
# Copy in the dependencies that cannot be assumed to be available
# on all target systems
########################################################################

# FIXME: How to find out which subset of plugins is really needed?
mkdir -p ./.local/lib/qt5/plugins/
PLUGINS=/opt/qt57/plugins/
cp -r $PLUGINS/* ./local/lib/qt5/plugins/

# Tcl/Tk, Tkinter (for Calendar script)
cp /.local/li*/python2.7/lib-dynload/_tkinter.so usr/ # It is indeed picked up here because we cd there at runtime
ldd .local/_tkinter.so | grep "=>" | awk '{print $3}' | xargs -I '{}' cp -v '{}' ./.local/lib || true
cp -r /.local/lib/tcltk usr/lib/
cp -r /.local/share/tcltk usr/share/

export LD_LIBRARY_PATH=/opt/qt57/lib/:LD_LIBRARY_PATH
copy_deps

# Move the libraries to usr/bin
move_lib
mv ./opt/qt57/lib/* usr/lib ; rm -rf ./opt
( cd .local/lib/qt5/plugins/platforms/../../ ; ln -s ../../lib/ . )
mv usr/lib/x86_64-linux-gnu/* usr/lib/

########################################################################
# Delete stuff that should not go into the AppImage
########################################################################

# Delete dangerous libraries; see
# https://github.com/probonopd/AppImages/blob/master/excludelist
delete_blacklisted

# We don't bundle the developer stuff
rm -rf .local/include || true
rm -rf .local/lib/cmake || true
rm -rf .local/lib/pkgconfig || true
find . -name '*.la' | xargs -i rm {}
strip .local/bin/* .local/lib/* || true

########################################################################
# desktopintegration asks the user on first run to install a menu item
########################################################################

get_desktopintegration $LOWERAPP

########################################################################
# Determine the version of the app; also include needed glibc version
########################################################################

GLIBC_NEEDED=$(glibc_needed)
VERSION=git$GIT_REV-glibc$GLIBC_NEEDED

########################################################################
# Patch away absolute paths; it would be nice if they were relative
########################################################################

sed -i -e 's|/.local/share/scribus|././/share/scribus|g' usr/bin/scribus
sed -i -e 's|/.local/lib/scribus|././/lib/scribus|g' usr/bin/scribus
sed -i -e 's|/.local/share/doc/scribus/|././/share/doc/scribus/|g' usr/bin/scribus
# Possibly need to patch additional hardcoded paths away, replace
# "/.local" with "././" which means ".local/ in the AppDir"

########################################################################
# AppDir complete
# Now packaging it as an AppImage
########################################################################

cd .. # Go out of AppImage

mkdir -p ../out/
generate_appimage

########################################################################
# Upload the AppDir
########################################################################

transfer ../out/*
