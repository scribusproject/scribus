include_directories(
"${CMAKE_SOURCE_DIR}/scribus"
)

install(FILES
Autoquote.py
Autoquote2.py
CalendarWizard.py
Caption.py
ColorChart.py
DirectImageImport.py
FontSample.py
InfoBox.py
Ligatursatz.py
color2csv.py
csv2color.py
importcsv2table.py
  DESTINATION ${SCRIPTSDIR}
)
