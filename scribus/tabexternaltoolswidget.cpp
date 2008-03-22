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
#include "prefsstructs.h"
#include "latexhelpers.h"

TabExternalToolsWidget::TabExternalToolsWidget(struct ApplicationPrefs *prefsData, QWidget* parent)
: QWidget(parent)
{
	setupUi(this);
	// switched off as it's called in main prefs classes - PV
	//restoreDefaults(prefsData);
	
	connect(psToolChangeButton, SIGNAL(clicked()), this, SLOT(changePostScriptTool()));
	connect(imageToolChangeButton, SIGNAL(clicked()), this, SLOT(changeImageTool()));
	connect(extBrowserToolChangeButton, SIGNAL(clicked()), this, SLOT(changeExtBrowserTool()));
	connect(latexEditorChangeButton, SIGNAL(clicked()), this, SLOT(changeLatexEditor()));
	connect(rescanButton, SIGNAL(clicked()), this, SLOT(rescanForTools()));
	connect(latexConfigUpButton, SIGNAL(clicked()), this, SLOT(upButtonPressed()));
	connect(latexConfigDownButton, SIGNAL(clicked()), this, SLOT(downButtonPressed()));
	connect(latexConfigAddButton, SIGNAL(clicked()), this, SLOT(addConfig()));
	connect(latexConfigDeleteButton, SIGNAL(clicked()), this, SLOT(deleteConfig()));
	connect(latexEmbeddedEditorCheckBox, SIGNAL(stateChanged(int)), 
			this, SLOT(changeLatexEmbeddedActive(int)));
}

TabExternalToolsWidget::~TabExternalToolsWidget()
{
}

const QString TabExternalToolsWidget::newPSTool() const 
{ 
	return QDir::fromNativeSeparators(psToolLineEdit->text()); 
}

const QString TabExternalToolsWidget::newImageTool() const 
{ 
	return QDir::fromNativeSeparators(imageToolLineEdit->text());
}

const QString TabExternalToolsWidget::newExtBrowserTool() const 
{ 
	return QDir::fromNativeSeparators(extBrowserToolLineEdit->text()); 
}

const QString TabExternalToolsWidget::newLatexEditor() const 
{
	return QDir::fromNativeSeparators(latexEditorLineEdit->text());
}

const QStringList TabExternalToolsWidget::newLatexConfigs() const
{
	QStringList list;
	int i;
	for (i=0; i < latexConfigsListWidget->count(); i++) {
		list.append(latexConfigsListWidget->item(i)->data(Qt::UserRole).toString());
	}
	
	return list;
}

void TabExternalToolsWidget::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	psToolLineEdit->setText(QDir::convertSeparators(prefsData->gs_exe));
	psAntialiasTextCheckBox->setChecked(prefsData->gs_AntiAliasText);
	psAntialiasGraphicsCheckBox->setChecked(prefsData->gs_AntiAliasGraphics);
	psResolutionSpinBox->setValue(prefsData->gs_Resolution);
	imageToolLineEdit->setText(QDir::convertSeparators(prefsData->imageEditorExecutable));
	extBrowserToolLineEdit->setText(QDir::convertSeparators(prefsData->extBrowserExecutable));
	latexResolutionSpinBox->setValue(prefsData->latexResolution);
	latexEditorLineEdit->setText(prefsData->latexEditorExecutable);
	latexForceDpiCheckBox->setCheckState(prefsData->latexForceDpi?Qt::Checked:Qt::Unchecked);
	latexEmbeddedEditorCheckBox->setCheckState(prefsData->latexUseEmbeddedEditor?Qt::Checked:Qt::Unchecked);
	latexEmptyFrameCheckBox->setCheckState(prefsData->latexStartWithEmptyFrames?Qt::Checked:Qt::Unchecked);
	latexConfigsListWidget->clear();
	QStringList configs = prefsData->latexConfigs;
	foreach (QString config, configs) {
		insertConfigItem(config);
	}
	latexConfigsListWidget->setCurrentRow(0);
}

void TabExternalToolsWidget::changePostScriptTool()
{
	QFileInfo fi(psToolLineEdit->text());
	QString s = QFileDialog::getOpenFileName(this, tr("Locate Ghostscript"), fi.path());
	if (!s.isEmpty())
		psToolLineEdit->setText( QDir::convertSeparators(s) );
}

void TabExternalToolsWidget::changeImageTool()
{
	QFileInfo fi(imageToolLineEdit->text());
	QString s = QFileDialog::getOpenFileName(this, tr("Locate your image editor"), fi.path());
	if (!s.isEmpty())
		imageToolLineEdit->setText( QDir::convertSeparators(s) );
}

void TabExternalToolsWidget::changeExtBrowserTool()
{
	QFileInfo fi(extBrowserToolLineEdit->text());
	QString s = QFileDialog::getOpenFileName(this, tr("Locate your web browser"), fi.path());
	if (!s.isEmpty())
		extBrowserToolLineEdit->setText( QDir::convertSeparators(s) );
}

void TabExternalToolsWidget::changeLatexEditor()
{
	QFileInfo fi(latexEditorLineEdit->text());
	QString s = QFileDialog::getOpenFileName(this, tr("Locate your editor"), fi.path());
	if (!s.isEmpty())
		latexEditorLineEdit->setText( QDir::convertSeparators(s) );
}

void TabExternalToolsWidget::insertConfigItem(QString config, int row)
{
	QListWidgetItem *widget = new QListWidgetItem(
			QString("%1 (%2)").arg(LatexConfigCache::instance()->parser(config)->description()).
			arg(QDir::toNativeSeparators(QDir::cleanPath(config))));
	widget->setData(Qt::UserRole, config);
	if (row == -1) {
		latexConfigsListWidget->addItem(widget);
	} else {
		latexConfigsListWidget->insertItem(row, widget);
	}
	latexConfigsListWidget->setCurrentItem(widget);
}

void TabExternalToolsWidget::addConfig()
{
	QString s = QFileDialog::getOpenFileName(this, 
		tr("Locate a Configuration file"), 
		ScPaths::instance().shareDir() + "/editorconfig/",
		tr("Configuration files")+" (*.xml)");
	if (!s.isEmpty()) {
		insertConfigItem(s);
	}
}

void TabExternalToolsWidget::deleteConfig()
{
	if (latexConfigsListWidget->currentItem()) {
		delete latexConfigsListWidget->currentItem();
	}
}

void TabExternalToolsWidget::upButtonPressed()
{
	if (latexConfigsListWidget->currentRow() < 1) return;
	QListWidgetItem *old = latexConfigsListWidget->currentItem();
	QString config = old->data(Qt::UserRole).toString();
	insertConfigItem(config, latexConfigsListWidget->currentRow()-1);
	delete old;
}

void TabExternalToolsWidget::downButtonPressed()
{
	if (latexConfigsListWidget->currentRow() >= latexConfigsListWidget->count()-1) return;
	QListWidgetItem *old = latexConfigsListWidget->currentItem();
	QString config = old->data(Qt::UserRole).toString();
	insertConfigItem(config, latexConfigsListWidget->currentRow()+2);
	delete old;
}


bool TabExternalToolsWidget::fileInPath(QString file)
{
	if (file.isEmpty()) 
		return false;
	file = file.split(' ', QString::SkipEmptyParts).at(0); //Ignore parameters
	
	file = QDir::fromNativeSeparators(file);
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
	QStringList splitpath;
	//TODO: Check this again! OS2?
	#ifdef _WIN32
		splitpath = path.split(';', QString::SkipEmptyParts);
	#else
		splitpath = path.split(':', QString::SkipEmptyParts);
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
	
	if (!fileInPath(latexEditorLineEdit->text())) {
		QStringList editors;
		editors << 
				/*Linux */ "kwrite" << "kate" << "gedit" << "gvim" <<
				/*Windows */ "notepad" << 
				/*Mac OS*/ "open";
		foreach (QString editor, editors) {
			if (fileInPath(editor)) {
				latexEditorLineEdit->setText(editor);
				break;
			}
		}
	}
}

void TabExternalToolsWidget::changeLatexEmbeddedActive(int state)
{
	bool enable = state == Qt::Unchecked;
	latexEditorLineEdit->setEnabled(enable);
	latexEditorChangeButton->setEnabled(enable);
}
