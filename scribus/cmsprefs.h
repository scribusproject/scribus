/****************************************************************************
** Form interface generated from reading ui file 'CMSpref.ui'
**
** Created: Tue Jan 1 10:37:11 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
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
