ADD_SUBDIRECTORY(tutorials/short-words)

#Install our html files
FILE( GLOB SCRIBUS_DOC_FR_FILES *.html *.xml )
INSTALL(FILES ${SCRIBUS_DOC_FR_FILES} DESTINATION ${DOCDIR}fr)
