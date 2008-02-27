/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCGZFILE_H
#define SCGZFILE_H

#include <QString>
#include <QByteArray>

#include "scribusapi.h"

struct ScGzFileWriteDataPrivate;

class SCRIBUS_API ScGzFile : public QFile
{
protected:

	ScGzFileWriteDataPrivate* m_writeData;
	ScGzFileWriteDataPrivate* newWriteData(void);
	bool initWriteData(void);
	void freeWriteData(void);

	void    putLong(unsigned long l);
	
	virtual qint64 writeData (const char * data, qint64 maxSize);
	virtual bool   writeDeflate(bool flush);

public:
	ScGzFile(const QString& filename);
	~ScGzFile(void);

	static const int gzipExpansionFactor;
	
	virtual bool open(QIODevice::OpenMode mode);
	virtual void close();

	static bool readFromFile(const QString& filename, QByteArray& bArray, uint maxBytes = 0);
	static bool writeToFile (const QString& filename, QByteArray& bArray);
	static bool writeToFile (const QString& filename, QByteArray& bArray, const char* header);
};

#endif
