/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <zlib.h>
#include <QDir>
#include <QFileInfo>

#include "scconfig.h"
#include "scgzfile.h"

const int ScGzFile::gzipExpansionFactor=8;

ScGzFile::ScGzFile(const QString& filename)
{
	fname = filename;
}

ScGzFile::ScGzFile(const QString& filename, const QByteArray& array)
{
	fname = filename;
	barray = array;
}

bool ScGzFile::read(uint maxBytes)
{
	int i;
	uint maxB = 0xFFFFFFFF;
	if (maxBytes != 0)
		maxB = maxBytes;
	barray.resize(0);
	gzFile gzDoc = NULL;
	QByteArray fn(fname.toLocal8Bit());
	gzDoc = gzopen(fn.data(),"rb");
	if(gzDoc == NULL)
	{ 
		return false; // FIXME: Needs better error return
	}
	// Allocate a buffer of a multiple of the compressed size of the file
	// as a starting point for loading. We'll expand this buffer by powers
	// of two if we run out of space.
	const QFileInfo fi(fname);
	uint bufSize = qMin<uint>(4096, fi.size()*gzipExpansionFactor);
	barray.resize(bufSize);
	char* buf = barray.data();
	uint bytesRead = 0;
	// While there's free space, read into the buffer....
	while ((i = gzread(gzDoc,buf,bufSize-bytesRead-1)) > 0)
	{
		// Ensure the string is null-terminated and move the
		// write pointer to the current position.
		buf[i]=0;
		buf+=i;
		bytesRead += i;
		// And check that there's free space to work with, expanding the
		// buffer if there's not.
		if (bufSize - bytesRead < 4096)
		{
			bufSize *= 2;
			barray.resize(bufSize);
			buf = barray.data() + bytesRead;
		}
		if (maxB >= 0 && bytesRead >= maxB)
			break;
	}
	gzclose(gzDoc);
	return (barray.size() > 0);
}

bool ScGzFile::write(void)
{
	gzFile gzDoc = NULL;
	QString localPath = QDir::toNativeSeparators(fname);
	QByteArray fn(localPath.toLocal8Bit());
	gzDoc = gzopen(fn.data(),"wb");
	if(gzDoc)
	{
		int res = gzputs(gzDoc, barray.data());
		gzclose(gzDoc);
		return (res > 0 && (res == barray.size()));
	}
	return false;
}

bool ScGzFile::write(const char* header)
{
	gzFile gzDoc = NULL;
	QString localPath = QDir::toNativeSeparators(fname);
	QByteArray fn(localPath.toLocal8Bit());
	gzDoc = gzopen(fn.data(),"wb");
	if(gzDoc)
	{
		int res1 = gzputs(gzDoc, header);
		int res2 = gzputs(gzDoc, barray.data());
		gzclose(gzDoc);
		bool done1 = (res1 > 0 && (res1 == static_cast<int>(strlen(header))));
		bool done2 = (res2 > 0 && (res2 == barray.size()));
		return (done1 && done2);
	}
	return false;
}
