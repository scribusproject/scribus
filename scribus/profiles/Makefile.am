####### kdevelop will overwrite this part!!! (begin)##########


EXTRA_DIST = Readme srgb.license sRGB.icm GenericCMYK.icm GenericCMYK.txt

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(libdir)/scribus/profiles/
	$(INSTALL_DATA) $(srcdir)/Readme $(DESTDIR)$(libdir)/scribus/profiles/ReadMe
	$(INSTALL_DATA) $(srcdir)/srgb.license $(DESTDIR)$(libdir)/scribus/profiles/srgb.license
	$(INSTALL_DATA) $(srcdir)/sRGB.icm $(DESTDIR)$(libdir)/scribus/profiles/sRGB.icm
	$(INSTALL_DATA) $(srcdir)/GenericCMYK.icm $(DESTDIR)$(libdir)/scribus/profiles/GenericCMYK.icm
	$(INSTALL_DATA) $(srcdir)/GenericCMYK.txt $(DESTDIR)$(libdir)/scribus/profiles/GenericCMYK.txt

uninstall-local:
	-rm -f $(DESTDIR)$(libdir)/scribus/profiles/ReadMe
	-rm -f $(DESTDIR)$(libdir)/scribus/profiles/sRGB.icm
	-rm -f $(DESTDIR)$(libdir)/scribus/profiles/srgb.license
	-rm -f $(DESTDIR)$(libdir)/scribus/profiles/GenericCMYK.icm
	-rm -f $(DESTDIR)$(libdir)/scribus/profiles/GenericCMYK.txt
####### kdevelop will overwrite this part!!! (end)############
