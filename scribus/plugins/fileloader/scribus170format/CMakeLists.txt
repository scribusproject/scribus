include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
)

set(SCR170FORMAT_FL_PLUGIN_SOURCES
	scribus170format.cpp
	scribus170format_save.cpp
	scribus170formatimpl.cpp
)

set(SCRIBUS_SCR170FORMAT_FL_PLUGIN "scribus170format")



add_library(${SCRIBUS_SCR170FORMAT_FL_PLUGIN} MODULE ${SCR170FORMAT_FL_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_SCR170FORMAT_FL_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_SCR170FORMAT_FL_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_SCR170FORMAT_FL_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)
