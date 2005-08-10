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
		Foi_ttf(QString scname, QString path, bool embedps) :
		Foi(scname,path,embedps), metricsread(false)
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
