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

	typedef enum  {
		RGB,
		CMYK,
		GRAY
	} Color;

	ScJpegEncodeFilter(QDataStream* stream, unsigned int imgWidth, unsigned int imgHeight, ScJpegEncodeFilter::Color color);
	ScJpegEncodeFilter(ScStreamFilter* filter, unsigned int imgWidth, unsigned int imgHeight, ScJpegEncodeFilter::Color color);
	~ScJpegEncodeFilter();

	virtual bool openFilter (void);
	virtual bool closeFilter(void);

	virtual bool writeData(const char* data, int dataLen);

	void setQuality(int quality) { m_quality = qMin(qMax(0, quality), 100); }

protected:

	ScJpegEncodeFilterData* m_filterData;

	void  freeData(void);
	bool  m_openedFilter;

	unsigned int m_width;
	unsigned int m_height;
	int          m_quality;
	Color        m_color;
};

#endif
