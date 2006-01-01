#ifndef GUIDEMANAGER_H
#define GUIDEMANAGER_H

#include "scribusapi.h"
#include "mspinbox.h"

class QWidget;
class QGroupBox;
class QListView;
class QListViewItem;
class QPushButton;
class QLabel;
class QString;
class QCheckBox;
class QHButtonGroup;


/*! \brief \brief GuideManager is the dialog for guides managing ;).
Its public interface is used in scrubus.cpp
ScribusMainWindow::ManageGuides() via refreshDoc().
Guides are applied via void Page::addXGuides(QValueList<double>& guides)
and void Page::addYGuides(QValueList<double>& guides).

\note Current limitations: Applying the guides on all pages with
automatic rows or columns removes old guides. To prevent guides
multiplications.

\author Petr Vanek <petr@yarpen.cz>
\author Alessandro Rimoldi
\author Franz Schmid
*/
class SCRIBUS_API GuideManager : public QDialog
{
    Q_OBJECT

public:
	GuideManager(QWidget* parent);
	~GuideManager();

private:
	//! \brief If there is a selection on the current page
	bool selected;
	//! \brief width of the current page
	double locPageWidth;
	//! \brief height of the current page
	double locPageHeight;
	//! \brief top margin of the current page
	double locTop;
	//! \brief bottom margin of the current page
	double locBottom;
	//! \brief right margin of the current page
	double locRight;
	//! \brief left margin of the current page
	double locLeft;
	//! \brief position of the group of selected objects
	double gx, gy, gw, gh;

	//! \brief vertical guides GUI
	QGroupBox* verGroup;
	QListView* verList;
	QLabel* TextLabel1;
	MSpinBox* verSpin;
	QPushButton* verSet;
	QPushButton* verDel;

	//! \brief horizontal guides GUI
	QGroupBox* horGroup;
	QListView* horList;
	QLabel* TextLabel2;
	MSpinBox* horSpin;
	QPushButton* horSet;
	QPushButton* horDel;

	//! \brief auto guides GUI
	QSpinBox* colSpin;
	QSpinBox* rowSpin;
	QHButtonGroup *bGroup;

	//! \brief lock the guides GUI
	QCheckBox* lockedCheckBox;

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
	User can create automatic guides with an optional two gapped instead one guide.
	For example: 100mm size - guide - 100mm size will be 95mm size - guide - 10mm gap
	- guide - 95mm size (with 10mm gap) */
	MSpinBox* rowGap;
	MSpinBox* colGap;

	/*! \brief Initialise the units. Spin boxes gets pt/mm/etc. extensions here. */
	void unitChange();

	//! \brief Document measurements and metrics
	double docUnitRatio;
	int docUnitIndex;
	int docUnitDecimals;
	//! \brief how much 0.xx
	int docUnitPrecision;
	//! \brief suffix of the unit [mm, ...]
	QString suffix;

	/*! \brief Refresh the guides in the document while the dialog is still opened.
	Or closed (of course). */
	void refreshDoc();

	/*! \brief Create actual guides on all pages in document */
	void refreshWholeDoc();

	/*! \brief Sets the guides from the common list into the GUI.
	\param w a widget to set the values. Horizontal or vertical guides list.
	\param guides a list with values. E.g. the real document guide list.
	*/
	void setGuidesFromList(QListView *w, QValueList<double> guides);

	/*! \brief Create a list with guides from GUI list.
	\param w a widget with list of guides.
	\retval QValueList<double> a list to set as document guides.
	*/
	QValueList<double> getValuesFromList(QListView *w);

	/*! \brief Recalculate the margins and measurements for the current page.
	It's used for automatic guides position. It's called for every
	page when is "apply to all pages" switched on */
	void resetMarginsForPage();

	/*! \brief Add a value from spin box to the list.
	It's called by "add" slots AddHorVal and AddVerVal.
	\param list a reference to the QListView to add a value.
	\param spin a reference to the spin box to take a value.
	\retval bool false on no add (duplicate etc.), true on success.
	*/
	bool addValueToList(QListView *list, MSpinBox *spin);

protected slots:

	/*! \brief delete horizontal value */
	void DelHorVal();
	/*! \brief delete vertical value */
	void DelVerVal();

	/*! \brief add horizontal value */
	void AddHorVal();
	/*! \brief add horizontal value */
	void AddVerVal();

	/*! \brief change horizontal value by spinbox */
	void ChangeHorVal();
	/*! \brief change horizontal value by spinbox */
	void ChangeVerVal();

	/*! \brief Set the spin box by selected list value.
	\param item current (new) item */
	void verList_currentChanged(QListViewItem *item);
	/*! \brief Set the spin box by selected list value.
	\param item current (new) item */
	void horList_currentChanged(QListViewItem *item);

	/*! \brief Create automatic vertical guides.
	Calculates positions of the guides.
	This algorithm is used for guides creating and deleting too.
	*/
	void addRows();

	/*! \brief Create automatic horizontal guides.
	Calculates positions of the guides.
	This algorithm is used for guides creating and deleting too.
	*/
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
