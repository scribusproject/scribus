include_directories(
"${CMAKE_SOURCE_DIR}/scribus"
)

#Install our image files
file( GLOB SCRIBUS_DOC_DE_IMAGES_FILES *.jpg *.png )

install(FILES ${SCRIBUS_DOC_DE_IMAGES_FILES} DESTINATION ${DOCDIR}de/images)

