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

#include <QComboBox>
#include <QSignalBlocker>

#include "documentlogtypes.h"
#include "documentlogmanager.h"
#include "documentlogviewer.h"


DocumentLogViewer::DocumentLogViewer(QWidget* parent, bool modal)
	: ScrPaletteBase( parent, "documentLogViewer", modal )
{
	setupUi(this);
	setModal(false);
	languageChange();
	clearPushButton->setDisabled(true);
	//Hide temporary sample log file generation button
	samplePushButton->hide();
	samplePushButton->setDisabled(true);
}

void DocumentLogViewer::setManager(DocumentLogManager* manager)
{
	m_manager = manager;
	if (!m_manager)
		return;

	qRegisterMetaType<DocumentLogEntry>("DocumentLogEntry");
	connect(m_manager, SIGNAL(logAdded(DocumentLogEntry)), this, SLOT(onLogAdded(DocumentLogEntry)), Qt::QueuedConnection);
	connect(m_manager, SIGNAL(logCleared()), this, SLOT(onLogCleared()), Qt::QueuedConnection);
	connect(sourceFilterComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(filterSet()));
	connect(levelFilterComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(filterSet()));
	connect(clearPushButton, SIGNAL(clicked()), this, SLOT(clearLog()));
	connect(samplePushButton, SIGNAL(clicked()), this, SLOT(addSampleData()));
}

void DocumentLogViewer::setDocument(const QString& docID)
{
	logTableWidget->setRowCount(0);
	m_docID = docID;

	m_sources.clear();
	m_sources.append(tr("All"));
	QSignalBlocker sfcbBlocker(sourceFilterComboBox);
	sourceFilterComboBox->clear();
	sourceFilterComboBox->addItems(m_sources);

	for (const auto& entry : m_manager->entries(docID))
		onLogAdded(entry);
}

void DocumentLogViewer::languageChange()
{
	levelStrings.clear();
	levelStrings.insert("All", tr("All", "Document log level all"));
	levelStrings.insert("Critical", tr("Critical", "Document log level critical"));
	levelStrings.insert("Debug", tr("Debug", "Document log level debug"));
	levelStrings.insert("Error", tr("Error", "Document log level error"));
	levelStrings.insert("Info", tr("Info", "Document log level info"));
	levelStrings.insert("Warning", tr("Warning", "Document log level warning"));

	levelFilterComboBox->clear();
	levelFilterComboBox->addItem(textForLevel(DocumentLogLevel::All));
	levelFilterComboBox->addItem(textForLevel(DocumentLogLevel::Debug));
	levelFilterComboBox->addItem(textForLevel(DocumentLogLevel::Info));
	levelFilterComboBox->addItem(textForLevel(DocumentLogLevel::Critical));
	levelFilterComboBox->addItem(textForLevel(DocumentLogLevel::Error));
	levelFilterComboBox->addItem(textForLevel(DocumentLogLevel::Warning));
}

void DocumentLogViewer::onLogAdded(const DocumentLogEntry& entry)
{
	int row = logTableWidget->rowCount();
	logTableWidget->insertRow(row);

	logTableWidget->setItem(row, 0, new QTableWidgetItem(entry.timestamp.toString("yyyy-MM-dd hh:mm:ss")));
	logTableWidget->setItem(row, 1, new QTableWidgetItem(entry.source));
	logTableWidget->setItem(row, 2, new QTableWidgetItem(textForLevel(entry.level)));
	logTableWidget->setItem(row, 3, new QTableWidgetItem(entry.message));
	logTableWidget->scrollToBottom();

	if (!clearPushButton->isEnabled())
		clearPushButton->setDisabled(false);

	if (!m_sources.contains(entry.source))
	{
		m_sources.append(entry.source);
		m_sources.sort();
		QSignalBlocker sfcbBlocker(sourceFilterComboBox);
		sourceFilterComboBox->clear();
		sourceFilterComboBox->addItems(m_sources);
	}
}

void DocumentLogViewer::onLogCleared()
{
	logTableWidget->clearContents();
	for (int i = logTableWidget->rowCount(); i >= 0; i--)
		logTableWidget->removeRow(i);
	clearPushButton->setEnabled(false);
}

void DocumentLogViewer::clearLog()
{
	m_manager->clear(m_docID);
}

void DocumentLogViewer::filterSet()
{
	QString filterString(levelFilterComboBox->currentText());
	DocumentLogLevel l = DocumentLogLevel::All;
	if (filterString == levelStrings.value("Critical"))
		l = DocumentLogLevel::Critical;
	else if (filterString == levelStrings.value("Error"))
		l = DocumentLogLevel::Error;
	else if (filterString == levelStrings.value("Debug"))
		l = DocumentLogLevel::Debug;
	else if (filterString == levelStrings.value("Info"))
		l = DocumentLogLevel::Info;
	else if (filterString == levelStrings.value("Warning"))
		l = DocumentLogLevel::Warning;

	filterLogView(sourceFilterComboBox->currentText(), l);
}


void DocumentLogViewer::addSampleData()
{
	DocumentLogManager::instance()->addLog(m_docID, DocumentLogLevel::Info, "System", "Application started");
	DocumentLogManager::instance()->addLog(m_docID, DocumentLogLevel::Warning, "Network", "Connection slow");
	DocumentLogManager::instance()->addLog(m_docID, DocumentLogLevel::Error, "Database", "Failed to open file");
	DocumentLogManager::instance()->addLog(m_docID, DocumentLogLevel::Warning, "System", "Application started");
	DocumentLogManager::instance()->addLog(m_docID, DocumentLogLevel::Error, "Network", "Connection slow");
	DocumentLogManager::instance()->addLog(m_docID, DocumentLogLevel::Info, "Database", "Failed to open file");
	DocumentLogManager::instance()->addLog(m_docID, DocumentLogLevel::Info, "Scripter", "Scripter could not find Tk/Tcl library");
}

void DocumentLogViewer::filterLogView(const QString& sourceFilter, DocumentLogLevel levelFilter)
{
	onLogCleared();
	int row = 0;
	for (const auto& entry : m_manager->entries(m_docID))
	{
		if (sourceFilter != levelStrings.value("All") && sourceFilter != entry.source)
			continue;
		if (levelFilter != DocumentLogLevel::All && levelFilter != entry.level)
			continue;

		logTableWidget->insertRow(row);
		logTableWidget->setItem(row, 0, new QTableWidgetItem(entry.timestamp.toString("yyyy-MM-dd hh:mm:ss")));
		logTableWidget->setItem(row, 1, new QTableWidgetItem(entry.source));
		logTableWidget->setItem(row, 2, new QTableWidgetItem(textForLevel(entry.level)));
		logTableWidget->setItem(row, 3, new QTableWidgetItem(entry.message));
		row++;
	}
	logTableWidget->scrollToBottom();
	clearPushButton->setEnabled(logTableWidget->rowCount() > 0);
}

QString DocumentLogViewer::textForLevel(DocumentLogLevel level)
{
	QString retVal;
	switch (level)
	{
		case DocumentLogLevel::Debug:
			retVal = levelStrings.value("Debug");
			break;
		case DocumentLogLevel::Info:
			retVal = levelStrings.value("Info");
			break;
		case DocumentLogLevel::Warning:
			retVal = levelStrings.value("Warning");
			break;
		case DocumentLogLevel::Error:
			retVal = levelStrings.value("Error");
			break;
		case DocumentLogLevel::Critical:
			retVal = levelStrings.value("Critical");
			break;
		case DocumentLogLevel::All:
			retVal = levelStrings.value("All");
			break;
		default:
			break;
	}
	return retVal;
}
