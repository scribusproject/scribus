/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "scstreamfilter_rc4.h"

#include <stdlib.h>
#include "rc4.h"

#define BUFFER_SIZE 16384
struct  ScRC4EncodeFilterData
{
    rc4_context_t rc4_context;
    unsigned char input_buffer [BUFFER_SIZE];
    unsigned char output_buffer[BUFFER_SIZE];
	unsigned int  available_in;
};

ScRC4EncodeFilter::ScRC4EncodeFilter(QDataStream* stream, const char* key, unsigned int keyLen)
				 : ScStreamFilter(stream), m_key(key, keyLen)
{
	m_filterData = NULL;
	m_openedFilter = false;
}

ScRC4EncodeFilter::ScRC4EncodeFilter(ScStreamFilter* filter, const char* key, unsigned int keyLen)
				 : ScStreamFilter(filter), m_key(key, keyLen)
{
	m_filterData = NULL;
	m_openedFilter = false;
}

ScRC4EncodeFilter::~ScRC4EncodeFilter()
{
	if (m_filterData && m_openedFilter)
		closeFilter();
	freeData();
}

void ScRC4EncodeFilter::freeData(void)
{
	if (m_filterData)
		free (m_filterData);
	m_filterData = NULL;
}

bool ScRC4EncodeFilter::openFilter (void)
{
	freeData();

	m_filterData = (ScRC4EncodeFilterData*) malloc(sizeof(ScRC4EncodeFilterData));
	if (m_filterData == NULL)
		return false;

	rc4_init(&m_filterData->rc4_context, (uchar*) m_key.data(),m_key.length());
	m_filterData->available_in  = 0;
	return true;
}

bool ScRC4EncodeFilter::closeFilter(void)
{
	bool closeSucceed = true;
	if (m_filterData->available_in > 0)
	{
		rc4_encrypt(&m_filterData->rc4_context, m_filterData->input_buffer, m_filterData->output_buffer, m_filterData->available_in);
		closeSucceed &= writeDataInternal((const char*) m_filterData->output_buffer, m_filterData->available_in);
	}
	m_openedFilter = false;
	closeSucceed  &= ScStreamFilter::closeFilter();
	return closeSucceed;
}

bool ScRC4EncodeFilter::writeData(const char* data, int dataLen)
{
	bool rc4Success = true;
    unsigned int count;
    const unsigned char *p = (const unsigned char *) data;

	if (!m_filterData)
		return false;

    while (dataLen) {
        count = dataLen;
        if (count > BUFFER_SIZE - m_filterData->available_in)
            count = BUFFER_SIZE - m_filterData->available_in;
        memcpy (m_filterData->input_buffer + m_filterData->available_in, p, count);
        p += count;
        m_filterData->available_in += count;
        dataLen -= count;

        if (m_filterData->available_in == BUFFER_SIZE)
		{
			rc4_encrypt(&m_filterData->rc4_context, m_filterData->input_buffer, m_filterData->output_buffer, m_filterData->available_in);
			rc4Success &= writeDataInternal((const char*) m_filterData->output_buffer, m_filterData->available_in);
			m_filterData->available_in = 0;
		}
    }

	return rc4Success;
}
