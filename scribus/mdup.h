/****************************************************************************
** Form interface generated from reading ui file 'Mdup.ui'
**
** Created: Mon Apr 30 09:06:58 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
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
    Mdup( QWidget* parent, float Dx, float Dy, int Ein );
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
