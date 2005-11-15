####### kdevelop will overwrite this part!!! (begin)##########


EXTRA_DIST = Readme srgb.license sRGB.icm tr01_d50.icm Fogra27L.icm

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(libdir)/scribus/profiles/
	$(INSTALL_DATA) $(srcdir)/Readme $(DESTDIR)$(libdir)/scribus/profiles/ReadMe
	$(INSTALL_DATA) $(srcdir)/srgb.license $(DESTDIR)$(libdir)/scribus/profiles/srgb.license
	$(INSTALL_DATA) $(srcdir)/sRGB.icm $(DESTDIR)$(libdir)/scribus/profiles/sRGB.icm
	$(INSTALL_DATA) $(srcdir)/Fogra27L.icm $(DESTDIR)$(libdir)/scribus/profiles/Fogra27L.icm
	$(INSTALL_DATA) $(srcdir)/tr01_d50.icm $(DESTDIR)$(libdir)/scribus/profiles/tr01_d50.icm

uninstall-local:
	-rm -f $(DESTDIR)$(libdir)/scribus/profiles/ReadMe
	-rm -f $(DESTDIR)$(libdir)/scribus/profiles/sRGB.icm
	-rm -f $(DESTDIR)$(libdir)/scribus/profiles/srgb.license
	-rm -f $(DESTDIR)$(libdir)/scribus/profiles/Fogra27L.icm
	-rm -f $(DESTDIR)$(libdir)/scribus/profiles/tr01_d50.icm
	-rmdir 	$(DESTDIR)$(libdir)/scribus/profiles/ 2>/dev/null

####### kdevelop will overwrite this part!!! (end)############
