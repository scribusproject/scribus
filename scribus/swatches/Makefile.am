EXTRA_DIST = Scribus_X11.txt \
             Scribus_Gnome.txt \
             Scribus_X11Grey.txt \
             Scribus_OpenOffice.txt \
             Scribus_SVG.txt 

install-data-local:
	$(mkinstalldirs) $(libdir)/scribus/
	$(mkinstalldirs) $(libdir)/scribus/swatches/
	$(INSTALL_DATA) $(srcdir)/Scribus_SVG.txt                    $(libdir)/scribus/swatches/Scribus_SVG.txt
	$(INSTALL_DATA) $(srcdir)/Scribus_X11Grey.txt           $(libdir)/scribus/swatches/Scribus_X11Grey.txt
	$(INSTALL_DATA) $(srcdir)/Scribus_Gnome.txt           $(libdir)/scribus/swatches/Scribus_Gnome.txt
	$(INSTALL_DATA) $(srcdir)/Scribus_X11.txt                $(libdir)/scribus/swatches/Scribus_X11.txt
	$(INSTALL_DATA) $(srcdir)/Scribus_OpenOffice.txt      $(libdir)/scribus/swatches/Scribus_OpenOffice.txt

uninstall-local:
	-rm -f $(libdir)/scribus/swatches/Scribus_X11.txt
	-rm -f $(libdir)/scribus/swatches/Scribus_Gnome.txt
	-rm -f $(libdir)/scribus/swatches/Scribus_X11Grey.txt
	-rm -f $(libdir)/scribus/swatches/Scribus_OpenOffice.txt
	-rm -f $(libdir)/scribus/swatches/Scribus_SVG.txt
