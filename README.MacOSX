THIS IS OUTDATED!

Some preliminary notes:

Scribus.app contains some files for the OSX application bundle infrastructure:

Contents/Info.plist application properties
Contents/Resources  application icons
Contents/MacOS      application binary

Currently you have to copy the Info.plist and the icon files to the install location yourself
and create a sysmlink in MacOS:

bash$ cd Contents/MacOS
bash$ ln -s ../bin/scribus Scribus

Mac OS X build support is being continuously improved in 1.3.5svn, and CMake 
is able to produce a .app bundle on install.
