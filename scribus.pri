######################################################################
#
# Common settings used in all Scribus (sub)projects are placed here
#
######################################################################

DEFINES += \
           DOCDIR=\"doc/\" \
           ICONDIR=\"icons/\" \
           SAMPLESDIR=\"samples/\" \
           SCRIPTSDIR=\"scripts/\" \
           TEMPLATEDIR=\"templates/\" \
           LIBDIR=\"\" \
           PLUGINDIR=\"plugins/\" \
           SHAREDIR=\"\" \
           HAVE_LIBZ

FREETYPE_INCLUDE = $$[QT_INSTALL_PREFIX]/src/3rdparty/freetype/include
JPEG_INCLUDE = $$[QT_INSTALL_PREFIX]/src/3rdparty/libjpeg
ZLIB_INCLUDE = $$[QT_INSTALL_PREFIX]/src/3rdparty/zlib