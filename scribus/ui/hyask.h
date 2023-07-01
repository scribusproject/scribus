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

protected:
	void keyPressEvent(QKeyEvent* k) override;
};

class SCRIBUS_API HyAsk : public QDialog
{
	Q_OBJECT

public:
	HyAsk (QWidget* parent, const QString& HWort );
	~HyAsk() {};

	WortEdit* Wort { nullptr };
	QCheckBox *addToExceptionList { nullptr };
	QCheckBox *addToIgnoreList { nullptr };
	QLabel *addToExceptionListText { nullptr };
	QLabel *addToIgnoreListText { nullptr };
	QPushButton* OK { nullptr };
	QPushButton* Skip { nullptr };
	QPushButton* Cancel { nullptr };
	int xpos { 0 };
	int ypos { 0 };

public slots:
	void Check();
	void DoSkip();
	void accept();
	void reject();

protected:
	QVBoxLayout* HyAskLayout { nullptr };
	QHBoxLayout* Layout1 { nullptr };
	QVBoxLayout* vboxLayout1 { nullptr };
	QVBoxLayout* vboxLayout2 { nullptr };
	QVBoxLayout* vboxLayout3 { nullptr };
	QHBoxLayout* hboxLayout1 { nullptr };
	QHBoxLayout* hboxLayout2 { nullptr };
};

#endif // HYASK_H
