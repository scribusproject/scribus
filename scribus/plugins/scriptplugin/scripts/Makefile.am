EXTRA_DIST = ReadMe FontSample.py ChangeLog NEWS TODO CalendarWizard.py

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/scribus/scripts/
	$(INSTALL_DATA) $(DESTDIR)$(srcdir)/ReadMe $(prefix)/share/scribus/scripts/ReadMe
	$(INSTALL_DATA) $(DESTDIR)$(srcdir)/ChangeLog $(prefix)/share/scribus/scripts/ChangeLog
	$(INSTALL_DATA) $(DESTDIR)$(srcdir)/NEWS $(prefix)/share/scribus/scripts/NEWS
	$(INSTALL_DATA) $(DESTDIR)$(srcdir)/TODO $(prefix)/share/scribus/scripts/TODO
	$(INSTALL_DATA) $(DESTDIR)$(srcdir)/FontSample.py $(prefix)/share/scribus/scripts/FontSample.py
	$(INSTALL_DATA) $(DESTDIR)$(srcdir)/CalendarWizard.py $(prefix)/share/scribus/scripts/CalendarWizard.py
	


uninstall-local:
	-rm -f $(DESTDIR)$(prefix)/share/scribus/scripts/ReadMe
	-rm -f $(DESTDIR)$(prefix)/share/scribus/scripts/ChangeLog
	-rm -f $(DESTDIR)$(prefix)/share/scribus/scripts/NEWS
	-rm -f $(DESTDIR)$(prefix)/share/scribus/scripts/TODO
	-rm -f $(DESTDIR)$(prefix)/share/scribus/scripts/FontSample.py
	-rm -f $(DESTDIR)$(prefix)/share/scribus/scripts/CalendarWizard.py
