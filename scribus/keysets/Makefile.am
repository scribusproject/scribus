EXTRA_DIST = scribus13.ksxml

install-data-local:
	$(mkinstalldirs) $(libdir)/scribus/keysets/
	$(INSTALL_DATA) $(srcdir)/scribus13.ksxml $(libdir)/scribus/keysets/scribus13.ksxml

uninstall-local:
	-rm -f $(libdir)/scribus/keysets/scribus13.ksxml

AM_CPPFLAGS = \
    -DDOCDIR=\"$(datadir)/scribus/doc/\" \
    -DICONDIR=\"$(datadir)/scribus/icons/\" \
    -DSAMPLESDIR=\"$(datadir)/scribus/samples/\" \
    -DSCRIPTSDIR=\"$(datadir)/scribus/scripts/\" \
    -DTEMPLATEDIR=\"$(datadir)/scribus/templates/\" \
    -DLIBDIR=\"$(libdir)/scribus/\" \
    -DPLUGINDIR=\"$(libdir)/scribus/plugins/\"

AM_CXXFLAGS = \
    $(GCC_CXX_VISIBILITY_FLAGS)
