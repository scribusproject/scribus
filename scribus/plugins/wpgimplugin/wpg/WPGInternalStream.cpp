/* libwpg
 * Copyright (C) 2004-2005 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WPGInternalStream.h"

WPGInternalInputStream::WPGInternalInputStream(const unsigned char *data, const size_t size) :
	WPXInputStream(false),
	m_offset(0),
	m_size(size),
	m_data(data),
	m_tmpBuf(0)
{
}

WPGInternalInputStream::~WPGInternalInputStream()
{
	if (m_tmpBuf)
		delete [] m_tmpBuf;
}

const uint8_t * WPGInternalInputStream::read(size_t numBytes, size_t &numBytesRead)
{
	numBytesRead = 0;
	if (0 == numBytes)
		return 0;

	if (m_tmpBuf)
		delete [] m_tmpBuf;
	m_tmpBuf = 0;
	int numBytesToRead;

	if ((m_offset+numBytes) < m_size)
		numBytesToRead = numBytes;
	else
		numBytesToRead = m_size - m_offset;
	
	numBytesRead = numBytesToRead; // about as paranoid as we can be..

	if (numBytesToRead == 0)
		return 0;

	m_tmpBuf = new uint8_t[numBytesToRead];
	for (size_t i=0; (long)i<(long)numBytesToRead; i++)
	{
		m_tmpBuf[i] = m_data[m_offset];
		m_offset++;
	}

	return m_tmpBuf;
}

int WPGInternalInputStream::seek(long offset, WPX_SEEK_TYPE seekType)
{
	if (seekType == WPX_SEEK_CUR)
		m_offset += offset;
	else if (seekType == WPX_SEEK_SET)
		m_offset = offset;

	if (m_offset < 0)
	{
		m_offset = 0;
		return 1;
	}
	if ((long)m_offset > (long)m_size)
	{
		m_offset = m_size;
		return 1;
	}

	return 0;
}

long WPGInternalInputStream::tell()
{
	return m_offset;
}

bool WPGInternalInputStream::atEOS()
{
	if ((long)m_offset == (long)m_size) 
		return true; 

	return false;
}
