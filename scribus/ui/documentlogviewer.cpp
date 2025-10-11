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

#include "documentlogtypes.h"
#include "documentlogmanager.h"
#include "documentlogviewer.h"


DocumentLogViewer::DocumentLogViewer(QWidget* parent, bool modal)
	: ScrPaletteBase( parent, "documentLogViewer", modal )
{
	setupUi(this);
	setModal(false);
	clearPushButton->setDisabled(true);
}

void DocumentLogViewer::setManager(DocumentLogManager* manager)
{
	m_manager = manager;
	if (!m_manager)
		return;

	qRegisterMetaType<DocumentLogEntry>("DocumentLogEntry");
	connect(m_manager, SIGNAL(logAdded(DocumentLogEntry)), this, SLOT(onLogAdded(DocumentLogEntry)), Qt::QueuedConnection);
	connect(m_manager, SIGNAL(logCleared()), this, SLOT(onLogCleared()), Qt::QueuedConnection);
	connect(clearPushButton, SIGNAL(clicked()), this, SLOT(clearLog()));
}

void DocumentLogViewer::setDocument(const QString& docID)
{
	m_docID = docID;

	for (const auto& entry : m_manager->entries(docID))
		onLogAdded(entry);
}

void DocumentLogViewer::onLogAdded(const DocumentLogEntry& entry)
{
	int row = logTableWidget->rowCount();
	logTableWidget->insertRow(row);

	logTableWidget->setItem(row, 0, new QTableWidgetItem(entry.timestamp.toString("yyyy-MM-dd hh:mm:ss")));
	QString level;
	switch (entry.level)
	{
		case DocumentLogLevel::Debug:
			level = tr("Debug");
			break;
		case DocumentLogLevel::Info:
			level = tr("Info");
			break;
		case DocumentLogLevel::Warning:
			level = tr("Warning");
			break;
		case DocumentLogLevel::Error:
			level = tr("Error");
			break;
		case DocumentLogLevel::Critical:
			level = tr("Critical");
			break;
		default:
			break;
	}
	logTableWidget->setItem(row, 1, new QTableWidgetItem(level));
	logTableWidget->setItem(row, 2, new QTableWidgetItem(entry.source));
	logTableWidget->setItem(row, 3, new QTableWidgetItem(entry.message));
	logTableWidget->scrollToBottom();

	if (!clearPushButton->isEnabled())
		clearPushButton->setDisabled(false);
}

void DocumentLogViewer::onLogCleared()
{
	logTableWidget->setRowCount(0);

	if (clearPushButton->isEnabled())
		clearPushButton->setDisabled(true);
}

void DocumentLogViewer::clearLog()
{
	m_manager->clear(m_docID);
}

void DocumentLogViewer::update()
{
	for (const auto& entry : m_manager->entries(m_docID))
		onLogAdded(entry);
}
