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
public:
	ScFlateEncodeFilter(QDataStream* stream);
	ScFlateEncodeFilter(ScStreamFilter* filter);
	~ScFlateEncodeFilter() override;

	bool openFilter() override;
	bool closeFilter() override;

	bool writeData(const char* data, int dataLen) override;

protected:
	bool m_openedFilter { false };
	ScFlateEncodeFilterData* m_filterData { nullptr };

	void freeData();

	bool writeDeflate(bool flush);
};

#endif
