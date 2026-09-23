# For general Scribus (>=1.3.2) copyright and licensing information please refer
# to the COPYING file provided with the program. Following this notice may exist
# a copyright and/or license notice that predates the release of Scribus 1.3.2
# for which a new license (GPL+exception) is in place.

cmake_minimum_required(VERSION 3.21)
project(ScribusPortableTests LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()
find_package(Qt6 6.4 REQUIRED COMPONENTS Core Gui Test Widgets)
find_package(TIFF QUIET)

get_filename_component(SCRIBUS_ROOT "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)

function(add_scribus_portable_test target)
	add_executable(${target} ${ARGN})
	target_include_directories(${target} PRIVATE "${SCRIBUS_ROOT}/scribus")
	target_compile_definitions(${target} PRIVATE COMPILE_SCRIBUS_MAIN_APP)
	add_test(NAME ${target} COMMAND ${target})
endfunction()

add_scribus_portable_test(actionsearchtests
	"${SCRIBUS_ROOT}/scribus/tests/actionsearchtests.cpp"
	"${SCRIBUS_ROOT}/scribus/actionsearch.cpp")
target_link_libraries(actionsearchtests PRIVATE Qt6::Core Qt6::Gui Qt6::Test Qt6::Widgets)
set_tests_properties(actionsearchtests PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")

add_scribus_portable_test(anchorpositiontests
	"${SCRIBUS_ROOT}/scribus/tests/anchorpositiontests.cpp"
	"${SCRIBUS_ROOT}/scribus/anchorposition.cpp")
target_link_libraries(anchorpositiontests PRIVATE Qt6::Core Qt6::Gui Qt6::Test)

add_scribus_portable_test(cellareatests
	"${SCRIBUS_ROOT}/scribus/tests/cellareatests.cpp"
	"${SCRIBUS_ROOT}/scribus/cellarea.cpp")
target_link_libraries(cellareatests PRIVATE Qt6::Core Qt6::Test)

add_scribus_portable_test(stylequickapplytests
	"${SCRIBUS_ROOT}/scribus/tests/stylequickapplytests.cpp"
	"${SCRIBUS_ROOT}/scribus/stylequickapplymodel.cpp")
target_link_libraries(stylequickapplytests PRIVATE Qt6::Core Qt6::Test)

add_scribus_portable_test(objectstyletests
	"${SCRIBUS_ROOT}/scribus/tests/objectstyletests.cpp"
	"${SCRIBUS_ROOT}/scribus/styles/objectstyle.cpp"
	"${SCRIBUS_ROOT}/scribus/styles/style.cpp"
	"${SCRIBUS_ROOT}/scribus/styles/stylecontext.cpp"
	"${SCRIBUS_ROOT}/scribus/styles/stylecontextproxy.cpp"
	"${SCRIBUS_ROOT}/scribus/updatemanager.cpp"
	"${SCRIBUS_ROOT}/scribus/observable.h")
target_link_libraries(objectstyletests PRIVATE Qt6::Core Qt6::Gui Qt6::Test)

add_scribus_portable_test(imagelinkmatchertests
	"${SCRIBUS_ROOT}/scribus/tests/imagelinkmatchertests.cpp"
	"${SCRIBUS_ROOT}/scribus/imagelinkmatcher.cpp"
	"${SCRIBUS_ROOT}/scribus/deferredtask.cpp")
target_link_libraries(imagelinkmatchertests PRIVATE Qt6::Core Qt6::Test)

add_scribus_portable_test(embeddedimageextractortests
	"${SCRIBUS_ROOT}/scribus/tests/embeddedimageextractortests.cpp"
	"${SCRIBUS_ROOT}/scribus/embeddedimageextractor.cpp")
target_link_libraries(embeddedimageextractortests PRIVATE Qt6::Core Qt6::Test)

if(TIFF_FOUND)
	add_scribus_portable_test(imagecmykexporttests
		"${SCRIBUS_ROOT}/scribus/tests/imagecmykexporttests.cpp"
		"${SCRIBUS_ROOT}/scribus/imagecmykexport.cpp")
	target_link_libraries(imagecmykexporttests PRIVATE Qt6::Core Qt6::Gui Qt6::Test TIFF::TIFF)
else()
	message(STATUS "Skipping portable TIFF writer test; the native Windows package job covers TIFF linkage")
endif()

add_scribus_portable_test(imagealphacontourtests
	"${SCRIBUS_ROOT}/scribus/tests/imagealphacontourtests.cpp"
	"${SCRIBUS_ROOT}/scribus/imagealphasilhouette.cpp")
target_link_libraries(imagealphacontourtests PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Test)
set_tests_properties(imagealphacontourtests PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")

add_scribus_portable_test(collectmanifesttests
	"${SCRIBUS_ROOT}/scribus/tests/collectmanifesttests.cpp"
	"${SCRIBUS_ROOT}/scribus/collectmanifest.cpp")
target_link_libraries(collectmanifesttests PRIVATE Qt6::Core Qt6::Test)

add_test(NAME windows_project_sources
	COMMAND "${CMAKE_COMMAND}"
		-DSCRIBUS_ROOT=${SCRIBUS_ROOT}
		-P "${CMAKE_CURRENT_LIST_DIR}/check_windows_projects.cmake")

add_test(NAME windows_packaging_contract
	COMMAND "${CMAKE_COMMAND}"
		-DSCRIBUS_ROOT=${SCRIBUS_ROOT}
		-P "${CMAKE_CURRENT_LIST_DIR}/check_windows_packaging.cmake")
