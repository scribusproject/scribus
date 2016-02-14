/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
*   Copyright (C) 2004 by Craig Bradney                                   *
*   cbradney@zip.com.au                                                   *
*   Copyright (C) 2005 by Petr Vanek                                      *
*   petr@yarpen.cz                                                        *
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
*   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
***************************************************************************/

#ifndef HELPBROWSER_H
#define HELPBROWSER_H

#include <QAction>
#include <QItemSelection>
#include <QList>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPair>
#include <QProgressBar>
#include <QString>
#include <QToolBar>
#include <QTreeWidgetItem>
#include <QUrl>
#include <QVariant>
#include <QWidget>

class QEvent;

#include "ui_helpbrowser.h"
#include "scribusapi.h"
#include "scconfig.h"
#include "scpaths.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "schelptreemodel.h"

class HelpNavigation;

//! \brief A structure holding title/file url reference.
struct histd2 {
	QString url;
	QString title;
};

class SCRIBUS_API HelpBrowser : public QMainWindow, Ui::HelpBrowser
{
    Q_OBJECT

public:
	HelpBrowser(QWidget* parent, const QString& caption, const QString& guiLangage="en", const QString& jumpToSection="", const QString& jumpToFile="");
	~HelpBrowser();
	
	virtual void changeEvent(QEvent *e);

	/*! \brief History menu. It's public because of history reader - separate class */
	QMenu* histMenu;
	/*! \brief Mapping the documents for history. */
	QMap<QAction*, histd2> mHistory;
	/*! \brief Set text to the browser
	\param str a QString with text (html) */
	void setHtml(const QString& str);

	static bool firstRun;

public slots:
	void jumpToHelpSection(const QString& jumpToSection, const QString& jumpToFile="", bool dontChangeIfAlreadyLoaded=false);

protected:
	void closeEvent(QCloseEvent * event);

	void setupLocalUI();
	/*! \brief Reads saved bookmarks from external file */
	void readBookmarks();
	/*! \brief Search in doc files in spec. dir.
	It uses directory-recursion. I hope that the documentation will have
	only 2-3 level dir structure so it doesn't matter.
	\author Petr Vanek <petr@yarpen.cz> */
	void searchingInDirectory(const QString&);

	/*! \brief Reads saved history of browsing. */
	void readHistory();

	/*! \brief Tell the user there is no help available */
	void displayNoHelp();

	HelpNavigation * helpNav;
	
	QMenu* fileMenu;
	QMenu* editMenu;
	QMenu* bookMenu;
	QAction* filePrint;
	QAction* fileExit;
	QAction* editFind;
	QAction* editFindNext;
	QAction* editFindPrev;
	QAction* bookAdd;
	QAction* bookDel;
	QAction* bookDelAll;
	QAction* goHome;
	QAction* goBack;
	QAction* goFwd;

	QProgressBar * progressBar;

	//! \brief Selected language is here. If there is no docs for this language, "en" is used.
	QString language;
	//! \brief QString holding location of menu.xml we are using, we load the help files from here
	QString finalBaseDir;
	/*! \brief Text to be found in document */
	QString findText;
	/** \brief Configuration structure */
	PrefsContext* prefs;
	ScHelpTreeModel* menuModel;
	QMap<QString, QString> quickHelpIndex;
	QMap<QString, QPair<QString, QString> > bookmarkIndex;
	
protected slots:
	virtual void languageChange();
	void histChosen(QAction* i);
	void loadHelp(const QString& filename);
	void loadMenu();
//	void showLinkContents(const QString &link);
	QString bookmarkFile();
	QString historyFile();

	/*! \brief Load doc file when user select filename in content view. */
	void itemSelected(const QItemSelection & selected, const QItemSelection & deselected);

	/*! \brief Load doc file when user select filename in search view. */
	void itemSearchSelected(QTreeWidgetItem *, int);

	/*! \brief Load doc file when user select filename in bookmark view. */
	void itemBookmarkSelected(QTreeWidgetItem *, int);

	/*! \brief Performs searching in documentation.
	It walks through installed documentation and searching in all text files
	\author Petr Vanek <petr@yarpen.cz> */
	void searchingButton_clicked();

	/*! \brief Find text in one document.
	Classical ctrl+f searching.
	\author Petr Vanek <petr@yarpen.cz> */
	void find();

	/*! \brief Find next occurrences of the text in one document.
	\author Petr Vanek <petr@yarpen.cz> */
	void findNext();

	/*! \brief Find previous occurrences of the text in one document.
	\author Petr Vanek <petr@yarpen.cz> */
	void findPrevious();

	/*! \brief Print the documentation.
	Based on the Qt example.
	*/
	void print();

	/*! \brief Add document into bookmarks. */
	void bookmarkButton_clicked();

	/*! \brief Delete selected document from bookmarks. */
	void deleteBookmarkButton_clicked();

	/*! \brief Delete all bookmarks */
	void deleteAllBookmarkButton_clicked();

signals:
	void closed();

};

#endif // HELPBROWSER_H

