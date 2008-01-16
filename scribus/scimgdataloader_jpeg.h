/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCIMGDATALOADER_JPEG_H
#define SCIMGDATALOADER_JPEG_H

#include "scimgdataloader.h"

extern "C"
{
#define XMD_H           // shut JPEGlib up
#if defined(Q_OS_UNIXWARE)
#  define HAVE_BOOLEAN  // libjpeg under Unixware seems to need this
#endif
#include <jpeglib.h>
#include <jerror.h>
#undef HAVE_STDLIB_H
#ifdef const
#  undef const          // remove crazy C hackery in jconfig.h
#endif
}

class ScImgDataLoader_JPEG : public ScImgDataLoader
{
protected:

	void initSupportedFormatList();

	bool marker_is_icc (jpeg_saved_marker_ptr marker);
	bool marker_is_photoshop (jpeg_saved_marker_ptr marker);
	bool read_jpeg_marker (UINT8 requestmarker, j_decompress_ptr cinfo, JOCTET **icc_data_ptr, unsigned int *icc_data_len);

public:
	ScImgDataLoader_JPEG(void);

//	virtual void preloadAlphaChannel(const QString& fn, int res);
	virtual bool preloadAlphaChannel(const QString& fn, int res, bool& hasAlpha);
	virtual void loadEmbeddedProfile(const QString& fn);
	virtual bool loadPicture(const QString& fn, int res, bool thumbnail);
};

#endif
