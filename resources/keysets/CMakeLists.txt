include_directories(
"${CMAKE_SOURCE_DIR}/scribus"
)

install(FILES
icalamus.xml
pageplusx3.xml
pagestream.xml
scribus15.xml
  DESTINATION ${SHAREDIR}keysets
)
