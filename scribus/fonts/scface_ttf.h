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
	Implements: realName() and embedFont().
*/

class SCRIBUS_API ScFace_ttf : public FtFace
{
public:
	ScFace_ttf (const QString& fam, const QString& sty, const QString& alt, const QString& scname, const QString& psname, const QString& path, int face, const QStringList& features);
	~ScFace_ttf();

	void load () const override;
	void unload () const override;

	bool embedFont(QByteArray &str) const override;
	void rawData(QByteArray & bb) const override;
	
	bool glyphNames(ScFace::FaceEncoding& glyphList) const override;
	bool hasNames() const override;
	bool isSymbolic() const override;

private:
	mutable sfnt::PostTable m_checkPost;

};

#endif
