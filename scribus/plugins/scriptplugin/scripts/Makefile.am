EXTRA_DIST = ReadMe FontSample.py ChangeLog NEWS TODO CalendarWizard.py

install-data-local:
	$(mkinstalldirs) $(prefix)/share/scribus/scripts/
	$(INSTALL_DATA) $(srcdir)/ReadMe $(prefix)/share/scribus/scripts/ReadMe
	$(INSTALL_DATA) $(srcdir)/ChangeLog $(prefix)/share/scribus/scripts/ChangeLog
	$(INSTALL_DATA) $(srcdir)/NEWS $(prefix)/share/scribus/scripts/NEWS
	$(INSTALL_DATA) $(srcdir)/TODO $(prefix)/share/scribus/scripts/TODO
	$(INSTALL_DATA) $(srcdir)/FontSample.py $(prefix)/share/scribus/scripts/FontSample.py
	$(INSTALL_DATA) $(srcdir)/CalendarWizard.py $(prefix)/share/scribus/scripts/CalendarWizard.py
	


uninstall-local:
	-rm -f $(prefix)/share/scribus/scripts/ReadMe
	-rm -f $(prefix)/share/scribus/scripts/ChangeLog
	-rm -f $(prefix)/share/scribus/scripts/NEWS
	-rm -f $(prefix)/share/scribus/scripts/TODO
	-rm -f $(prefix)/share/scribus/scripts/FontSample.py
	-rm -f $(prefix)/share/scribus/scripts/CalendarWizard.py
