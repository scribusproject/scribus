/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef DELPAGES_H
#define DELPAGES_H

#include "scribusapi.h"
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3HBoxLayout>
#include <QDialog>
#include <QLabel>

class QLabel;
class QLayout;
class QPushbutton;
class QSpinbox;

class SCRIBUS_API DelPages : public QDialog
{ 
    Q_OBJECT

public:
    DelPages( QWidget* parent, int currentPage, int maxPage );
    ~DelPages() {};

	const int getFromPage();
	const int getToPage();

private:
    Q3VBoxLayout* dialogLayout;
    Q3HBoxLayout* fromToLayout;
    Q3HBoxLayout* okCancelLayout;

    QPushButton* cancelButton;
    QPushButton* okButton;
    QLabel* fromLabel;
    QLabel* toLabel;
    QSpinBox* toPageData;
    QSpinBox* fromPageData;

private slots:
    virtual void fromChanged(int pageNumber);
    virtual void toChanged(int pageNumber);
};

#endif // DELPAGES_H
