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
		void setManager(DocumentLogManager *manager);
		void setDocument(const QString& docID);

	private slots:
		void onLogAdded(const DocumentLogEntry& entry);
		void onLogCleared();
		void clearLog();

	private:
		void update();

		DocumentLogManager* m_manager;
		QString m_docID;
};
#endif // DOCUMENTLOGVIEWER_H
