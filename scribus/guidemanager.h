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

#include "scribusapi.h"
#include "mspinbox.h"


/*! \brief \brief GuideManager is the dialog for guides managing ;).
Its public interface is used in scrubus.cpp
ScribusApp::ManageGuides() via refreshDoc().
Guides are applied via void Page::addXGuides(QValueList<double>& guides)
and void Page::addYGuides(QValueList<double>& guides).
\author Petr Vanek <petr@yarpen.cz>
\author Alessandro Rimoldi
\author Franz Schmid
*/
class SCRIBUS_API GuideManager : public QDialog
{
    Q_OBJECT

public:
	GuideManager(QWidget* parent);
	~GuideManager() {};

private:
	/*! \brief A list with horizontal guides */
	QValueList<double> horizontalGuides;
	/*! \brief A list with vertical guides */
	QValueList<double> verticalGuides;
	/*! \brief "Guides are locked" indicator */
	bool lockedGuides;

	double locPageWidth;
	double locPageHeight;
	double locTop;
	double locBottom;
	double locRight;
	double locLeft;
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

	/*! \brief Set the guides and exit */
	QPushButton* okButton;
	/*! \brief Exit without setting */
	QPushButton* cancelButton;
	/*! \brief Set the guides and keep the dialog open */
	QPushButton* setButton;

	/*! \brief Guide Gap widgets */
	QCheckBox* useRowGap;
	QCheckBox* useColGap;

	/*! \brief Apply selected guides on all pages */
	QCheckBox* allPages;

	/*! \brief Gaps between guides.
	User can create automatic guides with an optional twoo gapped instead one guide.
	For example: 100mm size - guide - 100mm size will be 95mm size - guide - 10mm gap
	- guide - 95mm size (with 10mm gap) */
	MSpinBox* rowGap;
	MSpinBox* colGap;

	/*! \brief Initialise the units. Spin boxes gets pt/mm/etc. extensions here. */
	void unitChange();

	void UpdateHorList();
	void UpdateVerList();

	double docUnitRatio;

	/*! \brief Refresh the guides in the document while the dialog is still opened.
	Or closed (of course). */
	void refreshDoc();

	/*! \brief Create actual guides on all pages in document */
	void refreshWholeDoc();

	/*! \brief Calculates the row position of the guide.
	This algorithm is used for guides creating and deleting too.
	\retval QValueList<double> a list with guides positions
	*/
	QValueList<double> getRowValues();

	/*! \brief Calculates the column position of the guide.
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

	/*! \brief Create automatic vertical guides. */
	void addRows();
	/*! \brief Create automatic horizontal guides. */
	void addCols();

	/*! \brief Gap related widget handling (enable/disable)
	\param state enable/disable gaps */
	void useRowGap_clicked(bool state);

	/*! \brief Gap related widget handling (enable/disable)
	\param state enable/disable gaps */
	void useColGap_clicked(bool state);

	/*! \brief Commits all chasnges on exit */
	void commitChanges();

	/*! \brief Commits all chasnges and keep the dialog open */
	void commitEditChanges();
};

#endif // GUIDEMANAGER_H
