#ifndef SCIMGDATALOADER_TIFF_H
#define SCIMGDATALOADER_TIFF_H

#include "scimgdataloader.h"

class ScImgDataLoader_TIFF : public ScImgDataLoader
{
protected:

	void initSupportedFormatList();

public:
	ScImgDataLoader_TIFF(void);

	virtual void preloadAlphaChannel(const QString& fn, int res);
	virtual void loadEmbeddedProfile(const QString& fn);
	virtual bool loadPicture(const QString& fn, int res, bool thumbnail);
};

#endif
