/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef ALIGNSELECT_H
#define ALIGNSELECT_H

#include <q3buttongroup.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
//Added by qt3to4:
#include <Q3GridLayout>

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
	Q3GridLayout* GroupAlignLayout;

};

#endif

