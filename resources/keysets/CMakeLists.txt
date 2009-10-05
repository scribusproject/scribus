INCLUDE_DIRECTORIES(
"${CMAKE_SOURCE_DIR}/scribus"
)

INSTALL(FILES
icalamus.xml
pageplusx3.xml
pagestream.xml
scribus13.xml
  DESTINATION ${SHAREDIR}keysets
)
