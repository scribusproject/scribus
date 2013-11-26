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

ScZipHandler::ScZipHandler(bool forWrite)
{
	m_uz = NULL;
	m_zi = NULL;
	if (forWrite)
		m_zi = new Zip();
	else
		m_uz = new UnZip();
}

ScZipHandler::~ScZipHandler()
{
	if (m_uz != NULL)
		delete m_uz;
	if (m_zi != NULL)
		delete m_zi;
}

bool ScZipHandler::open(QString fileName)
{
	bool retVal = false;
	if (m_uz != NULL)
	{
		UnZip::ErrorCode ec = m_uz->openArchive(fileName);
		retVal = (ec == UnZip::Ok);
	}
	if (m_zi != NULL)
	{
		Zip::ErrorCode ec = m_zi->createArchive(fileName);
		retVal = (ec == Zip::Ok);
	}
	return retVal;
}

bool ScZipHandler::close()
{
	bool retVal = false;
	if (m_uz != NULL)
	{
		m_uz->closeArchive();
		retVal = true;
	}
	if (m_zi != NULL)
	{
		Zip::ErrorCode ec = m_zi->closeArchive();
		retVal = (ec == Zip::Ok);
	}
	return retVal;
}

bool ScZipHandler::contains(QString fileName)
{
	bool retVal = false;
	if (m_uz != NULL)
		retVal = m_uz->contains(fileName);
	return retVal;
}

bool ScZipHandler::read(QString fileName, QByteArray &buf)
{
	bool retVal = false;
	if (m_uz != NULL)
	{
		QByteArray byteArray;
		QBuffer buffer(&byteArray);
		buffer.open(QIODevice::WriteOnly);
		UnZip::ErrorCode ec = m_uz->extractFile(fileName, &buffer);
		retVal = (ec == UnZip::Ok);
		if (retVal)
			buf = byteArray;
	}
	return retVal;
}

bool ScZipHandler::write(QString dirName)
{
	bool retVal = false;
	if (m_zi != NULL)
	{
		Zip::ErrorCode ec = m_zi->addDirectory(dirName, "", Zip::IgnoreRoot);
		retVal = (ec == Zip::Ok);
	}
	return retVal;
}

QStringList ScZipHandler::files()
{
	QStringList retVal = QStringList();
	if (m_uz != NULL)
		retVal = m_uz->fileList();
	return retVal;
}
