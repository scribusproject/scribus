/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCIMGDATALOADER_PS_H
#define SCIMGDATALOADER_PS_H

#include "scimgdataloader.h"
#include "sccolor.h"

class ScImgDataLoader_PS : public ScImgDataLoader
{
protected:

	void initSupportedFormatList();
	bool parseData(QString fn);
	void loadDCS1(QString fn, int gsRes);
	void loadDCS2(QString fn, int gsRes);
	void blendImages(QImage &source, ScColor col);
	struct plateOffsets
	{
		uint pos;
		uint len;
	};
	QMap<QString, plateOffsets> colorPlates2;
	QMap<QString, QString> colorPlates;
	QString BBox;
	QString Creator;
	bool isDCS1;
	bool isDCS2;
	QMap<QString,ScColor> CustColors;

public:
	ScImgDataLoader_PS(void);

	virtual void preloadAlphaChannel(const QString& fn, int res);
	virtual void loadEmbeddedProfile(const QString& fn);
	virtual bool loadPicture(const QString& fn, int res, bool thumbnail);
};

#endif
