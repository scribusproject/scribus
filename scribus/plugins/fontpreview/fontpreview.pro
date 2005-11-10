######################################################################
# 
# ColorWheel Scribus plugin
#
######################################################################
include(../../../scribus.pri)

TEMPLATE = lib
TARGET += ../fontpreview

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
            fontpreview.h \
            ui.h

SOURCES += \
            fontpreview.cpp \
            ui.cpp
