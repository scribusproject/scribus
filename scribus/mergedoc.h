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
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qtooltip.h>


class MergeDoc : public QDialog
{
    Q_OBJECT

public:
    MergeDoc( QWidget* parent, bool Mpages, int targetDocPageCount = -1, int activePage = 1 );
    ~MergeDoc();

    QLabel* textLabel1;
    QLineEdit* Filename;
    QPushButton* Change;
    QLabel* Inform;
    QLabel* textLabel3;
	QLabel* fromLabel;
    QLineEdit* PageNr;
    QComboBox* PageNa;
    QPushButton* Import;
    QPushButton* Cancel;
	QCheckBox* Create;
    QComboBox* Where;
    QSpinBox* ActualPage;
    int Count;
	bool Mpa;
    int targetCount;

public slots:
		void ChangeFile();
		void CheckDestPageStatus( int positionComboSelection );
		void enableCreateWidgets();

protected:
    QVBoxLayout* MergeDocLayout;
    QHBoxLayout* layout1;
    QHBoxLayout* layout2;
    QHBoxLayout* layout2_5;
	QHBoxLayout* layout2_6;
    QHBoxLayout* layout2_7;
    QHBoxLayout* layout3;
};

#endif // MERGEDOC_H
