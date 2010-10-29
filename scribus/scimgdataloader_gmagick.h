/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCIMGDATALOADER_GMAGICK_H
#define SCIMGDATALOADER_GMAGICK_H

#include "scimgdataloader.h"
#include <magick/api.h>

class ScImgDataLoader_GMagick : public ScImgDataLoader
{
protected:
	void initSupportedFormatList();
	bool m_useRawImage;
private:
	static bool gm_initialized;
	bool readCMYK(Image *input, RawImage *output, int width, int height);
	bool readRGB(Image *input, QImage *output, int width, int height);

public:
	ScImgDataLoader_GMagick(void);

	virtual bool preloadAlphaChannel(const QString& fn, int page, int res, bool& hasAlpha);
	virtual void loadEmbeddedProfile(const QString& fn, int page = 0);
	virtual bool loadPicture(const QString& fn, int page, int res, bool thumbnail);
	virtual bool useRawImage() { return m_useRawImage; }
	virtual void initGraphicsMagick();
};

#endif

