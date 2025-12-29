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

#include <QDateTime>

#include "documentlogmanager.h"

DocumentLogManager::DocumentLogManager(QObject* parent)
	: QObject(parent)
{
}

DocumentLogManager& DocumentLogManager::instance()
{
	static DocumentLogManager instance;
	return instance;
}

QString DocumentLogManager::msgUnsupportedFileFormat(const QString& filename)
{
	return tr("Unsupported file format for %1").arg(filename);
}

QString DocumentLogManager::msgFileParsingFailed(const QString& filename)
{
	return tr("File parsing failed for %1").arg(filename);
}

QString DocumentLogManager::msgFileNotFound(const QString& filename)
{
	return tr("File not found: %1").arg(filename);
}

void DocumentLogManager::addLog(const QString& docID, DocumentLogLevel level, const QString& source, const QString& message, const QString& details)
{
	DocumentLogEntry entry{QDateTime::currentDateTime(), level, source, message, details};
	QMutexLocker locker(&m_mutex);
	if (!m_LogEntries.contains(docID))
		m_LogEntries.insert(docID, QVector<DocumentLogEntry>());

	QVector<DocumentLogEntry>& entries = m_LogEntries[docID];

	if (m_LogEntries.contains(docID))
		entries.append(entry);

	// Trim old entries if we exceed max, Remove oldest entries (FIFO)
	if (m_maxEntries > 0 && entries.size() > m_maxEntries)
		entries.remove(0, entries.size() - m_maxEntries);

	emit logAdded(entry);
}

QVector<DocumentLogEntry> DocumentLogManager::entries(const QString& docID) const
{
	QMutexLocker locker(&m_mutex);
	if (m_LogEntries.contains(docID))
		return m_LogEntries[docID];
	return QVector<DocumentLogEntry>();
}

void DocumentLogManager::setMaxEntries(int maxEntries)
{
	QMutexLocker locker(&m_mutex);
	m_maxEntries = maxEntries;

	// Optionally trim existing logs to new limit
	if (m_maxEntries > 0)
	{
		for (auto& entries : m_LogEntries)
		{
			if (entries.size() > m_maxEntries)
				entries.remove(0, entries.size() - m_maxEntries);
		}
	}
}

int DocumentLogManager::maxEntries() const
{
	QMutexLocker locker(&m_mutex);
	return m_maxEntries;
}

void DocumentLogManager::clear(const QString& docID)
{
	QMutexLocker locker(&m_mutex);
	if (m_LogEntries.contains(docID))
		m_LogEntries[docID].clear();
	emit logCleared();
}
