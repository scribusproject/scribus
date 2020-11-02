/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */
#ifndef SCFACE_PS_H
#define SCFACE_PS_H

#include <QString>
#include <QStringList>
#include <QFont>
#include <QMap>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_GLYPH_H

#include "scribusapi.h"
#include "fpointarray.h"
#include "scconfig.h"


/*
	Class ScFace_postscript
	Subclass of ScFace, for PostScript fonts that could possibly have a .afm file
	associated with them for metrics information.
*/

class ScFace_PostScript : public FtFace
{
	public:
		ScFace_PostScript(const QString& fam, const QString& sty, const QString& alt, const QString& scname, const QString& psname, const QString& path, int face, const QStringList& features);
		
		void load() const override;

		virtual QStringList findFontMetrics(const QString& fontPath) const;
		virtual QStringList findFontMetrics(const QString& baseDir, const QString& baseName) const;
		virtual bool loadFontMetrics(FT_Face face, const QString& fontPath) const;
};

/*
	Class ScFace_pfb
	Subclass of ScFace, specifically for Adobe type 1 .pfb fonts.
	Implements: realName() and embedFont().
*/

class ScFace_PFB : public ScFace_PostScript
{
	public:
		ScFace_PFB(const QString& fam, const QString& sty, const QString& alt, const QString& scname, const QString& psname, const QString& path, int face, const QStringList& features);

		bool embedFont(QByteArray &str) const override;
};

/*
	Class ScFace_pfa
	Subclass of ScFace, specifically for Adobe type 1 and type 3 .pfa fonts.
	Implements: realName() and embedFont().
*/

class ScFace_PFA : public ScFace_PostScript
{
	public:
		ScFace_PFA(const QString& fam, const QString& sty, const QString& alt, const QString& scname, const QString& psname, const QString& path, int face, const QStringList& features);

		bool embedFont(QByteArray &str) const override;
};


#endif
