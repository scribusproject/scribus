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
public:
	ScImgDataLoader_PS();

	void initialize() override;

	bool preloadAlphaChannel(const QString& fn, int page, int res, bool& hasAlpha) override;
	void loadEmbeddedProfile(const QString& fn, int page = 0) override;
	bool loadPicture(const QString& fn, int page, int res, bool thumbnail) override;

protected:
	void initSupportedFormatList();
	void scanForFonts(const QString& fn);
	bool parseData(const QString& fn);
	void loadPhotoshop(const QString& fn, int gsRes);
	void decodeA85(QByteArray &psdata, const QString& tmp);
	bool loadPSjpeg(const QString& fn);
	bool loadPSjpeg(const QString& fn, QImage &tmpImg);
	void loadPhotoshopBinary(const QString& fn);
	void loadPhotoshopBinary(const QString& fn, QImage &tmpImg);
	void loadDCS1(const QString& fn, int gsRes);
	void loadDCS2(const QString& fn, int gsRes);
	void blendImages(QImage &source, const ScColor& col);
	struct plateOffsets
	{
		uint pos;
		uint len;
	};
	QMap<QString, plateOffsets> m_colorPlates2;
	QMap<QString, QString> m_colorPlates;
	QString m_BBox;
	QString m_Creator;
	bool m_isDCS1 { false };
	bool m_isDCS2 { false };
	bool m_isDCS2multi { false };
	bool m_isPhotoshop { false };
	bool m_hasPhotoshopImageData { false };
	bool m_doThumbnail { false };
	bool m_hasThumbnail { false };
	bool m_inTrailer { false };
	bool m_BBoxInTrailer { false };
	bool m_isRotated { false };
	int m_psXSize { 0 };
	int m_psYSize { 0 };
	int m_psDepth { 0 };
	int m_psMode { 0 };
	int m_psChannel { 0 };
	int m_psBlock { 0 };
	int m_psDataType { 0 };
	QString m_psCommand;
	QMap<QString,ScColor> m_CustColors;
	QStringList m_fontList;
};

#endif
