#ifndef HYSETTINGS_H
#define HYSETTINGS_H

#include <qwidget.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>

class HySettings : public QWidget
{ 
    Q_OBJECT

public:
    HySettings( QWidget* parent, QMap<QString,QString>* langs);
    ~HySettings() {};
    QCheckBox* verbose;
    QCheckBox* input;
    QComboBox* language;
    QLabel* text1;
    QLabel* text2;
    QLabel* text3;
    QSpinBox* wordLen;
    QSpinBox* maxCount;

protected:
    QGridLayout* layout3;
};

#endif // HYSETTINGS_H
