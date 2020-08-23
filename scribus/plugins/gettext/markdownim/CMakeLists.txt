#set(PLUGIN_QT_MINIMUM_VERSION 5.14.0)

#if(Qt5Widgets_VERSION LESS PLUGIN_QT_MINIMUM_VERSION)
#	MESSAGE(STATUS "Qt based Markdown plugin: Qt version < ${PLUGIN_QT_MINIMUM_VERSION} so skipping build, Installed version: ${Qt5Widgets_VERSION}")
#else()
#	MESSAGE(STATUS "Qt based Markdown plugin: Qt version >= ${PLUGIN_QT_MINIMUM_VERSION} Ok, Installed Qt version: ${Qt5Widgets_VERSION}")

	include_directories(
		${CMAKE_SOURCE_DIR}
		${CMAKE_SOURCE_DIR}/scribus
	)

    set(MARKDOWN_IM_PLUGIN_SOURCES
		markdownim.cpp
	)

    set(SCRIBUS_MARKDOWN_IM_PLUGIN "MARKDOWNimplugin")

	add_library(${SCRIBUS_MARKDOWN_IM_PLUGIN} MODULE ${MARKDOWN_IM_PLUGIN_SOURCES})

	target_link_libraries(${SCRIBUS_MARKDOWN_IM_PLUGIN} ${PLUGIN_LIBRARIES} ${SCRIBUS_MARKDOWN_LIB})

	install(TARGETS ${SCRIBUS_MARKDOWN_IM_PLUGIN}
		LIBRARY
		DESTINATION ${PLUGINDIR}gettext
		PERMISSIONS ${PLUGIN_PERMISSIONS}
	)

    # set_target_properties(${SCRIBUS_FONTPREVIEW_PLUGIN} PROPERTIES VERSION "0.0.0")

#endif()
