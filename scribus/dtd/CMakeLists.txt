include_directories(
"${CMAKE_SOURCE_DIR}/scribus"
)

install(FILES
scribuspdfoptions.dtd
  DESTINATION ${SHAREDIR}dtd
)
