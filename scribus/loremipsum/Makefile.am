EXTRA_DIST = loremipsum.xml arabic.xml czech.xml greece.xml jiddish.xml russian.xml

install-data-local:
	$(mkinstalldirs) $(prefix)/share/scribus/loremipsum/
	$(INSTALL_DATA) $(srcdir)/loremipsum.xml $(prefix)/share/scribus/loremipsum/loremipsum.xml
	$(INSTALL_DATA) $(srcdir)/arabic.xml $(prefix)/share/scribus/loremipsum/arabic.xml
	$(INSTALL_DATA) $(srcdir)/czech.xml $(prefix)/share/scribus/loremipsum/czech.xml
	$(INSTALL_DATA) $(srcdir)/greece.xml $(prefix)/share/scribus/loremipsum/greece.xml
	$(INSTALL_DATA) $(srcdir)/jiddish.xml $(prefix)/share/scribus/loremipsum/jiddish.xml
	$(INSTALL_DATA) $(srcdir)/russian.xml $(prefix)/share/scribus/loremipsum/russian.xml

uninstall-local:
	-rm -f $(prefix)/share/scribus/loremipsum/loremipsum.xml
	-rm -f $(prefix)/share/scribus/loremipsum/arabic.xml
	-rm -f $(prefix)/share/scribus/loremipsum/czech.xml
	-rm -f $(prefix)/share/scribus/loremipsum/greece.xml
	-rm -f $(prefix)/share/scribus/loremipsum/jiddish.xml
	-rm -f $(prefix)/share/scribus/loremipsum/russian.xml
