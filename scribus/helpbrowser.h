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

class HelpBrowser : public QWidget
{
	Q_OBJECT

public:
	HelpBrowser( QWidget* parent, QString caption, QString guiLangage="en", QString jumpToSection="", QString jumpToFile="");
	~HelpBrowser();

	QToolButton* homeButton;
	QToolButton* backButton;
	QToolButton* forwButton;

	QTabWidget* tabWidget;
	QWidget* tabContents;
	QListView* listView;
	QTextBrowser* textBrowser;

protected:
	QVBoxLayout* helpBrowsermainLayout;
	QHBoxLayout* helpBrowserLayout;
	QHBoxLayout* tabLayout;
	QHBoxLayout* buttonLayout;

	QPopupMenu* histMenu;
	struct histd {
					QString Url;
					QString Title;
				};
	QMap<int, histd> mHistory;

protected slots:
	virtual void languageChange();
	void itemSelected( QListViewItem *);
	void sourceChanged(const QString& url);
	void histChosen(int i);
	void jumpToHelpSection(QString guiLanguage, QString jumpToSection, QString jumpToFile="");
	void loadHelp(QString filename);
	void loadMenu();
};

#endif // HELPBROWSER_H
