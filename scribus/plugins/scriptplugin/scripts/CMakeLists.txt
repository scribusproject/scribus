INCLUDE_DIRECTORIES(
"${CMAKE_SOURCE_DIR}/scribus"
)

INSTALL(FILES
BleedAndPrintMarks.py
CalendarWizard.py
DirectImageImport.py
ExtractText.py
FontSample.py
InfoBox.py
UnflipContent.py
ChangeLog
NEWS
ReadMe
TODO
color2csv.py
csv2color.py
ColorChart.py
  DESTINATION ${SCRIPTSDIR}
)
