#ifndef ALIGNSELECT_H
#define ALIGNSELECT_H

#include <q3buttongroup.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
//Added by qt3to4:
#include <QGridLayout>

#include "scribusapi.h"

class SCRIBUS_API AlignSelect : public Q3ButtonGroup
{

Q_OBJECT

public:
	AlignSelect(QWidget* parent);
	~AlignSelect() {};
	void setStyle(int s);
	int getStyle();

	Q3ButtonGroup* GroupAlign;
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

