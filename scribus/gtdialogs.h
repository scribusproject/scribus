/***************************************************************************
 *   Copyright (C) 2004 by Riku Leino                                      *
 *   riku.leino@gmail.com                                                      *
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

#ifndef GTDIALOGS_H
#define GTDIALOGS_H

#include <qcheckbox.h>
#include <qcombobox.h>
#include <qdialog.h>
#include <qdir.h>
#include <qfiledialog.h>
#include <qframe.h>
#include <qinputdialog.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qtextcodec.h>
#include <qtoolbutton.h>
#include <qtooltip.h>
#include "prefscontext.h"

class gtFileDialog : public QFileDialog
{
	Q_OBJECT
private:
	QDir dir;
	QFrame* importerFrame;
	QFrame* encodingFrame;
	QHBoxLayout* importerLayout;
	QHBoxLayout* encodingLayout;
	QToolButton* HomeB;
	void createWidgets(const QStringList& importers);
public:
	gtFileDialog(const QString& filters, const QStringList& importers, const QString& wdir);
	~gtFileDialog();
	QCheckBox* textOnlyCheckBox;
	QComboBox* importerCombo;
	QComboBox* encodingCombo;
private slots:
	void slotHome();
};

class gtImporterDialog : public QDialog
{
	Q_OBJECT
private:
	QComboBox*   importerCombo;
	QCheckBox*   rememberCheck;
	QPushButton* okButton;
public:
	gtImporterDialog(const QStringList& importers, int currentSelection);
	~gtImporterDialog();
	bool shouldRemember();
	QString getImporter();
};

class gtDialogs
{
private:
	gtFileDialog* fdia;
	QString fileName;
	QString encoding;
	int importer;
	PrefsContext* prefs;
	QString pwd;
public:
	gtDialogs();
	~gtDialogs();
	const QString& getFileName();
	const QString& getEncoding();
	int getImporter();
	bool importTextOnly();
	bool runFileDialog(const QString& filters, const QStringList& importers);
	bool runImporterDialog(const QStringList& importers);
};

#endif // GTDIALOGS_H
