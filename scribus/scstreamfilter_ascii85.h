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
public:
	ScASCII85EncodeFilter(QDataStream* stream);
	ScASCII85EncodeFilter(ScStreamFilter* filter);

	bool openFilter () override;
	bool closeFilter() override;

	bool writeData(const char* data, int dataLen) override;

protected:
	QByteArray     m_buffer;
	int            m_buffer_pending { 0 };

	unsigned char  m_four_tuple[4] { 0 };
	int            m_four_tuple_pending { 0 };
};

#endif
