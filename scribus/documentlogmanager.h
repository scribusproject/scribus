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

#ifndef DOCUMENTLOG_H
#define DOCUMENTLOG_H

#include <QMutex>
#include <QObject>
#include <QString>
#include <QVector>

#include "documentlogtypes.h"

class DocumentLogManager : public QObject
{
	Q_OBJECT
	public:
		DocumentLogManager(QObject* parent = nullptr);

		static DocumentLogManager* instance();
		static void deleteInstance();

		void addLog(const QString& docID, DocumentLogLevel level, const QString& source, const QString& message, const QString& details = QString());
		QVector<DocumentLogEntry> entries(const QString& docID) const;

	signals:
		void logAdded(const DocumentLogEntry&);
		void logCleared();

	public slots:
		void clear(const QString& docID);

	private:
		static DocumentLogManager* m_instance;
		mutable QMutex m_mutex;
		QMap<QString, QVector<DocumentLogEntry>> m_LogEntries;
};

#endif // DOCUMENTLOG_H
