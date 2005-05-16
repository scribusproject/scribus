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
	//QPushButton* Cancel;

	/*! Guide Gap widgets */
	QCheckBox* useRowGap;
	QCheckBox* useColGap;
	QSpinBox* rowGap;
	QSpinBox* colGap;

	void UnitChange();

	void UpdateHorList();
	void UpdateVerList();

	QHBoxLayout* Layout7;
	QHBoxLayout* Layout8;
	QHBoxLayout* Layout9;
	QHBoxLayout* Layout10;
	QHBoxLayout* hGapLayout;
	QHBoxLayout* vGapLayout;


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

	/*! Calculates the row position of the guide.
	\param iter position in the sequence
	\param offset an offset
	\param spacing space between the guides
	\param gap optional space between twoo gapped guides
	*/
	void addRowGap(int iter, double offset, double spacing, double gap);
	/*! Calculates the row position of the guide.
	\param iter position in the sequence
	\param offset an offset
	\param spacing space between the guides
	\param gap optional space between twoo gapped guides
	*/
	void addColGap(int iter, double offset, double spacing, double gap);

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

	/*! Gap related widget handling (enable/disable)
	\param state enable/disable gaps */
	void useRowGap_clicked(bool state);
	/*! Gap related widget handling (enable/disable)
	\param state enable/disable gaps */
	void useColGap_clicked(bool state);
};

#endif // GUIDEMANAGER_H
