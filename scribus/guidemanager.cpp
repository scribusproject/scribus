/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                              guidemanager.cpp
                             -------------------
    begin                : Fri Jan 30 2004
    copyright            : (C) 2004 by Alessandro Rimoldi
    copyright            : (C) by Franz Schmid
    e-mail               : http://www.ideale.ch/contact
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qradiobutton.h>
#include <qlayout.h>
#include <qcursor.h>
#include <q3listview.h>
#include <qdialog.h>
#include <q3groupbox.h>
#include <qlabel.h>
#include <q3listview.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qtooltip.h>
#include <qbuttongroup.h>
#include <qtabwidget.h>
#include <QList>

#include "guidemanager.h"
//#include "guidemanager.moc"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "page.h"
#include "units.h"
#include "util.h"
#include "commonstrings.h"
#include "scinputdialog.h"
#include "selection.h"
#include "undomanager.h"


int GuideListItem::compare(Q3ListViewItem *i, int col, bool asc) const
{
	if (col == 0)
	{
		double d;
		d = text(col).toDouble() - i->text(col).toDouble();
		if (d > 0.0)
			return 1;
		return -1;
	}
	else
		return Q3ListViewItem::compare(i, col, asc);
}


GuideManager::GuideManager(QWidget* parent) :
		ScrPaletteBase(parent, "GuideManager"),
		m_Doc(0),
		currentPage(0),
		m_drawGuides(true)
{
	setupUi(this);
	tabWidget->setEnabled(false);
	horizontalAutoGapSpin->setMinimum(0.0);
	horizontalAutoGapSpin->setMaximum(100.0);
	verticalAutoGapSpin->setMinimum(0.0);
	verticalAutoGapSpin->setMaximum(100.0);

	// signals that cannot be defined in designer (mspinbox related)
	connect(horizontalAutoGapSpin, SIGNAL(valueChanged(double)), this, SLOT(horizontalAutoGapSpin_valueChanged(double)));
	connect(verticalAutoGapSpin, SIGNAL(valueChanged(double)), this, SLOT(verticalAutoGapSpin_valueChanged(double)));
}

void GuideManager::setDoc(ScribusDoc* doc)
{
	m_Doc=doc;
	if (!m_Doc)
		currentPage = 0;
	tabWidget->setEnabled(doc ? true : false);
}

void GuideManager::setupPage()
{
	if (!m_Doc)
		return;
	m_drawGuides = false;
	setEnabled(true);
	// store old values for current page (=page to leave)
	storePageValues(currentPage);
	currentPage = m_Doc->currentPage();
	unitChange();
	setupGui();
}

void GuideManager::setupGui()
{
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
	horizontalReferGroup->setButton(currentPage->guides.horizontalAutoRefer());
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
	verticalReferGroup->setButton(currentPage->guides.verticalAutoRefer());
	// allow the selection radio button?
	verticalSelectionAutoButton->setEnabled(!m_Doc->m_Selection->isEmpty());

	m_drawGuides = true;
	drawGuides();
}

void GuideManager::storePageValues(Page *page)
{
	if (!page || !m_Doc)
		return;

	double gapValue = 0.0;
	if (horizontalAutoGapCheck->isChecked())
		gapValue = value2pts(horizontalAutoGapSpin->value(), docUnitIndex);
	page->guides.setHorizontalAutoGap(gapValue);
	page->guides.setHorizontalAutoCount(horizontalAutoCountSpin->value());
	page->guides.setHorizontalAutoRefer(horizontalReferGroup->selectedId());
	page->guides.addHorizontals(getAutoHorizontals(), GuideManagerCore::Auto);

	gapValue = 0.0;
	if (verticalAutoGapCheck->isChecked())
		gapValue = value2pts(verticalAutoGapSpin->value(), docUnitIndex);
	page->guides.setVerticalAutoGap(gapValue);
	page->guides.setVerticalAutoCount(verticalAutoCountSpin->value());
	page->guides.setVerticalAutoRefer(verticalReferGroup->selectedId());
	page->guides.addVerticals(getAutoVerticals(), GuideManagerCore::Auto);
}

void GuideManager::unitChange()
{
	if (!m_Doc)
		return;
	// a little bit magic to get Verticals (unit) into group boxes
	horizontalGroupBox->setTitle(horizontalGroupBox->title().remove(" ("+suffix.stripWhiteSpace()+")"));
	verticalGroupBox->setTitle(verticalGroupBox->title().remove(" ("+suffix.stripWhiteSpace()+")"));
	docUnitIndex = m_Doc->unitIndex();
	docUnitPrecision = unitGetPrecisionFromIndex(docUnitIndex);
	docUnitRatio = unitGetRatioFromIndex(docUnitIndex);
	docUnitDecimals = unitGetPrecisionFromIndex(docUnitIndex);
	
	suffix = unitGetSuffixFromIndex(docUnitIndex);
	horizontalAutoGapSpin->setSuffix(suffix);
	verticalAutoGapSpin->setSuffix(suffix);
	horizontalAutoGapSpin->setDecimals(docUnitDecimals);
	verticalAutoGapSpin->setDecimals(docUnitDecimals);
	horizontalGroupBox->setTitle(horizontalGroupBox->title() + " ("+suffix.stripWhiteSpace()+")");
	verticalGroupBox->setTitle(verticalGroupBox->title() + " ("+suffix.stripWhiteSpace()+")");
}

bool GuideManager::deleteValueFormList(Q3ListView *list)
{
	/* previous item pointer to ensure that ++it
	runs before item goes deleted */
	Q3ListViewItem *itemToDelete;
	Q3ListViewItemIterator it(list, Q3ListViewItemIterator::Selected);
	QString value;
	while (it.current())
	{
		itemToDelete = it.current();
		value = itemToDelete->text(0);
		if (list == horizontalList)
			currentPage->guides.deleteHorizontal(m_horMap[value], GuideManagerCore::Standard);
		else
			currentPage->guides.deleteVertical(m_verMap[value], GuideManagerCore::Standard);
		++it;
//		if (itemToDelete)  coverity complains since itemToDelete was dereferenced before
		{
			list->takeItem(itemToDelete);
			delete itemToDelete;
		}
	}
	drawGuides();
	return true;
}

void GuideManager::delHorButton_clicked()
{
	deleteValueFormList(horizontalList);
}

void GuideManager::delVerButton_clicked()
{
	deleteValueFormList(verticalList);
}

bool GuideManager::editValueToList(Q3ListView *list)
{
	bool ok;
	QString original = list->currentItem()->text(0);
	double newGuide = ScInputDialog::getDouble( tr("Edit Guide"),
											  tr("Enter a position:"),
											  original.toDouble(),
											  0, 1000, docUnitDecimals, suffix,
											  &ok, this);
	if (!ok)
		return false;

	if (list == horizontalList)
	{
		currentPage->guides.moveHorizontal(m_horMap[original], value2pts(newGuide, docUnitIndex),
										   GuideManagerCore::Standard);
		clearRestoreHorizontalList();
	}
	else
	{
		currentPage->guides.moveVertical(m_verMap[original], value2pts(newGuide, docUnitIndex),
										 GuideManagerCore::Standard);
		clearRestoreVerticalList();
	}
	drawGuides();
	return true;
}

bool GuideManager::addValueToList(Q3ListView *list)
{
	bool ok;
	double newGuide = ScInputDialog::getDouble( tr("New Guide"),
										 tr("Enter a position:"),
										 0.0, 0, 1000, docUnitDecimals, suffix,
										 &ok, this );
	if (!ok)
		return false;

	QString tmp;
	tmp = tmp.setNum(newGuide, 'f', docUnitPrecision);
	double ng = value2pts(newGuide, docUnitIndex);
	if (list == horizontalList)
	{
		Guides tmpGuides = currentPage->guides.horizontals(GuideManagerCore::Standard);
		if (tmpGuides.contains(ng))
			return false;
		currentPage->guides.addHorizontal(ng, GuideManagerCore::Standard);
		m_horMap[tmp] = ng;
	}
	else
	{
		Guides tmpGuides = currentPage->guides.verticals(GuideManagerCore::Standard);
		if (tmpGuides.contains(ng))
			return false;
		currentPage->guides.addVertical(ng, GuideManagerCore::Standard);
		m_verMap[tmp] = ng;
	}

	GuideListItem *item = new GuideListItem(list, tmp);
	item->setRenameEnabled(0, true);
	list->insertItem(item);
	list->setCurrentItem(item);
	list->clearSelection();
	list->setSelected(item, true);
	drawGuides();
	return true;
}

void GuideManager::addHorButton_clicked()
{
	addValueToList(horizontalList);
}

void GuideManager::addVerButton_clicked()
{
	addValueToList(verticalList);
}

void GuideManager::setGuidesFromList(Q3ListView *w, GuideGUIMap *map, Guides guides)
{
	QString tmp;
	//w->clear(); // clearing is moved into the setupPage()
	for (Guides::iterator it = guides.begin(); it != guides.end(); ++it)
	{
		tmp = tmp.setNum((*it) * docUnitRatio , 'f', docUnitPrecision);
		// no insert for duplicates
		if (w->findItem(tmp, 0) != 0)
			continue;
		GuideListItem *item = new GuideListItem(w, tmp);
		w->insertItem(item);
		map->insert(tmp, (*it));
	}
}

void GuideManager::lockCheck_stateChanged( int )
{
	m_Doc->lockGuides(lockCheck->isChecked());
}

void GuideManager::copyGuidesToAllPages(GuideManagerCore::GuideType t)
{
	QListIterator<Page*> it(*m_Doc->Pages);
	Page *tmpPage;
	while (it.hasNext())
	{
		tmpPage=it.next();
		if (tmpPage->pageNr() == currentPage->pageNr())
			continue;
		tmpPage->guides.clearHorizontals(t);
		tmpPage->guides.clearVerticals(t);
		currentPage->guides.copy(&tmpPage->guides, t);
		if (t == GuideManagerCore::Auto)
			storePageValues(tmpPage);
	}
	drawGuides();
}

void GuideManager::applyToAllStdButton_clicked()
{
	copyGuidesToAllPages(GuideManagerCore::Standard);
}

void GuideManager::applyToAllAutoButton_clicked()
{
	copyGuidesToAllPages(GuideManagerCore::Auto);
}

void GuideManager::horizontalAutoCountSpin_valueChanged(int val)
{
	bool enable = (val == 0) ? false : true;
	horizontalAutoGapCheck->setEnabled(enable);
	if (enable && horizontalAutoGapCheck->isChecked())
		horizontalAutoGapSpin->setEnabled(true);
	else
		horizontalAutoGapSpin->setEnabled(false);
	currentPage->guides.setHorizontalAutoCount(val);
	drawGuides();
}

void GuideManager::horizontalAutoGapSpin_valueChanged(double)
{
	currentPage->guides.setHorizontalAutoGap(value2pts(horizontalAutoGapSpin->value(), docUnitIndex));
	drawGuides();
}

void GuideManager::horizontalAutoGapCheck_stateChanged( int )
{
	horizontalAutoGapSpin->setEnabled(horizontalAutoGapCheck->isChecked());
	if (horizontalAutoGapCheck->isChecked())
		currentPage->guides.setHorizontalAutoGap(value2pts(horizontalAutoGapSpin->value(), docUnitIndex));
	else
		currentPage->guides.setHorizontalAutoGap(value2pts(0.0, docUnitIndex));
	drawGuides();
}

void GuideManager::verticalAutoCountSpin_valueChanged(int val)
{
	bool enable = (val == 0) ? false : true;
	verticalAutoGapCheck->setEnabled(enable);
	if (enable && verticalAutoGapCheck->isChecked())
		verticalAutoGapSpin->setEnabled(true);
	else
		verticalAutoGapSpin->setEnabled(false);
	currentPage->guides.setVerticalAutoCount(val);
	drawGuides();
}

void GuideManager::verticalAutoGapSpin_valueChanged(double)
{
	currentPage->guides.setVerticalAutoGap(value2pts(verticalAutoGapSpin->value(), docUnitIndex));
	drawGuides();
}

void GuideManager::verticalAutoGapCheck_stateChanged( int )
{
	verticalAutoGapSpin->setEnabled(verticalAutoGapCheck->isChecked());
	if (verticalAutoGapCheck->isChecked())
		currentPage->guides.setVerticalAutoGap(value2pts(verticalAutoGapSpin->value(), docUnitIndex));
	else
		currentPage->guides.setVerticalAutoGap(value2pts(0.0, docUnitIndex));
	drawGuides();
}

void GuideManager::horizontalReferGroup_clicked(int val)
{
	currentPage->guides.setHorizontalAutoRefer(val);
	if (val == 2 && horizontalSelectionAutoButton->isEnabled())
		resetSelectionForPage();
	drawGuides();
}

void GuideManager::verticalReferGroup_clicked(int val)
{
	currentPage->guides.setVerticalAutoRefer(val);
	if (val == 2 && verticalSelectionAutoButton->isEnabled())
		resetSelectionForPage();
	drawGuides();
}

void GuideManager::tabWidget_currentChanged(QWidget *)
{
	drawGuides();
	if (tabWidget->currentPageIndex() == 1)
	{
		horizontalAutoGapSpin->setEnabled(horizontalAutoGapCheck->isChecked());
		verticalAutoGapSpin->setEnabled(verticalAutoGapCheck->isChecked());
	}
}

void GuideManager::horizontalList_doubleClicked( Q3ListViewItem * )
{
	editValueToList(horizontalList);
}

void GuideManager::horizontalList_returnPressed( Q3ListViewItem * )
{
	editValueToList(horizontalList);
}

void GuideManager::verticalList_returnPressed( Q3ListViewItem * )
{
	editValueToList(verticalList);
}

void GuideManager::verticalList_doubleClicked( Q3ListViewItem * )
{
	editValueToList(verticalList);
}

Guides GuideManager::selectedHorizontals()
{
	Guides retval;
	Q3ListViewItemIterator it(horizontalList);
	while (it.current())
	{
		if (it.current()->isSelected())
			retval.append(it.current()->text(0).toDouble() / docUnitRatio);
		++it;
	}
	return retval;
}

Guides GuideManager::selectedVerticals()
{
	Guides retval;
	Q3ListViewItemIterator it(verticalList);
	while (it.current())
	{
		if (it.current()->isSelected())
			retval.append(it.current()->text(0).toDouble() / docUnitRatio);
		++it;
	}
	return retval;
}

void GuideManager::horizontalList_selectionChanged()
{
	drawGuides();
}

void GuideManager::verticalList_selectionChanged()
{
	drawGuides();
}

void GuideManager::drawGuides()
{
	if (!m_Doc || !m_drawGuides)
		return;

	currentPage->guides.addHorizontals(getAutoHorizontals(), GuideManagerCore::Auto);
	currentPage->guides.addVerticals(getAutoVerticals(), GuideManagerCore::Auto);
	ScCore->primaryMainWindow()->view->DrawNew();
}

void GuideManager::clearRestoreHorizontalList()
{
	m_horMap.clear();
	horizontalList->clear();
	setGuidesFromList(horizontalList, &m_horMap,
					  currentPage->guides.horizontals(GuideManagerCore::Standard));
}

void GuideManager::clearRestoreVerticalList()
{
	m_verMap.clear();
	verticalList->clear();
	setGuidesFromList(verticalList, &m_verMap,
					  currentPage->guides.verticals(GuideManagerCore::Standard));
}

void GuideManager::deletePageButton_clicked()
{
	UndoManager::instance()->beginTransaction(currentPage->getUName(),
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
	UndoManager::instance()->commit();

	drawGuides();
}

void GuideManager::deleteAllGuides_clicked()
{
	UndoManager::instance()->beginTransaction(m_Doc->getUName(),
	                              m_Doc->getUPixmap(),
	                              Um::RemoveAllGuides, "",
	                              Um::IGuides);
	m_drawGuides = false;
	deletePageButton_clicked();
	copyGuidesToAllPages(GuideManagerCore::Standard);
	copyGuidesToAllPages(GuideManagerCore::Auto);
	m_drawGuides = true;
	UndoManager::instance()->commit();
	drawGuides();
}

void GuideManager::windowActivationChange(bool oldActive)
{
	if (m_Doc)
	{
		bool enable = !m_Doc->m_Selection->isEmpty();
		horizontalSelectionAutoButton->setEnabled(enable);
		verticalSelectionAutoButton->setEnabled(enable);
	}
	QDialog::windowActivationChange( oldActive );
}

Guides GuideManager::getAutoVerticals()
{
	Guides retval;
	double columnSize;
	int value = verticalAutoCountSpin->value();
	double offset = 0.0;
	double newPageWidth = currentPage->width();

	if (value == 0)
		return retval;
	++value;

	if (verticalReferGroup->selectedId() == 1)
	{
		newPageWidth = newPageWidth - currentPage->Margins.Left - currentPage->Margins.Right;
		offset = currentPage->Margins.Left;
	}
	else if (qRound(currentPage->guides.gx) != 0)
	{
		offset = currentPage->guides.gx;
		newPageWidth = currentPage->guides.gw;
	}

	if (verticalAutoGapSpin->value() > 0.0)
		columnSize = (newPageWidth - (value - 1) * verticalAutoGapSpin->value()) / value;
	else
		columnSize = newPageWidth / value;

	for (int i = 1, gapCount = 0; i < value; ++i)
	{
		if (verticalAutoGapSpin->value() > 0.0)
		{
			retval.append(offset + i * columnSize + gapCount * verticalAutoGapSpin->value());
			++gapCount;
			retval.append(offset + i * columnSize + gapCount * verticalAutoGapSpin->value());
		}
		else
			retval.append(offset + columnSize * i);
	}
	return retval;
}

Guides GuideManager::getAutoHorizontals()
{
	Guides retval;
	double rowSize;
	int value = horizontalAutoCountSpin->value();
	double offset = 0.0;
	double newPageHeight = currentPage->height();

	if (value == 0)
		return retval;
	++value;

	if (horizontalReferGroup->selectedId() == 1)
	{
		newPageHeight = newPageHeight - currentPage->Margins.Top - currentPage->Margins.Bottom;
		offset = currentPage->Margins.Top;
	}
	else if (qRound(currentPage->guides.gy) != 0.0)
	{
		offset = currentPage->guides.gy;
		newPageHeight = currentPage->guides.gh;
	}

	if (horizontalAutoGapSpin->value() > 0.0)
		rowSize = (newPageHeight - (value - 1) * horizontalAutoGapSpin->value()) / value;
	else
		rowSize = newPageHeight / value;

	for (int i = 1, gapCount = 0; i < value; ++i)
	{
		if (horizontalAutoGapSpin->value() > 0.0)
		{
			retval.append(offset + i * rowSize + gapCount * horizontalAutoGapSpin->value());
			++gapCount;
			retval.append(offset + i * rowSize + gapCount * horizontalAutoGapSpin->value());
		}
		else
			retval.append(offset + rowSize * i);
	}
	return retval;
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
