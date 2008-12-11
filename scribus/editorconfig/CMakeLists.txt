INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
)

INSTALL(FILES
100_latex.tar
100_latex.xml
#sample.xml
200_gnuplot.xml
gnuplot.png
300_lilypond.xml
lilypond.png
400_dot.xml
dot.svg
500_povray.xml
povray_32.png

  DESTINATION ${SHAREDIR}/editorconfig
)
