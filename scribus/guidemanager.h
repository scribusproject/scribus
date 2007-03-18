/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef GUIDEMANAGER_H
#define GUIDEMANAGER_H

#include <q3listview.h>
//Added by qt3to4:
#include <QLabel>
#include "scribusapi.h"
#include "scrspinbox.h"
#include "ui_guidemanager.h"
#include "guidemanagercore.h"
#include "scrpalettebase.h"
#include "scribusstructs.h"
#include "page.h"

class QWidget;
class Q3GroupBox;
class QPushButton;
class QLabel;
class QString;
class QCheckBox;
class Q3HButtonGroup;

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
class SCRIBUS_API GuideListItem : public Q3ListViewItem
{
public:
	//! \brief Only 2 columns here...
	GuideListItem(Q3ListView *parent, QString c1) : Q3ListViewItem(parent, c1){};

	/*! \brief Reimplemented compare method to handle double values.
	When is no double in column col parent string compare() is called.
	\param i QListViewItem to compare with.
	\param col column to sort (0 here)
	\param asc ascendent on true.
	\retval int -1 for (x lt y), 1 for (x gt y). See Qt docs for more info.
	*/
	int compare(Q3ListViewItem *i, int col, bool asc) const;
};


/*! \brief GuideManager is the dialog for guides managing ;).
It's scribus non-modal palette now.
\warning Be careful with UI file guidemanagerbase.ui it uses ScrPaletteBase
as base class instead of QDialog. It should provide correct header file too.

\author Petr vanek <petr@scribus.info>
\author Alessandro Rimoldi
\author Franz Schmid
*/
class SCRIBUS_API GuideManager : public ScrPaletteBase, Ui::GuideManager
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
	marks on the others pages. See GuideManagerCore::drawPage.
	\retval uint page no */
	uint pageNr() const { return currentPage->pageNr(); }

	int currentPageIndex() { return tabWidget->currentPageIndex(); };

private:
	ScribusDoc* m_Doc;
	//! \brief a reference to the current pages
	Page * currentPage;
	//! \brief A flag to prevent guides drawing when it's not needed
	bool m_drawGuides;

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
	void setGuidesFromList(Q3ListView *w, GuideGUIMap *map, Guides guides);

	/*! \brief Add a value from spin box to the list.
	It's called by "add" slots.
	\param list a reference to the QListView to add a value.
	\retval bool false on no add (duplicate etc.), true on success.
	*/
	bool addValueToList(Q3ListView *list);
	/*! \brief Edit a value taken from guides list.
	It's called by "edit" slots.
	\param list a reference to the QListView to edit a value.
	\retval bool false on no edit. */
	bool editValueToList(Q3ListView *list);

	/*! \brief Delete all selected values from list.
	\param list a pointer to the chosen QListView
	\retval bool false on error
	*/
	bool deleteValueFormList(Q3ListView *list);

	/*! \brief Copy guides from currentPage to all remaining.
	All gudes are deleted before copying.
	\param t a type to clear and copy.*/
	void copyGuidesToAllPages(GuideManagerCore::GuideType t);

	/*! \brief Draw guides into painter */
	void drawGuides();

	/*! \brief Save needed (Auto) values into GuideManagerCore.
	To be restored on the page return.
	\param page A reference to the page to store values. */
	void storePageValues(Page * page);

	/*! \brief Overriden QDialog method to operate selection related widgets.
	Auto guides tab. It disables the "selection" radio buttons when there
	is no selection on current page. */
	void windowActivationChange(bool oldActive);

	/*! \brief Create automatic horizontal guides.
	Calculates positions of the guides. */
	Guides getAutoHorizontals();

	/*! \brief Create automatic vertical guides.
	Calculates positions of the guides. */
	Guides getAutoVerticals();

	/*! \brief Recalculate the selection position and measurements for the current page.
	It's used for automatic guides position. It's called for every
	selection GUI widgets change to handle selection change only
	when needed. */
	void resetSelectionForPage();

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
	void horizontalReferGroup_clicked( int );
	void verticalReferGroup_clicked( int );
	void tabWidget_currentChanged( QWidget * );
	void horizontalList_doubleClicked( Q3ListViewItem * );
	void horizontalList_returnPressed( Q3ListViewItem * );
	void verticalList_returnPressed( Q3ListViewItem * );
	void verticalList_doubleClicked( Q3ListViewItem * );
	void horizontalList_selectionChanged();
	void verticalList_selectionChanged();
	void deletePageButton_clicked();
	void deleteAllGuides_clicked();
};

#endif // GUIDEMANAGER_H
