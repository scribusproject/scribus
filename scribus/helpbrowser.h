/****************************************************************************
** Form interface generated from reading ui file 'Help.ui'
**
** Created: Tue Jul 3 21:51:30 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef HELPBROWSER_H
#define HELPBROWSER_H

#include <qdialog.h>
#include <qpushbutton.h>
#include <qtextbrowser.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qpopupmenu.h>
#include <qmap.h>

class HelpBrowser : public QWidget
{ 
    Q_OBJECT

public:
    HelpBrowser( QWidget* parent, QString Capt, QString Datei = "" );
    ~HelpBrowser() {};

    QToolButton* HomeB;
    QToolButton* BackB;
    QToolButton* ForwB;
    QTextBrowser* Anzeige;
		QPopupMenu* hist;
		struct histd { QString Url;
									 QString Title;
								 };
    QMap<int, histd> mHistory;

public slots:
		void sourceChanged(const QString& url);
		void histChosen(int i);

protected:
    QVBoxLayout* HelpBrowserLayout;
    QHBoxLayout* Layout13;
};

#endif // HELPBROWSER_H
