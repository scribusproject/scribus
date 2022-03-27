/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCSTREAMFILTER_JPEG_H
#define SCSTREAMFILTER_JPEG_H

#include <QGlobalStatic>
#include "scstreamfilter.h"

struct ScJpegEncodeFilterData;

class ScJpegEncodeFilter : public ScStreamFilter
{
	friend struct ScJpegDestinationMgr;

public:
	enum Color
	{
		RGB,
		CMYK,
		GRAY
	};

	ScJpegEncodeFilter(QDataStream* stream, unsigned int imgWidth, unsigned int imgHeight, ScJpegEncodeFilter::Color color);
	ScJpegEncodeFilter(ScStreamFilter* filter, unsigned int imgWidth, unsigned int imgHeight, ScJpegEncodeFilter::Color color);
	~ScJpegEncodeFilter() override;

	bool openFilter () override;
	bool closeFilter() override;

	bool writeData(const char* data, int dataLen) override;

	void setQuality(int quality) { m_quality = qMin(qMax(0, quality), 100); }

protected:
	bool  m_openedFilter { false };
	ScJpegEncodeFilterData* m_filterData { nullptr };

	void  freeData();

	unsigned int m_width { 0 };
	unsigned int m_height { 0 };
	int          m_quality { 75 };
	Color        m_color;
};

#endif
