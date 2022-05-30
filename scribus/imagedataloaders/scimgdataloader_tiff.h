/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCIMGDATALOADER_TIFF_H
#define SCIMGDATALOADER_TIFF_H

#include <cstdint>
#include <tiffio.h>

#include <QList>

#include "scimgdataloader.h"

class ScImgDataLoader_TIFF : public ScImgDataLoader
{
public:
	ScImgDataLoader_TIFF();

//	virtual void preloadAlphaChannel(const QString& fn, int res);
	bool preloadAlphaChannel(const QString& fn, int page, int res, bool& hasAlpha) override;
	void loadEmbeddedProfile(const QString& fn, int page = 0) override;
	bool loadPicture(const QString& fn, int page, int res, bool thumbnail) override;

	bool useRawImage() const override { return true; }

protected:

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

	struct SampleFormatInfo
	{
		uint16_t photometric;
		uint16_t bitsPerSample;
		uint16_t samplesPerPixel;
		uint16_t samplesFormat;
		uint16_t fillOrder;
	};

	void initSupportedFormatList();
	int  getLayers(const QString& fn, int page);
	bool getImageData(TIFF* tif, RawImage *image, uint widtht, uint heightt, uint size, const SampleFormatInfo& sampleInfo, bool &bilevel, bool &isCMYK);
	bool getImageData_RGBA(TIFF* tif, RawImage *image, uint widtht, uint heightt, uint size, const SampleFormatInfo& sampleInfo);
	void blendOntoTarget(RawImage *tmp, int layOpa, const QString& layBlend, bool cmyk, bool useMask);
	QString getLayerString(QDataStream & s);
	bool loadChannel( QDataStream & s, const PSDHeader & header, QList<PSDLayer> &layerInfo, uint layer, int channel, int component, RawImage &tmpImg);
	bool loadLayerInfo(QDataStream & s, QList<PSDLayer> &layerInfo);
	bool loadLayerChannels( QDataStream & s, const PSDHeader & header, QList<PSDLayer> &layerInfo, uint layer, bool* firstLayer);

	bool testAlphaChannelAvailability(const QString& fn, int page, bool& hasAlpha);
	void unmultiplyRGBA(RawImage *image);

	int      m_random_table[4096];
	uint16_t m_photometric { PHOTOMETRIC_MINISBLACK };
	uint16_t m_samplesPerPixel { 1 };
};

#endif
