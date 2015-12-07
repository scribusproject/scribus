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
	QMap<QString, plateOffsets> m_colorPlates2;
	QMap<QString, QString> m_colorPlates;
	QString m_BBox;
	QString m_Creator;
	bool m_isDCS1;
	bool m_isDCS2;
	bool m_isDCS2multi;
	bool m_isPhotoshop;
	bool m_hasPhotoshopImageData;
	bool m_doThumbnail;
	bool m_hasThumbnail;
	bool m_inTrailer;
	bool m_BBoxInTrailer;
	bool m_isRotated;
	int m_psXSize;
	int m_psYSize;
	int m_psDepth;
	int m_psMode;
	int m_psChannel;
	int m_psBlock;
	int m_psDataType;
	QString m_psCommand;
	QMap<QString,ScColor> m_CustColors;
	QStringList m_FontListe;

public:
	ScImgDataLoader_PS(void);

	virtual void initialize(void);

//	virtual void preloadAlphaChannel(const QString& fn, int res);
	virtual bool preloadAlphaChannel(const QString& fn, int page, int res, bool& hasAlpha);
	virtual void loadEmbeddedProfile(const QString& fn, int page = 0);
	virtual bool loadPicture(const QString& fn, int page, int res, bool thumbnail);
};

#endif
