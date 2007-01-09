INCLUDE_DIRECTORIES(
"${CMAKE_SOURCE_DIR}/scribus"
)

INSTALL(FILES
menu.xml
after_short_words.png
before_short_words.png
dialog.png
menu.png
short-words-manual.html
  DESTINATION ${DOCDIR}pl/tutorials/scribus-short-words
)
