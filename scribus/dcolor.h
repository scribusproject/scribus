/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef DELCOLOR_H
#define DELCOLOR_H

#include <qdialog.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include "scribusdoc.h"

class DelColor : public QDialog
{ 
    Q_OBJECT

public:
    DelColor(QWidget* parent, CListe farben, QString Fnam, bool HDoc );
    ~DelColor() {};
    QString EFarbe;

private:
    QLabel* TextLabel1;
    QPushButton* PushButton2;
    QLabel* TextLabel3;
    QPushButton* PushButton3;
    QLabel* TextLabel4;
    QLabel* DColor;
    QComboBox* Ersatz;

protected:
    QVBoxLayout* DLayout;
    QGridLayout* Layout4;
    QHBoxLayout* Layout3;

private slots:
    virtual void ReplaceColor(int);
};

#endif // DELCOLOR_H
