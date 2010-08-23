/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QFileDialog>
#include <QInputDialog>
#include <QProcess>
#include <QMessageBox>
#include "scpaths.h"
#include "prefsstructs.h"
#include "latexhelpers.h"
#include "commonstrings.h"
#include "util_file.h"
#include "util_ghostscript.h"


#include "prefs_externaltools.h"

Prefs_ExternalTools::Prefs_ExternalTools(QWidget* parent, ScribusDoc* doc)
	: Prefs_Pane(parent)
{
	setupUi(this);
	connect(psToolChangeButton, SIGNAL(clicked()), this, SLOT(changePostScriptTool()));
	connect(imageToolChangeButton, SIGNAL(clicked()), this, SLOT(changeImageTool()));
	connect(webBrowserChangeButton, SIGNAL(clicked()), this, SLOT(changeWebBrowser()));
	connect(rescanButton, SIGNAL(clicked()), this, SLOT(rescanForTools()));
	connect(uniconvertorChangeButton, SIGNAL(clicked()), this, SLOT(changeUniconvertor()));
	connect(pdfViewerChangeButton, SIGNAL(clicked()), this, SLOT(changePDFViewer()));
	connect(latexConfigUpButton, SIGNAL(clicked()), this, SLOT(upButtonPressed()));
	connect(latexConfigDownButton, SIGNAL(clicked()), this, SLOT(downButtonPressed()));
	connect(latexConfigAddButton, SIGNAL(clicked()), this, SLOT(addConfig()));
	connect(latexConfigDeleteButton, SIGNAL(clicked()), this, SLOT(deleteConfig()));
	connect(latexEditorChangeButton, SIGNAL(clicked()), this, SLOT(changeLatexEditor()));
	connect(latexConfigChangeCommandButton, SIGNAL(clicked()), this, SLOT(changeLatexPath()));

}

Prefs_ExternalTools::~Prefs_ExternalTools()
{
}

void Prefs_ExternalTools::languageChange()
{
}

void Prefs_ExternalTools::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	psToolLineEdit->setText(QDir::convertSeparators(prefsData->extToolPrefs.gs_exe));
	psAntialiasTextCheckBox->setChecked(prefsData->extToolPrefs.gs_AntiAliasText);
	psAntialiasGraphicsCheckBox->setChecked(prefsData->extToolPrefs.gs_AntiAliasGraphics);
	psResolutionSpinBox->setValue(prefsData->extToolPrefs.gs_Resolution);
	imageToolLineEdit->setText(QDir::convertSeparators(prefsData->extToolPrefs.imageEditorExecutable));
	webBrowserLineEdit->setText(QDir::convertSeparators(prefsData->extToolPrefs.extBrowserExecutable));
	uniconvertorLineEdit->setText(QDir::convertSeparators(prefsData->extToolPrefs.uniconvExecutable));
	latexResolutionSpinBox->setValue(prefsData->extToolPrefs.latexResolution);
	latexEditorLineEdit->setText(prefsData->extToolPrefs.latexEditorExecutable);
	latexForceDPICheckBox->setCheckState(prefsData->extToolPrefs.latexForceDpi?Qt::Checked:Qt::Unchecked);
	latexEmptyFrameCheckBox->setCheckState(prefsData->extToolPrefs.latexStartWithEmptyFrames?Qt::Checked:Qt::Unchecked);
	latexConfigsListWidget->clear();
	QStringList configs = prefsData->extToolPrefs.latexConfigs;
	commands = prefsData->extToolPrefs.latexCommands;
	foreach (QString config, configs)
		insertConfigItem(config);
	latexConfigsListWidget->setCurrentRow(0);
	pdfViewerLineEdit->setText(QDir::convertSeparators(prefsData->extToolPrefs.pdfViewerExecutable));
}

void Prefs_ExternalTools::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->extToolPrefs.gs_AntiAliasGraphics = psAntialiasGraphicsCheckBox->isChecked();
	prefsData->extToolPrefs.gs_AntiAliasText = psAntialiasTextCheckBox->isChecked();
	prefsData->extToolPrefs.gs_exe=QDir::fromNativeSeparators(psToolLineEdit->text());
	prefsData->extToolPrefs.gs_Resolution = psResolutionSpinBox->value();;

	prefsData->extToolPrefs.imageEditorExecutable=QDir::fromNativeSeparators(imageToolLineEdit->text());
	prefsData->extToolPrefs.extBrowserExecutable=QDir::fromNativeSeparators(webBrowserLineEdit->text());

	prefsData->extToolPrefs.latexResolution = latexResolutionSpinBox->value();
	prefsData->extToolPrefs.latexForceDpi = latexForceDPICheckBox->checkState() == Qt::Checked;
	prefsData->extToolPrefs.latexStartWithEmptyFrames = latexEmptyFrameCheckBox->checkState() == Qt::Checked;
	QStringList configsList;
	for (int i=0; i < latexConfigsListWidget->count(); i++)
		configsList.append(latexConfigsListWidget->item(i)->data(Qt::UserRole).toString());
	prefsData->extToolPrefs.latexConfigs=configsList;
	prefsData->extToolPrefs.latexCommands=commands;
	prefsData->extToolPrefs.latexEditorExecutable=QDir::fromNativeSeparators(latexEditorLineEdit->text());
	prefsData->extToolPrefs.uniconvExecutable=QDir::fromNativeSeparators(uniconvertorLineEdit->text());
	prefsData->extToolPrefs.pdfViewerExecutable=QDir::fromNativeSeparators(pdfViewerLineEdit->text());
}

void Prefs_ExternalTools::changePostScriptTool()
{
	QFileInfo fi(psToolLineEdit->text());
	QString s = QFileDialog::getOpenFileName(this, tr("Locate Ghostscript"), fi.path());
	if (!s.isEmpty())
		psToolLineEdit->setText( QDir::convertSeparators(s) );
}

void Prefs_ExternalTools::changeImageTool()
{
	QFileInfo fi(imageToolLineEdit->text());
	QString s = QFileDialog::getOpenFileName(this, tr("Locate your image editor"), fi.path());
	if (!s.isEmpty())
		imageToolLineEdit->setText( QDir::convertSeparators(s) );
}

void Prefs_ExternalTools::changeWebBrowser()
{
	QFileInfo fi(webBrowserLineEdit->text());
	QString s = QFileDialog::getOpenFileName(this, tr("Locate your web browser"), fi.path());
	if (!s.isEmpty())
		webBrowserLineEdit->setText( QDir::convertSeparators(s) );
}

void Prefs_ExternalTools::changePDFViewer()
{
	QFileInfo fi(pdfViewerLineEdit->text());
	QString s = QFileDialog::getOpenFileName(this, tr("Locate your PDF viewer"), fi.path());
	if (!s.isEmpty())
		pdfViewerLineEdit->setText( QDir::convertSeparators(s) );
}

void Prefs_ExternalTools::changeUniconvertor()
{
	QFileInfo fi(uniconvertorLineEdit->text());
	QString s = QFileDialog::getOpenFileName(this, tr("Locate the uniconvertor executable"), fi.path());
	if (!s.isEmpty())
		uniconvertorLineEdit->setText( QDir::convertSeparators(s) );
}

void Prefs_ExternalTools::changeLatexEditor()
{
	QFileInfo fi(latexEditorLineEdit->text());
	QString s = QFileDialog::getOpenFileName(this, tr("Locate your editor"), fi.path());
	if (!s.isEmpty())
	{
		s = QDir::convertSeparators(s);
		if (s.contains(' ') && !s.contains('"'))
			s = QString("\"%1\"").arg(s);
		latexEditorLineEdit->setText(s);
	}
}

void Prefs_ExternalTools::rescanForTools()
{
	if (!fileInPath(psToolLineEdit->text()))
	{
		QString gsDef = getGSDefaultExeName();
		psToolLineEdit->setText( QDir::convertSeparators(gsDef) );
	}

	if (!fileInPath(imageToolLineEdit->text()))
		imageToolLineEdit->setText("gimp");

	if (!fileInPath(uniconvertorLineEdit->text()))
	{
		if (fileInPath("uniconvertor"))
			uniconvertorLineEdit->setText("uniconvertor");
		else if (fileInPath("uniconv"))
			uniconvertorLineEdit->setText("uniconv");
		else
		{
			QMessageBox::warning(0, CommonStrings::trWarning, tr("Uniconvertor executable not found!"), 1, 0, 0);
			uniconvertorLineEdit->setText("");
		}
	}

	if (!fileInPath(latexEditorLineEdit->text()))
	{
		QStringList editors;
		editors <<
				/*Linux */ "kwrite" << "kate" << "gedit" << "gvim" <<
				/*Windows */ "notepad" <<
				/*Mac OS*/ "open";
		foreach (QString editor, editors)
		{
			if (fileInPath(editor))
			{
				latexEditorLineEdit->setText(editor);
				break;
			}
		}
	}

	//Scan for render frame render applications
	for (int i=0; i < latexConfigsListWidget->count(); i++)
	{
		QString config(latexConfigsListWidget->item(i)->data(Qt::UserRole).toString());
		QString oldCommand = commands[config];
		if (config=="100_latex.xml")
		{
			if (!fileInPath(oldCommand))
			{
				QStringList pdflatexPaths;
#ifdef Q_OS_MAC
				pdflatexPaths	<<"/opt/local/bin/pdflatex"
								<<"/sw/bin/pdflatex"
								<<"/usr/local/texlive/2009/bin/universal-darwin/pdflatex"
								<<"/usr/local/texlive/2008/bin/universal-darwin/pdflatex";
#endif
#ifdef Q_OS_LINUX
				pdflatexPaths	<<"/usr/local/bin/pdflatex"
								<<"/usr/bin/pdflatex";
#endif
				QString parms(" --interaction nonstopmode");
				for (int i = 0; i < pdflatexPaths.size(); ++i) //do nothing when we have no paths.. need some more from other OSes
				{
					QString cmd(pdflatexPaths.at(i));
					QFileInfo fInfo(cmd);
					if (fInfo.exists())
					{
						cmd.append(parms);
						int ret = QMessageBox::question(this, tr("LaTeX Command"),
								tr("Scribus has found the following pdflatex command:\n%1\nDo you want to use this?").arg(cmd),
								QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
						if (ret==QMessageBox::Yes)
						{
							commands[config]=cmd;
							setConfigItemText(latexConfigsListWidget->item(i));
						}
					}
				}
			}
		}
	}
}

void Prefs_ExternalTools::upButtonPressed()
{
	if (latexConfigsListWidget->currentRow() < 1)
		return;
	QListWidgetItem *old = latexConfigsListWidget->currentItem();
	QString config = old->data(Qt::UserRole).toString();
	insertConfigItem(config, latexConfigsListWidget->currentRow()-1);
	delete old;
}

void Prefs_ExternalTools::downButtonPressed()
{
	if (latexConfigsListWidget->currentRow() >= latexConfigsListWidget->count()-1)
		return;
	QListWidgetItem *old = latexConfigsListWidget->currentItem();
	QString config = old->data(Qt::UserRole).toString();
	insertConfigItem(config, latexConfigsListWidget->currentRow()+2);
	delete old;
}

void Prefs_ExternalTools::addConfig()
{
	QString s = QFileDialog::getOpenFileName(this,
		tr("Locate a Configuration file"),
		LatexConfigParser::configBase(),
		tr("Configuration files")+" (*.xml)");
	if (!s.isEmpty())
		insertConfigItem(s);
}

void Prefs_ExternalTools::deleteConfig()
{
	if (latexConfigsListWidget->currentItem())
		delete latexConfigsListWidget->currentItem();
}

void Prefs_ExternalTools::changeLatexPath()
{
	QListWidgetItem *item = latexConfigsListWidget->currentItem();
	QString config = item->data(Qt::UserRole).toString();
	//TODO: Better dialog
	QString oldCommand = commands[config];
	if (oldCommand.isEmpty())
		oldCommand = LatexConfigCache::instance()->parser(config)->executable();
	bool ok=false;
	QString newCommand = QInputDialog::getText(this, tr("Change Command"),
		tr("Enter new command: (leave empty to reset to default command; use quotes around arguments with spaces)"), QLineEdit::Normal, oldCommand, &ok);
	if (ok)
	{
		commands[config] = newCommand;
		setConfigItemText(item);
	}
}

void Prefs_ExternalTools::insertConfigItem(QString config, int row)
{
	QListWidgetItem *item = new QListWidgetItem();
	item->setData(Qt::UserRole, config);
	setConfigItemText(item);
	if (row == -1)
		latexConfigsListWidget->addItem(item);
	else
		latexConfigsListWidget->insertItem(row, item);
	latexConfigsListWidget->setCurrentItem(item);
}

void Prefs_ExternalTools::setConfigItemText(QListWidgetItem *item)
{
	QString config = item->data(Qt::UserRole).toString();
	QString description = LatexConfigCache::instance()->parser(config)->description();
	QString command = commands[config];
	if (command.isEmpty())
		item->setText(description);
	else
	{
		item->setText(QString("%1 (" + tr("Command: ") + "%2)" ).
			arg(description).
			arg(QDir::toNativeSeparators(QDir::cleanPath(command))));
	}
}

