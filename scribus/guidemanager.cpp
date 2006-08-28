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
#include <qlistview.h>
#include <qdialog.h>
#include <qvaluelist.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qtooltip.h>
#include <qhbuttongroup.h>
#include <qtabwidget.h>

#include "guidemanager.h"
#include "guidemanager.moc"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "page.h"
#include "units.h"
#include "util.h"
#include "commonstrings.h"
#include "scinputdialog.h"
#include "selection.h"


int GuideListItem::compare(QListViewItem *i, int col, bool asc) const
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
		return QListViewItem::compare(i, col, asc);
}


GuideManager::GuideManager(QWidget* parent) :
		GuideManagerBase(parent, "GuideManager"),
		m_Doc(0),
		currentPage(0)
		//ScrPaletteBase(parent, "GuideManager", false, 0)
{
	m_drawGuides = false;
	tabWidget->setEnabled(false);
	setIcon(loadIcon("AppIcon.png"));
	horizontalAutoGapSpin->setMinValue(0.0);
	horizontalAutoGapSpin->setMaxValue(100.0);
	verticalAutoGapSpin->setMinValue(0.0);
	verticalAutoGapSpin->setMaxValue(100.0);
	

	// signals that cannot be defined in designer (mspinbox related)
	connect(horizontalAutoGapSpin, SIGNAL(valueChanged(int)), this, SLOT(horizontalAutoGapSpin_valueChanged(int)));
	connect(verticalAutoGapSpin, SIGNAL(valueChanged(int)), this, SLOT(verticalAutoGapSpin_valueChanged(int)));
	m_drawGuides = true;
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
	bGroup->setButton(currentPage->guides.autoRefer());
	// allow the selection radio button?
	int docSelectionCount = m_Doc->m_Selection->count();
	selectionAutoButton->setEnabled((docSelectionCount <= 0));
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
	page->guides.setHorizontalAutoGap(value2pts(gapValue, docUnitIndex));
	page->guides.setHorizontalAutoCount(horizontalAutoCountSpin->value());

	gapValue = 0.0;
	if (verticalAutoGapCheck->isChecked())
		gapValue = value2pts(verticalAutoGapSpin->value(), docUnitIndex);
	page->guides.setVerticalAutoGap(value2pts(gapValue, docUnitIndex));
	page->guides.setVerticalAutoCount(verticalAutoCountSpin->value());

	page->guides.setAutoRefer(bGroup->selectedId());
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
	docUnitDecimals = unitGetDecimalsFromIndex(docUnitIndex);
	
	suffix = unitGetSuffixFromIndex(docUnitIndex);
	horizontalAutoGapSpin->setSuffix(suffix);
	verticalAutoGapSpin->setSuffix(suffix);
	horizontalAutoGapSpin->setDecimals(docUnitDecimals);
	verticalAutoGapSpin->setDecimals(docUnitDecimals);
	horizontalGroupBox->setTitle(horizontalGroupBox->title() + " ("+suffix.stripWhiteSpace()+")");
	verticalGroupBox->setTitle(verticalGroupBox->title() + " ("+suffix.stripWhiteSpace()+")");
}

bool GuideManager::deleteValueFormList(QListView *list)
{
	/* previous item pointer to ensure that ++it
	runs before item goes deleted */
	QListViewItem *itemToDelete;
	QListViewItemIterator it(list, QListViewItemIterator::Selected);
	while (it.current())
	{
		itemToDelete = it.current();
		QString value = itemToDelete->text(0);
		if (list == horizontalList)
			currentPage->guides.deleteHorizontal(m_horMap[value], GuideManagerCore::Standard);
		else
			currentPage->guides.deleteVertical(m_verMap[value], GuideManagerCore::Standard);
		++it;
		if (itemToDelete)
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

bool GuideManager::editValueToList(QListView *list)
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

bool GuideManager::addValueToList(QListView *list)
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
		currentPage->guides.addHorizontal(ng, GuideManagerCore::Standard);
		m_horMap[tmp] = ng;
	}
	else
	{
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

void GuideManager::setGuidesFromList(QListView *w, GuideGUIMap *map, Guides guides)
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
	QPtrListIterator<Page> it(*m_Doc->Pages);
	Page *tmpPage;
	while ((tmpPage = it.current()) != 0 )
	{
		++it;
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
//	getAutoHorizontals();
	currentPage->guides.setHorizontalAutoCount(val);
	drawGuides();
}

void GuideManager::horizontalAutoGapSpin_valueChanged(int)
{
	currentPage->guides.setHorizontalAutoGap(value2pts(horizontalAutoGapSpin->value(), docUnitIndex));
	//getAutoHorizontals();
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
//	getAutoVerticals();
	currentPage->guides.setVerticalAutoCount(val);
	drawGuides();
}

void GuideManager::verticalAutoGapSpin_valueChanged(int)
{
	//getAutoVerticals();
	currentPage->guides.setVerticalAutoGap(value2pts(verticalAutoGapSpin->value(), docUnitIndex));
	drawGuides();
}

void GuideManager::verticalAutoGapCheck_stateChanged( int )
{
	verticalAutoGapSpin->setEnabled(verticalAutoGapCheck->isChecked());
	//getAutoVerticals();
	if (verticalAutoGapCheck->isChecked())
		currentPage->guides.setVerticalAutoGap(value2pts(verticalAutoGapSpin->value(), docUnitIndex));
	else
		currentPage->guides.setVerticalAutoGap(value2pts(0.0, docUnitIndex));
	drawGuides();
}

void GuideManager::bGroup_clicked( int val)
{
	currentPage->guides.setAutoRefer(val);
	drawGuides();
}

void GuideManager::tabWidget_currentChanged( QWidget * )
{
	drawGuides();
	if (tabWidget->currentPageIndex() == 1)
	{
		horizontalAutoGapSpin->setEnabled(horizontalAutoGapCheck->isChecked());
		verticalAutoGapSpin->setEnabled(verticalAutoGapCheck->isChecked());
	}
}

void GuideManager::horizontalList_doubleClicked( QListViewItem * )
{
	editValueToList(horizontalList);
}

void GuideManager::horizontalList_returnPressed( QListViewItem * )
{
	editValueToList(horizontalList);
}

void GuideManager::verticalList_returnPressed( QListViewItem * )
{
	editValueToList(verticalList);
}

void GuideManager::verticalList_doubleClicked( QListViewItem * )
{
	editValueToList(verticalList);
}

Guides GuideManager::selectedHorizontals()
{
	Guides retval;
	QListViewItemIterator it(horizontalList);
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
	QListViewItemIterator it(verticalList);
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
	currentPage->guides.clearHorizontals(GuideManagerCore::Standard);
	currentPage->guides.clearVerticals(GuideManagerCore::Standard);
	clearRestoreHorizontalList();
	clearRestoreVerticalList();

	currentPage->guides.setHorizontalAutoCount(0);
	currentPage->guides.setVerticalAutoCount(0);
	currentPage->guides.setHorizontalAutoGap(0.0);
	currentPage->guides.setVerticalAutoGap(0.0);
	currentPage->guides.setAutoRefer(0);
	horizontalAutoCountSpin->setValue(0);
	verticalAutoCountSpin->setValue(0);

	drawGuides();
}

void GuideManager::deleteAllGuides_clicked()
{
	m_drawGuides = false;
	deletePageButton_clicked();
	copyGuidesToAllPages(GuideManagerCore::Standard);
	copyGuidesToAllPages(GuideManagerCore::Auto);
	m_drawGuides = true;
	drawGuides();
}
