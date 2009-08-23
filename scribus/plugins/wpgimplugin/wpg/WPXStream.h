/* libwpd
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
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WPXSTREAM_H
#define WPXSTREAM_H
#include <stdio.h>
#include "libwpd_types.h"

enum WPX_SEEK_TYPE
{
	WPX_SEEK_CUR,
	WPX_SEEK_SET
};

class WPXInputStream
{
public:
	WPXInputStream(bool supportsOLE) :
		m_supportsOLE(supportsOLE) {}
	virtual ~WPXInputStream() {}

	/**
	Analyses the content of the input stream to see whether it is an OLE2 storage.
	\return A boolean value that should be true if the input stream is an OLE2 storage
	and false if it is not the case
	*/
	virtual bool isOLEStream() = 0;
	/**
	Extracts a \c PerfectOffice_MAIN stream from an OLE2 storage.
	\return Should be a pointer to WPXInputStream constructed from the \c PerfectOffice_MAIN stream if it exists.
	\return Should be 0, if the \c PerfectOffice_MAIN stream does not exist inside the OLE2 storage
	or if the input stream is not an OLE2 storage.
	*/
	virtual WPXInputStream * getDocumentOLEStream() = 0;

	/**
	Tries to read a given number of bytes starting from the current position inside the input stream.
	\param numBytes Number of bytes desired to be read.
	\param numBytesRead Number of bytes that were possible to be read.
	\return Should be a pointer to an array of numBytesRead bytes (uint8_t[numBytesRead]).
	\return Optionally it could be 0 if the desired number of bytes could not be read.
	*/
	const virtual uint8_t *read(size_t numBytes, size_t &numBytesRead) = 0;
	/**
	Moves to the next location inside the input stream.
	\param offset The offset of the location inside the input stream to move to.
	It is relative either to the current position or to the beginning of the input stream
	depending on the value of the \c seekType parameter.
	\param seekType Determines whether the \c offset is relative to the
	beginning of the input stream (\c WPX_SEEK_SET) or to the current position (\c WPX_SEEK_CUR).
	\return An integer value that should be 0 (zero) if the seek was successful and any other value
	if it failed (i.e. the requested \c offset is beyond the end of the input stream or before its beginning).
	*/
	virtual int seek(long offset, WPX_SEEK_TYPE seekType) = 0;
	/**
	Returns the actual position inside the input stream.
	\return A long integer value that should correspond to the position of the next location to be read in the input stream.
	*/
	virtual long tell() = 0;
	/**
	Determines whether the current position is at the end of the stream.
	\return A boolean value that should be true if the next location to be read in the input stream
	is beyond its end. In all other cases, it should be false.
	*/
	virtual bool atEOS() = 0;

private:
	bool m_supportsOLE;
};
#endif
