include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
)

set(SCR150FORMAT_FL_PLUGIN_SOURCES
	scribus150format.cpp
	scribus150format_save.cpp
	scribus150formatimpl.cpp
)

set(SCRIBUS_SCR150FORMAT_FL_PLUGIN "scribus150format")



add_library(${SCRIBUS_SCR150FORMAT_FL_PLUGIN} MODULE ${SCR150FORMAT_FL_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_SCR150FORMAT_FL_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_SCR150FORMAT_FL_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_SCR150FORMAT_FL_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)
