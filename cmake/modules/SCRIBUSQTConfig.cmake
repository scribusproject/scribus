#Based on QT4_QRAP_UI/CPP from cmake 2.3.4
  MACRO(SCRIBUS_QT_WRAP_UI outfiles )

    FOREACH(it ${ARGN})
      GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)
      SET(infile ${CMAKE_CURRENT_SOURCE_DIR}/${it})
      SET(outfile ${CMAKE_CURRENT_BINARY_DIR}/ui_${outfile}.h)
      ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
        COMMAND ${QT_UIC_EXECUTABLE}
        ARGS -o ${outfile} ${infile}
        MAIN_DEPENDENCY ${infile})
      SET(${outfiles} ${${outfiles}} ${outfile})
    ENDFOREACH(it)

  ENDMACRO(SCRIBUS_QT_WRAP_UI)

  MACRO(SCRIBUS_QT_WRAP_CPP outfiles )

    # get include dirs
    GET_DIRECTORY_PROPERTY(moc_includes_tmp INCLUDE_DIRECTORIES)
    SET(moc_includes)
    #FOREACH(it ${moc_includes_tmp})
    #  SET(moc_includes ${moc_includes} "-I${it}")
    #ENDFOREACH(it)

    FOREACH(it ${ARGN})
      GET_FILENAME_COMPONENT(outfilename ${it} NAME_WE)
	
      GET_FILENAME_COMPONENT(infile ${it} ABSOLUTE)
#       SET(infile ${CMAKE_CURRENT_SOURCE_DIR}/${it})
      SET(outfile ${CMAKE_CURRENT_BINARY_DIR}/${outfilename}.moc)
      ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
        COMMAND ${QT_MOC_EXECUTABLE}
#         ARGS ${moc_includes} -o ${outfile} ${infile}
          ARGS -o ${outfile} ${infile}
#         MAIN_DEPENDENCY ${infile})
        DEPENDS ${infile})
      SET(${outfiles} ${${outfiles}} ${outfile})
    ENDFOREACH(it)

  ENDMACRO(SCRIBUS_QT_WRAP_CPP)
