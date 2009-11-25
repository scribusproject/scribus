SET(LIB_TYPE STATIC)
SET(WPG_SOURCES
    WPG1Parser.cpp
    WPG2Parser.cpp
    WPGBinaryData.cpp
    WPGBitmap.cpp
    WPGBrush.cpp
    WPGColor.cpp
    WPGGradient.cpp
    WPGHeader.cpp
    WPGInternalStream.cpp
    WPGOLEStream.cpp
    WPGPath.cpp
    WPGPen.cpp
    WPGPoint.cpp
    WPGraphics.cpp
    WPGRect.cpp
    WPGStreamImplementation.cpp
    WPGString.cpp
    WPGXParser.cpp
  )
ADD_LIBRARY(wpg ${LIB_TYPE} ${WPG_SOURCES})
TARGET_LINK_LIBRARIES(wpg)
