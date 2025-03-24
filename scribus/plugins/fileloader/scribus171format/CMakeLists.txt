include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
)

set(SCR171FORMAT_FL_PLUGIN_SOURCES
	scribus171format.cpp
	scribus171format_save.cpp
	scribus171formatimpl.cpp
)

set(SCRIBUS_SCR171FORMAT_FL_PLUGIN "scribus171format")



add_library(${SCRIBUS_SCR171FORMAT_FL_PLUGIN} MODULE ${SCR171FORMAT_FL_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_SCR171FORMAT_FL_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_SCR171FORMAT_FL_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_SCR171FORMAT_FL_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)
