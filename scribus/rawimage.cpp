/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "rawimage.h"

RawImage::RawImage()
{
	m_width = 0;
	m_height = 0;
	m_channels = 0;
	resize(0);
}

RawImage::RawImage( int width, int height, int channels )
{
	create(width, height, channels);
}

RawImage::~RawImage()
{
	resize(0);
}

bool RawImage::create( int width, int height, int channels )
{
	m_width = width;
	m_height = height;
	m_channels = channels;
	int finalSize=width * height * channels;
	resize(finalSize);
	return (size()==finalSize);
}

uchar *RawImage::scanLine(int row)
{
	if (row < m_height)
		return (uchar*)(data() + (row * m_channels * m_width));
	else
		return (uchar*)data();
}

void RawImage::setAlpha(int x, int y, int alpha)
{
	uchar *d;
	if ((y < m_height) && (x < m_width))
	{
		d = (uchar*)(data() + (y * m_channels * m_width) + (x * m_channels));
		d[m_channels-1] = alpha;
	}
}

QImage RawImage::convertToQImage(bool cmyk, bool raw)
{
	int chans = channels();
	QImage img = QImage(width(), height(), QImage::Format_ARGB32);
	QRgb *ptr;
	uchar *src;
	uchar cr, cg, cb, ck, ca;
//	img.create(width(), height(), 32);
	if (raw)
	{
		for (int i = 0; i < height(); i++)
		{
			ptr = (QRgb *)img.scanLine(i);
			src = scanLine(i);
			for (int j = 0; j < width(); j++)
			{
				*ptr++ = qRgba(src[0],src[1],src[2],src[3]);
				src += chans;
			}
		}
	}
	else
	{
//		img.setAlphaBuffer( true );
		for (int i = 0; i < height(); i++)
		{
			ptr = (QRgb *)img.scanLine(i);
			src = scanLine(i);
			for (int j = 0; j < width(); j++)
			{
				if (chans > 1)
				{
					if (cmyk)
					{
						ck = src[3];
						cr = 255 - qMin(255, src[0] + ck);
						cg = 255 - qMin(255, src[1] + ck);
						cb = 255 - qMin(255, src[2] + ck);
						if (chans > 4)
						{
							ca = src[4];
							*ptr++ = qRgba(cr,cg,cb,ca);
						}
						else
							*ptr++ = qRgba(cr,cg,cb,255);
					}
					else
					{
						if (chans > 3)
							*ptr++ = qRgba(src[0],src[1],src[2],src[3]);
						else
							*ptr++ = qRgba(src[0],src[1],src[2],255);
					}
				}
				else
					*ptr++ = qRgba(src[0],src[0],src[0],255);
				src += chans;
			}
		}
	}
	return img;
}
