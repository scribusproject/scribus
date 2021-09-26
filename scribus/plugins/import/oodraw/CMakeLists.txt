include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
)

set(OODRAW_FL_PLUGIN_SOURCES
	oodrawimp.cpp
	stylestack.cpp
)

set(SCRIBUS_OODRAW_FL_PLUGIN "oodrawimp")

add_library(${SCRIBUS_OODRAW_FL_PLUGIN} MODULE ${OODRAW_FL_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_OODRAW_FL_PLUGIN} ${EXE_NAME} ${SCRIBUS_ZIP_LIB})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_OODRAW_FL_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_OODRAW_FL_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)
