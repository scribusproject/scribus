include_directories(
"${CMAKE_SOURCE_DIR}/scribus"
)

add_subdirectory(images)
add_subdirectory(tutorials/short-words)

#Install our html files
file( GLOB SCRIBUS_DOC_EN_FILES *.html *.xml *.css *.xsl )
install(FILES ${SCRIBUS_DOC_EN_FILES} DESTINATION ${DOCDIR}en)
