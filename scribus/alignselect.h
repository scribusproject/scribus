/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef ALIGNSELECT_H
#define ALIGNSELECT_H

#include <qbuttongroup.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qtooltip.h>

#include "scribusapi.h"

class SCRIBUS_API AlignSelect : public QButtonGroup
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

