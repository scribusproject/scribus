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