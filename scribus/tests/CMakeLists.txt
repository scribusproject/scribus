INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
${FREETYPE_INCLUDE_DIRS}
..
)


SET(SCRIBUS_TEST_MOC_CLASSES
#testIndex.h
testStoryText.h
)

SET(SCRIBUS_TEST_SOURCES
runtests.cpp
#testIndex.cpp
testStoryText.cpp
)

# IF(QT4_WRAP_CPP)
  QT4_WRAP_CPP(SCRIBUS_TEST_MOC_SOURCES ${SCRIBUS_TEST_MOC_CLASSES})
# ENDIF(QT4_WRAP_CPP)

SET(SCRIBUS_TESTS_LIB "scribus_tests_lib")
ADD_LIBRARY(${SCRIBUS_TESTS_LIB} STATIC ${SCRIBUS_TEST_SOURCES} ${SCRIBUS_TEST_MOC_SOURCES})


# This is a convenience library that for linkage purposes is part of Scribus's
# main API.
SET_TARGET_PROPERTIES(${SCRIBUS_TESTS_LIB}
  PROPERTIES
  COMPILE_FLAGS -DCOMPILE_SCRIBUS_MAIN_APP
  )


# Regular unit tests below.
#
# These tests are built as standalone executables and runned using "make test"
#

SET(TESTS_LIBRARIES ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES})

# Unit tests for CellArea
SET(CELLAREATESTS_CLASSES cellareatests.h)
SET(CELLAREATESTS_SOURCES cellareatests.cpp ../cellarea.cpp)
QT4_WRAP_CPP(CELLAREATESTS_SOURCES ${CELLAREATESTS_CLASSES})
ADD_EXECUTABLE(cellareatests ${CELLAREATESTS_SOURCES})
TARGET_LINK_LIBRARIES(cellareatests ${TESTS_LIBRARIES})
ADD_TEST(NAME cellareatests COMMAND cellareatests)

