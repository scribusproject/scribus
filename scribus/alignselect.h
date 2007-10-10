/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef ALIGNSELECT_H
#define ALIGNSELECT_H

#include <QHBoxLayout>
#include <QToolButton>
#include <QButtonGroup>

class QEvent;

#include "scribusapi.h"

class SCRIBUS_API AlignSelect : public QWidget
{

Q_OBJECT

public:
	AlignSelect(QWidget* parent);
	~AlignSelect() {};
	void setStyle(int s);
	int getStyle();
	int selectedId();
	
	virtual void changeEvent(QEvent *e);

	QButtonGroup* buttonGroup;
	int selected;
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
	QHBoxLayout* GroupAlignLayout;

};

#endif

