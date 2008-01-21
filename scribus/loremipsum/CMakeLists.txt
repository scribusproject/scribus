INCLUDE_DIRECTORIES(
"${CMAKE_SOURCE_DIR}/scribus"
)

INSTALL(FILES
arabic.xml
czech.xml
dutch.xml
finnish.xml
english.xml
eo.xml
fr.xml
german.xml
greek.xml
hebrew.xml
italian.xml
loremipsum.xml
russian.xml
slovenian.xml
  DESTINATION ${SHAREDIR}loremipsum
)
