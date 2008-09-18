/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
*   Copyright (C) 2005 by Craig Bradney                                   *
*   cbradney@zip.com.au                                                   *
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

#ifndef TABEXTERNALTOOLSWIDGET_H
#define TABEXTERNALTOOLSWIDGET_H

#include "ui_tabexternaltoolswidget.h"
#include "scribusapi.h"

struct ApplicationPrefs;

class SCRIBUS_API TabExternalToolsWidget : public QWidget, Ui::TabExternalToolsWidget
{
	Q_OBJECT

	public:
		TabExternalToolsWidget(struct ApplicationPrefs *prefsData, QWidget* parent=0);
		~TabExternalToolsWidget();
		void restoreDefaults(struct ApplicationPrefs *prefsData);
		
		const QString newPSTool() const;
		const QString newImageTool() const;
		const QString newExtBrowserTool() const;
		const QString newLatexEditor() const;
		const QStringList newLatexConfigs() const;
		bool newAntialiasText() const { return psAntialiasTextCheckBox->isChecked();}
		bool newAntialiasGraphics() const { return psAntialiasGraphicsCheckBox->isChecked();}
		int newPSToolResolution() const { return psResolutionSpinBox->value();}
		int newLatexToolResolution() const { return latexResolutionSpinBox->value();}
		bool newLatexForceDpi() const { return latexForceDpiCheckBox->checkState() == Qt::Checked;}
		bool newLatexStartWithEmptyFrames() const { return latexEmptyFrameCheckBox->checkState() == Qt::Checked;}
		
	protected:
		bool fileInPath(QString file);
		void insertConfigItem(QString config, int row = -1);
		
	public slots:
		void changePostScriptTool();
		void changeImageTool();
		void changeExtBrowserTool();
		void changeLatexEditor();
		void rescanForTools();
		void upButtonPressed();
		void downButtonPressed();
		void addConfig();
		void deleteConfig();
		void changePath();
};

#endif
