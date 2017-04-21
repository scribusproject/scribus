include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
)

set(SHORTWORDS_PLUGIN_UI_SRC
	swdialog.ui
	prefs_shortwordsbase.ui
)

set(SHORTWORDS_PLUGIN_MOC_CLASSES
	configuration.h
	parse.h
	prefs_shortwords.h
	shortwords.h
	swprefsgui.h
	swdialog.h
	swsyntaxhighlighter.h
)

set(SHORTWORDS_PLUGIN_SOURCES
	configuration.cpp
	parse.cpp
	prefs_shortwords.cpp
	shortwords.cpp
	swprefsgui.cpp
	swsyntaxhighlighter.cpp
	swdialog.cpp
)

set(SCRIBUS_SHORTWORDS_PLUGIN "scribusshortwords")

QT5_WRAP_UI(SHORTWORDS_PLUGIN_UI_SOURCES ${SHORTWORDS_PLUGIN_UI_SRC} )
QT5_WRAP_CPP(SHORTWORDS_PLUGIN_MOC_SOURCES ${SHORTWORDS_PLUGIN_MOC_CLASSES})

add_library(${SCRIBUS_SHORTWORDS_PLUGIN} MODULE
	${SHORTWORDS_PLUGIN_SOURCES}
	${SHORTWORDS_PLUGIN_MOC_SOURCES}
	${SHORTWORDS_PLUGIN_UI_SOURCES}
)

target_link_libraries(${SCRIBUS_SHORTWORDS_PLUGIN} ${PLUGIN_LIBRARIES})

install(TARGETS ${SCRIBUS_SHORTWORDS_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)

install(FILES scribus-short-words.rc
  DESTINATION ${SHAREDIR}/plugins
)

add_dependencies(${SCRIBUS_SHORTWORDS_PLUGIN} ${EXE_NAME})

# set_target_properties(${SCRIBUS_FONTPREVIEW_PLUGIN} PROPERTIES VERSION "0.0.0")
