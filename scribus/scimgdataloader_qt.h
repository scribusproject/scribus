#ifndef SCIMGDATALOADER_QT_H
#define SCIMGDATALOADER_QT_H

#include "scimgdataloader.h"

class ScImgDataLoader_QT : public ScImgDataLoader
{
protected:

	void initSupportedFormatList();

public:
	ScImgDataLoader_QT(void);

	virtual void preloadAlphaChannel(const QString& fn, int res);
	virtual void loadEmbeddedProfile(const QString& fn);
	virtual bool loadPicture(const QString& fn, int res, bool thumbnail);
};

#endif
