EXTRA_DIST = template.xml

install-data-local:
	$(mkinstalldirs) $(prefix)/share/scribus/templates/
	$(INSTALL_DATA) $(srcdir)/template.xml $(prefix)/share/scribus/templates/template.xml
	$(mkinstalldirs) $(prefix)/share/scribus/templates/nl1
	$(INSTALL_DATA) $(srcdir)/nl1/* $(prefix)/share/scribus/templates/nl1/
	$(mkinstalldirs) $(prefix)/share/scribus/templates/nl2
	$(INSTALL_DATA) $(srcdir)/nl2/* $(prefix)/share/scribus/templates/nl2/
	$(mkinstalldirs) $(prefix)/share/scribus/templates/br1
	$(INSTALL_DATA) $(srcdir)/br1/* $(prefix)/share/scribus/templates/br1/

uninstall-local:
	-rm -f $(prefix)/share/scribus/templates/nl1/*
	-rmdir $(prefix)/share/scribus/templates/nl1
	-rm -f $(prefix)/share/scribus/templates/nl2/*
	-rmdir $(prefix)/share/scribus/templates/nl2
	-rm -f $(prefix)/share/scribus/templates/br1/*
	-rmdir $(prefix)/share/scribus/templates/br1
	-rm -f $(prefix)/share/scribus/templates/template.xml
	-rmdir $(prefix)/share/scribus/templates

