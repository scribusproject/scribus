#ifndef ALIGN_H
#define ALIGN_H

#include <qdialog.h>
#include <qlayout.h>
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include "mspinbox.h"
class ScribusDoc;
class ScribusView;

class Align : public QDialog
{
	Q_OBJECT

public:
	Align( QWidget* parent, int anz, int ein, ScribusDoc* docc, ScribusView* vie);
	~Align();

	QButtonGroup* ButtonGroup1_2;
	QRadioButton* VerteilenV;
	QRadioButton* NichtsV;
	QLabel* TextLabel1_2;
	QLabel* TextLabelD;
	QLabel* TextLabelD2;
	QComboBox* VartV;
	MSpinBox* AVert;
	QButtonGroup* ButtonGroup1;
	QRadioButton* VerteilenH;
	QRadioButton* NichtsH;
	MSpinBox* AHor;
	QComboBox* VartH;
	QLabel* TextLabel1;
	QPushButton* OKbutton;
	QPushButton* ButtonApply;
	QPushButton* CancelB;
	QRadioButton* CheckV;
	QRadioButton* CheckH;
	int Anzahl;
	ScribusDoc* doc;
	ScribusView* view;
	struct ItemPos {
							int Nr;
							double x;
							double y;
						   };
	QValueList<ItemPos> Backup;

public slots:
	virtual void cancel();
	virtual void DistHoriz();
	virtual void DistVert();
	virtual void slotApplyDiag();

signals:
	void ApplyDist(bool, bool, bool, bool, double, double, int, int);

protected:
	QVBoxLayout* AlignLayout;
	QHBoxLayout* Layout4;
	QGridLayout* ButtonGroup1Layout;
	QGridLayout* ButtonGroup1_2Layout;
	QHBoxLayout* Layout3;
};

#endif // ALIGN_H
