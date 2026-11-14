/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							  guidemanager.cpp
							 -------------------
	begin			   : Fri Jan 30 2004
	copyright		   : (C) 2004 by Alessandro Rimoldi
	copyright		   : (C) by Franz Schmid
	e-mail			 : http://www.ideale.ch/contact
 ***************************************************************************/

/***************************************************************************
 *																	   *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or   *
 *   (at your option) any later version.								   *
 *																	   *
 ***************************************************************************/

#include <QEvent>
#include <QMetaObject>
#include <QSignalBlocker>

#include "guidemanager.h"
#include "guidesdelegate.h"
#include "guidesmodel.h"
#include "guidesview.h"
#include "iconmanager.h"
#include "scpage.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "undomanager.h"
#include "units.h"

GuideManager::GuideManager(QWidget* parent) :
		ScrPaletteBase(parent, "GuideManager")
{
	setupUi(this);
	horizontalAutoGapSpin->setMinimum(0.0);
	horizontalAutoGapSpin->setMaximum(10000.0);
	verticalAutoGapSpin->setMinimum(0.0);
	verticalAutoGapSpin->setMaximum(10000.0);
	horizontalAutoCountSpin->setDecimals(0);
	verticalAutoCountSpin->setDecimals(0);

	// Clicking the checkmark must not close the editor before the click handler.
	// Tab still reaches these buttons through ordinary keyboard navigation.
	addHorizontalButton->setFocusPolicy(Qt::TabFocus);
	addVerticalButton->setFocusPolicy(Qt::TabFocus);
	horizontalGroupBox->installEventFilter(this);
	verticalGroupBox->installEventFilter(this);

	horizontalModel = new GuidesModel(this);
	verticalModel = new GuidesModel(this);
	GuidesHDelegate* hdelegate = new GuidesHDelegate(horizontalView);
	GuidesVDelegate* vdelegate = new GuidesVDelegate(verticalView);
	horizontalView->setItemDelegateForColumn(0, hdelegate);
	verticalView->setItemDelegateForColumn(0, vdelegate);
	horizontalView->setModel(horizontalModel);
	verticalView->setModel(verticalModel);

	updateClearButtons();

	connect(hdelegate, &GuidesDelegate::editorOpened, this, [this](QWidget *editor) {
		m_horizontalEditor = editor;
		setAddButtonEditing(addHorizontalButton, true, true);
		connect(editor, &QObject::destroyed, this, [this, editor]() {
			if (m_horizontalEditor == editor)
			{
				m_horizontalEditor = nullptr;
				setAddButtonEditing(addHorizontalButton, false, true);
			}
		});
	});
	connect(vdelegate, &GuidesDelegate::editorOpened, this, [this](QWidget *editor) {
		m_verticalEditor = editor;
		setAddButtonEditing(addVerticalButton, true, false);
		connect(editor, &QObject::destroyed, this, [this, editor]() {
			if (m_verticalEditor == editor)
			{
				m_verticalEditor = nullptr;
				setAddButtonEditing(addVerticalButton, false, false);
			}
		});
	});
	connect(hdelegate, &QAbstractItemDelegate::closeEditor, this, [this](QWidget *editor) {
		if (m_horizontalEditor == editor)
		{
			m_horizontalEditor = nullptr;
			setAddButtonEditing(addHorizontalButton, false, true);
			// Allow QTableView to finish closing the editor before removing its row.
			const auto generation = m_pageGeneration;
			QMetaObject::invokeMethod(this, [this, generation]() {
				if (m_pageGeneration == generation)
					discardPendingGuide(true);
			}, Qt::QueuedConnection);
		}
	});
	connect(vdelegate, &QAbstractItemDelegate::closeEditor, this, [this](QWidget *editor) {
		if (m_verticalEditor == editor)
		{
			m_verticalEditor = nullptr;
			setAddButtonEditing(addVerticalButton, false, false);
			const auto generation = m_pageGeneration;
			QMetaObject::invokeMethod(this, [this, generation]() {
				if (m_pageGeneration == generation)
					discardPendingGuide(false);
			}, Qt::QueuedConnection);
		}
	});

	connect(addHorizontalButton, &QToolButton::clicked, this, &GuideManager::addHorizontalButton_clicked);
	connect(deleteHorizontalButton, &QToolButton::clicked, this, &GuideManager::deleteHorizontalButton_clicked);
	connect(clearHorizontalButton, &QToolButton::clicked, this, &GuideManager::clearHorizontalButton_clicked);
	connect(horizontalView, &GuidesView::deleteKeyPressed, this, &GuideManager::deleteHorizontalButton_clicked);
	connect(horizontalView, &GuidesView::plusKeyPressed, this, &GuideManager::addHorizontalButton_clicked);
	connect(addVerticalButton, &QToolButton::clicked, this, &GuideManager::addVerticalButton_clicked);
	connect(deleteVerticalButton, &QToolButton::clicked, this, &GuideManager::deleteVerticalButton_clicked);
	connect(clearVerticalButton, &QToolButton::clicked, this, &GuideManager::clearVerticalButton_clicked);
	connect(clearPageGuidesButton, &QToolButton::clicked, this, &GuideManager::clearPageGuidesButton_clicked);
	connect(clearAllGuidesButton, &QToolButton::clicked, this, &GuideManager::clearAllGuidesButton_clicked);
	connect(verticalView, &GuidesView::deleteKeyPressed, this, &GuideManager::deleteVerticalButton_clicked);
	connect(verticalView, &GuidesView::plusKeyPressed, this, &GuideManager::addVerticalButton_clicked);
	connect(horizontalView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &GuideManager::forceDrawGuides);
	connect(verticalView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &GuideManager::forceDrawGuides);

	connect(applyToAllPagesButton, &QToolButton::clicked, this, &GuideManager::applyToAllPagesButton_clicked);
	connect(applyAutoToAllPagesButton, &QToolButton::clicked, this, &GuideManager::applyAutoToAllPagesButton_clicked);
	connect(horizontalAutoCountSpin, qOverload<double>(&ScrSpinBox::valueChanged), this, qOverload<double>(&GuideManager::horizontalAutoCountSpin_valueChanged));
	connect(verticalAutoCountSpin, qOverload<double>(&ScrSpinBox::valueChanged), this, &GuideManager::verticalAutoCountSpin_valueChanged);
	connect(horizontalAutoGapSpin, qOverload<double>(&ScrSpinBox::valueChanged), this, qOverload<double>(&GuideManager::horizontalAutoGapSpin_valueChanged));
	connect(verticalAutoGapSpin, qOverload<double>(&ScrSpinBox::valueChanged), this, qOverload<double>(&GuideManager::verticalAutoGapSpin_valueChanged));

	connect(horizontalPageAutoButton, &QToolButton::toggled, this, &GuideManager::horizontalPageAutoButton_toggled);
	connect(horizontalMarginsAutoButton, &QToolButton::toggled, this, &GuideManager::horizontalMarginsAutoButton_toggled);
	connect(horizontalSelectionAutoButton, &QToolButton::toggled, this, &GuideManager::horizontalSelectionAutoButton_toggled);
	connect(verticalPageAutoButton, &QToolButton::toggled, this, &GuideManager::verticalPageAutoButton_toggled);
	connect(verticalMarginsAutoButton, &QToolButton::toggled, this, &GuideManager::verticalMarginsAutoButton_toggled);
	connect(verticalSelectionAutoButton, &QToolButton::toggled, this, &GuideManager::verticalSelectionAutoButton_toggled);

	connect(lockGuidesButton, &QToolButton::clicked, this, &GuideManager::lockGuidesButton_clicked);

	connect(horizontalModel, &GuidesModel::valueChanged, this, &GuideManager::horizontalModel_valueChanged);
	connect(verticalModel, &GuidesModel::valueChanged, this, &GuideManager::verticalModel_valueChanged);
	connect(horizontalModel, &GuidesModel::guideAdded, this, [this](double pos) { addGuide(true, pos); });
	connect(verticalModel, &GuidesModel::guideAdded, this, [this](double pos) { addGuide(false, pos); });
}



void GuideManager::setDoc(ScribusDoc* doc)
{
	finishGuideEditors();
	++m_pageGeneration;
	if (m_doc && (m_doc != doc))
	{
		if (currentPage && (currentPage->doc() == m_doc))
			storePageValues(currentPage);
		currentPage = nullptr;
	}

	m_doc = doc;
	if (m_doc)
	{
		int oldUnitIndex = docUnitIndex;
		if (oldUnitIndex != m_doc->unitIndex())
			unitChange();
	}

	qobject_cast<GuidesHDelegate*>(horizontalView->itemDelegateForColumn(0))->setDoc(doc);
	qobject_cast<GuidesVDelegate*>(verticalView->itemDelegateForColumn(0))->setDoc(doc);
	if (!m_doc)
		currentPage = nullptr;
	setEnabled(m_doc != nullptr);
	updateClearButtons();
	const bool multiplePages = m_doc && m_doc->Pages->count() > 1;
	applyToAllPagesButton->setEnabled(multiplePages);
	applyAutoToAllPagesButton->setEnabled(multiplePages);
}

void GuideManager::setVisible(bool visible)
{
	ScrPaletteBase::setVisible(visible);
	drawGuides();
}

void GuideManager::setupPage(bool storeValues)
{
	if (!m_doc)
		return;
	finishGuideEditors();
	++m_pageGeneration;
	m_drawGuides = false;
	setEnabled(true);
	if (currentPage && storeValues)
		storePageValues(currentPage);

	currentPage = m_doc->currentPage();
	unitChange();
	setupGui();
	const bool multiplePages = m_doc->Pages->count() > 1;
	applyToAllPagesButton->setEnabled(multiplePages);
	applyAutoToAllPagesButton->setEnabled(multiplePages);
}

void GuideManager::setupGui()
{
	if (!m_doc || !currentPage)
		return;
	// Restore controls without treating a page change as a guide edit.
	clearRestoreHorizontalList();
	clearRestoreVerticalList();
	updateClearButtons();
	QSignalBlocker horizontalCountBlock(horizontalAutoCountSpin);
	QSignalBlocker horizontalGapBlock(horizontalAutoGapSpin);
	QSignalBlocker verticalCountBlock(verticalAutoCountSpin);
	QSignalBlocker verticalGapBlock(verticalAutoGapSpin);
	QSignalBlocker horizontalPageBlock(horizontalPageAutoButton);
	QSignalBlocker horizontalMarginsBlock(horizontalMarginsAutoButton);
	QSignalBlocker horizontalSelectionBlock(horizontalSelectionAutoButton);
	QSignalBlocker verticalPageBlock(verticalPageAutoButton);
	QSignalBlocker verticalMarginsBlock(verticalMarginsAutoButton);
	QSignalBlocker verticalSelectionBlock(verticalSelectionAutoButton);
	QSignalBlocker lockBlock(lockGuidesButton);
	horizontalAutoCountSpin->setValue(currentPage->guides.horizontalAutoCount());
	horizontalAutoGapSpin->setValue(currentPage->guides.horizontalAutoGap(), SC_PT);
	horizontalAutoGapSpin->setEnabled(horizontalAutoCountSpin->value() > 0);
	setHorizontalReference(currentPage->guides.horizontalAutoRefer());
	horizontalSelectionAutoButton->setEnabled(m_doc->m_Selection->isNotEmpty());

	verticalAutoCountSpin->setValue(currentPage->guides.verticalAutoCount());
	verticalAutoGapSpin->setValue(currentPage->guides.verticalAutoGap(), SC_PT);
	verticalAutoGapSpin->setEnabled(verticalAutoCountSpin->value() > 0);
	setVerticalReference(currentPage->guides.verticalAutoRefer());
	verticalSelectionAutoButton->setEnabled(m_doc->m_Selection->isNotEmpty());

	lockGuidesButton->setChecked(m_doc->GuideLock);

	iconSetChange();
	languageChange();

	m_drawGuides = true;
}

void GuideManager::storePageValues(ScPage* page) const
{
	if (!page || !m_doc)
		return;

	page->guides.setHorizontalAutoGap(horizontalAutoGapSpin->getValue(SC_PT));
	page->guides.setHorizontalAutoCount(static_cast<int>(horizontalAutoCountSpin->value()));
	page->guides.setHorizontalAutoRefer(horizontalReference());
	page->guides.addHorizontals(getAutoHorizontals(page), GuideManagerCore::Auto);

	page->guides.setVerticalAutoGap(verticalAutoGapSpin->getValue(SC_PT));
	page->guides.setVerticalAutoCount(static_cast<int>(verticalAutoCountSpin->value()));
	page->guides.setVerticalAutoRefer(verticalReference());
	page->guides.addVerticals(getAutoVerticals(page), GuideManagerCore::Auto);
}

void GuideManager::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void GuideManager::languageChange()
{
	retranslateUi(this);
	setAddButtonEditing(addHorizontalButton, m_horizontalEditor != nullptr, true);
	setAddButtonEditing(addVerticalButton, m_verticalEditor != nullptr, false);
}

void GuideManager::discardPendingGuide(bool horizontal)
{
	if (!currentPage || (horizontal ? m_horizontalEditor : m_verticalEditor))
		return;
	(horizontal ? horizontalModel : verticalModel)->discardPendingRow();
	updateClearButtons();
}

void GuideManager::finishGuideEditors()
{
	if (m_horizontalEditor)
		qobject_cast<GuidesDelegate*>(horizontalView->itemDelegateForColumn(0))->finishEditing(m_horizontalEditor);
	if (m_verticalEditor)
		qobject_cast<GuidesDelegate*>(verticalView->itemDelegateForColumn(0))->finishEditing(m_verticalEditor);
}

void GuideManager::updateClearButtons()
{
	const bool hasPage = m_doc && currentPage;
	clearHorizontalButton->setEnabled(hasPage && !currentPage->guides.horizontals(GuideManagerCore::Standard).isEmpty());
	clearVerticalButton->setEnabled(hasPage && !currentPage->guides.verticals(GuideManagerCore::Standard).isEmpty());
}

void GuideManager::iconSetChange()
{
	IconManager& im = IconManager::instance();

	addHorizontalButton->setIcon(im.loadIcon("list-add"));
	deleteHorizontalButton->setIcon(im.loadIcon("list-remove"));
	clearHorizontalButton->setIcon(im.loadIcon("trashcan"));
	addVerticalButton->setIcon(im.loadIcon("list-add"));
	deleteVerticalButton->setIcon(im.loadIcon("list-remove"));
	clearVerticalButton->setIcon(im.loadIcon("trashcan"));
	clearPageGuidesButton->setIcon(im.loadIcon("trashcan"));
	clearAllGuidesButton->setIcon(im.loadIcon("trashcan"));

	horizontalPageAutoButton->setIcon(im.loadIcon("align-to-page"));
	horizontalMarginsAutoButton->setIcon(im.loadIcon("align-to-margin"));
	horizontalSelectionAutoButton->setIcon(im.loadIcon("align-to-last-selection"));
	verticalPageAutoButton->setIcon(im.loadIcon("align-to-page"));
	verticalMarginsAutoButton->setIcon(im.loadIcon("align-to-margin"));
	verticalSelectionAutoButton->setIcon(im.loadIcon("align-to-last-selection"));

	applyToAllPagesButton->setIcon(im.loadIcon("page-duplicate"));
	applyAutoToAllPagesButton->setIcon(im.loadIcon("page-duplicate"));

	QIcon lockIcon;
	lockIcon.addPixmap(im.loadPixmap("lock"), QIcon::Normal, QIcon::On);
	lockIcon.addPixmap(im.loadPixmap("lock-unlocked"), QIcon::Normal, QIcon::Off);
	lockGuidesButton->setIcon(lockIcon);
}

void GuideManager::setAddButtonEditing(QToolButton* button, bool editing, bool horizontal)
{
	button->setIcon(IconManager::instance().loadIcon(editing ? "ok" : "list-add"));
	button->setToolTip(editing ? tr("Finish editing")
	                           : horizontal ? tr("Add a new horizontal guide")
	                                        : tr("Add a new vertical guide"));
}

bool GuideManager::eventFilter(QObject *watched, QEvent *event)
{
	if (event->type() == QEvent::MouseButtonPress &&
	    (watched == horizontalGroupBox || watched == verticalGroupBox))
	{
		// Guide-list blank space already finishes editing. Match it on the
		// surrounding background without changing the selected guide.
		if (m_horizontalEditor)
			horizontalView->setFocus(Qt::MouseFocusReason);
		else if (m_verticalEditor)
			verticalView->setFocus(Qt::MouseFocusReason);
	}
	return ScrPaletteBase::eventFilter(watched, event);
}

void GuideManager::unitChange()
{
	if (!m_doc)
		return;

	docUnitIndex = m_doc->unitIndex();
	int docUnitDecimals = unitGetPrecisionFromIndex(docUnitIndex);

	QSignalBlocker horizontalAutoGapSpinBlock(horizontalAutoGapSpin);
	QSignalBlocker verticalAutoGapSpinBlock(verticalAutoGapSpin);
	horizontalAutoGapSpin->setNewUnit(docUnitIndex);
	verticalAutoGapSpin->setNewUnit(docUnitIndex);

	horizontalModel->unitChange(docUnitIndex, docUnitDecimals, m_doc->rulerYoffset);
	verticalModel->unitChange(docUnitIndex, docUnitDecimals, m_doc->rulerXoffset);
	qobject_cast<GuidesHDelegate*>(horizontalView->itemDelegateForColumn(0))->setDoc(m_doc);
	qobject_cast<GuidesVDelegate*>(verticalView->itemDelegateForColumn(0))->setDoc(m_doc);
}



void GuideManager::deleteHorizontalButton_clicked()
{
	if (!currentPage)
		return;
	finishGuideEditors();
	discardPendingGuide(true);
	const QModelIndexList indexes = horizontalView->selectionModel()->selectedRows(0);
	Guides v;

	if (indexes.isEmpty())
		return;
	for (const QModelIndex& ix : indexes)
		// here *must* go EditRole due truncations in DisplayRole
		// see GuidesModel::data()
		v.append(horizontalModel->data(ix, Qt::UserRole).toDouble());

	horizontalModel->removeValues(v);

	currentPage->guides.clearHorizontals(GuideManagerCore::Standard);
	currentPage->guides.addHorizontals(horizontalModel->values(), GuideManagerCore::Standard);
	drawGuides();
	m_doc->changed();

	updateClearButtons();
}

void GuideManager::deleteVerticalButton_clicked()
{
	if (!currentPage)
		return;
	finishGuideEditors();
	discardPendingGuide(false);
	const QModelIndexList indexes = verticalView->selectionModel()->selectedRows(0);
	Guides v;

	if (indexes.isEmpty())
		return;
	for (const QModelIndex& ix : indexes)
		// here *must* go EditRole due truncations in DisplayRole
		// see GuidesModel::data()
		v.append(verticalModel->data(ix, Qt::UserRole).toDouble());

	verticalModel->removeValues(v);

	currentPage->guides.clearVerticals(GuideManagerCore::Standard);
	currentPage->guides.addVerticals(verticalModel->values(), GuideManagerCore::Standard);
	drawGuides();
	m_doc->changed();

	updateClearButtons();
}

void GuideManager::clearHorizontalButton_clicked()
{
	if (!currentPage)
		return;
	finishGuideEditors();
	if (!currentPage->guides.horizontals(GuideManagerCore::Standard).empty())
	{
		UndoTransaction trans;
		if (UndoManager::undoEnabled())
			trans = UndoManager::instance()->beginTransaction(currentPage->getUName(),
				currentPage->getUPixmap(),
				Um::RemoveHGuides, "",
				Um::IGuides);
		currentPage->guides.clearHorizontals(GuideManagerCore::Standard);

		if (trans)
			trans.commit();

		m_doc->changed();
	}

	clearRestoreHorizontalList();
	drawGuides();

	updateClearButtons();
}

void GuideManager::clearVerticalButton_clicked()
{
	if (!currentPage)
		return;
	finishGuideEditors();
	if (!currentPage->guides.verticals(GuideManagerCore::Standard).empty())
	{
		UndoTransaction trans;
		if (UndoManager::undoEnabled())
			trans = UndoManager::instance()->beginTransaction(currentPage->getUName(),
				currentPage->getUPixmap(),
				Um::RemoveVGuides, "",
				Um::IGuides);
		currentPage->guides.clearVerticals(GuideManagerCore::Standard);

		if (trans)
			trans.commit();

		m_doc->changed();
	}

	clearRestoreVerticalList();
	drawGuides();

	updateClearButtons();
}

void GuideManager::clearPageGuides(ScPage* page) const
{
	page->guides.clearHorizontals(GuideManagerCore::Standard);
	page->guides.clearVerticals(GuideManagerCore::Standard);
	page->guides.clearHorizontals(GuideManagerCore::Auto);
	page->guides.clearVerticals(GuideManagerCore::Auto);

	// Rows and columns go back to their defaults
	const GuideManagerCore defaults;
	page->guides.setHorizontalAutoCount(defaults.horizontalAutoCount());
	page->guides.setVerticalAutoCount(defaults.verticalAutoCount());
	page->guides.setHorizontalAutoGap(defaults.horizontalAutoGap());
	page->guides.setVerticalAutoGap(defaults.verticalAutoGap());
	page->guides.setHorizontalAutoRefer(defaults.horizontalAutoRefer());
	page->guides.setVerticalAutoRefer(defaults.verticalAutoRefer());
}

void GuideManager::clearPageGuidesButton_clicked()
{
	if (!m_doc || !currentPage)
		return;
	finishGuideEditors();
	if (!currentPage->guides.hasGuides())
		return;

	UndoTransaction trans;
	if (UndoManager::undoEnabled())
		trans = UndoManager::instance()->beginTransaction(currentPage->getUName(),
			currentPage->getUPixmap(),
			Um::RemoveAllPageGuides, "",
			Um::IGuides);
	clearPageGuides(currentPage);
	if (trans)
		trans.commit();

	// Show the cleared lists and the reset rows and columns
	setupGui();
	drawGuides();
	m_doc->changed();
}

void GuideManager::clearAllGuidesButton_clicked()
{
	if (!m_doc || !currentPage)
		return;
	finishGuideEditors();
	bool hasGuides = false;
	for (const ScPage* page : std::as_const(*m_doc->Pages))
	{
		if (page->guides.hasGuides())
		{
			hasGuides = true;
			break;
		}
	}
	if (!hasGuides)
		return;

	UndoTransaction trans;
	if (UndoManager::undoEnabled())
		trans = UndoManager::instance()->beginTransaction(m_doc->getUName(),
			m_doc->getUPixmap(),
			Um::RemoveAllGuides, "",
			Um::IGuides);
	for (ScPage* page : std::as_const(*m_doc->Pages))
		clearPageGuides(page);
	if (trans)
		trans.commit();

	setupGui();
	drawGuides();
	m_doc->changed();
}

void GuideManager::addHorizontalButton_clicked()
{
	if (m_verticalEditor)
		qobject_cast<GuidesDelegate*>(verticalView->itemDelegateForColumn(0))->finishEditing(m_verticalEditor);
	if (m_horizontalEditor)
	{
		qobject_cast<GuidesDelegate*>(horizontalView->itemDelegateForColumn(0))->finishEditing(m_horizontalEditor);
		horizontalView->setFocus(Qt::OtherFocusReason);
		updateClearButtons();
		return;
	}
	horizontalView->clearSelection();
	const int row = horizontalModel->insertRow();
	horizontalView->setCurrentIndex(horizontalModel->index(row, 0));
	horizontalView->edit(horizontalView->currentIndex());

	updateClearButtons();
}

void GuideManager::addVerticalButton_clicked()
{
	if (m_horizontalEditor)
		qobject_cast<GuidesDelegate*>(horizontalView->itemDelegateForColumn(0))->finishEditing(m_horizontalEditor);
	if (m_verticalEditor)
	{
		qobject_cast<GuidesDelegate*>(verticalView->itemDelegateForColumn(0))->finishEditing(m_verticalEditor);
		verticalView->setFocus(Qt::OtherFocusReason);
		updateClearButtons();
		return;
	}
	verticalView->clearSelection();
	const int row = verticalModel->insertRow();
	verticalView->setCurrentIndex(verticalModel->index(row, 0));
	verticalView->edit(verticalView->currentIndex());

	updateClearButtons();
}

void GuideManager::lockGuidesButton_clicked()
{
	m_doc->lockGuides(lockGuidesButton->isChecked());
}

void GuideManager::copyGuidesToAllPages(GuideManagerCore::GuideType t)
{
	QListIterator<ScPage*> it(*m_doc->Pages);
	ScPage *tmpPage;
	while (it.hasNext())
	{
		tmpPage = it.next();
		if (tmpPage->pageNr() == currentPage->pageNr())
			continue;
		tmpPage->guides.clearHorizontals(t);
		tmpPage->guides.clearVerticals(t);
		switch (t)
		{
			case GuideManagerCore::Standard:
				currentPage->guides.copy(&tmpPage->guides, t);
				break;
			case GuideManagerCore::Auto:
				storePageValues(tmpPage);
				break;
		}
	}
	drawGuides();
	m_doc->changed();
}

void GuideManager::applyToAllPagesButton_clicked()
{
	copyGuidesToAllPages(GuideManagerCore::Standard);
}

void GuideManager::applyAutoToAllPagesButton_clicked()
{
	copyGuidesToAllPages(GuideManagerCore::Auto);
}

void GuideManager::horizontalAutoCountSpin_valueChanged(double val)
{
	horizontalAutoGapSpin->setEnabled(val > 0);
	currentPage->guides.setHorizontalAutoCount(static_cast<int>(val));
	drawGuides();
	m_doc->changed();
}

void GuideManager::horizontalAutoGapSpin_valueChanged(double)
{
	currentPage->guides.setHorizontalAutoGap(horizontalAutoGapSpin->getValue(SC_PT));
	drawGuides();
	m_doc->changed();
}

void GuideManager::verticalAutoCountSpin_valueChanged(double val)
{
	verticalAutoGapSpin->setEnabled(val > 0);
	currentPage->guides.setVerticalAutoCount(static_cast<int>(val));
	drawGuides();
	m_doc->changed();
}

void GuideManager::verticalAutoGapSpin_valueChanged(double)
{
	currentPage->guides.setVerticalAutoGap(verticalAutoGapSpin->getValue(SC_PT));
	drawGuides();
	m_doc->changed();
}

Guides GuideManager::selectedHorizontals() const
{
	const QModelIndexList selectedIndexes = horizontalView->selectionModel()->selectedIndexes();
	Guides ret;
	Guides curr = horizontalModel->values();
	for (const QModelIndex& idx : selectedIndexes)
	{
		ret.append(curr.at(idx.row()));
	}
	return ret;
}

Guides GuideManager::selectedVerticals() const
{
	const QModelIndexList selectedIndexes = verticalView->selectionModel()->selectedIndexes();
	Guides ret;
	Guides curr = verticalModel->values();
	for (const QModelIndex& idx : selectedIndexes)
	{
		ret.append(curr.at(idx.row()));
	}
	return ret;
}

void GuideManager::setGuideLock(bool guidesLocked)
{
	QSignalBlocker blocker(lockGuidesButton);
	lockGuidesButton->setChecked(guidesLocked);
}

void GuideManager::forceDrawGuides(const QItemSelection &, const QItemSelection &)
{
	drawGuides();
}

void GuideManager::drawGuides()
{
	if (!m_doc || !currentPage || !m_drawGuides)
		return;

	currentPage->guides.addHorizontals(getAutoHorizontals(currentPage), GuideManagerCore::Auto);
	currentPage->guides.addVerticals(getAutoVerticals(currentPage), GuideManagerCore::Auto);
	ScCore->primaryMainWindow()->view->DrawNew();
}

void GuideManager::clearRestoreHorizontalList()
{
	horizontalModel->setValues(currentPage->guides.horizontals(GuideManagerCore::Standard));
}

void GuideManager::clearRestoreVerticalList()
{
	verticalModel->setValues(currentPage->guides.verticals(GuideManagerCore::Standard));
}

Guides GuideManager::getAutoVerticals(const ScPage * page) const
{
	GuideManagerCore guides;

	guides.setHorizontalAutoGap(horizontalAutoGapSpin->getValue(SC_PT));
	guides.setHorizontalAutoCount(static_cast<int>(horizontalAutoCountSpin->value()));
	guides.setHorizontalAutoRefer(horizontalReference());

	guides.setVerticalAutoGap(verticalAutoGapSpin->getValue(SC_PT));
	guides.setVerticalAutoCount(static_cast<int>(verticalAutoCountSpin->value()));
	guides.setVerticalAutoRefer(verticalReference());

	return guides.getAutoVerticals(page);
}

Guides GuideManager::getAutoHorizontals(const ScPage * page) const
{
	GuideManagerCore guides;

	guides.setHorizontalAutoGap(horizontalAutoGapSpin->getValue(SC_PT));
	guides.setHorizontalAutoCount(static_cast<int>(horizontalAutoCountSpin->value()));
	guides.setHorizontalAutoRefer(horizontalReference());

	guides.setVerticalAutoGap(verticalAutoGapSpin->getValue(SC_PT));
	guides.setVerticalAutoCount(static_cast<int>(verticalAutoCountSpin->value()));
	guides.setVerticalAutoRefer(verticalReference());

	return guides.getAutoHorizontals(page);
}

void GuideManager::addGuide(bool horizontal, double position)
{
	if (!currentPage)
		return;
	const Guides existing = horizontal ? currentPage->guides.horizontals(GuideManagerCore::Standard)
	                                   : currentPage->guides.verticals(GuideManagerCore::Standard);
	if (existing.contains(position))
		return;
	if (horizontal)
		currentPage->guides.addHorizontal(position, GuideManagerCore::Standard);
	else
		currentPage->guides.addVertical(position, GuideManagerCore::Standard);
	updateClearButtons();
	drawGuides();
	m_doc->changed();
}

void GuideManager::guideValueChanged(bool horizontal, double oldPosition, double newPosition)
{
	if (!currentPage || oldPosition == newPosition)
		return;
	const Guides existing = horizontal ? currentPage->guides.horizontals(GuideManagerCore::Standard)
	                                   : currentPage->guides.verticals(GuideManagerCore::Standard);
	if (!existing.contains(oldPosition))
		return;
	if (horizontal)
		currentPage->guides.moveHorizontal(oldPosition, newPosition, GuideManagerCore::Standard);
	else
		currentPage->guides.moveVertical(oldPosition, newPosition, GuideManagerCore::Standard);
	updateClearButtons();
	drawGuides();
	m_doc->changed();
}

void GuideManager::verticalModel_valueChanged(double oldPosition, double newPosition)
{
	guideValueChanged(false, oldPosition, newPosition);
}

void GuideManager::horizontalModel_valueChanged(double oldPosition, double newPosition)
{
	guideValueChanged(true, oldPosition, newPosition);
}

void GuideManager::setHorizontalReference(int button)
{
	switch (button)
	{
		case 0 :
			horizontalPageAutoButton->setChecked(true);
			break;
		case 1 :
			horizontalMarginsAutoButton->setChecked(true);
			break;
		case 2 :
			horizontalSelectionAutoButton->setChecked(true);
		default:
			break;
	}
}

int GuideManager::horizontalReference() const
{
	if (horizontalPageAutoButton->isChecked())
		return 0;
	if (horizontalMarginsAutoButton->isChecked())
		return 1;
	if (horizontalSelectionAutoButton->isChecked())
		return 2;
	return 0;
}

void GuideManager::setVerticalReference(int button)
{
	switch (button)
	{
		case 0 :
			verticalPageAutoButton->setChecked(true);
			break;
		case 1 :
			verticalMarginsAutoButton->setChecked(true);
			break;
		case 2 :
			verticalSelectionAutoButton->setChecked(true);
		default:
			break;
	}
}

int GuideManager::verticalReference() const
{
	if (verticalPageAutoButton->isChecked())
		return 0;
	if (verticalMarginsAutoButton->isChecked())
		return 1;
	if (verticalSelectionAutoButton->isChecked())
		return 2;
	return 0;
}

void GuideManager::horizontalPageAutoButton_toggled(bool state)
{
	if (!state)
		return;
	currentPage->guides.setHorizontalAutoRefer(0);
	drawGuides();
	if (currentPage->guides.horizontalAutoCount() > 0)
		m_doc->changed();
}

void GuideManager::horizontalMarginsAutoButton_toggled(bool state)
{
	if (!state)
		return;
	currentPage->guides.setHorizontalAutoRefer(1);
	drawGuides();
	if (currentPage->guides.horizontalAutoCount() > 0)
		m_doc->changed();
}

void GuideManager::horizontalSelectionAutoButton_toggled(bool state)
{
	if (!state)
		return;
	currentPage->guides.setHorizontalAutoRefer(2);
	if (horizontalSelectionAutoButton->isEnabled())
		currentPage->guides.resetSelectionForPage(currentPage);
	drawGuides();
	if (currentPage->guides.horizontalAutoCount() > 0)
		m_doc->changed();
}

void GuideManager::verticalPageAutoButton_toggled(bool state)
{
	if (!state)
		return;
	currentPage->guides.setVerticalAutoRefer(0);
	drawGuides();
	if (currentPage->guides.verticalAutoCount() > 0)
		m_doc->changed();
}

void GuideManager::verticalMarginsAutoButton_toggled(bool state)
{
	if (!state)
		return;
	currentPage->guides.setVerticalAutoRefer(1);
	drawGuides();
	if (currentPage->guides.verticalAutoCount() > 0)
		m_doc->changed();
}

void GuideManager::verticalSelectionAutoButton_toggled(bool state)
{
	if (!state)
		return;
	currentPage->guides.setVerticalAutoRefer(2);
	if (verticalSelectionAutoButton->isEnabled())
		currentPage->guides.resetSelectionForPage(currentPage);
	drawGuides();
	if (currentPage->guides.verticalAutoCount() > 0)
		m_doc->changed();
}
