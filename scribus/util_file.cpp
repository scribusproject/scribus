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
#include "scstreamfilter.h"

bool copyFile(const QString& source, const QString& target)
{
	int bytesread, error = -1;
	if ((source.isNull()) || (target.isNull()))
		return false;
	if (source == target)
		return false;
	QFile s(source);
	if (!s.exists())
		return false;
	QFile t(target);
	QByteArray bb( 65536, ' ' );
	if (bb.size() <= 0) // Check for memory allocation failure
		return false;
	if (s.open(QIODevice::ReadOnly))
	{
		if (t.open(QIODevice::WriteOnly))
		{
			bytesread = s.read( bb.data(), bb.size() );
			while( bytesread > 0 )
			{
				t.write( bb.data(), bytesread );
				bytesread = s.read( bb.data(), bb.size() );
			}
			if (s.error() == QFile::NoError && t.error() == QFile::NoError)
				error = 0;
			t.close();
		}
		s.close();
	}
	return (error == 0);
}

bool copyFileToFilter(const QString& source, ScStreamFilter& target)
{
	bool copySucceed = true;
	int  bytesread, error = -1;
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
