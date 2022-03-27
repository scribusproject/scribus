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
public:
	ScStreamFilter(QDataStream* stream);
	ScStreamFilter(ScStreamFilter* filter);
	virtual ~ScStreamFilter() = default;

	virtual bool openFilter();
	virtual bool closeFilter();

	virtual bool writeData(const QByteArray& data);
	virtual bool writeData(const char* data, int dataLen) = 0;

	int writtenToStream();

protected:
	enum FilterMode
	{
		FilterToStream = 0,
		FilterToFilter = 1
	};

	int             m_writtenToStream { 0 };
	FilterMode      m_filterMode;
	QDataStream*    m_dataStream { nullptr };
	ScStreamFilter* m_filter { nullptr };

	bool writeDataInternal(const char* data, int dataLen);
};

class ScNullEncodeFilter : public ScStreamFilter
{
public:
	ScNullEncodeFilter(QDataStream* stream) : ScStreamFilter(stream) {};
	ScNullEncodeFilter(ScStreamFilter* filter) : ScStreamFilter(filter) {};

	bool writeData(const char* data, int dataLen) override { return writeDataInternal(data, dataLen); }
};

#endif
