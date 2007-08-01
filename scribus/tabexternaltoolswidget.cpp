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
#include <QFileDialog>
#include <QProcess>
#include "util_ghostscript.h"
#include "scpaths.h"

TabExternalToolsWidget::TabExternalToolsWidget(struct ApplicationPrefs *prefsData, QWidget* parent, const char*name)
: QWidget(parent, name)
{
	setupUi(this);
	// switched off as it's called in main prefs classes - PV
	//restoreDefaults(prefsData);
	
	connect(psToolChangeButton, SIGNAL(clicked()), this, SLOT(changePostScriptTool()));
	connect(imageToolChangeButton, SIGNAL(clicked()), this, SLOT(changeImageTool()));
	connect(extBrowserToolChangeButton, SIGNAL(clicked()), this, SLOT(changeExtBrowserTool()));
	connect(latexToolChangeButton, SIGNAL(clicked()), this, SLOT(changeLatexTool()));
	connect(latexEditorChangeButton, SIGNAL(clicked()), this, SLOT(changeLatexEditor()));
	connect(rescanButton, SIGNAL(clicked()), this, SLOT(rescanForTools()));
}

TabExternalToolsWidget::~TabExternalToolsWidget()
{
}

const QString TabExternalToolsWidget::newPSTool() const 
{ 
	return ScPaths::separatorsToSlashes(psToolLineEdit->text()); 
}

const QString TabExternalToolsWidget::newImageTool() const 
{ 
	return ScPaths::separatorsToSlashes(imageToolLineEdit->text());
}

const QString TabExternalToolsWidget::newExtBrowserTool() const 
{ 
	return ScPaths::separatorsToSlashes(extBrowserToolLineEdit->text()); 
}

const QString TabExternalToolsWidget::newLatexTool() const 
{ 
	return ScPaths::separatorsToSlashes(latexToolLineEdit->text()); 
}

const QString TabExternalToolsWidget::newLatexExtension() const 
{ 
	return latexExtensionLineEdit->text(); 
}

const QString TabExternalToolsWidget::newLatexEditor() const 
{ 
	return ScPaths::separatorsToSlashes(latexEditorLineEdit->text());
}

void TabExternalToolsWidget::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	psToolLineEdit->setText(QDir::convertSeparators(prefsData->gs_exe));
	psAntialiasTextCheckBox->setChecked(prefsData->gs_AntiAliasText);
	psAntialiasGraphicsCheckBox->setChecked(prefsData->gs_AntiAliasGraphics);
	psResolutionSpinBox->setValue(prefsData->gs_Resolution);
	imageToolLineEdit->setText(QDir::convertSeparators(prefsData->imageEditorExecutable));
	extBrowserToolLineEdit->setText(QDir::convertSeparators(prefsData->extBrowserExecutable));
	latexToolLineEdit->setText(QDir::convertSeparators(prefsData->latexExecutable));
	latexExtensionLineEdit->setText(prefsData->latexExtension);
	latexResolutionSpinBox->setValue(prefsData->latexResolution);
	latexEditorLineEdit->setText(prefsData->latexEditorExecutable);
	latexForceDpiCheckBox->setCheckState(prefsData->latexForceDpi?Qt::Checked:Qt::Unchecked);
}

void TabExternalToolsWidget::changePostScriptTool()
{
	QFileInfo fi(psToolLineEdit->text());
	QString s = QFileDialog::getOpenFileName(fi.dirPath(), QString::null, this, "changeGhostscript", tr("Locate Ghostscript"));
	if (!s.isEmpty())
		psToolLineEdit->setText( QDir::convertSeparators(s) );
}

void TabExternalToolsWidget::changeImageTool()
{
	QFileInfo fi(imageToolLineEdit->text());
	QString s = QFileDialog::getOpenFileName(fi.dirPath(), QString::null, this, "changeImageEditor", tr("Locate your image editor"));
	if (!s.isEmpty())
		imageToolLineEdit->setText( QDir::convertSeparators(s) );
}

void TabExternalToolsWidget::changeExtBrowserTool()
{
	QFileInfo fi(extBrowserToolLineEdit->text());
	QString s = QFileDialog::getOpenFileName(fi.dirPath(), QString::null, this, "changeextBrowser", tr("Locate your web browser"));
	if (!s.isEmpty())
		extBrowserToolLineEdit->setText( QDir::convertSeparators(s) );
}

void TabExternalToolsWidget::changeLatexTool()
{
	QFileInfo fi(latexToolLineEdit->text());
	QString s = QFileDialog::getOpenFileName(fi.dirPath(), QString::null, this, "changeLatex", tr("Locate your LaTeX executable"));
	if (!s.isEmpty())
		latexToolLineEdit->setText( QDir::convertSeparators(s) );
}

void TabExternalToolsWidget::changeLatexEditor()
{
	QFileInfo fi(latexEditorLineEdit->text());
	QString s = QFileDialog::getOpenFileName(fi.dirPath(), QString::null, this, "changeLatex", tr("Locate your LaTeX editor"));
	if (!s.isEmpty())
		latexEditorLineEdit->setText( QDir::convertSeparators(s) );
}

bool TabExternalToolsWidget::fileInPath(QString file)
{
	if (file.isEmpty()) 
		return false;
	file = file.split(' ').at(0); //Ignore parameters
	
	file = ScPaths::separatorsToSlashes(file);
	if (file.indexOf('/') >= 0) {
		//Looks like an absolute path
		QFileInfo info(file);
		return info.exists();
	}
	
	//Get $PATH
	QStringList env = QProcess::systemEnvironment();
	QString path;
	foreach (QString line, env) {
		if (line.indexOf("PATH") == 0) {
			path = line.mid(5); //Strip "PATH="
			break;
		}
	}
	//std::cout << "path: " << qPrintable(path) << "\n";
	QStringList splitpath;
	//TODO: Check this again! OS2?
	#ifdef _WIN32
		splitpath = path.split(';');
	#else
		splitpath = path.split(':');
	#endif
	foreach (QString dir, splitpath) {
		QFileInfo info(dir, file);
		if (info.exists()) {
			return true;
		}
	}
	return false;
}

void TabExternalToolsWidget::rescanForTools()
{
	if (!fileInPath(psToolLineEdit->text()))
	{
		QString gsDef = getGSDefaultExeName();
		psToolLineEdit->setText( QDir::convertSeparators(gsDef) );
	}
	
	if (!fileInPath(imageToolLineEdit->text()))
		imageToolLineEdit->setText("gimp");
	
	if (!fileInPath(latexToolLineEdit->text())) {
		latexToolLineEdit->setText("latexpng %dpi");
		latexExtensionLineEdit->setText("1.png");
	}
	
	if (!fileInPath(latexEditorLineEdit->text())) {
		QStringList editors;
		editors << 
				/*Linux */ "kwrite" << "kate" << "gedit" << "gvim" <<
				/*Windows TODO*/ "notepad" << 
				/*Mac OS*/ "open";
		foreach (QString editor, editors) {
			if (fileInPath(editor)) {
				latexEditorLineEdit->setText(editor);
				break;
			}
		}
	}
}
