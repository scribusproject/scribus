include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
	${SCRIBUS_AUTOGEN_INCLUDE_PATH}
)

set(COLORWHEEL_PLUGIN_SOURCES
	colorwheel.cpp
	colorwheelwidget.cpp
	cwdialog.cpp
)

set(SCRIBUS_COLORWHEEL_PLUGIN "colorwheel")

add_library(${SCRIBUS_COLORWHEEL_PLUGIN} MODULE ${COLORWHEEL_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_COLORWHEEL_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_COLORWHEEL_PLUGIN} PRIVATE "../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_COLORWHEEL_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)

add_dependencies(Plugins ${SCRIBUS_COLORWHEEL_PLUGIN})

# set_target_properties(${SCRIBUS_COLORWHEEL_PLUGIN} PROPERTIES VERSION "0.0.0")
