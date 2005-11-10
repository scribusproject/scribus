######################################################################
# 
# CSVim Scribus plugin
#
######################################################################
include(../../../../scribus.pri)

TEMPLATE = lib
TARGET += ../../csvim

QT += qt3support xml

DEPENDPATH += \
            . \

INCLUDEPATH += \
            . \
            ../../.. \
            $$FREETYPE_INCLUDE \
            $$JPEG_INCLUDE \
            $$ZLIB_INCLUDE

# Input
HEADERS += \
            csvdia.h \
            csvim.h

SOURCES += \
            csvdia.cpp \
            csvim.cpp
