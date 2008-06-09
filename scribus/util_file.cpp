/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "util_file.h"

#include <QDataStream>
#include <QFile>
#include <QString>
#include <QTemporaryFile>

#include "scstreamfilter.h"

bool copyData(QIODevice& src, QIODevice& dest)
{
	bool success = false;
	if ((src.openMode() & QIODevice::ReadOnly) == 0)
		return false;
	if ((dest.openMode() & QIODevice::WriteOnly) == 0)
		return false;
	QByteArray bb( 65536, ' ' );
	if (bb.size() > 0) // Check for memory allocation failure
	{
		qint64 byteswritten;
		qint64 bytesread = src.read( bb.data(), bb.size() );
		success = (bytesread > 0);
		while (bytesread > 0)
		{
			byteswritten = dest.write( bb.data(), bytesread );
			success  &= (bytesread == byteswritten);
			bytesread = src.read( bb.data(), bb.size() );
		}
	}
	return success;
}

bool copyFile(const QString& source, const QString& target)
{
	bool success = true;
	if ((source.isEmpty()) || (target.isEmpty()))
		return false;
	if (source == target)
		return false;
	QFile s(source);
	if (!s.exists())
		return false;
	QFile t(target);
	if (s.open(QIODevice::ReadOnly))
	{
		if (t.open(QIODevice::WriteOnly))
		{
			success  = copyData(s, t);
			success &= (s.error() == QFile::NoError && t.error() == QFile::NoError);
			t.close();
		}
		s.close();
	}
	return success;
}

bool copyFileAtomic(const QString& source, const QString& target)
{
	bool success = false;
	if ((source.isEmpty()) || (target.isEmpty()))
		return false;
	if (source == target)
		return false;
	QFile srcFile(source);
	QString tempFileName;
	QTemporaryFile tempFile(target + "_XXXXXX");
	if (srcFile.open(QIODevice::ReadOnly))
	{
		if (tempFile.open())
		{
			tempFileName = tempFile.fileName();
			success  = copyData(srcFile, tempFile);
			success &= (srcFile.error() == QFile::NoError && tempFile.error() == QFile::NoError);
			tempFile.close();
		}
		srcFile.close();
	}
	if (success)
	{
		if (QFile::exists(target))
			success = QFile::remove(target);
		if (success)
		{
			success = QFile::rename(tempFileName, target);
			tempFile.setAutoRemove(success);
		}
	}
	return success;
}

bool copyFileToFilter(const QString& source, ScStreamFilter& target)
{
	bool copySucceed = true;
	int  bytesread = 0;
	if (source.isEmpty())
		return false;
	if (!QFile::exists(source))
		return false;
	QFile s(source);
	QByteArray bb( 65536, ' ' );
	if (bb.size() <= 0) // Check for memory allocation failure
		return false;
	if (s.open(QIODevice::ReadOnly))
	{
		bytesread = s.read( bb.data(), bb.size() );
		while (bytesread > 0)
		{
			copySucceed &= target.writeData(bb.data(), bytesread);
			bytesread = s.read( bb.data(), bb.size() );
		}
		copySucceed &= (s.error() == QFile::NoError);
		s.close();
	}
	return copySucceed;
}

bool copyFileToStream(const QString& source, QDataStream& target)
{
	bool copySucceed = true;
	int  bytesread, byteswrite;
	if (source.isEmpty())
		return false;
	if (!QFile::exists(source))
		return false;
	if (!target.device()->isOpen() || !target.device()->isWritable())
		return false;
	QFile s(source);
	QByteArray bb( 65536, ' ' );
	if (bb.size() <= 0) // Check for memory allocation failure
		return false;
	if (s.open(QIODevice::ReadOnly))
	{
		bytesread = s.read( bb.data(), bb.size() );
		while (bytesread > 0)
		{
			byteswrite   = target.writeRawData(bb.data(), bytesread);
			copySucceed &= (byteswrite == bytesread);
			bytesread    = s.read( bb.data(), bb.size() );
		}
		copySucceed &= (s.error() == QFile::NoError);
		s.close();
	}
	return copySucceed;
}

bool moveFile(const QString& source, const QString& target)
{
	if (source.isEmpty() || target.isEmpty())
		return false;
	if (source == target)
		return false;
	bool moveSucceed = copyFile(source, target);
	if (moveSucceed)
		moveSucceed &= QFile::remove(source);
	return moveSucceed;
}
