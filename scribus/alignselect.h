#ifndef ALIGNSELECT_H
#define ALIGNSELECT_H

#include <qbuttongroup.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qtooltip.h>

class AlignSelect : public QButtonGroup
{

Q_OBJECT

public:
	AlignSelect(QWidget* parent);
	~AlignSelect() {};
	void setStyle(int s);
	int getStyle();

	QButtonGroup* GroupAlign;
	QToolButton* TextL;
	QToolButton* TextR;
	QToolButton* TextC;
	QToolButton* TextB;
	QToolButton* TextF;

public slots:
	void languageChange();
		
private slots:
	void setTypeStyle(int a);

signals:
	void State(int);

protected:
	QGridLayout* GroupAlignLayout;

};

#endif

