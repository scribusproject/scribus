# An expected Python exception must be the reason for headless Scribus to fail.
# A missing executable, bad preferences path, or crash must not pass this test.
execute_process(
	COMMAND "${CMAKE_COMMAND}" -E env "QT_QPA_PLATFORM=offscreen"
		"${APP}" --no-splash --prefs "${PREFS}" --no-gui
		--python-script "${TEST_SCRIPT}"
	RESULT_VARIABLE result
	OUTPUT_VARIABLE stdout
	ERROR_VARIABLE stderr
	TIMEOUT 180)

if(NOT "${result}" STREQUAL "1" OR NOT "${stdout}${stderr}" MATCHES "HEADLESS_SCRIPT_FAILURE_EXPECTED")
	message(FATAL_ERROR "Expected Python failure with exit status 1; got '${result}'.\n${stdout}\n${stderr}")
endif()
