EXTRA_DIST = docheader1.png docheader2.png

install-data-local:
	$(mkinstalldirs) $(prefix)/share/scribus/doc/en/images/
	$(INSTALL_DATA) $(srcdir)/docheader1.png $(prefix)/share/scribus/doc/en/images/docheader1.png
	$(mkinstalldirs) $(prefix)/share/scribus/doc/en/images/
	$(INSTALL_DATA) $(srcdir)/docheader2.png $(prefix)/share/scribus/doc/en/images/docheader2.png
		
uninstall-local:
	-rm -f $(prefix)/share/scribus/doc/en/images/docheader1.png
	-rm -f $(prefix)/share/scribus/doc/en/images/docheader2.png
