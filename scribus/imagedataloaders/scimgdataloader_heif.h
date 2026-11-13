/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef SCIMGDATALOADER_HEIF_H
#define SCIMGDATALOADER_HEIF_H

#include <QString>

#include "scimgdataloader.h"

#include <libheif/heif.h>

/**
 * Loads HEIF images (.heic, .heif, .hif) with libheif. The primary image of the
 * file is loaded, with its rotation, mirroring and crop applied, along with its
 * alpha channel and colour profile.
 */
class ScImgDataLoader_HEIF : public ScImgDataLoader
{

public:
	ScImgDataLoader_HEIF();

	bool preloadAlphaChannel(const QString& fn, int page, int res, bool& hasAlpha) override;
	void loadEmbeddedProfile(const QString& fn, int page = 0) override;
	bool loadPicture(const QString& fn, int page, int res, bool thumbnail) override;

private:
	class HeifFile;
	class HeifLibrary;

	void initSupportedFormatList();
	/// Decodes the image into m_image as ARGB32. On failure, \a error holds the reason.
	bool decodeImage(heif_image_handle* handle, QString& error);
	/// Reads the image's colour profile into m_embeddedProfile and m_profileComponents.
	void readProfile(heif_image_handle* handle);
	/// Returns true for a greyscale image.
	bool isMonochrome(heif_image_handle* handle) const;
};

#endif
