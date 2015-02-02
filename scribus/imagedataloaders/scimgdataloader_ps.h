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
	void scanForFonts(QString fn);
	bool parseData(QString fn);
	void loadPhotoshop(QString fn, int gsRes);
	void decodeA85(QByteArray &psdata, QString tmp);
	bool loadPSjpeg(QString fn);
	bool loadPSjpeg(QString fn, QImage &tmpImg);
	void loadPhotoshopBinary(QString fn);
	void loadPhotoshopBinary(QString fn, QImage &tmpImg);
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
	bool isDCS2multi;
	bool isPhotoshop;
	bool hasPhotoshopImageData;
	bool doThumbnail;
	bool hasThumbnail;
	bool inTrailer;
	bool BBoxInTrailer;
	bool isRotated;
	int psXSize;
	int psYSize;
	int psDepth;
	int psMode;
	int psChannel;
	int psBlock;
	int psDataType;
	QString psCommand;
	QMap<QString,ScColor> CustColors;
	QStringList FontListe;

public:
	ScImgDataLoader_PS(void);

//	virtual void preloadAlphaChannel(const QString& fn, int res);
	virtual bool preloadAlphaChannel(const QString& fn, int page, int res, bool& hasAlpha);
	virtual void loadEmbeddedProfile(const QString& fn, int page = 0);
	virtual bool loadPicture(const QString& fn, int page, int res, bool thumbnail);
};

#endif
