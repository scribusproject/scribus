include_directories(
"${CMAKE_SOURCE_DIR}/scribus"
)

add_subdirectory(images)

#Install our html files
file( GLOB SCRIBUS_DOC_IT_FILES *.html *.xml )
install(FILES ${SCRIBUS_DOC_IT_FILES} DESTINATION ${DOCDIR}it)
