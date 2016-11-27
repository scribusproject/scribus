include_directories(
"${CMAKE_SOURCE_DIR}/scribus"
)

install(FILES
menu.xml
after_short_words.png
before_short_words.png
dialog.png
menu.png
short-words-manual.html
  DESTINATION ${DOCDIR}cs/tutorials/scribus-short-words
)
