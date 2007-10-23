/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QTextEdit>
#include <QWidget>
#include <QWidgetAction>

#include "contextmenu.h"
#include "scraction.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "undomanager.h"

ContextMenu::ContextMenu(Selection & sel, ScribusMainWindow *actionsParent, QWidget * parent) :
	QMenu(parent),
	m_Sel(sel),
	m_AP(actionsParent)
{
	processSelection();
	
	createMenuItems();
}

void ContextMenu::processSelection()
{
	int selectedItemCount=m_Sel.count();
	if (selectedItemCount == 0)
		return;
	
	PageItem* currItem=NULL;
	for (int i = 0; i < selectedItemCount; ++i)
	{
		currItem = m_Sel.itemAt(i);
		if (currItem)
		{
			currItem->applicableActions(m_actionList);
		}
	}
}

void ContextMenu::createMenuItems()
{
	//CB TODO clean
	int selectedItemCount=m_Sel.count();
	//Handle 0 items in selection
	if (selectedItemCount==0)
		return;
	bool itemsAreSameType=m_Sel.itemsAreSameType();
	//Find our doc from first item in selection if we have an item
	PageItem *currItem = m_Sel.itemAt(0);
	ScribusDoc* m_doc = 0;
	if (currItem)
		m_doc=currItem->doc();
	assert(m_doc!=0 && currItem!=0);
	
	QMenu *menuConvertTo = new QMenu(this);
	QMenu *menuLayer = new QMenu(this);
	QMenu *menuInfo = new QMenu(this);
	QMenu *menuEditContents = new QMenu(this);
	QMenu *menuLevel = new QMenu(this);
	QMenu *menuResolution = new QMenu(this);
	
	//<-- Add Info
	//Test new method with image frames first
	if (selectedItemCount==1 && currItem->asImageFrame())
	{
		QAction *act = addMenu(menuInfo);
		act->setText( ScribusView::tr("In&fo"));
		if (currItem->asImageFrame())
		{
			QTextEdit* menuTextEdit= new QTextEdit("<html>" + currItem->infoDescription() + "</html>", this);
			menuTextEdit->setReadOnly(true);
			QWidgetAction* menuTextWidget = new QWidgetAction(this);
			menuTextWidget->setDefaultWidget(menuTextEdit);
			menuInfo->addAction(menuTextWidget);
		}
	}
	else
	{
		QFrame *infoGroup = new QFrame( m_doc->view() );
		infoGroup->setFrameShape( QFrame::NoFrame );
		QGridLayout *infoGroupLayout = new QGridLayout( infoGroup );
		infoGroupLayout->setAlignment( Qt::AlignTop );
		infoGroupLayout->setSpacing( 2 );
		infoGroupLayout->setMargin( 0 );
		if (currItem->createInfoGroup(infoGroup, infoGroupLayout)) 
		{
			int row = infoGroupLayout->rowCount(); // <a.l.e>
	
			QLabel *printCT = new QLabel(infoGroup);
			QLabel *printT = new QLabel(infoGroup);
			printCT->setText( ScribusView::tr("Print: "));
			infoGroupLayout->addWidget( printCT, row, 0, Qt::AlignRight );
			if (currItem->printEnabled())
				printT->setText( ScribusView::tr("Enabled"));
			else
				printT->setText( ScribusView::tr("Disabled"));
			infoGroupLayout->addWidget( printT, row, 1 ); // </a.l.e>
					
			QWidgetAction* MenAct = new QWidgetAction(this);
			MenAct->setDefaultWidget(infoGroup);
			menuInfo->addAction(MenAct);
	
	// Qt4				menuInfo->insertItem(infoGroup);
			currItem->createContextMenu(menuInfo, 5);
			QAction *act = addMenu(menuInfo);
			act->setText( ScribusView::tr("In&fo"));
		} else	{
			delete infoGroupLayout;
			delete infoGroup;
		}
	}
	//-->
	
	//<-- Add undo
	UndoManager * const undoManager(UndoManager::instance());
	if (undoManager->hasUndoActions())
		addAction(m_AP->scrActions["editUndoAction"]);
	if (undoManager->hasRedoActions())
		addAction(m_AP->scrActions["editRedoAction"]);
	//-->
	
	//<-- Item specific actions
	if (itemsAreSameType)
	{
		if (m_actionList.contains("editEditWithLatexEditor"))
		{
			addSeparator();
			addAction(m_AP->scrActions["editEditWithLatexEditor"]);
		}
		if (m_actionList.contains("fileImportText"))
		{
			addSeparator();
			addAction(m_AP->scrActions["fileImportText"]);
			addAction(m_AP->scrActions["fileImportAppendText"]);
			addAction(m_AP->scrActions["toolsEditWithStoryEditor"]);
			addAction(m_AP->scrActions["insertSampleText"]);
		}
		addSeparator();
		if (m_actionList.contains("fileImportImage"))
			addAction(m_AP->scrActions["fileImportImage"]);
		if (m_actionList.contains("itemAdjustFrameToImage"))
			addAction(m_AP->scrActions["itemAdjustFrameToImage"]);
		if (m_actionList.contains("itemExtendedImageProperties"))
			addAction(m_AP->scrActions["itemExtendedImageProperties"]);
		if (m_actionList.contains("itemUpdateImage"))
			addAction(m_AP->scrActions["itemUpdateImage"]);
		
		if (m_actionList.contains("itemPreviewLow"))
		{
			if (m_actionList.contains("itemImageIsVisible"))
				menuResolution->addAction(m_AP->scrActions["itemImageIsVisible"]);
			menuResolution->addSeparator();
			if (m_actionList.contains("itemPreviewLow"))
				menuResolution->addAction(m_AP->scrActions["itemPreviewLow"]);
			if (m_actionList.contains("itemPreviewNormal"))
				menuResolution->addAction(m_AP->scrActions["itemPreviewNormal"]);
			if (m_actionList.contains("itemPreviewFull"))
				menuResolution->addAction(m_AP->scrActions["itemPreviewFull"]);
			if (menuResolution->actions().count()>0)
			{
				QAction *act = addMenu(menuResolution);
				act->setText( tr("Preview Settings"));
			}
		}
		
		if (m_actionList.contains("styleImageEffects"))
			addAction(m_AP->scrActions["styleImageEffects"]);
		if (m_actionList.contains("editEditWithImageEditor"))
			addAction(m_AP->scrActions["editEditWithImageEditor"]);
		
	}
	//-->
	
	//<-- Item Attributes
	if (selectedItemCount == 1)
	{
		addSeparator();
		addAction(m_AP->scrActions["itemAttributes"]);
	}
	//-->
	
	//<-- Item Locking
	addSeparator();
	addAction(m_AP->scrActions["itemLock"]);
	addAction(m_AP->scrActions["itemLockSize"]);
	//-->
	
	if (selectedItemCount>0)
	{
		addAction(m_AP->scrActions["itemSendToScrapbook"]);
		addAction(m_AP->scrActions["itemSendToPattern"]);
		//<-- Add Layer Items
		if (m_doc->layerCount() > 1)
		{
			QMap<int,int> layerMap;
			for (ScLayers::iterator it = m_doc->Layers.begin(); it != m_doc->Layers.end(); ++it)
				layerMap.insert((*it).Level, (*it).LNr);
			int i=layerMap.count()-1;
			while (i>=0)
			{
				if (m_doc->layerLocked(layerMap[i]))
					m_AP->scrLayersActions[QString::number(layerMap[i])]->setEnabled(false);
				else
					m_AP->scrLayersActions[QString::number(layerMap[i])]->setEnabled(true);
				menuLayer->addAction(m_AP->scrLayersActions[QString::number(layerMap[i--])]);
			}
			QAction *act = addMenu(menuLayer);
			act->setText( ScribusView::tr("Send to La&yer"));
		}
		//-->
	}
	//<-- Add Groups Items
	if (selectedItemCount>1)
	{
		bool isGroup = true;
		int firstElem = -1;
		if (currItem->Groups.count() != 0)
			firstElem = currItem->Groups.top();
		for (int bx = 0; bx < selectedItemCount; ++bx)
		{
			if (m_Sel.itemAt(bx)->Groups.count() != 0)
			{
				if (m_Sel.itemAt(bx)->Groups.top() != firstElem)
					isGroup = false;
			}
			else
				isGroup = false;
		}
		if (!isGroup)
			addAction(m_AP->scrActions["itemGroup"]);
	}
	if (currItem->Groups.count() != 0)
		addAction(m_AP->scrActions["itemUngroup"]);
	//-->

	//<-- Add Level Item
	if (!currItem->locked())
	{
		if ((!currItem->isTableItem) && (!currItem->isSingleSel))
		{
			QAction *act = addMenu(menuLevel);
			act->setText( ScribusView::tr("Le&vel"));
			menuLevel->addAction(m_AP->scrActions["itemRaiseToTop"]);
			menuLevel->addAction(m_AP->scrActions["itemRaise"]);
			menuLevel->addAction(m_AP->scrActions["itemLower"]);
			menuLevel->addAction(m_AP->scrActions["itemLowerToBottom"]);
		}
	}
	//-->
	
	//<-- Add Convert To Items
	if (m_doc->appMode != modeEdit && (itemsAreSameType || currItem->isSingleSel)) //Create convertTo Menu
	{
		if (m_AP->scrActions["itemConvertToBezierCurve"]->isEnabled() && m_actionList.contains("itemConvertToBezierCurve"))
			menuConvertTo->addAction(m_AP->scrActions["itemConvertToBezierCurve"]);
		if (m_AP->scrActions["itemConvertToImageFrame"]->isEnabled() && m_actionList.contains("itemConvertToImageFrame"))
			menuConvertTo->addAction(m_AP->scrActions["itemConvertToImageFrame"]);
		if (m_AP->scrActions["itemConvertToOutlines"]->isEnabled() && m_actionList.contains("itemConvertToOutlines"))
			menuConvertTo->addAction(m_AP->scrActions["itemConvertToOutlines"]);
		if (m_AP->scrActions["itemConvertToPolygon"]->isEnabled() && m_actionList.contains("itemConvertToPolygon"))
			menuConvertTo->addAction(m_AP->scrActions["itemConvertToPolygon"]);
		if (m_AP->scrActions["itemConvertToTextFrame"]->isEnabled() && m_actionList.contains("itemConvertToTextFrame"))
			menuConvertTo->addAction(m_AP->scrActions["itemConvertToTextFrame"]);
		if (menuConvertTo->actions().count()>0)
		{
			QAction *act = addMenu(menuConvertTo);
			act->setText( ScribusView::tr("Conve&rt to"));
		}
	}
	//-->
	
	//<-- Add Copy/Paste Actions
	addSeparator();
	if (!currItem->locked() && !(currItem->isSingleSel))
		addAction(m_AP->scrActions["editCut"]);
	if (!(currItem->isSingleSel))
		addAction(m_AP->scrActions["editCopy"]);
	if ((m_doc->appMode == modeEdit) && (m_AP->Buffer2.startsWith("<SCRIBUSTEXT")) && (currItem->itemType() == PageItem::TextFrame))
		addAction(m_AP->scrActions["editPaste"]);
	if (!currItem->locked() && (m_doc->appMode != modeEdit) && (!(currItem->isSingleSel)))
		addAction(m_AP->scrActions["itemDelete"]);
	//-->
	
	//<-- Add Contents Actions
	if (itemsAreSameType)
	{
		if (m_actionList.contains("editCopyContents"))
			menuEditContents->addAction(m_AP->scrActions["editCopyContents"]);
		if (m_actionList.contains("editPasteContents"))
			menuEditContents->addAction(m_AP->scrActions["editPasteContents"]);
		if (m_actionList.contains("editPasteContentsAbs"))
			menuEditContents->addAction(m_AP->scrActions["editPasteContentsAbs"]);
	}
	if (m_actionList.contains("editClearContents"))
		menuEditContents->addAction(m_AP->scrActions["editClearContents"]);
	if (menuEditContents->actions().count()>0)
	{
		QAction *act = addMenu(menuEditContents);
		act->setText( ScribusView::tr("Contents"));
	}
	//-->
	
	//<-- Add Properties
	addSeparator();
	addAction(m_AP->scrActions["toolsProperties"]);
	//-->
}

