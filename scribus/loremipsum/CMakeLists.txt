INCLUDE_DIRECTORIES(
"${CMAKE_SOURCE_DIR}/scribus"
)

INSTALL(FILES
ar.xml
cs.xml
de.xml
el.xml
fi.xml
he.xml
it.xml
la.xml
ru.xml
  DESTINATION ${SHAREDIR}loremipsum
)
