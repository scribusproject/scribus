/****************************************************************************
** Form interface generated from reading ui file 'Info.ui'
**
** Created: Mon Apr 30 10:14:32 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef DOCINFOS_H
#define DOCINFOS_H

#include <qdialog.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qmultilineedit.h>
#include <qpushbutton.h>

class DocInfos : public QDialog
{
public:
    DocInfos( QWidget* parent, QString Autor, QString Titel, QString Des, QString keyW );
    ~DocInfos() {};

    QLabel* TextLabel3;
    QLineEdit* AutorInfo;
    QLabel* TextLabel1;
    QLineEdit* TitelInfo;
    QMultiLineEdit* MultiLineEdit1;
    QLabel* TextLabel2;
    QLabel* TextLabel3a;
    QMultiLineEdit* MultiLineEdit2;
    QPushButton* Cancel;
    QPushButton* OK;

protected:
    QVBoxLayout* DocInfosLayout;
    QGridLayout* Layout1;
    QHBoxLayout* Layout3;

};

#endif // DOCINFOS_H
