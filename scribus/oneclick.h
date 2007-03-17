/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef ONECLICK_H
#define ONECLICK_H

#include <qdialog.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3Frame>
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <QLabel>
class QLayout;
class QLabel;
class QPushButton;
class Q3ButtonGroup;
class Q3GroupBox;
class QRadioButton;
class Q3Frame;
class QString;
class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QCheckBox;
class ScrSpinBox;
class LinkButton;

#include "scribusapi.h"

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
	Q3ButtonGroup* RotationGroup;

public slots:
	void ToggleKette();
	void changeSizesH();
	void changeSizesW();

private:
	Q3VBoxLayout* queryLayout;
	Q3GridLayout* editLayout;
	Q3GridLayout* Layout12a;
	Q3GridLayout* Layout12;
	Q3GridLayout* SizeGroupLayout;
	Q3HBoxLayout* okCancelLayout;
	QPushButton* okButton;
	QPushButton* cancelButton;
	QLabel* questionLabel;
	QLabel* questionLabel2;
	QRadioButton* TopLeft;
	QRadioButton* TopRight;
	QRadioButton* Center;
	QRadioButton* BottomLeft;
	QRadioButton* BottomRight;
	Q3Frame* Line1;
	Q3Frame* Line2;
	Q3Frame* Line4;
	Q3Frame* Line5;
	Q3GroupBox* SizeGroup;
};

#endif
