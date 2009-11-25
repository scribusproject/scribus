/* libwpg
 * Copyright (C) 2007 Ariya Hidayat (ariya@kde.org)
 * Copyright (C) 2006 Ariya Hidayat (ariya@kde.org)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02111-1301 USA
 *
 * For further information visit http://libwpg.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WPGBitmap.h"
#include "libwpg_utils.h"

#include <sstream>

#define DUMP_BITMAP 0

#if DUMP_BITMAP
static unsigned bitmapId = 0;
#endif

class libwpg::WPGBitmap::Private
{
public:
	int width;
	int height;
	bool vFlip;
	bool hFlip;
	WPGColor* pixels;

	Private(int w, int h): width(w), height(h), vFlip(false), hFlip(false), pixels(0) {}
};

libwpg::WPGBitmap::WPGBitmap(int width, int height): rect(), d(new Private(width, height))
{
	d->pixels = new WPGColor[width*height];
}

libwpg::WPGBitmap::WPGBitmap(int width, int height, bool verticalFlip, bool horizontalFlip) :
	rect(),
	d(new Private(width, height))
{
	d->vFlip = verticalFlip;
	d->hFlip = horizontalFlip;
	d->pixels = new WPGColor[width*height];
}

libwpg::WPGBitmap::~WPGBitmap()
{
	if (d)
	{
		if (d->pixels)
			delete [] d->pixels;
		delete d;
	}
}

libwpg::WPGBitmap::WPGBitmap(const WPGBitmap& bitmap): rect(), d(new Private(0,0))
{
	copyFrom(bitmap);
}

libwpg::WPGBitmap& libwpg::WPGBitmap::operator=(const WPGBitmap& bitmap)
{
	copyFrom(bitmap);
	return *this;
}

void libwpg::WPGBitmap::copyFrom(const WPGBitmap& bitmap)
{
	rect = bitmap.rect;
	d->width = bitmap.d->width;
	d->height = bitmap.d->height;
	delete [] d->pixels;
	d->pixels = new WPGColor[d->width*d->height];
	for(int i=0; i < d->width*d->height; i++)
		d->pixels[i] = bitmap.d->pixels[i];
}

int libwpg::WPGBitmap::width() const
{
	return d->width;
}

int libwpg::WPGBitmap::height() const
{
	return d->height;
}

libwpg::WPGColor libwpg::WPGBitmap::pixel(int x, int y) const
{
  if((x < 0) || (y <0) || (x >= d->width) || (y >= d->height))
		return libwpg::WPGColor();

	return d->pixels[y*d->width + x];
}

void libwpg::WPGBitmap::setPixel(int x, int y, const libwpg::WPGColor& color)
{
	if((x < 0) || (y <0) || (x >= d->width) || (y >= d->height))
		return;

	d->pixels[y*d->width + x] = color;
}

void libwpg::WPGBitmap::generateBase64DIB(libwpg::WPGString& bmp) const
{
	if (d->height <= 0 || d->width <= 0)
		return;

	unsigned tmpPixelSize = (unsigned)(d->height * d->width);
	if (tmpPixelSize < (unsigned)d->height) // overflow
		return;

	unsigned tmpBufferPosition = 0;
	unsigned tmpDIBImageSize = tmpPixelSize * 4;
	if (tmpPixelSize > tmpDIBImageSize) // overflow !!!
		return;

	unsigned tmpDIBOffsetBits = 14 + 40;
	unsigned tmpDIBFileSize = tmpDIBOffsetBits + tmpDIBImageSize;
	if (tmpDIBImageSize > tmpDIBFileSize) // overflow !!!
		return;

	char *tmpDIBBuffer = new char[tmpDIBFileSize];

	// Create DIB file header
	writeU16(tmpDIBBuffer, tmpBufferPosition, 0x4D42);  // Type
	writeU32(tmpDIBBuffer, tmpBufferPosition, tmpDIBFileSize); // Size
	writeU16(tmpDIBBuffer, tmpBufferPosition, 0); // Reserved1
	writeU16(tmpDIBBuffer, tmpBufferPosition, 0); // Reserved2
	writeU32(tmpDIBBuffer, tmpBufferPosition, tmpDIBOffsetBits); // OffsetBits
	
	WPG_DEBUG_MSG(("WPGBitmap: DIB file header end = %i\n", tmpBufferPosition - 1));

	// Create DIB Info header
	writeU32(tmpDIBBuffer, tmpBufferPosition, 40); // Size
	writeU32(tmpDIBBuffer, tmpBufferPosition, width());  // Width
	writeU32(tmpDIBBuffer, tmpBufferPosition, height()); // Height
	writeU16(tmpDIBBuffer, tmpBufferPosition, 1); // Planes
	writeU16(tmpDIBBuffer, tmpBufferPosition, 32); // BitCount 
	writeU32(tmpDIBBuffer, tmpBufferPosition, 0); // Compression
	writeU32(tmpDIBBuffer, tmpBufferPosition, tmpDIBImageSize); // SizeImage
	writeU32(tmpDIBBuffer, tmpBufferPosition, 0); // XPelsPerMeter
	writeU32(tmpDIBBuffer, tmpBufferPosition, 0); // YPelsPerMeter
	writeU32(tmpDIBBuffer, tmpBufferPosition, 0); // ColorsUsed
	writeU32(tmpDIBBuffer, tmpBufferPosition, 0); // ColorsImportant
	
	WPG_DEBUG_MSG(("WPGBitmap: DIB info header end = %i\n", tmpBufferPosition - 1));

	// Write DIB Image data
	
	int i = 0; int j = 0;
	if (d->vFlip)
		for	(i = 0; i < d->height && tmpBufferPosition < tmpDIBFileSize; i++)
		{
			if (d->hFlip)
				for (j = d->width - 1; j >= 0 && tmpBufferPosition < tmpDIBFileSize; j--)
				{
					writeU8(tmpDIBBuffer, tmpBufferPosition, d->pixels[i*d->width + j].blue);
					writeU8(tmpDIBBuffer, tmpBufferPosition, d->pixels[i*d->width + j].green);
					writeU8(tmpDIBBuffer, tmpBufferPosition, d->pixels[i*d->width + j].red);
					writeU8(tmpDIBBuffer, tmpBufferPosition, d->pixels[i*d->width + j].alpha);
				}
			else
				for (int j = 0; j < d->width && tmpBufferPosition < tmpDIBFileSize; j++)
				{
					writeU8(tmpDIBBuffer, tmpBufferPosition, d->pixels[i*d->width + j].blue);
					writeU8(tmpDIBBuffer, tmpBufferPosition, d->pixels[i*d->width + j].green);
					writeU8(tmpDIBBuffer, tmpBufferPosition, d->pixels[i*d->width + j].red);
					writeU8(tmpDIBBuffer, tmpBufferPosition, d->pixels[i*d->width + j].alpha);
				}
		}
	else
		for (int i = d->height - 1; i >= 0 && tmpBufferPosition < tmpDIBFileSize; i--)
		{
			if (d->hFlip)
				for (j = d->width - 1; j >= 0 && tmpBufferPosition < tmpDIBFileSize; j--)
				{
					writeU8(tmpDIBBuffer, tmpBufferPosition, d->pixels[i*d->width + j].blue);
					writeU8(tmpDIBBuffer, tmpBufferPosition, d->pixels[i*d->width + j].green);
					writeU8(tmpDIBBuffer, tmpBufferPosition, d->pixels[i*d->width + j].red);
					writeU8(tmpDIBBuffer, tmpBufferPosition, d->pixels[i*d->width + j].alpha);
				}
			else
				for (int j = 0; j < d->width && tmpBufferPosition < tmpDIBFileSize; j++)
				{
					writeU8(tmpDIBBuffer, tmpBufferPosition, d->pixels[i*d->width + j].blue);
					writeU8(tmpDIBBuffer, tmpBufferPosition, d->pixels[i*d->width + j].green);
					writeU8(tmpDIBBuffer, tmpBufferPosition, d->pixels[i*d->width + j].red);
					writeU8(tmpDIBBuffer, tmpBufferPosition, d->pixels[i*d->width + j].alpha);
				}
		}

	WPG_DEBUG_MSG(("WPGBitmap: DIB file size = %i\n", tmpBufferPosition - 1));

	base64Encode(bmp, tmpDIBBuffer, tmpDIBFileSize);
	
	// temporary for debug - dump the binary bmp (need to have write access in the current directory
#if DUMP_BITMAP
	std::ostringstream filename;
	filename << "binarydump" << bitmapId++ << ".bmp";
	FILE *f = fopen("binarydump.bmp", "wb");
	if (f)
	{
		for (unsigned k = 0; k < tmpDIBFileSize; k++)
			fprintf(f, "%c",tmpDIBBuffer[k]);
		fclose(f);
	}
#endif

	// Cleanup things before returning
	delete [] tmpDIBBuffer;
}

void libwpg::WPGBitmap::writeU16(char *buffer, unsigned &position, const int value)
{
	buffer[position++] = (char)(value & 0xFF);
	buffer[position++] = (char)((value >> 8) & 0xFF);
}

void libwpg::WPGBitmap::writeU32(char *buffer, unsigned &position, const int value)
{
	buffer[position++] = (char)(value & 0xFF);
	buffer[position++] = (char)((value >> 8) & 0xFF);
	buffer[position++] = (char)((value >> 16) & 0xFF);
	buffer[position++] = (char)((value >> 24) & 0xFF);
}

void libwpg::WPGBitmap::writeU8(char *buffer, unsigned &position, const int value)
{
	buffer[position++] = (char)(value & 0xFF);
}

void libwpg::WPGBitmap::base64Encode(WPGString& base64, const char *data, const int len)
{
	static const char* base64Chars =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	char tempCharsToEncode[3];
	int i = 0; int j = 0; int modifiedLen;
	if (len % 3)
		modifiedLen = 3 * ((int)(len / 3) + 1);
	else
		modifiedLen = len;
	
	bool shouldIexit = false;	
	for (; i < modifiedLen; i++)
	{
		if (i < len)
			tempCharsToEncode[j++] = data[i];
		else
		{
			tempCharsToEncode[j++] = '\0';
			shouldIexit = true;
		}
		if (shouldIexit)
		{
			if (j == 3)
			{
				base64.append(base64Chars[(tempCharsToEncode[0] & 0xfc) >> 2]);
				base64.append(base64Chars[((tempCharsToEncode[0] & 0x03) << 4) | ((tempCharsToEncode[1] & 0xf0) >> 4)]);
				base64.append(base64Chars[((tempCharsToEncode[1] & 0x0f) << 2) | ((tempCharsToEncode[2] & 0xc0) >> 6)]);
				base64.append('=');
				break;
			}
			if (j == 2)
			{
				base64.append(base64Chars[(tempCharsToEncode[0] & 0xfc) >> 2]);
				base64.append(base64Chars[((tempCharsToEncode[0] & 0x03) << 4) | ((tempCharsToEncode[1] & 0xf0) >> 4)]);
				base64.append('='); base64.append('=');
				break;
			}
		}
		else if (j == 3)
		{
			base64.append(base64Chars[(tempCharsToEncode[0] & 0xfc) >> 2]);
			base64.append(base64Chars[((tempCharsToEncode[0] & 0x03) << 4) | ((tempCharsToEncode[1] & 0xf0) >> 4)]);
			base64.append(base64Chars[((tempCharsToEncode[1] & 0x0f) << 2) | ((tempCharsToEncode[2] & 0xc0) >> 6)]);
			base64.append(base64Chars[tempCharsToEncode[2] & 0x3f]);
			j = 0;
		}
	}
}
