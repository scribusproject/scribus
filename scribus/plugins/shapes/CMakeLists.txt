include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
)

set(SHAPE_PLUGIN_SOURCES
	shapepalette.cpp
	shapeplugin.cpp
)

set(SCRIBUS_SHAPE_PLUGIN "shapeplugin")



add_library(${SCRIBUS_SHAPE_PLUGIN} MODULE
	${SHAPE_PLUGIN_SOURCES}
)

target_link_libraries(${SCRIBUS_SHAPE_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_SHAPE_PLUGIN} PRIVATE "../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_SHAPE_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)

add_dependencies(Plugins ${SCRIBUS_SHAPE_PLUGIN})

