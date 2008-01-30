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
#include <QMessageBox>
#include <QStringList>
#include <QDir>
#include <QLineEdit>
#include <QToolTip>
#include <QPushButton>
#include <QToolTip>
#include <QCheckBox>
#include <QFileDialog>
#include <QLabel>
#include "picsearchoptions.h"
#include "filesearch.h"

PicSearchOptions::PicSearchOptions(QWidget* parent, const QString & fileName, const QString & searchBase) : QDialog( parent )
{
	setupUi(this);
	setModal(true);
	m_fileName = fileName;
	fileEdit->setText(fileName);
	directoryEdit->setText(searchBase);
	progressBar1->hide();
	searchLabel->hide();
	connect(startButton, SIGNAL(clicked()), this, SLOT(SearchPic()));
	connect(changeDirButton, SIGNAL(clicked()), this, SLOT(changeSearchDir()));
	caseInsensitiveCheck->setToolTip( "<qt>" + tr("The filesystem will be searched for case insensitive file names when you check this on. Remember it is not default on most operating systems except MS Windows") + "</qt>");
}

void PicSearchOptions::setSearchButton(bool toCancel, const FileSearch* searcher)
{
	if (toCancel)
	{
		startButton->setText( tr("Cancel Search") );
		progressBar1->reset();
		progressBar1->show();
		searchLabel->show();
		disconnect(startButton, SIGNAL(clicked()), this, SLOT(SearchPic()));
		connect(startButton, SIGNAL(clicked()), searcher, SLOT(cancel()));
	}
	else
	{
		startButton->setText( tr("Start Search"));
		progressBar1->reset();
		progressBar1->hide();
		searchLabel->hide();
		disconnect(startButton, SIGNAL(clicked()), searcher, SLOT(cancel()));
		connect(startButton, SIGNAL(clicked()), this, SLOT(SearchPic()));
	}
}

void PicSearchOptions::changeSearchDir()
{
	QString workDir;
#ifndef _WIN32
	workDir = QDir::homePath();
#endif
	QString searchBase;
	searchBase = QFileDialog::getExistingDirectory( this, workDir, tr("Select a base directory for search"));
	if( searchBase.isEmpty() || !QDir().exists(searchBase) )
		return;
	directoryEdit->setText(searchBase);
}

void PicSearchOptions::SearchPic()
{
	QString searchBase = directoryEdit->text();
	if( searchBase.isEmpty() || !QDir().exists(searchBase) )
	{
		if (QMessageBox::warning(this, tr("Scribus - Image Search"), tr("Base directory for search does not exist.\nPlease choose another one."),
				QMessageBox::Ok|QMessageBox::Default|QMessageBox::Escape|QMessageBox::Cancel,
				QMessageBox::NoButton) != QMessageBox::Ok)
			return;
		changeSearchDir();
		if( searchBase.isEmpty() || !QDir().exists(searchBase) )
			return;
	}
	// Set up the search, then return to the event loop until it notifies us
	// that it's done.
	// Note: search will be deleted when this PicStatus is, so there's no
	// need to worry about cleanup.
	// case sensitive note: it has no meaning on windows
	int recurse = -1;
	if (recursiveSearch->isChecked())
		recurse = -1;
	else
		recurse = 0;
	FileSearch* search = new FileSearch(this, fileEdit->text(), searchBase, recurse, caseInsensitiveCheck->isChecked());
	Q_CHECK_PTR(search);
	connect(search, SIGNAL(searchComplete(const QStringList&, const QString&)), SLOT(SearchPicFinished(const QStringList&, const QString&)));
	connect(search, SIGNAL(aborted(bool)), SLOT(SearchPicAborted(bool)));
	// Set up the UI to let the user cancel the search, then start it
	setSearchButton(true, search);
	progressBar1->setRange(0, 0);
	search->start();
}

void PicSearchOptions::SearchPicAborted(bool userCancelled)
{
	m_userCancelled = userCancelled;
	const FileSearch* search = dynamic_cast<const FileSearch*>(sender());
	assert(search);
	setSearchButton(false, search);
	if (!userCancelled)
		// A running search failed
		QMessageBox::warning(this, tr("Scribus - Image Search"), tr("The search failed: %1").arg(search->lastError()),
				QMessageBox::Ok|QMessageBox::Default|QMessageBox::Escape,
				QMessageBox::NoButton);
	reject();
}

void PicSearchOptions::SearchPicFinished(const QStringList & matches, const QString & fileName)
{
	m_matches = matches;
	m_fileName = fileEdit->text();
	accept();
}
