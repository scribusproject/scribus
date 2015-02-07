INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
)

#Install our hyphenation dict files, selected from *.dic in this directory
FILE( GLOB SCRIBUS_DICT_FILES *.dic *.txt )
LIST(REMOVE_ITEM SCRIBUS_DICT_FILES "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")
INSTALL(FILES ${SCRIBUS_DICT_FILES} DESTINATION ${SHAREDIR}dicts/hyph)
