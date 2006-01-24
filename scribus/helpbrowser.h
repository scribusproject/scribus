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
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#ifndef HELPBROWSER_H
#define HELPBROWSER_H

#include <qvariant.h>
#include <qdialog.h>
#include <qstring.h>

#include "scribusapi.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QTabWidget;
class QWidget;
class QListView;
class QListViewItem;
class QTextBrowser;
class QToolButton;
class QPopupMenu;
class QSplitter;
class QLabel;
class QLineEdit;
class QPushButton;
class QMenuBar;


//! A structure holding title/file url reference.
struct histd {
	QString url;
	QString title;
};


/*! \brief This is the Help dialog for Scribus. */
class SCRIBUS_API HelpBrowser : public QWidget
{
	Q_OBJECT

public:
	HelpBrowser( QWidget* parent, QString caption, QString guiLangage="en", QString jumpToSection="", QString jumpToFile="");
	~HelpBrowser();

	/*! \brief History menu. It's public because of history reader - separate class */
	QPopupMenu* histMenu;
	/*! \brief Mapping the documents for history. */
	QMap<int, histd> mHistory;
	/*! \brief Set text to the browser
	\param str a QString with text (html) */
	void setText(QString str);

protected:
	//! layouts
	QVBoxLayout* helpBrowsermainLayout;
	QHBoxLayout* helpBrowserLayout;
	QHBoxLayout* tabLayout;
	QHBoxLayout* buttonLayout;
	QVBoxLayout* searchingMainLayout;
	QHBoxLayout* searchingButtonLayout;
	//! \brief History tool buttons
	QToolButton* homeButton;
	//! \brief Holds histMenu
	QToolButton* backButton;
	QToolButton* forwButton;
	//! bookmarks
	QHBoxLayout* bookmarksButtonLayout;
	QVBoxLayout* bookmarksMainLayout;

	//! \brief Widget holding the listviews
	QTabWidget* tabWidget;
	//! \brief Main table of contents - parent widget
	QWidget* tabContents;
	//! \brief Main table of contents
	QListView* listView;
	//! \brief There is documentation shown
	QTextBrowser* textBrowser;
	//! \brief Rubber bar between ToC and browser
	QSplitter* splitter;
	//! \brief Selected language is here. If there is no docs for this language, "en" is used.
	QString language;
	//! \brief searching
	QWidget* tabSearching;
	QLineEdit* searchingEdit;
	QPushButton* searchingButton;
	QListView* searchingView;
	//! \brief bookmarks
	QWidget* tabBookmarks;
	QPushButton* bookmarkButton;
	QPushButton* deleteBookmarkButton;
	QPushButton* deleteAllBookmarkButton;
	QListView* bookmarksView;
	//! \brief menu
	QMenuBar *menuBar;

	/*! \brief Text to be finded in document */
	QString findText;

	/*! \brief Search in doc files in spec. dir.
	It uses directory-recursion. I hope that the documentation will have
	only 2-3 level dir structure so it doesn't matter.
	\author Petr Vanek <petr@yarpen.cz> */
	void searchingInDirectory(QString);

	/*! \brief Reads saved bookmarks from external file */
	void readBookmarks();

	/*! \brief Reads saved history of browsing. */
	void readHistory();

protected slots:
	virtual void languageChange();

	/*! \brief Load doc file when user select filename in content view. */
	void itemSelected( QListViewItem *);

	/*! \brief Load doc file when user select filename in search view.
	Then it performs some doc-finding and highlighting.
	\author Petr Vanek <petr@yarpen.cz> */
	void itemSearchSelected( QListViewItem *);
	void histChosen(int i);
	void jumpToHelpSection(QString jumpToSection, QString jumpToFile="");
	void loadHelp(QString filename);
	void loadMenu();

	/*! \brief Performs searching in documentation.
	It walks through installed documentation and searching in all text files
	\author Petr Vanek <petr@yarpen.cz> */
	void searchingButton_clicked();

	/*! \brief Find text in one document.
	Classical ctrl+f searching.
	\author Petr Vanek <petr@yarpen.cz> */
	void find();

	/*! \brief Find next occurences of the text in one document.
	\author Petr Vanek <petr@yarpen.cz> */
	void findNext();

	/*! \brief Find previous occurences of the text in one document.
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

};

#endif // HELPBROWSER_H

