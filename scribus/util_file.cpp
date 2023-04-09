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
#include <QScopedPointer>
#include <QTemporaryFile>

#include "fileloader.h"
#include "loadsaveplugin.h"
#include "prefsmanager.h"
#include "scpaths.h"
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

	// Make sure to compare source and target paths
	// by using same directory separators. Source and target
	// arguments might use native and Qt's separators for eg
	QString qtSource = QDir::fromNativeSeparators(source);
	QString qtTarget = QDir::fromNativeSeparators(target);
	if (qtSource == qtTarget)
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

	// Make sure to compare source and target paths
	// by using same directory separators. Source and target
	// arguments might use native and Qt's separators for eg
	QString qtSource = QDir::fromNativeSeparators(source);
	QString qtTarget = QDir::fromNativeSeparators(target);
	if (qtSource == qtTarget)
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
			tempFile = nullptr;
			success = QFile::rename(tempFileName, target);
		}
	}
	delete tempFile;
	return success;
}

bool copyFileToFilter(const QString& source, ScStreamFilter& target)
{
	bool copySucceed = true;
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
		int bytesread = s.read( bb.data(), bb.size() );
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
		int byteswrite = 0;
		int bytesread = s.read( bb.data(), bb.size() );
		while (bytesread > 0)
		{
			byteswrite = target.writeRawData(bb.data(), bytesread);
			copySucceed &= (byteswrite == bytesread);
			bytesread = s.read( bb.data(), bb.size() );
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

	// Make sure to compare source and target paths
	// by using same directory separators. Source and target
	// arguments might use native and Qt's separators for eg
	QString qtSource = QDir::fromNativeSeparators(source);
	QString qtTarget = QDir::fromNativeSeparators(target);
	if (qtSource == qtTarget)
		return false;

	bool moveSucceed = copyFile(source, target);
	if (moveSucceed)
		moveSucceed &= QFile::remove(source);
	return moveSucceed;
}

bool touchFile(const QString& file)
{
#if defined(_WIN32) && defined(HAVE_UNICODE)
	return _wutime((const wchar_t*) file.utf16(), nullptr) == 0;
#else
	QByteArray fname = file.toLocal8Bit();
	return utime(fname.data(), nullptr) == 0;
#endif
}


bool fileInPath(const QString& filename)
{
	if (filename.isEmpty())
		return false;
	QString file = filename;
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
	QString path;
	const QStringList env = QProcess::systemEnvironment();
	for (const QString& line : env)
	{
		if (line.indexOf("PATH") == 0)
		{
			path = line.mid(5); //Strip "PATH="
			break;
		}
	}

	QChar envPathSeparator(ScPaths::envPathSeparator);
	const QStringList splitpath = path.split(envPathSeparator, Qt::SkipEmptyParts);
	for (const QString& dir : splitpath)
	{
		QFileInfo info(dir, file);
		if (info.exists())
			return true;
	}
	return false;
}

PageItem* getVectorFileFromData(ScribusDoc *doc, QByteArray &data, const QString& ext, double x, double y, double w, double h)
{
	PageItem* retObj = nullptr;

	QScopedPointer<QTemporaryFile> tempFile(new QTemporaryFile(QDir::tempPath() + "/scribus_temp_XXXXXX." + ext));
	if (!tempFile->open())
		return nullptr;

	QString fileName = getLongPathName(tempFile->fileName());
	if (fileName.isEmpty())
		return nullptr;

	tempFile->write(data);
	tempFile->close();

	FileLoader *fileLoader = new FileLoader(fileName);
	int testResult = fileLoader->testFile();
	delete fileLoader;

	if (testResult == -1)
		return nullptr;

	const FileFormat * fmt = LoadSavePlugin::getFormatById(testResult);
	if (!fmt)
		return nullptr;

	doc->m_Selection->clear();
	doc->m_Selection->delaySignalsOn();
	fmt->setupTargets(doc, nullptr, nullptr, nullptr, &(PrefsManager::instance().appPrefs.fontPrefs.AvailFonts));
	fmt->loadFile(fileName, LoadSavePlugin::lfUseCurrentPage|LoadSavePlugin::lfInteractive|LoadSavePlugin::lfScripted);
	if (!doc->m_Selection->isEmpty())
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

	return retObj;
}

bool checkFileHash(const QString& directory, const QString& filename, const QString& hashFilename, QCryptographicHash::Algorithm method)
{
	//In a single directory, make a hash of filename, and compare it to the string for that file in hashFilename
	//Assumption is that the hash file only has one line for now
	QByteArray ba_hash;
	if (!loadRawText(directory + hashFilename, ba_hash))
	{
		qDebug() << "checkFileHash: loadRawText file unsuccessful";
		return false;
	}

	QFile source(directory + filename);
	if (source.open(QIODevice::ReadOnly))
	{
		ba_hash = ba_hash.simplified();
		QList<QByteArray> fileData(ba_hash.split(' '));
		QCryptographicHash ch(method);
		ch.addData(&source);
		source.close();
		if (fileData[0] == ch.result().toHex() && fileData[1] == filename)
			return true;
		qDebug()<<"checkFileHash: checksum failed for"<<directory<<filename;
	}
	return false;
}
