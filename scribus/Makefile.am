bin_PROGRAMS = scribus

scribus_SOURCES = scpreview.cpp lineformats.cpp multiline.cpp splash.cpp mergedoc.cpp bookpalette.cpp rc4.c md5.c cupsoptions.cpp guidemanager.cpp kimageeffect.cpp scribuswin.cpp hyphenator.cpp hyphen.c hyask.cpp hnjalloc.c hysettings.cpp seiten.cpp fpointarray.cpp fpoint.cpp annota.cpp buttonicon.cpp javadocs.cpp selfield.cpp editor.cpp page.cpp layers.cpp polyprops.cpp mpalette.cpp werktoolb.cpp keymanager.cpp navigator.cpp annot.cpp bookmwin.cpp cmsprefs.cpp customfdialog.cpp picsearch.cpp picstatus.cpp newtemp.cpp applytemplate.cpp muster.cpp scfonts_ttf.cpp scfonts_encoding.cpp scrap.cpp scfonts.cpp frameedit.cpp spalette.cpp edit1format.cpp editformats.cpp druck.cpp colorchart.cpp util.cpp cmykcolor.cpp cmykfw.cpp scribusXml.cpp linecombo.cpp linkbutton.cpp helpbrowser.cpp mspinbox.cpp modify.cpp movepage.cpp delpages.cpp inspage.cpp pdfopts.cpp tree.cpp missing.cpp fontprefs.cpp fmitem.cpp align.cpp serializer.cpp reformdoc.cpp docinfo.cpp mdup.cpp cpalette.cpp dcolor.cpp query.cpp colorm.cpp vruler.cpp hruler.cpp pageback.cpp pageitem.cpp newfile.cpp scribusview.cpp scribusdoc.cpp scribus.cpp main.cpp 
SOURCES = libpdf/pdflib.cpp libpostscript/pslib.cpp plugins/libchar/charselect.cpp libprefs/prefs.cpp libabout/about.cpp scpreview.cpp lineformats.cpp multiline.cpp splash.cpp mergedoc.cpp bookpalette.cpp rc4.c md5.c cupsoptions.cpp guidemanager.cpp kimageeffect.cpp scribuswin.cpp hyphenator.cpp hyphen.c hyask.cpp hnjalloc.c hysettings.cpp seiten.cpp fpointarray.cpp fpoint.cpp annota.cpp buttonicon.cpp javadocs.cpp selfield.cpp editor.cpp page.cpp layers.cpp polyprops.cpp mpalette.cpp werktoolb.cpp keymanager.cpp navigator.cpp annot.cpp bookmwin.cpp cmsprefs.cpp customfdialog.cpp picsearch.cpp picstatus.cpp newtemp.cpp applytemplate.cpp muster.cpp scfonts_ttf.cpp scfonts_encoding.cpp scrap.cpp scfonts.cpp frameedit.cpp spalette.cpp edit1format.cpp editformats.cpp druck.cpp colorchart.cpp util.cpp cmykcolor.cpp cmykfw.cpp scribusXml.cpp linecombo.cpp linkbutton.cpp helpbrowser.cpp mspinbox.cpp modify.cpp movepage.cpp delpages.cpp inspage.cpp pdfopts.cpp tree.cpp missing.cpp fontprefs.cpp fmitem.cpp align.cpp serializer.cpp reformdoc.cpp docinfo.cpp mdup.cpp cpalette.cpp dcolor.cpp query.cpp colorm.cpp vruler.cpp hruler.cpp pageback.cpp pageitem.cpp newfile.cpp scribusview.cpp scribusdoc.cpp scribus.cpp main.cpp 
INTERFACES = 
TRANSLATIONS = 

SUBDIRS = icons libabout libprefs plugins libpostscript libpdf profiles dicts po qm 

EXTRA_DIST = main.cpp scribus.cpp scribus.h scribusdoc.cpp scribusdoc.h scribusview.cpp scribusview.h newfile.h newfile.cpp pageitem.cpp pageitem.h pageback.cpp pageback.h hruler.cpp hruler.h vruler.cpp vruler.h colorm.cpp colorm.h query.cpp query.h dcolor.cpp dcolor.h cpalette.cpp cpalette.h mdup.cpp mdup.h docinfo.cpp docinfo.h reformdoc.cpp reformdoc.h serializer.cpp serializer.h align.cpp align.h fmitem.cpp fmitem.h fontprefs.cpp fontprefs.h missing.cpp missing.h tree.cpp tree.h pdfopts.cpp pdfopts.h inspage.cpp inspage.h delpages.cpp delpages.h movepage.cpp movepage.h modify.cpp modify.h mspinbox.cpp mspinbox.h helpbrowser.cpp helpbrowser.h icons5.h linkbutton.cpp linkbutton.h icons6.h linecombo.cpp linecombo.h scribusXml.cpp scribusXml.h cmykfw.cpp cmykfw.h cmykcolor.cpp cmykcolor.h util.cpp colorchart.cpp colorchart.h druck.cpp druck.h editformats.cpp editformats.h edit1format.cpp edit1format.h spalette.cpp spalette.h ISOenc.txt frameedit.cpp frameedit.h scfonts.cpp scfonts.h scrap.cpp scrap.h TTFenc.txt Stdenc.txt scfonts_encoding.cpp scfonts_encoding.h scfonts_ttf.cpp scfonts_ttf.h scfonts_ttftypes.h muster.cpp muster.h applytemplate.cpp applytemplate.h newtemp.cpp newtemp.h picstatus.cpp picstatus.h picsearch.cpp picsearch.h customfdialog.cpp customfdialog.h cmsprefs.cpp cmsprefs.h bookmwin.cpp bookmwin.h annot.cpp annot.h navigator.cpp navigator.h keymanager.cpp keymanager.h icc34.h werktoolb.cpp werktoolb.h mpalette.h mpalette.cpp polyprops.cpp polyprops.h layers.cpp layers.h page.cpp page.h editor.cpp editor.h selfield.cpp selfield.h javadocs.cpp javadocs.h buttonicon.cpp buttonicon.h annota.cpp annota.h fpoint.cpp fpoint.h fpointarray.cpp fpointarray.h seiten.cpp seiten.h hysettings.cpp hysettings.h hnjalloc.c hnjalloc.h hyask.cpp hyask.h hyphen.c hyphen.h hyphenator.cpp hyphenator.h scribuswin.cpp scribuswin.h kimageeffect.cpp kimageeffect.h guidemanager.cpp guidemanager.h cupsoptions.cpp cupsoptions.h md5.c md5.h rc4.c rc4.h mergedoc.cpp mergedoc.h bookpalette.cpp bookpalette.h iso8859-2.enc iso8859-13.enc iso8859-15.enc iso8859-2ps.enc iso8859-3ps.enc iso8859-4ps.enc iso8859-5ps.enc iso8859-7ps.enc iso8859-9ps.enc iso8859-10ps.enc iso8859-13ps.enc koi8ps.enc splash.h

install-data-local:
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/scribus.h $(prefix)/include/scribus/scribus.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/splash.h $(prefix)/include/scribus/splash.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/scribusdoc.h $(prefix)/include/scribus/scribusdoc.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/scribusview.h $(prefix)/include/scribus/scribusview.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/pageitem.h $(prefix)/include/scribus/pageitem.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/hruler.h $(prefix)/include/scribus/hruler.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/vruler.h $(prefix)/include/scribus/vruler.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/colorm.h $(prefix)/include/scribus/colorm.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/query.h $(prefix)/include/scribus/query.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/cpalette.h $(prefix)/include/scribus/cpalette.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/tree.h $(prefix)/include/scribus/tree.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/modify.h $(prefix)/include/scribus/modify.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/mspinbox.h $(prefix)/include/scribus/mspinbox.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/helpbrowser.h $(prefix)/include/scribus/helpbrowser.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/linkbutton.h $(prefix)/include/scribus/linkbutton.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/linecombo.h $(prefix)/include/scribus/linecombo.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/cmykcolor.h $(prefix)/include/scribus/cmykcolor.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/spalette.h $(prefix)/include/scribus/spalette.h
	$(mkinstalldirs) $(prefix)/lib/scribus/
	$(INSTALL_DATA) $(srcdir)/ISOenc.txt $(prefix)/lib/scribus/ISOenc.txt
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/frameedit.h $(prefix)/include/scribus/frameedit.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/scfonts.h $(prefix)/include/scribus/scfonts.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/scrap.h $(prefix)/include/scribus/scrap.h
	$(mkinstalldirs) $(prefix)/lib/scribus/
	$(INSTALL_DATA) $(srcdir)/TTFenc.txt $(prefix)/lib/scribus/TTFenc.txt
	$(mkinstalldirs) $(prefix)/lib/scribus/
	$(INSTALL_DATA) $(srcdir)/Stdenc.txt $(prefix)/lib/scribus/Stdenc.txt
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/scfonts_encoding.h $(prefix)/include/scribus/scfonts_encoding.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/muster.h $(prefix)/include/scribus/muster.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/customfdialog.h $(prefix)/include/scribus/customfdialog.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/bookmwin.h $(prefix)/include/scribus/bookmwin.h
	$(mkinstalldirs) $(prefix)/lib/scribus/
	$(INSTALL_DATA) $(srcdir)/iso8859-15.enc $(prefix)/lib/scribus/iso8859-15.enc
	$(mkinstalldirs) $(prefix)/lib/scribus/
	$(INSTALL_DATA) $(srcdir)/iso8859-2.enc $(prefix)/lib/scribus/iso8859-2.enc
	$(mkinstalldirs) $(prefix)/lib/scribus/
	$(INSTALL_DATA) $(srcdir)/iso8859-13.enc $(prefix)/lib/scribus/iso8859-13.enc
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/werktoolb.h $(prefix)/include/scribus/werktoolb.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/mpalette.h $(prefix)/include/scribus/mpalette.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/layers.h $(prefix)/include/scribus/layers.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/page.h $(prefix)/include/scribus/page.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/fpoint.h $(prefix)/include/scribus/fpoint.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/fpointarray.h $(prefix)/include/scribus/fpointarray.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/seiten.h $(prefix)/include/scribus/seiten.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/hyphen.h $(prefix)/include/scribus/hyphen.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/hyphenator.h $(prefix)/include/scribus/hyphenator.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/scribuswin.h $(prefix)/include/scribus/scribuswin.h
	$(mkinstalldirs) $(prefix)/include/scribus/
	$(INSTALL_DATA) $(srcdir)/bookpalette.h $(prefix)/include/scribus/bookpalette.h
	$(mkinstalldirs) $(prefix)/lib/scribus/
	$(INSTALL_DATA) $(srcdir)/iso8859-2ps.enc $(prefix)/lib/scribus/iso8859-2ps.enc
	$(mkinstalldirs) $(prefix)/lib/scribus/
	$(INSTALL_DATA) $(srcdir)/iso8859-3ps.enc $(prefix)/lib/scribus/iso8859-3ps.enc
	$(mkinstalldirs) $(prefix)/lib/scribus/
	$(INSTALL_DATA) $(srcdir)/iso8859-4ps.enc $(prefix)/lib/scribus/iso8859-4ps.enc
	$(mkinstalldirs) $(prefix)/lib/scribus/
	$(INSTALL_DATA) $(srcdir)/iso8859-5ps.enc $(prefix)/lib/scribus/iso8859-5ps.enc
	$(mkinstalldirs) $(prefix)/lib/scribus/
	$(INSTALL_DATA) $(srcdir)/iso8859-7ps.enc $(prefix)/lib/scribus/iso8859-7ps.enc
	$(mkinstalldirs) $(prefix)/lib/scribus/
	$(INSTALL_DATA) $(srcdir)/iso8859-9ps.enc $(prefix)/lib/scribus/iso8859-9ps.enc
	$(mkinstalldirs) $(prefix)/lib/scribus/
	$(INSTALL_DATA) $(srcdir)/iso8859-10ps.enc $(prefix)/lib/scribus/iso8859-10ps.enc
	$(mkinstalldirs) $(prefix)/lib/scribus/
	$(INSTALL_DATA) $(srcdir)/iso8859-13ps.enc $(prefix)/lib/scribus/iso8859-13ps.enc
	$(mkinstalldirs) $(prefix)/lib/scribus/
	$(INSTALL_DATA) $(srcdir)/koi8ps.enc $(prefix)/lib/scribus/koi8ps.enc

uninstall-local:
	-rm -f $(prefix)/include/scribus/scribus.h
	-rm -f $(prefix)/include/scribus/splash.h
	-rm -f $(prefix)/include/scribus/scribusdoc.h
	-rm -f $(prefix)/include/scribus/scribusview.h
	-rm -f $(prefix)/include/scribus/pageitem.h
	-rm -f $(prefix)/include/scribus/hruler.h
	-rm -f $(prefix)/include/scribus/vruler.h
	-rm -f $(prefix)/include/scribus/colorm.h
	-rm -f $(prefix)/include/scribus/query.h
	-rm -f $(prefix)/include/scribus/cpalette.h
	-rm -f $(prefix)/include/scribus/tree.h
	-rm -f $(prefix)/include/scribus/modify.h
	-rm -f $(prefix)/include/scribus/mspinbox.h
	-rm -f $(prefix)/include/scribus/helpbrowser.h
	-rm -f $(prefix)/include/scribus/linkbutton.h
	-rm -f $(prefix)/include/scribus/linecombo.h
	-rm -f $(prefix)/include/scribus/cmykcolor.h
	-rm -f $(prefix)/include/scribus/spalette.h
	-rm -f $(prefix)/lib/scribus/ISOenc.txt
	-rm -f $(prefix)/include/scribus/frameedit.h
	-rm -f $(prefix)/include/scribus/scfonts.h
	-rm -f $(prefix)/include/scribus/scrap.h
	-rm -f $(prefix)/lib/scribus/TTFenc.txt
	-rm -f $(prefix)/lib/scribus/Stdenc.txt
	-rm -f $(prefix)/include/scribus/scfonts_encoding.h
	-rm -f $(prefix)/include/scribus/muster.h
	-rm -f $(prefix)/include/scribus/customfdialog.h
	-rm -f $(prefix)/include/scribus/bookmwin.h
	-rm -f $(prefix)/lib/scribus/iso8859-15.enc
	-rm -f $(prefix)/lib/scribus/iso8859-2.enc
	-rm -f $(prefix)/lib/scribus/iso8859-13.enc
	-rm -f $(prefix)/include/scribus/werktoolb.h
	-rm -f $(prefix)/include/scribus/mpalette.h
	-rm -f $(prefix)/include/scribus/layers.h
	-rm -f $(prefix)/include/scribus/page.h
	-rm -f $(prefix)/include/scribus/fpoint.h
	-rm -f $(prefix)/include/scribus/fpointarray.h
	-rm -f $(prefix)/include/scribus/seiten.h
	-rm -f $(prefix)/include/scribus/hyphen.h
	-rm -f $(prefix)/include/scribus/hyphenator.h
	-rm -f $(prefix)/include/scribus/scribuswin.h
	-rm -f $(prefix)/include/scribus/bookpalette.h
	-rm -f $(prefix)/lib/scribus/iso8859-2ps.enc
	-rm -f $(prefix)/lib/scribus/iso8859-3ps.enc
	-rm -f $(prefix)/lib/scribus/iso8859-4ps.enc
	-rm -f $(prefix)/lib/scribus/iso8859-5ps.enc
	-rm -f $(prefix)/lib/scribus/iso8859-7ps.enc
	-rm -f $(prefix)/lib/scribus/iso8859-9ps.enc
	-rm -f $(prefix)/lib/scribus/iso8859-10ps.enc
	-rm -f $(prefix)/lib/scribus/iso8859-13ps.enc
	-rm -f $(prefix)/lib/scribus/koi8ps.enc

scribus_LDADD  = $(LIBFREETYPE_LIBS) $(LIBCUPS) $(LIBLCMS) $(LIBDL) $(LIBDL2) $(LIBTIFF) -lm $(LIB_QT) -lXext -lX11 $(LIBSOCKET)
AM_LDFLAGS =   -export-dynamic -s
# set the include path for X, qt and KDE
INCLUDES= $(LIBFREETYPE_CFLAGS) $(all_includes)

scribus_METASOURCES = AUTO

# the library search path.
scribus_LDFLAGS = $(all_libraries)

# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(scribus_METASOURCES)

# this is to enable Qt behavoir of am_edit
KDE_OPTIONS = qtonly



