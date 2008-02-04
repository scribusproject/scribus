/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "scstreamfilter.h"

#include <cassert>
#include <QByteArray>
#include <QDataStream>
using namespace std;

ScStreamFilter::ScStreamFilter(QDataStream* stream)
{
	assert(stream != NULL);
	m_dataStream = stream;
	m_filter     = NULL;
	m_filterMode = FilterToStream;
	m_writtenToStream = 0;
}

ScStreamFilter::ScStreamFilter(ScStreamFilter* filter)
{
	assert(filter != NULL);
	m_dataStream = NULL;
	m_filter     = filter;
	m_filterMode = FilterToFilter;
	m_writtenToStream = 0;
}

bool ScStreamFilter::writeDataInternal(const char* data, int dataLen)
{
	bool writeSuccess = false;
	if (m_filterMode == FilterToStream)
	{
		int written  = m_dataStream->writeRawData(data, dataLen);
		m_writtenToStream += written;
		writeSuccess = (written == dataLen);
	}
	else if (m_filterMode == FilterToFilter)
		writeSuccess = m_filter->writeData(data, dataLen);
	return writeSuccess;
}

bool ScStreamFilter::writeData(const QByteArray& data)
{
	return writeData(data.constData(), data.size());
}

bool ScStreamFilter::openFilter (void)
{
	m_writtenToStream = 0;
	if (m_filterMode == FilterToFilter)
		return m_filter->openFilter();
	return true;
}

bool ScStreamFilter::closeFilter(void)
{
	if (m_filterMode == FilterToFilter)
		return m_filter->closeFilter();
	return true;
}

int ScStreamFilter::writtenToStream(void)
{
	if (m_filterMode == FilterToFilter)
		return (m_writtenToStream + m_filter->writtenToStream());
	return m_writtenToStream;
}
