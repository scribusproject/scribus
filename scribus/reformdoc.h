/****************************************************************************
** Form interface generated from reading ui file 'ReformDoc.ui'
**
** Created: Mon Apr 30 10:53:08 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef REFORMDOC_H
#define REFORMDOC_H

#include <qdialog.h>
#include <qlayout.h>
#include <qcheckbox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include "mspinbox.h"

class ReformDoc : public QDialog
{ 
    Q_OBJECT

public:
    ReformDoc( QWidget* parent, float t, float l, float r, float b, float Pagebr, float Pageho, bool fp, bool fpe, int Einh );
    ~ReformDoc() {};

    QGroupBox* GroupBox7;
    QLabel* TextLabel5;
    QLabel* TextLabel7;
    MSpinBox* TopR;
    MSpinBox* BottomR;
    MSpinBox* LeftR;
    MSpinBox* RightR;
    QCheckBox* Doppelseiten;
    QCheckBox* ErsteSeite;
    QLabel* Links;
    QLabel* Rechts;
    QPushButton* OKButton;
    QPushButton* CancelB;
		int einheit;
		int Breite;
		int Hoehe;

protected:
    QVBoxLayout* ReformDocLayout;
    QHBoxLayout* GroupBox7Layout;
    QGridLayout* Layout4;
    QHBoxLayout* Layout3;

protected slots:
    virtual void setDS();
		virtual void setTop(int v);
		virtual void setBottom(int v);
		virtual void setLeft(int v);
		virtual void setRight(int v);

};

#endif // REFORMDOC_H
