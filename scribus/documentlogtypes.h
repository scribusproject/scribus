/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#ifndef DOCUMENTLOGTYPES_H
#define DOCUMENTLOGTYPES_H

#include <QDateTime>
#include <QMetaType>
#include <QString>

enum class DocumentLogLevel
{
	Debug,
	Info,
	Warning,
	Error,
	Critical
};

struct DocumentLogEntry
{
	QDateTime timestamp;
	DocumentLogLevel level;
	QString source;
	QString message;
	QString details;
};

Q_DECLARE_METATYPE(DocumentLogEntry)

#endif // DOCUMENTLOGTYPES_H
