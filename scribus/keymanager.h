/****************************************************************************
** Form interface generated from reading ui file 'KeysManager.ui'
**
** Created: Sun Feb 24 09:24:21 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef KEYMANAGER_H
#define KEYMANAGER_H

#include <qdialog.h>
#include <qbuttongroup.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qtable.h>
#include <qheader.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qevent.h>
#include "scribusdoc.h"

class KeyManager : public QDialog
{ 
    Q_OBJECT

public:
    KeyManager( QWidget* parent, QMap<int,Keys> Ke);
    bool event( QEvent* ev );
    void keyPressEvent(QKeyEvent *k);
    void keyReleaseEvent(QKeyEvent *k);
    QString GetKeyText(int KeyC);
    bool CheckKey(int Code);
    ~KeyManager() {};

    QTable* TastenT;
    QButtonGroup* KeyGroup;
    QRadioButton* NoKey;
    QRadioButton* UserDef;
    QLabel* Tdisplay;
    QPushButton* SetKey;
    QPushButton* CancelB;
    QPushButton* OKButton;
    QHeader *Header;
    QMap<int,Keys> KK;
    int ActRow;
    QString Part1;
    QString Part2;
    QString Part3;
    QString Part4;
    int Kcode;

public slots:
	void SetKeyText();
	void DispKey(int r);
	void SetNoKey();

protected:
    QVBoxLayout* KeyManagerLayout;
    QGridLayout* KeyGroupLayout;
    QHBoxLayout* Layout4;
};

#endif // KEYMANAGER_H
