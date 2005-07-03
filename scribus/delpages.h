/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef DELPAGES_H
#define DELPAGES_H

#include <qdialog.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qspinbox.h>

class DelPages : public QDialog
{ 
    Q_OBJECT

public:
    DelPages( QWidget* parent, int act, int max );
    ~DelPages() {};

    QPushButton* Cancel;
    QPushButton* OK;
    QLabel* TextLabel2;
    QSpinBox* ToPage;
    QLabel* TextLabel1;
    QSpinBox* FromPage;

protected:
    QVBoxLayout* DLayout;
    QHBoxLayout* Layout4;
    QHBoxLayout* Layout3;

public slots:
    virtual void FromChanged(int nr);
    virtual void ToChanged(int nr);
};

#endif // DELPAGES_H
