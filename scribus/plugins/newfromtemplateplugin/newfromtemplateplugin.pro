######################################################################
# 
# NewFromTemplatePlugin Scribus plugin
#
######################################################################
include(../../../scribus.pri)

TEMPLATE = lib
TARGET += ../newfromtemplateplugin

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
            nftdialog.h \
            nftemplate.h \
            nftrcreader.h \
            nftsettings.h \
            nfttemplate.h

SOURCES += \
            nftdialog.cpp \
            nftemplate.cpp \
            nftrcreader.cpp \
            nftsettings.cpp \
            nfttemplate.cpp
