/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef ONECLICK_H
#define ONECLICK_H

#include <qdialog.h>
class QLayout;
class QLabel;
class QPushButton;
class QButtonGroup;
class QGroupBox;
class QRadioButton;
class QFrame;
class QString;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QCheckBox;
class MSpinBox;
class LinkButton;

#include "scribusapi.h"

class SCRIBUS_API OneClick : public QDialog
{
	Q_OBJECT

public:
	OneClick( QWidget* parent, QString titel, int unitIndex, double defW, double defH, bool remember, int origin );
	~OneClick() {};
	MSpinBox *spinWidth;
	MSpinBox *spinHeight;
	LinkButton* linkSize;
	QCheckBox* checkRemember;
	QButtonGroup* RotationGroup;

public slots:
	void ToggleKette();
	void changeSizesH();
	void changeSizesW();

private:
	QVBoxLayout* queryLayout;
	QGridLayout* editLayout;
	QGridLayout* Layout12a;
	QGridLayout* Layout12;
	QGridLayout* SizeGroupLayout;
	QHBoxLayout* okCancelLayout;
	QPushButton* okButton;
	QPushButton* cancelButton;
	QLabel* questionLabel;
	QLabel* questionLabel2;
	QRadioButton* TopLeft;
	QRadioButton* TopRight;
	QRadioButton* Center;
	QRadioButton* BottomLeft;
	QRadioButton* BottomRight;
	QFrame* Line1;
	QFrame* Line2;
	QFrame* Line4;
	QFrame* Line5;
	QGroupBox* SizeGroup;
};

#endif
