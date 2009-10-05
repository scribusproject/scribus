INCLUDE_DIRECTORIES(
"${CMAKE_SOURCE_DIR}/scribus"
)

#Install our lorem ipsum sample files, selected from *.xml in this directory
FILE( GLOB SCRIBUS_LOREM_FILES *.xml )
INSTALL(FILES ${SCRIBUS_LOREM_FILES} DESTINATION ${SHAREDIR}loremipsum)
