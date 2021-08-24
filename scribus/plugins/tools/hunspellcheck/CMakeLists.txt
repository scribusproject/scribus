include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
	${hunspell_INCLUDE_DIR}
)

set(HUNSPELL_PLUGIN_SOURCES
	hunspelldialog.cpp
	hunspelldict.cpp
	hunspellplugin.cpp
	hunspellpluginimpl.cpp
)

set(SCRIBUS_HUNSPELL_PLUGIN "hunspellplugin")


add_library(${SCRIBUS_HUNSPELL_PLUGIN} MODULE
	${HUNSPELL_PLUGIN_SOURCES}
)

target_link_libraries(${SCRIBUS_HUNSPELL_PLUGIN} ${hunspell_LIBRARIES} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_HUNSPELL_PLUGIN} PRIVATE "../../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_HUNSPELL_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)

add_dependencies(Plugins ${SCRIBUS_HUNSPELL_PLUGIN})
