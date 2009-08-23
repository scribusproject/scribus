/* libwpg
 * Copyright (C) 2004-2005 William Lachance (wrlach@gmail.com)
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
 * For further information visit http://libwpg.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WPGINTERNALSTREAM_H
#define WPGINTERNALSTREAM_H
#include "WPXStream.h"

class WPGInternalInputStream : public WPXInputStream
{
public:
	WPGInternalInputStream(const unsigned char *data, const size_t size);
	virtual ~WPGInternalInputStream();

	virtual bool isOLEStream() { return false; }
	virtual WPXInputStream * getDocumentOLEStream() { return 0; }
	
	const virtual uint8_t *read(size_t numBytes, size_t &numBytesRead);
	virtual int seek(long offset, WPX_SEEK_TYPE seekType);
	virtual long tell();
	virtual bool atEOS();

private:
	long m_offset;
	const size_t m_size;
	const unsigned char *m_data;
	unsigned char *m_tmpBuf;
	WPGInternalInputStream(const WPGInternalInputStream&);
	WPGInternalInputStream& operator=(const WPGInternalInputStream&);
};

#endif
