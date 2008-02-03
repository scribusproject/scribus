/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCSTREAMFILTER_RC4_H
#define SCSTREAMFILTER_RC4_H

#include "scstreamfilter.h"

#include <QByteArray>

struct ScRC4EncodeFilterData;

class ScRC4EncodeFilter : public ScStreamFilter
{
protected:

	ScRC4EncodeFilterData* m_filterData;
	QByteArray             m_key;

	void freeData(void);
	bool m_openedFilter;

public:
	ScRC4EncodeFilter(QDataStream* stream, const char* key, unsigned int keyLen);
	ScRC4EncodeFilter(ScStreamFilter* filter, const char* key, unsigned int keyLen);
	~ScRC4EncodeFilter();

	virtual bool openFilter (void);
	virtual bool closeFilter(void);

	virtual bool writeData(const char* data, int dataLen);
};

#endif
