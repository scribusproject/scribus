######################################################################
# 
# MyPlugin Scribus plugin
#
######################################################################
include(../../../scribus.pri)

TEMPLATE = lib
TARGET += ../myplugin

QT += qt3support xml


DEPENDPATH += \
            .

INCLUDEPATH += \
            . \
            ../..\
            $$FREETYPE_INCLUDE \
            $$JPEG_INCLUDE


# Input
HEADERS += \
            myplugin.h \
            mypluginimpl.h

SOURCES += \
            myplugin.cpp \
            mypluginimpl.cpp
