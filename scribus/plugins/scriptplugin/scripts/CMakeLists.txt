INCLUDE_DIRECTORIES(
"${CMAKE_SOURCE_DIR}/scribus"
)

INSTALL(FILES
CalendarWizard.py
FontSample.py
InfoBox.py
ChangeLog
NEWS
ReadMe
TODO
  DESTINATION ${SCRIPTSDIR}
)