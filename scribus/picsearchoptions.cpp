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

#include <qmessagebox.h>
#include <qstringlist.h>
#include <qdir.h>
#include <qlineedit.h>
#include <qtooltip.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include <qcheckbox.h>
#include "picsearchoptions.h"
#include "picsearchoptions.moc"
#include "filesearch.h"

PicSearchOptions::PicSearchOptions(QWidget* parent, const QString & fileName, const QString & searchBase) : PicSearchOptionsBase( parent, "PicSearchOptions", true, 0 )
{
	m_fileName = fileName;
	directoryEdit->setText(searchBase);
	connect(startButton, SIGNAL(clicked()), this, SLOT(SearchPic()));
	QToolTip::add(caseInsensitiveCheck, "<qt>" + tr("The filesystem will be searched for case insensitive file names when you check this on. Remember it is not default on most operating systems except MS Windows") + "</qt>");
}

void PicSearchOptions::setSearchButton(bool toCancel, const FileSearch* searcher)
{
	if (toCancel)
	{
		startButton->setText( tr("Cancel Search") );
		disconnect(startButton, SIGNAL(clicked()), this, SLOT(SearchPic()));
		connect(startButton, SIGNAL(clicked()), searcher, SLOT(cancel()));
	}
	else
	{
		startButton->setText( tr("Start Search"));
		disconnect(startButton, SIGNAL(clicked()), searcher, SLOT(cancel()));
		connect(startButton, SIGNAL(clicked()), SLOT(SearchPic()));
	}
}

void PicSearchOptions::SearchPic()
{
	QString workDir;
#ifndef _WIN32
	workDir = QDir::homeDirPath();
#endif
	// Pictures may be located completely outside home or documents directory
	// so ask base search directory first
	QString searchBase = directoryEdit->text();
	if( searchBase.isEmpty() || !QDir().exists(searchBase) )
		return;
	// Set up the search, then return to the event loop until it notifies us
	// that it's done.
	// Note: search will be deleted when this PicStatus is, so there's no
	// need to worry about cleanup.
	// case sensitive note: it has no meaning on windows

	FileSearch* search = new FileSearch(this, m_fileName, searchBase, -1, caseInsensitiveCheck->isChecked());
	Q_CHECK_PTR(search);
	connect(search, SIGNAL(searchComplete(const QStringList&, const QString&)), SLOT(SearchPicFinished(const QStringList&, const QString&)));
	connect(search, SIGNAL(aborted(bool)), SLOT(SearchPicAborted(bool)));
	// Set up the UI to let the user cancel the search, then start it
	setSearchButton(true, search);
	search->start();
}

void PicSearchOptions::SearchPicAborted(bool userCancelled)
{
	m_userCancelled = userCancelled;
	const FileSearch* search = dynamic_cast<const FileSearch*>(sender());
	Q_ASSERT(search);
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
	m_fileName = fileName;
	accept();
}
