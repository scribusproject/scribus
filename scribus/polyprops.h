/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef POLYGONPROPS_H
#define POLYGONPROPS_H

#include <qdialog.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qslider.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qpixmap.h>

class PolygonProps : public QDialog
{ 
    Q_OBJECT

public:
    PolygonProps(QWidget* parent = 0);
    ~PolygonProps() {};
    double GetZeroFaktor();
    double GetMaxFaktor();

    QLabel* Text1;
    QSpinBox* Ecken;
    QCheckBox* Konvex;
    QLabel* Text2;
    QSpinBox* Faktor;
    QSlider* Slider1;
    QLabel* Preview;
    QLabel* Text2_2;
    QSpinBox* Faktor2;
    QSlider* Slider2;
    QPushButton* PushButton1;
    QPushButton* PushButton2;
	QPixmap* Pre;
	double PFactor;

public slots:
	void UpdatePreView();
	double GetFaktor();
	void ValFromSpin(int a);
	void ValFromSpin2(int a);

protected:
    QVBoxLayout* PolygonPropsLayout;
    QHBoxLayout* Layout11;
    QVBoxLayout* Layout10;
    QHBoxLayout* Layout2;
    QHBoxLayout* Layout9;
    QVBoxLayout* Layout8;
    QHBoxLayout* Layout7;
    QHBoxLayout* Layout1_2;
    QHBoxLayout* Layout9_2;
    QVBoxLayout* Layout8_2;
    QHBoxLayout* Layout7_2;
};

#endif // POLYGONPROPS_H
