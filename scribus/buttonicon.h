/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef BUTTONICON_H
#define BUTTONICON_H

#include <qdialog.h>
#include <qcombobox.h>
#include <q3groupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qslider.h>
#include <qlayout.h>
#include <qtooltip.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3GridLayout>
#include <Q3HBoxLayout>

#include "scribusapi.h"

class PageItem;

class SCRIBUS_API ButtonIcon : public QDialog
{ 
    Q_OBJECT

public:
    ButtonIcon(QWidget* parent, PageItem* ite);
    ~ButtonIcon() {};

    QLabel* TextLabel1_2;
    QComboBox* Place;
    QLabel* TextLabel1;
    QComboBox* ScaleW;
    QLabel* TextLabel2;
    QComboBox* ScaleH;
    Q3GroupBox* GroupButton;
    QLabel* TextLabel3;
    QSlider* SliderX;
    QSlider* SliderY;
    QPushButton* OK;
    QPushButton* Cancel;
    QPushButton* Reset;
	PageItem* Item;
	int IcPlace;
	int IcScaleW;
	int IcScaleH;
	double IcPlaceX;
	double IcPlaceY;

private slots:
	void MoveIconY(int y);
	void MoveIconX(int x);
	void SetAllVals();
	void SetScaleHow(int s);

protected:
    Q3VBoxLayout* ButtonIconLayout;
    Q3GridLayout* Layout1;
    Q3HBoxLayout* Layout5;
    Q3GridLayout* Layout3;
    Q3HBoxLayout* Layout4;
};

#endif // BUTTONICON_H
