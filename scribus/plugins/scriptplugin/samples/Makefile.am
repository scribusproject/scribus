####### kdevelop will overwrite this part!!! (begin)##########


EXTRA_DIST = Sample1.py Calender.py ReadMe 

install-data-local:
	$(mkinstalldirs) $(prefix)/lib/scribus/samples/
	$(INSTALL_DATA) $(srcdir)/Sample1.py $(prefix)/lib/scribus/samples/Sample1.py
	$(mkinstalldirs) $(prefix)/lib/scribus/samples/
	$(INSTALL_DATA) $(srcdir)/Calender.py $(prefix)/lib/scribus/samples/Calendar.py
	$(mkinstalldirs) $(prefix)/lib/scribus/samples/
	$(INSTALL_DATA) $(srcdir)/ReadMe $(prefix)/lib/scribus/samples/ReadMe

uninstall-local:
	-rm -f $(prefix)/lib/scribus/samples/Sample1.py
	-rm -f $(prefix)/lib/scribus/samples/Calendar.py
	-rm -f $(prefix)/lib/scribus/samples/ReadMe

####### kdevelop will overwrite this part!!! (end)############
