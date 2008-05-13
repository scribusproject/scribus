/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "scimagestructs.h"

ExifValues::ExifValues(void)
{
	init();
}

void ExifValues::init(void)
{
	width = 0;
	height = 0;
	ExposureTime = 0;
	ApertureFNumber = 0;
	ISOequivalent = 0;
	cameraName.resize(0);
	cameraVendor.resize(0);
	comment.resize(0);
	userComment.resize(0);
	artist.resize(0);
	copyright.resize(0);
	dateTime.resize(0);
	thumbnail = QImage();
}

ImageInfoRecord::ImageInfoRecord(void)
{
	init();
}

void ImageInfoRecord::init(void)
{
	type = ImageTypeOther;	/* 0 = jpg, 1 = tiff, 2 = psd, 3 = eps/ps, 4 = pdf, 5 = jpg2000, 6 = other */
	xres = 72;
	yres = 72;
	BBoxX = 0;
	BBoxH = 0;
	colorspace = ColorSpaceRGB; /* 0 = RGB  1 = CMYK  2 = Grayscale 3 = Duotone */
	valid = false;
	isRequest = false;
	progressive = false;
	isEmbedded = false;
	exifDataValid = false;
	lowResType = 1; /* 0 = full Resolution, 1 = 72 dpi, 2 = 36 dpi */
	lowResScale = 1.0;
	PDSpathData.clear();
	RequestProps.clear();
	numberOfPages = 1;
	actualPageNumber = 0; // default
	clipPath.resize(0);
	usedPath.resize(0);
	profileName.resize(0);
	layerInfo.clear();
	duotoneColors.clear();
	exifInfo.init();
}
