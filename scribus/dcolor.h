/****************************************************************************
** Form interface generated from reading ui file 'Cdel.ui'
**
** Created: Tue Apr 24 22:08:26 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef DELCOLOR_H
#define DELCOLOR_H

#include <qdialog.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qpopupmenu.h>
#include "scribusdoc.h"

class DelColor : public QDialog
{ 
    Q_OBJECT

public:
    DelColor(QWidget* parent, CListe farben, QString Fnam, bool HDoc );
    ~DelColor() {};
    QString EFarbe;
private:
    QLabel* TextLabel1;
    QPushButton* PushButton2;
    QLabel* TextLabel3;
    QPushButton* PushButton3;
    QLabel* TextLabel4;
    QLabel* DColor;
    QPushButton* Ersatz;
    QPopupMenu *Fausw;

protected:
    QVBoxLayout* DLayout;
    QGridLayout* Layout4;
    QHBoxLayout* Layout3;

private slots:
    virtual void ReplaceColor(int);

};

#endif // DELCOLOR_H
