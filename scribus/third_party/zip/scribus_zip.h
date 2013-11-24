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
#ifndef SCZIPHANDLER_H
#define SCZIPHANDLER_H

#include <QtCore>
class Zip;
class UnZip;

class ScZipHandler
{
	public:
		ScZipHandler(bool forWrite = false);
		virtual ~ScZipHandler();
		bool open(QString fileName);
		bool close();
		bool contains(QString fileName);
		bool read(QString fileName, QByteArray &buf);
		QStringList files();
	private:
		UnZip* m_uz;
		Zip* m_zi;
};

#endif
