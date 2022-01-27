include(FindPkgConfig)
if (HAVE_REVENGE)
	pkg_check_modules(LIBCDR libcdr-0.1>=0.1)
	if (LIBCDR_FOUND)
		message("Building Corel Draw Importer with librevenge")
		include_directories(
			${CMAKE_SOURCE_DIR}
			${CMAKE_SOURCE_DIR}/scribus
			${LIBREVENGE_INCLUDE_DIRS}
			${LIBREVENGE_GENERATORS_INCLUDE_DIRS}
			${LIBREVENGE_STREAM_INCLUDE_DIRS}
			${LIBCDR_INCLUDE_DIRS}
			${SCRIBUS_AUTOGEN_INCLUDE_PATH}
			)
		set(IMPORTCDR_PLUGIN_SOURCES ../revenge/rawpainter.cpp importcdr.cpp importcdrplugin.cpp)
		set(SCRIBUS_IMPORTCDR_PLUGIN "importcdr")

		add_library(${SCRIBUS_IMPORTCDR_PLUGIN} MODULE ${IMPORTCDR_PLUGIN_SOURCES})
		target_link_libraries(${SCRIBUS_IMPORTCDR_PLUGIN} ${EXE_NAME} ${LIBREVENGE_LDFLAGS} ${LIBREVENGE_GENERATORS_LDFLAGS} ${LIBREVENGE_STREAM_LDFLAGS} ${LIBCDR_LDFLAGS})
		if(WANT_PCH)
			target_precompile_headers(${SCRIBUS_IMPORTCDR_PLUGIN} PRIVATE "../../plugins_pch.h")
		endif()
		install(TARGETS ${SCRIBUS_IMPORTCDR_PLUGIN} LIBRARY DESTINATION ${PLUGINDIR} PERMISSIONS ${PLUGIN_PERMISSIONS})
		add_dependencies(Plugins ${SCRIBUS_IMPORTCDR_PLUGIN})
	endif ()
else ()
	pkg_check_modules(LIBWPG libwpg-0.2)
	if (LIBWPG_FOUND)
		pkg_check_modules(LIBWPD libwpd-0.9)
		if (LIBWPD_FOUND)
			pkg_check_modules(LIBWPD_STREAM libwpd-stream-0.9)
			if (LIBWPD_STREAM_FOUND)
				pkg_check_modules(LIBCDR libcdr-0.0<=0.1)
				if (LIBCDR_FOUND)
					include_directories(
						${CMAKE_SOURCE_DIR}
						${CMAKE_SOURCE_DIR}/scribus
						${LIBWPG_INCLUDE_DIRS}
						${LIBWPD_INCLUDE_DIRS}
						${LIBWPD_STREAM_INCLUDE_DIRS}
						${LIBCDR_INCLUDE_DIRS}
						${SCRIBUS_AUTOGEN_INCLUDE_PATH}
						)
					set(IMPORTCDR_PLUGIN_SOURCES importcdr.cpp importcdrplugin.cpp)
					set(SCRIBUS_IMPORTCDR_PLUGIN "importcdr")

					add_library(${SCRIBUS_IMPORTCDR_PLUGIN} MODULE ${IMPORTCDR_PLUGIN_SOURCES})
					target_link_libraries(${SCRIBUS_IMPORTCDR_PLUGIN} ${EXE_NAME} ${LIBWPG_LDFLAGS} ${LIBWPD_LDFLAGS} ${LIBWPD_STREAM_LDFLAGS} ${LIBCDR_LDFLAGS})
					if(WANT_PCH)
						target_precompile_headers(${SCRIBUS_IMPORTCDR_PLUGIN} PRIVATE "../../plugins_pch.h")
					endif()
					install(TARGETS ${SCRIBUS_IMPORTCDR_PLUGIN} LIBRARY DESTINATION ${PLUGINDIR} PERMISSIONS ${PLUGIN_PERMISSIONS})
					add_dependencies(Plugins ${SCRIBUS_IMPORTCDR_PLUGIN})
				endif ()
			endif ()
		endif ()
	endif ()
endif ()
