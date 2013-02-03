/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
*   Copyright (C) 2008 by Franz Schmid                                     *
*   franz.schmid@altmuehlnet.de                                            *
*                                                                          *
*   This program is free software; you can redistribute it and/or modify   *
*   it under the terms of the GNU General Public License as published by   *
*   the Free Software Foundation; either version 2 of the License, or      *
*   (at your option) any later version.                                    *
*                                                                          *
*   This program is distributed in the hope that it will be useful,        *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
*   GNU General Public License for more details.                           *
*                                                                          *
*   You should have received a copy of the GNU General Public License      *
*   along with this program; if not, write to the                          *
*   Free Software Foundation, Inc.,                                        *
*   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.              *
****************************************************************************/

#ifndef PATHCONNECTDIALOG_H
#define PATHCONNECTDIALOG_H

#include <QDialog>
#include "ui_pathconnectdialogbase.h"
#include "pluginapi.h"
#include "scribusdoc.h"
#include "scribus.h"

class PLUGIN_API PathConnectDialog : public QDialog, Ui::PathConnectDialogBase
{
	Q_OBJECT

public:
	PathConnectDialog(QWidget* parent);
	~PathConnectDialog() {};
	int getFirstLinePoint();
	int getSecondLinePoint();
	int getMode();
signals:
	void updateValues(int, int, int, int);
private slots:
	void pointsChanged();
	void togglePreview();
};

#endif
