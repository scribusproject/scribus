#ifndef SCIMGDATALOADER_PS_H
#define SCIMGDATALOADER_PS_H

#include "scimgdataloader.h"

class ScImgDataLoader_PS : public ScImgDataLoader
{
protected:

	void initSupportedFormatList();

public:
	ScImgDataLoader_PS(void);

	virtual void preloadAlphaChannel(const QString& fn, int res);
	virtual void loadEmbeddedProfile(const QString& fn);
	virtual bool loadPicture(const QString& fn, int res, bool thumbnail);
};

#endif
