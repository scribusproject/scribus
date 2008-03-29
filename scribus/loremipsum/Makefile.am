EXTRA_DIST = loremipsum.xml arabic.xml czech.xml finnish.xml english.xml german.xml greek.xml hebrew.xml russian.xml italian.xml slovenian.xml CMakeLists.txt dutch.xml eo.xml fr.xml hu.xml

install-data-local:
	$(mkinstalldirs) $(prefix)/share/scribus/loremipsum/
	$(INSTALL_DATA) $(srcdir)/loremipsum.xml $(prefix)/share/scribus/loremipsum/loremipsum.xml
	$(INSTALL_DATA) $(srcdir)/arabic.xml $(prefix)/share/scribus/loremipsum/arabic.xml
	$(INSTALL_DATA) $(srcdir)/czech.xml $(prefix)/share/scribus/loremipsum/czech.xml
	$(INSTALL_DATA) $(srcdir)/dutch.xml $(prefix)/share/scribus/loremipsum/dutch.xml
	$(INSTALL_DATA) $(srcdir)/fr.xml $(prefix)/share/scribus/loremipsum/fr.xml
	$(INSTALL_DATA) $(srcdir)/finnish.xml $(prefix)/share/scribus/loremipsum/finnish.xml
	$(INSTALL_DATA) $(srcdir)/german.xml $(prefix)/share/scribus/loremipsum/german.xml
	$(INSTALL_DATA) $(srcdir)/english.xml $(prefix)/share/scribus/loremipsum/english.xml
	$(INSTALL_DATA) $(srcdir)/eo.xml $(prefix)/share/scribus/loremipsum/eo.xml
	$(INSTALL_DATA) $(srcdir)/greek.xml $(prefix)/share/scribus/loremipsum/greek.xml
	$(INSTALL_DATA) $(srcdir)/hebrew.xml $(prefix)/share/scribus/loremipsum/hebrew.xml
        $(INSTALL_DATA) $(srcdir)/hu.xml $(prefix)/share/scribus/loremipsum/hu.xml
	$(INSTALL_DATA) $(srcdir)/italian.xml $(prefix)/share/scribus/loremipsum/italian.xml
	$(INSTALL_DATA) $(srcdir)/russian.xml $(prefix)/share/scribus/loremipsum/russian.xml
	$(INSTALL_DATA) $(srcdir)/slovenian.xml $(prefix)/share/scribus/loremipsum/slovenian.xml

uninstall-local:
	-rm -f $(prefix)/share/scribus/loremipsum/loremipsum.xml
	-rm -f $(prefix)/share/scribus/loremipsum/arabic.xml
	-rm -f $(prefix)/share/scribus/loremipsum/czech.xml
	-rm -f $(prefix)/share/scribus/loremipsum/dutch.xml
	-rm -f $(prefix)/share/scribus/loremipsum/english.xml
	-rm -f $(prefix)/share/scribus/loremipsum/eo.xml
	-rm -f $(prefix)/share/scribus/loremipsum/finnish.xml
	-rm -f $(prefix)/share/scribus/loremipsum/fr.xml
	-rm -f $(prefix)/share/scribus/loremipsum/german.xml
	-rm -f $(prefix)/share/scribus/loremipsum/greek.xml
	-rm -f $(prefix)/share/scribus/loremipsum/hebrew.xml
	-rm -f $(prefix)/share/scribus/loremipsum/hu.xml
	-rm -f $(prefix)/share/scribus/loremipsum/russian.xml
	-rm -f $(prefix)/share/scribus/loremipsum/slovenian.xml
