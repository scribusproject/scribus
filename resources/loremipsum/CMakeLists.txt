include_directories(
"${CMAKE_SOURCE_DIR}/scribus"
)

#Install our lorem ipsum sample files, selected from *.xml in this directory
file( GLOB SCRIBUS_LOREM_FILES *.xml )
install(FILES ${SCRIBUS_LOREM_FILES} DESTINATION ${SHAREDIR}loremipsum)
