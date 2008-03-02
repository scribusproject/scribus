/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <cstdlib>
#include <zlib.h>
#include <QDataStream>
#include <QDir>
#include <QFileInfo>

#include "scconfig.h"
#include "scgzfile.h"

/* Code inspired by zlib and adapted for Scribus by Jean Ghali */

const  int ScGzFile::gzipExpansionFactor = 8;
static int const gz_magic_header[2] = {0x1f, 0x8b}; /* gzip magic header */

#define BUFFER_SIZE 16384
struct  ScGzFileWriteDataPrivate
{
    z_stream      zlib_stream;
	z_off_t       bytesIn;
    unsigned char input_buffer [BUFFER_SIZE];
    unsigned char output_buffer[BUFFER_SIZE];
	unsigned long crc;
};

ScGzFile::ScGzFile(const QString& fileName) : QFile(fileName)
{
	m_writeData = NULL;
}

ScGzFile::~ScGzFile(void)
{
	freeWriteData();
}

ScGzFileWriteDataPrivate* ScGzFile::newWriteData(void)
{
	freeWriteData();
	m_writeData = (ScGzFileWriteDataPrivate *) malloc(sizeof(ScGzFileWriteDataPrivate));
	if (m_writeData)
	{
		bool initSuccess = initWriteData();
		if (!initSuccess)
			freeWriteData();
	}
	return m_writeData;
}

bool ScGzFile::initWriteData(void)
{
	bool success = false;
	if (m_writeData)
	{
		m_writeData->zlib_stream.zalloc = Z_NULL;
		m_writeData->zlib_stream.zfree  = Z_NULL;
		m_writeData->zlib_stream.opaque = Z_NULL;

		int err = deflateInit2(&m_writeData->zlib_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -MAX_WBITS, 8, Z_DEFAULT_STRATEGY);
		if (err != Z_OK) return false;

		m_writeData->bytesIn = 0;
		m_writeData->zlib_stream.next_in   = m_writeData->input_buffer;
		m_writeData->zlib_stream.avail_in  = 0;
		m_writeData->zlib_stream.next_out  = m_writeData->output_buffer;
		m_writeData->zlib_stream.avail_out = BUFFER_SIZE;
		m_writeData->crc = crc32(0L, Z_NULL, 0);
		success = true;
	}
	return success;
}

void ScGzFile::freeWriteData(void)
{
	if (m_writeData)
	{
		free(m_writeData);
		m_writeData = NULL;
	}
}

bool ScGzFile::open(QIODevice::OpenMode mode)
{
	if (mode != QIODevice::WriteOnly)
		return false;

	m_writeData = newWriteData();
	if (!m_writeData) 
	{
		return false;
	}

	bool opened = QFile::open(mode);
	if (opened)
	{
		char buffer[16];
		qsnprintf(buffer, 15, "%c%c%c%c%c%c%c%c%c%c", gz_magic_header[0], gz_magic_header[1],
				  Z_DEFLATED, 0 /*flags*/, 0,0,0,0 /*time*/, 0 /*xflags*/, 0x03);
		QFile::writeData(buffer, 10);
		return true;
	}
	return false;
}

qint64 ScGzFile::writeData (const char * data, qint64 dataLen)
{
	bool deflateSuccess = true;
    qint64 count;
    const unsigned char *p = (const unsigned char *) data;

	if (!m_writeData)
		return false;

    while (dataLen) {
        count = dataLen;
        if (count > BUFFER_SIZE - m_writeData->zlib_stream.avail_in)
            count = BUFFER_SIZE - m_writeData->zlib_stream.avail_in;
        memcpy (m_writeData->input_buffer + m_writeData->zlib_stream.avail_in, p, count);
		m_writeData->crc = crc32(m_writeData->crc, (const Bytef *) p, count);
        p += count;
        m_writeData->zlib_stream.avail_in += count;
        dataLen -= count;

        if (m_writeData->zlib_stream.avail_in == BUFFER_SIZE)
            deflateSuccess &= writeDeflate(false);
    }

	return (deflateSuccess ? dataLen : 0);
}

bool ScGzFile::writeDeflate(bool flush)
{
	int  ret;
	bool deflateSuccess = true;
    bool finished;
	
	do {
		m_writeData->bytesIn += m_writeData->zlib_stream.avail_in;
		ret = deflate (&m_writeData->zlib_stream, flush ? Z_FINISH : Z_NO_FLUSH);
		m_writeData->bytesIn -= m_writeData->zlib_stream.avail_in;
        if (flush || m_writeData->zlib_stream.avail_out == 0)
        {
			qint64 written = QFile::writeData((const char*) m_writeData->output_buffer, BUFFER_SIZE - m_writeData->zlib_stream.avail_out);
			deflateSuccess &= (written == (BUFFER_SIZE - m_writeData->zlib_stream.avail_out));
            m_writeData->zlib_stream.next_out  = m_writeData->output_buffer;
            m_writeData->zlib_stream.avail_out = BUFFER_SIZE;
        }

        finished = TRUE;
        if (m_writeData->zlib_stream.avail_in != 0)
            finished = FALSE;
        if (flush && ret != Z_STREAM_END)
            finished = FALSE;

    } while (!finished);

    m_writeData->zlib_stream.next_in = m_writeData->input_buffer;
	return deflateSuccess;
}

void ScGzFile::close()
{
	if (!QFile::isOpen() || !m_writeData)
		return;

	writeDeflate(true);
	deflateEnd (&m_writeData->zlib_stream);
	putLong (m_writeData->crc);
	putLong ((unsigned long) (m_writeData->bytesIn & 0xffffffff));
	QFile::close();
}

void ScGzFile::putLong(unsigned long l)
{
	for (int n = 0; n < 4; ++n)
	{
		QFile::putChar((char)(l & 0xff));
		l >>= 8;
	}
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
