/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef GUIDEMANAGER_H
#define GUIDEMANAGER_H

#include <qlistview.h>
#include "scribusapi.h"
#include "mspinbox.h"
#include "guidemanagerbase.h"
#include "guidemanagercore.h"
#include "scrpalettebase.h"
#include "scribusstructs.h"
#include "page.h"

class QWidget;
class QGroupBox;
class QPushButton;
class QLabel;
class QString;
class QCheckBox;
class QHButtonGroup;

class ScribusDoc;

/*! \brief Mapping of the GUI representation to the real double values.
It's againts the divide/multiple rounding errors
\author Petr Vanek <petr@scribus.info>
 */
typedef QMap<QString,double> GuideGUIMap;


/*! \brief Inherited QListViewItem provides double number values sorting.
Guides lists contains double values in 1st (0) columns. Standard QListViewItem
provides string sorting so I have to create some special number related one ;)
Due the rounding problems in divisions and multiplications of the GUI representation
of the guide, there are two internal mappings: GuideGUIMap. There are hold the
real double values as they are in page.guides structures mapped into QString
representation i this GUI palette.
\author Petr Vanek <petr@scribus.info>
*/
class SCRIBUS_API GuideListItem : public QListViewItem
{
public:
	//! \brief Only 2 columns here...
	GuideListItem(QListView *parent, QString c1, QString c2) : QListViewItem(parent, c1, c2){};

	/*! \brief Reimplemented compare method to handle double values.
	When is no double in column col parent string compare() is called.
	\param i QListViewItem to compare with.
	\param col column to sort (0 here)
	\param asc ascendent on true.
	\retval int -1 for (x lt y), 1 for (x gt y). See Qt docs for more info.
	*/
	int compare(QListViewItem *i, int col, bool asc) const;
};


/*! \brief GuideManager is the dialog for guides managing ;).
It's scribus non-modal palette now.
\warning Be careful with UI file guidemanagerbase.ui it uses ScrPaletteBase
as base cleass instead of QDialog. It should provide correct header file too.

\author Petr Vanek <petr@yarpen.cz>
\author Alessandro Rimoldi
\author Franz Schmid
*/
class SCRIBUS_API GuideManager : public GuideManagerBase//, public ScrPaletteBase
{
    Q_OBJECT

public:
	GuideManager(QWidget* parent);
	~GuideManager(){};

	/*! \brief Set the doc fo the guidemanager to work on. */
	void setDoc(ScribusDoc* doc);

	/*! \brief Set the widgets on the page change.
	It has to be called on every page to page transition */
	void setupPage();

	/*! \brief Clear and reset the GUI horizontal list widget. */
	void clearRestoreHorizontalList();
	/*! \brief Clear and reset the GUI vertical list widget. */
	void clearRestoreVerticalList();

	/*! \brief Get selected standard guides from GUI list.
	Used in highlight painting.
	\retval Guides a list with double values */
	Guides selectedHorizontals();
	/*! \brief Get selected standard guides from GUI list.
	Used in highlight painting.
	\retval Guides a list with double values */
	Guides selectedVerticals();

	/*! \brief check the current page number to prevent drawing
	marks on the others pages.
	\retval uint page no */
	uint pageNr() const { return currentPage->pageNr(); }

private:
	ScribusDoc* m_Doc;
	//! \brief a reference to the current pages
	Page * currentPage;
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

	//! \brief Mapping of the GUI representation to the real double values.
	GuideGUIMap m_horMap;
	GuideGUIMap m_verMap;

	//! \brief Initialise the units. Spin boxes gets pt/mm/etc. extensions here.
	void unitChange();

	//! \brief Document measurements and metrics
	double docUnitRatio;
	int docUnitIndex;
	int docUnitDecimals;
	//! \brief how much 0.xx
	int docUnitPrecision;
	//! \brief suffix of the unit [mm, ...]
	QString suffix;

	/*! \brief Sets the guides from the common list into the GUI.
	\param w a widget to set the values. Horizontal or vertical guides list.
	\param guides a list with values. E.g. the real document guide list.
	*/
	void setGuidesFromList(QListView *w, GuideGUIMap *map, Guides guides);

	/*! \brief Recalculate the margins and measurements for the current page.
	It's used for automatic guides position. It's called for every
	page when is "apply to all pages" switched on */
	void resetMarginsForPage();

	/*! \brief Add a value from spin box to the list.
	It's called by "add" slots.
	\param list a reference to the QListView to add a value.
	\retval bool false on no add (duplicate etc.), true on success.
	*/
	bool addValueToList(QListView *list);
	/*! \brief Edit a value taken from guides list.
	It's called by "edit" slots.
	\param list a reference to the QListView to edit a value.
	\retval bool false on no edit. */
	bool editValueToList(QListView *list);

	/*! \brief Delete all selected values from list.
	\param list a pointer to the chosen QListView
	\retval bool false on error
	*/
	bool deleteValueFormList(QListView *list);

	/*! \brief Create automatic horizontal guides.
	Calculates positions of the guides. */
	void getAutoHorizontals();

	/*! \brief Create automatic vertical guides.
	Calculates positions of the guides. */
	void getAutoVerticals();

	/*! \brief Copy guides from currentPage to all remaining.
	All gudes are deleted before copying.
	\param t a type to clear and copy.*/
	void copyGuidesToAllPages(GuideManagerCore::GuideType t);

	/*! \brief Draw guides into painter */
	void drawGuides();

	/*! \brief Save needed (Auto) values into GuideManagerCore.
	To be restored on the page return. */
	void storePreviousValues();

protected slots:
	void addHorButton_clicked();
	void delHorButton_clicked();
	void addVerButton_clicked();
	void delVerButton_clicked();
	void applyToAllStdButton_clicked();
	void lockCheck_stateChanged( int );
	void horizontalAutoGapCheck_stateChanged( int );
	void verticalAutoGapCheck_stateChanged( int );
	void applyToAllAutoButton_clicked();
	void horizontalAutoCountSpin_valueChanged( int );
	void verticalAutoCountSpin_valueChanged( int );
	void horizontalAutoGapSpin_valueChanged(int);
	void verticalAutoGapSpin_valueChanged(int);
	void bGroup_clicked( int );
	void tabWidget_currentChanged( QWidget * );
	void horizontalList_doubleClicked( QListViewItem * );
	void horizontalList_returnPressed( QListViewItem * );
	void verticalList_returnPressed( QListViewItem * );
	void verticalList_doubleClicked( QListViewItem * );
	void horizontalList_selectionChanged();
	void verticalList_selectionChanged();
};

#endif // GUIDEMANAGER_H
