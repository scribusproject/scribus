######################################################################
# 
# ColorWheel Scribus plugin
#
######################################################################
include(../../../scribus.pri)

TEMPLATE = lib
TARGET += ../colorwheel

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
            colorblind.h \
            colorwheel.h \
            colorwheelwidget.h \
            cwdialog.h

SOURCES += \
            colorblind.cpp \
            colorwheel.cpp \
            colorwheelwidget.cpp \
            cwdialog.cpp
