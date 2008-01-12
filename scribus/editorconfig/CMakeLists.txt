INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
)

INSTALL(FILES
latex.tar
latex.xml
sample.xml
gnuplot.xml
  DESTINATION ${SHAREDIR}/editorconfig
)
