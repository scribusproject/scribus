#ifndef ABOUT_H
#define ABOUT_H

#include <qdialog.h>
#include <qlabel.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <q3textview.h>
#include <qpushbutton.h>
#include <qlayout.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "scribusapi.h"

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
	Q3TextView* textView1;
	Q3TextView* textView2;
	Q3TextView* textView4;

protected:
	QVBoxLayout* aboutLayout;
	QHBoxLayout* tabLayout;
	QVBoxLayout* tabLayout1;
	QHBoxLayout* tabLayout_2;
	QHBoxLayout* tabLayout_4;
	QHBoxLayout* layout2;

};

#endif // ABOUT_H
