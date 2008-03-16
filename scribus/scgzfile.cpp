/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <cstdio>
#include <cstdlib>
#include <zlib.h>
#include <QDir>
#include <QFileInfo>

#include "scconfig.h"
#include "scgzfile.h"

const  int ScGzFile::gzipExpansionFactor = 8;

struct  ScGzFileDataPrivate
{
	FILE*  file;
	gzFile gzfile;
};

ScGzFile::ScGzFile(const QString& fileName) : QIODevice(), m_fileName(fileName)
{
	m_fileName = fileName;
	m_data     = NULL;
}

ScGzFile::~ScGzFile(void)
{
	freeData();
}

int ScGzFile::error(void) const
{
	int errCode = 0;
	if (m_data)
		gzerror(m_data->gzfile, &errCode);
	return errCode;
}

bool ScGzFile::errorOccurred(void) const
{
	bool errorOccurred = false;
	if (m_data)
	{
		int errCode = error();
		errorOccurred = (errCode != Z_OK && errCode != Z_STREAM_END);
	}
	return errorOccurred;
}

ScGzFileDataPrivate* ScGzFile::newPrivateData(void)
{
	freeData();
	m_data = (ScGzFileDataPrivate*) malloc(sizeof(ScGzFileDataPrivate));
	if (m_data)
	{
		m_data->file   = NULL;
		m_data->gzfile = NULL;
	}
	return m_data;
}

void ScGzFile::freeData(void)
{
	if (m_data)
	{
		free(m_data);
		m_data = NULL;
	}
}

bool ScGzFile::gzFileOpen(QString fileName, ScGzFileDataPrivate* data, QIODevice::OpenMode mode)
{
	bool success = false;
	FILE* file   = NULL;
	gzFile gzf   = NULL;
	QString localPath = QDir::toNativeSeparators(fileName);
#if defined(_WIN32)
	if (mode == QIODevice::ReadOnly)
		file = _wfopen((const wchar_t*) localPath.utf16(), L"rb");
	else if (mode == QIODevice::WriteOnly)
		file = _wfopen((const wchar_t*) localPath.utf16(), L"wb");
	if (file)
	{
		int fno = _fileno(file);
		gzf = gzdopen(fno,(mode == QIODevice::ReadOnly) ? "rb" : "wb");
		if (!gzf) fclose(file);
	}
#else
	if (mode == QIODevice::ReadOnly)
		file = fopen(localPath.toLocal8Bit().data(), "rb");
	else if (mode == QIODevice::WriteOnly)
		file = fopen(localPath.toLocal8Bit().data(), "wb");
	if (file)
	{
		int fno = fileno(file);
		gzf = gzdopen(fno,(mode == QIODevice::ReadOnly) ? "rb" : "wb");
		if (!gzf) fclose(file);
	}
#endif
	if (gzf)
	{
		data->file   = file;
		data->gzfile = gzf;
		success = true;
	}
	return success;
}

bool ScGzFile::open(QIODevice::OpenMode mode)
{
	bool success = true;
	if (isOpen()) return false;

	freeData();
	if (mode == QIODevice::ReadOnly || mode == QIODevice::WriteOnly)
		m_data = newPrivateData();
	if (!m_data) return false;

	bool opened = gzFileOpen(m_fileName, m_data, mode);
	if (opened)
	{
		opened &= QIODevice::open(mode);
		if (!opened)
			this->close();
		success = opened;
	}
	return success;
}

bool ScGzFile::reset(void)
{
	if (!m_data || (openMode() != QIODevice::ReadOnly))
		return false;

	int res = gzrewind(m_data->gzfile);
	QIODevice::reset();
	return (res == 0);
}

bool ScGzFile::atEnd()
{
	if (m_data)
	{
		if (openMode() == QIODevice::ReadOnly)
		{
			int result = gzeof(m_data->gzfile);
			return result;
		}
		return QIODevice::atEnd();
	}
	return QIODevice::atEnd();
}

qint64 ScGzFile::readData (char * data, qint64 maxSize)
{
	if (!m_data || (openMode() != QIODevice::ReadOnly))
		return 0;

	int result = gzread(m_data->gzfile, data, maxSize);
	return result;
}

qint64 ScGzFile::writeData (const char * data, qint64 dataLen)
{
	bool deflateSuccess = true;
    const unsigned char *p = (const unsigned char *) data;

	if (!m_data || (openMode() != QIODevice::WriteOnly))
		return false;

    int result = gzwrite(m_data->gzfile, data, dataLen);
	return result;
}

void ScGzFile::close()
{
	if (!isOpen() || !m_data)
		return;

	gzclose(m_data->gzfile);
	QIODevice::close();
	freeData();
}

bool ScGzFile::readFromFile(const QString& filename, QByteArray& bArray, uint maxBytes)
{
	int i;
	uint maxB = 0xFFFFFFFF;
	if (maxBytes != 0)
		maxB = maxBytes;
	bArray.resize(0);
	gzFile gzDoc = NULL;
	QByteArray fn(filename.toLocal8Bit());
	gzDoc = gzopen(fn.data(),"rb");
	if(gzDoc == NULL)
	{ 
		return false; // FIXME: Needs better error return
	}
	// Allocate a buffer of a multiple of the compressed size of the file
	// as a starting point for loading. We'll expand this buffer by powers
	// of two if we run out of space.
	const QFileInfo fi(filename);
	uint bufSize = qMin<uint>(4096, fi.size()*gzipExpansionFactor);
	bArray.resize(bufSize);
	char* buf = bArray.data();
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
			bArray.resize(bufSize);
			buf = bArray.data() + bytesRead;
		}
		if (maxB >= 0 && bytesRead >= maxB)
			break;
	}
	gzclose(gzDoc);
	return (bArray.size() > 0);
}

bool ScGzFile::writeToFile(const QString& filename, const QByteArray& bArray)
{
	gzFile gzDoc = NULL;
	QString localPath = QDir::toNativeSeparators(filename);
	QByteArray fn(localPath.toLocal8Bit());
	gzDoc = gzopen(fn.data(),"wb");
	if(gzDoc)
	{
		int res = gzputs(gzDoc, bArray.data());
		gzclose(gzDoc);
		return (res > 0 && (res == bArray.size()));
	}
	return false;
}

bool ScGzFile::writeToFile(const QString& filename, const QByteArray& bArray, const char* header)
{
	gzFile gzDoc = NULL;
	QString localPath = QDir::toNativeSeparators(filename);
	QByteArray fn(localPath.toLocal8Bit());
	gzDoc = gzopen(fn.data(),"wb");
	if(gzDoc)
	{
		int res1 = gzputs(gzDoc, header);
		int res2 = gzputs(gzDoc, bArray.data());
		gzclose(gzDoc);
		bool done1 = (res1 > 0 && (res1 == static_cast<int>(strlen(header))));
		bool done2 = (res2 > 0 && (res2 == bArray.size()));
		return (done1 && done2);
	}
	return false;
}
