#ifndef REFORMDOC_H
#define REFORMDOC_H

#include <qdialog.h>
#include <qlayout.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include "mspinbox.h"

class ReformDoc : public QDialog
{ 
    Q_OBJECT

public:
    ReformDoc( QWidget* parent, double t, double l, double r, double b, double Pagebr, double Pageho, 
				bool fp, bool fpe, int Einh, int ori, QString pageSize, int PageNr );
    ~ReformDoc() {};

    QGroupBox* GroupBox7;
    QGroupBox* dsGroupBox7;
    QLabel* TextLabel5;
    QLabel* TextLabel7;
    QLabel* TextLabel1_3;
    QSpinBox* PgNr;
    MSpinBox* TopR;
    MSpinBox* BottomR;
    MSpinBox* LeftR;
    MSpinBox* RightR;
    QCheckBox* Doppelseiten;
    QCheckBox* ErsteSeite;
    QLabel* Links;
    QLabel* Rechts;
	QLabel*	widthQLabel;
	QLabel*	heightQLabel;
	QLabel*	sizeQLabel;
	QLabel*	orientationQLabel;
	QLabel*	sizedataQLabel;
	QLabel*	orientationdataQLabel;
    QLabel* widthdataQLabel;
    QLabel* heightdataQLabel;
    QPushButton* OKButton;
    QPushButton* CancelB;
	int einheit;
	double Breite;
	double Hoehe;

protected:
    QVBoxLayout* ReformDocLayout;
    QHBoxLayout* dsGroupBox7Layout;
    QHBoxLayout* GroupBox7Layout;
    QGridLayout* dsLayout4;
    QGridLayout* Layout4;
    QHBoxLayout* Layout3;

protected slots:
    virtual void setDS();
	virtual void setTop(int v);
	virtual void setBottom(int v);
	virtual void setLeft(int v);
	virtual void setRight(int v);
};

#endif // REFORMDOC_H
