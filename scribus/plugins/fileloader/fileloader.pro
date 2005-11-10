######################################################################
# 
# OODraw Scribus plugin
#
######################################################################
include(../../../scribus.pri)

TEMPLATE = lib
TARGET += ../oodraw

QT += qt3support xml

DEPENDPATH += \
            . \
            oodraw

INCLUDEPATH += \
            . \
            ../..\
            $$FREETYPE_INCLUDE \
            $$JPEG_INCLUDE \
            $$ZLIB_INCLUDE

# Input
HEADERS += \
            oodraw/color.h \
            oodraw/oodrawimp.h \
            oodraw/stylestack.h
            
SOURCES += \
            oodraw/oodrawimp.cpp \
            oodraw/stylestack.cpp
