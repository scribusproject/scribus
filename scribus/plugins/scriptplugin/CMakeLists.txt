include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/scribus
	${PYTHON_INCLUDE_PATH}
)

# Gentoo has an option for no examples. Let's make the ebuild simpler and open up the option to others, but leave as a default for us
if (NOT WANT_NOEXAMPLES)
	add_subdirectory(samples)
endif ()
add_subdirectory(scripts)

set(SCRIPTER_PLUGIN_UI_SRC
	pconsole.ui
	runscriptdialog.ui
	scripterprefsgui.ui
	prefs_scripterbase.ui
)

set(SCRIPTER_PLUGIN_MOC_CLASSES
	pconsole.h
	scriptplugin.h
	scriptercore.h
	scripterprefsgui.h
	runscriptdialog.h
	prefs_scripter.h
)

set(SCRIPTER_PLUGIN_SOURCES
	cmdannotations.cpp
	cmdcell.cpp
	cmdcolor.cpp
	cmddialog.cpp
	cmddoc.cpp
	cmdgetprop.cpp
	cmdgetsetprop.cpp
	cmdmani.cpp
	cmdmisc.cpp
	cmdobj.cpp
	cmdpage.cpp
	cmdsetprop.cpp
	cmdstyle.cpp
	cmdtable.cpp
	cmdtext.cpp
	cmdutil.cpp
	guiapp.cpp
	objimageexport.cpp
	objpdffile.cpp
	objprinter.cpp
	pconsole.cpp
	prefs_scripter.cpp
	runscriptdialog.cpp
	scriptercore.cpp
	scripterprefsgui.cpp
	scriptplugin.cpp
	svgimport.cpp
)

set(SCRIBUS_SCRIPTER_PLUGIN "scriptplugin")

QT5_WRAP_UI(SCRIPTER_PLUGIN_UI_SOURCES ${SCRIPTER_PLUGIN_UI_SRC} )
QT5_WRAP_CPP(SCRIPTER_PLUGIN_MOC_SOURCES ${SCRIPTER_PLUGIN_MOC_CLASSES})

add_library(${SCRIBUS_SCRIPTER_PLUGIN} MODULE 
	${SCRIPTER_PLUGIN_SOURCES}
	${SCRIPTER_PLUGIN_MOC_SOURCES}
	${SCRIPTER_PLUGIN_UI_SOURCES}
)

target_link_libraries(${SCRIBUS_SCRIPTER_PLUGIN}
	${PYTHON_LIBRARIES}
)

target_link_libraries(${SCRIBUS_SCRIPTER_PLUGIN} ${PLUGIN_LIBRARIES})

install(TARGETS ${SCRIBUS_SCRIPTER_PLUGIN}
	LIBRARY
	DESTINATION ${PLUGINDIR}
	PERMISSIONS ${PLUGIN_PERMISSIONS}
)

add_dependencies(${SCRIBUS_SCRIPTER_PLUGIN} ${EXE_NAME})

# set_target_properties(${SCRIBUS_SCRIPTER_PLUGIN} PROPERTIES VERSION "0.0.0")
