/****************************************************************************
** Form interface generated from reading ui file 'mergedoc.ui'
**
** Created: Sam Apr 5 09:30:16 2003
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef MERGEDOC_H
#define MERGEDOC_H

#include <qdialog.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>


class MergeDoc : public QDialog
{
    Q_OBJECT

public:
    MergeDoc( QWidget* parent = 0);
    ~MergeDoc();

    QLabel* textLabel1;
    QLineEdit* Filename;
    QPushButton* Change;
    QLabel* Inform;
    QLabel* textLabel3;
    QSpinBox* PageNr;
    QPushButton* Import;
    QPushButton* Cancel;
    int Count;

public slots:
		void ChangeFile();

protected:
    QVBoxLayout* MergeDocLayout;
    QHBoxLayout* layout1;
    QHBoxLayout* layout2;
    QHBoxLayout* layout3;


};

#endif // MERGEDOC_H
