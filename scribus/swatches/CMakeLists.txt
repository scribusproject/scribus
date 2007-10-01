INCLUDE_DIRECTORIES(
"${CMAKE_SOURCE_DIR}/scribus"
)

INSTALL(FILES
rgbscribusgnome.txt
rgbscribusgreys.txt
rgbscribusopenoffice.txt
rgbscribus.txt
rgbsvg.txt
Scribus_Basic.txt
Scribus_Gnome.txt
Scribus_OpenOffice.txt
Scribus_SVG.txt
Scribus_X11Grey.txt
Scribus_X11.txt
  DESTINATION ${LIBDIR}swatches
)