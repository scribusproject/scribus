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
	
	QLabel* pixmapLabel1;
	QLabel* buildID;
	QPushButton* okButton;
	QTabWidget* tabWidget2;
	QWidget* tab;
	QWidget* tab_2;
	QWidget* tab_3;
	QWidget* tab_4;
	QTextView* textView1;
	QTextView* textView2;
	QTextView* textView4;

protected:
	QVBoxLayout* aboutLayout;
	QHBoxLayout* tabLayout;
	QVBoxLayout* tabLayout1;
	QHBoxLayout* tabLayout_2;
	QHBoxLayout* tabLayout_4;
	QHBoxLayout* layout2;

};

#endif // ABOUT_H
