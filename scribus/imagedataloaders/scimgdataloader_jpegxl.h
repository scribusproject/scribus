/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

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

	struct JXLImageInfo
	{
		size_t width;
		size_t height;
		int channels { -1 };
		int bits_per_sample { -1 };
	};

	ScImgDataLoader_JPEGXL();

	bool preloadAlphaChannel(const QString& fn, int page, int res, bool& hasAlpha) override;
	void loadEmbeddedProfile(const QString& fn, int page = 0) override;
	bool loadPicture(const QString& fn, int page, int res, bool thumbnail) override;

protected:
	void initSupportedFormatList();
	bool loadFile(const char* filename, std::vector<uint8_t>* out);
	bool decodeJpegXlOneShot(const uint8_t* jxl, size_t size, std::vector<uint8_t>* pixels, JXLImageInfo *imageInfo, QByteArray* icc_profile);

	JxlBasicInfo m_basicinfo;

};

#endif
