/****************************************************************************
** Form interface generated from reading ui file 'HelpBrowser.ui'
**
** Created: Sat Nov 6 22:08:59 2004
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

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
	HelpBrowser( QWidget* parent, QString caption, QString guiLangage="en", QString jumpTo="", QString jumpToFile="");
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
	void jumpToHelpSection(QString guiLanguage, QString jumpTo, QString jumpToFile="");
	void loadHelp(QString filename);
	void loadMenu();
};

#endif // HELPBROWSER_H
