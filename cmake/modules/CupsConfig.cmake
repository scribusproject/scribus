# - Try to find Cups
# Once done this will define
#
#  CUPS_FOUND - system has Cups
#  CUPS_INCLUDE_DIR - the Cups include directory
#  CUPS_LIBRARY - Link these to use CUPS
#

FIND_PATH(CUPS_INCLUDE_DIR cups/cups.h
  /usr/include
  /usr/local/include
)

FIND_LIBRARY(CUPS_LIBRARY NAMES cups PATHS /usr/lib /usr/local/lib)

IF(CUPS_INCLUDE_DIR AND CUPS_LIBRARY)
   SET(CUPS_FOUND TRUE)
ENDIF(CUPS_INCLUDE_DIR AND CUPS_LIBRARY)

SET(Cups_FIND_QUIETLY 1)

IF(CUPS_FOUND)
  SET( CUPS_LIBRARIES ${CUPS_LIBRARY} )
  IF(NOT Cups_FIND_QUIETLY)
    MESSAGE(STATUS "Found Cups: ${CUPS_LIBRARY}")
  ENDIF(NOT Cups_FIND_QUIETLY)
ELSE(CUPS_FOUND)
  IF(Cups_FIND_REQUIRED)
    MESSAGE(SEND_ERROR "Could not find Cups")
  ENDIF(Cups_FIND_REQUIRED)
ENDIF(CUPS_FOUND)