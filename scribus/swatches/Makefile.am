EXTRA_DIST = rgbsribus.txt \
             rgbscribusgnome.txt \
             rgbscribusgreys.txt \
             rgbscribusopenoffice.txt \
             rgbsvg.txt 

install-data-local:
	$(mkinstalldirs) $(libdir)/scribus/
	$(mkinstalldirs) $(libdir)/scribus/swatches/
	$(INSTALL_DATA) $(srcdir)/rgbsvg.txt                    $(libdir)/scribus/swatches/rgbsvg.txt
	$(INSTALL_DATA) $(srcdir)/rgbscribusgreys.txt           $(libdir)/scribus/swatches/rgbscribusgreys.txt
	$(INSTALL_DATA) $(srcdir)/rgbscribusgnome.txt           $(libdir)/scribus/swatches/rgbscribusgnome.txt
	$(INSTALL_DATA) $(srcdir)/rgbscribus.txt                $(libdir)/scribus/swatches/rgbscribus.txt
	$(INSTALL_DATA) $(srcdir)/rgbscribusopenoffice.txt      $(libdir)/scribus/swatches/rgbscribusopenoffice.txt

uninstall-local:
	-rm -f $(libdir)/scribus/swatches/rgbscribus.txt
	-rm -f $(libdir)/scribus/swatches/rgbscribusgnome.txt
	-rm -f $(libdir)/scribus/swatches/rgbscribusgreys.txt
	-rm -f $(libdir)/scribus/swatches/rgbscribusopenoffice.txt
	-rm -f $(libdir)/scribus/swatches/rgbsvg.txt
