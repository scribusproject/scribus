EXTRA_DIST = ReadMe FontSample.py ChangeLog NEWS TODO

install-data-local:
	$(mkinstalldirs) $(prefix)/share/scribus/scripts/
	$(INSTALL_DATA) $(srcdir)/ReadMe $(prefix)/share/scribus/scripts/ReadMe
	$(INSTALL_DATA) $(srcdir)/ReadMe $(prefix)/share/scribus/scripts/ChangeLog
	$(INSTALL_DATA) $(srcdir)/ReadMe $(prefix)/share/scribus/scripts/NEWS
	$(INSTALL_DATA) $(srcdir)/ReadMe $(prefix)/share/scribus/scripts/TODO
	$(INSTALL_DATA) $(srcdir)/FontSample.py $(prefix)/share/scribus/scripts/FontSample.py


uninstall-local:
	-rm -f $(prefix)/share/scribus/scripts/ReadMe
	-rm -f $(prefix)/share/scribus/scripts/ChangeLog
	-rm -f $(prefix)/share/scribus/scripts/NEWS
	-rm -f $(prefix)/share/scribus/scripts/TODO
	-rm -f $(prefix)/share/scribus/scripts/FontSample.py
