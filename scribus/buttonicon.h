#ifndef BUTTONICON_H
#define BUTTONICON_H

#include <qdialog.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qslider.h>
#include <qlayout.h>
#include <qtooltip.h>
#include "pageitem.h"

class ButtonIcon : public QDialog
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
    QGroupBox* GroupButton;
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
    QVBoxLayout* ButtonIconLayout;
    QGridLayout* Layout1;
    QHBoxLayout* Layout5;
    QGridLayout* Layout3;
    QHBoxLayout* Layout4;
};

#endif // BUTTONICON_H
