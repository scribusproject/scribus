/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCIMGDATALOADER_PGF_H
#define SCIMGDATALOADER_PGF_H

#include "scimgdataloader.h"

class ScImgDataLoader_PGF : public ScImgDataLoader
{
protected:

	void initSupportedFormatList();

public:
	ScImgDataLoader_PGF(void);

	virtual bool preloadAlphaChannel(const QString& fn, int page, int res, bool& hasAlpha);
	virtual void loadEmbeddedProfile(const QString& fn, int page = 0);
	virtual bool loadPicture(const QString& fn, int page, int res, bool thumbnail);
};

#endif
