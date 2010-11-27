/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/**************************************************************************
*   Copyright (C) 2010 by Franz Schmid                                    *
*   franz.schmid@altmuehlnet.de                                           *
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
#ifndef PAINTMANAGER_H
#define PAINTMANAGER_H
#include "ui_paintmanagerbase.h"
#include "colorsetmanager.h"
#include "sccolor.h"
#include "scribusapi.h"
#include "scribusdoc.h"
#include "scribus.h"
#include "vgradient.h"
#include "scpattern.h"

class SCRIBUS_API PaintManagerDialog : public QDialog, Ui::PaintManagerBase
{
	Q_OBJECT

	public:
		PaintManagerDialog(QWidget* parent, QMap<QString, VGradient> *docGradients, ColorList doco, QString docColSet, QMap<QString, ScPattern> *docPatterns, ScribusDoc *doc, ScribusMainWindow* scMW);
		~PaintManagerDialog() {};
		/*! \brief Returns the name of the current/selected color set.
		\retval QString selected name. */
		QString getColorSetName();
		/*! \brief Returns currently selected ScColor.
		It's used e.g. in BarcodeGenerator plugin.
		*/
		ScColor selectedColor();
		/*! \brief Returns name of the currently selected color.
		It's used e.g. in BarcodeGenerator plugin.
		*/
		QString selectedColorName();
		ScribusDoc *m_doc;
		ScribusMainWindow *mainWin;
		QMap<QString, VGradient> dialogGradients;
		QMap<QString,QString> replaceMap;
		QMap<QString,QString> origNames;
		QStringList origGradients;
		ColorList m_colorList;
		QMap<QString,QString> replaceColorMap;
		//! \brief Custom user's color set
		QStringList customColSet;
		bool hasImportedColors;
		QMap<QString, ScPattern> dialogPatterns;
		QMap<QString,QString> replaceMapPatterns;
		QMap<QString,QString> origNamesPatterns;
		QStringList origPatterns;
	private slots:
		void leaveDialog();
		void slotRightClick(QPoint p);
		void selEditColor(QTreeWidgetItem *it);
		void itemSelectionChanged();
		void itemSelected(QTreeWidgetItem* it);
		void createNew();
		void editColorItem();
		void duplicateColorItem();
		void removeColorItem();
		void removeUnusedColorItem();
		void importColorItems();
		void loadDefaults(QTreeWidgetItem* item);
		void saveDefaults();
	protected:
		QTreeWidgetItem* updatePatternList(QString addedName = "");
		QTreeWidgetItem* updateGradientList(QString addedName = "");
		QTreeWidgetItem* updateColorList(QString addedName = "");
		ColorList getGradientColors();
		void loadPatternDir();
		void loadVectors(QString data);
		void updateGradientColors(QString newName, QString oldName);
		void loadGimpFormat(QString fileName);
		void addGimpColor(QString &colorName, double r, double g, double b);
		void loadScribusFormat(QString fileName);
		QStringList getUsedPatternsHelper(QString pattern, QStringList &results);
		void doSaveDefaults(QString name, bool changed = false);
		ColorSetManager csm;
		bool paletteLocked;
		bool modified;
		int sortRule;
		QTreeWidgetItem *colorItems;
		QTreeWidgetItem *gradientItems;
		QTreeWidgetItem *patternItems;
		QTreeWidgetItem *systemSwatches;
		QTreeWidgetItem *userSwatches;
		ColorList inDocUsedColors;
};

#endif
