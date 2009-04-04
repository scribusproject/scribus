INCLUDE_DIRECTORIES(
"${CMAKE_SOURCE_DIR}/scribus"
)

INSTALL(FILES
CalendarWizard.py
ColorChart.py
DirectImageImport.py
FontSample.py
InfoBox.py
ChangeLog
NEWS
ReadMe
TODO
color2csv.py
csv2color.py
importcsv2table.py
  DESTINATION ${SCRIPTSDIR}
)
