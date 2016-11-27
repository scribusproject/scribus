add_subdirectory(images)

#Install our html files
file( GLOB SCRIBUS_DOC_DE_FILES *.html *.xml )
install(FILES ${SCRIBUS_DOC_DE_FILES} DESTINATION ${DOCDIR}de)
