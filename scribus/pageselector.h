/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PAGESELECTOR_H
#define PAGESELECTOR_H


#include "styleoptions.h"

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>
#include <qvalidator.h>
#include "scribusapi.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QComboBox;
class QLineEdit;
class QPopupMenu;
class QPushButton;
class QToolButton;

class SCRIBUS_API PageSelector : public QWidget
{
	Q_OBJECT

public:
	PageSelector( QWidget* parent, int maxPg );
	~PageSelector() {};
	bool hasFocus();
	void focusPolicy(QWidget::FocusPolicy policy);

#if OPTION_USE_QTOOLBUTTON
	QToolButton* Start;
	QToolButton* Back;
	QToolButton* Forward;
	QToolButton* Last;
#else
	QPushButton* Start;
	QPushButton* Back;
	QPushButton* Forward;
	QPushButton* Last;
#endif
	
	QComboBox* PageCombo;
	QValidator *v;
	int LastPG;
	int APage;

public slots:
	virtual void GotoPg(int);
	virtual void setMaxValue(int);
	void languageChange();
	void clearFocus();

private slots:
	virtual void GotoPgE(int);
	virtual void GotoPage();
	virtual void ToStart();
	virtual void ToEnd();
	virtual void goBk();
	virtual void goFw();

protected:
	QHBoxLayout* PageSelectorLayout;

signals:
	void GotoPage(int);

};

#endif // PAGESELECTOR_H
