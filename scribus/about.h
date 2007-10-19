/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "scribusapi.h"

class ScTextBrowser;
class QLabel;
class QWidget;
class QTabWidget;
class QPushButton;
class QShowEvent;


class SCRIBUS_API About : public QDialog
{ 
    	Q_OBJECT

public:

	typedef enum
	{
		Default = 0,
		CheckUpdates = 1
	} AboutMode;

	About( QWidget* parent, AboutMode mode = About::Default );
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
	ScTextBrowser* textView1;
	ScTextBrowser* textView2;
	ScTextBrowser* textView4;
	ScTextBrowser* textView5;
	QVBoxLayout* aboutLayout;
	QVBoxLayout* updateLayout;
	QHBoxLayout* tabLayout;
	QVBoxLayout* tabLayout1;
	QHBoxLayout* tabLayout_2;
	QHBoxLayout* tabLayout_4;
	QHBoxLayout* layout2;

	bool         m_firstShow;
	AboutMode    m_mode;
	virtual void showEvent ( QShowEvent * event ); 

protected slots:
	void runUpdateCheck();

public slots:
	virtual void setVisible (bool visible); 
};

#endif // ABOUT_H
