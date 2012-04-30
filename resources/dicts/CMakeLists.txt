INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
)

#Install our hyphenation dict files, selected from *.dic in this directory
FILE( GLOB SCRIBUS_HYPH_DICT_FILES *.dic README*.txt )
INSTALL(FILES ${SCRIBUS_HYPH_DICT_FILES} DESTINATION ${SHAREDIR}dicts)

#Install our spelling dict files, selected from * in this directory
FILE( GLOB SCRIBUS_SPELL_DICT_FILES spelling/*.dic spelling/*.txt spelling/*.aff )
INSTALL(FILES ${SCRIBUS_SPELL_DICT_FILES} DESTINATION ${SHAREDIR}dicts/spelling)
