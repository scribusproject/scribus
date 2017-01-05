INCLUDE_DIRECTORIES(
"${CMAKE_SOURCE_DIR}/scribus"
)

INSTALL(FILES
Autoquote.py
Align_image_in_frame.py
CalendarWizard.py
Caption.py
ColorChart.py
DirectImageImport.py
FontSample.py
Gallery.py
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
