EXTRA_DIST = loremipsum.xml arabic.xml czech.xml greek.xml hebrew.xml russian.xml

install-data-local:
	$(mkinstalldirs) $(prefix)/share/scribus/loremipsum/
	$(INSTALL_DATA) $(srcdir)/loremipsum.xml $(prefix)/share/scribus/loremipsum/loremipsum.xml
	$(INSTALL_DATA) $(srcdir)/arabic.xml $(prefix)/share/scribus/loremipsum/arabic.xml
	$(INSTALL_DATA) $(srcdir)/czech.xml $(prefix)/share/scribus/loremipsum/czech.xml
	$(INSTALL_DATA) $(srcdir)/greek.xml $(prefix)/share/scribus/loremipsum/greek.xml
	$(INSTALL_DATA) $(srcdir)/hebrew.xml $(prefix)/share/scribus/loremipsum/hebrew.xml
	$(INSTALL_DATA) $(srcdir)/russian.xml $(prefix)/share/scribus/loremipsum/russian.xml

uninstall-local:
	-rm -f $(prefix)/share/scribus/loremipsum/loremipsum.xml
	-rm -f $(prefix)/share/scribus/loremipsum/arabic.xml
	-rm -f $(prefix)/share/scribus/loremipsum/czech.xml
	-rm -f $(prefix)/share/scribus/loremipsum/greek.xml
	-rm -f $(prefix)/share/scribus/loremipsum/hebrew.xml
	-rm -f $(prefix)/share/scribus/loremipsum/russian.xml
