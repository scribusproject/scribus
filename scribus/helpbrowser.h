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
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QTextBrowser;
class QToolButton;

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

protected:
    QVBoxLayout* HelpBrowserLayout;
    QHBoxLayout* Layout13;
};

#endif // HELPBROWSER_H
