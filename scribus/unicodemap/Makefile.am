EXTRA_DIST = unicodenameslist.txt CMakeLists.txt

install-data-local:
	$(mkinstalldirs) $(datadir)/scribus/
	$(INSTALL_DATA) $(srcdir)/unicodenameslist.txt $(datadir)/scribus/unicodenameslist.txt

uninstall-local:
	-rm -f $(datadir)/unicodenameslist.txt

