EXTRA_DIST = la.xml ar.xml cs.xml fi.xml de.xml el.xml he.xml ru.xml it.xml sl.xml CMakeLists.txt

install-data-local:
	$(mkinstalldirs) $(prefix)/share/scribus/loremipsum/
	$(INSTALL_DATA) $(srcdir)/la.xml $(prefix)/share/scribus/loremipsum/la.xml
	$(INSTALL_DATA) $(srcdir)/ar.xml $(prefix)/share/scribus/loremipsum/ar.xml
	$(INSTALL_DATA) $(srcdir)/cs.xml $(prefix)/share/scribus/loremipsum/cs.xml
	$(INSTALL_DATA) $(srcdir)/fi.xml $(prefix)/share/scribus/loremipsum/fi.xml
	$(INSTALL_DATA) $(srcdir)/de.xml $(prefix)/share/scribus/loremipsum/de.xml
	$(INSTALL_DATA) $(srcdir)/el.xml $(prefix)/share/scribus/loremipsum/el.xml
	$(INSTALL_DATA) $(srcdir)/he.xml $(prefix)/share/scribus/loremipsum/he.xml
	$(INSTALL_DATA) $(srcdir)/it.xml $(prefix)/share/scribus/loremipsum/it.xml
	$(INSTALL_DATA) $(srcdir)/ru.xml $(prefix)/share/scribus/loremipsum/ru.xml
	$(INSTALL_DATA) $(srcdir)/sl.xml $(prefix)/share/scribus/loremipsum/sl.xml

uninstall-local:
	-rm -f $(prefix)/share/scribus/loremipsum/la.xml
	-rm -f $(prefix)/share/scribus/loremipsum/ar.xml
	-rm -f $(prefix)/share/scribus/loremipsum/cs.xml
	-rm -f $(prefix)/share/scribus/loremipsum/fi.xml
	-rm -f $(prefix)/share/scribus/loremipsum/de.xml
	-rm -f $(prefix)/share/scribus/loremipsum/el.xml
	-rm -f $(prefix)/share/scribus/loremipsum/he.xml
	-rm -f $(prefix)/share/scribus/loremipsum/ru.xml
	-rm -f $(prefix)/share/scribus/loremipsum/sl.xml
