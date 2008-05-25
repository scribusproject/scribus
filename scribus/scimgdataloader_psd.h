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

	ScImgDataLoader_PSD(void);

//	virtual void preloadAlphaChannel(const QString& fn, int res);
	virtual bool preloadAlphaChannel(const QString& fn, int page, int res, bool& hasAlpha);
	virtual void loadEmbeddedProfile(const QString& fn, int page = 0);
	virtual bool loadPicture(const QString& fn, int page, int res, bool thumbnail);

protected:

	QList<unsigned int> colorTable;
	QList<ScColor> colorTableSc;
	int random_table[4096];

	void initSupportedFormatList();

	bool IsValid( const PSDHeader & header );
	bool IsSupported( const PSDHeader & header );

	bool LoadPSD( QDataStream & s, const PSDHeader & header);
	bool LoadPSDResources( QDataStream & s, const PSDHeader & header, uint dataOffset );
	bool LoadPSDImgData( QDataStream & s, const PSDHeader & header, uint dataOffset );
	bool loadChannel( QDataStream & s, const PSDHeader & header, QList<PSDLayer> &layerInfo, uint layer, int channel, int component, RawImage &tmpImg);
	bool loadLayerChannels( QDataStream & s, const PSDHeader & header, QList<PSDLayer> &layerInfo, uint layer, bool* firstLayer);
	bool loadLayer( QDataStream & s, const PSDHeader & header);
	bool parseLayer( QDataStream & s, const PSDHeader & header);
	QString getLayerString(QDataStream & s);
	void putDuotone(uchar *ptr, uchar cbyte);
	int maxChannels;
	QVector<int> curveTable1;
	QVector<int> curveTable2;
	QVector<int> curveTable3;
	QVector<int> curveTable4;
};

#endif
