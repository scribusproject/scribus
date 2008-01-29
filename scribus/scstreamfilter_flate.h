/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCSTREAMFILTER_FLATE_H
#define SCSTREAMFILTER_FLATE_H

#include "scstreamfilter.h"

struct ScFlateEncodeFilterData;

class ScFlateEncodeFilter : public ScStreamFilter
{
protected:

	ScFlateEncodeFilterData* m_filterData;

	void freeData(void);
	bool m_openedFilter;

	bool writeDeflate(bool flush);

public:
	ScFlateEncodeFilter(QDataStream* stream);
	ScFlateEncodeFilter(ScStreamFilter* filter);
	~ScFlateEncodeFilter();

	virtual bool openFilter (void);
	virtual bool closeFilter(void);

	virtual bool writeData(const char* data, int dataLen);
};

#endif