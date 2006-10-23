/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCIMGDATALOADER_TIFF_H
#define SCIMGDATALOADER_TIFF_H

#include <tiffio.h>
#include "scimgdataloader.h"

class ScImgDataLoader_TIFF : public ScImgDataLoader
{
protected:

	void initSupportedFormatList();
	int getLayers(const QString& fn);
	bool getImageData(TIFF* tif, QImage *image, uint widtht, uint heightt, uint size, uint16 photometric, uint16 bitspersample, uint16 samplesperpixel, bool &bilevel, bool &isCMYK);
	void blendOntoTarget(QImage *tmp, int layOpa, QString layBlend, bool cmyk);
	QString getLayerString(QDataStream & s);
	int random_table[4096];

public:
	ScImgDataLoader_TIFF(void);

	virtual void preloadAlphaChannel(const QString& fn, int res);
	virtual void loadEmbeddedProfile(const QString& fn);
	virtual bool loadPicture(const QString& fn, int res, bool thumbnail);
};

#endif
