EXTRA_DIST = scribus.1 scribus.1.pl

install-data-local:
	$(mkinstalldirs) $(prefix)/share/man
	$(mkinstalldirs) $(prefix)/share/man/man1
	$(INSTALL_DATA) $(srcdir)/scribus.1 $(prefix)/share/man/man1/scribus.1
	$(mkinstalldirs) $(prefix)/share/man/pl/man1
	$(INSTALL_DATA) $(srcdir)/scribus.1.pl $(prefix)/share/man/pl/man1/scribus.1

uninstall-local:
	-rm -f $(prefix)/share/man/man1/scribus.1
	-rm -f $(prefix)/share/man/pl/man1/scribus.1
