include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
	${SCRIBUS_AUTOGEN_INCLUDE_PATH}
)

set(SHORTWORDS_PLUGIN_SOURCES
	configuration.cpp
	parse.cpp
	prefs_shortwords.cpp
	shortwords.cpp
	swsyntaxhighlighter.cpp
	swdialog.cpp
)

set(SCRIBUS_SHORTWORDS_PLUGIN "scribusshortwords")

add_library(${SCRIBUS_SHORTWORDS_PLUGIN} MODULE
	${SHORTWORDS_PLUGIN_SOURCES}
)

target_link_libraries(${SCRIBUS_SHORTWORDS_PLUGIN} ${EXE_NAME})

if(WANT_PCH)
	target_precompile_headers(${SCRIBUS_SHORTWORDS_PLUGIN} PRIVATE "../plugins_pch.h")
endif()

install(TARGETS ${SCRIBUS_SHORTWORDS_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)

install(FILES scribus-short-words.rc
  DESTINATION ${SHAREDIR}/plugins
)

add_dependencies(Plugins ${SCRIBUS_SHORTWORDS_PLUGIN})
