include_directories(
"${CMAKE_SOURCE_DIR}/scribus"
)

install(FILES
scribus.1
  DESTINATION ${CMAKE_INSTALL_MANDIR}/man1
  RENAME scribus${TAG_VERSION}.1
)

install(FILES
scribus.1.de
  DESTINATION ${CMAKE_INSTALL_MANDIR}/de/man1
  RENAME scribus${TAG_VERSION}.1
)

install(FILES
scribus.1.pl
  DESTINATION ${CMAKE_INSTALL_MANDIR}/pl/man1
  RENAME scribus${TAG_VERSION}.1
)
