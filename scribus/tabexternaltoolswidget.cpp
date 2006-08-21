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

#include "tabexternaltoolswidget.h"
#include "tabexternaltoolswidget.moc"
#include "gsutil.h"

TabExternalToolsWidget::TabExternalToolsWidget(struct ApplicationPrefs *prefsData, QWidget* parent, const char*name)
: TabExternalToolsWidgetBase(parent, name)
{
	// switched off as it's called in main prefs classes - PV
	//restoreDefaults(prefsData);
	
	connect(psToolChangeButton, SIGNAL(clicked()), this, SLOT(changePostScriptTool()));
	connect(imageToolChangeButton, SIGNAL(clicked()), this, SLOT(changeImageTool()));
	connect(extBrowserToolChangeButton, SIGNAL(clicked()), this, SLOT(changeExtBrowserTool()));
	connect(rescanButton, SIGNAL(clicked()), this, SLOT(rescanForTools()));
}

TabExternalToolsWidget::~TabExternalToolsWidget()
{
}

void TabExternalToolsWidget::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	psToolLineEdit->setText(QDir::convertSeparators(prefsData->gs_exe));
	psAntialiasTextCheckBox->setChecked(prefsData->gs_AntiAliasText);
	psAntialiasGraphicsCheckBox->setChecked(prefsData->gs_AntiAliasGraphics);
	psResolutionSpinBox->setValue(prefsData->gs_Resolution);
	imageToolLineEdit->setText(QDir::convertSeparators(prefsData->imageEditorExecutable));
	extBrowserToolLineEdit->setText(QDir::convertSeparators(prefsData->extBrowserExecutable));
}

void TabExternalToolsWidget::changePostScriptTool()
{
	QFileInfo fi(psToolLineEdit->text());
	QString s = QFileDialog::getOpenFileName(fi.dirPath(), QString::null, this, "changeGhostscript", tr("Locate Ghostscript"));
	if (!s.isEmpty())
		psToolLineEdit->setText(s);
}

void TabExternalToolsWidget::changeImageTool()
{
	QFileInfo fi(imageToolLineEdit->text());
	QString s = QFileDialog::getOpenFileName(fi.dirPath(), QString::null, this, "changeImageEditor", tr("Locate your image editor"));
	if (!s.isEmpty())
		imageToolLineEdit->setText(s);
}

void TabExternalToolsWidget::changeExtBrowserTool()
{
	QFileInfo fi(extBrowserToolLineEdit->text());
	QString s = QFileDialog::getOpenFileName(fi.dirPath(), QString::null, this, "changeextBrowser", tr("Locate your web browser"));
	if (!s.isEmpty())
		extBrowserToolLineEdit->setText(s);
}

void TabExternalToolsWidget::rescanForTools()
{
	QFileInfo fi(psToolLineEdit->text());
	if (!fi.exists())
		psToolLineEdit->setText(getGSDefaultExeName());
	QFileInfo fi2(imageToolLineEdit->text());
	if (!fi2.exists())
		imageToolLineEdit->setText("gimp");
}
