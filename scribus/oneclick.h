/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef ONECLICK_H
#define ONECLICK_H

#include <QDialog>
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QCheckBox;
class QPushButton;
class QGroupBox;
class ScrSpinBox;
class LinkButton;

#include "scribusapi.h"
#include "basepointwidget.h"

class SCRIBUS_API OneClick : public QDialog
{
	Q_OBJECT

public:
	OneClick( QWidget* parent, QString titel, int unitIndex, double defW, double defH, bool remember, int origin, int mode );
	~OneClick() {};
	ScrSpinBox *spinWidth;
	ScrSpinBox *spinHeight;
	LinkButton* linkSize;
	QCheckBox* checkRemember;
	QGroupBox* RotationGroupContainer;
	BasePointWidget* RotationGroup;

public slots:
	void ToggleKette();
	void changeSizesH();
	void changeSizesW();

private:
	QVBoxLayout* queryLayout;
	QGridLayout* editLayout;
	QGridLayout* Layout12a;
	QGridLayout* SizeGroupLayout;
	QHBoxLayout* okCancelLayout;
	QPushButton* okButton;
	QPushButton* cancelButton;
	QLabel* questionLabel;
	QLabel* questionLabel2;
	QGroupBox* SizeGroup;
};

#endif
