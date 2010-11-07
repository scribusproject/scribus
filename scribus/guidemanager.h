/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef GUIDEMANAGER_H
#define GUIDEMANAGER_H

#include "scribusapi.h"
#include "scrspinbox.h"
#include "ui_guidemanager.h"
#include "guidemanagercore.h"
#include "scrpalettebase.h"
#include "scribusstructs.h"
#include "page.h"

class GuidesModel;
class ScribusDoc;



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
	void setupPage(bool storeValues = true);
	void setupGui();

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
	\retval int page no */
	int pageNr() const { return ((currentPage) ? currentPage->pageNr() : -1); }

	int currentIndex() { return tabWidget->currentIndex(); };
	
	virtual void changeEvent(QEvent *e);

private:
	//! \brief Store the guide values in the Qt4 model
	GuidesModel * horizontalModel;
	//! \brief Store the guide values in the Qt4 model
	GuidesModel * verticalModel;

	ScribusDoc* m_Doc;
	//! \brief a reference to the current pages
	Page * currentPage;
	//! \brief A flag to prevent guides drawing when it's not needed
	bool m_drawGuides;

	//! \brief Initialise the units. Spin boxes gets pt/mm/etc. extensions here.
	void unitChange();

	//! \brief Document measurements and metrics
	int docUnitIndex;

	//! \brief suffix of the unit [mm, ...]
	QString suffix;

	/*! \brief Copy guides from currentPage to all remaining.
	All gudes are deleted before copying.
	\param t a type to clear and copy.*/
	void copyGuidesToAllPages(GuideManagerCore::GuideType t);

	/*! \brief Save needed (Auto) values into GuideManagerCore.
	To be restored on the page return.
	\param page A reference to the page to store values. */
	void storePageValues(Page * page);

	/*! \brief Save needed (Auto) values into GuideManagerCore.
	To be restored on the page return.
	\param page A reference to the page to store values. */
	void storePageValues(GuideManagerCore);

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

	/*! \brief Draw guides into painter */
	void drawGuides();

	/*! \brief Theese methods simulate old button group behaviour.
	Is there a better way to do it? QButtonGroup? It's more code...
	*/
	void setHorizontalRefer(int button);
	int horizontalRefer();
	void setVerticalRefer(int button);
	int verticalRefer();
	void languageChange();

private slots:
	//! Wrapper slot for drawGuides()
	void forceDrawGuides(const QItemSelection &, const QItemSelection &);
	void verticalModel_valueChanged();
	void horizontalModel_valueChanged();
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
	void horizontalAutoGapSpin_valueChanged(double);
	void verticalAutoGapSpin_valueChanged(double);
	void horizontalPageAutoButton_toggled(bool);
	void horizontalMarginsAutoButton_toggled(bool);
	void horizontalSelectionAutoButton_toggled(bool);
	void verticalPageAutoButton_toggled(bool);
	void verticalMarginsAutoButton_toggled(bool);
	void verticalSelectionAutoButton_toggled(bool);
	void tabWidget_currentChanged( QWidget * );
	void deletePageButton_clicked();
	void deleteAllGuides_clicked();
};

#endif // GUIDEMANAGER_H
