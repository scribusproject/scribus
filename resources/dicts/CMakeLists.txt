include_directories(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
)

#Install our hyphenation dict files, selected from *.dic in this directory
file( GLOB SCRIBUS_DICT_FILES *.dic *.txt )
list(REMOVE_ITEM SCRIBUS_DICT_FILES "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")
install(FILES ${SCRIBUS_DICT_FILES} DESTINATION ${SHAREDIR}dicts/hyph)
