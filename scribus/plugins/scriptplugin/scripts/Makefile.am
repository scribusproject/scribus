####### kdevelop will overwrite this part!!! (begin)##########


EXTRA_DIST = ReadMe FontSample.py 

install-data-local:
	$(mkinstalldirs) $(prefix)/share/scribus/scripts/
	$(INSTALL_DATA) $(srcdir)/ReadMe $(prefix)/share/scribus/scripts/ReadMe
	$(INSTALL_DATA) $(srcdir)/FontSample.py $(prefix)/share/scribus/scripts/FontSample.py


uninstall-local:
	-rm -f $(prefix)/share/scribus/scripts/ReadMe
	-rm -f $(prefix)/share/scribus/scripts/FontSample.py

####### kdevelop will overwrite this part!!! (end)############
