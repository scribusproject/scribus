/****************************************************************************
** Form interface generated from reading ui file 'Query.ui'
**
** Created: Mon Apr 23 20:48:47 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef QUERY_H
#define QUERY_H

#include <qdialog.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include "scribusdoc.h"

class NewTm : public QDialog
{

public:
    NewTm( QWidget* parent, QString text, QString titel, ScribusDoc *doc );
    ~NewTm() {};

    QPushButton* PushButton1;
    QPushButton* PushButton2;
    QLineEdit* Answer;
    QLabel* Frage;
    QComboBox* Links;

protected:
    QVBoxLayout* QueryLayout;
    QHBoxLayout* Layout3;
    QHBoxLayout* Layout2;
    QHBoxLayout* Layout1;
};

#endif // QUERY_H
