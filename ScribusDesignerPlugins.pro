QTDIR_build {
# This is only for the Qt build. Do not use externally. We mean it.
PLUGIN_TYPE = designer
PLUGIN_CLASS_NAME = ScribusUIWidgets
load(qt_plugin)
CONFIG += install_ok
} else {
# Public example:

TEMPLATE = lib
CONFIG  += plugin

TARGET   = $$qtLibraryTarget($$TARGET)

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS += target

}

DEFINES += "EXCLUDE_FOR_DESIGNER_PLUGIN=1"

QT      += widgets designer #uitools

HEADERS += \
    scribus/designer/formwidget_plugin.h \
    scribus/designer/sectioncontainer_plugin.h \
    scribus/designer/sectioncontainer_extension.h \
    scribus/designer/sectioncontainer_extensionfactory.h \
    scribus/designer/uiwidgets.h \    
    scribus/ui/widgets/form_widget.h \
    scribus/ui/widgets/section_container.h

SOURCES += \
    scribus/designer/formwidget_plugin.cpp \
    scribus/designer/sectioncontainer_plugin.cpp \
    scribus/designer/sectioncontainer_extension.cpp \
    scribus/designer/sectioncontainer_extensionfactory.cpp \
    scribus/designer/uiwidgets.cpp \    
    scribus/ui/widgets/form_widget.cpp \
    scribus/ui/widgets/section_container.cpp

