/****************************************************************************
** Form interface generated from reading ui file 'InsPage.ui'
**
** Created: Tue Jun 12 21:21:09 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef INSPAGE_H
#define INSPAGE_H

#include <qdialog.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include "scribusdoc.h"

class InsPage : public QDialog
{ 
    Q_OBJECT

public:
    InsPage( QWidget* parent, ScribusDoc* currentDoc, int currentPage, int maxPages, bool facingPages );
    ~InsPage() {};

	const QString getTemplate();
	const QString getTemplate2();
	const int getWhere();
	const int getWherePage();
	const int getCount();

private:
    QLabel* insCountQLabel;
    QLabel* templQLabel;
    QLabel* templ2QLabel;
    QLabel* pagesQLabel;
    QPushButton* cancelQPButton;
    QPushButton* okQPButton;
    QComboBox* insWhereQCBox;
    QComboBox* templQCBox;
    QComboBox* templ2QCBox;
    QSpinBox* insWherePageQSBox;
    QSpinBox* insCountQSBox;
    QVBoxLayout* dialogLayout;
    QGridLayout* whereLayout;
    QHBoxLayout* templLayout;
    QHBoxLayout* templ2Layout;
    QHBoxLayout* okCancelLayout;

private slots:
	void insWherePageQSBoxDisable (int index);
};

#endif // INSPAGE_H
