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

#ifndef __WPGBITMAP_H__
#define __WPGBITMAP_H__

#include "WPGRect.h"
#include "WPGColor.h"
#include "WPGString.h"

namespace libwpg
{

class WPGBitmap
{
public:
	WPGRect rect;

	WPGBitmap(int width, int height);
	
	WPGBitmap(int width, int height, bool verticalFlip, bool horizontalFlip);

	WPGBitmap(const WPGBitmap&);

	WPGBitmap& operator=(const WPGBitmap&);

	void copyFrom(const WPGBitmap&);

	~WPGBitmap();

	// return width in pixel
	int width() const;

	// return height in pixel
	int height() const;

	WPGColor pixel(int x, int y) const;

	void setPixel(int x, int y, const WPGColor& color);
	
	void generateBase64DIB(WPGString& bmp) const;

	static void base64Encode(WPGString& base64, const char *data, const int len);

private:
	class Private;
	Private* const d;
	static void writeU16(char *buffer, unsigned &position, const int value);
	static void writeU32(char *buffer, unsigned &position, const int value);
	static void writeU8(char *buffer, unsigned &position, const int value);
};

} // namespace libwpg

#endif // __WPGBITMAP_H__
