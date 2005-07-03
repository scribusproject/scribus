/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef HYSETTINGS_H
#define HYSETTINGS_H

#include <qdialog.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>

class HySettings : public QDialog
{ 
    Q_OBJECT

public:
    HySettings( QWidget* parent, QMap<QString,QString>* langs);
    ~HySettings() {};
    QCheckBox* Verbose;
    QCheckBox* Input;
    QComboBox* Language;
    QLabel* Text1;
    QLabel* Text2;
    QLabel* Text3;
    QSpinBox* WordLen;
    QSpinBox* MaxCount;
    QPushButton* OK;
    QPushButton* Cancel;

protected:
    QVBoxLayout* HySettingsLayout;
    QGridLayout* Layout3;
    QHBoxLayout* Layout1;
};

#endif // HYSETTINGS_H
