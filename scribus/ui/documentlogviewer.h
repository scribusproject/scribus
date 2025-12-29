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

#ifndef DOCUMENTLOGVIEWER_H
#define DOCUMENTLOGVIEWER_H

#include "documentlogtypes.h"
#include "ui_documentlogviewerbase.h"

#include "scrpalettebase.h"

class DocumentLogManager;

class DocumentLogViewer : public ScrPaletteBase, public Ui::DocumentLogViewerBase
{
	Q_OBJECT
	public:
		explicit DocumentLogViewer(QWidget* parent, bool modal);
		void setManagerConnections();
		void setDocument(const QString& docID);

	public slots:
		void languageChange();

	private slots:
		void onLogAdded(const DocumentLogEntry& entry);
		void onLogCleared();
		void clearLog();
		void filterSet();
		void addSampleData();

	private:
		void filterLogView(const QString& sourceFilter, DocumentLogLevel levelFilter);
		QString textForLevel(DocumentLogLevel level);

		QString m_docID;
		QStringList m_sources;
		QMap<QString, QString> levelStrings;
};
#endif // DOCUMENTLOGVIEWER_H
