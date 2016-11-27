file(GLOB QML_FILES *.qml)

install(FILES ${QML_FILES}
    DESTINATION ${QMLDIR}
    )

