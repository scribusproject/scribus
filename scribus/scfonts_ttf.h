/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCFONTS_TTF_H
#define SCFONTS_TTF_H

#include "scribusapi.h"
#include "scfonts.h"

/*
	Class Foi_ttf
	Subclass of Foi, specifically for TrueType fonts.
	Implements: RealName() and EmbedFont().
*/

class SCRIBUS_API Foi_ttf : public Foi
{
	public:
		Foi_ttf(QString fam, QString sty, QString alt, QString psname, QString path, int face, bool embedps) :
		Foi(fam,sty,alt,psname,path,face,embedps), metricsread(false)
		{
			HasMetrics=true;
			formatCode = SFNT;
		}
		QString RealName();
		bool ReadMetrics();
		virtual bool EmbedFont(QString &str);
		virtual void RawData(QByteArray & bb);
	private:
		bool metricsread;
};

#endif
