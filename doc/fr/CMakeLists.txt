add_subdirectory(tutorials/short-words)

#Install our html files
file( GLOB SCRIBUS_DOC_FR_FILES *.html *.xml )
install(FILES ${SCRIBUS_DOC_FR_FILES} DESTINATION ${DOCDIR}fr)
