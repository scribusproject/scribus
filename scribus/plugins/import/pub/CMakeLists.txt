include(FindPkgConfig)
if (HAVE_REVENGE)
	pkg_check_modules(LIBMSPUB libmspub-0.1>=0.1)
	if (LIBMSPUB_FOUND)
		message("Building MS Publisher Importer with librevenge")
		include_directories(
			${CMAKE_SOURCE_DIR}
			${CMAKE_SOURCE_DIR}/scribus
			${LIBREVENGE_INCLUDE_DIRS}
			${LIBREVENGE_GENERATORS_INCLUDE_DIRS}
			${LIBREVENGE_STREAM_INCLUDE_DIRS}
			${LIBMSPUB_INCLUDE_DIRS}
			${SCRIBUS_AUTOGEN_INCLUDE_PATH}
			)
		set(IMPORTPUB_PLUGIN_SOURCES ../revenge/rawpainter.cpp importpub.cpp importpubplugin.cpp)
		set(SCRIBUS_IMPORTPUB_PLUGIN "importpub")

		add_library(${SCRIBUS_IMPORTPUB_PLUGIN} MODULE ${IMPORTPUB_PLUGIN_SOURCES})
		target_link_libraries(${SCRIBUS_IMPORTPUB_PLUGIN} ${EXE_NAME} ${LIBREVENGE_LDFLAGS} ${LIBREVENGE_GENERATORS_LDFLAGS} ${LIBREVENGE_STREAM_LDFLAGS} ${LIBMSPUB_LDFLAGS})
		if(WANT_PCH)
			target_precompile_headers(${SCRIBUS_IMPORTPUB_PLUGIN} PRIVATE "../../plugins_pch.h")
		endif()
		install(TARGETS ${SCRIBUS_IMPORTPUB_PLUGIN} LIBRARY DESTINATION ${PLUGINDIR} PERMISSIONS ${PLUGIN_PERMISSIONS})
		add_dependencies(Plugins ${SCRIBUS_IMPORTPUB_PLUGIN})
	endif ()
else ()
	pkg_check_modules(LIBMSPUB libmspub-0.0<=0.1)
	if (LIBMSPUB_FOUND)
		pkg_check_modules(LIBWPG libwpg-0.2)
		if (LIBWPG_FOUND)
			pkg_check_modules(LIBWPD libwpd-0.9)
			if (LIBWPD_FOUND)
				pkg_check_modules(LIBWPD_STREAM libwpd-stream-0.9)
				if (LIBWPD_STREAM_FOUND)
					message("Building MS Publisher Importer without librevenge")
					include_directories(
						${CMAKE_SOURCE_DIR}
						${CMAKE_SOURCE_DIR}/scribus
						${LIBWPG_INCLUDE_DIRS}
						${LIBWPD_INCLUDE_DIRS}
						${LIBWPD_STREAM_INCLUDE_DIRS}
						${LIBMSPUB_INCLUDE_DIRS}
						${SCRIBUS_AUTOGEN_INCLUDE_PATH}
						)
					set(IMPORTPUB_PLUGIN_SOURCES ../revenge/rawpainter.cpp importpub.cpp importpubplugin.cpp)
					set(SCRIBUS_IMPORTPUB_PLUGIN "importpub")

					add_library(${SCRIBUS_IMPORTPUB_PLUGIN} MODULE ${IMPORTPUB_PLUGIN_SOURCES})
					target_link_libraries(${SCRIBUS_IMPORTPUB_PLUGIN} ${EXE_NAME} ${LIBWPG_LDFLAGS} ${LIBWPD_LDFLAGS} ${LIBWPD_STREAM_LDFLAGS} ${LIBMSPUB_LDFLAGS})
					if(WANT_PCH)
						target_precompile_headers(${SCRIBUS_IMPORTPUB_PLUGIN} PRIVATE "../../plugins_pch.h")
					endif()
					install(TARGETS ${SCRIBUS_IMPORTPUB_PLUGIN} LIBRARY DESTINATION ${PLUGINDIR} PERMISSIONS ${PLUGIN_PERMISSIONS})
					add_dependencies(Plugins ${SCRIBUS_IMPORTPUB_PLUGIN})
				endif ()
			endif ()
		endif ()
	endif ()
endif ()
