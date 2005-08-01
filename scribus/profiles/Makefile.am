####### kdevelop will overwrite this part!!! (begin)##########


EXTRA_DIST = Readme srgb.license sRGB.icm 

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(libdir)/scribus/profiles/
	$(INSTALL_DATA) $(srcdir)/Readme $(DESTDIR)$(libdir)/scribus/profiles/ReadMe
	$(INSTALL_DATA) $(srcdir)/srgb.license $(DESTDIR)$(libdir)/scribus/profiles/srgb.license
	$(INSTALL_DATA) $(srcdir)/sRGB.icm $(DESTDIR)$(libdir)/scribus/profiles/sRGB.icm

uninstall-local:
	-rm -f $(DESTDIR)$(libdir)/scribus/profiles/ReadMe

####### kdevelop will overwrite this part!!! (end)############
