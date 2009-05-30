INCLUDE_DIRECTORIES(
"${CMAKE_SOURCE_DIR}/scribus"
)

INSTALL(FILES
icalamus.xml
pagestream.xml
scribus13.xml
  DESTINATION ${SHAREDIR}keysets
)
