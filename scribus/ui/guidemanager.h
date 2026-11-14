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
#include "ui/scrpalettebase.h"
#include "scribusstructs.h"
#include "scpage.h"

class GuidesModel;
class ScribusDoc;
class QToolButton;



/*! \brief Non-modal palette for managing manual and automatic page guides.
The guidemanager.ui form uses ScrPaletteBase as its base class.

\author Petr vanek <petr@scribus.info>
\author Alessandro Rimoldi
\author Franz Schmid
*/
class SCRIBUS_API GuideManager : public ScrPaletteBase, Ui::GuideManager
{
    Q_OBJECT

public:
	GuideManager(QWidget* parent);
	~GuideManager() = default;

	/*! \brief Set the document managed by this palette. */
	void setDoc(ScribusDoc* doc);

	/*! \brief Override ScrPaletteBase::setVisible(). */
	void setVisible(bool visible) override;

	/*! \brief Update the palette when the current page changes. */
	void setupPage(bool storeValues = true);
	void setupGui();

	/*! \brief Reload the horizontal manual-guide model. */
	void clearRestoreHorizontalList();
	/*! \brief Reload the vertical manual-guide model. */
	void clearRestoreVerticalList();

	/*! \brief Get selected standard guides from GUI list.
	Used in highlight painting.
	\retval Guides a list with double values */
	Guides selectedHorizontals() const;
	/*! \brief Get selected standard guides from GUI list.
	Used in highlight painting.
	\retval Guides a list with double values */
	Guides selectedVerticals() const;

	/*! \brief Current page number, or -1 when no page is set. */
	int pageNr() const { return ((currentPage) ? currentPage->pageNr() : -1); }

	void setGuideLock(bool guidesLocked);

protected:
	void changeEvent(QEvent *e) override;
	bool eventFilter(QObject *watched, QEvent *event) override;

private:
	GuidesModel* horizontalModel { nullptr };
	GuidesModel* verticalModel { nullptr };

	ScribusDoc* m_doc { nullptr };
	//! \brief a reference to the current pages
	ScPage* currentPage { nullptr };
	//! \brief A flag to prevent guides drawing when it's not needed
	bool m_drawGuides { true };
	QWidget* m_horizontalEditor { nullptr };
	QWidget* m_verticalEditor { nullptr };
	unsigned int m_pageGeneration { 0 };

	//! \brief Initialise the units. Spin boxes gets pt/mm/etc. extensions here.
	void unitChange();

	//! \brief Document measurements and metrics
	int docUnitIndex { 0 };

	/*! \brief Apply one guide category to the other pages without changing the other category. */
	void copyGuidesToAllPages(GuideManagerCore::GuideType t);

	/*! \brief Store automatic-guide settings and positions for the given page. */
	void storePageValues(ScPage* page) const;

	/*! \brief Create automatic horizontal guides.
	Calculates positions of the guides.
	\param p a Page for what should be calculation performed
	*/
	Guides getAutoHorizontals(const ScPage * p) const;

	/*! \brief Create automatic vertical guides.
	Calculates positions of the guides.
	\param p a Page for what should be calculation performed
	*/
	Guides getAutoVerticals(const ScPage * p) const;

	/*! \brief Refresh the canvas guides. */
	void drawGuides();
	void guideValueChanged(bool horizontal, double oldPosition, double newPosition);
	void addGuide(bool horizontal, double position);
	void setAddButtonEditing(QToolButton* button, bool editing, bool horizontal);
	void discardPendingGuide(bool horizontal);
	void finishGuideEditors();
	void updateClearButtons();
	/*! \brief Remove all guides from \a page and reset its rows and columns. */
	void clearPageGuides(ScPage* page) const;

	/*! \brief Map the mutually exclusive reference buttons to stored indices. */
	void setHorizontalReference(int button);
	int horizontalReference() const;
	void setVerticalReference(int button);
	int verticalReference() const;
	void languageChange();
	void iconSetChange();

private slots:
	//! Wrapper slot for drawGuides()
	void forceDrawGuides(const QItemSelection &, const QItemSelection &);
	void verticalModel_valueChanged(double oldPosition, double newPosition);
	void horizontalModel_valueChanged(double oldPosition, double newPosition);
	void addHorizontalButton_clicked();
	void deleteHorizontalButton_clicked();
	void addVerticalButton_clicked();
	void deleteVerticalButton_clicked();
	void clearHorizontalButton_clicked();
	void clearVerticalButton_clicked();
	void clearPageGuidesButton_clicked();
	void clearAllGuidesButton_clicked();
	void applyToAllPagesButton_clicked();
	void applyAutoToAllPagesButton_clicked();
	void lockGuidesButton_clicked();
	void horizontalAutoCountSpin_valueChanged( double );
	void verticalAutoCountSpin_valueChanged( double );
	void horizontalAutoGapSpin_valueChanged(double);
	void verticalAutoGapSpin_valueChanged(double);
	void horizontalPageAutoButton_toggled(bool);
	void horizontalMarginsAutoButton_toggled(bool);
	void horizontalSelectionAutoButton_toggled(bool);
	void verticalPageAutoButton_toggled(bool);
	void verticalMarginsAutoButton_toggled(bool);
	void verticalSelectionAutoButton_toggled(bool);
};

#endif // GUIDEMANAGER_H
