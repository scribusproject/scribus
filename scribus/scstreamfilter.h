/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCSTREAMFILTER_H
#define SCSTREAMFILTER_H

class QByteArray;
class QDataStream;

class ScStreamFilter
{
protected:

	typedef enum
	{
		FilterToStream = 0,
		FilterToFilter = 1
	} FilterMode;

	int             m_writtenToStream;
	FilterMode      m_filterMode;
	QDataStream*    m_dataStream;
	ScStreamFilter* m_filter;

	bool writeDataInternal(const char* data, int dataLen);

public:

	ScStreamFilter(QDataStream* stream);
	ScStreamFilter(ScStreamFilter* filter);
	virtual ~ScStreamFilter() {};

	virtual bool openFilter (void);
	virtual bool closeFilter(void);

	virtual bool writeData(const QByteArray& data);
	virtual bool writeData(const char* data, int dataLen) = 0;

	int writtenToStream(void);
};

class ScNullEncodeFilter : public ScStreamFilter
{
public:
	ScNullEncodeFilter(QDataStream* stream) : ScStreamFilter(stream) {};
	ScNullEncodeFilter(ScStreamFilter* filter) : ScStreamFilter(filter) {};

	virtual bool writeData(const char* data, int dataLen) { return writeDataInternal(data, dataLen); }
};

#endif
