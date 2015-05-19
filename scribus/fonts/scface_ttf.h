/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCFACE_TTF_H
#define SCFACE_TTF_H

#include "scribusapi.h"
#include "fonts/ftface.h"
#include "fonts/sfnt.h"


/*
	Class ScFace_ttf
	Subclass of ScFace, specifically for TrueType fonts.
	Implements: RealName() and EmbedFont().
*/

class SCRIBUS_API ScFace_ttf : public FtFace
{
public:
	ScFace_ttf ( QString fam, QString sty, QString alt, QString scname, QString psname, QString path, int face );
	~ScFace_ttf();

	void load () const;
	void unload () const;

	bool EmbedFont(QByteArray &str) const;
	void RawData(QByteArray & bb) const;

	qreal glyphKerning ( ScFace::gid_type gl1, ScFace::gid_type gl2, qreal sz ) const;
	
	virtual bool glyphNames(ScFace::FaceEncoding& GList) const;
	virtual bool hasNames() const;
	virtual bool isSymbolic() const;

private:
	mutable KernFeature * kernFeature;
    mutable sfnt::PostTable checkPost;

};

#endif
