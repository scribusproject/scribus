#ifndef STYLESELECT_H
#define STYLESELECT_H

#include <qgroupbox.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qtooltip.h>

class StyleSelect : public QWidget
{

Q_OBJECT

public:
	StyleSelect(QWidget* parent);
	~StyleSelect() {};
	void setStyle(int s);
	int getStyle();

    QGroupBox* ButtonGroup1;
    QToolButton* Underline;
    QToolButton* Subs;
    QToolButton* Supers;
    QToolButton* Kapital;
    QToolButton* Strike;
    QToolButton* Outlined;
		
private slots:
	void setTypeStyle();

signals:
	void State(int);

protected:
    QHBoxLayout* Layout1;
    QHBoxLayout* ButtonGroup1Layout;

};

#endif

