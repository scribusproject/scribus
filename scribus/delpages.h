/****************************************************************************
** Form interface generated from reading ui file 'DelPage.ui'
**
** Created: Thu Jun 14 11:37:35 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef DELPAGES_H
#define DELPAGES_H

#include <qdialog.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qspinbox.h>

class DelPages : public QDialog
{ 
    Q_OBJECT

public:
    DelPages( QWidget* parent, int act, int max );
    ~DelPages() {};

    QPushButton* Cancel;
    QPushButton* OK;
    QLabel* TextLabel2;
    QSpinBox* ToPage;
    QLabel* TextLabel1;
    QSpinBox* FromPage;

protected:
    QVBoxLayout* DLayout;
    QHBoxLayout* Layout4;
    QHBoxLayout* Layout3;

public slots:
    virtual void FromChanged(int nr);
    virtual void ToChanged(int nr);

};

#endif // DELPAGES_H
