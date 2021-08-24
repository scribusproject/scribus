include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
)

set(IMPORTWMF_PLUGIN_SOURCES
	wmfcontext.cpp
	wmfimport.cpp
	wmfimportplugin.cpp
)

set(SCRIBUS_IMPORTWMF_PLUGIN "wmfimplugin")

add_library(${SCRIBUS_IMPORTWMF_PLUGIN} MODULE ${IMPORTWMF_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_IMPORTWMF_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_IMPORTWMF_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_IMPORTWMF_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)

