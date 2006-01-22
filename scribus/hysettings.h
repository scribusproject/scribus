/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef HYSETTINGS_H
#define HYSETTINGS_H

#include <qwidget.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>

#include "scribusapi.h"

class SCRIBUS_API HySettings : public QWidget
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
