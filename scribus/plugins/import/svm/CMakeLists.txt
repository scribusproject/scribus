include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
	${SCRIBUS_AUTOGEN_INCLUDE_PATH}
)

set(IMPORTSVM_PLUGIN_SOURCES
	importsvm.cpp
	importsvmplugin.cpp
)

set(SCRIBUS_IMPORTSVM_PLUGIN "importsvm")

add_library(${SCRIBUS_IMPORTSVM_PLUGIN} MODULE ${IMPORTSVM_PLUGIN_SOURCES})

target_link_libraries(${SCRIBUS_IMPORTSVM_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_IMPORTSVM_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_IMPORTSVM_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)

add_dependencies(Plugins ${SCRIBUS_IMPORTSVM_PLUGIN})
