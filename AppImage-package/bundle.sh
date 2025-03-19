#!/bin/bash

cat /proc/1/cgroup # Check if we run in Docker; https://github.com/AppImage/AppImageKit/issues/912

# for docker images:
# if qt is not in the standard path, load its environment variables
# . /opt/qt*/bin/qt*-env.sh || true

SCRIBUS_VERSION=nightly

########################################################################
# Build Scribus and install to appdir/
########################################################################

cmake . -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
    -DCMAKE_C_COMPILER_LAUNCHER=ccache \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DWANT_DEBUG=0 \
    -DWANT_RELOCATABLE=1 \
    -DWITH_PODOFO=1 \
    -DWANT_GRAPHICSMAGICK=1 \
    -DQT_VERSION_MAJOR=6 \
    -DWANT_SVNVERSION=0
make -j$(nproc)

make DESTDIR=appdir -j$(nproc) install
#find appdir/

# TODO: make sure that the appdir directory exits
cp AppImage-package/AppRun appdir/
chmod +x appdir/AppRun

cp ./appdir/usr/share/icons/hicolor/256x256/apps/scribus.png ./appdir/
# TODO: is this needed?
sed -i -e 's|^Icon=.*|Icon=scribus|g' ./appdir/usr/share/applications/scribus.desktop

########################################################################
# Bundle everything
# to allow the AppImage to run on older systems as well
########################################################################

cd appdir/

# Bundle all of glibc; this should eventually be done by linuxdeployqt
apt-get update -q
apt-get download libc6
find *.deb -exec dpkg-deb -x {} . \;
rm *deb

# On Ubuntu 24.04 /lib is a symlink to /usr/lib
ln -s usr/lib lib
ln -s usr/lib64 lib64

# Make absolutely sure it will not load stuff from /lib or /usr
sed -i -e 's|/usr|/xxx|g' lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
sed -i -e 's|/usr/lib|/ooo/ooo|g' lib/x86_64-linux-gnu/ld-linux-x86-64.so.2

# Bundle fontconfig settings
mkdir -p etc/fonts/
cp /etc/fonts/fonts.conf etc/fonts/

# Bundle Python
apt-get download python3.12 python3.12-minimal libpython3.12-minimal libpython3.12-stdlib python3-tk
find *.deb -exec dpkg-deb -x {} . \;
rm *deb
cd -

########################################################################
# Also bundle Tcl/Tk, Tkinter (for Calendar script)
########################################################################

mkdir -p appdir/usr/lib appdir/usr/share
cp -r /usr/lib/tcltk appdir/usr/lib/
cp -r /usr/share/tcltk appdir/usr/share/

########################################################################
# Create extra qt.conf in a strange location;
# FIXME: why is this needed?
########################################################################

mkdir -p appdir/lib/x86_64-linux-gnu/
cat > appdir/lib/x86_64-linux-gnu/qt.conf <<\EOF
# Why is this needed here? Bug?
[Paths]
Prefix = ../../../usr
Plugins = plugins
Imports = qml
Qml2Imports = qml
EOF

########################################################################
# Generate AppImage
########################################################################

# Finalize AppDir but do not turn into AppImage just yet
wget -c -nv "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
chmod a+x linuxdeployqt-continuous-x86_64.AppImage

ARCH=x86_64 VERSION=$SCRIBUS_VERSION ./linuxdeployqt-continuous-x86_64.AppImage --appimage-extract-and-run appdir/usr/share/applications/scribus.desktop \
   -appimage -unsupported-bundle-everything \
   -executable=appdir/usr/bin/python3.12 \
   -executable=appdir/usr/lib/python3.12/lib-dynload/_tkinter.cpython-312-x86_64-linux-gnu.so \
   -qmake=qmake6;
