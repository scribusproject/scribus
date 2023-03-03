/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCIMGDATALOADER_JPEGXL_H
#define SCIMGDATALOADER_JPEGXL_H

#include <QByteArray>
#include <QImage>
#include "scimgdataloader.h"

#include <jxl/decode.h>
#include <jxl/decode_cxx.h>

class ScImgDataLoader_JPEGXL : public ScImgDataLoader
{
public:
	ScImgDataLoader_JPEGXL();

	bool preloadAlphaChannel(const QString& fn, int page, int res, bool& hasAlpha) override;
	void loadEmbeddedProfile(const QString& fn, int page = 0) override;
	bool loadPicture(const QString& fn, int page, int res, bool thumbnail) override;

protected:
	void initSupportedFormatList();
	bool loadFile(const char* filename, std::vector<uint8_t>* out);
	bool decodeJpegXlOneShot(const uint8_t* jxl, size_t size,
							 std::vector<float>* pixels, size_t* xsize,
							 size_t* ysize, QByteArray* icc_profile);
	QImage tmpImage;
	JxlBasicInfo m_basicinfo;
	QImage::Format m_input_image_format;
//	QImage::Format m_target_image_format;

	JxlPixelFormat m_input_pixel_format;

};

#endif
