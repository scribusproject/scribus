/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
*   Copyright (C) 2016 by Franz Schmid                                   *
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
*   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
***************************************************************************/

#ifndef RECOVERDIALOG_H
#define RECOVERDIALOG_H

#include <QDialog>
#include "ui_recoverdialog.h"
#include "scribusapi.h"

class SCRIBUS_API RecoverDialog : public QDialog, Ui::recoverDialogBase
{
	Q_OBJECT

public:
	RecoverDialog(QWidget* parent, QStringList files);
	~RecoverDialog() {};
	QStringList recoverFiles;
	QStringList recoverNames;
private:
	void updateFilesTable();
	QStringList m_files;
private slots:
	void toggleAllfromHeader(int col);
	void handleItemClick();
	void doRecover();
	void doRemove();
};

#endif
