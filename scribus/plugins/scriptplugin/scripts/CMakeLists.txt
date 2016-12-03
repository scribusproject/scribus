include_directories(
"${CMAKE_SOURCE_DIR}/scribus"
)

install(FILES
Autoquote.py
Autoquote2.py
CalendarWizard.py
ColorChart.py
DirectImageImport.py
FontSample.py
InfoBox.py
Ligatursatz.py
ChangeLog
NEWS
ReadMe
TODO
color2csv.py
csv2color.py
importcsv2table.py
  DESTINATION ${SCRIPTSDIR}
)
