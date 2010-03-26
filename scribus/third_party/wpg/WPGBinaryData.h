/* libwpd
 * Copyright (C) 2007 Fridrich Strba (fridrich.strba@bluewin.ch)
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
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WPGBINARYDATA_H
#define WPGBINARYDATA_H

#include "WPGString.h"
#include "WPGRect.h"
#include <stdio.h>

class WPXInputStream;

namespace libwpg
{

class WPGBinaryDataImpl;

class WPGBinaryData
{
public:
	WPGBinaryData();
	WPGBinaryData(const WPGBinaryData &);
	WPGBinaryData(const char *buffer, const size_t bufferSize);
	~WPGBinaryData();

	void append(const WPGBinaryData &data);
	void append(const char *buffer, const size_t bufferSize);
	void append(const char c);
	void clear();
	
	size_t size() const;
	const char *getDataBuffer() const;
	const WPGString getBase64Data() const;

	WPGBinaryData& operator=(const WPGBinaryData &);

	WPGRect rect;
	WPGString mimeType;

private:
	WPGBinaryDataImpl *m_binaryDataImpl;
};

} // namespace libwpg
#endif
