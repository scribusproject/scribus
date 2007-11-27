INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
)

#Install our hyphenation dict files, selected from *.dic in this directory
FILE( GLOB SCRIBUS_DICT_FILES *.dic )
INSTALL(FILES ${SCRIBUS_DICT_FILES} DESTINATION ${SHAREDIR}dicts)
