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
    Q_OBJECT

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
