include_directories(
"${CMAKE_SOURCE_DIR}/scribus"
)

#Install our qm files, selected from *.qm in this directory
file( GLOB SCRIBUS_TS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.ts )
list(REMOVE_ITEM SCRIBUS_TS_FILES "${CMAKE_CURRENT_SOURCE_DIR}/scribus.untranslated.ts")
#message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})
#message(STATUS ${SCRIBUS_TS_FILES})

message(STATUS "The following GUI languages will be installed: ${WANT_GUI_LANG}")
set(QM_FILES)
set(TS_LANGS)

#Bit of a hack, preprocess all the filenames to generate our language string, needed for -DWANT_GUI_LANG=en_GB;de_DE , etc
foreach(TS_FILEPATH ${SCRIBUS_TS_FILES})
	get_filename_component(TS_FILENAME ${TS_FILEPATH} NAME)
	string(REGEX MATCH "\\.([0-9a-zA-Z_]*)\\." LANGSTR ${TS_FILENAME})
	string(REPLACE "." ""  LANGSTR ${LANGSTR})
	#append ts_filename to get the file name, append ts_filepath to get the full path
	if(WANT_GUI_LANG)
		foreach(WANTED_LANG ${WANT_GUI_LANG})
			if(LANGSTR STREQUAL WANTED_LANG)
#				message(STATUS "new language: ${TS_FILENAME}")
				set(TS_LANGS ${TS_LANGS} ${TS_FILEPATH})
			endif()
		endforeach(WANTED_LANG)
	else()
		set(TS_LANGS ${TS_LANGS} ${TS_FILEPATH})
	endif()
endforeach(TS_FILEPATH)
#message(STATUS "new language list: ${TS_LANGS}")
set_source_files_properties(${TS_LANGS}
	PROPERTIES OUTPUT_LOCATION "${SHAREDIR}/translations")

#qt_add_translations(${EXE_NAME} TS_FILES ${TS_LANGS} QM_FILES_OUTPUT_VARIABLE QM_FILES)
qt_add_lrelease(${EXE_NAME}
	TS_FILES ${TS_LANGS}
	QM_FILES_OUTPUT_VARIABLE QM_FILES
	OPTIONS -silent)

install(FILES ${QM_FILES} DESTINATION "${SHAREDIR}/translations")


#message(STATUS "The following GUI languages will be installed: ${WANT_GUI_LANG}")

#set(QM_FILES)

##If the user specified WANT_GUI_LANG, then we only create build/install targets for those languages
#if(WANT_GUI_LANG)
#  foreach(TS_FILEPATH ${SCRIBUS_TS_FILES})
#	get_filename_component(TS_FILENAME ${TS_FILEPATH} NAME)
#	string(REGEX MATCH "\\.([0-9a-zA-Z_]*)\\." LANGSTR ${TS_FILENAME})
#	string(REPLACE "." ""  LANGSTR ${LANGSTR})
#	foreach(WANTED_LANG ${WANT_GUI_LANG})
#	  if(LANGSTR STREQUAL WANTED_LANG)
#		#message(STATUS "You want to install ${LANGSTR}:${WANTED_LANG}")
#		set(QM_FILENAME "scribus.${LANGSTR}.qm")
#		add_custom_command(
#      OUTPUT ${QM_FILENAME}
#			COMMAND ${LRELEASE_EXECUTABLE} ${TS_FILEPATH} -silent -qm ${QM_FILENAME}
#      MAIN_DEPENDENCY ${TS_FILEPATH}
#      COMMENT "release_ts_${LANGSTR}"
#		)
#    list(APPEND QM_FILES "${QM_FILENAME}")
#	  endif(LANGSTR STREQUAL WANTED_LANG)
#	endforeach(WANTED_LANG)
#  endforeach(TS_FILEPATH)
#else(WANT_GUI_LANG)
##Just install all of them
#  foreach(TS_FILEPATH ${SCRIBUS_TS_FILES})
#	get_filename_component(TS_FILENAME ${TS_FILEPATH} NAME)
#	string(REGEX MATCH "\\.([0-9a-zA-Z_]*)\\." LANGSTR ${TS_FILENAME})
#	string(REPLACE "." ""  LANGSTR ${LANGSTR})
#	set(QM_FILENAME "scribus.${LANGSTR}.qm")
#  add_custom_command(
#    OUTPUT ${QM_FILENAME}
#    COMMAND ${LRELEASE_EXECUTABLE} ${TS_FILEPATH} -silent -qm ${QM_FILENAME}
#    MAIN_DEPENDENCY ${TS_FILEPATH}
#    COMMENT "release_ts_${LANGSTR}"
#  )
#  list(APPEND QM_FILES "${QM_FILENAME}")
#  endforeach(TS_FILEPATH)
#endif(WANT_GUI_LANG)

#add_custom_target(Translations ALL
#  DEPENDS ${QM_FILES}
#  )
#foreach(QM_FILE IN LISTS QM_FILES)
#		install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${QM_FILE}" DESTINATION ${SHAREDIR}/translations)
#endforeach(QM_FILE)

#unset(QM_FILES)
