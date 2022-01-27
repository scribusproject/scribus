include_directories(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
)

set(PDB_GT_PLUGIN_SOURCES
	pdbim.cpp
)

set(SCRIBUS_PDB_GT_PLUGIN "pdbim")

add_library(${SCRIBUS_PDB_GT_PLUGIN} MODULE ${PDB_GT_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_PDB_GT_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_PDB_GT_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_PDB_GT_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}gettext
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)

