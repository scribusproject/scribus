/****************************************************************************
** Form interface generated from reading ui file 'InsPage.ui'
**
** Created: Tue Jun 12 21:21:09 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef INSPAGE_H
#define INSPAGE_H

#include <qdialog.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include "scribusview.h"

class InsPage : public QDialog
{ 
    Q_OBJECT

public:
    InsPage( QWidget* parent, ScribusView* view, int act, int max, bool ds );
    ~InsPage() {};

    QLabel* TextLabel1;
    QPushButton* PushButton4_2;
    QPushButton* PushButton4;
    QComboBox* Where;
    QComboBox* Based;
    QComboBox* Based2;
    QLabel* TextLabel3;
    QLabel* TextLabel3a;
    QLabel* TextLabel2;
    QSpinBox* ActualPage;
    QSpinBox* NumPages;

protected:
    QVBoxLayout* DLayout;
    QGridLayout* Layout4;
    QHBoxLayout* Layout4a;
    QHBoxLayout* Layout4b;
    QHBoxLayout* Layout3;

};

#endif // INSPAGE_H
