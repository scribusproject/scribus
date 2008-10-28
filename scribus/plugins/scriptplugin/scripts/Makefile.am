EXTRA_DIST = ReadMe FontSample.py ChangeLog NEWS TODO CalendarWizard.py CMakeLists.txt BleedAndPrintMarks.py DirectImageImport.py DrawGrid.py ExtractText.py InfoBox.py UnflipContent.py

install-data-local:
	$(mkinstalldirs) $(prefix)/share/scribus/scripts/
	$(INSTALL_DATA) $(srcdir)/ReadMe $(prefix)/share/scribus/scripts/ReadMe
	$(INSTALL_DATA) $(srcdir)/ChangeLog $(prefix)/share/scribus/scripts/ChangeLog
	$(INSTALL_DATA) $(srcdir)/NEWS $(prefix)/share/scribus/scripts/NEWS
	$(INSTALL_DATA) $(srcdir)/TODO $(prefix)/share/scribus/scripts/TODO
	$(INSTALL_DATA) $(srcdir)/BleedAndPrintMarks.py $(prefix)/share/scribus/scripts/BleedAndPrintMarks.py
	$(INSTALL_DATA) $(srcdir)/CalendarWizard.py $(prefix)/share/scribus/scripts/CalendarWizard.py
	$(INSTALL_DATA) $(srcdir)/DirectImageImport.py $(prefix)/share/scribus/scripts/DirectImageImport.py
	$(INSTALL_DATA) $(srcdir)/DrawGrid.py $(prefix)/share/scribus/scripts/DrawGrid.py
	$(INSTALL_DATA) $(srcdir)/ExtractText.py $(prefix)/share/scribus/scripts/ExtractText.py
	$(INSTALL_DATA) $(srcdir)/FontSample.py $(prefix)/share/scribus/scripts/FontSample.py
	$(INSTALL_DATA) $(srcdir)/InfoBox.py $(prefix)/share/scribus/scripts/InfoBox.py
	$(INSTALL_DATA) $(srcdir)/UnflipContent.py $(prefix)/share/scribus/scripts/UnflipContent.py
	$(INSTALL_DATA) $(srcdir)/ColorChart.py $(prefix)/share/scribus/scripts/ColorChart.py
	$(INSTALL_DATA) $(srcdir)/color2csv.py $(prefix)/share/scribus/scripts/color2csv.py
	$(INSTALL_DATA) $(srcdir)/csv2color.py $(prefix)/share/scribus/scripts/csv2color.py
	


uninstall-local:
	-rm -f $(prefix)/share/scribus/scripts/ReadMe
	-rm -f $(prefix)/share/scribus/scripts/ChangeLog
	-rm -f $(prefix)/share/scribus/scripts/NEWS
	-rm -f $(prefix)/share/scribus/scripts/TODO
	-rm -f $(prefix)/share/scribus/scripts/BleedAndPrintMarks.py
	-rm -f $(prefix)/share/scribus/scripts/CalendarWizard.py
	-rm -f $(prefix)/share/scribus/scripts/DirectImageImport.py
	-rm -f $(prefix)/share/scribus/scripts/DrawGrid.py
	-rm -f $(prefix)/share/scribus/scripts/ExtractText.py
	-rm -f $(prefix)/share/scribus/scripts/FontSample.py
	-rm -f $(prefix)/share/scribus/scripts/InfoBox.py
	-rm -f $(prefix)/share/scribus/scripts/UnflipContent.py
	-rm -f $(prefix)/share/scribus/scripts/ColorChart.py
	-rm -f $(prefix)/share/scribus/scripts/color2csv.py
	-rm -f $(prefix)/share/scribus/scripts/csv2color.py

