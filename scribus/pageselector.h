/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PAGESELECTOR_H
#define PAGESELECTOR_H

#include <QWidget>

class QEvent;
class QHBoxLayout;
class QLabel;
class QLineEdit;
class QPushButton;
class QToolButton;
class QIntValidator;

#include "scribusapi.h"
#include "styleoptions.h"

class ScComboBox;

class SCRIBUS_API PageSelector : public QWidget
{
	Q_OBJECT

public:
	PageSelector( QWidget* parent, int maxPg );
	~PageSelector() {};
	
	virtual void changeEvent(QEvent *e);
	
	bool hasFocus();
	void focusPolicy(Qt::FocusPolicy policy);
	void setFont ( const QFont & );

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
	
	ScComboBox* PageCombo;
	QIntValidator *m_validator;
	int LastPG;
	int APage;

public slots:
	virtual void GotoPg(int);
	virtual void setMaximum(int);
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
	QLabel *PageCount;
	QString PageCountString;

signals:
	void GotoPage(int);

};

#endif // PAGESELECTOR_H
