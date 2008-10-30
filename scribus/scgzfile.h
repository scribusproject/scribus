/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCGZFILE_H
#define SCGZFILE_H

#include <QByteArray>
#include <QIODevice>
#include <QString>

#include "scribusapi.h"

struct ScGzFileDataPrivate;

class SCRIBUS_API ScGzFile : public QIODevice
{
protected:
	QString              m_fileName;
	ScGzFileDataPrivate* m_data;
	ScGzFileDataPrivate* newPrivateData(void);
	void freeData(void);

	bool    gzFileOpen(QString fileName, ScGzFileDataPrivate* data, QIODevice::OpenMode mode);
	
	virtual qint64 readData  (char * data, qint64 maxSize);
	virtual qint64 writeData (const char * data, qint64 maxSize);

public:
	ScGzFile(const QString& filename);
	~ScGzFile(void);

	static const int gzipExpansionFactor;
	
	virtual bool atEnd() const;
	virtual bool open(QIODevice::OpenMode mode);
	virtual void close();

	bool errorOccurred(void) const;
	int  error(void) const;

	bool reset(void);

	static bool readFromFile(const QString& filename, QByteArray& bArray, uint maxBytes = 0);
	static bool writeToFile (const QString& filename, const QByteArray& bArray);
	static bool writeToFile (const QString& filename, const QByteArray& bArray, const char* header);
};

#endif
