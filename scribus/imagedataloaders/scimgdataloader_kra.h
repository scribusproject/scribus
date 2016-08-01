/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCIMGDATALOADER_KRA_H
#define SCIMGDATALOADER_KRA_H

#include "scimgdataloader.h"
#include "scpainter.h"
#include "third_party/zip/scribus_zip.h"
#include <QDomDocument>
#include <QDomElement>

class ScImgDataLoader_KRA : public ScImgDataLoader
{
public:
        ScImgDataLoader_KRA(void);

	virtual bool preloadAlphaChannel(const QString& fn, int page, int res, bool& hasAlpha);
	virtual void loadEmbeddedProfile(const QString& fn, int page = 0);
	virtual bool loadPicture(const QString& fn, int page, int res, bool thumbnail);
protected:
	void initSupportedFormatList();
};

#endif
