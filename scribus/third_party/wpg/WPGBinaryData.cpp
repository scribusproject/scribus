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

#include "WPGBinaryData.h"
#include "WPGInternalStream.h"
#include "WPGBitmap.h"

#include <vector>
#include <stdarg.h>
#include <stdio.h>

class libwpg::WPGBinaryDataImpl
{
public:
	WPGBinaryDataImpl() : m_buf() {}
	std::vector<char> m_buf;
};

libwpg::WPGBinaryData::~WPGBinaryData()
{
	delete m_binaryDataImpl;
}

libwpg::WPGBinaryData::WPGBinaryData() :
	rect(),
	mimeType(),
	m_binaryDataImpl(new WPGBinaryDataImpl)
{
}

libwpg::WPGBinaryData::WPGBinaryData(const WPGBinaryData &data) :
	m_binaryDataImpl(new WPGBinaryDataImpl)
{
	m_binaryDataImpl->m_buf = data.m_binaryDataImpl->m_buf;
}

libwpg::WPGBinaryData::WPGBinaryData(const char *buffer, const size_t bufferSize) :
	m_binaryDataImpl(new WPGBinaryDataImpl)
{
	m_binaryDataImpl->m_buf = std::vector<char> (bufferSize);
	for (size_t i = 0; i < bufferSize; i++)
		m_binaryDataImpl->m_buf[i] = buffer[i];
}

void libwpg::WPGBinaryData::append(const WPGBinaryData &data)
{
	size_t previousSize = m_binaryDataImpl->m_buf.size();
	m_binaryDataImpl->m_buf.resize(previousSize + data.m_binaryDataImpl->m_buf.size());
	for (size_t i = previousSize; i < previousSize + data.m_binaryDataImpl->m_buf.size(); i++)
		m_binaryDataImpl->m_buf[previousSize + i] = data.m_binaryDataImpl->m_buf[i];
}

void libwpg::WPGBinaryData::append(const char *buffer, const size_t bufferSize )
{
	size_t previousSize = m_binaryDataImpl->m_buf.size();
	m_binaryDataImpl->m_buf.resize(previousSize + bufferSize);
	for (size_t i = previousSize; i < previousSize + bufferSize; i++)
		m_binaryDataImpl->m_buf[previousSize + i] = buffer[i];
}

void libwpg::WPGBinaryData::append(const char c)
{
	m_binaryDataImpl->m_buf.push_back(c);
}

void libwpg::WPGBinaryData::clear()
{
	m_binaryDataImpl->m_buf.clear();
}

size_t libwpg::WPGBinaryData::size() const
{ 
	return (size_t)m_binaryDataImpl->m_buf.size(); 
}

libwpg::WPGBinaryData& libwpg::WPGBinaryData::operator=(const WPGBinaryData &dataBuf)
{
	m_binaryDataImpl->m_buf = dataBuf.m_binaryDataImpl->m_buf;
	return *this;
}

const char * libwpg::WPGBinaryData::getDataBuffer() const
{
	return &(m_binaryDataImpl->m_buf[0]);
}

const libwpg::WPGString libwpg::WPGBinaryData::getBase64Data() const
{
	WPGString base64;	

	libwpg::WPGBitmap::base64Encode(base64, &(m_binaryDataImpl->m_buf[0]), m_binaryDataImpl->m_buf.size());
	return base64;
}
