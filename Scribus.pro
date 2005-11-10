######################################################################
#
# qmake project file for build Scribus, and all plugins.
#
######################################################################

TEMPLATE = subdirs

CONFIG += ordered
SUBDIRS += \
            scribus/scribus.pro

# Add plugins for everyone except Windows, as they definitly don't
# work there yet.
!win32: SUBDIRS += scribus/plugins
