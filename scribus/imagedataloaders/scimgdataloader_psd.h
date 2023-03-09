/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCIMGDATALOADER_PSD_H
#define SCIMGDATALOADER_PSD_H

#include <QVector>
#include <QList>
#include "scimgdataloader.h"
#include "sccolor.h"

class ScImgDataLoader_PSD : public ScImgDataLoader
{
public:

	enum PSDColorMode
	{
		CM_BITMAP = 0,
		CM_GRAYSCALE = 1,
		CM_INDEXED = 2,
		CM_RGB = 3,
		CM_CMYK = 4,
		CM_MULTICHANNEL = 7,
		CM_DUOTONE = 8,
		CM_LABCOLOR = 9
	};

	ScImgDataLoader_PSD();

	bool preloadAlphaChannel(const QString& fn, int page, int res, bool& hasAlpha) override;
	void loadEmbeddedProfile(const QString& fn, int page = 0) override;
	bool loadPicture(const QString& fn, int page, int res, bool thumbnail) override;

	bool useRawImage() const  override { return true; }

protected:

	QList<unsigned int> m_colorTable;
	QList<ScColor> m_colorTableSc;
	int m_random_table[4096];

	void initSupportedFormatList();

	bool IsValid( const PSDHeader & header ) const;
	bool IsSupported( const PSDHeader & header ) const;

	bool LoadPSD(QDataStream& s, const PSDHeader& header);
	bool LoadPSDResources(QDataStream& s, const PSDHeader& header, qint64 dataOffset);
	bool LoadPSDImgData(QDataStream& s, const PSDHeader& header, qint64 dataOffset);
	bool loadChannel(QDataStream& s, const PSDHeader& header, QList<PSDLayer>& layerInfo, uint layer, int channel, int component, RawImage& tmpImg);
	bool loadLayerChannels(QDataStream& s, const PSDHeader& header, QList<PSDLayer>& layerInfo, uint layer, bool* firstLayer);
	bool loadLayer(QDataStream& s, const PSDHeader& header);
	bool parseLayer(QDataStream& s, const PSDHeader& header);
	QString getLayerString(QDataStream & s);
	void putDuotone(uchar *ptr, uchar cbyte);

	int m_maxChannels { 0 };
	QVector<int> m_curveTable1;
	QVector<int> m_curveTable2;
	QVector<int> m_curveTable3;
	QVector<int> m_curveTable4;
};

#endif
