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
#include "colorutil.h"
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

int ScImgDataLoader_TIFF::getLayers(const QString& fn)
{
	int layerNum = 1;
	int test;
	struct PSDLayer lay;
	TIFF* tif = TIFFOpen(fn.local8Bit(), "r");
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

bool ScImgDataLoader_TIFF::getImageData(TIFF* tif, QImage *image, uint widtht, uint heightt, uint size, uint16 photometric, uint16 bitspersample, uint16 samplesperpixel, bool &bilevel, bool &isCMYK)
{
	image->setAlphaBuffer(true);
	uint32 *bits = 0;
	if (photometric == PHOTOMETRIC_SEPARATED)
	{
		if (samplesperpixel > 4)  // we can't handle CMYKA yet
		{
			bits = (uint32 *) _TIFFmalloc(size * sizeof(uint32));
			if(bits)
			{
				if (TIFFReadRGBAImage(tif, widtht, heightt, bits, 0))
				{
					for(unsigned int y = 0; y < heightt; y++)
						memcpy(image->scanLine(heightt - 1 - y), bits + y * widtht, widtht * 4);
				}
				_TIFFfree(bits);
				if (bitspersample == 1)
					bilevel = true;
				isCMYK = false;
			}
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
					for (xt = 0; xt < (uint) image->width(); xt += columns)
					{
						TIFFReadTile(tif, tile_buf, xt, yt, 0, 0);
						for (yi = 0; yi < tileH; yi++)
							_TIFFmemcpy(image->scanLine(yt+(tileH-1-yi))+xt, tile_buf+tileW*yi, tileW*4);
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
							memcpy(image->scanLine(y), bits, widtht * 4);
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
		bits = (uint32 *) _TIFFmalloc(size * sizeof(uint32));
		if(bits)
		{
			if (TIFFReadRGBAImage(tif, widtht, heightt, bits, 0))
			{
				for(unsigned int y = 0; y < heightt; y++)
					memcpy(image->scanLine(heightt - 1 - y), bits + y * widtht, widtht * 4);
			}
			_TIFFfree(bits);
			if (bitspersample == 1)
				bilevel = true;
		}
	}
	return true;
}

void ScImgDataLoader_TIFF::blendOntoTarget(QImage *tmp, int layOpa, QString layBlend, bool cmyk)
{
	if ((layBlend == "diss") && (!cmyk))
	{
		for (int l = 0; l < tmp->height(); l++)
		{
			srand(random_table[ l  % 4096]);
			for (int k = 0; k < tmp->width(); k++)
			{
				int rand_val = rand() & 0xff;
				QRgb pixel = tmp->pixel(k, l);
				if (rand_val > layOpa)
					tmp->setPixel(k, l, qRgba(qRed(pixel), qGreen(pixel), qBlue(pixel), 0));
			}
		}
	}
	int w = m_image.width();
	int h = m_image.height();
	for( int yi=0; yi < h; ++yi )
	{
		QRgb *src = (QRgb*)(tmp->scanLine( yi ));
		QRgb *dst = (QRgb*)(m_image.scanLine( yi ));
		unsigned char r, g, b, a, src_r, src_g, src_b, src_a;
		for(int xi=0; xi < w; ++xi )
		{
			unsigned char *d = (unsigned char *) dst;
			unsigned char *s = (unsigned char *) src;
			src_r = s[0];
			src_g = s[1];
			src_b = s[2];
			src_a = s[3];
			if (layBlend == "mul ")
			{
				if (cmyk)
				{
					src_r = INT_MULT(src_r, d[0]);
					src_g = INT_MULT(src_g, d[1]);
					src_b = INT_MULT(src_b, d[2]);
					src_a = INT_MULT(src_a, d[3]);
				}
				else
				{
					if (d[3] > 0)
					{
						src_r = INT_MULT(src_r, d[0]);
						src_g = INT_MULT(src_g, d[1]);
						src_b = INT_MULT(src_b, d[2]);
					}
				}
			}
			else if (layBlend == "scrn")
			{
				if (cmyk)
				{
					src_r = 255 - ((255-src_r) * (255-d[0]) / 128);
					src_g = 255 - ((255-src_g) * (255-d[1]) / 128);
					src_b = 255 - ((255-src_b) * (255-d[2]) / 128);
					src_a = 255 - ((255-src_a) * (255-d[3]) / 128);
				}
				else
				{
					if (d[3] > 0)
					{
						src_r = 255 - ((255-src_r) * (255-d[0]) / 128);
						src_g = 255 - ((255-src_g) * (255-d[1]) / 128);
						src_b = 255 - ((255-src_b) * (255-d[2]) / 128);
					}
				}
			}
			else if (layBlend == "over")
			{
				if (cmyk)
				{
					src_r = d[0] < 128 ? src_r * d[0] / 128 : 255 - ((255-src_r) * (255-d[0]) / 128);
					src_g = d[1] < 128 ? src_g * d[1] / 128 : 255 - ((255-src_g) * (255-d[1]) / 128);
					src_b = d[2] < 128 ? src_b * d[2] / 128 : 255 - ((255-src_b) * (255-d[2]) / 128);
					src_a = d[3] < 128 ? src_a * d[3] / 128 : 255 - ((255-src_a) * (255-d[3]) / 128);
				}
				else
				{
					if (d[3] > 0)
					{
						src_r = d[0] < 128 ? src_r * d[0] / 128 : 255 - ((255-src_r) * (255-d[0]) / 128);
						src_g = d[1] < 128 ? src_g * d[1] / 128 : 255 - ((255-src_g) * (255-d[1]) / 128);
						src_b = d[2] < 128 ? src_b * d[2] / 128 : 255 - ((255-src_b) * (255-d[2]) / 128);
					}
				}
			}
			else if (layBlend == "diff")
			{
				if (cmyk)
				{
					src_r = d[0] > src_r ? d[0] - src_r : src_r - d[0];
					src_g = d[1] > src_g ? d[1] - src_g : src_g - d[1];
					src_b = d[2] > src_b ? d[2] - src_b : src_b - d[2];
					src_a = d[3] > src_a ? d[3] - src_a : src_a - d[3];
				}
				else
				{
					if (d[3] > 0)
					{
						src_r = d[0] > src_r ? d[0] - src_r : src_r - d[0];
						src_g = d[1] > src_g ? d[1] - src_g : src_g - d[1];
						src_b = d[2] > src_b ? d[2] - src_b : src_b - d[2];
					}
				}
			}
			else if (layBlend == "dark")
			{
				if (cmyk)
				{
					src_r = d[0]  < src_r ? d[0]  : src_r;
					src_g = d[1] < src_g ? d[1] : src_g;
					src_b = d[2] < src_b ? d[2] : src_b;
					src_a = d[3] < src_a ? d[3] : src_a;
				}
				else
				{
					if (d[3] > 0)
					{
						src_r = d[0]  < src_r ? d[0]  : src_r;
						src_g = d[1] < src_g ? d[1] : src_g;
						src_b = d[2] < src_b ? d[2] : src_b;
					}
				}
			}
			else if (layBlend == "hLit")
			{
				if (cmyk)
				{
					src_r = src_r < 128 ? src_r * d[0] / 128 : 255 - ((255-src_r) * (255-d[0]) / 128);
					src_g = src_g < 128 ? src_g * d[1] / 128 : 255 - ((255-src_g) * (255-d[1]) / 128);
					src_b = src_b < 128 ? src_b * d[2] / 128 : 255 - ((255-src_b) * (255-d[2]) / 128);
					src_a = src_a < 128 ? src_a * d[3] / 128 : 255 - ((255-src_a) * (255-d[3]) / 128);
				}
				else
				{
					if (d[3] > 0)
					{
						src_r = src_r < 128 ? src_r * d[0] / 128 : 255 - ((255-src_r) * (255-d[0]) / 128);
						src_g = src_g < 128 ? src_g * d[1] / 128 : 255 - ((255-src_g) * (255-d[1]) / 128);
						src_b = src_b < 128 ? src_b * d[2] / 128 : 255 - ((255-src_b) * (255-d[2]) / 128);
					}
				}
			}
			else if (layBlend == "sLit")
			{
				if (cmyk)
				{
					src_r = src_r * d[0] / 256 + src_r * (255 - ((255-src_r)*(255-d[0]) / 256) - src_r * d[0] / 256) / 256;
					src_g = src_g * d[1] / 256 + src_g * (255 - ((255-src_g)*(255-d[1]) / 256) - src_g * d[1] / 256) / 256;
					src_b = src_b * d[2] / 256 + src_b * (255 - ((255-src_b)*(255-d[2]) / 256) - src_b * d[2] / 256) / 256;
					src_a = src_a * d[3] / 256 + src_a * (255 - ((255-src_a)*(255-d[3]) / 256) - src_a * d[3] / 256) / 256;
				}
				else
				{
					if (d[3] > 0)
					{
						src_r = src_r * d[0] / 256 + src_r * (255 - ((255-src_r)*(255-d[0]) / 256) - src_r * d[0] / 256) / 256;
						src_g = src_g * d[1] / 256 + src_g * (255 - ((255-src_g)*(255-d[1]) / 256) - src_g * d[1] / 256) / 256;
						src_b = src_b * d[2] / 256 + src_b * (255 - ((255-src_b)*(255-d[2]) / 256) - src_b * d[2] / 256) / 256;
					}
				}
			}
			else if (layBlend == "lite")
			{
				if (cmyk)
				{
					src_r = d[0] < src_r ? src_r : d[0];
					src_g = d[1] < src_g ? src_g : d[1];
					src_b = d[2] < src_b ? src_b : d[2];
					src_a = d[3] < src_a ? src_a : d[3];
				}
				else
				{
					if (d[3] > 0)
					{
						src_r = d[0] < src_r ? src_r : d[0];
						src_g = d[1] < src_g ? src_g : d[1];
						src_b = d[2] < src_b ? src_b : d[2];
					}
				}
			}
			else if (layBlend == "smud")
			{
				if (cmyk)
				{
					src_r = d[0] + src_r - src_r * d[0] / 128;
					src_g = d[1] + src_g - src_g * d[1] / 128;
					src_b = d[2] + src_b - src_b * d[2] / 128;
					src_a = d[3] + src_a - src_a * d[3] / 128;
				}
				else
				{
					if (d[3] > 0)
					{
						src_r = d[0] + src_r - src_r * d[0] / 128;
						src_g = d[1] + src_g - src_g * d[1] / 128;
						src_b = d[2] + src_b - src_b * d[2] / 128;
					}
				}
			}
			else if (layBlend == "div ")
			{
				if (cmyk)
				{
					src_r = src_r == 255 ? 255 : ((d[0] * 256) / (255-src_r)) > 255 ? 255 : (d[0] * 256) / (255-src_r);
					src_g = src_g == 255 ? 255 : ((d[1] * 256) / (255-src_g)) > 255 ? 255 : (d[1] * 256) / (255-src_g);
					src_b = src_b == 255 ? 255 : ((d[2] * 256) / (255-src_b)) > 255 ? 255 : (d[2] * 256) / (255-src_b);
					src_a = src_a == 255 ? 255 : ((d[3] * 256) / (255-src_a)) > 255 ? 255 : (d[3] * 256) / (255-src_a);
				}
				else
				{
					if (d[3] > 0)
					{
						src_r = src_r == 255 ? 255 : ((d[0] * 256) / (255-src_r)) > 255 ? 255 : (d[0] * 256) / (255-src_r);
						src_g = src_g == 255 ? 255 : ((d[1] * 256) / (255-src_g)) > 255 ? 255 : (d[1] * 256) / (255-src_g);
						src_b = src_b == 255 ? 255 : ((d[2] * 256) / (255-src_b)) > 255 ? 255 : (d[2] * 256) / (255-src_b);
					}
				}
			}
			else if (layBlend == "idiv")
			{
				if (cmyk)
				{
					src_r = src_r == 0 ? 0 : (255 - (((255-d[0]) * 256) / src_r)) < 0 ? 0 : 255 - (((255-d[0]) * 256) / src_r);
					src_g = src_g == 0 ? 0 : (255 - (((255-d[1]) * 256) / src_g)) < 0 ? 0 : 255 - (((255-d[1]) * 256) / src_g);
					src_b = src_b == 0 ? 0 : (255 - (((255-d[2]) * 256) / src_b)) < 0 ? 0 : 255 - (((255-d[2]) * 256) / src_b);
					src_a = src_a == 0 ? 0 : (255 - (((255-d[3]) * 256) / src_a)) < 0 ? 0 : 255 - (((255-d[3]) * 256) / src_a);
				}
				else
				{
					if (d[3] > 0)
					{
						src_r = src_r == 0 ? 0 : (255 - (((255-d[0]) * 256) / src_r)) < 0 ? 0 : 255 - (((255-d[0]) * 256) / src_r);
						src_g = src_g == 0 ? 0 : (255 - (((255-d[1]) * 256) / src_g)) < 0 ? 0 : 255 - (((255-d[1]) * 256) / src_g);
						src_b = src_b == 0 ? 0 : (255 - (((255-d[2]) * 256) / src_b)) < 0 ? 0 : 255 - (((255-d[2]) * 256) / src_b);
					}
				}
			}
			else if (layBlend == "hue ")
			{
				if (!cmyk)
				{
					if (d[3] > 0)
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
			}
			else if (layBlend == "sat ")
			{
				if (!cmyk)
				{
					if (d[3] > 0)
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
			}
			else if (layBlend == "lum ")
			{
				if (!cmyk)
				{
					if (d[3] > 0)
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
			}
			else if (layBlend == "colr")
			{
				if (!cmyk)
				{
					if (d[3] > 0)
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
			int layOpa2 = layOpa;
			if (layBlend != "diss")
			{
				if (!cmyk)
					src_a = INT_MULT(src_a, layOpa);
			}
			else
				layOpa2 = 255;
			if (d[3] > 0)
			{
				r = (d[0] * (255 - layOpa) + src_r * layOpa2) / 255;
				g = (d[1] * (255 - layOpa) + src_g * layOpa2) / 255;
				b = (d[2] * (255 - layOpa) + src_b * layOpa2) / 255;
				if (cmyk)
					a = (d[3] * (255 - layOpa) + src_a * layOpa2) / 255;
				else
				{
					a = d[3] + INT_MULT(255 - d[3], src_a);
					r = (d[0] * (255 - src_a) + src_r * src_a) / 255;
					g = (d[1] * (255 - src_a) + src_g * src_a) / 255;
					b = (d[2] * (255 - src_a) + src_b * src_a) / 255;
				}
			}
			else
			{
				r = src_r;
				g = src_g;
				b = src_b;
				a = src_a;
			}
			if (cmyk)
			{
				d[0] = r;
				d[1] = g;
				d[2] = b;
				d[3] = a;
			}
			else
			{
				if (src_a > 0)
				{
					d[0] = r;
					d[1] = g;
					d[2] = b;
					d[3] = a;
				}
			}
			dst++;
			src++;
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
	m_imageInfoRecord.type = 1;
	getLayers(fn);
	TIFF* tif = TIFFOpen(fn.local8Bit(), "r");
	if(tif)
	{
		bool isCMYK = false;
		unsigned int widtht, heightt, size;
		char *description=0, *copyright=0, *datetime=0, *artist=0, *scannerMake=0, *scannerModel=0;
		uint16 photometric, bitspersample, samplesperpixel, fillorder;

		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &widtht);
		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &heightt);
		TIFFGetField(tif, TIFFTAG_XRESOLUTION, &xres);
		TIFFGetField(tif, TIFFTAG_YRESOLUTION, &yres);
		TIFFGetField(tif, TIFFTAG_RESOLUTIONUNIT , &resolutionunit);
		size = widtht * heightt;
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
				fakeHeader.width = widtht;
				fakeHeader.height = heightt;
				parseRessourceData(strPhot, fakeHeader, PhotoshopLen);
				m_imageInfoRecord.exifInfo.width = widtht;
				m_imageInfoRecord.exifInfo.height = heightt;
				if (!m_imageInfoRecord.valid)
					m_imageInfoRecord.valid = (m_imageInfoRecord.PDSpathData.size())>0?true:false;
			}
		}
/*		PhotoshopLen = 0;
		if (TIFFGetField(tif, 37724, &PhotoshopLen, &PhotoshopBuffer) )
		{
			if (PhotoshopLen != 0)
			{
				QByteArray arrayPhot(PhotoshopLen);
				arrayPhot.duplicate((const char*)PhotoshopBuffer,PhotoshopLen);
				QDataStream s(arrayPhot,IO_ReadOnly);
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
					s >> signature;
				}
				while (signature != 0x4c617972);
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
						if (numChannels > 5)	// we don't support images with more than 5 channels yet
						{
							m_imageInfoRecord.layerInfo.clear();
							return false;
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
								return false;
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
						s.device()->at( s.device()->at() + layerRange );
						lay.layerName = getLayerString(s);
						m_imageInfoRecord.layerInfo.append(lay);
						s >> signature;
						if( signature == 0x3842494D )
						{
							while (signature == 0x3842494D )
							{
								s >> signature;
								s >> addRes;
								s.device()->at( s.device()->at() + addRes );
								s >> signature;
							}
							s.device()->at( s.device()->at() - 4 );
						}
						else
						{
							s.device()->at( s.device()->at() - 2 );
							s >> signature;
							if( signature == 0x3842494D )
							{
								while (signature == 0x3842494D )
								{
									s >> signature;
									s >> addRes;
									s.device()->at( s.device()->at() + addRes );
									s >> signature;
								}
								s.device()->at( s.device()->at() - 4 );
							}
							else
								s.device()->at( s.device()->at() - 6 );
						}
					}
				}
				uint base = s.device()->at();
				ushort compression;
				s >> compression;
			}
		} */
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
		do
		{
			QImage tmpImg = QImage();
			if( !tmpImg.create(widtht,heightt,32))
			{
				TIFFClose(tif);
				return false;
			}
			else
			{
				if (!getImageData(tif, &tmpImg, widtht, heightt, size, photometric, bitspersample, samplesperpixel, bilevel, isCMYK))
				{
					TIFFClose(tif);
					return false;
				}
				bool visible = true;
				if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layerNum)))
					visible = m_imageInfoRecord.RequestProps[layerNum].visible;
				QString layBlend = "norm";
				if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layerNum)))
					layBlend = m_imageInfoRecord.RequestProps[layerNum].blend;
				int layOpa = 255;
				if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layerNum)))
					layOpa = m_imageInfoRecord.RequestProps[layerNum].opacity;
				if (visible)
					blendOntoTarget(&tmpImg, layOpa, layBlend, isCMYK);
				QImage imt = tmpImg.copy();
				swapRGBA(&imt);
				double sx = tmpImg.width() / 40.0;
				double sy = tmpImg.height() / 40.0;
				if (isCMYK)
				{
					for (int i = 0; i < imt.height(); i++)
					{
						unsigned int *ptr2 = (unsigned int *) imt.scanLine(i);
						unsigned char r, g, b;
						for (int j = 0; j < imt.width(); j++)
						{
							unsigned char *p2 = (unsigned char *) ptr2;
							r = 255 - QMIN(255, p2[0] + p2[3]);
							g = 255 - QMIN(255, p2[1] + p2[3]);
							b = 255 - QMIN(255, p2[2] + p2[3]);
							p2[0] = r;
							p2[1] = g;
							p2[2] = b;
							p2[3] = 255;
							ptr2++;
						}
					}
				}
				imt = sy < sx ?	imt.smoothScale(qRound(imt.width() / sx), qRound(imt.height() / sx)) :
											imt.smoothScale(qRound(imt.width() / sy), qRound(imt.height() / sy));
				m_imageInfoRecord.layerInfo[layerNum].thumb = imt.copy();
				layerNum++;
			}
			test = TIFFReadDirectory(tif);
		}
		while (test == 1);
		swapRGBA();
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
//		if (layerNum == 1)
//			m_imageInfoRecord.layerInfo.clear();
		m_imageInfoRecord.BBoxX = 0;
		m_imageInfoRecord.BBoxH = m_image.height();
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
	s.device()->at( s.device()->at() + adj );
	return ret;
}
