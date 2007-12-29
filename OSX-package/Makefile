# currently a handpicked list of libraries Scribus depends on
DEPLIBRARIES=zlib.pkg jpeg.pkg tiff.pkg lcms.pkg freetype.pkg cairo.pkg
# these wil be optional packages later
ADDITIONAL=ghostscript.mpkg antiword.pkg aspell.pkg
# this MUST be the installed path of the Scribus.app bundle
SCRIBUSAPP=/usr/local/Applications/Scribus135.app
# How to name the package
PKGNAME=Scribus-135

# no need to change those:
DISTDIR=dist
INPUTDIR=inputs
INFOFILE=${INPUTDIR}/Info.plist
SCRIBUSINFOFILE=${INPUTDIR}/Scribus.pkg-Info.plist
SCRIBUSPKG=${INPUTDIR}/ScribusPkg
DESCFILE=${INPUTDIR}/Description.plist
RESOURCES=${INPUTDIR}/Resources

all: ${DEPLIBRARIES} Scribus.pkg
	@echo Packaging ${PKGNAME} with ${DEPLIBRARIES}
	@/Developer/Tools/packagemaker -build -p ${DISTDIR}/${PKGNAME}.mpkg -mi ${DISTDIR}/packages -i ${INFOFILE} -r ${RESOURCES} 

tmpdirs:
	mkdir -p ${DISTDIR}/packages

%.pkg: tmpdirs
	port -f -k pkg $*
	@cp -Rpn /opt/local/var/macports/build/*$**/work/*.pkg ${DISTDIR}/packages/

cairo.pkg: fontconfig.pkg expat.pkg libiconv.pkg libpng.pkg
	port -f -k pkg cairo +no-x11
	@cp -Rpn /opt/local/var/macports/build/*cairo*/work/*.pkg ${DISTDIR}/packages/

%.mpkg: tmpdirs
	#port -f -k upgrade $*
	port -f -k -v mpkg $*
	@cp -Rpn /opt/local/var/macports/build/*$**/work/*.mpkg ${DISTDIR}/packages/

Scribus.pkg: tmpdirs
	@mkdir -p ${SCRIBUSPKG}/Applications
	@(cd ${SCRIBUSPKG}/Applications; mv ${SCRIBUSAPP} .)
	/Developer/Tools/packagemaker -build -p ${DISTDIR}/packages/Scribus.pkg -f ${SCRIBUSPKG} -i ${SCRIBUSINFOFILE} 
	@(cd ${SCRIBUSPKG}/Applications ; mv Scribus*.app ${SCRIBUSAPP})



clean:
	rm -rf ${DISTDIR}/packages/*

