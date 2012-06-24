INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
${PYTHON_INCLUDE_PATH}
)

SET(SCRIPTER_PLUGIN_MOC_CLASSES
scripter.h
scripterimpl.h
api_prefs.h
api_imageitem.h
api_item.h
api_textitem.h
api_printer.h
api_imageexport.h
api_layer.h
api_page.h
api_dialogs.h
api_window.h
api_document.h
api_color.h
)

SET(SCRIPTER_PLUGIN_SOURCES
scripter.cpp
scripterimpl.cpp
pythonize.cpp
api_prefs.cpp
api_imageitem.cpp
api_item.cpp
api_textitem.cpp
api_printer.cpp
api_imageexport.cpp
api_layer.cpp
api_page.cpp
api_dialogs.cpp
api_window.cpp
api_document.cpp
api_color.cpp
utils.cpp
)


#SET(PYTHONIZE "pythonize")
#SET(PYTHONIZE_SOURCES "pythonize.cpp")
#ADD_LIBRARY(${PYTHONIZE} SHARED ${PYTHONIZE_SOURCES})
#TARGET_LINK_LIBRARIES(${PYTHONIZE} ${PYTHON_LIBRARIES})
#INSTALL(TARGETS ${PYTHONIZE}
#  LIBRARY
#  DESTINATION ${PLUGINDIR}
#  PERMISSIONS ${PLUGIN_PERMISSIONS}
#)


SET(SCRIBUS_SCRIPTER_PLUGIN "scripter")

QT4_WRAP_CPP(SCRIPTER_PLUGIN_MOC_SOURCES ${SCRIPTER_PLUGIN_MOC_CLASSES})


ADD_LIBRARY(${SCRIBUS_SCRIPTER_PLUGIN} MODULE ${SCRIPTER_PLUGIN_SOURCES} ${SCRIPTER_PLUGIN_MOC_SOURCES})

TARGET_LINK_LIBRARIES(${SCRIBUS_SCRIPTER_PLUGIN} ${PLUGIN_LIBRARIES} ${PYTHONIZE})

INSTALL(TARGETS ${SCRIBUS_SCRIPTER_PLUGIN}
  LIBRARY
  DESTINATION ${PLUGINDIR}
  PERMISSIONS ${PLUGIN_PERMISSIONS}
)

FILE(GLOB pyfiles "python/*.py")
INSTALL(FILES ${pyfiles}
  DESTINATION ${PLUGINDIR}/scripter
)

FILE(GLOB autoload "python/autoload/*.py" "python/autoload/*.spy" "python/autoload/*.js" "python/autoload/*.sqts" "python/autoload/*.sjs")
INSTALL(FILES ${autoload}
  DESTINATION ${PLUGINDIR}/scripter/autoload
)

FILE(GLOB examples "python/examples/*.py" "python/examples/*.spy" "python/examples/*.js" "python/examples/*.sqts" "python/examples/*.sjs")
INSTALL(FILES ${examples}
  DESTINATION ${PLUGINDIR}/scripter/examples
)

FILE(GLOB sceditor "python/sceditor/*.zip" "python/sceditor/*.png" "python/sceditor/*.py")
INSTALL(FILES ${sceditor}
  DESTINATION ${PLUGINDIR}/scripter/sceditor
)

ADD_DEPENDENCIES(${SCRIBUS_SCRIPTER_PLUGIN} ${EXE_NAME})
