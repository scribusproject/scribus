/***************************************************************************
*   Copyright (C) 2004 by Craig Bradney                                   *
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

#ifndef HELPBROWSER_H
#define HELPBROWSER_H

#include <qvariant.h>
#include <qdialog.h>
#include <qstring.h>

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

class HelpBrowser : public QWidget
{
	Q_OBJECT

public:
	HelpBrowser( QWidget* parent, QString caption, QString guiLangage="en", QString jumpToSection="", QString jumpToFile="");
	~HelpBrowser();

protected:
	QVBoxLayout* helpBrowsermainLayout;
	QHBoxLayout* helpBrowserLayout;
	QHBoxLayout* tabLayout;
	QHBoxLayout* buttonLayout;
	QVBoxLayout* searchingMainLayout;
	QHBoxLayout* searchingButtonLayout;
	QToolButton* homeButton;
	QToolButton* backButton;
	QToolButton* forwButton;

	QTabWidget* tabWidget;
	QWidget* tabContents;
	QWidget* tabSearching;
	QListView* listView;
	QTextBrowser* textBrowser;
	QSplitter* splitter;
	QString language;
	//! searching
	QLineEdit* searchingEdit;
	QPushButton* searchingButton;
	QListView* searchingView;
	//! menu
	QMenuBar *menuBar;

	QPopupMenu* histMenu;
	struct histd {
					QString Url;
					QString Title;
				};
	QMap<int, histd> mHistory;

	/*! Text to be finded in document */
	QString findText;
	/*! \brief Search in doc files in spec. dir.
	It uses directory-recursion. I hope that the documentation will have
	only 2-3 level dir structure so it doesn't matter.
	\author Petr Vanek <petr@yarpen.cz> */
	void searchingInDirectory(QString);

protected slots:
	virtual void languageChange();

	/*! Load doc file when user select filename in content view. */
	void itemSelected( QListViewItem *);

	/*! Load doc file when user select filename in search view.
	Then it performs some doc-finding and highlighting.
	\author Petr Vanek <petr@yarpen.cz> */
	void itemSearchSelected( QListViewItem *);
	void sourceChanged(const QString& url);
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

};

#endif // HELPBROWSER_H

