#ifndef GUIDEMANAGER_H
#define GUIDEMANAGER_H

#include <qdialog.h>
#include <qvaluelist.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qhbuttongroup.h>

#include "mspinbox.h"
#include "page.h"


class GuideManager : public QDialog
{
    Q_OBJECT

public:
	GuideManager(QWidget* parent);
	~GuideManager() {};

	/*! Refresh the guides in the document while the dialog is still opened.
	    Or closed (of course). */
	void refreshDoc();

	QValueList<double> LocHor;
	QValueList<double> LocVer;
	bool LocLocked;

private:
	double LocPageWidth;
	double LocPageHeight;
	double LocTop;
	double LocBottom;
	double LocRight;
	double LocLeft;
	double gx, gy, gw, gh;

	int docUnitIndex;

	int selHor;
	int selVer;

	QGroupBox* VerGroup;
	QListBox* VerList;
	QLabel* TextLabel1;
	MSpinBox* VerSpin;
	QPushButton* VerSet;
	QPushButton* VerDel;

	QGroupBox* HorGroup;
	QListBox* HorList;
	QLabel* TextLabel2;
	MSpinBox* HorSpin;
	QPushButton* HorSet;
	QPushButton* HorDel;

	QSpinBox* ColSpin;
	QSpinBox* RowSpin;
	QHButtonGroup *BGroup;

	QCheckBox* Lock;

	QPushButton* OK;
	QPushButton* Cancel;

	void UnitChange();

	void UpdateHorList();
	void UpdateVerList();

	QHBoxLayout* Layout7;
	QHBoxLayout* Layout8;
	QHBoxLayout* Layout9;
	QHBoxLayout* Layout10;


	QVBoxLayout* GuideManagerLayout;
	QHBoxLayout* Layout6;
	QVBoxLayout* VerGroupLayout;
	QHBoxLayout* Layout2;
	QHBoxLayout* Layout1;

	QVBoxLayout* HorGroupLayout;
	QHBoxLayout* Layout4;
	QHBoxLayout* Layout3;

	QHBoxLayout* Layout5;

	double docUnitRatio;

private slots:

	void DelHorVal();
	void DelVerVal();

	void AddHorVal();
	void AddVerVal();

	void HandleLock();

	void selHorIte(QListBoxItem *c);
	void selVerIte(QListBoxItem *c);

	void ChangeHorVal();
	void ChangeVerVal();
	void AddRows();
	void AddCols();
};

#endif // GUIDEMANAGER_H
