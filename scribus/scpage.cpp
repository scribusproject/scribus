/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          page.cpp  -  description
                             -------------------
    begin                : Sat Apr 7 2001
    copyright            : (C) 2001 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QDebug>
#include <QPixmap>

#include "scpage.h"
#include "scribus.h"
#include "selection.h"
#include "undomanager.h"
#include "undostate.h"
#include "util_icon.h"

#include "ui/guidemanager.h"
#include "ui/nodeeditpalette.h"

ScPage::ScPage(const double x, const double y, const double b, const double h) :
	UndoObject(QObject::tr("Page")),
	SingleObservable<ScPage>(NULL),
	Margins(40,40,40,40),
	initialMargins(40,40,40,40),
	LeftPg(0),
	MPageNam(""),
	FromMaster(),
	undoManager(UndoManager::instance()),
	m_xOffset(x),
	m_yOffset(y),
	m_width(b),
	m_height(h),
	m_initialWidth(b),
	m_initialHeight(h),
	m_PageName(""),
	m_Doc(NULL)
{
	guides.setPage(this);
	marginPreset = 0;
}

ScPage::~ScPage()
{
	// Clean up any modified template items (unused)
//	for (PageItem *currItem = FromMaster.first(); currItem; currItem = FromMaster.next())
//	{
//		if (currItem->ChangedMasterItem)
//		{
//			FromMaster.remove(currItem);
//			delete currItem;
//		}
//	}
	FromMaster.clear();
}

void ScPage::setDocument(ScribusDoc *doc)
{
	m_Doc=doc;
	setMassObservable(doc? doc->pagesChanged() : NULL);
}

void ScPage::setPageNr(int pageNr)
{
	m_pageNr = pageNr;
	if (m_PageName.isEmpty())
		setUName(QString(QObject::tr("Page") + " %1").arg(m_Doc->FirstPnum + m_pageNr));
	else
		setUName(m_PageName);
}

void ScPage::setPageName(const QString& newName)
{
	m_PageName = newName;
	if (!newName.isEmpty())
		setUName(QObject::tr("Master Page ") + newName);
}

void ScPage::restore(UndoState* state, bool isUndo)
{
	SimpleState* ss = dynamic_cast<SimpleState*>(state);
	if (ss)
	{
//		int stateCode = ss->transactionCode;
		if (ss->contains("ADD_V"))
		{
			double position = ss->getDouble("ADD_V");
			if (isUndo)
				guides.deleteVertical(position, GuideManagerCore::Standard);//removeXGuide(position);
			else
				guides.addVertical(position, GuideManagerCore::Standard);//addXGuide(position);
			m_Doc->scMW()->guidePalette->setupGui();
		}
		else if (ss->contains("ADD_H"))
		{
			double position = ss->getDouble("ADD_H");
			if (isUndo)
				guides.deleteHorizontal(position, GuideManagerCore::Standard);//removeYGuide(position);
			else
				guides.addHorizontal(position, GuideManagerCore::Standard);//addYGuide(position);
			m_Doc->scMW()->guidePalette->setupGui();
		}
		else if (ss->contains("REMOVE_V"))
		{
			double position = ss->getDouble("REMOVE_V");
			if (isUndo)
				guides.addVertical(position, GuideManagerCore::Standard);//addXGuide(position);
			else
				guides.deleteVertical(position, GuideManagerCore::Standard);//removeXGuide(position);
			m_Doc->scMW()->guidePalette->setupGui();
		}
		else if (ss->contains("REMOVE_H"))
		{
			double position = ss->getDouble("REMOVE_H");
			if (isUndo)
				guides.addHorizontal(position, GuideManagerCore::Standard);//addYGuide(position);
			else
				guides.deleteHorizontal(position, GuideManagerCore::Standard);//removeYGuide(position);
			m_Doc->scMW()->guidePalette->setupGui();
		}
		else if (ss->contains("MOVE_H_FROM"))
		{
			double from = ss->getDouble("MOVE_H_FROM");
			double to   = ss->getDouble("MOVE_H_TO");
			if (isUndo)
			{
				guides.deleteHorizontal(to, GuideManagerCore::Standard);//removeYGuide(position);
				guides.addHorizontal(from, GuideManagerCore::Standard);//addYGuide(position);
			}
			else
			{
				guides.deleteHorizontal(from, GuideManagerCore::Standard);//removeYGuide(position);
				guides.addHorizontal(to, GuideManagerCore::Standard);//addYGuide(position);
			}
			m_Doc->scMW()->guidePalette->setupGui();
		}
		else if (ss->contains("MOVE_V_FROM"))
		{
			double from = ss->getDouble("MOVE_V_FROM");
			double to   = ss->getDouble("MOVE_V_TO");
			if (isUndo)
			{
				guides.deleteVertical(to, GuideManagerCore::Standard);//removeXGuide(position);
				guides.addVertical(from, GuideManagerCore::Standard);//removeXGuide(position);
			}
			else
			{
				guides.deleteVertical(from, GuideManagerCore::Standard);//removeXGuide(position);
				guides.addVertical(to, GuideManagerCore::Standard);//removeXGuide(position);
			}
			m_Doc->scMW()->guidePalette->setupGui();
		}
		// automatic guides
		else if (ss->contains("REMOVE_HA_GAP"))
		{
			if (isUndo)
			{
				guides.setHorizontalAutoCount(ss->getInt("REMOVE_HA_COUNT"));
				guides.setHorizontalAutoGap(ss->getDouble("REMOVE_HA_GAP"));
				guides.setHorizontalAutoRefer(ss->getInt("REMOVE_HA_REFER"));
			}
			else
			{
				guides.setHorizontalAutoCount(0);
				guides.setHorizontalAutoGap(0.0);
				guides.setHorizontalAutoRefer(0);
			}
			m_Doc->scMW()->guidePalette->setupGui();
		}
		else if (ss->contains("REMOVE_VA_GAP"))
		{
			if (isUndo)
			{
				guides.setVerticalAutoCount(ss->getInt("REMOVE_VA_COUNT"));
				guides.setVerticalAutoGap(ss->getDouble("REMOVE_VA_GAP"));
				guides.setVerticalAutoRefer(ss->getInt("REMOVE_VA_REFER"));
			}
			else
			{
				guides.setVerticalAutoCount(0);
				guides.setVerticalAutoGap(0.0);
				guides.setVerticalAutoRefer(0);
			}
			m_Doc->scMW()->guidePalette->setupGui();
		}
		else if (ss->contains("CREATE_ITEM"))
			restorePageItemCreation(dynamic_cast<ScItemState<PageItem*>*>(ss), isUndo);
		else if (ss->contains("DELETE_ITEM"))
			restorePageItemDeletion(dynamic_cast<ScItemState< QList<PageItem*> >*>(ss), isUndo);
		else if (ss->contains("CONVERT_ITEM"))
			restorePageItemConversion(dynamic_cast<ScItemState<std::pair<PageItem*, PageItem*> >*>(ss), isUndo);
		else if (ss->contains("PAGE_ATTRS"))
			restorePageAttributes(ss, isUndo);
	}
}

void ScPage::restorePageAttributes(SimpleState *state, bool isUndo)
{
	int left_old = state->getInt("LEFT_OLD");
	QString name_old = state->get("NAME_OLD");
	int orientation_old = state->getInt("ORIENTATION_OLD");
	QString size_old =state->get("SIZE_OLD");
	double width_old = state->getDouble("WIDTH_OLD");
	double height_old = state->getDouble("HEIGHT_OLD");
	double init_height_old = state->getDouble("INIT_HEIGHT_OLD");
	double init_width_old = state->getDouble("INIT_WIDTH_OLD");
	double init_margin_top_old = state->getDouble("INIT_MARGINTOP_OLD");
	double init_margin_bottom_old = state->getDouble("INIT_MARGINBOTTOM_OLD");
	double init_margin_right_old = state->getDouble("INIT_MARGINRIGHT_OLD");
	double init_margin_left_old = state->getDouble("INIT_MARGINLEFT_OLD");
	double margin_top_old = state->getDouble("MARGINTOP_OLD");
	double margin_bottom_old = state->getDouble("MARGINBOTTOM_OLD");
	int margin_preset_old = state->getInt("MARGINPRESET_OLD");
	int left = state->getInt("LEFT");
	QString name = state->get("NAME");
	int orientation = state->getInt("ORIENTATION");
	QString size =state->get("SIZE");
	double width = state->getDouble("WIDTH");
	double height = state->getDouble("HEIGHT");
	double init_height = state->getDouble("INIT_HEIGHT");
	double init_width = state->getDouble("INIT_WIDTH");
	double init_margin_top = state->getDouble("INIT_MARGINTOP");
	double init_margin_bottom = state->getDouble("INIT_MARGINBOTTOM");
	double init_margin_right = state->getDouble("INIT_MARGINRIGHT");
	double init_margin_left = state->getDouble("INIT_MARGINLEFT");
	double margin_top = state->getDouble("MARGINTOP");
	double margin_bottom = state->getDouble("MARGINBOTTOM");
	int margin_preset = state->getInt("MARGINPRESET");
	double horizontal_autogap_old = state->getDouble("HORIZONTAL_AUTOGAP");
	double vertical_autogap_old = state->getDouble("VERTICAL_AUTOGAP");
	double horizontal_autocount_old = state->getDouble("HORIZONTAL_AUTOCOUNT");
	double vertical_autocount_old = state->getDouble("VERTICAL_AUTOCOUNT");
	double horizontal_autorefer_old = state->getDouble("HORIZONTAL_AUTOREFER");
	double vertical_autorefer_old = state->getDouble("VERTICAL_AUTOREFER");
	double horizontal_autogap = state->getDouble("HORIZONTAL_AUTOGAP");
	double vertical_autogap = state->getDouble("VERTICAL_AUTOGAP");
	double horizontal_autocount = state->getDouble("HORIZONTAL_AUTOCOUNT");
	double vertical_autocount = state->getDouble("VERTICAL_AUTOCOUNT");
	double horizontal_autorefer = state->getDouble("HORIZONTAL_AUTOREFER");
	double vertical_autorefer = state->getDouble("VERTICAL_AUTOREFER");

	if (isUndo)
	{
		this->LeftPg = left_old;
		this->setPageName(name_old);
		this->m_pageSize = size_old;
		this->setOrientation(orientation_old);
		this->setWidth(width_old);
		this->setHeight(height_old);
		this->setInitialHeight(init_height_old);
		this->setInitialWidth(init_width_old);
		this->initialMargins.Top    = init_margin_top_old;
		this->initialMargins.Bottom = init_margin_bottom_old;
		this->initialMargins.Left   = init_margin_left_old;
		this->initialMargins.Right  = init_margin_right_old;
		this->marginPreset = margin_preset_old;
		this->Margins.Top = margin_top_old;
		this->Margins.Bottom = margin_bottom_old;
		this->guides.setHorizontalAutoGap(horizontal_autogap_old);
		this->guides.setVerticalAutoGap(vertical_autogap_old);
		this->guides.setHorizontalAutoCount(horizontal_autocount_old);
		this->guides.setVerticalAutoCount(vertical_autocount_old);
		this->guides.setHorizontalAutoRefer(horizontal_autorefer_old);
		this->guides.setVerticalAutoRefer(vertical_autorefer_old);
	}
	else
	{
		this->LeftPg = left;
		this->setPageName(name);
		this->m_pageSize = size;
		this->setOrientation(orientation);
		this->setWidth(width);
		this->setHeight(height);
		this->setInitialHeight(init_height);
		this->setInitialWidth(init_width);
		this->initialMargins.Top    = init_margin_top;
		this->initialMargins.Bottom = init_margin_bottom;
		this->initialMargins.Left   = init_margin_left;
		this->initialMargins.Right  = init_margin_right;
		this->marginPreset = margin_preset;
		this->Margins.Top = margin_top;
		this->Margins.Bottom = margin_bottom;
		this->guides.setHorizontalAutoGap(horizontal_autogap);
		this->guides.setVerticalAutoGap(vertical_autogap);
		this->guides.setHorizontalAutoCount(horizontal_autocount);
		this->guides.setVerticalAutoCount(vertical_autocount);
		this->guides.setHorizontalAutoRefer(horizontal_autorefer);
		this->guides.setVerticalAutoRefer(vertical_autorefer);
	}
}

void ScPage::restorePageItemCreation(ScItemState<PageItem*> *state, bool isUndo)
{
	if (!state)
		return;
	int stateCode = state->transactionCode;
	PageItem *ite = state->getItem();
	bool oldMPMode=m_Doc->masterPageMode();
	if ((stateCode == 0) || (stateCode == 1))
	{
		m_Doc->setMasterPageMode(!ite->OnMasterPage.isEmpty());
		if (m_Doc->appMode == modeEditClip) // switch off from edit shape
			m_Doc->scMW()->nodePalette->EndEdit();
	}
	m_Doc->m_Selection->delaySignalsOn();
	if (isUndo)
	{
		if (m_Doc->m_Selection->findItem(ite)!=-1)
		{
			if (m_Doc->appMode == modeEdit)
				m_Doc->view()->requestMode(modeNormal);
			m_Doc->m_Selection->removeItem(ite);
		}
		if ((stateCode == 0) || (stateCode == 1))
			m_Doc->view()->Deselect(true);
		Selection tmpSelection(m_Doc, false);
		tmpSelection.addItem(ite);
		m_Doc->itemSelection_DeleteItem(&tmpSelection);
	}
	else
	{
		if ((stateCode == 0) || (stateCode == 1))
			m_Doc->view()->Deselect(true);
		m_Doc->Items->append(ite);
		ite->OwnPage = m_Doc->OnPage(ite);
		if ((stateCode == 0) || (stateCode == 2))
			update();
	}
	m_Doc->setMasterPageMode(oldMPMode);
	m_Doc->m_Selection->delaySignalsOff();
}

void ScPage::restorePageItemDeletion(ScItemState< QList<PageItem*> > *state, bool isUndo)
{
	if (!state)
		return;
	QList<PageItem*> itemList = state->getItem();
	int id = state->getInt("ITEMID");
	int id2 = state->getInt("ID");
	if (itemList.count() <= 0) 
		return;
	m_Doc->view()->Deselect(true);
	bool oldMPMode=m_Doc->masterPageMode();
	m_Doc->setMasterPageMode(!itemList.at(0)->OnMasterPage.isEmpty());
	if (m_Doc->appMode == modeEditClip) // switch off from edit shape
		m_Doc->scMW()->nodePalette->EndEdit();
	m_Doc->m_Selection->delaySignalsOn();
	if (isUndo)
	{
		//CB #3373 reinsert at old position and renumber items
		PageItem* ite = itemList.at(id2);
		m_Doc->Items->insert(id, ite);
		for (int i = 0; i < itemList.count(); ++i)
		{
			PageItem* ite = itemList.at(i);
			m_Doc->view()->SelectItem(ite);
		}
 		update();
	}
	else
	{
		Selection tmpSelection(m_Doc, false);
		PageItem* ite = itemList.at(id2);
		if (m_Doc->m_Selection->findItem(ite)!=-1)
		{
			if (m_Doc->appMode == modeEdit)
				m_Doc->view()->requestMode(modeNormal);
			m_Doc->m_Selection->removeItem(ite);
		}
		tmpSelection.addItem(ite);
		m_Doc->itemSelection_DeleteItem(&tmpSelection);
	}
	m_Doc->m_Selection->delaySignalsOff();
	m_Doc->setMasterPageMode(oldMPMode);
}

void ScPage::restorePageItemConversion(ScItemState<std::pair<PageItem*, PageItem*> >*state, bool isUndo)
{
	if (!state)
		return;

	PageItem *oldItem=state->getItem().first;
	PageItem *newItem=state->getItem().second;
	bool oldMPMode=m_Doc->masterPageMode();
	m_Doc->setMasterPageMode(!oldItem->OnMasterPage.isEmpty());
	if (isUndo)
	{
		m_Doc->Items->replace(m_Doc->Items->indexOf(newItem), oldItem);
		oldItem->updatePolyClip();
		m_Doc->AdjustItemSize(oldItem);
	}
	else
	{
		m_Doc->Items->replace(m_Doc->Items->indexOf(oldItem), newItem);
	}
	m_Doc->setMasterPageMode(oldMPMode);
}

void ScPage::setXOffset(const double newCanvasXOffset)
{
	m_xOffset = newCanvasXOffset;
}

void ScPage::setYOffset(const double newCanvasYOffset)
{
	m_yOffset = newCanvasYOffset;
}

void ScPage::setWidth(const double newWidth)
{
	m_width = newWidth;
}

void ScPage::setHeight(const double newHeight)
{
	m_height = newHeight;
}

void ScPage::setInitialWidth(const double newInitialWidth)
{
	m_initialWidth = newInitialWidth;
}

void ScPage::setInitialHeight(const double newInitialHeight)
{
	m_initialHeight = newInitialHeight;
}

void ScPage::setOrientation(int ori)
{
	m_orientation = ori;
}

void ScPage::setPageSectionNumber(const QString& newPageSectionNumber)
{
	m_pageSectionNumber=newPageSectionNumber;
}

void ScPage::copySizingProperties(ScPage* sourcePage, const MarginStruct& pageMargins)
{
	if (sourcePage==NULL)
		return;
	m_pageSize = sourcePage->m_pageSize;
	m_orientation = sourcePage->m_orientation;
	m_width=sourcePage->m_width;
	m_height=sourcePage->m_height;
	m_initialWidth = sourcePage->m_initialWidth;
	m_initialHeight = sourcePage->m_initialHeight;

	Margins.Top    = pageMargins.Top;
	Margins.Bottom = pageMargins.Bottom;
	Margins.Left   = pageMargins.Left;//todo fix for layouts
	Margins.Right  = pageMargins.Right;
	// #8859 do not get initialMargins from pageMargins otherwise
	// margins may be inverted when applying master pages
	initialMargins.Top    = sourcePage->initialMargins.Top;
	initialMargins.Bottom = sourcePage->initialMargins.Bottom;
	initialMargins.Left   = sourcePage->initialMargins.Left;
	initialMargins.Right  = sourcePage->initialMargins.Right;
}
