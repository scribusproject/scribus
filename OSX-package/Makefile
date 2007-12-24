DEPLIBRARIES=jpeg.pkg tiff.mpkg lcms.pkg freetype.pkg cairo.mpkg
ADDITIONAL=ghostscript.mpkg antiword.pkg aspell.pkg

PKGNAME=Scribus-135
DISTDIR=dist
INPUTDIR=inputs
INFOFILE=${INPUTDIR}/Info.plist
DESCFILE=${INPUTDIR}/Description.plist
RESOURCES=${INPUTDIR}/Resources

all: clean ${DEPLIBRARIES}
	@echo Packaging ${PKGNAME} from ${DEPLIBRARIES}
	@/Developer/Tools/packagemaker -build -p ${DISTDIR}/${PKGNAME}.mpkg -mi ${DISTDIR}/packages -i ${INFOFILE} -r ${RESOURCES} 

tmpdirs:
	mkdir -p ${DISTDIR}/packages

%.pkg: tmpdirs
	port -f -k pkg $*
	@cp -Rpn /opt/local/var/macports/build/*$**/work/*.pkg ${DISTDIR}/packages/

%.mpkg: tmpdirs
	port -f -k upgrade $*
	port -f -k -v mpkg $*
	@cp -Rpn /opt/local/var/macports/build/*$**/work/*.mpkg ${DISTDIR}/packages/

clean:
	rm -rf ${DISTDIR}/packages/*

