/* libwpg
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

#ifndef __WPGSTREAMIMPLEMENTATION_H__
#define __WPGSTREAMIMPLEMENTATION_H__

#include "WPXStream.h"

namespace libwpg
{

class WPGFileStreamPrivate;

class WPGFileStream: public WPXInputStream
{
public:
	explicit WPGFileStream(const char* filename);
	~WPGFileStream();
	
	const uint8_t *read(size_t numBytes, size_t &numBytesRead);
	long tell();
	int seek(long offset, WPX_SEEK_TYPE seekType);
	bool atEOS();

	bool isOLEStream();
	WPXInputStream *getDocumentOLEStream(const char * name);
	WPXInputStream *getDocumentOLEStream();

private:
	WPGFileStreamPrivate* d;
	WPGFileStream(const WPGFileStream&); // copy is not allowed
	WPGFileStream& operator=(const WPGFileStream&); // assignment is not allowed
};

class WPGMemoryStreamPrivate;

class WPGMemoryStream: public WPXInputStream
{
public:
	WPGMemoryStream(const char *data, const unsigned int dataSize);
	~WPGMemoryStream();

	const uint8_t *read(size_t numBytes, size_t &numBytesRead);
	long tell();
	int seek(long offset, WPX_SEEK_TYPE seekType);
	bool atEOS();

	bool isOLEStream();
	WPXInputStream *getDocumentOLEStream(const char * name);
	WPXInputStream *getDocumentOLEStream();

private:
	WPGMemoryStreamPrivate* d;
	WPGMemoryStream(const WPGMemoryStream&); // copy is not allowed
	WPGMemoryStream& operator=(const WPGMemoryStream&); // assignment is not allowed
};

} // namespace libwpg

#endif // __WPGSTREAMIMPLEMENTATION_H__
