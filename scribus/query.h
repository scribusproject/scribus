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

class Query : public QDialog
{
    Q_OBJECT

public:
    Query( QWidget* parent=0, const char* name=0, bool modal = FALSE, WFlags fl=0, QString text=0, QString titel=0 );
    ~Query() {};

    QPushButton* PushButton1;
    QPushButton* PushButton2;
    QLineEdit* Answer;
    QLabel* Frage;

public slots:
		void Leave();

protected:
    QVBoxLayout* QueryLayout;
    QHBoxLayout* Layout2;
    QHBoxLayout* Layout1;
};

#endif // QUERY_H
