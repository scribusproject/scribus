/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCGZFILE_H
#define SCGZFILE_H

#include <qstring.h>
#include <qcstring.h>

#include "scribusapi.h"

class SCRIBUS_API ScGzFile 
{
protected:
	QString fname;
	QByteArray barray;
public:
	ScGzFile(const QString& filename);
	ScGzFile(const QString& filename, const QByteArray& array);

	static const int gzipExpansionFactor;

	const QString& fileName() { return fname; }
	const QByteArray& data() { return barray; }
	void  setData(const QByteArray& array) { barray = array; }

	bool read(uint maxBytes = -1);
	bool write(void);
	bool write(const char* header);
};

#endif
