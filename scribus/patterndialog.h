/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
*   Copyright (C) 2006 by Franz Schmid                                   *
*   franz.schmid@altmuehlnet.de                                                   *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#ifndef PATTERNDIALOG_H
#define PATTERNDIALOG_H

#include <QMap>
#include <QString>
#include <QDialog>
#include <QListWidgetItem>
#include <QPushButton>
#include <QLayout>
#include "ui_patterndialog.h"
#include "scribusapi.h"
#include "scribusdoc.h"
#include "scribus.h"
#include "scpattern.h"

class SCRIBUS_API PatternDialog : public QDialog, Ui::PatternDialog
{
	Q_OBJECT

	public:
		PatternDialog(QWidget* parent, QMap<QString, ScPattern> *docPatterns, ScribusDoc *doc, ScribusMainWindow* scMW);
		~PatternDialog() {};
		void updatePatternList();
		void loadVectors(QString data);
		QStringList getUsedPatternsHelper(QString pattern, QStringList &results);
		ScribusDoc *m_doc;
		ScribusMainWindow *mainWin;
		QMap<QString, ScPattern> dialogPatterns;
		QMap<QString,QString> replaceMap;
		QMap<QString,QString> origNames;
		QStringList origPatterns;
	public slots:
		void renamePattern();
		void loadPatternDir();
		void loadPattern();
		void patternSelected(QListWidgetItem* it);
		void removeAllPatterns();
		void removePattern();
};

#endif
