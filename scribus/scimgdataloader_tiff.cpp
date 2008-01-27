/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QFile>
#include <QFileInfo>
#include <QObject>
//Added by qt3to4:
#include <QList>
#include "scconfig.h"
#include "scimgdataloader_tiff.h"
#include "util_color.h"
#include CMS_INC

static void TagExtender(TIFF *tiff)
{
	static const TIFFFieldInfo xtiffFieldInfo[] =
	{
		{ 37724, -3, -3, TIFF_UNDEFINED, FIELD_CUSTOM, TRUE, TRUE,   const_cast<char*>("PhotoshopLayerData") }
	};
	TIFFMergeFieldInfo(tiff, xtiffFieldInfo, sizeof (xtiffFieldInfo) / sizeof (xtiffFieldInfo[0]));
}

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
	TIFFSetTagExtender(TagExtender);
	TIFF* tif = TIFFOpen(fn.toLocal8Bit(), "r");
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
				m_embeddedProfile = QByteArray((const char*) EmbedBuffer, EmbedLen);
			}
			cmsCloseProfile(tiffProf);
		}
		TIFFClose(tif);
	}
}

bool ScImgDataLoader_TIFF::preloadAlphaChannel(const QString& fn, int res, bool& hasAlpha)
{
	bool valid = m_imageInfoRecord.isRequest;
	QMap<int, ImageLoadRequest> req = m_imageInfoRecord.RequestProps;
	initialize();
	hasAlpha = false;
	m_imageInfoRecord.RequestProps = req;
	m_imageInfoRecord.isRequest = valid;
	QFileInfo fi = QFileInfo(fn);
	if (!fi.exists())
		return false;
	if(loadPicture(fn, res, false))
	{
		m_imageInfoRecord.valid = true;
		hasAlpha = true;
		if (photometric == PHOTOMETRIC_SEPARATED)
		{
			if (samplesperpixel == 4)
				m_imageInfoRecord.valid = hasAlpha = false;
		}
		else
		{
			if (samplesperpixel == 3)
				m_imageInfoRecord.valid = hasAlpha = false;
		}
		return true;
	}
	else
		r_image.resize(0);
	return false;
}

int ScImgDataLoader_TIFF::getLayers(const QString& fn)
{
	int layerNum = 1;
	int test;
	struct PSDLayer lay;
	TIFFSetTagExtender(TagExtender);
	TIFF* tif = TIFFOpen(fn.toLocal8Bit(), "r");
	if(tif)
	{
		do
		{
			char *layerName=0;
			TIFFGetField(tif, TIFFTAG_PAGENAME, &layerName);
			QString name = QString(layerName);
			if (name.isEmpty())
				lay.layerName = QString("Layer #%1").arg(layerNum);
			else
				lay.layerName = name;
			lay.blend = "norm";
			lay.opacity = 255;
			lay.flags = 0;
			m_imageInfoRecord.layerInfo.append(lay);
			m_imageInfoRecord.valid = true;
			layerNum++;
			test = TIFFReadDirectory(tif);
		}
		while (test == 1);
		TIFFClose(tif);
	}
	return layerNum;
}

void ScImgDataLoader_TIFF::unmultiplyRGBA(RawImage *image)
{
	double r1, g1, b1, coeff;
	unsigned char r, g, b, a;
	for (int i = 0; i < image->height(); ++i)
	{
		unsigned int *ptr = (QRgb *) image->scanLine(i);
		for (int j = 0; j < image->width(); ++j)
		{
			r = qRed(*ptr);
			g = qGreen(*ptr);
			b = qBlue(*ptr);
			a = qAlpha(*ptr);
			if (a > 0 && a < 255)
			{
				coeff = 255.0 / a;
				r1 = coeff * r;
				g1 = coeff * g;
				b1 = coeff * b;
				r  = (r1 <= 255.0) ? (unsigned char) r1 : 255;
				g  = (g1 <= 255.0) ? (unsigned char) g1 : 255;
				b  = (b1 <= 255.0) ? (unsigned char) b1 : 255;
				*ptr++ = qRgba(r,g,b,a);
			}
			else
				++ptr;
		}
	}
}

bool ScImgDataLoader_TIFF::getImageData(TIFF* tif, RawImage *image, uint widtht, uint heightt, uint size, uint16 photometric, uint16 bitspersample, uint16 samplesperpixel, bool &bilevel, bool &isCMYK)
{
	uint32 *bits = 0;
	if (photometric == PHOTOMETRIC_SEPARATED)
	{
		if (samplesperpixel > 5)
		{
			if (!getImageData_RGBA(tif, image, widtht, heightt, size, bitspersample, samplesperpixel))
				return false;
			if (bitspersample == 1)
				bilevel = true;
			isCMYK = false;
		}
		else
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
				for (yt = 0; yt < (uint32) image->height(); yt += rows)
				{
					if (yt > (uint) image->height())
						break;
					if (image->height()-yt < rows)
						tileH = image->height()-yt;
					tileW = columns;
					register uint32 yi;
					int chans = image->channels();
					for (xt = 0; xt < (uint) image->width(); xt += columns)
					{
						TIFFReadTile(tif, tile_buf, xt, yt, 0, 0);
						for (yi = 0; yi < tileH; yi++)
						{
							_TIFFmemcpy(image->scanLine(yt+(tileH-1-yi))+xt, tile_buf+tileW*yi, tileW*chans);
						}
					}
				}
				_TIFFfree(tile_buf);
			}
			else
			{
				tsize_t bytesperrow = TIFFScanlineSize(tif);
				bits = (uint32 *) _TIFFmalloc(bytesperrow);
				int chans = image->channels();
				if (bits)
				{
					for (unsigned int y = 0; y < heightt; y++)
					{
						if (TIFFReadScanline(tif, bits, y, 0))
						{
							memcpy(image->scanLine(y), bits, chans * widtht);
						}
					}
					_TIFFfree(bits);
				}
			}
			isCMYK = true;
		}
	}
	else
	{
		if (!getImageData_RGBA(tif, image, widtht, heightt, size, bitspersample, samplesperpixel))
			return false;
		if (bitspersample == 1)
			bilevel = true;
	}
	return true;
}

bool ScImgDataLoader_TIFF::getImageData_RGBA(TIFF* tif, RawImage *image, uint widtht, uint heightt, uint size, uint16 bitspersample, uint16 samplesperpixel)
{
	bool gotData = false;
	uint32* bits = (uint32 *) _TIFFmalloc(size * sizeof(uint32));
	uint16  extrasamples, *extratypes;
	if (!TIFFGetField (tif, TIFFTAG_EXTRASAMPLES, &extrasamples, &extratypes))
		extrasamples = 0;
	if(bits)
	{
		if (TIFFReadRGBAImage(tif, widtht, heightt, bits, 0))
		{
			for(unsigned int y = 0; y < heightt; y++)
			{
				memcpy(image->scanLine(heightt - 1 - y), bits + y * widtht, widtht * image->channels());
				if (QSysInfo::ByteOrder==QSysInfo::BigEndian)
				{
					unsigned char *s = image->scanLine( heightt - 1 - y );
					unsigned char r, g, b, a;
					for(uint xi=0; xi < widtht; ++xi )
					{
						r = s[0];
						g = s[1];
						b = s[2];
						a = s[3];
						s[0] = a;
						s[1] = b;
						s[2] = g;
						s[3] = r;
						s += image->channels();
					}
				}
			}
			if (extrasamples > 0 && extratypes[0] == EXTRASAMPLE_ASSOCALPHA)
				unmultiplyRGBA(image);
			gotData = true;
		}
		_TIFFfree(bits);
	}
	return gotData;
}

void ScImgDataLoader_TIFF::blendOntoTarget(RawImage *tmp, int layOpa, QString layBlend, bool cmyk, bool useMask)
{
	if (layBlend == "diss")
	{
		for (int l = 0; l < tmp->height(); l++)
		{
			srand(random_table[ l  % 4096]);
			for (int k = 0; k < tmp->width(); k++)
			{
				int rand_val = rand() & 0xff;
				if (rand_val > layOpa)
					tmp->setAlpha(k, l, 0);
			}
		}
	}
	int w = r_image.width();
	int h = r_image.height();
	for( int yi=0; yi < h; ++yi )
	{
		unsigned char *s = tmp->scanLine( yi );
		unsigned char *d = r_image.scanLine( yi );
		unsigned char r, g, b, src_r, src_g, src_b, src_a, src_alpha, dst_alpha;
		unsigned char a = 0;
		for(int xi=0; xi < w; ++xi )
		{
			src_r = s[0];
			src_g = s[1];
			src_b = s[2];
			src_a = s[3];
			if (r_image.channels() == 5)
			{
				dst_alpha = d[4];
				if (useMask)
					src_alpha = s[4];
				else
					src_alpha = 255;
			}
			else
			{
				if (cmyk)
				{
					dst_alpha = 255;
					src_alpha = 255;
				}
				else
				{
					dst_alpha = d[3];
					src_alpha = s[3];
				}
			}
			if (layBlend != "diss")
				src_alpha = INT_MULT(src_alpha, layOpa);
			if ((dst_alpha > 0) && (src_alpha > 0))
			{
				if (layBlend == "mul ")
				{
					src_r = INT_MULT(src_r, d[0]);
					src_g = INT_MULT(src_g, d[1]);
					src_b = INT_MULT(src_b, d[2]);
					if (cmyk)
						src_a = INT_MULT(src_a, d[3]);
				}
				else if (layBlend == "scrn")
				{
					src_r = 255 - ((255-src_r) * (255-d[0]) / 128);
					src_g = 255 - ((255-src_g) * (255-d[1]) / 128);
					src_b = 255 - ((255-src_b) * (255-d[2]) / 128);
					if (cmyk)
						src_a = 255 - ((255-src_a) * (255-d[3]) / 128);
				}
				else if (layBlend == "over")
				{
					src_r = d[0] < 128 ? src_r * d[0] / 128 : 255 - ((255-src_r) * (255-d[0]) / 128);
					src_g = d[1] < 128 ? src_g * d[1] / 128 : 255 - ((255-src_g) * (255-d[1]) / 128);
					src_b = d[2] < 128 ? src_b * d[2] / 128 : 255 - ((255-src_b) * (255-d[2]) / 128);
					if (cmyk)
						src_a = d[3] < 128 ? src_a * d[3] / 128 : 255 - ((255-src_a) * (255-d[3]) / 128);
				}
				else if (layBlend == "diff")
				{
					src_r = d[0] > src_r ? d[0] - src_r : src_r - d[0];
					src_g = d[1] > src_g ? d[1] - src_g : src_g - d[1];
					src_b = d[2] > src_b ? d[2] - src_b : src_b - d[2];
					if (cmyk)
						src_a = d[3] > src_a ? d[3] - src_a : src_a - d[3];
				}
				else if (layBlend == "dark")
				{
					src_r = d[0]  < src_r ? d[0]  : src_r;
					src_g = d[1] < src_g ? d[1] : src_g;
					src_b = d[2] < src_b ? d[2] : src_b;
					if (cmyk)
						src_a = d[3] < src_a ? d[3] : src_a;
				}
				else if (layBlend == "hLit")
				{
					src_r = src_r < 128 ? src_r * d[0] / 128 : 255 - ((255-src_r) * (255-d[0]) / 128);
					src_g = src_g < 128 ? src_g * d[1] / 128 : 255 - ((255-src_g) * (255-d[1]) / 128);
					src_b = src_b < 128 ? src_b * d[2] / 128 : 255 - ((255-src_b) * (255-d[2]) / 128);
					if (cmyk)
						src_a = src_a < 128 ? src_a * d[3] / 128 : 255 - ((255-src_a) * (255-d[3]) / 128);
				}
				else if (layBlend == "sLit")
				{
					src_r = src_r * d[0] / 256 + src_r * (255 - ((255-src_r)*(255-d[0]) / 256) - src_r * d[0] / 256) / 256;
					src_g = src_g * d[1] / 256 + src_g * (255 - ((255-src_g)*(255-d[1]) / 256) - src_g * d[1] / 256) / 256;
					src_b = src_b * d[2] / 256 + src_b * (255 - ((255-src_b)*(255-d[2]) / 256) - src_b * d[2] / 256) / 256;
					if (cmyk)
						src_a = src_a * d[3] / 256 + src_a * (255 - ((255-src_a)*(255-d[3]) / 256) - src_a * d[3] / 256) / 256;
				}
				else if (layBlend == "lite")
				{
					src_r = d[0] < src_r ? src_r : d[0];
					src_g = d[1] < src_g ? src_g : d[1];
					src_b = d[2] < src_b ? src_b : d[2];
					if (cmyk)
						src_a = d[3] < src_a ? src_a : d[3];
				}
				else if (layBlend == "smud")
				{
					src_r = d[0] + src_r - src_r * d[0] / 128;
					src_g = d[1] + src_g - src_g * d[1] / 128;
					src_b = d[2] + src_b - src_b * d[2] / 128;
					if (cmyk)
						src_a = d[3] + src_a - src_a * d[3] / 128;
				}
				else if (layBlend == "div ")
				{
					src_r = src_r == 255 ? 255 : ((d[0] * 256) / (255-src_r)) > 255 ? 255 : (d[0] * 256) / (255-src_r);
					src_g = src_g == 255 ? 255 : ((d[1] * 256) / (255-src_g)) > 255 ? 255 : (d[1] * 256) / (255-src_g);
					src_b = src_b == 255 ? 255 : ((d[2] * 256) / (255-src_b)) > 255 ? 255 : (d[2] * 256) / (255-src_b);
					if (cmyk)
						src_a = src_a == 255 ? 255 : ((d[3] * 256) / (255-src_a)) > 255 ? 255 : (d[3] * 256) / (255-src_a);
				}
				else if (layBlend == "idiv")
				{
					src_r = src_r == 0 ? 0 : (255 - (((255-d[0]) * 256) / src_r)) < 0 ? 0 : 255 - (((255-d[0]) * 256) / src_r);
					src_g = src_g == 0 ? 0 : (255 - (((255-d[1]) * 256) / src_g)) < 0 ? 0 : 255 - (((255-d[1]) * 256) / src_g);
					src_b = src_b == 0 ? 0 : (255 - (((255-d[2]) * 256) / src_b)) < 0 ? 0 : 255 - (((255-d[2]) * 256) / src_b);
					if (cmyk)
						src_a = src_a == 0 ? 0 : (255 - (((255-d[3]) * 256) / src_a)) < 0 ? 0 : 255 - (((255-d[3]) * 256) / src_a);
				}
				else if (layBlend == "hue ")
				{
					if (!cmyk)
					{
						uchar new_r = d[0];
						uchar new_g = d[1];
						uchar new_b = d[2];
						RGBTOHSV(src_r, src_g, src_b);
						RGBTOHSV(new_r, new_g, new_b);
						new_r = src_r;
						HSVTORGB(new_r, new_g, new_b);
						src_r = new_r;
						src_g = new_g;
						src_b = new_b;
					}
				}
				else if (layBlend == "sat ")
				{
					if (!cmyk)
					{
						uchar new_r = d[0];
						uchar new_g = d[1];
						uchar new_b = d[2];
						RGBTOHSV(src_r, src_g, src_b);
						RGBTOHSV(new_r, new_g, new_b);
						new_g = src_g;
						HSVTORGB(new_r, new_g, new_b);
						src_r = new_r;
						src_g = new_g;
						src_b = new_b;
					}
				}
				else if (layBlend == "lum ")
				{
					if (!cmyk)
					{
						uchar new_r = d[0];
						uchar new_g = d[1];
						uchar new_b = d[2];
						RGBTOHSV(src_r, src_g, src_b);
						RGBTOHSV(new_r, new_g, new_b);
						new_b = src_b;
						HSVTORGB(new_r, new_g, new_b);
						src_r = new_r;
						src_g = new_g;
						src_b = new_b;
					}
				}
				else if (layBlend == "colr")
				{
					if (!cmyk)
					{
						uchar new_r = d[0];
						uchar new_g = d[1];
						uchar new_b = d[2];
						RGBTOHLS(src_r, src_g, src_b);
						RGBTOHLS(new_r, new_g, new_b);
						new_r = src_r;
						new_b = src_b;
						HLSTORGB(new_r, new_g, new_b);
						src_r = new_r;
						src_g = new_g;
						src_b = new_b;
					}
				}
			}
			if (dst_alpha == 0)
			{
				r = src_r;
				g = src_g;
				b = src_b;
				a = src_a;
			}
			else
			{
				if (src_alpha > 0)
				{
					r = (d[0] * (255 - src_alpha) + src_r * src_alpha) / 255;
					g = (d[1] * (255 - src_alpha) + src_g * src_alpha) / 255;
					b = (d[2] * (255 - src_alpha) + src_b * src_alpha) / 255;
					if (cmyk)
						a = (d[3] * (255 - src_alpha) + src_a * src_alpha) / 255;
					if (layBlend != "diss")
						src_alpha = dst_alpha + INT_MULT(255 - dst_alpha, src_alpha);
				}
			}
			if (src_alpha > 0)
			{
				d[0] = r;
				d[1] = g;
				d[2] = b;
				if (r_image.channels() == 5)
				{
					d[3] = a;
					d[4] = src_alpha;
				}
				else
				{
					if (cmyk)
						d[3] = a;
					else
						d[3] = src_alpha;
				}
			}
			d += r_image.channels();
			s += tmp->channels();
		}
	}
}

bool ScImgDataLoader_TIFF::loadPicture(const QString& fn, int res, bool thumbnail)
{
	bool bilevel = false;
	bool failedPS = false;
	bool foundPS = false;
	short resolutionunit = 0;
	float xres = 72.0, yres = 72.0;
	if (!QFile::exists(fn))
		return false;
	QByteArray byteOrder(2, ' ');
	QFile fo(fn);
	if (fo.open(QIODevice::ReadOnly))
	{
		fo.read(byteOrder.data(), 1);
		fo.close();
	}
	srand(314159265);
	for (int i = 0; i < 4096; i++)
		random_table[i] = rand();
	for (int i = 0; i < 4096; i++)
	{
		int tmp;
		int swap = i + rand() % (4096 - i);
		tmp = random_table[i];
		random_table[i] = random_table[swap];
		random_table[swap] = tmp;
	}
	int test;
	int layerNum = 0;
	bool valid = m_imageInfoRecord.isRequest;
	QMap<int, ImageLoadRequest> req = m_imageInfoRecord.RequestProps;
	initialize();
	m_imageInfoRecord.RequestProps = req;
	m_imageInfoRecord.isRequest = valid;
	m_imageInfoRecord.type = ImageTypeTIF;
	getLayers(fn);
	TIFFSetTagExtender(TagExtender);
	TIFF* tif = TIFFOpen(fn.toLocal8Bit(), "r");
	if(tif)
	{
		bool isCMYK = false;
		unsigned int widtht, heightt, size;
		char *description=0, *copyright=0, *datetime=0, *artist=0, *scannerMake=0, *scannerModel=0;
		uint16 bitspersample, fillorder, planar;

		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &widtht);
		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &heightt);
		TIFFGetField(tif, TIFFTAG_XRESOLUTION, &xres);
		TIFFGetField(tif, TIFFTAG_YRESOLUTION, &yres);
		TIFFGetField(tif, TIFFTAG_RESOLUTIONUNIT , &resolutionunit);
		size = widtht * heightt;
		TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
		TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planar);
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
		DWORD EmbedLen = 0;
		LPBYTE EmbedBuffer;
		if (TIFFGetField(tif, TIFFTAG_ICCPROFILE, &EmbedLen, &EmbedBuffer))
		{
			const char *Descriptor;
			cmsHPROFILE tiffProf = cmsOpenProfileFromMem(EmbedBuffer, EmbedLen);
			Descriptor = cmsTakeProductDesc(tiffProf);
			m_embeddedProfile = QByteArray((const char*) EmbedBuffer, EmbedLen);
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
				QByteArray arrayPhot = QByteArray((const char*)PhotoshopBuffer,PhotoshopLen);
				QDataStream strPhot(&arrayPhot,QIODevice::ReadOnly);
				strPhot.setByteOrder( QDataStream::BigEndian );
				PSDHeader fakeHeader;
				fakeHeader.width = widtht;
				fakeHeader.height = heightt;
				parseRessourceData(strPhot, fakeHeader, PhotoshopLen);
				m_imageInfoRecord.exifInfo.width = widtht;
				m_imageInfoRecord.exifInfo.height = heightt;
				if (!m_imageInfoRecord.valid)
					m_imageInfoRecord.valid = (m_imageInfoRecord.PDSpathData.size())>0?true:false;
				if (thumbnail)
				{
					if (photometric == PHOTOMETRIC_SEPARATED)
					{
						isCMYK = true;
						m_imageInfoRecord.colorspace = ColorSpaceCMYK;
					}
					else
						m_imageInfoRecord.colorspace = ColorSpaceRGB;
					if (bitspersample == 1)
						bilevel = true;
					if (!m_imageInfoRecord.exifInfo.thumbnail.isNull())
					{
						if (isCMYK)
							r_image.create(m_imageInfoRecord.exifInfo.thumbnail.width(), m_imageInfoRecord.exifInfo.thumbnail.height(), 5);
						else
							r_image.create(m_imageInfoRecord.exifInfo.thumbnail.width(), m_imageInfoRecord.exifInfo.thumbnail.height(), 4);
						r_image.fill(0);
						QRgb *s;
						uchar *d;
						unsigned char cc, cm, cy, ck;
						for( int yit=0; yit < m_imageInfoRecord.exifInfo.thumbnail.height(); ++yit )
						{
							s = (QRgb*)(m_imageInfoRecord.exifInfo.thumbnail.scanLine( yit ));
							d = r_image.scanLine( yit );
							for(int xit=0; xit < m_imageInfoRecord.exifInfo.thumbnail.width(); ++xit )
							{
								if (isCMYK)
								{
									cc = 255 - qRed(*s);
									cm = 255 - qGreen(*s);
									cy = 255 - qBlue(*s);
									ck = qMin(qMin(cc, cm), cy);
									d[0] = cc-ck;
									d[1] = cm-ck;
									d[2] = cy-ck;
									d[3] = ck;
									d[4] = 255;
								}
								else
								{
									d[0] = qRed(*s);
									d[1] = qGreen(*s);
									d[2] = qBlue(*s);
									d[3] = 255;
								}
								s++;
								d += r_image.channels();
							}
						}
						TIFFClose(tif);
						return true;
					}
				}
			}
		}
		unsigned int PhotoshopLen2 = 0;
		unsigned char* PhotoshopBuffer2;
		if (TIFFGetField(tif, 37724, &PhotoshopLen2, &PhotoshopBuffer2) )
		{
			if (PhotoshopLen2 > 40)
			{
				m_imageInfoRecord.layerInfo.clear();
				QByteArray arrayPhot;
				arrayPhot.fromRawData((const char*)PhotoshopBuffer2, PhotoshopLen2);
				QDataStream s(&arrayPhot,QIODevice::ReadOnly);
				if (byteOrder[0] == 'M')
					s.setByteOrder( QDataStream::BigEndian );
				else
					s.setByteOrder( QDataStream::LittleEndian );
				uint addRes, layerinfo, channelLen, signature, extradata, layermasksize, layerRange, dummy;
				int top, left, bottom, right;
				short numLayers, numChannels;
				short channelType;
				uchar blendKey[4];
				uchar opacity, clipping, flags, filler;
				QString layerName, blend;
				struct PSDLayer lay;
				do
				{
					if(s.atEnd())
					{
						m_imageInfoRecord.layerInfo.clear();
						failedPS = true;
						break;
					}
					s >> signature;
				}
				while (signature != 0x4c617972);
				if (!failedPS)
				{
					s >> layerinfo;
					s >> numLayers;
					if (numLayers < 0)
						numLayers = -numLayers;
					if (numLayers != 0)
					{
						for (int layer = 0; layer < numLayers; layer++)
						{
							s >> top;
							lay.ypos = top;
							s >> left;
							lay.xpos = left;
							s >> bottom;
							lay.height = bottom - top;
							s >> right;
							lay.width = right - left;
							s >> numChannels;
							if (numChannels > 6)	// we don't support images with more than 6 channels yet
							{
								m_imageInfoRecord.layerInfo.clear();
								failedPS = true;
								break;
							}
							lay.channelType.clear();
							lay.channelLen.clear();
							for (int channels = 0; channels < numChannels; channels++)
							{
								s >> channelType;
								s >> channelLen;
								lay.channelType.append(channelType);
								lay.channelLen.append(channelLen);
							}
							s >> signature;
							blend = "";
							for( int i = 0; i < 4; i++ )
							{
								s >> blendKey[i];
								if (byteOrder[0] == 'M')
									blend.append(QChar(blendKey[i]));
								else
									blend.prepend(QChar(blendKey[i]));
							}
							lay.blend = blend;
							s >> opacity;
							lay.opacity = opacity;
							s >> clipping;
							lay.clipping = clipping;
							s >> flags;
							if (flags & 8)
							{
								if (flags & 16)	// Unknown combination of layer flags, probably an adjustment or effects layer
								{
									m_imageInfoRecord.layerInfo.clear();
									failedPS = true;
									break;
								}
							}
							lay.flags = flags;
							s >> filler;
							s >> extradata;
							s >> layermasksize;
							lay.maskYpos = 0;
							lay.maskXpos = 0;
							lay.maskHeight = 0;
							lay.maskWidth = 0;
							if (layermasksize != 0)
							{
								s >> lay.maskYpos;
								s >> lay.maskXpos;
								s >> dummy;
								lay.maskHeight = dummy - lay.maskYpos;
								s >> dummy;
								lay.maskWidth = dummy - lay.maskXpos;
								s >> dummy;
							}
							s >> layerRange;
							s.device()->seek( s.device()->pos() + layerRange );
							lay.layerName = getLayerString(s);
							m_imageInfoRecord.layerInfo.append(lay);
							s >> signature;
							if( signature == 0x3842494D )
							{
								while (signature == 0x3842494D )
								{
									s >> signature;
									s >> addRes;
									s.device()->seek( s.device()->pos() + addRes );
									s >> signature;
								}
								s.device()->seek( s.device()->pos() - 4 );
							}
							else
							{
								s.device()->seek( s.device()->pos() - 2 );
								s >> signature;
								if( signature == 0x3842494D )
								{
									while (signature == 0x3842494D )
									{
										s >> signature;
										s >> addRes;
										s.device()->seek( s.device()->pos() + addRes );
										s >> signature;
									}
									s.device()->seek( s.device()->pos() - 4 );
								}
								else
									s.device()->seek( s.device()->pos() - 6 );
							}
						}
					}
				}
				if (!failedPS)
				{
					int chans = 4;
					bilevel = false;
					PSDHeader fakeHeader;
					fakeHeader.width = widtht;
					fakeHeader.height = heightt;
					fakeHeader.channel_count = numChannels;
					fakeHeader.depth = 8;
					if (photometric == PHOTOMETRIC_SEPARATED)
					{
						isCMYK = true;
						fakeHeader.color_mode = CM_CMYK;
						chans = 5;
					}
					else
					{
						fakeHeader.color_mode = CM_RGB;
						isCMYK = false;
						chans = 5;
					}
					if( !r_image.create( widtht, heightt, chans ))
						return false;
					r_image.fill(0);
					bool firstLayer = true;
					for (int layer = 0; layer < numLayers; layer++)
					{
						loadLayerChannels( s, fakeHeader, m_imageInfoRecord.layerInfo, layer, &firstLayer );
					}
					arrayPhot.clear();
					TIFFClose(tif);
					foundPS = true;
					if (m_imageInfoRecord.layerInfo.count() == 1)
						m_imageInfoRecord.layerInfo.clear();
				}
				else
				{
					arrayPhot.clear();
					getLayers(fn);
				}
			}
		}
		if( xres <= 1.0 || yres <= 1.0 )
		{
			xres = yres = 72.0;
			QFileInfo qfi(fn);
			m_message = QObject::tr("%1 may be corrupted : missing resolution tags").arg(qfi.fileName());
			m_msgType = warningMsg;
		}
		if ((!foundPS) || (failedPS))
		{
			int chans = 4;
			if (photometric == PHOTOMETRIC_SEPARATED)
			{
				if (samplesperpixel > 5) 
					chans = 4;
				else
					chans = samplesperpixel;
			}
			else
				chans = 4;
			if (!r_image.create(widtht, heightt, chans))
			{
				TIFFClose(tif);
				return false;
			}
			r_image.fill(0);
			bool firstL = true;
			do
			{
				RawImage tmpImg;
				if( !tmpImg.create(widtht, heightt, chans))
				{
					TIFFClose(tif);
					return false;
				}
				else
				{
					tmpImg.fill(0);
					if (!getImageData(tif, &tmpImg, widtht, heightt, size, photometric, bitspersample, samplesperpixel, bilevel, isCMYK))
					{
						TIFFClose(tif);
						return false;
					}
					bool visible = true;
					bool useMask = true;
					if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layerNum)))
						visible = m_imageInfoRecord.RequestProps[layerNum].visible;
					QString layBlend = "norm";
					if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layerNum)))
						layBlend = m_imageInfoRecord.RequestProps[layerNum].blend;
					if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layerNum)))
						useMask = m_imageInfoRecord.RequestProps[layerNum].useMask;
					int layOpa = 255;
					if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layerNum)))
						layOpa = m_imageInfoRecord.RequestProps[layerNum].opacity;
					if (visible)
					{
						if ((firstL) && (layBlend != "diss"))
							r_image = tmpImg;
						else
							blendOntoTarget(&tmpImg, layOpa, layBlend, isCMYK, useMask);
						firstL = false;
					}
					//JG Copy should not be necessary as QImage is implicitly shared in Qt4
					QImage imt; //QImage imt = tmpImg.copy();
					if (chans > 4)
						imt = tmpImg.convertToQImage(true);
					else
						imt = tmpImg.convertToQImage(false);
					double sx = tmpImg.width() / 40.0;
					double sy = tmpImg.height() / 40.0;
					imt = sy < sx ?	imt.scaled(qRound(imt.width() / sx), qRound(imt.height() / sx), Qt::IgnoreAspectRatio, Qt::SmoothTransformation) :
												imt.scaled(qRound(imt.width() / sy), qRound(imt.height() / sy), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
					m_imageInfoRecord.layerInfo[layerNum].thumb = imt.copy();
					if (chans > 4)
					{
						QImage imt2 = imt.createAlphaMask();
						imt2.invertPixels();
						m_imageInfoRecord.layerInfo[layerNum].thumb_mask = imt2.copy();
					}
					else
						m_imageInfoRecord.layerInfo[layerNum].thumb_mask = QImage();
					layerNum++;
				}
				if ((m_imageInfoRecord.layerInfo.count() == 1) && (chans < 5))
					m_imageInfoRecord.layerInfo.clear();
				test = TIFFReadDirectory(tif);
			}
			while (test == 1);
			TIFFClose(tif);
		}
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
			m_imageInfoRecord.colorspace = ColorSpaceCMYK;
		else if (bilevel)
			m_imageInfoRecord.colorspace = ColorSpaceGray;
		else
			m_imageInfoRecord.colorspace = ColorSpaceRGB;
		m_imageInfoRecord.BBoxX = 0;
		m_imageInfoRecord.BBoxH = r_image.height();
		if ((m_imageInfoRecord.layerInfo.isEmpty()) && (m_imageInfoRecord.PDSpathData.isEmpty()))
			m_imageInfoRecord.valid = false;
		else
			m_imageInfoRecord.valid = true;
		return true;
	}
	return false;
}

QString ScImgDataLoader_TIFF::getLayerString(QDataStream & s)
{
	uchar len, tmp;
	uint adj;
	QString ret = "";
	s >> len;
	if (len == 0)
	{
		s >> tmp;
		s >> tmp;
		s >> tmp;
		return ret;
	}
	for( int i = 0; i < len; i++ )
	{
		s >> tmp;
		ret += QChar(tmp);
	}
	adj = 0;
	if (((ret.length()+1) % 4) != 0)
		adj = 4 - ((ret.length()+1) % 4);
	s.device()->seek( s.device()->pos() + adj );
	return ret;
}

bool ScImgDataLoader_TIFF::loadChannel( QDataStream & s, const PSDHeader & header, QList<PSDLayer> &layerInfo, uint layer, int channel, int component, RawImage &tmpImg)
{
	uint base = s.device()->pos();
	uchar cbyte;
	ushort compression;
	s >> compression;
	if( compression > 1 )
		return false;
	if (compression == 0)
	{
		int count = layerInfo[layer].channelLen[channel]-2;
		for (int i = 0; i < tmpImg.height(); i++)
		{
			uchar *ptr =  tmpImg.scanLine(i);
			for (int j = 0; j < tmpImg.width(); j++)
			{
				s >> cbyte;
				count--;
				if ((header.color_mode == CM_CMYK) && (component < 4))
					cbyte = 255 - cbyte;
				ptr[component] = cbyte;
				if (count == 0)
					break;
				ptr += tmpImg.channels();
			}
			if (count == 0)
				break;
		}
	}
	else
	{
		s.device()->seek( s.device()->pos() + tmpImg.height() * 2 );
		uint pixel_count = tmpImg.width();
		for (int hh = 0; hh < tmpImg.height(); hh++)
		{
			uint count = 0;
			uchar *ptr = tmpImg.scanLine(hh);
			uchar *ptr2 = ptr+tmpImg.width() * tmpImg.channels();
			ptr += component;
			while( count < pixel_count )
			{
				uchar c;
				if(s.atEnd())
					return false;
				s >> c;
				uint len = c;
				if( len < 128 )
				{
					// Copy next len+1 bytes literally.
					len++;
					count += len;
					while( len != 0 )
					{
						s >> cbyte;
						if (ptr < ptr2)
						{
							if ((header.color_mode == CM_CMYK) && (component < 4))
								cbyte = 255 - cbyte;
							*ptr = cbyte;
						}
						ptr += tmpImg.channels();
						len--;
					}
				}
				else if( len > 128 )
				{
					// Next -len+1 bytes in the dest are replicated from next source byte.
					// (Interpret len as a negative 8-bit int.)
					len ^= 0xFF;
					len += 2;
					count += len;
					uchar val;
					s >> val;
					if ((header.color_mode == CM_CMYK) && (component < 4))
						val = 255 - val;
					while( len != 0 )
					{
						if (ptr < ptr2)
							*ptr = val;
						ptr += tmpImg.channels();
						len--;
					}
				}
				else if( len == 128 )
				{
					// No-op.
				}
			}
		}
	}
	s.device()->seek( base+layerInfo[layer].channelLen[channel] );
	return true;
}

bool ScImgDataLoader_TIFF::loadLayerChannels( QDataStream & s, const PSDHeader & header, QList<PSDLayer> &layerInfo, uint layer, bool* firstLayer)
{
	// Find out if the data is compressed.
	// Known values:
	//   0: no compression
	//   1: RLE compressed
	uint base = s.device()->pos();
	uint base2 = base;
	uint channel_num = layerInfo[layer].channelLen.count();
	bool hasMask = false;
	bool hasAlpha = false;
	RawImage r2_image;
	RawImage mask;
	bool createOk = false;
	if (header.color_mode == CM_CMYK)
	{
		createOk = r2_image.create(layerInfo[layer].width, layerInfo[layer].height, qMax(channel_num, (uint) 5));
		r2_image.fill(0);
	}
	else
	{
		createOk = r2_image.create(layerInfo[layer].width, layerInfo[layer].height, qMax(channel_num, (uint) 4));
		r2_image.fill(0);
	}
	if( !createOk )
	{
		for(uint channel = 0; channel < channel_num; channel++)
		{
			base2 += layerInfo[layer].channelLen[channel];
		}
		s.device()->seek( base2 );
		return false;
	}
	channel_num = qMin(channel_num, (uint) 39);
	uint components[40];
	for(uint channel = 0; channel < channel_num; channel++)
	{
		switch(layerInfo[layer].channelType[channel])
		{
		case 0:
			components[channel] = 0;
			break;
		case 1:
			components[channel] = 1;
			break;
		case 2:
			components[channel] = 2;
			break;
		case 3:
			components[channel] = 3;
			break;
		case -1:
			if (header.color_mode == CM_CMYK)
			{
				if (channel_num == 6)
					components[channel] = channel_num-2;
				else
					components[channel] = channel_num-1;
			}
			else
			{
				if (channel_num == 5)
					components[channel] = channel_num-2;
				else
					components[channel] = channel_num-1;
			}
			hasAlpha = true;
			break;
		case -2:
			components[channel] = channel_num-1;
			break;
		}
	}
	if (!hasAlpha)
		r2_image.fill((char) 255);
	for(uint channel = 0; channel < channel_num; channel++)
	{
		if (layerInfo[layer].channelType[channel] == -2)
		{
			if (!mask.create( layerInfo[layer].maskWidth, layerInfo[layer].maskHeight, 1 ))
				break;
			mask.fill(0);
			if (!loadChannel(s, header, layerInfo, layer, channel, 0, mask))
				break;
			hasMask = true;
		}
		if (!loadChannel(s, header, layerInfo, layer, channel, components[channel], r2_image))
			break;
	}
	for(uint channel = 0; channel < channel_num; channel++)
	{
		base2 += layerInfo[layer].channelLen[channel];
	}
	s.device()->seek( base2 );
	QImage tmpImg2;
	if (header.color_mode == CM_CMYK)
		tmpImg2 = r2_image.convertToQImage(true);
	else
		tmpImg2 = r2_image.convertToQImage(false);
	QImage imt;
	double sx = tmpImg2.width() / 40.0;
	double sy = tmpImg2.height() / 40.0;
	imt = sy < sx ?  tmpImg2.scaled(qRound(tmpImg2.width() / sx), qRound(tmpImg2.height() / sx), Qt::IgnoreAspectRatio, Qt::SmoothTransformation) :
	      tmpImg2.scaled(qRound(tmpImg2.width() / sy), qRound(tmpImg2.height() / sy), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	layerInfo[layer].thumb = imt.copy();
	if (hasMask)
	{
		QImage imt2;
		QImage tmpImg;
		tmpImg = mask.convertToQImage(true);
		double sx = tmpImg.width() / 40.0;
		double sy = tmpImg.height() / 40.0;
		imt2 = sy < sx ?  tmpImg.scaled(qRound(tmpImg.width() / sx), qRound(tmpImg.height() / sx), Qt::IgnoreAspectRatio, Qt::SmoothTransformation) :
	      tmpImg.scaled(qRound(tmpImg.width() / sy), qRound(tmpImg.height() / sy), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		imt2.invertPixels();
		layerInfo[layer].thumb_mask = imt2.copy();
	}
	else
		layerInfo[layer].thumb_mask = QImage();
	if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layer)))
		m_imageInfoRecord.RequestProps[layer].useMask = m_imageInfoRecord.RequestProps[layer].useMask;
	else
		m_imageInfoRecord.RequestProps[layer].useMask = true;
	bool visible = !(layerInfo[layer].flags & 2);
	if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layer)))
		visible = m_imageInfoRecord.RequestProps[layer].visible;
	if (visible)
	{
		unsigned int startSrcY, startSrcX, startDstY, startDstX;
		if (layerInfo[layer].ypos < 0)
		{
			startSrcY = abs(layerInfo[layer].ypos);
			startDstY = 0;
		}
		else
		{
			startSrcY = 0;
			startDstY = layerInfo[layer].ypos;
		}
		if (layerInfo[layer].xpos < 0)
		{
			startSrcX = abs(layerInfo[layer].xpos);
			startDstX = 0;
		}
		else
		{
			startSrcX = 0;
			startDstX = layerInfo[layer].xpos;
		}
		unsigned int startSrcYm, startSrcXm, startDstYm, startDstXm;
		if (layerInfo[layer].maskYpos < 0)
		{
			startSrcYm = abs(layerInfo[layer].maskYpos);
			startDstYm = 0;
		}
		else
		{
			startSrcYm = 0;
			startDstYm = layerInfo[layer].maskYpos;
		}
		if (layerInfo[layer].maskXpos < 0)
		{
			startSrcXm = abs(layerInfo[layer].maskXpos);
			startDstXm = 0;
		}
		else
		{
			startSrcXm = 0;
			startDstXm = layerInfo[layer].maskXpos;
		}
		QString layBlend2 = layerInfo[layer].blend;
		if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layer)))
			layBlend2 = m_imageInfoRecord.RequestProps[layer].blend;
		if (layBlend2 == "diss")
		{
			hasAlpha = true;
			int layOpa = layerInfo[layer].opacity;
			if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layer)))
				layOpa = m_imageInfoRecord.RequestProps[layer].opacity;
			for (int l = 0; l < r2_image.height(); l++)
			{
				srand(random_table[ l  % 4096]);
				for (int k = 0; k < r2_image.width(); k++)
				{
					int rand_val = rand() & 0xff;
					if (rand_val > layOpa)
						r2_image.setAlpha(k, l, 0);
				}
			}
		}
		if (*firstLayer)
		{
			for( int yi=static_cast<int>(startSrcY); yi < qMin(r2_image.height(),  r_image.height()); ++yi )
			{
				unsigned char *s = r2_image.scanLine( yi );
				unsigned char *d = r_image.scanLine( qMin(static_cast<int>(startDstY),  r_image.height()-1) );
				d += qMin(static_cast<int>(startDstX), r_image.width()-1) * r_image.channels();
				s += qMin(static_cast<int>(startSrcX), r2_image.width()-1) * r2_image.channels();
				for(int xi=static_cast<int>(startSrcX); xi < qMin(r2_image.width(),  r_image.width()); ++xi )
				{
					d[0] = s[0];
					d[1] = s[1];
					d[2] = s[2];
					if (header.color_mode == CM_RGB)
					{
						if (hasAlpha)
							d[3] = s[3];
						else
							d[3] = 255;
					}
					else
					{
						d[3] = s[3];
						if (hasAlpha)
							d[4] = s[4];
						else
							d[4] = 255;
					}
					s += r2_image.channels();
					d += r_image.channels();
				}
				startDstY++;
			}
		}
		else
		{
			for (int i = static_cast<int>(startSrcY); i < layerInfo[layer].height; i++)
			{
				unsigned char *d = r_image.scanLine(qMin(static_cast<int>(startDstY),  r_image.height()-1));
				unsigned char *s = r2_image.scanLine(qMin(i, r2_image.height()-1));
				d += qMin(static_cast<int>(startDstX),  r_image.width()-1) * r_image.channels();
				s += qMin(static_cast<int>(startSrcX), r2_image.width()-1) * r2_image.channels();
				unsigned char *sm = 0;
				if (hasMask)
				{
					sm = mask.scanLine(qMin(i, mask.height()-1));
					sm += qMin(static_cast<int>(startSrcXm), mask.width()-1) * mask.channels();
				}
				startDstY++;
				unsigned char r, g, b, src_r, src_g, src_b, src_a, src_alpha, dst_alpha;
				unsigned char a = 0;
				unsigned int maxDestX = r_image.width() - startDstX + startSrcX - 1;
				for (unsigned int j = startSrcX; j < qMin(maxDestX, static_cast<unsigned int>(layerInfo[layer].width)); j++)
				{
					src_r = s[0];
					src_g = s[1];
					src_b = s[2];
					src_a = s[3];
					if (hasAlpha)
					{
						if (hasMask)
						{
							if (m_imageInfoRecord.RequestProps[layer].useMask)
								src_alpha = sm[0];
							else
								src_alpha = s[channel_num - 2];
						}
						else
							src_alpha = s[channel_num - 1];
					}
					else
						src_alpha = 255;
					if ((hasMask) && (m_imageInfoRecord.RequestProps[layer].useMask))
						src_alpha = sm[0];
					int layOpa = layerInfo[layer].opacity;
					if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layer)))
						layOpa = m_imageInfoRecord.RequestProps[layer].opacity;
					QString layBlend = layerInfo[layer].blend;
					if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layer)))
						layBlend = m_imageInfoRecord.RequestProps[layer].blend;
					if (layBlend != "diss")
						src_alpha = INT_MULT(src_alpha, layOpa);
					if (header.color_mode == CM_CMYK)
						dst_alpha = d[4];
					else
						dst_alpha = d[3];
					if ((dst_alpha > 0) && (src_alpha > 0))
					{
						if (layBlend == "mul ")
						{
							src_r = INT_MULT(src_r, d[0]);
							src_g = INT_MULT(src_g, d[1]);
							src_b = INT_MULT(src_b, d[2]);
							if (header.color_mode == CM_CMYK)
								src_a = INT_MULT(src_a, d[3]);
						}
						else if (layBlend == "scrn")
						{
							src_r = 255 - ((255-src_r) * (255-d[0]) / 128);
							src_g = 255 - ((255-src_g) * (255-d[1]) / 128);
							src_b = 255 - ((255-src_b) * (255-d[2]) / 128);
							if (header.color_mode == CM_CMYK)
								src_a = 255 - ((255-src_a) * (255-d[3]) / 128);
						}
						else if (layBlend == "over")
						{
							src_g = d[1] < 128 ? src_g * d[1] / 128 : 255 - ((255-src_g) * (255-d[1]) / 128);
							src_b = d[2] < 128 ? src_b * d[2] / 128 : 255 - ((255-src_b) * (255-d[2]) / 128);
							src_a = d[3] < 128 ? src_a * d[3] / 128 : 255 - ((255-src_a) * (255-d[3]) / 128);
							if (header.color_mode == CM_CMYK)
								src_r = d[0] < 128 ? src_r * d[0] / 128 : 255 - ((255-src_r) * (255-d[0]) / 128);
						}
						else if (layBlend == "diff")
						{
							src_r = d[0] > src_r ? d[0] - src_r : src_r - d[0];
							src_g = d[1] > src_g ? d[1] - src_g : src_g - d[1];
							src_b = d[2] > src_b ? d[2] - src_b : src_b - d[2];
							if (header.color_mode == CM_CMYK)
								src_a = d[3] > src_a ? d[3] - src_a : src_a - d[3];
						}
						else if (layBlend == "dark")
						{
							src_r = d[0]  < src_r ? d[0]  : src_r;
							src_g = d[1] < src_g ? d[1] : src_g;
							src_b = d[2] < src_b ? d[2] : src_b;
							if (header.color_mode == CM_CMYK)
								src_a = d[3] < src_a ? d[3] : src_a;
						}
						else if (layBlend == "hLit")
						{
							src_r = src_r < 128 ? src_r * d[0] / 128 : 255 - ((255-src_r) * (255-d[0]) / 128);
							src_g = src_g < 128 ? src_g * d[1] / 128 : 255 - ((255-src_g) * (255-d[1]) / 128);
							src_b = src_b < 128 ? src_b * d[2] / 128 : 255 - ((255-src_b) * (255-d[2]) / 128);
							if (header.color_mode == CM_CMYK)
								src_a = src_a < 128 ? src_a * d[3] / 128 : 255 - ((255-src_a) * (255-d[3]) / 128);
						}
						else if (layBlend == "sLit")
						{
							src_r = src_r * d[0] / 256 + src_r * (255 - ((255-src_r)*(255-d[0]) / 256) - src_r * d[0] / 256) / 256;
							src_g = src_g * d[1] / 256 + src_g * (255 - ((255-src_g)*(255-d[1]) / 256) - src_g * d[1] / 256) / 256;
							src_b = src_b * d[2] / 256 + src_b * (255 - ((255-src_b)*(255-d[2]) / 256) - src_b * d[2] / 256) / 256;
							if (header.color_mode == CM_CMYK)
								src_a = src_a * d[3] / 256 + src_a * (255 - ((255-src_a)*(255-d[3]) / 256) - src_a * d[3] / 256) / 256;
						}
						else if (layBlend == "lite")
						{
							src_r = d[0] < src_r ? src_r : d[0];
							src_g = d[1] < src_g ? src_g : d[1];
							src_b = d[2] < src_b ? src_b : d[2];
							if (header.color_mode == CM_CMYK)
								src_a = d[3] < src_a ? src_a : d[3];
						}
						else if (layBlend == "smud")
						{
							src_r = d[0] + src_r - src_r * d[0] / 128;
							src_g = d[1] + src_g - src_g * d[1] / 128;
							src_b = d[2] + src_b - src_b * d[2] / 128;
							if (header.color_mode == CM_CMYK)
								src_a = d[3] + src_a - src_a * d[3] / 128;
						}
						else if (layBlend == "div ")
						{
							src_r = src_r == 255 ? 255 : ((d[0] * 256) / (255-src_r)) > 255 ? 255 : (d[0] * 256) / (255-src_r);
							src_g = src_g == 255 ? 255 : ((d[1] * 256) / (255-src_g)) > 255 ? 255 : (d[1] * 256) / (255-src_g);
							src_b = src_b == 255 ? 255 : ((d[2] * 256) / (255-src_b)) > 255 ? 255 : (d[2] * 256) / (255-src_b);
							if (header.color_mode == CM_CMYK)
								src_a = src_a == 255 ? 255 : ((d[3] * 256) / (255-src_a)) > 255 ? 255 : (d[3] * 256) / (255-src_a);
						}
						else if (layBlend == "idiv")
						{
							src_r = src_r == 0 ? 0 : (255 - (((255-d[0]) * 256) / src_r)) < 0 ? 0 : 255 - (((255-d[0]) * 256) / src_r);
							src_g = src_g == 0 ? 0 : (255 - (((255-d[1]) * 256) / src_g)) < 0 ? 0 : 255 - (((255-d[1]) * 256) / src_g);
							src_b = src_b == 0 ? 0 : (255 - (((255-d[2]) * 256) / src_b)) < 0 ? 0 : 255 - (((255-d[2]) * 256) / src_b);
							if (header.color_mode == CM_CMYK)
								src_a = src_a == 0 ? 0 : (255 - (((255-d[3]) * 256) / src_a)) < 0 ? 0 : 255 - (((255-d[3]) * 256) / src_a);
						}
						else if (layBlend == "hue ")
						{
							if (header.color_mode != CM_CMYK)
							{
								uchar new_r = d[0];
								uchar new_g = d[1];
								uchar new_b = d[2];
								RGBTOHSV(src_r, src_g, src_b);
								RGBTOHSV(new_r, new_g, new_b);
								new_r = src_r;
								HSVTORGB(new_r, new_g, new_b);
								src_r = new_r;
								src_g = new_g;
								src_b = new_b;
							}
						}
						else if (layBlend == "sat ")
						{
							if (header.color_mode != CM_CMYK)
							{
								uchar new_r = d[0];
								uchar new_g = d[1];
								uchar new_b = d[2];
								RGBTOHSV(src_r, src_g, src_b);
								RGBTOHSV(new_r, new_g, new_b);
								new_g = src_g;
								HSVTORGB(new_r, new_g, new_b);
								src_r = new_r;
								src_g = new_g;
								src_b = new_b;
							}
						}
						else if (layBlend == "lum ")
						{
							if (header.color_mode != CM_CMYK)
							{
								uchar new_r = d[0];
								uchar new_g = d[1];
								uchar new_b = d[2];
								RGBTOHSV(src_r, src_g, src_b);
								RGBTOHSV(new_r, new_g, new_b);
								new_b = src_b;
								HSVTORGB(new_r, new_g, new_b);
								src_r = new_r;
								src_g = new_g;
								src_b = new_b;
							}
						}
						else if (layBlend == "colr")
						{
							if (header.color_mode != CM_CMYK)
							{
								uchar new_r = d[0];
								uchar new_g = d[1];
								uchar new_b = d[2];
								RGBTOHLS(src_r, src_g, src_b);
								RGBTOHLS(new_r, new_g, new_b);
								new_r = src_r;
								new_b = src_b;
								HLSTORGB(new_r, new_g, new_b);
								src_r = new_r;
								src_g = new_g;
								src_b = new_b;
							}
						}
					}
					if (dst_alpha == 0)
					{
						r = src_r;
						g = src_g;
						b = src_b;
						a = src_a;
					}
					else
					{
						if (src_alpha > 0)
						{
							r = (d[0] * (255 - src_alpha) + src_r * src_alpha) / 255;
							g = (d[1] * (255 - src_alpha) + src_g * src_alpha) / 255;
							b = (d[2] * (255 - src_alpha) + src_b * src_alpha) / 255;
							if (header.color_mode == CM_CMYK)
								a = (d[3] * (255 - src_alpha) + src_a * src_alpha) / 255;
							if (layBlend != "diss")
								src_alpha = dst_alpha + INT_MULT(255 - dst_alpha, src_alpha);
						}
					}
					if (src_alpha > 0)
					{
						d[0] = r;
						d[1] = g;
						d[2] = b;
						if (header.color_mode == CM_CMYK)
						{
							d[3] = a;
							d[4] = src_alpha;
						}
						else
							d[3] = src_alpha;
					}
					s += r2_image.channels();
					d += r_image.channels();
					if (hasMask)
						sm += mask.channels();
				}
			}
		}
	}
	*firstLayer = false;
	return true;
}
