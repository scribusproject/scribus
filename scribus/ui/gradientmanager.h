/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
*   Copyright (C) 2009 by Franz Schmid                                   *
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

#ifndef GRADIENTMDIALOG_H
#define GRADIENTMDIALOG_H

#include <QMap>
#include <QString>
#include <QDialog>
#include <QListWidgetItem>
#include "ui_gradientmanager.h"
#include "scribusapi.h"
#include "scribusdoc.h"
#include "scribus.h"
#include "vgradient.h"

class SCRIBUS_API gradientManagerDialog : public QDialog, Ui::gradientManager
{
	Q_OBJECT

	public:
		gradientManagerDialog(QWidget* parent, QMap<QString, VGradient> *docGradients, ColorList doco, ScribusDoc *doc, ScribusMainWindow* scMW);
		~gradientManagerDialog() {};
		void updateGradientList();
		void loadScribusFormat(QString fileName);
		ScribusDoc *m_doc;
		ScribusMainWindow *mainWin;
		QMap<QString, VGradient> dialogGradients;
		QMap<QString,QString> replaceMap;
		QMap<QString,QString> origNames;
		QStringList origGradients;
		ColorList m_colorList;
		bool hasImportedColors;
	public slots:
		void gradientSelected(QListWidgetItem* it);
		void addGradient();
		void editGradient();
		void removeGradient();
		void removeAllGradients();
		void loadGradients();
		void saveGradients();
};

#endif
