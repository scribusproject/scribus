INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
)

#CMakeLists.txt  Makefile.am     colorblind.h    colorwheel.h          colorwheelwidget.h  cwdialog.h      cwsetcolor.h
#CVS             colorblind.cpp  colorwheel.cpp  colorwheelwidget.cpp  cwdialog.cpp        cwsetcolor.cpp


SET(COLORWHEEL_PLUGIN_MOC_CLASSES
colorwheel.h
colorwheelwidget.h
cwdialog.h
)

SET(COLORWHEEL_PLUGIN_SOURCES
colorwheel.cpp
colorwheelwidget.cpp
cwdialog.cpp
)

SET(SCRIBUS_COLORWHEEL_PLUGIN "colorwheel")

IF(QT_WRAP_CPP)
  SCRIBUS_QT_WRAP_CPP(COLORWHEEL_PLUGIN_MOC_SOURCES ${COLORWHEEL_PLUGIN_MOC_CLASSES})
ENDIF(QT_WRAP_CPP)

ADD_LIBRARY(${SCRIBUS_COLORWHEEL_PLUGIN} MODULE ${COLORWHEEL_PLUGIN_SOURCES} ${COLORWHEEL_PLUGIN_MOC_SOURCES})

INSTALL(TARGETS ${SCRIBUS_COLORWHEEL_PLUGIN}
  LIBRARY
  DESTINATION ${PLUGINDIR}
  PERMISSIONS ${PLUGIN_PERMISSIONS}
)

# SET_TARGET_PROPERTIES(${SCRIBUS_COLORWHEEL_PLUGIN} PROPERTIES VERSION "0.0.0")
