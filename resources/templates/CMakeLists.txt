INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
)

INSTALL(FILES
template.xml
  DESTINATION ${TEMPLATEDIR}
)

INSTALL(FILES
br1/br1.png
br1/br1tn.png
br1/map.jpg
br1/br1.sla.gz
  DESTINATION ${TEMPLATEDIR}br1
)

INSTALL(FILES
nl1/kites.jpg
nl1/nl1.png
nl1/nl1tn.png
nl1/nl1.sla.gz
  DESTINATION ${TEMPLATEDIR}nl1
)

INSTALL(FILES
nl2/flowers.jpg
nl2/nl2.png
nl2/nl2tn.png
nl2/nl2.sla.gz
  DESTINATION ${TEMPLATEDIR}nl1
)

INSTALL(FILES
textbased/an_image.jpg
textbased/author.jpg
textbased/textbased.png
textbased/textbased.sla.gz
textbased/textbasedtn.png
  DESTINATION ${TEMPLATEDIR}textbased
)
INSTALL(FILES
sc_presentation/presentation.png
sc_presentation/presentation.sla.gz
sc_presentation/presentationtn.png
  DESTINATION ${TEMPLATEDIR}sc_presentation
)

INSTALL(FILES
cc/Cheat_Cube.png
cc/Cheat_Cubetn.png
cc/Cheat_Cube.sla.gz
  DESTINATION ${TEMPLATEDIR}cc
)

INSTALL(FILES
mc/menu_template1.png
mc/menu_template1tn.png
mc/menu_template1.sla.gz
  DESTINATION ${TEMPLATEDIR}mc
)

INSTALL(FILES
cover1/cover1.png
cover1/cover1tn.png
cover1/cover1.sla.gz
  DESTINATION ${TEMPLATEDIR}cover1
)

INSTALL(FILES
cover2/cover2.png
cover2/cover2tn.png
cover2/cover2.sla.gz
  DESTINATION ${TEMPLATEDIR}cover2
)

INSTALL(FILES
cover3/cover3.png
cover3/cover3tn.png
cover3/cover3.sla.gz
  DESTINATION ${TEMPLATEDIR}cover3
)

INSTALL(FILES
cover4/cover4.png
cover4/cover4tn.png
cover4/cover4.sla.gz
  DESTINATION ${TEMPLATEDIR}cover4
)

INSTALL(FILES
cover5/cover5.png
cover5/cover5tn.png
cover5/cover5.sla.gz
  DESTINATION ${TEMPLATEDIR}cover5
)