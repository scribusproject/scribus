/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <qfile.h>
#include <qfileinfo.h>
#include <qobject.h>
#include "scconfig.h"
#include "scimgdataloader_tiff.h"
#include <tiffio.h>
#include CMS_INC

ScImgDataLoader_TIFF::ScImgDataLoader_TIFF(void) : ScImgDataLoader()
{
	initSupportedFormatList();
}

void ScImgDataLoader_TIFF::initSupportedFormatList(void)
{
	m_supportedFormats.clear();
	m_supportedFormats.append( "tif" );
	m_supportedFormats.append( "tiff" );
}

void ScImgDataLoader_TIFF::loadEmbeddedProfile(const QString& fn)
{
	m_embeddedProfile.resize(0);
	m_profileComponents = 0;
	if ( !QFile::exists(fn) )
		return;
	TIFF* tif = TIFFOpen(fn.local8Bit(), "r");
	if(tif)
	{
		DWORD EmbedLen = 0;
		LPBYTE EmbedBuffer;
		if (TIFFGetField(tif, TIFFTAG_ICCPROFILE, &EmbedLen, &EmbedBuffer))
		{
			cmsHPROFILE tiffProf = cmsOpenProfileFromMem(EmbedBuffer, EmbedLen);
			if (tiffProf)
			{
				if (static_cast<int>(cmsGetColorSpace(tiffProf)) == icSigRgbData)
					m_profileComponents = 3;
				if (static_cast<int>(cmsGetColorSpace(tiffProf)) == icSigCmykData)
					m_profileComponents = 4;
				m_embeddedProfile.duplicate((const char*) EmbedBuffer, EmbedLen);
			}
			cmsCloseProfile(tiffProf);
		}
		TIFFClose(tif);
	}
}

void ScImgDataLoader_TIFF::preloadAlphaChannel(const QString& fn, int res)
{
	float xres, yres;
	short resolutionunit = 0;
	initialize();
	QFileInfo fi = QFileInfo(fn);
	if (!fi.exists())
		return;
	QString tmp, BBox, tmp2;
	TIFF* tif = TIFFOpen(fn.local8Bit(), "r");
	if(tif)
	{
		unsigned width, height, size;
		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
		TIFFGetField(tif, TIFFTAG_XRESOLUTION, &xres);
		TIFFGetField(tif, TIFFTAG_YRESOLUTION, &yres);
		TIFFGetField(tif, TIFFTAG_RESOLUTIONUNIT , &resolutionunit);
		size = width * height;
		uint16 photometric, bitspersample, samplesperpixel, fillorder;
		TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
		TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
		TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
		TIFFGetField(tif, TIFFTAG_FILLORDER, &fillorder);
		uint32 *bits = 0;
		if (photometric == PHOTOMETRIC_SEPARATED)
		{
			TIFFClose(tif);
			return;
		}
		else
		{
			m_image.create(width,height,32);
			m_image.setAlphaBuffer(true);
			bits = (uint32 *) _TIFFmalloc(size * sizeof(uint32));
			if(bits)
			{
				if (TIFFReadRGBAImage(tif, width, height, bits, 0))
				{
					for(unsigned int y = 0; y < height; y++)
						memcpy(m_image.scanLine(height - 1 - y), bits + y * width, width * 4);
				}
				_TIFFfree(bits);
			}
			else
				m_image.reset();
			TIFFClose(tif);
		}
	}
}

bool ScImgDataLoader_TIFF::loadPicture(const QString& fn, int /*gsRes*/, bool /*thumbnail*/)
{
	bool bilevel = false;
	short resolutionunit = 0;
	float xres = 72.0, yres = 72.0;
	if (!QFile::exists(fn))
		return false;

	initialize();
	m_imageInfoRecord.type = 1;
	TIFF* tif = TIFFOpen(fn.local8Bit(), "r");
	if(tif)
	{
		bool isCMYK = false;
		unsigned widtht, heightt, size;
		char *description=0, *copyright=0, *datetime=0, *artist=0, *scannerMake=0, *scannerModel=0;

		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &widtht);
		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &heightt);
		TIFFGetField(tif, TIFFTAG_XRESOLUTION, &xres);
		TIFFGetField(tif, TIFFTAG_YRESOLUTION, &yres);
		TIFFGetField(tif, TIFFTAG_RESOLUTIONUNIT , &resolutionunit);
		size = widtht * heightt;
		uint16 photometric, bitspersample, samplesperpixel, fillorder;
		TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
		TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
		TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
		TIFFGetField(tif, TIFFTAG_FILLORDER, &fillorder);

		TIFFGetField(tif, TIFFTAG_MAKE, &scannerMake);
		TIFFGetField(tif, TIFFTAG_MODEL, &scannerModel);
		TIFFGetField(tif, TIFFTAG_IMAGEDESCRIPTION, &description);
		TIFFGetField(tif, TIFFTAG_COPYRIGHT, &copyright);
		TIFFGetField(tif, TIFFTAG_DATETIME, &datetime);
		TIFFGetField(tif, TIFFTAG_ARTIST, &artist);
		m_imageInfoRecord.exifInfo.cameraName = QString(scannerModel);
		m_imageInfoRecord.exifInfo.cameraVendor = QString(scannerMake);
		m_imageInfoRecord.exifInfo.comment = QString(description);
		m_imageInfoRecord.exifInfo.userComment = QString(copyright);
		m_imageInfoRecord.exifInfo.width = widtht;
		m_imageInfoRecord.exifInfo.height = heightt;
		m_imageInfoRecord.exifInfo.dateTime = QString(datetime);
		m_imageInfoRecord.exifInfo.artist = QString(artist);
		m_imageInfoRecord.exifInfo.thumbnail = QImage();
		m_imageInfoRecord.exifDataValid = true;

		if( xres <= 1.0 || yres <= 1.0 )
		{
			xres = yres = 72.0;
			QFileInfo qfi(fn);
			m_message = QObject::tr("%1 may be corrupted : missing resolution tags").arg(qfi.fileName());
			m_msgType = warningMsg;
		}

		if (!m_image.create(widtht,heightt,32))
		{
			TIFFClose(tif);
			return false;
		}
		m_image.setAlphaBuffer(true);
		uint32 *bits = 0;
		if (photometric == PHOTOMETRIC_SEPARATED)
		{
			if (TIFFIsTiled(tif))
			{
				uint32 columns, rows;
				uint32 *tile_buf;
				uint32 xt, yt;
				TIFFGetField(tif, TIFFTAG_TILEWIDTH,  &columns);
				TIFFGetField(tif, TIFFTAG_TILELENGTH, &rows);
				tile_buf = (uint32*) _TIFFmalloc(columns*rows*sizeof(uint32));
				if (tile_buf == NULL)
				{
					TIFFClose(tif);
					return false;
				}
				uint32 tileW = columns, tileH = rows;
				for (yt = 0; yt < (uint32) m_image.height(); yt += rows)
				{
					if (yt > (uint) m_image.height())
						break;
					if (m_image.height()-yt < rows)
						tileH = m_image.height()-yt;
					tileW = columns;
					register uint32 yi;
					for (xt = 0; xt < (uint) m_image.width(); xt += columns)
					{
						TIFFReadTile(tif, tile_buf, xt, yt, 0, 0);
						for (yi = 0; yi < tileH; yi++)
							_TIFFmemcpy(m_image.scanLine(yt+(tileH-1-yi))+xt, tile_buf+tileW*yi, tileW*4);
					}
				}
				_TIFFfree(tile_buf);
			}
			else
			{
				tsize_t bytesperrow = TIFFScanlineSize(tif);
				bits = (uint32 *) _TIFFmalloc(bytesperrow);
				if (bits)
				{
					for (unsigned int y = 0; y < heightt; y++)
					{
						if (TIFFReadScanline(tif, bits, y, 0))
						{
							memcpy(m_image.scanLine(y), bits, widtht * 4);
						}
					}
					_TIFFfree(bits);
				}
			}
			isCMYK = true;
		}
		else
		{
			bits = (uint32 *) _TIFFmalloc(size * sizeof(uint32));
			if(bits)
			{
				if (TIFFReadRGBAImage(tif, widtht, heightt, bits, 0))
				{
					for(unsigned int y = 0; y < heightt; y++)
						memcpy(m_image.scanLine(heightt - 1 - y), bits + y * widtht, widtht * 4);
				}
				_TIFFfree(bits);
				if (bitspersample == 1)
					bilevel = true;
			}
		}
		swapRGBA();
		DWORD EmbedLen = 0;
		LPBYTE EmbedBuffer;
		if (TIFFGetField(tif, TIFFTAG_ICCPROFILE, &EmbedLen, &EmbedBuffer))
		{
			const char *Descriptor;
			cmsHPROFILE tiffProf = cmsOpenProfileFromMem(EmbedBuffer, EmbedLen);
			Descriptor = cmsTakeProductDesc(tiffProf);
			m_embeddedProfile.duplicate((const char*) EmbedBuffer, EmbedLen);
			m_imageInfoRecord.profileName = QString(Descriptor);
			m_imageInfoRecord.isEmbedded = true;
			cmsCloseProfile(tiffProf);
		}
		else
		{
			m_imageInfoRecord.isEmbedded = false;
			m_imageInfoRecord.profileName = "";
		}
		unsigned int PhotoshopLen = 0;
		unsigned char* PhotoshopBuffer;
		if (TIFFGetField(tif, TIFFTAG_PHOTOSHOP, &PhotoshopLen, &PhotoshopBuffer) )
		{
			if (PhotoshopLen != 0)
			{
				QByteArray arrayPhot(PhotoshopLen);
				arrayPhot.duplicate((const char*)PhotoshopBuffer,PhotoshopLen);
				QDataStream strPhot(arrayPhot,IO_ReadOnly);
				strPhot.setByteOrder( QDataStream::BigEndian );
				PSDHeader fakeHeader;
				fakeHeader.width = m_image.width();
				fakeHeader.height = m_image.height();
				parseRessourceData(strPhot, fakeHeader, PhotoshopLen);
				m_imageInfoRecord.valid = (m_imageInfoRecord.PDSpathData.size())>0?true:false;
			}
		}
		TIFFClose(tif);
		if (resolutionunit == RESUNIT_INCH)
		{
			m_image.setDotsPerMeterX ((int) (xres / 0.0254));
			m_image.setDotsPerMeterY ((int) (yres / 0.0254));
			m_imageInfoRecord.xres = qRound(xres);
			m_imageInfoRecord.yres = qRound(yres);
		}
		else if (resolutionunit == RESUNIT_CENTIMETER)
		{
			m_image.setDotsPerMeterX ((int) (xres * 100.0));
			m_image.setDotsPerMeterY ((int) (yres * 100.0));
			m_imageInfoRecord.xres = qRound(xres*2.54);
			m_imageInfoRecord.yres = qRound(yres*2.54);
		}
		if (isCMYK)
			m_imageInfoRecord.colorspace = 1;
		else if (bilevel)
			m_imageInfoRecord.colorspace = 2;
		else
			m_imageInfoRecord.colorspace = 0;
		m_imageInfoRecord.layerInfo.clear();
		m_imageInfoRecord.BBoxX = 0;
		m_imageInfoRecord.BBoxH = m_image.height();
		return true;
	}
	return false;
}
