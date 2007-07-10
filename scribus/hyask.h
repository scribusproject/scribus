/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef HYASK_H
#define HYASK_H

#include <QDialog>
#include <QLineEdit>

#include "scribusapi.h"
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QKeyEvent;
class QCheckBox;
class QLabel;

class SCRIBUS_API WortEdit : public QLineEdit
{
		Q_OBJECT
public:
	WortEdit ( QWidget* parent );
	~WortEdit() {};
	void keyPressEvent ( QKeyEvent *k );
};

class SCRIBUS_API HyAsk : public QDialog
{
	Q_OBJECT

public:
	HyAsk ( QWidget* parent, QString HWort );
	~HyAsk() {};

	WortEdit* Wort;
	QCheckBox *addToExceptionList;
	QCheckBox *addToIgnoreList;
	QLabel *addToExceptionListText;
	QLabel *addToIgnoreListText;
	QPushButton* OK;
	QPushButton* Skip;
	QPushButton* Cancel;
	int xpos;
	int ypos;

public slots:
	void Check();
	void DoSkip();
	void accept();
	void reject();

protected:
	QVBoxLayout* HyAskLayout;
	QHBoxLayout* Layout1;
	QVBoxLayout* vboxLayout1;
	QVBoxLayout* vboxLayout2;
	QVBoxLayout* vboxLayout3;
	QHBoxLayout* hboxLayout1;
	QHBoxLayout* hboxLayout2;
};

#endif // HYASK_H
