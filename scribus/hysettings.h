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
    QSpinBox* WordLen;
    QPushButton* OK;
    QPushButton* Cancel;


protected:
    QVBoxLayout* HySettingsLayout;
    QGridLayout* Layout3;
    QHBoxLayout* Layout1;
};

#endif // HYSETTINGS_H
