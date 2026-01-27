/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "pagelayout.h"

#include <QComboBox>
#include <QToolButton>
#include <QVBoxLayout>
#include <QListWidgetItem>
#include <QLabel>
#include <QPixmap>
#include <QList>

#include "commonstrings.h"
#include "iconmanager.h"
#include "scribusapp.h"
#include "ui/widgets/form_widget.h"
#include "ui/widgets/dropdown_button.h"

PageLayouts::PageLayouts(QWidget* parent)  : QWidget( parent )
{
	struct PageSet pageS;
	pageS.Name = CommonStrings::trPageSet2;
	pageS.FirstPage = 0;
	pageS.Rows = 1;
	pageS.Columns = 1;
	pageS.pageNames.clear();
	pageS.pageNames.append(CommonStrings::trPageLocMiddleRight);
	m_pageSets.append(pageS);

	layoutGroupLayout = new QHBoxLayout(this);
	layoutGroupLayout->setSpacing(4);
	layoutGroupLayout->setContentsMargins(0, 0, 0, 0);

	QFont layFont(this->font());
	layFont.setPointSize(8);

	buttonScheme = new DropdownButton(this);
	buttonScheme->setPopupMode(QToolButton::InstantPopup);

	labelScheme = new FormWidget();
	labelScheme->setFont(layFont);
	labelScheme->addWidget(buttonScheme);
	labelScheme->setLabelVisibility(!m_hideLabels);
	layoutGroupLayout->addWidget( labelScheme );

	buttonFirstPage = new DropdownButton(this);
	buttonFirstPage->setPopupMode(QToolButton::InstantPopup);

	labelPages = new FormWidget();
	labelPages->setFont(layFont);
	labelPages->addWidget( buttonFirstPage );
	labelPages->setLabelVisibility(!m_hideLabels);
	layoutGroupLayout->addWidget( labelPages );

	languageChange();

	connect(ScQApp, SIGNAL(labelVisibilityChanged(bool)), this, SLOT(toggleLabelVisibility(bool)));
	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(languageChange()));
	connect(buttonScheme, &DropdownButton::activated, this, &PageLayouts::changeScheme, Qt::UniqueConnection);
	connect(buttonFirstPage, &DropdownButton::activated, this, &PageLayouts::changeFirstPage, Qt::UniqueConnection);
}

void PageLayouts::updateSchemeSelector(QList<PageSet> pageSets, int pagePositioning)
{
	m_pageSets = pageSets;
	docPagePositioning = pagePositioning;
	labelPages->setVisible( docPagePositioning > 0 );

	reloadScheme();
}

void PageLayouts::setFirstPage(int nr)
{
	if (buttonFirstPage->actions().isEmpty())
		return;

	m_firstPage = qBound(0, nr, static_cast<int>(buttonFirstPage->actions().count()));
	buttonFirstPage->setCurrentIndex(m_firstPage);
}

void PageLayouts::setScheme(int nr)
{
	if (buttonScheme->actions().isEmpty())
		return;

	m_scheme = qBound(0, nr, static_cast<int>(buttonScheme->actions().count()));

	reloadFirstPage(m_scheme);
	buttonScheme->setCurrentIndex(m_scheme);

}

void PageLayouts::setHideLabelsPermanently(bool hide)
{
	m_hideLabels = hide;
	toggleLabelVisibility(!hide);
}

void PageLayouts::reloadScheme()
{
	buttonScheme->clear();

	for (int pg = 0; pg < m_pageSets.count(); ++pg)
	{
		//Leave the code in for 3-/4- fold but exclude from UI in case we bring it back.
		if (pg > 1 && docPagePositioning < 2)
			continue;

		QString psname = CommonStrings::translatePageSetString(m_pageSets.at(pg).Name);
		if (pg == 0)
			buttonScheme->addAction(IconManager::instance().loadIcon("page-simple"), psname)->setData(QVariant(pg));
		else if (pg == 1)
			buttonScheme->addAction(IconManager::instance().loadIcon("page-doublesided"), psname)->setData(QVariant(pg));
		else if (pg == 2)
			buttonScheme->addAction(IconManager::instance().loadIcon("page-3fold"), psname)->setData(QVariant(pg));
		else if (pg == 3)
			buttonScheme->addAction(IconManager::instance().loadIcon("page-4fold"), psname)->setData(QVariant(pg));
		else
			buttonScheme->addAction(IconManager::instance().loadIcon("page-simple"), psname)->setData(QVariant(pg));
	}
}

void PageLayouts::reloadFirstPage(int scheme)
{
	buttonFirstPage->clear();

	// We have to add the other cases if want to support them again
	// CommonStrings::pageLocLeft
	// CommonStrings::pageLocMiddle
	// CommonStrings::pageLocMiddleLeft
	// CommonStrings::pageLocMiddleRight
	// CommonStrings::pageLocRight

	for (int pg = 0; pg < m_pageSets.at(scheme).pageNames.count(); ++pg)
	{
		QString psname = m_pageSets.at(scheme).pageNames.at(pg);

		if (psname == CommonStrings::pageLocLeft)
			buttonFirstPage->addAction(IconManager::instance().loadIcon("page-first-left"), psname)->setData(QVariant(pg));
		else if (psname == CommonStrings::pageLocRight)
			buttonFirstPage->addAction(IconManager::instance().loadIcon("page-first-right"), psname)->setData(QVariant(pg));
		else
			buttonFirstPage->addAction(IconManager::instance().loadIcon("page-first-left"), psname)->setData(QVariant(pg));
	}

}

void PageLayouts::toggleLabelVisibility(bool visibility)
{
	// hide labels always if flag is false
	if (m_hideLabels)
		visibility = false;

	labelScheme->setLabelVisibility(visibility);
	labelPages->setLabelVisibility(visibility);
}

void PageLayouts::languageChange()
{

	reloadScheme();
	reloadFirstPage(m_scheme);

	labelScheme->setText( tr( "Scheme" ) );
	labelPages->setText( tr( "First Page" ) );

	QSignalBlocker sigButtonScheme(buttonScheme);
	buttonScheme->setCurrentIndex(buttonScheme->currentIndex());
	buttonScheme->setToolTip( tr( "Number of pages to show side-by-side on the canvas. Often used for allowing items to be placed across page spreads." ) );

	QSignalBlocker sigFirstPage(buttonFirstPage);
	buttonFirstPage->setCurrentIndex(buttonFirstPage->currentIndex());
	buttonFirstPage->setToolTip( tr( "Location on the canvas where the first page of the document is placed" ) );
}

void PageLayouts::changeScheme(int index)
{
	QAction * action = buttonScheme->menu()->actions().at(qBound(0, index, buttonScheme->menu()->actions().size()));

	if (!action)
		return;

	int ic = action->data().toInt();
	reloadFirstPage(ic);
	labelPages->setVisible( ic > 0 );
	emit schemeChanged(ic);
}

void PageLayouts::changeFirstPage(int index)
{
	QAction * action = buttonFirstPage->menu()->actions().at(qBound(0, index, buttonFirstPage->menu()->actions().size()));

	if (!action)
		return;

	m_firstPage = action->data().toInt();
	emit firstPageChanged(m_firstPage);
}
