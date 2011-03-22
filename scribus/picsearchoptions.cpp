/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          picsearchoptions.cpp  -  description
                             -------------------
    begin                : Mon Feb 26 2007
    copyright            : (C) 2007 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <cassert>

#include <QCheckBox>
#include <QDir>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QStringList>
#include <QToolTip>

#include "picsearchoptions.h"
#include "filesearch.h"

PicSearchOptions::PicSearchOptions(QWidget* parent, const QString & fileName, const QString & searchBase) : QDialog( parent )
{
	setupUi(this);
	setModal(true);
	m_bCancelled = false;
	m_strFileName = fileName;
	m_strLastDirSearched = searchBase;

	fileEdit->setText(m_strFileName);
	directoryEdit->setText(m_strLastDirSearched);
	progressBar1->hide();
	searchLabel->hide();
	connect(startButton, SIGNAL(clicked()), this, SLOT(slotSearchPic()));
	connect(changeDirButton, SIGNAL(clicked()), this, SLOT(slotChangeSearchDir()));

	caseInsensitiveCheck->setToolTip("<qt>" + tr("The filesystem will be searched for case insensitive file names when you check this on. Remember it is not default on most operating systems except MS Windows") + "</qt>");
	}

void PicSearchOptions::setSearchButton(bool toCancel, const FileSearch* searcher)
{
	if (toCancel)
	{
		startButton->setText(tr("Cancel Search"));
		progressBar1->reset();
		progressBar1->show();
		searchLabel->show();
		disconnect(startButton, SIGNAL(clicked()), this, SLOT(slotSearchPic()));
		connect(startButton, SIGNAL(clicked()), searcher, SLOT(cancel()));
	}
	else
	{
		startButton->setText( tr("Start Search"));
		progressBar1->reset();
		progressBar1->hide();
		searchLabel->hide();
		disconnect(startButton, SIGNAL(clicked()), searcher, SLOT(cancel()));
		connect(startButton, SIGNAL(clicked()), this, SLOT(slotSearchPic()));
	}
}

void PicSearchOptions::slotChangeSearchDir()
{
	QString workDir = directoryEdit->text();
#ifndef _WIN32
	if (workDir.isEmpty() || !QDir().exists(workDir))
	{
		workDir = m_strLastDirSearched;
		if (workDir.isEmpty() || !QDir().exists(workDir))
		{
			workDir = QDir::homePath();
		}
	}
#else 
	// TODO: and on _WIN32 workDir is ... ?
#endif
	QString searchBase = QFileDialog::getExistingDirectory(this, tr("Select a base directory for search"), workDir);
	if (searchBase.isEmpty() || !QDir().exists(searchBase))
		return;
	m_strLastDirSearched = searchBase;
	directoryEdit->setText(m_strLastDirSearched);
}

void PicSearchOptions::slotSearchPic()
{
	QString searchBase = directoryEdit->text();
	if (searchBase.isEmpty() || !QDir().exists(searchBase))
	{
		if (QMessageBox::warning(this, tr("Scribus - Image Search"), tr("Base directory for search does not exist.\nPlease choose another one."),
			QMessageBox::Ok | QMessageBox::Default | QMessageBox::Escape | QMessageBox::Cancel,
			QMessageBox::NoButton) != QMessageBox::Ok)
			return;
		
		slotChangeSearchDir();
		searchBase = directoryEdit->text();
		if (searchBase.isEmpty() || !QDir().exists(searchBase))
			return;
	}
	m_strLastDirSearched = searchBase;
	// Set up the search, then return to the event loop until it notifies us
	// that it's done.
	// Note: search will be deleted when this PicStatus is, so there's no
	// need to worry about cleanup.
	// case sensitive note: it has no meaning on windows
	int recurse = (recursiveSearch->isChecked()) ? -1 : 0;
	FileSearch* search = new FileSearch(this, fileEdit->text(), m_strLastDirSearched, recurse, caseInsensitiveCheck->isChecked());
	Q_CHECK_PTR(search);
	connect(search, SIGNAL(searchComplete(const QStringList&, const QString&)), SLOT(slotSearchPicFinished(const QStringList&, const QString&)));
	connect(search, SIGNAL(aborted(bool)), SLOT(slotSearchPicAborted(bool)));
	// Set up the UI to let the user cancel the search, then start it
	setSearchButton(true, search);
	progressBar1->setRange(0, 0);
	enableGuiWhileSearching(false);
	search->start();
}

void PicSearchOptions::slotSearchPicAborted(bool userCancelled)
{
	m_bCancelled = userCancelled;
	const FileSearch* search = dynamic_cast<const FileSearch*>(sender());
	assert(search);
	setSearchButton(false, search);
	enableGuiWhileSearching(true);
	if (!userCancelled)
		// A running search failed
		QMessageBox::warning(this, tr("Scribus - Image Search"), tr("The search failed: %1").arg(search->lastError()),
				QMessageBox::Ok|QMessageBox::Default|QMessageBox::Escape,
				QMessageBox::NoButton);
	reject();
}

void PicSearchOptions::slotSearchPicFinished(const QStringList & matches, const QString & fileName)
{
	enableGuiWhileSearching(true);
	m_listMatches = matches;
	m_strFileName = fileEdit->text();
	accept();
}

void PicSearchOptions::enableGuiWhileSearching(bool enable)
{
	fileEdit->setEnabled(enable);
	directoryEdit->setEnabled(enable);
	changeDirButton->setEnabled(enable);
	caseInsensitiveCheck->setEnabled(enable);
	recursiveSearch->setEnabled(enable);
}
