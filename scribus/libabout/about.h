#ifndef ABOUT_H
#define ABOUT_H

#include <qdialog.h>
#include <qlabel.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qtextview.h>
#include <qpushbutton.h>
#include <qlayout.h>

class About : public QDialog
{ 
    	Q_OBJECT

public:
	About( QWidget* parent );
	~About() {};
	
	QLabel* PixmapLabel1;
	QLabel* BuildID;
	QPushButton* PushButton1;
	QTabWidget* TabWidget2;
	QWidget* tab;
	QWidget* tab_2;
	QWidget* tab_3;
	QTextView* TextView1;
	QTextView* TextView2;

protected:
	QVBoxLayout* AboutLayout;
	QHBoxLayout* tabLayout;
	QVBoxLayout* tabLayout1;
	QHBoxLayout* tabLayout_2;
	QHBoxLayout* Layout2;

};

#endif // ABOUT_H
