include_directories(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
${FREETYPE_INCLUDE_DIRS}
..
)


set(SCRIBUS_TEST_MOC_CLASSES
#testIndex.h
testStoryText.h
)

set(SCRIBUS_TEST_SOURCES
runtests.cpp
#testIndex.cpp
testStoryText.cpp
)

  QT5_WRAP_CPP(SCRIBUS_TEST_MOC_SOURCES ${SCRIBUS_TEST_MOC_CLASSES})

set(SCRIBUS_TESTS_LIB "scribus_tests_lib")
add_library(${SCRIBUS_TESTS_LIB} STATIC ${SCRIBUS_TEST_SOURCES} ${SCRIBUS_TEST_MOC_SOURCES})


# This is a convenience library that for linkage purposes is part of Scribus's
# main API.
set_target_properties(${SCRIBUS_TESTS_LIB}
  PROPERTIES
  COMPILE_FLAGS -DCOMPILE_SCRIBUS_MAIN_APP
  )


# Regular unit tests below.
#
# These tests are built as standalone executables and runned using "make test"
#

set(TESTS_LIBRARIES ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES})

# Unit tests for CellArea
set(CELLAREATESTS_CLASSES cellareatests.h)
set(CELLAREATESTS_SOURCES cellareatests.cpp ../cellarea.cpp)
QT5_WRAP_CPP(CELLAREATESTS_SOURCES ${CELLAREATESTS_CLASSES})
add_executable(cellareatests ${CELLAREATESTS_SOURCES})
target_link_libraries(cellareatests ${TESTS_LIBRARIES})
add_test(NAME cellareatests COMMAND cellareatests)

