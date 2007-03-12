/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef ABOUT_H
#define ABOUT_H

#include <qdialog.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3HBoxLayout>
#include <QLabel>

#include "scribusapi.h"

class TextBrowser; // taken from helpbrowser.h
class QLabel;
class QWidget;
class Q3HBoxLayout;
class Q3VBoxLayout;
class QTabWidget;
class QPushButton;


class SCRIBUS_API About : public QDialog
{ 
    	Q_OBJECT

public:
	About( QWidget* parent );
	~About() {};
	
protected:
	QLabel* pixmapLabel1;
	QLabel* buildID;
	QPushButton* okButton;
	QPushButton* checkForUpdateButton;
	QTabWidget* tabWidget2;
	QWidget* tab;
	QWidget* tab_2;
	QWidget* tab_3;
	QWidget* tab_4;
	QWidget* tab_5;
	TextBrowser* textView1;
	TextBrowser* textView2;
	TextBrowser* textView4;
	TextBrowser* textView5;
	Q3VBoxLayout* aboutLayout;
	Q3VBoxLayout* updateLayout;
	Q3HBoxLayout* tabLayout;
	Q3VBoxLayout* tabLayout1;
	Q3HBoxLayout* tabLayout_2;
	Q3HBoxLayout* tabLayout_4;
	Q3HBoxLayout* layout2;

protected slots:
	void runUpdateCheck();
};

#endif // ABOUT_H
