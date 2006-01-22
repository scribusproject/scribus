/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef MDUP_H
#define MDUP_H

#include <qdialog.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qpushbutton.h>
#include "mspinbox.h"

class Mdup : public QDialog
{
    Q_OBJECT

public:
    Mdup( QWidget* parent, double Dx, double Dy, int Ein );
    ~Mdup() {};

    QLabel* TextLabel1_2;
    QLabel* TextLabel1_2_2;
    QSpinBox* Ncopies;
    MSpinBox* ShiftV;
    QPushButton* PushButton12;
    QLabel* TextLabel1;
    MSpinBox* ShiftH;
    QPushButton* PushButton13;

protected:
    QVBoxLayout* MdupLayout;
    QGridLayout* Layout4;
    QHBoxLayout* Layout3;

};

#endif // MDUP_H
