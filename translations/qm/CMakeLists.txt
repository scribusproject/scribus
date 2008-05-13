INCLUDE_DIRECTORIES(
"${CMAKE_SOURCE_DIR}/scribus"
)

#Install our qm files, selected from *.qm in this directory
FILE( GLOB SCRIBUS_QM_FILES *.qm )
INSTALL(FILES ${SCRIBUS_QM_FILES} DESTINATION ${SHAREDIR}/translations)