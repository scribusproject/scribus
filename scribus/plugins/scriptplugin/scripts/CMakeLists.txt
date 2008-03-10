INCLUDE_DIRECTORIES(
"${CMAKE_SOURCE_DIR}/scribus"
)

INSTALL(FILES
BleedAndPrintMarks.py
CalendarWizard.py
DirectImageImport.py
DrawGrid.py
ExtractText.py
FontSample.py
ImageWizard.py
InfoBox.py
UnflipContent.py
ChangeLog
NEWS
ReadMe
TODO
  DESTINATION ${SCRIPTSDIR}
)
