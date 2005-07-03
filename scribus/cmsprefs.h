/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef CMSPREFS_H
#define CMSPREFS_H

#include <qdialog.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include "scribusdoc.h"

class CMSPrefs : public QDialog
{ 
    Q_OBJECT

public:
    CMSPrefs( QWidget* parent, CMSset *Vor,
				ProfilesL *InputProfiles, ProfilesL *PrinterProfiles, ProfilesL *MonitorProfiles);
    ~CMSPrefs() {};

    QCheckBox* CheckBox1;
    QGroupBox* SysProfiles;
    QLabel* Text2;
    QLabel* Text1;
    QLabel* Text3;
    QLabel* Text4;
    QComboBox* InputP;
    QComboBox* InputP2;
    QComboBox* MonitorP;
    QComboBox* PrinterP;
    QGroupBox* Render;
    QLabel* Text22;
    QComboBox* MonitorI;
    QComboBox* PrinterI;
    QLabel* Text21;
    QCheckBox* Simulate;
	QCheckBox* GamutC;
	QCheckBox* BlackP;
    QPushButton* Cancel;
    QPushButton* OK;
  	CMSset *Prefs;
  	bool Changed;

public slots:
    virtual void slotCMSon();
    virtual void SetValues();
	virtual void slotSimula();

protected:
    QVBoxLayout* CMSPrefsLayout;
    QGridLayout* SysProfilesLayout;
    QGridLayout* RenderLayout;
    QHBoxLayout* Layout1;
};

#endif // CMSPREFS_H
