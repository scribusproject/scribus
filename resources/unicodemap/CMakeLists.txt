include_directories(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
)

install(FILES
unicodenameslist.txt
  DESTINATION ${SHAREDIR}
)
