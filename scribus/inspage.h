/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef INSPAGE_H
#define INSPAGE_H

#include <qdialog.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include "scribusview.h"

class InsPage : public QDialog
{ 
    Q_OBJECT

public:
    InsPage( QWidget* parent, ScribusView* view, int act, int max, bool ds );
    ~InsPage() {};

    QLabel* TextLabel1;
    QPushButton* PushButton4_2;
    QPushButton* PushButton4;
    QComboBox* Where;
    QComboBox* Based;
    QComboBox* Based2;
    QLabel* TextLabel3;
    QLabel* TextLabel3a;
    QLabel* TextLabel2;
    QSpinBox* ActualPage;
    QSpinBox* NumPages;

protected:
    QVBoxLayout* DLayout;
    QGridLayout* Layout4;
    QHBoxLayout* Layout4a;
    QHBoxLayout* Layout4b;
    QHBoxLayout* Layout3;

};

#endif // INSPAGE_H
