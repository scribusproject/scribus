#ifndef SCFONTS_TTF_H
#define SCFONTS_TTF_H

#include "scfonts.h"

/*
	Class Foi_ttf
	Subclass of Foi, specifically for TrueType fonts.
	Implements: RealName() and EmbedFont().
*/

class Foi_ttf : public Foi
{
	public:
		Foi_ttf(QString scname, QString path, bool embedps) :
		Foi(scname,path,embedps), metricsread(false)
		{
			HasMetrics=true;
		}
		QString RealName();
		bool ReadMetrics();
		virtual bool EmbedFont(QString &str);
	private:
		bool metricsread;
};

#endif
