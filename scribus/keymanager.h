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
    KeyManager( QWidget* parent, QMap<int, Keys> oldKeyMap);
    ~KeyManager() {};

	const QMap<int, Keys> getNewKeyMap();
    bool event( QEvent* ev );
    void keyPressEvent(QKeyEvent *k);
    void keyReleaseEvent(QKeyEvent *k);

public slots:
	void setKeyText();
	void dispKey(int row);
	void setNoKey();

protected:
    QVBoxLayout* keyManagerLayout;
    QGridLayout* keyGroupLayout;
    QHBoxLayout* okCancelLayout;

    QTable* keyTable;
    QButtonGroup* keyGroup;
    QRadioButton* noKey;
    QRadioButton* userDef;
    QLabel* keyDisplay;
    QPushButton* setKeyButton;
    QPushButton* cancelButton;
    QPushButton* okButton;
    QHeader *header;
    QMap<int,Keys> keyMap;
    int actRow;
    int keyCode;
    QString Part1;
    QString Part2;
    QString Part3;
    QString Part4;
    QString getKeyText(int KeyC);
    bool checkKey(int Code);
};

#endif // KEYMANAGER_H
