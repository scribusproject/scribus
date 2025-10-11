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

#include "documentlogmanager.h"

DocumentLogManager* DocumentLogManager::m_instance = nullptr;

DocumentLogManager::DocumentLogManager(QObject* parent)
	: QObject(parent)
{
}

DocumentLogManager* DocumentLogManager::instance()
{
	if (m_instance == nullptr)
		m_instance = new DocumentLogManager();

	return m_instance;
}

void DocumentLogManager::deleteInstance()
{
	delete m_instance;
	m_instance = nullptr;
}

void DocumentLogManager::addLog(const QString& docID, DocumentLogLevel level, const QString& source, const QString& message, const QString& details)
{
	DocumentLogEntry entry{QDateTime::currentDateTime(), level, source, message, details};
	QMutexLocker locker(&m_mutex);
	if (!m_LogEntries.contains(docID))
		m_LogEntries.insert(docID, QVector<DocumentLogEntry>());
	if (m_LogEntries.contains(docID))
		m_LogEntries[docID].append(entry);
	emit logAdded(entry);
}

QVector<DocumentLogEntry> DocumentLogManager::entries(const QString& docID) const
{
	QMutexLocker locker(&m_mutex);
	if (m_LogEntries.contains(docID))
		return m_LogEntries[docID];
	return QVector<DocumentLogEntry>();
}

void DocumentLogManager::clear(const QString& docID)
{
	QMutexLocker locker(&m_mutex);
	if (m_LogEntries.contains(docID))
		m_LogEntries[docID].clear();
	emit logCleared();
}
