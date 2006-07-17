#ifndef SCIMGDATALOADER_PSD_H
#define SCIMGDATALOADER_PSD_H

#include <qvaluelist.h>
#include "scimgdataloader.h"

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

	virtual void preloadAlphaChannel(const QString& fn, int res);
	virtual void loadEmbeddedProfile(const QString& fn);
	virtual bool loadPicture(const QString& fn, int res, bool thumbnail);

protected:

	QValueList<unsigned int> colorTable;
	int random_table[4096];

	void initSupportedFormatList();

	bool IsValid( const PSDHeader & header );
	bool IsSupported( const PSDHeader & header );

	bool LoadPSD( QDataStream & s, const PSDHeader & header);
	bool loadChannel( QDataStream & s, const PSDHeader & header, QValueList<PSDLayer> &layerInfo, uint layer, int channel, int component, QImage &tmpImg);
	bool loadLayerChannels( QDataStream & s, const PSDHeader & header, QValueList<PSDLayer> &layerInfo, uint layer, bool* firstLayer);
	bool loadLayer( QDataStream & s, const PSDHeader & header);
	bool parseLayer( QDataStream & s, const PSDHeader & header);
	QString getLayerString(QDataStream & s);
};

#endif
