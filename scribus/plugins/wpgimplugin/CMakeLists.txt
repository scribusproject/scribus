INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/scribus
)
LINK_DIRECTORIES(${PLUGIN_LIBRARIES})

SET(IMPORTWPG_PLUGIN_MOC_CLASSES
    importwpg.h
    importwpgplugin.h
  )
SET(IMPORTWPG_PLUGIN_SOURCES
    importwpg.cpp
    importwpgplugin.cpp
    wpg/WPG1Parser.cpp
    wpg/WPG2Parser.cpp
    wpg/WPGBinaryData.cpp
    wpg/WPGBitmap.cpp
    wpg/WPGBrush.cpp
    wpg/WPGColor.cpp
    wpg/WPGGradient.cpp
    wpg/WPGHeader.cpp
    wpg/WPGInternalStream.cpp
    wpg/WPGOLEStream.cpp
    wpg/WPGPath.cpp
    wpg/WPGPen.cpp
    wpg/WPGPoint.cpp
    wpg/WPGraphics.cpp
    wpg/WPGRect.cpp
    wpg/WPGStreamImplementation.cpp
    wpg/WPGString.cpp
    wpg/WPGXParser.cpp
  )
SET(SCRIBUS_IMPORTWPG_PLUGIN "importwpg")
QT4_WRAP_CPP(IMPORTWPG_PLUGIN_MOC_SOURCES ${IMPORTWPG_PLUGIN_MOC_CLASSES})
ADD_LIBRARY(${SCRIBUS_IMPORTWPG_PLUGIN} MODULE ${IMPORTWPG_PLUGIN_SOURCES} ${IMPORTWPG_PLUGIN_MOC_SOURCES})
TARGET_LINK_LIBRARIES(${SCRIBUS_IMPORTWPG_PLUGIN} ${PLUGIN_LIBRARIES})
INSTALL(TARGETS ${SCRIBUS_IMPORTWPG_PLUGIN}
    LIBRARY
    DESTINATION ${PLUGINDIR}
    PERMISSIONS ${PLUGIN_PERMISSIONS}
)
ADD_DEPENDENCIES(${SCRIBUS_IMPORTWPG_PLUGIN} ${EXE_NAME})
