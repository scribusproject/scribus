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
nl.xml
ru.xml
sl.xml
  DESTINATION ${SHAREDIR}loremipsum
)
