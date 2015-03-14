/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "util_file.h"

#ifdef _MSC_VER
# include <sys/utime.h>
#else
# include <utime.h>
#endif

#include <QByteArray>
#include <QDataStream>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QProcess>
#include <QTemporaryFile>

#include "fileloader.h"
#include "loadsaveplugin.h"
#include "prefsmanager.h"
#include "scribusdoc.h"
#include "scstreamfilter.h"
#include "selection.h"
#include "util.h"

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
	QTemporaryFile* tempFile = new QTemporaryFile(target + "_XXXXXX");
	if (!tempFile)
		return false;
	if (srcFile.open(QIODevice::ReadOnly))
	{
		if (tempFile->open())
		{
			tempFileName = tempFile->fileName();
			success  = copyData(srcFile, *tempFile);
			success &= (srcFile.error() == QFile::NoError && tempFile->error() == QFile::NoError);
			tempFile->close();
		}
		srcFile.close();
	}
	if (success)
	{
		if (QFile::exists(target))
			success = QFile::remove(target);
		if (success)
		{
			// We delete temporary file now to force file close
			// QTemporaryFile::close() do not really close file
			tempFile->setAutoRemove(false);
			delete tempFile; 
			tempFile = NULL;
			success = QFile::rename(tempFileName, target);
		}
	}
	if (tempFile)
		delete tempFile;
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

bool touchFile(const QString& file)
{
#if defined(_WIN32) && defined(HAVE_UNICODE)
	return _wutime((const wchar_t*) file.utf16(), NULL) == 0;
#else
	QByteArray fname = file.toLocal8Bit();
	return utime(fname.data(), NULL) == 0;
#endif
}


bool fileInPath(const QString& filename)
{
	if (filename.isEmpty())
		return false;
	QString file = filename.split(' ', QString::SkipEmptyParts).at(0); //Ignore parameters
#if defined(Q_OS_WIN32)
	if (QFileInfo(file).suffix().isEmpty())
		file += ".exe";
#endif 

	file = QDir::fromNativeSeparators(file);
	if (file.indexOf('/') >= 0)
	{
		//Looks like an absolute path
		QFileInfo info(file);
		return info.exists();
	}

	//Get $PATH
	QStringList env = QProcess::systemEnvironment();
	QString path;
	foreach (QString line, env)
	{
		if (line.indexOf("PATH") == 0)
		{
			path = line.mid(5); //Strip "PATH="
			break;
		}
	}
	QStringList splitpath;
	#if defined(Q_OS_WIN32) || defined (Q_OS_OS2)
		splitpath = path.split(';', QString::SkipEmptyParts);
	#else
		splitpath = path.split(':', QString::SkipEmptyParts);
	#endif
	foreach (QString dir, splitpath)
	{
		QFileInfo info(dir, file);
		if (info.exists())
			return true;
	}
	return false;
}

PageItem*  getVectorFileFromData(ScribusDoc *doc, QByteArray &data, QString ext, double x, double y, double w, double h)
{
	PageItem* retObj = NULL;
	QTemporaryFile *tempFile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_XXXXXX." + ext);
	if (tempFile->open())
	{
		QString fileName = getLongPathName(tempFile->fileName());
		if (!fileName.isEmpty())
		{
			tempFile->write(data);
			tempFile->close();
			FileLoader *fileLoader = new FileLoader(fileName);
			int testResult = fileLoader->testFile();
			delete fileLoader;
			if (testResult != -1)
			{
				const FileFormat * fmt = LoadSavePlugin::getFormatById(testResult);
				if( fmt )
				{
					doc->m_Selection->clear();
					doc->m_Selection->delaySignalsOn();
					fmt->setupTargets(doc, 0, 0, 0, &(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts));
					fmt->loadFile(fileName, LoadSavePlugin::lfUseCurrentPage|LoadSavePlugin::lfInteractive|LoadSavePlugin::lfScripted);
					if (doc->m_Selection->count() > 0)
					{
						retObj = doc->groupObjectsSelection();
						retObj->setTextFlowMode(PageItem::TextFlowUsesBoundingBox);
						retObj->setXYPos(x, y, true);
						if ((w >= 0) && (h >= 0))
							retObj->setWidthHeight(w, h, true);
						retObj->updateClip();
						retObj->update();
					}
					doc->m_Selection->clear();
					doc->m_Selection->delaySignalsOff();
				}
			}
		}
	}
	delete tempFile;
	return retObj;
}
