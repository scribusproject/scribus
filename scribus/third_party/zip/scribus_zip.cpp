/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/
/***************************************************************************
							 -------------------
	begin                : Sat Nov 23 2013
	copyright            : (C) 2013 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

#include "zip.h"
#include "unzip.h"
#include "scribus_zip.h"
#include "util.h"
#include "scpaths.h"

ScZipHandler::ScZipHandler(bool forWrite)
{
	if (forWrite)
		m_zi = new Zip();
	else
		m_uz = new UnZip();
}

ScZipHandler::~ScZipHandler()
{
	if (m_uz && m_uz->isOpen())
		m_uz->closeArchive();
	if (m_zi && m_zi->isOpen())
		m_zi->closeArchive();
	delete m_uz;
	delete m_zi;
}

bool ScZipHandler::open(const QString& fileName)
{
	bool retVal = false;
	if (m_uz != nullptr)
	{
		UnZip::ErrorCode ec = m_uz->openArchive(fileName);
		retVal = (ec == UnZip::Ok);
	}
	if (m_zi != nullptr)
	{
		Zip::ErrorCode ec = m_zi->createArchive(fileName);
		retVal = (ec == Zip::Ok);
	}
	return retVal;
}

bool ScZipHandler::close()
{
	bool retVal = false;
	if (m_uz != nullptr)
	{
		m_uz->closeArchive();
		retVal = true;
	}
	if (m_zi != nullptr)
	{
		Zip::ErrorCode ec = m_zi->closeArchive();
		retVal = (ec == Zip::Ok);
	}
	return retVal;
}

bool ScZipHandler::contains(const QString& fileName)
{
	if (m_uz == nullptr)
		return false;
	return m_uz->contains(fileName);
}

bool ScZipHandler::read(const QString& fileName, QByteArray &buf)
{
	if (m_uz == nullptr)
		return false;
	QByteArray byteArray;
	QBuffer buffer(&byteArray);
	buffer.open(QIODevice::WriteOnly);
	UnZip::ErrorCode ec = m_uz->extractFile(fileName, &buffer);
	bool retVal = (ec == UnZip::Ok);
	if (retVal)
		buf = byteArray;
	return retVal;
}

bool ScZipHandler::write(const QString& dirName)
{
	if (m_zi == nullptr)
		return false;
	Zip::ErrorCode ec = m_zi->addDirectory(dirName, "", Zip::IgnoreRoot);
	return (ec == Zip::Ok);
}

bool ScZipHandler::extract(const QString& name, const QString& path, ExtractionOption eo)
{
	if (m_uz == nullptr)
		return false;
	bool retVal = false;
	QString pwd(QDir::currentPath());
	QString outDir;
	if (path.isNull())
		outDir = ScPaths::tempFileDir();
	else
		outDir=path;
	QFile f(outDir);
	QFileInfo fi(f);
	if (!fi.isWritable())
		outDir = ScPaths::applicationDataDir();
	QDir::setCurrent(outDir);
	UnZip::ErrorCode ec = m_uz->extractFile(name, outDir, static_cast<UnZip::ExtractionOption>(eo));
	retVal = (ec == UnZip::Ok);
	QDir::setCurrent(pwd);
	return retVal;
}

QStringList ScZipHandler::files()
{
	QStringList retVal;
	if (m_uz != nullptr)
		retVal = m_uz->fileList();
	return retVal;
}
