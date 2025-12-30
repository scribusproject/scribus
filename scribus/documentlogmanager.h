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

#ifndef DOCUMENTLOGMANAGER_H
#define DOCUMENTLOGMANAGER_H

#include <QMap>
#include <QMutex>
#include <QObject>
#include <QString>
#include <QVector>

#include "documentlogtypes.h"
#include "scribusapi.h"

class SCRIBUS_API DocumentLogManager : public QObject
{
	Q_OBJECT
	public:
		static DocumentLogManager& instance();

		void addLog(const QString& docID, DocumentLogLevel level, const QString& source, const QString& message, const QString& details = QString());
		QVector<DocumentLogEntry> entries(const QString& docID) const;

		// Standard error messages
		static QString msgUnsupportedFileFormat(const QString& filename);
		static QString msgFileImportFailed(const QString& filename);
		static QString msgFileNotFound(const QString& filename);

		// Max entries configuration
		void setMaxEntries(int maxEntries);
		int maxEntries() const;

	signals:
		void logAdded(const DocumentLogEntry&);
		void logCleared();

	public slots:
		void clear(const QString& docID);

	private:
		DocumentLogManager(QObject* parent = nullptr);
		~DocumentLogManager() = default;
		mutable QMutex m_mutex;
		QMap<QString, QVector<DocumentLogEntry>> m_LogEntries;
		int m_maxEntries { 1000 };
};

#endif // DOCUMENTLOGMANAGER_H
