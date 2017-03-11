INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/scribus
)

##################

#Install the program icon if not on Mac and not on Windows
if(NOT WIN32 AND NOT APPLE)
	SET(SYSICONDIR "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor")
	FOREACH(res 16x16 32x32 128x128 256x256 512x512 1024x1024)
		INSTALL(FILES
			icon_${res}.png
			RENAME scribus${TAG_VERSION}.png
			DESTINATION ${SYSICONDIR}/${res}/apps/
		)
	ENDFOREACH(res)
endif()
