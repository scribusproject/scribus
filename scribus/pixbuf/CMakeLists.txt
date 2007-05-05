INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
)

SET(SCRIBUS_PIXBUF_LIB_SOURCES
gdk-pixbuf-xlib.c
gdk-pixbuf-xlib-drawable.c
gdk-pixbuf-xlib-render.c
gdk-pixbuf-xlibrgb.c
)

SET(SCRIBUS_PIXBUF_LIB "scribus_pixbuf_lib")
ADD_LIBRARY(${SCRIBUS_PIXBUF_LIB} STATIC ${SCRIBUS_PIXBUF_LIB_SOURCES})