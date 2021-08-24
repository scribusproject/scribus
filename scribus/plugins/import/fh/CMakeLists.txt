include(FindPkgConfig)
if (HAVE_REVENGE)
	pkg_check_modules(LIBFREEHAND libfreehand-0.1)
	if (LIBFREEHAND_FOUND)
		message("Building Freehand Importer with librevenge")
		include_directories(
			${CMAKE_SOURCE_DIR}
			${CMAKE_SOURCE_DIR}/scribus
			${LIBREVENGE_INCLUDE_DIRS}
			${LIBREVENGE_GENERATORS_INCLUDE_DIRS}
			${LIBREVENGE_STREAM_INCLUDE_DIRS}
			${LIBFREEHAND_INCLUDE_DIRS}
			${SCRIBUS_AUTOGEN_INCLUDE_PATH}
			)
		set(IMPORTFH_PLUGIN_SOURCES ../revenge/rawpainter.cpp importfh.cpp importfhplugin.cpp)
		set(SCRIBUS_IMPORTFH_PLUGIN "importfh")

		add_library(${SCRIBUS_IMPORTFH_PLUGIN} MODULE ${IMPORTFH_PLUGIN_SOURCES})
		target_link_libraries(${SCRIBUS_IMPORTFH_PLUGIN} ${EXE_NAME} ${LIBREVENGE_LDFLAGS} ${LIBREVENGE_GENERATORS_LDFLAGS} ${LIBREVENGE_STREAM_LDFLAGS} ${LIBFREEHAND_LDFLAGS})
		if(WANT_PCH)
			target_precompile_headers(${SCRIBUS_IMPORTFH_PLUGIN} PRIVATE "../../plugins_pch.h")
		endif()
		install(TARGETS ${SCRIBUS_IMPORTFH_PLUGIN} LIBRARY DESTINATION ${PLUGINDIR} PERMISSIONS ${PLUGIN_PERMISSIONS})
		add_dependencies(Plugins ${SCRIBUS_IMPORTFH_PLUGIN})
	endif ()
endif ()
