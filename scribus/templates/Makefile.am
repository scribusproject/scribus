EXTRA_DIST = br1/br1.png br1/br1tn.png br1/map.png br1/br1.sla nl2/flowers.jpg nl2/nl2.sla nl2/nl2tn.png nl2/nl2.png nl1/kites.png nl1/nl1.png nl1/nl1tn.png nl1/nl1.sla template.xml textbased/an_image.tif textbased/author.tif textbased/textbased.png textbased/textbased.sla.gz textbased/textbasedtn.png sc_presentation/presentation.png sc_presentation/presentation.sla.gz sc_presentation/presentationtn.png

install-data-local:
	$(mkinstalldirs) $(prefix)/share/scribus/templates/
	$(INSTALL_DATA) $(srcdir)/template.xml $(prefix)/share/scribus/templates/template.xml
	$(mkinstalldirs) $(prefix)/share/scribus/templates/nl1
	$(INSTALL_DATA) $(srcdir)/nl1/kites.png $(prefix)/share/scribus/templates/nl1/kites.png
	$(INSTALL_DATA) $(srcdir)/nl1/nl1.png $(prefix)/share/scribus/templates/nl1/nl1.png
	$(INSTALL_DATA) $(srcdir)/nl1/nl1tn.png $(prefix)/share/scribus/templates/nl1/nl1tn.png
	$(INSTALL_DATA) $(srcdir)/nl1/nl1.sla $(prefix)/share/scribus/templates/nl1/nl1.sla
	$(mkinstalldirs) $(prefix)/share/scribus/templates/nl2
	$(INSTALL_DATA) $(srcdir)/nl2/flowers.jpg $(prefix)/share/scribus/templates/nl2/flowers.jpg
	$(INSTALL_DATA) $(srcdir)/nl2/nl2.sla $(prefix)/share/scribus/templates/nl2/nl2.sla
	$(INSTALL_DATA) $(srcdir)/nl2/nl2tn.png $(prefix)/share/scribus/templates/nl2/nl2tn.png
	$(INSTALL_DATA) $(srcdir)/nl2/nl2.png $(prefix)/share/scribus/templates/nl2/nl2.png
	$(mkinstalldirs) $(prefix)/share/scribus/templates/br1
	$(INSTALL_DATA) $(srcdir)/br1/br1.png $(prefix)/share/scribus/templates/br1/br1.png
	$(INSTALL_DATA) $(srcdir)/br1/br1tn.png $(prefix)/share/scribus/templates/br1/br1tn.png
	$(INSTALL_DATA) $(srcdir)/br1/map.png $(prefix)/share/scribus/templates/br1/map.png
	$(INSTALL_DATA) $(srcdir)/br1/br1.sla $(prefix)/share/scribus/templates/br1/br1.sla
	$(mkinstalldirs) $(prefix)/share/scribus/templates/sc_presentation
	$(INSTALL_DATA) $(srcdir)/sc_presentation/presentation.png $(prefix)/share/scribus/templates/sc_presentation/presentation.png
	$(INSTALL_DATA) $(srcdir)/sc_presentation/presentationtn.png $(prefix)/share/scribus/templates/sc_presentation/presentationtn.png
	$(INSTALL_DATA) $(srcdir)/sc_presentation/presentation.sla.gz $(prefix)/share/scribus/templates/sc_presentation/presentation.sla.gz
	$(mkinstalldirs) $(prefix)/share/scribus/templates/textbased
	$(INSTALL_DATA) $(srcdir)/textbased/textbased.png $(prefix)/share/scribus/templates/textbased/textbased.png
	$(INSTALL_DATA) $(srcdir)/textbased/textbasedtn.png $(prefix)/share/scribus/templates/textbased/textbasedtn.png
	$(INSTALL_DATA) $(srcdir)/textbased/textbased.sla.gz $(prefix)/share/scribus/templates/textbased/textbased.sla.gz
	$(INSTALL_DATA) $(srcdir)/textbased/an_image.tif $(prefix)/share/scribus/templates/textbased/an_image.tif
	$(INSTALL_DATA) $(srcdir)/textbased/author.tif $(prefix)/share/scribus/templates/textbased/author.tif

uninstall-local:
	-rm -f $(prefix)/share/scribus/templates/nl1/*
	-rmdir $(prefix)/share/scribus/templates/nl1
	-rm -f $(prefix)/share/scribus/templates/nl2/*
	-rmdir $(prefix)/share/scribus/templates/nl2
	-rm -f $(prefix)/share/scribus/templates/br1/*
	-rmdir $(prefix)/share/scribus/templates/br1
	-rm -f $(prefix)/share/scribus/templates/sc_presentation/*
	-rmdir $(prefix)/share/scribus/templates/sc_presentation
	-rm -f $(prefix)/share/scribus/templates/textbased/*
	-rmdir $(prefix)/share/scribus/templates/textbased
	-rm -f $(prefix)/share/scribus/templates/template.xml
	-rmdir $(prefix)/share/scribus/templates

