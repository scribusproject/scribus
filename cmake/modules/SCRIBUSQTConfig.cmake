#Based on QT4_QRAP_UI/CPP from cmake 2.3.4
  macro(SCRIBUS_QT_WRAP_UI outfiles )

    foreach(it ${ARGN})
      get_filename_component(outfile ${it} NAME_WE)
      set(infile ${CMAKE_CURRENT_SOURCE_DIR}/${it})
      set(outfile ${CMAKE_CURRENT_BINARY_DIR}/ui_${outfile}.h)
      add_custom_command(OUTPUT ${outfile}
        COMMAND ${QT_UIC_EXECUTABLE}
        ARGS -o ${outfile} ${infile}
        MAIN_DEPENDENCY ${infile})
      set(${outfiles} ${${outfiles}} ${outfile})
    endforeach(it)

  endmacro(SCRIBUS_QT_WRAP_UI)

  macro(SCRIBUS_QT_WRAP_CPP outfiles )

    # get include dirs
    get_directory_property(moc_includes_tmp INCLUDE_DIRECTORIES)
    set(moc_includes)
    #foreach(it ${moc_includes_tmp})
    #  set(moc_includes ${moc_includes} "-I${it}")
    #endforeach(it)

    foreach(it ${ARGN})
      get_filename_component(outfilename ${it} NAME_WE)
	
      get_filename_component(infile ${it} ABSOLUTE)
#       set(infile ${CMAKE_CURRENT_SOURCE_DIR}/${it})
      set(outfile ${CMAKE_CURRENT_BINARY_DIR}/${outfilename}.moc)
      add_custom_command(OUTPUT ${outfile}
        COMMAND ${QT_MOC_EXECUTABLE}
#         ARGS ${moc_includes} -o ${outfile} ${infile}
          ARGS -o ${outfile} ${infile}
#         MAIN_DEPENDENCY ${infile})
        DEPENDS ${infile})
      set(${outfiles} ${${outfiles}} ${outfile})
    endforeach(it)

  endmacro(SCRIBUS_QT_WRAP_CPP)
