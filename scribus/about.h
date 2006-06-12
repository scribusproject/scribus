/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef ABOUT_H
#define ABOUT_H

#include <qdialog.h>

#include "scribusapi.h"

class TextBrowser; // taken from helpbrowser.h
class QLabel;
class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class QTabWidget;
class QPushButton;


class SCRIBUS_API About : public QDialog
{ 
    	Q_OBJECT

public:
	About( QWidget* parent );
	~About() {};
	
	QLabel* pixmapLabel1;
	QLabel* buildID;
	QPushButton* okButton;
	QTabWidget* tabWidget2;
	QWidget* tab;
	QWidget* tab_2;
	QWidget* tab_3;
	QWidget* tab_4;
	TextBrowser* textView1;
	TextBrowser* textView2;
	TextBrowser* textView4;

protected:
	QVBoxLayout* aboutLayout;
	QHBoxLayout* tabLayout;
	QVBoxLayout* tabLayout1;
	QHBoxLayout* tabLayout_2;
	QHBoxLayout* tabLayout_4;
	QHBoxLayout* layout2;

};

#endif // ABOUT_H
