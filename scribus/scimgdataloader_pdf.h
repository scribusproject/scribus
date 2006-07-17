#ifndef SCIMGDATALOADER_PDF_H
#define SCIMGDATALOADER_PDF_H

#include "scimgdataloader.h"

class ScImgDataLoader_PDF : public ScImgDataLoader
{
protected:

	void initSupportedFormatList();

public:
	ScImgDataLoader_PDF(void);

	virtual void preloadAlphaChannel(const QString& fn, int res);
	virtual void loadEmbeddedProfile(const QString& fn);
	virtual bool loadPicture(const QString& fn, int res, bool thumbnail);
};

#endif
