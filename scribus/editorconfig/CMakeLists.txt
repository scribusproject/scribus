INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
)

INSTALL(FILES
latex.tar
latex.xml
# sample.xml
  DESTINATION ${SHAREDIR}/editorconfig
)
