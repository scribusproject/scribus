#ifndef APPLYT_H
#define APPLYT_H

#include <qdialog.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qspinbox.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include "scribusview.h"

class ApplyT : public QDialog
{ 
    Q_OBJECT

public:
    ApplyT( QWidget* parent, ScribusView *view);
    ~ApplyT() {};

    QLabel* TextLabel1;
    QLabel* TextLabel2;
    QComboBox* Templ;
    QButtonGroup* buttonGroup1;
    QRadioButton* SinglePage;
    QRadioButton* PageRange;
    QSpinBox* FromPage;
    QSpinBox* ToPage;
    QPushButton* CancelB;
    QPushButton* OKButton;

public slots:
		void RangeSel();
		void SingleSel();
		void CheckRangeF();
		void CheckRangeT();

protected:
    QVBoxLayout* ApplyTLayout;
    QHBoxLayout* Layout4;
    QHBoxLayout* layout3;
    QVBoxLayout* buttonGroup1Layout;
    QHBoxLayout* layout2;
};

#endif // APPLYT_H
