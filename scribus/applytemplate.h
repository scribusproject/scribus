#ifndef APPLYT_H
#define APPLYT_H

#include <qdialog.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include "scribusview.h"

class ApplyT : public QDialog
{ 
    Q_OBJECT

public:
    ApplyT( QWidget* parent, ScribusView *view);
    ~ApplyT() {};

    QLabel* TextLabel1;
    QComboBox* Templ;
    QPushButton* CancelB;
    QPushButton* OKButton;

protected:
    QGridLayout* ApplyTLayout;
    QHBoxLayout* Layout4;
};

#endif // APPLYT_H
