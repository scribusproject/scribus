include_directories(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
${PYTHON_INCLUDE_PATH}
${SCRIBUS_AUTOGEN_INCLUDE_PATH}
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

set(SCRIBUS_SCRIPTER_PLUGIN "scripter")

add_library(${SCRIBUS_SCRIPTER_PLUGIN} MODULE ${SCRIPTER_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_SCRIPTER_PLUGIN}
  ${PYTHON_LIBRARIES}
)

target_link_libraries(${SCRIBUS_SCRIPTER_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_SCRIPTER_PLUGIN} PRIVATE "../plugins_pch.h")
endif()

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

add_dependencies(Plugins ${SCRIBUS_SCRIPTER_PLUGIN})
