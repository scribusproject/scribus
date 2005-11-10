######################################################################
# 
# HTMLim Scribus plugin
#
######################################################################
include(../../../../scribus.pri)

TEMPLATE = lib
TARGET += ../../htmlim

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
            htmlim.h \
            htmlreader.h

SOURCES += \
            htmlim.cpp \
            htmlreader.cpp
