/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCIMGDATALOADER_GIMP_H
#define SCIMGDATALOADER_GIMP_H

#include "scimgdataloader.h"

class ScImgDataLoader_GIMP : public ScImgDataLoader
{
protected:

	void initSupportedFormatList();

public:
	ScImgDataLoader_GIMP(void);

//	virtual void preloadAlphaChannel(const QString& fn, int res);
	virtual bool preloadAlphaChannel(const QString& fn, int res, bool& hasAlpha);
	virtual void loadEmbeddedProfile(const QString& fn);
	virtual bool loadPicture(const QString& fn, int res, bool thumbnail);
};

#endif
