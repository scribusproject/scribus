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

#include <QSignalBlocker>

#include "guidemanager.h"

#include "commonstrings.h"
#include "guidesdelegate.h"
#include "guidesmodel.h"
#include "scinputdialog.h"
#include "scpage.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "undomanager.h"
#include "units.h"
#include "util.h"


GuideManager::GuideManager(QWidget* parent) :
		ScrPaletteBase(parent, "GuideManager"),
		m_Doc(0),
		currentPage(0),
		m_drawGuides(true)
{
	docUnitIndex = 0;

	setupUi(this);
	tabWidget->setEnabled(false);
	horizontalAutoGapSpin->setMinimum(0.0);
	horizontalAutoGapSpin->setMaximum(10000.0);
	verticalAutoGapSpin->setMinimum(0.0);
	verticalAutoGapSpin->setMaximum(10000.0);
	horizontalAutoCountSpin->setDecimals(0);
	verticalAutoCountSpin->setDecimals(0);

	// MVC
	horizontalModel = new GuidesModel(this);
	verticalModel = new GuidesModel(this);
	GuidesHDelegate * hdelegate = new GuidesHDelegate();
	GuidesVDelegate * vdelegate = new GuidesVDelegate();
	horizontalView->setItemDelegateForColumn(0, hdelegate);
	verticalView->setItemDelegateForColumn(0, vdelegate);
	horizontalView->setModel(horizontalModel);
	verticalView->setModel(verticalModel);

	connect(addHorButton, SIGNAL(clicked()), this, SLOT(addHorButton_clicked()));
	connect(delHorButton, SIGNAL(clicked()), this, SLOT(delHorButton_clicked()));
	connect(horizontalView, SIGNAL(deleteKeyPressed()), this, SLOT(delHorButton_clicked()));
	connect(addVerButton, SIGNAL(clicked()), this, SLOT(addVerButton_clicked()));
	connect(delVerButton, SIGNAL(clicked()), this, SLOT(delVerButton_clicked()));
	connect(verticalView, SIGNAL(deleteKeyPressed()), this, SLOT(delVerButton_clicked()));
	connect(horizontalView->selectionModel(),
			 SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
			 this, SLOT(forceDrawGuides(const QItemSelection &, const QItemSelection &)));
	connect(verticalView->selectionModel(),
			 SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
			 this, SLOT(forceDrawGuides(const QItemSelection &, const QItemSelection &)));

	connect(applyToAllStdButton, SIGNAL(clicked()), this, SLOT(applyToAllStdButton_clicked()));
	connect(horizontalAutoCountSpin, SIGNAL(valueChanged(double)), this, SLOT(horizontalAutoCountSpin_valueChanged(double)));
	connect(verticalAutoCountSpin, SIGNAL(valueChanged(double)), this, SLOT(verticalAutoCountSpin_valueChanged(double)));
	connect(horizontalAutoGapCheck, SIGNAL(stateChanged(int)), this, SLOT(horizontalAutoGapCheck_stateChanged(int)));
	connect(verticalAutoGapCheck, SIGNAL(stateChanged(int)), this, SLOT(verticalAutoGapCheck_stateChanged(int)));
	connect(horizontalAutoGapSpin, SIGNAL(valueChanged(double)), this, SLOT(horizontalAutoGapSpin_valueChanged(double)));
	connect(verticalAutoGapSpin, SIGNAL(valueChanged(double)), this, SLOT(verticalAutoGapSpin_valueChanged(double)));

	connect(horizontalPageAutoButton, SIGNAL(toggled(bool)), this, SLOT(horizontalPageAutoButton_toggled(bool)));
	connect(horizontalMarginsAutoButton, SIGNAL(toggled(bool)), this, SLOT(horizontalMarginsAutoButton_toggled(bool)));
	connect(horizontalSelectionAutoButton, SIGNAL(toggled(bool)), this, SLOT(horizontalSelectionAutoButton_toggled(bool)));
	connect(verticalPageAutoButton, SIGNAL(toggled(bool)), this, SLOT(verticalPageAutoButton_toggled(bool)));
	connect(verticalMarginsAutoButton, SIGNAL(toggled(bool)), this, SLOT(verticalMarginsAutoButton_toggled(bool)));
	connect(verticalSelectionAutoButton, SIGNAL(toggled(bool)), this, SLOT(verticalSelectionAutoButton_toggled(bool)));

	connect(applyToAllAutoButton, SIGNAL(clicked()), this, SLOT(applyToAllAutoButton_clicked()));
	connect(deletePageButton, SIGNAL(clicked()), this, SLOT(deletePageButton_clicked()));
	connect(deleteAllGuides, SIGNAL(clicked()), this, SLOT(deleteAllGuides_clicked()));

	connect(lockCheck, SIGNAL(stateChanged(int)), this, SLOT(lockCheck_stateChanged(int)));

	connect(horizontalModel, SIGNAL(valueChanged()), this, SLOT(horizontalModel_valueChanged()));
	connect(verticalModel, SIGNAL(valueChanged()), this, SLOT(verticalModel_valueChanged()));
}

GuideManager::~GuideManager()
{
	delete horizontalView->itemDelegateForColumn(0);
	delete verticalView->itemDelegateForColumn(0);
}

void GuideManager::setDoc(ScribusDoc* doc)
{
	if (m_Doc && (m_Doc != doc))
	{
		if (currentPage && (currentPage->doc() == m_Doc))
			storePageValues(currentPage);
		currentPage = 0;
	}

	m_Doc = doc;
	if (m_Doc)
	{
		int oldUnitIndex = docUnitIndex;
		if (oldUnitIndex != docUnitIndex)
			unitChange();
	}

	qobject_cast<GuidesHDelegate*>(horizontalView->itemDelegateForColumn(0))->setDoc(doc);
	qobject_cast<GuidesVDelegate*>(verticalView->itemDelegateForColumn(0))->setDoc(doc);
	if (!m_Doc)
		currentPage = 0;
	tabWidget->setEnabled(doc ? true : false);
}

void GuideManager::setupPage(bool storeValues)
{
	if (!m_Doc)
		return;
	m_drawGuides = false;
	setEnabled(true);
	// store old values for current page (=page to leave)
	if (currentPage && storeValues)
		storePageValues(currentPage);

	currentPage = m_Doc->currentPage();
	unitChange();
	setupGui();
}

void GuideManager::setupGui()
{
	disconnect(horizontalAutoGapCheck, SIGNAL(stateChanged(int)),
			this, SLOT(horizontalAutoGapCheck_stateChanged(int)));
	disconnect(verticalAutoGapCheck, SIGNAL(stateChanged(int)),
			this, SLOT(verticalAutoGapCheck_stateChanged(int)));
	disconnect(lockCheck, SIGNAL(stateChanged(int)), 
			this, SLOT(lockCheck_stateChanged(int)));

	// restore values from new page
	clearRestoreHorizontalList();
	clearRestoreVerticalList();
	// restore: brand "auto guides into GUI restore algorithm"
	bool enable = currentPage->guides.horizontalAutoGap() > 0.0 ? true : false;
	horizontalAutoGapCheck->setChecked(enable);
	horizontalAutoGapSpin->setEnabled(enable);
	horizontalAutoGapSpin->setValue(pts2value(currentPage->guides.horizontalAutoGap(), docUnitIndex));
	horizontalAutoCountSpin->setValue(currentPage->guides.horizontalAutoCount());
	if (horizontalAutoCountSpin->value()==0)
	{
		horizontalAutoGapSpin->setEnabled(false);
		horizontalAutoGapCheck->setEnabled(false);
	}
	setHorizontalRefer(currentPage->guides.horizontalAutoRefer());
	// allow the selection radio button?
	horizontalSelectionAutoButton->setEnabled(!m_Doc->m_Selection->isEmpty());

	// verticals
	enable = currentPage->guides.verticalAutoGap() > 0.0 ? true : false;
	verticalAutoGapCheck->setChecked(enable);
	verticalAutoGapSpin->setEnabled(enable);
	verticalAutoGapSpin->setValue(pts2value(currentPage->guides.verticalAutoGap(), docUnitIndex));
	verticalAutoCountSpin->setValue(currentPage->guides.verticalAutoCount());
	if (verticalAutoCountSpin->value()==0)
	{
		verticalAutoGapSpin->setEnabled(false);
		verticalAutoGapCheck->setEnabled(false);
	}
	setVerticalRefer(currentPage->guides.verticalAutoRefer());
	// allow the selection radio button?
	verticalSelectionAutoButton->setEnabled(!m_Doc->m_Selection->isEmpty());

	// restore guide locking
	lockCheck->setChecked(m_Doc->GuideLock);

	connect(horizontalAutoGapCheck, SIGNAL(stateChanged(int)),
			this, SLOT(horizontalAutoGapCheck_stateChanged(int)));
	connect(verticalAutoGapCheck, SIGNAL(stateChanged(int)),
			this, SLOT(verticalAutoGapCheck_stateChanged(int)));
	connect(lockCheck, SIGNAL(stateChanged(int)), 
			this, SLOT(lockCheck_stateChanged(int)));

	m_drawGuides = true;
// 	drawGuides(); let Canvas::DrawPageMarks take care of it and avoid a costly ScribusView::DrawNew()
}

void GuideManager::storePageValues(ScPage *page)
{
	if (!page || !m_Doc)
		return;

	// Handle case where page to store values from does not belong
	// to current doc, eg. when switching between documents
	ScribusDoc* doc = page->doc();
	int unitIndex   = doc->unitIndex();

	double gapValue = 0.0;
	if (horizontalAutoGapCheck->isChecked())
		gapValue = value2pts(horizontalAutoGapSpin->value(), unitIndex);
	page->guides.setHorizontalAutoGap(gapValue);
	page->guides.setHorizontalAutoCount(horizontalAutoCountSpin->value());
	page->guides.setHorizontalAutoRefer(horizontalRefer());
	page->guides.addHorizontals(getAutoHorizontals(page), GuideManagerCore::Auto);

	gapValue = 0.0;
	if (verticalAutoGapCheck->isChecked())
		gapValue = value2pts(verticalAutoGapSpin->value(), unitIndex);
	page->guides.setVerticalAutoGap(gapValue);
	page->guides.setVerticalAutoCount(verticalAutoCountSpin->value());
	page->guides.setVerticalAutoRefer(verticalRefer());
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
}

void GuideManager::unitChange()
{
	if (!m_Doc)
		return;

	// a little bit magic to get Verticals (unit) into group boxes
	horizontalGroupBox->setTitle(horizontalGroupBox->title().remove(" ("+suffix.trimmed()+")"));
	verticalGroupBox->setTitle(verticalGroupBox->title().remove(" ("+suffix.trimmed()+")"));
	docUnitIndex = m_Doc->unitIndex();
	int docUnitDecimals = unitGetPrecisionFromIndex(docUnitIndex);

	QSignalBlocker horizontalAutoGapSpinBlock(horizontalAutoGapSpin);
	QSignalBlocker verticalAutoGapSpinBlock(verticalAutoGapSpin);
	suffix = unitGetSuffixFromIndex(docUnitIndex);
	horizontalAutoGapSpin->setNewUnit(docUnitIndex);
	verticalAutoGapSpin->setNewUnit(docUnitIndex);

	horizontalGroupBox->setTitle(horizontalGroupBox->title() + " ("+suffix.trimmed()+")");
	verticalGroupBox->setTitle(verticalGroupBox->title() + " ("+suffix.trimmed()+")");
	// models display
	horizontalModel->unitChange(docUnitIndex, docUnitDecimals, m_Doc->rulerYoffset);
	verticalModel->unitChange(docUnitIndex, docUnitDecimals, m_Doc->rulerXoffset);
	qobject_cast<GuidesHDelegate*>(horizontalView->itemDelegateForColumn(0))->setDoc(m_Doc);
	qobject_cast<GuidesVDelegate*>(verticalView->itemDelegateForColumn(0))->setDoc(m_Doc);
}

void GuideManager::delHorButton_clicked()
{
	QModelIndexList indexes = horizontalView->selectionModel()->selectedRows(0);
	QModelIndex ix;
	Guides v;

	foreach(ix, indexes)
		// here *must* go EditRole due truncations in DisplayRole
		// see GuidesModel::data()
		v.append(horizontalModel->data(ix, Qt::EditRole).toDouble());

	horizontalModel->removeValues(v);

	currentPage->guides.clearHorizontals(GuideManagerCore::Standard);
	currentPage->guides.addHorizontals(horizontalModel->values(), GuideManagerCore::Standard);
	drawGuides();
	m_Doc->changed();
}

void GuideManager::delVerButton_clicked()
{
	QModelIndexList indexes = verticalView->selectionModel()->selectedRows(0);
	QModelIndex ix;
	Guides v;

	foreach(ix, indexes)
		// here *must* go EditRole due truncations in DisplayRole
		// see GuidesModel::data()
		v.append(verticalModel->data(ix, Qt::EditRole).toDouble());

	verticalModel->removeValues(v);

	currentPage->guides.clearVerticals(GuideManagerCore::Standard);
	currentPage->guides.addVerticals(verticalModel->values(), GuideManagerCore::Standard);
	drawGuides();
	m_Doc->changed();
}

void GuideManager::addHorButton_clicked()
{
	horizontalModel->insertRow();
	horizontalView->setCurrentIndex(horizontalModel->index(0, 0, QModelIndex()));
	horizontalView->edit(horizontalView->currentIndex());
}

void GuideManager::addVerButton_clicked()
{
	verticalModel->insertRow();
	verticalView->setCurrentIndex(verticalModel->index(0, 0, QModelIndex()));
	verticalView->edit(verticalView->currentIndex());
}

void GuideManager::lockCheck_stateChanged( int )
{
	m_Doc->lockGuides(lockCheck->isChecked());
}

void GuideManager::copyGuidesToAllPages(GuideManagerCore::GuideType t)
{
	QListIterator<ScPage*> it(*m_Doc->Pages);
	ScPage *tmpPage;
	while (it.hasNext())
	{
		tmpPage=it.next();
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
	m_Doc->changed();
}

void GuideManager::applyToAllStdButton_clicked()
{
	copyGuidesToAllPages(GuideManagerCore::Standard);
}

void GuideManager::applyToAllAutoButton_clicked()
{
	copyGuidesToAllPages(GuideManagerCore::Auto);
}

void GuideManager::horizontalAutoCountSpin_valueChanged(double val)
{
	bool enable = (val == 0) ? false : true;
	horizontalAutoGapCheck->setEnabled(enable);
	if (enable && horizontalAutoGapCheck->isChecked())
		horizontalAutoGapSpin->setEnabled(true);
	else
		horizontalAutoGapSpin->setEnabled(false);
	currentPage->guides.setHorizontalAutoCount(val);
	drawGuides();
	m_Doc->changed();
}

void GuideManager::horizontalAutoGapSpin_valueChanged(double)
{
	currentPage->guides.setHorizontalAutoGap(value2pts(horizontalAutoGapSpin->value(), docUnitIndex));
	drawGuides();
	m_Doc->changed();
}

void GuideManager::horizontalAutoGapCheck_stateChanged( int )
{
	horizontalAutoGapSpin->setEnabled(horizontalAutoGapCheck->isChecked());
	if (horizontalAutoGapCheck->isChecked())
		currentPage->guides.setHorizontalAutoGap(value2pts(horizontalAutoGapSpin->value(), docUnitIndex));
	else
		currentPage->guides.setHorizontalAutoGap(0.0);
	drawGuides();
	m_Doc->changed();
}

void GuideManager::verticalAutoCountSpin_valueChanged(double val)
{
	bool enable = (val == 0) ? false : true;
	verticalAutoGapCheck->setEnabled(enable);
	if (enable && verticalAutoGapCheck->isChecked())
		verticalAutoGapSpin->setEnabled(true);
	else
		verticalAutoGapSpin->setEnabled(false);
	currentPage->guides.setVerticalAutoCount(val);
	drawGuides();
	m_Doc->changed();
}

void GuideManager::verticalAutoGapSpin_valueChanged(double)
{
	currentPage->guides.setVerticalAutoGap(value2pts(verticalAutoGapSpin->value(), docUnitIndex));
	drawGuides();
	m_Doc->changed();
}

void GuideManager::verticalAutoGapCheck_stateChanged( int )
{
	verticalAutoGapSpin->setEnabled(verticalAutoGapCheck->isChecked());
	if (verticalAutoGapCheck->isChecked())
		currentPage->guides.setVerticalAutoGap(value2pts(verticalAutoGapSpin->value(), docUnitIndex));
	else
		currentPage->guides.setVerticalAutoGap(0.0);
	drawGuides();
	m_Doc->changed();
}

void GuideManager::tabWidget_currentChanged(QWidget *)
{
	drawGuides();
	if (tabWidget->currentIndex() == 1)
	{
		horizontalAutoGapSpin->setEnabled(horizontalAutoGapCheck->isChecked());
		verticalAutoGapSpin->setEnabled(verticalAutoGapCheck->isChecked());
	}
}

Guides GuideManager::selectedHorizontals()
{
	QModelIndex i;
	QModelIndexList selectedIndexes = horizontalView->selectionModel()->selectedIndexes();
	Guides ret;
	Guides curr = horizontalModel->values();
	foreach (i , selectedIndexes)
	{
		ret.append(curr.at(i.row()));
	}
	return ret;
}

Guides GuideManager::selectedVerticals()
{
	QModelIndex i;
	QModelIndexList selectedIndexes = verticalView->selectionModel()->selectedIndexes();
	Guides ret;
	Guides curr = verticalModel->values();
	foreach (i , selectedIndexes)
	{
		ret.append(curr.at(i.row()));
	}
	return ret;
}

void GuideManager::setGuideLock(bool guidesLocked)
{
	bool b=lockCheck->blockSignals(true);
	lockCheck->setChecked(guidesLocked);
	lockCheck->blockSignals(b);
}

void GuideManager::forceDrawGuides(const QItemSelection &, const QItemSelection &)
{
	drawGuides();
}

void GuideManager::drawGuides()
{
	if (!m_Doc || !m_drawGuides)
		return;

	currentPage->guides.addHorizontals(getAutoHorizontals(currentPage), GuideManagerCore::Auto);
	currentPage->guides.addVerticals(getAutoVerticals(currentPage), GuideManagerCore::Auto);
	ScCore->primaryMainWindow()->view->DrawNew();
}

void GuideManager::clearRestoreHorizontalList()
{
	horizontalModel->setValues(currentPage->guides.horizontals(GuideManagerCore::Standard));
	drawGuides();
}

void GuideManager::clearRestoreVerticalList()
{
	verticalModel->setValues(currentPage->guides.verticals(GuideManagerCore::Standard));
	drawGuides();
}

void GuideManager::deletePageButton_clicked()
{
	UndoTransaction trans;
	if(UndoManager::undoEnabled())
		trans = UndoManager::instance()->beginTransaction(currentPage->getUName(),
														currentPage->getUPixmap(),
														Um::RemoveAllPageGuides, "",
														Um::IGuides);
	currentPage->guides.clearHorizontals(GuideManagerCore::Standard);
	currentPage->guides.clearVerticals(GuideManagerCore::Standard);
	currentPage->guides.clearHorizontals(GuideManagerCore::Auto);
	currentPage->guides.clearVerticals(GuideManagerCore::Auto);
	clearRestoreHorizontalList();
	clearRestoreVerticalList();

	currentPage->guides.setHorizontalAutoCount(0);
	currentPage->guides.setVerticalAutoCount(0);
	currentPage->guides.setHorizontalAutoGap(0.0);
	currentPage->guides.setVerticalAutoGap(0.0);
	currentPage->guides.setHorizontalAutoRefer(0);
	currentPage->guides.setVerticalAutoRefer(0);
	horizontalAutoCountSpin->setValue(0);
	verticalAutoCountSpin->setValue(0);
	
	if (trans)
		trans.commit();

	drawGuides();
	m_Doc->changed();
}

void GuideManager::deleteAllGuides_clicked()
{
	UndoTransaction trans;
	if (UndoManager::undoEnabled())
		trans = UndoManager::instance()->beginTransaction(m_Doc->getUName(),
														m_Doc->getUPixmap(),
														Um::RemoveAllGuides, "",
														Um::IGuides);
	m_drawGuides = false;
	deletePageButton_clicked();
	copyGuidesToAllPages(GuideManagerCore::Standard);
	copyGuidesToAllPages(GuideManagerCore::Auto);
	m_drawGuides = true;
	if (trans)
		trans.commit();
	drawGuides();
	m_Doc->changed();
}

Guides GuideManager::getAutoVerticals(ScPage * page)
{
	GuideManagerCore guides;

	// Handle case where page to store values from does not belong
	// to current doc, eg. when switching between documents
	ScribusDoc* doc = page->doc();
	int unitIndex = doc->unitIndex();

	double gapValue = 0.0;
	if (horizontalAutoGapCheck->isChecked())
		gapValue = value2pts(horizontalAutoGapSpin->value(), unitIndex);
	guides.setHorizontalAutoGap(gapValue);
	guides.setHorizontalAutoCount(horizontalAutoCountSpin->value());
	guides.setHorizontalAutoRefer(horizontalRefer());

	gapValue = 0.0;
	if (verticalAutoGapCheck->isChecked())
		gapValue = value2pts(verticalAutoGapSpin->value(), unitIndex);
	guides.setVerticalAutoGap(gapValue);
	guides.setVerticalAutoCount(verticalAutoCountSpin->value());
	guides.setVerticalAutoRefer(verticalRefer());

	return guides.getAutoVerticals(page);
}

Guides GuideManager::getAutoHorizontals(ScPage * page)
{
	GuideManagerCore guides;

	// Handle case where page to store values from does not belong
	// to current doc, eg. when switching between documents
	ScribusDoc* doc = page->doc();
	int unitIndex = doc->unitIndex();

	double gapValue = 0.0;
	if (horizontalAutoGapCheck->isChecked())
		gapValue = value2pts(horizontalAutoGapSpin->value(), unitIndex);
	guides.setHorizontalAutoGap(gapValue);
	guides.setHorizontalAutoCount(horizontalAutoCountSpin->value());
	guides.setHorizontalAutoRefer(horizontalRefer());

	gapValue = 0.0;
	if (verticalAutoGapCheck->isChecked())
		gapValue = value2pts(verticalAutoGapSpin->value(), unitIndex);
	guides.setVerticalAutoGap(gapValue);
	guides.setVerticalAutoCount(verticalAutoCountSpin->value());
	guides.setVerticalAutoRefer(verticalRefer());

	return guides.getAutoHorizontals(page);
}

void GuideManager::resetSelectionForPage()
{
	int docSelectionCount = currentPage->doc()->m_Selection->count();

	currentPage->guides.gx = currentPage->guides.gy = currentPage->guides.gw = currentPage->guides.gh = 0.0;

	// multiselection
	if (docSelectionCount > 1)
	{
		double x, y;
		m_Doc->m_Selection->getGroupRect(&x, &y, &currentPage->guides.gw, &currentPage->guides.gh);
		currentPage->guides.gx = x - currentPage->xOffset();
		currentPage->guides.gy = y - currentPage->yOffset();
	}
	// only one item selected
	else if (docSelectionCount == 1)
	{
		PageItem *currItem = m_Doc->m_Selection->itemAt(0);
		currentPage->guides.gx = currItem->xPos() - currentPage->xOffset();
		currentPage->guides.gy = currItem->yPos() - currentPage->yOffset();
		currentPage->guides.gw = currItem->width();
		currentPage->guides.gh = currItem->height();
	}
}

void GuideManager::verticalModel_valueChanged()
{
	currentPage->guides.clearVerticals(GuideManagerCore::Standard);
	currentPage->guides.addVerticals(verticalModel->values(), GuideManagerCore::Standard);
	drawGuides();
	m_Doc->changed();
}

void GuideManager::horizontalModel_valueChanged()
{
	currentPage->guides.clearHorizontals(GuideManagerCore::Standard);
	currentPage->guides.addHorizontals(horizontalModel->values(), GuideManagerCore::Standard);
	drawGuides();
	m_Doc->changed();
}

void GuideManager::setHorizontalRefer(int button)
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
	}
}

int GuideManager::horizontalRefer()
{
	if (horizontalPageAutoButton->isChecked())
		return 0;
	if (horizontalMarginsAutoButton->isChecked())
		return 1;
	if (horizontalSelectionAutoButton->isChecked())
		return 2;
	return 0;
}

void GuideManager::setVerticalRefer(int button)
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
	}
}

int GuideManager::verticalRefer()
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
}

void GuideManager::horizontalMarginsAutoButton_toggled(bool state)
{
	if (!state)
		return;
	currentPage->guides.setHorizontalAutoRefer(1);
	drawGuides();
}

void GuideManager::horizontalSelectionAutoButton_toggled(bool state)
{
	if (!state)
		return;
	currentPage->guides.setHorizontalAutoRefer(2);
	if (horizontalSelectionAutoButton->isEnabled())
		resetSelectionForPage();
	drawGuides();
}

void GuideManager::verticalPageAutoButton_toggled(bool state)
{
	if (!state)
		return;
	currentPage->guides.setVerticalAutoRefer(0);
	drawGuides();
	m_Doc->changed();
}

void GuideManager::verticalMarginsAutoButton_toggled(bool state)
{
	if (!state)
		return;
	currentPage->guides.setVerticalAutoRefer(1);
	drawGuides();
	m_Doc->changed();
}

void GuideManager::verticalSelectionAutoButton_toggled(bool state)
{
	if (!state)
		return;
	currentPage->guides.setVerticalAutoRefer(2);
	if (verticalSelectionAutoButton->isEnabled())
		resetSelectionForPage();
	drawGuides();
	m_Doc->changed();
}
