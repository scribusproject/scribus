include_directories(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
${FREETYPE_INCLUDE_DIRS}
${SCRIBUS_AUTOGEN_INCLUDE_PATH}
..
)

set(SCRIBUS_TEST_SOURCES
runtests.cpp
#testIndex.cpp
testStoryText.cpp
)

set(SCRIBUS_TESTS_LIB "scribus_tests_lib")
add_library(${SCRIBUS_TESTS_LIB} STATIC ${SCRIBUS_TEST_SOURCES})
target_link_libraries(${SCRIBUS_TESTS_LIB} PRIVATE Qt6::Test)


# This is a convenience library that for linkage purposes is part of Scribus's
# main API.
set_target_properties(${SCRIBUS_TESTS_LIB}
  PROPERTIES
  COMPILE_FLAGS -DCOMPILE_SCRIBUS_MAIN_APP
  )


# Regular unit tests below.
#
# These tests are built as standalone executables and are run using "make test"
#

set(TESTS_LIBRARIES Qt6::Test Qt6::Core)

# Unit tests for anchored-object positioning and wrap geometry
set(ANCHORPOSITIONTESTS_SOURCES anchorpositiontests.cpp ../anchorposition.cpp)
add_executable(anchorpositiontests ${ANCHORPOSITIONTESTS_SOURCES})
target_link_libraries(anchorpositiontests ${TESTS_LIBRARIES} Qt6::Gui)
target_compile_definitions(anchorpositiontests PRIVATE COMPILE_SCRIBUS_MAIN_APP)
add_test(NAME anchorpositiontests COMMAND anchorpositiontests)
set_tests_properties(anchorpositiontests PROPERTIES LABELS "anchored-objects;phase-3")

# Unit tests for CellArea
set(CELLAREATESTS_SOURCES cellareatests.cpp ../cellarea.cpp)
add_executable(cellareatests ${CELLAREATESTS_SOURCES})
target_link_libraries(cellareatests ${TESTS_LIBRARIES})
add_test(NAME cellareatests COMMAND cellareatests)

# Unit tests for Quick Actions discovery and execution
set(ACTIONSEARCHTESTS_SOURCES actionsearchtests.cpp ../actionsearch.cpp)
add_executable(actionsearchtests ${ACTIONSEARCHTESTS_SOURCES})
target_link_libraries(actionsearchtests Qt6::Test Qt6::Core Qt6::Gui Qt6::Widgets)
target_compile_definitions(actionsearchtests PRIVATE COMPILE_SCRIBUS_MAIN_APP)
add_test(NAME actionsearchtests COMMAND actionsearchtests)
set_tests_properties(actionsearchtests PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")

# Unit tests for Quick Apply style discovery, filtering and ranking
set(STYLEQUICKAPPLYTESTS_SOURCES stylequickapplytests.cpp ../stylequickapplymodel.cpp)
add_executable(stylequickapplytests ${STYLEQUICKAPPLYTESTS_SOURCES})
target_link_libraries(stylequickapplytests Qt6::Test Qt6::Core)
target_compile_definitions(stylequickapplytests PRIVATE COMPILE_SCRIBUS_MAIN_APP)
add_test(NAME stylequickapplytests COMMAND stylequickapplytests)
set_tests_properties(stylequickapplytests PROPERTIES LABELS "phase-3;quick-apply")

# Unit tests for the inherited object-style value model and named resources
set(OBJECTSTYLETESTS_SOURCES
	objectstyletests.cpp
	../styles/objectstyle.cpp
	../styles/style.cpp
	../styles/stylecontext.cpp
	../styles/stylecontextproxy.cpp
	../updatemanager.cpp
	../observable.h)
add_executable(objectstyletests ${OBJECTSTYLETESTS_SOURCES})
target_link_libraries(objectstyletests Qt6::Test Qt6::Core Qt6::Gui)
target_compile_definitions(objectstyletests PRIVATE COMPILE_SCRIBUS_MAIN_APP)
add_test(NAME objectstyletests COMMAND objectstyletests)
set_tests_properties(objectstyletests PROPERTIES LABELS "object-styles;phase-3")

# Unit tests for cross-platform missing-image filename discovery
set(IMAGELINKMATCHERTESTS_SOURCES imagelinkmatchertests.cpp ../imagelinkmatcher.cpp ../deferredtask.cpp)
add_executable(imagelinkmatchertests ${IMAGELINKMATCHERTESTS_SOURCES})
target_link_libraries(imagelinkmatchertests Qt6::Test Qt6::Core)
target_compile_definitions(imagelinkmatchertests PRIVATE COMPILE_SCRIBUS_MAIN_APP)
add_test(NAME imagelinkmatchertests COMMAND imagelinkmatchertests)
set_tests_properties(imagelinkmatchertests PROPERTIES LABELS "image-relinking;phase-4")

# Unit tests for byte-preserving embedded-image extraction and naming
set(EMBEDDEDIMAGEEXTRACTORTESTS_SOURCES embeddedimageextractortests.cpp ../embeddedimageextractor.cpp)
add_executable(embeddedimageextractortests ${EMBEDDEDIMAGEEXTRACTORTESTS_SOURCES})
target_link_libraries(embeddedimageextractortests Qt6::Test Qt6::Core)
target_compile_definitions(embeddedimageextractortests PRIVATE COMPILE_SCRIBUS_MAIN_APP)
add_test(NAME embeddedimageextractortests COMMAND embeddedimageextractortests)
set_tests_properties(embeddedimageextractortests PROPERTIES LABELS "embedded-image-extraction;phase-4")

# End-to-end running-header tests use the installed application because the
# Scripter and file-format plugins are loaded from the installation layout.
if(APPLEBUNDLE)
	set(SCRIBUS_SCRIPT_TEST_EXECUTABLE
		"${APPLE_APP_DIR}/Scribus${VERSION}.app/Contents/MacOS/Scribus")
elseif(WIN32)
	set(SCRIBUS_SCRIPT_TEST_EXECUTABLE "${CMAKE_INSTALL_PREFIX}/scribus.exe")
else()
	set(SCRIBUS_SCRIPT_TEST_EXECUTABLE
		"${CMAKE_INSTALL_PREFIX}/bin/scribus${TAG_VERSION}")
endif()

add_test(NAME runningheader_test_install
	COMMAND ${CMAKE_COMMAND} --install ${CMAKE_BINARY_DIR} --config $<CONFIG>)
set_tests_properties(runningheader_test_install PROPERTIES
	FIXTURES_SETUP runningheader_test_app
	LABELS "integration;running-headers"
	TIMEOUT 180)

set(dynamic_variable_prefs_dir "${CMAKE_CURRENT_BINARY_DIR}/prefs-dynamic_variables")
set(dynamic_variable_output "${CMAKE_CURRENT_BINARY_DIR}/dynamic_variables.sla")
file(MAKE_DIRECTORY "${dynamic_variable_prefs_dir}")
add_test(NAME dynamic_variables
	COMMAND "${SCRIBUS_SCRIPT_TEST_EXECUTABLE}"
		--no-splash
		--prefs "${dynamic_variable_prefs_dir}"
		--no-gui
		--python-script "${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_dynamic_variables.py")
set_tests_properties(dynamic_variables PROPERTIES
	ENVIRONMENT "QT_QPA_PLATFORM=offscreen;SCRIBUS_DYNAMIC_VARIABLE_TEST_OUTPUT=${dynamic_variable_output}"
	FIXTURES_REQUIRED runningheader_test_app
	LABELS "integration;dynamic-variables;phase-1"
	PASS_REGULAR_EXPRESSION "DYNAMIC_VARIABLE_TEST_PASSED"
	RUN_SERIAL TRUE
	TIMEOUT 180)

set(object_style_prefs_dir "${CMAKE_CURRENT_BINARY_DIR}/prefs-object_style_persistence")
set(object_style_output_dir "${CMAKE_CURRENT_BINARY_DIR}/output-object_style_persistence")
file(MAKE_DIRECTORY "${object_style_prefs_dir}")
file(MAKE_DIRECTORY "${object_style_output_dir}")
add_test(NAME object_style_persistence
	COMMAND "${SCRIBUS_SCRIPT_TEST_EXECUTABLE}"
		--no-splash
		--prefs "${object_style_prefs_dir}"
		--no-gui
		--python-script "${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_object_style_persistence.py")
set_tests_properties(object_style_persistence PROPERTIES
	ENVIRONMENT "QT_QPA_PLATFORM=offscreen;SCRIBUS_TEST_OUTPUT_DIR=${object_style_output_dir}"
	FIXTURES_REQUIRED runningheader_test_app
	LABELS "integration;object-styles;phase-3"
	PASS_REGULAR_EXPRESSION "OBJECT_STYLE_PERSISTENCE_QA_PASSED"
	RUN_SERIAL TRUE
	TIMEOUT 180)

set(object_style_scripter_prefs_dir "${CMAKE_CURRENT_BINARY_DIR}/prefs-object_style_scripter")
set(object_style_scripter_output "${CMAKE_CURRENT_BINARY_DIR}/object_style_scripter.sla")
file(MAKE_DIRECTORY "${object_style_scripter_prefs_dir}")
add_test(NAME object_style_scripter
	COMMAND "${SCRIBUS_SCRIPT_TEST_EXECUTABLE}"
		--no-splash
		--prefs "${object_style_scripter_prefs_dir}"
		--no-gui
		--python-script "${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_object_style_scripter.py")
set_tests_properties(object_style_scripter PROPERTIES
	ENVIRONMENT "QT_QPA_PLATFORM=offscreen;SCRIBUS_OBJECT_STYLE_SCRIPT_OUTPUT=${object_style_scripter_output}"
	FIXTURES_REQUIRED runningheader_test_app
	LABELS "integration;object-styles;phase-3;scripter"
	PASS_REGULAR_EXPRESSION "OBJECT_STYLE_SCRIPTER_QA_PASSED"
	RUN_SERIAL TRUE
	TIMEOUT 180)

set(object_style_import_prefs_dir "${CMAKE_CURRENT_BINARY_DIR}/prefs-object_style_import")
set(object_style_import_output_dir "${CMAKE_CURRENT_BINARY_DIR}/output-object_style_import")
file(MAKE_DIRECTORY "${object_style_import_prefs_dir}")
file(MAKE_DIRECTORY "${object_style_import_output_dir}")
add_test(NAME object_style_import_scripter
	COMMAND "${SCRIBUS_SCRIPT_TEST_EXECUTABLE}"
		--no-splash
		--prefs "${object_style_import_prefs_dir}"
		--no-gui
		--python-script "${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_object_style_import.py")
set_tests_properties(object_style_import_scripter PROPERTIES
	ENVIRONMENT "QT_QPA_PLATFORM=offscreen;SCRIBUS_TEST_OUTPUT_DIR=${object_style_import_output_dir}"
	FIXTURES_REQUIRED runningheader_test_app
	LABELS "integration;object-styles;phase-3;scripter"
	PASS_REGULAR_EXPRESSION "OBJECT_STYLE_IMPORT_QA_PASSED"
	RUN_SERIAL TRUE
	TIMEOUT 180)

set(object_style_management_prefs_dir "${CMAKE_CURRENT_BINARY_DIR}/prefs-object_style_management")
set(object_style_management_output_dir "${CMAKE_CURRENT_BINARY_DIR}/output-object_style_management")
file(MAKE_DIRECTORY "${object_style_management_prefs_dir}")
file(MAKE_DIRECTORY "${object_style_management_output_dir}")
add_test(NAME object_style_management_scripter
	COMMAND "${SCRIBUS_SCRIPT_TEST_EXECUTABLE}"
		--no-splash
		--prefs "${object_style_management_prefs_dir}"
		--no-gui
		--python-script "${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_object_style_management.py")
set_tests_properties(object_style_management_scripter PROPERTIES
	ENVIRONMENT "QT_QPA_PLATFORM=offscreen;SCRIBUS_TEST_OUTPUT_DIR=${object_style_management_output_dir}"
	FIXTURES_REQUIRED runningheader_test_app
	LABELS "integration;object-styles;phase-3;scripter;undo-redo"
	PASS_REGULAR_EXPRESSION "OBJECT_STYLE_MANAGEMENT_QA_PASSED"
	RUN_SERIAL TRUE
	TIMEOUT 180)

function(add_running_header_test test_name script_name pass_marker timeout_seconds)
	set(test_prefs_dir "${CMAKE_CURRENT_BINARY_DIR}/prefs-${test_name}")
	set(test_output_dir "${CMAKE_CURRENT_BINARY_DIR}/output-${test_name}")
	file(MAKE_DIRECTORY "${test_prefs_dir}")
	file(MAKE_DIRECTORY "${test_output_dir}")
	add_test(NAME ${test_name}
		COMMAND "${SCRIBUS_SCRIPT_TEST_EXECUTABLE}"
			--no-splash
			--prefs "${test_prefs_dir}"
			--no-gui
			--python-script "${CMAKE_CURRENT_SOURCE_DIR}/scripts/${script_name}")
	set_tests_properties(${test_name} PROPERTIES
		ENVIRONMENT "QT_QPA_PLATFORM=offscreen;SCRIBUS_TEST_OUTPUT_DIR=${test_output_dir}"
		FIXTURES_REQUIRED runningheader_test_app
		LABELS "integration;running-headers;phase-2"
		PASS_REGULAR_EXPRESSION "${pass_marker}"
		RUN_SERIAL TRUE
		TIMEOUT ${timeout_seconds})
endfunction()

add_running_header_test(runningheader_masters test_running_header_masters.py
	"RUNNING_HEADER_MASTERS_QA_PASSED" 180)
add_running_header_test(runningheader_spreads test_running_header_spreads.py
	"RUNNING_HEADER_SPREADS_QA_PASSED" 120)
add_running_header_test(runningheader_sections test_running_header_sections.py
	"RUNNING_HEADER_SECTIONS_QA_PASSED" 120)
add_running_header_test(runningheader_scale test_running_header_scale.py
	"RUNNING_HEADER_SCALE_QA_PASSED" 180)

set(crossreference_prefs_dir "${CMAKE_CURRENT_BINARY_DIR}/prefs-crossreference_core")
set(crossreference_output_dir "${CMAKE_CURRENT_BINARY_DIR}/output-crossreference_core")
file(MAKE_DIRECTORY "${crossreference_prefs_dir}")
file(MAKE_DIRECTORY "${crossreference_output_dir}")
add_test(NAME crossreference_core
	COMMAND "${SCRIBUS_SCRIPT_TEST_EXECUTABLE}"
		--no-splash
		--prefs "${crossreference_prefs_dir}"
		--no-gui
		--python-script "${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_cross_references.py")
set_tests_properties(crossreference_core PROPERTIES
	ENVIRONMENT "QT_QPA_PLATFORM=offscreen;SCRIBUS_TEST_OUTPUT_DIR=${crossreference_output_dir}"
	FIXTURES_REQUIRED runningheader_test_app
	LABELS "integration;cross-references;phase-3"
	PASS_REGULAR_EXPRESSION "CROSS_REFERENCE_QA_PASSED"
	RUN_SERIAL TRUE
	TIMEOUT 120)

set(anchored_object_prefs_dir "${CMAKE_CURRENT_BINARY_DIR}/prefs-anchored_objects")
set(anchored_object_output_dir "${CMAKE_CURRENT_BINARY_DIR}/output-anchored_objects")
file(MAKE_DIRECTORY "${anchored_object_prefs_dir}")
file(MAKE_DIRECTORY "${anchored_object_output_dir}")
add_test(NAME anchored_objects
	COMMAND "${SCRIBUS_SCRIPT_TEST_EXECUTABLE}"
		--no-splash
		--prefs "${anchored_object_prefs_dir}"
		--no-gui
		--python-script "${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_anchored_objects.py")
set_tests_properties(anchored_objects PROPERTIES
	ENVIRONMENT "QT_QPA_PLATFORM=offscreen;SCRIBUS_TEST_OUTPUT_DIR=${anchored_object_output_dir}"
	FIXTURES_REQUIRED runningheader_test_app
	LABELS "integration;anchored-objects;phase-3"
	PASS_REGULAR_EXPRESSION "ANCHORED_OBJECT_QA_PASSED"
	RUN_SERIAL TRUE
	TIMEOUT 120)

set(image_relink_prefs_dir "${CMAKE_CURRENT_BINARY_DIR}/prefs-image-relink")
set(image_relink_output_dir "${CMAKE_CURRENT_BINARY_DIR}/output-image-relink")
file(MAKE_DIRECTORY "${image_relink_prefs_dir}")
file(MAKE_DIRECTORY "${image_relink_output_dir}")
add_test(NAME image_relinking
	COMMAND "${SCRIBUS_SCRIPT_TEST_EXECUTABLE}"
		--no-splash
		--prefs "${image_relink_prefs_dir}"
		--no-gui
		--python-script "${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_image_relinking.py")
set_tests_properties(image_relinking PROPERTIES
	ENVIRONMENT "QT_QPA_PLATFORM=offscreen;SCRIBUS_TEST_OUTPUT_DIR=${image_relink_output_dir}"
	FIXTURES_REQUIRED runningheader_test_app
	LABELS "integration;image-relinking;phase-4;scripter;undo-redo"
	PASS_REGULAR_EXPRESSION "IMAGE_RELINK_QA_PASSED"
	RUN_SERIAL TRUE
	TIMEOUT 120)

set(embedded_image_prefs_dir "${CMAKE_CURRENT_BINARY_DIR}/prefs-embedded-image")
set(embedded_image_output_dir "${CMAKE_CURRENT_BINARY_DIR}/output-embedded-image")
file(MAKE_DIRECTORY "${embedded_image_prefs_dir}")
file(MAKE_DIRECTORY "${embedded_image_output_dir}")
add_test(NAME embedded_image_extraction
	COMMAND "${SCRIBUS_SCRIPT_TEST_EXECUTABLE}"
		--no-splash
		--prefs "${embedded_image_prefs_dir}"
		--no-gui
		--python-script "${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_embedded_image_extraction.py")
set_tests_properties(embedded_image_extraction PROPERTIES
	ENVIRONMENT "QT_QPA_PLATFORM=offscreen;SCRIBUS_TEST_OUTPUT_DIR=${embedded_image_output_dir}"
	FIXTURES_REQUIRED runningheader_test_app
	LABELS "integration;embedded-images;phase-4;scripter;undo-redo;persistence"
	PASS_REGULAR_EXPRESSION "EMBEDDED_IMAGE_EXTRACTION_QA_PASSED"
	RUN_SERIAL TRUE
	TIMEOUT 120)

find_program(PDFTOTEXT_EXECUTABLE pdftotext)
if(NOT PDFTOTEXT_EXECUTABLE)
	set_tests_properties(runningheader_masters PROPERTIES DISABLED TRUE)
endif()
