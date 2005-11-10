######################################################################
# 
# TXTim Scribus plugin
#
######################################################################
include(../../../../scribus.pri)

TEMPLATE = lib
TARGET += ../../txtim

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
            txtim.h

SOURCES += \
            txtim.cpp
