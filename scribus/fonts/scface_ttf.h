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

/*
	Class ScFace_ttf
	Subclass of ScFace, specifically for TrueType fonts.
	Implements: RealName() and EmbedFont().
*/

class SCRIBUS_API ScFace_ttf : public FtFace
{
public:
	ScFace_ttf(QString fam, QString sty, QString alt, QString scname, QString psname, QString path, int face) 
	: FtFace(fam, sty, alt, scname, psname, path, face )
	{
		formatCode = ScFace::SFNT;
	}
	bool EmbedFont(QString &str) const;
	void RawData(QByteArray & bb) const;
};

#endif
