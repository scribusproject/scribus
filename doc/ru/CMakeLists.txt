include_directories(
"${CMAKE_SOURCE_DIR}/scribus"
)

# For now use fallback on English doc images
#add_subdirectory(images)

#Install our html files
file( GLOB SCRIBUS_DOC_RU_FILES *.html *.xml *.css *.xsl )
install(FILES ${SCRIBUS_DOC_RU_FILES} DESTINATION ${DOCDIR}ru)
