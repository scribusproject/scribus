SUBDIRS = tutorials 

EXTRA_DIST = cms2.html cms3.html cms4.html cms.html codingstandards.html cross-platform.html developers.html gettexthowto.html lipsum.html machints1.html machints2.html moncal.html plugin_howto.html readme-win32.html scribusfileformat.html scripter1.html scripterapi-page.html scripterapi-pydoc.html


install-data-local:
	$(mkinstalldirs) $(prefix)/share/doc/scribus-$(VERSION)/fr/
	$(INSTALL_DATA) $(srcdir)/cms.html $(prefix)/share/doc/scribus-$(VERSION)/fr/cms.html
	$(INSTALL_DATA) $(srcdir)/cms2.html $(prefix)/share/doc/scribus-$(VERSION)/fr/cms2.html
	$(INSTALL_DATA) $(srcdir)/cms3.html $(prefix)/share/doc/scribus-$(VERSION)/fr/cms3.html
	$(INSTALL_DATA) $(srcdir)/cms4.html $(prefix)/share/doc/scribus-$(VERSION)/fr/cms4.html
	$(INSTALL_DATA) $(srcdir)/codingstandards.html $(prefix)/share/doc/scribus-$(VERSION)/fr/codingstandards.html
	$(INSTALL_DATA) $(srcdir)/cross-platform.html $(prefix)/share/doc/scribus-$(VERSION)/fr/cross-platform.html
	$(INSTALL_DATA) $(srcdir)/developers.html $(prefix)/share/doc/scribus-$(VERSION)/fr/developers.html
	$(INSTALL_DATA) $(srcdir)/gettexthowto.html  $(prefix)/share/doc/scribus-$(VERSION)/fr/gettexthowto.html
	$(INSTALL_DATA) $(srcdir)/lipsum.html $(prefix)/share/doc/scribus-$(VERSION)/fr/lipsum.html
	$(INSTALL_DATA) $(srcdir)/machints1.html $(prefix)/share/doc/scribus-$(VERSION)/fr/machints1.html
	$(INSTALL_DATA) $(srcdir)/machints2.html $(prefix)/share/doc/scribus-$(VERSION)/fr/machints2.html
	$(INSTALL_DATA) $(srcdir)/moncal.html $(prefix)/share/doc/scribus-$(VERSION)/fr/moncal.html
	$(INSTALL_DATA) $(srcdir)/plugin_howto.html $(prefix)/share/doc/scribus-$(VERSION)/fr/plugin_howto.html
	$(INSTALL_DATA) $(srcdir)/readme-win32.html $(prefix)/share/doc/scribus-$(VERSION)/fr/readme-win32.html
	$(INSTALL_DATA) $(srcdir)/scribusfileformat.html $(prefix)/share/doc/scribus-$(VERSION)/fr/scribusfileformat.html
	$(INSTALL_DATA) $(srcdir)/scripter1.html $(prefix)/share/doc/scribus-$(VERSION)/fr/scripter1.html
	$(INSTALL_DATA) $(srcdir)/scripterapi-page.html $(prefix)/share/doc/scribus-$(VERSION)/fr/scripterapi-page.html
	$(INSTALL_DATA) $(srcdir)/scripterapi-pydoc.html $(prefix)/share/doc/scribus-$(VERSION)/fr/scripterapi-pydoc.html

uninstall-local:
	-rm -f $(prefix)/share/doc/scribus-$(VERSION)/fr/*.html