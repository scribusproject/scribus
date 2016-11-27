include_directories(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
${PYTHON_INCLUDE_PATH}
)

set(SCRIPTER_PLUGIN_MOC_CLASSES
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

set(SCRIPTER_PLUGIN_SOURCES
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


#set(PYTHONIZE "pythonize")
#set(PYTHONIZE_SOURCES "pythonize.cpp")
#add_library(${PYTHONIZE} SHARED ${PYTHONIZE_SOURCES})
#target_link_libraries(${PYTHONIZE} ${PYTHON_LIBRARIES})
#install(TARGETS ${PYTHONIZE}
#  LIBRARY
#  DESTINATION ${PLUGINDIR}
#  PERMISSIONS ${PLUGIN_PERMISSIONS}
#)


set(SCRIBUS_SCRIPTER_PLUGIN "scripter")

QT5_WRAP_CPP(SCRIPTER_PLUGIN_MOC_SOURCES ${SCRIPTER_PLUGIN_MOC_CLASSES})

add_library(${SCRIBUS_SCRIPTER_PLUGIN} MODULE ${SCRIPTER_PLUGIN_SOURCES} ${SCRIPTER_PLUGIN_MOC_SOURCES})

target_link_libraries(${SCRIBUS_SCRIPTER_PLUGIN}
  ${PYTHON_LIBRARIES}
)

target_link_libraries(${SCRIBUS_SCRIPTER_PLUGIN} ${PLUGIN_LIBRARIES})

install(TARGETS ${SCRIBUS_SCRIPTER_PLUGIN}
  LIBRARY
  DESTINATION ${PLUGINDIR}
  PERMISSIONS ${PLUGIN_PERMISSIONS}
)

file(GLOB pyfiles "python/*.py")
install(FILES ${pyfiles}
  DESTINATION ${PLUGINDIR}/scripter
)

file(GLOB autoload "python/autoload/*.py" "python/autoload/*.spy" "python/autoload/*.js" "python/autoload/*.sqts" "python/autoload/*.sjs")
install(FILES ${autoload}
  DESTINATION ${PLUGINDIR}/scripter/autoload
)

file(GLOB examples "python/examples/*.py" "python/examples/*.spy" "python/examples/*.js" "python/examples/*.sqts" "python/examples/*.sjs")
install(FILES ${examples}
  DESTINATION ${PLUGINDIR}/scripter/examples
)

file(GLOB sceditor "python/sceditor/*.zip" "python/sceditor/*.png" "python/sceditor/*.py")
install(FILES ${sceditor}
  DESTINATION ${PLUGINDIR}/scripter/sceditor
)

add_dependencies(${SCRIBUS_SCRIPTER_PLUGIN} ${EXE_NAME})
