EXTRA_DIST = scribus.1 scribus.1.pl

install-data-local:
	$(mkinstalldirs) $(mandir)
	$(mkinstalldirs) $(mandir)/man1
	$(mkinstalldirs) $(mandir)/pl
	$(mkinstalldirs) $(mandir)/pl/man1
	$(INSTALL_DATA) $(srcdir)/scribus.1 $(mandir)/man1/scribus.1
	$(mkinstalldirs) $(mandir)/pl/man1
	$(INSTALL_DATA) $(srcdir)/scribus.1.pl $(mandir)/pl/man1/scribus.1

uninstall-local:
	-rm -f $(mandir)/man1/scribus.1
	-rm -f $(mandir)/pl/man1/scribus.1
