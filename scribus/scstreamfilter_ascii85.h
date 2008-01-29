/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCSTREAMFILTER_ASCII95_H
#define SCSTREAMFILTER_ASCII95_H

#include "scstreamfilter.h"

#include <QByteArray>

class ScASCII85EncodeFilter : public ScStreamFilter
{
protected:

	QByteArray     m_buffer;
	int            m_buffer_pending;

	unsigned char  m_four_tuple[4];
	int            m_four_tuple_pending;

public:
	ScASCII85EncodeFilter(QDataStream* stream);
	ScASCII85EncodeFilter(ScStreamFilter* filter);

	virtual bool openFilter (void);
	virtual bool closeFilter(void);

	virtual bool writeData(const char* data, int dataLen);
};

#endif
