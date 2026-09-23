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

set(IMAGECMYKEXPORTTESTS_SOURCES imagecmykexporttests.cpp ../imagecmykexport.cpp)
add_executable(imagecmykexporttests ${IMAGECMYKEXPORTTESTS_SOURCES})
target_link_libraries(imagecmykexporttests Qt6::Test Qt6::Core Qt6::Gui ${TIFF_LIBRARIES})
target_compile_definitions(imagecmykexporttests PRIVATE COMPILE_SCRIBUS_MAIN_APP)
add_test(NAME imagecmykexporttests COMMAND imagecmykexporttests)
set_tests_properties(imagecmykexporttests PROPERTIES LABELS "image-color-conversion;phase-4")

add_executable(imagealphacontourtests imagealphacontourtests.cpp ../imagealphasilhouette.cpp)
target_link_libraries(imagealphacontourtests ${TESTS_LIBRARIES} Qt6::Gui Qt6::Widgets)
target_compile_definitions(imagealphacontourtests PRIVATE COMPILE_SCRIBUS_MAIN_APP)
add_test(NAME imagealphacontourtests COMMAND imagealphacontourtests)

add_executable(collectmanifesttests collectmanifesttests.cpp ../collectmanifest.cpp)
target_link_libraries(collectmanifesttests Qt6::Test Qt6::Core)
target_compile_definitions(collectmanifesttests PRIVATE COMPILE_SCRIBUS_MAIN_APP)
add_test(NAME collectmanifesttests COMMAND collectmanifesttests)
set_tests_properties(collectmanifesttests PROPERTIES LABELS "collect-for-output;phase-4")
set_tests_properties(imagealphacontourtests PROPERTIES LABELS "image-contour;phase-4")
set_tests_properties(imagealphacontourtests PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")

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

set(image_link_replacement_prefs_dir "${CMAKE_CURRENT_BINARY_DIR}/prefs-image-link-replacement")
set(image_link_replacement_output_dir "${CMAKE_CURRENT_BINARY_DIR}/output-image-link-replacement")
file(MAKE_DIRECTORY "${image_link_replacement_prefs_dir}")
file(MAKE_DIRECTORY "${image_link_replacement_output_dir}")
add_test(NAME image_link_replacement
	COMMAND "${SCRIBUS_SCRIPT_TEST_EXECUTABLE}"
		--no-splash
		--prefs "${image_link_replacement_prefs_dir}"
		--no-gui
		--python-script "${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_image_link_replacement.py")
set_tests_properties(image_link_replacement PROPERTIES
	ENVIRONMENT "QT_QPA_PLATFORM=offscreen;SCRIBUS_TEST_OUTPUT_DIR=${image_link_replacement_output_dir}"
	FIXTURES_REQUIRED runningheader_test_app
	LABELS "integration;image-relinking;phase-4;scripter;undo-redo;persistence"
	PASS_REGULAR_EXPRESSION "IMAGE_LINK_REPLACEMENT_QA_PASSED"
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

set(document_font_replacement_prefs_dir "${CMAKE_CURRENT_BINARY_DIR}/prefs-document-font-replacement")
set(document_font_replacement_output_dir "${CMAKE_CURRENT_BINARY_DIR}/output-document-font-replacement")
file(MAKE_DIRECTORY "${document_font_replacement_prefs_dir}")
file(MAKE_DIRECTORY "${document_font_replacement_output_dir}")
add_test(NAME document_font_replacement
	COMMAND "${SCRIBUS_SCRIPT_TEST_EXECUTABLE}"
		--no-splash
		--prefs "${document_font_replacement_prefs_dir}"
		--no-gui
		--python-script "${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_document_font_replacement.py")
set_tests_properties(document_font_replacement PROPERTIES
	ENVIRONMENT "QT_QPA_PLATFORM=offscreen;SCRIBUS_TEST_OUTPUT_DIR=${document_font_replacement_output_dir}"
	FIXTURES_REQUIRED runningheader_test_app
	LABELS "integration;font-replacement;phase-4;scripter;undo-redo;persistence"
	PASS_REGULAR_EXPRESSION "DOCUMENT_FONT_REPLACEMENT_QA_PASSED"
	RUN_SERIAL TRUE
	TIMEOUT 120)

set(rgb_to_cmyk_prefs_dir "${CMAKE_CURRENT_BINARY_DIR}/prefs-rgb-to-cmyk")
set(rgb_to_cmyk_output_dir "${CMAKE_CURRENT_BINARY_DIR}/output-rgb-to-cmyk")
file(MAKE_DIRECTORY "${rgb_to_cmyk_prefs_dir}")
file(MAKE_DIRECTORY "${rgb_to_cmyk_output_dir}")
add_test(NAME rgb_to_cmyk
	COMMAND "${SCRIBUS_SCRIPT_TEST_EXECUTABLE}"
		--no-splash
		--prefs "${rgb_to_cmyk_prefs_dir}"
		--no-gui
		--python-script "${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_rgb_to_cmyk.py")
set_tests_properties(rgb_to_cmyk PROPERTIES
	ENVIRONMENT "QT_QPA_PLATFORM=offscreen;SCRIBUS_TEST_OUTPUT_DIR=${rgb_to_cmyk_output_dir}"
	FIXTURES_REQUIRED runningheader_test_app
	LABELS "integration;rgb-to-cmyk;phase-4;scripter;undo-redo;persistence"
	PASS_REGULAR_EXPRESSION "RGB_TO_CMYK_QA_PASSED"
	RUN_SERIAL TRUE
	TIMEOUT 120)

set(image_cmyk_export_prefs_dir "${CMAKE_CURRENT_BINARY_DIR}/prefs-image-cmyk-export")
set(image_alpha_contour_prefs_dir "${CMAKE_CURRENT_BINARY_DIR}/prefs-image-alpha-contour")
set(image_alpha_contour_output_dir "${CMAKE_CURRENT_BINARY_DIR}/output-image-alpha-contour")
file(MAKE_DIRECTORY "${image_alpha_contour_prefs_dir}")
file(MAKE_DIRECTORY "${image_alpha_contour_output_dir}")
add_test(NAME image_alpha_contour
	COMMAND "${SCRIBUS_SCRIPT_TEST_EXECUTABLE}"
		--no-splash
		--prefs "${image_alpha_contour_prefs_dir}"
		--no-gui
		--python-script "${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_image_alpha_contour.py")
set_tests_properties(image_alpha_contour PROPERTIES
	ENVIRONMENT "QT_QPA_PLATFORM=offscreen;SCRIBUS_TEST_OUTPUT_DIR=${image_alpha_contour_output_dir}"
	FIXTURES_REQUIRED runningheader_test_app
	LABELS "integration;image-contour;phase-4;scripter;undo-redo;persistence"
	PASS_REGULAR_EXPRESSION "IMAGE_ALPHA_CONTOUR_QA_PASSED"
	RUN_SERIAL TRUE
	TIMEOUT 120)

set(image_cmyk_export_output_dir "${CMAKE_CURRENT_BINARY_DIR}/output-image-cmyk-export")
file(MAKE_DIRECTORY "${image_cmyk_export_prefs_dir}")
file(MAKE_DIRECTORY "${image_cmyk_export_output_dir}")
add_test(NAME image_cmyk_export
	COMMAND "${SCRIBUS_SCRIPT_TEST_EXECUTABLE}"
		--no-splash
		--prefs "${image_cmyk_export_prefs_dir}"
		--no-gui
		--python-script "${CMAKE_CURRENT_SOURCE_DIR}/scripts/test_image_cmyk_export.py")
set_tests_properties(image_cmyk_export PROPERTIES
	ENVIRONMENT "QT_QPA_PLATFORM=offscreen;SCRIBUS_TEST_OUTPUT_DIR=${image_cmyk_export_output_dir}"
	FIXTURES_REQUIRED runningheader_test_app
	LABELS "integration;image-color-conversion;phase-4;scripter"
	PASS_REGULAR_EXPRESSION "IMAGE_CMYK_EXPORT_QA_PASSED"
	RUN_SERIAL TRUE
	TIMEOUT 120)

find_program(PDFTOTEXT_EXECUTABLE pdftotext)
if(NOT PDFTOTEXT_EXECUTABLE)
	set_tests_properties(runningheader_masters PROPERTIES DISABLED TRUE)
endif()
