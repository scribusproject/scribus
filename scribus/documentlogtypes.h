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
	Debug = 0,
	Info = 1,
	Warning = 2,
	Error = 3,
	Critical = 4,
	All = 99
};

struct DocumentLogEntry
{
	QDateTime timestamp;
	DocumentLogLevel level;
	QString source;
	QString message;
	QString details;
	QString docID;          // document the entry belongs to
	quint64 sequence { 0 }; // increases with every entry added, across all documents
};

Q_DECLARE_METATYPE(DocumentLogEntry)

#endif // DOCUMENTLOGTYPES_H
