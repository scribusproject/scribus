/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "scstreamfilter_ascii85.h"
#include "util.h"

ScASCII85EncodeFilter::ScASCII85EncodeFilter(QDataStream* stream) 
					 : ScStreamFilter(stream)
{
	m_buffer_pending     = 0;
	m_four_tuple_pending = 0;
}

ScASCII85EncodeFilter::ScASCII85EncodeFilter(ScStreamFilter* filter)
					 : ScStreamFilter(filter)
{
	m_buffer_pending     = 0;
	m_four_tuple_pending = 0;
}

bool ScASCII85EncodeFilter::openFilter (void)
{
	m_buffer_pending     = 0;
	m_four_tuple_pending = 0;
	m_buffer.resize(65536);
	if (m_buffer.size() <= 0)
		return false;
	return ScStreamFilter::openFilter();
}

bool ScASCII85EncodeFilter::closeFilter(void)
{
	bool success = true;
	if (m_buffer_pending)
	{
		char* writeBuffer = m_buffer.data();
		writeBuffer[m_buffer_pending] = 0;
		success &= writeDataInternal((const char*) writeBuffer, m_buffer_pending);
	}
	if (m_four_tuple_pending) 
	{
		quint32 value;
		const char* ascii85;
		unsigned char five_tuple[6];
		bool allZero;

		memset (m_four_tuple + m_four_tuple_pending, 0, 4 - m_four_tuple_pending);
		value   = m_four_tuple[0] << 24 | m_four_tuple[1] << 16 | m_four_tuple[2] << 8 | m_four_tuple[3];
		ascii85 = toAscii85(value, allZero);
		memcpy (five_tuple, ascii85, 5);
		five_tuple[m_four_tuple_pending + 1] = 0;
		int five_tuple_len  = strlen((const char* ) five_tuple);
		success &= writeDataInternal((const char* ) five_tuple, five_tuple_len);
	}
	success &= writeDataInternal("~>\n", 3);
	success &= ScStreamFilter::closeFilter();
	return success;
}

bool ScASCII85EncodeFilter::writeData(const char* data, int dataLen)
{
	bool writeSuccess = true;
	int  lineLength = 0;
	bool allZero  = true;
	const char* ascii85;
	quint32 value;

	int dataLenB = dataLen;
	int maxWrite = m_buffer.size() - (m_buffer.size() % 5) - 5;
	if (maxWrite <= 5) return 0;

	const unsigned char *ptr   = (const unsigned char*) data;
	unsigned char* writeBuffer = (unsigned char*) m_buffer.data();
	unsigned char* ptrw = writeBuffer + m_buffer_pending;

	while (dataLen) 
	{
		m_four_tuple[m_four_tuple_pending++] = *ptr++;
		dataLen--;
		if (m_four_tuple_pending == 4) 
		{
			value   = m_four_tuple[0] << 24 | m_four_tuple[1] << 16 | m_four_tuple[2] << 8 | m_four_tuple[3];
			ascii85 = toAscii85(value, allZero);
			if (allZero)
				*ptrw++ = 'z';
			else
			{
				*ptrw++ = ascii85[0];
				*ptrw++ = ascii85[1];
				*ptrw++ = ascii85[2];
				*ptrw++ = ascii85[3];
				*ptrw++ = ascii85[4];
			}
			lineLength       += ((allZero) ? 1 : 5);
			m_buffer_pending += ((allZero) ? 1 : 5);
			if (lineLength > 75)
			{
				*ptrw++ = '\n';
				++m_buffer_pending;
				lineLength = 0;
			}
			if (m_buffer_pending > maxWrite)
			{
				writeSuccess &= writeDataInternal((const char*) writeBuffer, m_buffer_pending);
				ptrw = writeBuffer;
				m_buffer_pending = 0;
			}
			m_four_tuple_pending = 0;
		}
	}
	return writeSuccess;
}
