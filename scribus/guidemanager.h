#ifndef GUIDEMANAGER_H
#define GUIDEMANAGER_H

#include <qdialog.h>
#include <qvaluelist.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qtooltip.h>
#include <qhbuttongroup.h>

#include "mspinbox.h"


/*! \brief GuideManager is the dialog for guides managing ;).
Its public interface is used in scrubus.cpp
ScribusApp::ManageGuides() via refreshDoc().
Guides are applied via void Page::addXGuides(QValueList<double>& guides)
and void Page::addYGuides(QValueList<double>& guides).
\author Petr Vanek <petr@yarpen.cz>
\author Alessandro Rimoldi
\author Franz Schmid
*/
class GuideManager : public QDialog
{
    Q_OBJECT

public:
	GuideManager(QWidget* parent);
	~GuideManager() {};

	/*! Refresh the guides in the document while the dialog is still opened.
	    Or closed (of course). */
	void refreshDoc();

private:
	/*! A list with horizontal guides */
	QValueList<double> horizontalGuides;
	/*! A list with vertical guides */
	QValueList<double> verticalGuides;
	/*! "Guides are locked" indicator */
	bool lockedGuides;

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

	//! Set the guides and exit
	QPushButton* okButton;
	//! Exit without setting
	QPushButton* cancelButton;
	//! Set the guides and keep the dialog open
	QPushButton* setButton;

	/*! Guide Gap widgets */
	QCheckBox* useRowGap;
	QCheckBox* useColGap;

	/*! Gaps between guides.
	User can create automatic guides with an optional twoo gapped instead one guide.
	For example: 100mm size - guide - 100mm size will be 95mm size - guide - 10mm gap
	- guide - 95mm size (with 10mm gap) */
	MSpinBox* rowGap;
	MSpinBox* colGap;

	/*! Initialise the units. Spin boxes gets pt/mm/etc. extensions here. */
	void unitChange();

	void UpdateHorList();
	void UpdateVerList();

	double docUnitRatio;

	/*! Calculates the row position of the guide.
	This algorithm is used for guides creating and deleting too.
	\retval QValueList<double> a list with guides positions
	*/
	QValueList<double> getRowValues();

	/*! Calculates the column position of the guide.
	This algorithm is used for guides creating and deleting too.
	\retval QValueList<double> a list with guides positions
	*/
	QValueList<double> getColValues();

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

	/*! Create automatic vertical guides. */
	void addRows();
	/*! Create automatic horizontal guides. */
	void addCols();

	/*! Gap related widget handling (enable/disable)
	\param state enable/disable gaps */
	void useRowGap_clicked(bool state);

	/*! Gap related widget handling (enable/disable)
	\param state enable/disable gaps */
	void useColGap_clicked(bool state);

	/*! Commits all chasnges on exit */
	void commitChanges();

	/*! Commits all chasnges and keep the dialog open */
	void commitEditChanges();
};

#endif // GUIDEMANAGER_H
