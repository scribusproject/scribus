/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
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
