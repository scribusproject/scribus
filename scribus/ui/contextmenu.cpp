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

// #include <QDebug>
#include <QApplication>
#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QTextEdit>
#include <QWidget>
#include <QWidgetAction>

#include "contextmenu.h"
#include "prefsmanager.h"
#include "pageitem_textframe.h"
#include "scmimedata.h"
#include "scraction.h"
#include "scrapbookpalette.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "ui/scmwmenumanager.h"
#include "undomanager.h"
#include "canvasmode_normal.h"

// #include "util.h" //just for tdebug

ContextMenu::ContextMenu(Selection & sel, ScribusMainWindow *actionsParent, ScribusDoc* doc, QWidget * parent) :
	QMenu(parent),
	m_Sel(sel),
	m_ScMW(actionsParent),
	m_doc(doc),
	onAPage(false)
{
	if (m_Sel.count()>0)
	{
//		tDebug("process Selection start");
		processSelection();
//		tDebug("process Selection end");
		createMenuItems_Selection();
	}
}

ContextMenu::ContextMenu(ScribusMainWindow *actionsParent, ScribusDoc* doc, double mx, double my, QWidget * parent) :
	QMenu(parent),
	m_Sel(Selection(this)),
	m_ScMW(actionsParent),
	m_doc(doc)
{
	createMenuItems_NoSelection(mx, my);
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

void ContextMenu::createMenuItems_Selection()
{
	//CB TODO clean
	int selectedItemCount=m_Sel.count();
	if (selectedItemCount==0)
		return;
	bool itemsAreSameType=m_Sel.itemsAreSameType();
	//Find our doc from first item in selection if we have an item
	PageItem *currItem = m_Sel.itemAt(0);
	assert(m_doc!=0 && currItem!=0);
	
	QMenu *menuConvertTo = new QMenu(this);
	QMenu *menuEditContents = new QMenu(this);
	QMenu *menuInfo = new QMenu(this);
	QMenu *menuLayer = new QMenu(this);
	QMenu *menuLevel = new QMenu(this);
	QMenu *menuPDF = new QMenu(this);
	QMenu *menuMark = new QMenu(this);
	QMenu *menuResolution = new QMenu(this);
	QMenu *menuLocking = new QMenu(this);
	QMenu *menuSendTo = new QMenu(this);
	QMenu *menuScrapbook = new QMenu(this);
//	QMenu *menuWeld = new QMenu(this);

	//<-- Add Info
	//Test new method with image frames first
	if (selectedItemCount==1 && currItem->asImageFrame())
	{
		QAction *act = addMenu(menuInfo);
		act->setText( ScribusView::tr("In&fo"));
		if (currItem->asImageFrame())
		{
			QLabel* menuLabel = new QLabel("<html>" + currItem->infoDescription() + "</html>", this);
			menuLabel->setFrameShape(QFrame::NoFrame);
			QWidgetAction* menuTextWidget = new QWidgetAction(this);
			menuTextWidget->setDefaultWidget(menuLabel);
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
//			currItem->createContextMenu(menuInfo, 5);
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
		addAction(m_ScMW->scrActions["editUndoAction"]);
	if (undoManager->hasRedoActions())
		addAction(m_ScMW->scrActions["editRedoAction"]);
	//-->

	//<-- Item specific actions
	if (itemsAreSameType)
	{
		if (m_actionList.contains("editEditRenderSource"))
		{
			addSeparator();
			addAction(m_ScMW->scrActions["editEditRenderSource"]);
		}
		if (m_doc->appMode == modeEdit)
		{
			//add actions for marks in edit mode
			addSeparator();
			QAction *act2 = addMenu(menuMark);
			act2->setText( ScribusView::tr("Insert Mark"));
			menuMark->addAction(m_ScMW->scrActions["insertMarkVariableText"]);
			if (m_actionList.contains("insertMarkAnchor"))
			{
				menuMark->addAction(m_ScMW->scrActions["insertMarkAnchor"]);
				menuMark->addAction(m_ScMW->scrActions["insertMarkItem"]);
				menuMark->addAction(m_ScMW->scrActions["insertMark2Mark"]);
				if (!currItem->isNoteFrame())
					menuMark->addAction(m_ScMW->scrActions["insertMarkNote"]);
				//	menuMark->addAction(m_AP->scrActions["insertMarkIndex"]);
			}
			if (currItem->itemText.cursorPosition() < currItem->itemText.length())
			{
				ScText *hl = currItem->itemText.item(currItem->itemText.cursorPosition());
				if (hl->hasMark())
					addAction(m_ScMW->scrActions["editMark"]);
			}
		}
		if (!m_doc->marksList().isEmpty())
		{
			addSeparator();
			addAction(m_ScMW->scrActions["itemUpdateMarks"]);
		}
		if (m_actionList.contains("fileImportText"))
		{
			addSeparator();
			addAction(m_ScMW->scrActions["fileImportText"]);
			addAction(m_ScMW->scrActions["fileImportAppendText"]);
			addAction(m_ScMW->scrActions["toolsEditWithStoryEditor"]);
			addAction(m_ScMW->scrActions["insertSampleText"]);
		}
		else //enable this for, eg, text on a path
			if (m_actionList.contains("toolsEditWithStoryEditor"))
			{
				addSeparator();
				addAction(m_ScMW->scrActions["toolsEditWithStoryEditor"]);
			}
		addSeparator();
		if (m_actionList.contains("fileImportImage"))
			addAction(m_ScMW->scrActions["fileImportImage"]);
		if (selectedItemCount==1 && currItem->asImageFrame())
		{
			if (QApplication::clipboard()->mimeData()->hasImage())
				addAction(m_ScMW->scrActions["editPasteImageFromClipboard"]);
		}
		if (m_actionList.contains("itemAdjustFrameToImage"))
			addAction(m_ScMW->scrActions["itemAdjustFrameToImage"]);
		if (m_actionList.contains("itemAdjustImageToFrame"))
			addAction(m_ScMW->scrActions["itemAdjustImageToFrame"]);

		if (m_actionList.contains("tableInsertRows"))
			addAction(m_ScMW->scrActions["tableInsertRows"]);
		if (m_actionList.contains("tableInsertColumns"))
			addAction(m_ScMW->scrActions["tableInsertColumns"]);
		if (m_actionList.contains("tableDeleteRows"))
			addAction(m_ScMW->scrActions["tableDeleteRows"]);
		if (m_actionList.contains("tableDeleteColumns"))
			addAction(m_ScMW->scrActions["tableDeleteColumns"]);
		if (m_actionList.contains("tableMergeCells"))
			addAction(m_ScMW->scrActions["tableMergeCells"]);
		if (m_actionList.contains("tableSplitCells"))
			addAction(m_ScMW->scrActions["tableSplitCells"]);
		if (m_actionList.contains("tableSetRowHeights"))
			addAction(m_ScMW->scrActions["tableSetRowHeights"]);
		if (m_actionList.contains("tableSetColumnWidths"))
			addAction(m_ScMW->scrActions["tableSetColumnWidths"]);
		if (m_actionList.contains("tableDistributeRowsEvenly"))
			addAction(m_ScMW->scrActions["tableDistributeRowsEvenly"]);
		if (m_actionList.contains("tableDistributeColumnsEvenly"))
			addAction(m_ScMW->scrActions["tableDistributeColumnsEvenly"]);
		if (m_actionList.contains("tableAdjustFrameToTable"))
			addAction(m_ScMW->scrActions["tableAdjustFrameToTable"]);
		if (m_actionList.contains("tableAdjustTableToFrame"))
			addAction(m_ScMW->scrActions["tableAdjustTableToFrame"]);
		if (m_actionList.contains("itemAdjustFrameHeightToText"))
			addAction(m_ScMW->scrActions["itemAdjustFrameHeightToText"]);
		if (m_actionList.contains("itemExtendedImageProperties"))
			addAction(m_ScMW->scrActions["itemExtendedImageProperties"]);
		if (m_actionList.contains("itemAdjustFrameToImage"))
		{
			if (currItem->PictureIsAvailable)
				addAction(m_ScMW->scrActions["itemToggleInlineImage"]);
		}
		if (m_actionList.contains("itemImageInfo"))
			addAction(m_ScMW->scrActions["itemImageInfo"]);
		if (m_actionList.contains("itemUpdateImage"))
			addAction(m_ScMW->scrActions["itemUpdateImage"]);
		
		if (m_actionList.contains("itemPreviewLow"))
		{
			if (m_actionList.contains("itemImageIsVisible"))
				menuResolution->addAction(m_ScMW->scrActions["itemImageIsVisible"]);
			menuResolution->addSeparator();
			if (m_actionList.contains("itemPreviewLow"))
				menuResolution->addAction(m_ScMW->scrActions["itemPreviewLow"]);
			if (m_actionList.contains("itemPreviewNormal"))
				menuResolution->addAction(m_ScMW->scrActions["itemPreviewNormal"]);
			if (m_actionList.contains("itemPreviewFull"))
				menuResolution->addAction(m_ScMW->scrActions["itemPreviewFull"]);
			if (menuResolution->actions().count()>0)
			{
				QAction *act = addMenu(menuResolution);
				act->setText( tr("Preview Settings"));
			}
		}
		
		if (m_actionList.contains("styleImageEffects"))
			addAction(m_ScMW->scrActions["styleImageEffects"]);
		if (m_actionList.contains("editEditWithImageEditor"))
			addAction(m_ScMW->scrActions["editEditWithImageEditor"]);
		if (selectedItemCount==1 && currItem->asImageFrame())
		{
			if (currItem->PictureIsAvailable)
			{
				m_ScMW->scrActions["itemAdjustFrameToImage"]->setEnabled(true);
				m_ScMW->scrActions["itemAdjustImageToFrame"]->setEnabled(true);
				if (currItem->pixm.imgInfo.valid)
					m_ScMW->scrActions["itemExtendedImageProperties"]->setEnabled(true);
				if (currItem->pixm.imgInfo.exifDataValid)
					m_ScMW->scrActions["itemImageInfo"]->setEnabled(true);
				m_ScMW->scrActions["itemUpdateImage"]->setEnabled(true);
				if (currItem->isRaster)
				{
					m_ScMW->scrActions["styleImageEffects"]->setEnabled(true);
					m_ScMW->scrActions["editEditWithImageEditor"]->setEnabled(true);
				}
			}
		}
		
		if ((selectedItemCount==1) && currItem->asTextFrame())
		{
			if (currItem->itemText.length() > 0)
				m_ScMW->scrActions["itemAdjustFrameHeightToText"]->setEnabled(true);
		}
	}
	//-->

	//<-- Item Attributes
	if (selectedItemCount == 1)
	{
		addSeparator();
		addAction(m_ScMW->scrActions["itemAttributes"]);
	}
	//-->

	//<-- Item PDF Options
	if (currItem->itemType() == PageItem::TextFrame)
	{
		QAction *act = addMenu(menuPDF);
		act->setText( ScribusView::tr("&PDF Options"));
		menuPDF->addAction(m_ScMW->scrActions["itemPDFIsAnnotation"]);
		if (!m_doc->masterPageMode())
			menuPDF->addAction(m_ScMW->scrActions["itemPDFIsBookmark"]);
		if (selectedItemCount == 1)
		{
			menuPDF->addSeparator();
			if (m_actionList.contains("itemPDFAnnotationProps"))
				menuPDF->addAction(m_ScMW->scrActions["itemPDFAnnotationProps"]);
			if (m_actionList.contains("itemPDFFieldProps"))
				menuPDF->addAction(m_ScMW->scrActions["itemPDFFieldProps"]);
		}
	}
	//-->

	//<-- Item Locking
	addSeparator();
	//-->
	
	if (selectedItemCount>0)
	{
		menuLocking->addAction(m_ScMW->scrActions["itemLock"]);
		menuLocking->addAction(m_ScMW->scrActions["itemLockSize"]);
		QAction *actL = addMenu(menuLocking);
		actL->setText( ScribusView::tr("Locking"));

		//menuSendTo->addAction(m_AP->scrActions["itemSendToScrapbook"]);

		QAction *actST = addMenu(menuSendTo);
		actST->setText( ScribusView::tr("Send to"));
		QAction *actScr = menuSendTo->addMenu(menuScrapbook);
		actScr->setText( ScribusView::tr("Scrapbook"));
		menuSendTo->addAction(m_ScMW->scrActions["itemSendToPattern"]);
		menuSendTo->addAction(m_ScMW->scrActions["itemSendToInline"]);

		QStringList scrapNames = m_ScMW->scrapbookNames();
		scrapNames.removeAt(1);
		for (int i = 0; i < scrapNames.count(); i++)
		{
			ScrAction *act = new ScrAction( ScrAction::DataInt, QPixmap(), QPixmap(), scrapNames[i], QKeySequence(), this, i);
			menuScrapbook->addAction(act);
			//scrScrapActions.insert(scrapNames[i], act);
			//scrMenuMgr->addMenuItem(act, "itemSendToScrapbook", true);
			connect(act, SIGNAL(triggeredData(int)), m_ScMW, SLOT(PutScrap(int)));
		}
		//QAction *actSl = addMenu(m_AP->scrMenuMgr->getLocalPopupMenu("Locking"));
		//actSl->setText( ScribusView::tr("Locking"));
		//QAction *actSc = addMenu(m_AP->scrMenuMgr->getLocalPopupMenu("SendTo"));
		//actSc->setText( ScribusView::tr("Send to"));
		//<-- Add Layer Items

		if (m_doc->layerCount() > 1)
		{
			QMap<int,int> layerMap;
			for (ScLayers::iterator it = m_doc->Layers.begin(); it != m_doc->Layers.end(); ++it)
				layerMap.insert((*it).Level, (*it).ID);
			int i=layerMap.count()-1;
			while (i>=0)
			{
				if (m_doc->layerLocked(layerMap[i]))
					m_ScMW->scrLayersActions[QString::number(layerMap[i])]->setEnabled(false);
				else
					m_ScMW->scrLayersActions[QString::number(layerMap[i])]->setEnabled(true);
				QPixmap pm(20,15);
				pm.fill(m_doc->layerMarker(layerMap[i]));
				m_ScMW->scrLayersActions[QString::number(layerMap[i])]->setIcon(pm);
				menuLayer->addAction(m_ScMW->scrLayersActions[QString::number(layerMap[i--])]);
			}
			QAction *act = addMenu(menuLayer);
			act->setText( ScribusView::tr("Send to La&yer"));
		}
		//-->
	}
	//<-- Add Groups Items
	if (selectedItemCount > 1)
	{
		if (m_Sel.objectsLayer() != -1)
			addAction(m_ScMW->scrActions["itemGroup"]);
	}
	else
	{
		if (currItem->isGroup())
		{
			addAction(m_ScMW->scrActions["itemUngroup"]);
			addAction(m_ScMW->scrActions["itemGroupAdjust"]);
		}
	}
	//-->

	//<-- Add Level Item
	if (!currItem->locked())
	{
		menuLevel->addAction(m_ScMW->scrActions["itemRaiseToTop"]);
		menuLevel->addAction(m_ScMW->scrActions["itemRaise"]);
		menuLevel->addAction(m_ScMW->scrActions["itemLower"]);
		menuLevel->addAction(m_ScMW->scrActions["itemLowerToBottom"]);
		if (menuLevel->actions().count()>0)
		{
			QAction *act = addMenu(menuLevel);
			act->setText( ScribusView::tr("Le&vel"));
		}
	}
	//-->
	
	//<-- Add Convert To Items
	if (m_doc->appMode != modeEdit && (itemsAreSameType || currItem->isSingleSel)) //Create convertTo Menu
	{
		if (m_ScMW->scrActions["itemConvertToBezierCurve"]->isEnabled() && m_actionList.contains("itemConvertToBezierCurve"))
			menuConvertTo->addAction(m_ScMW->scrActions["itemConvertToBezierCurve"]);
		if (m_ScMW->scrActions["itemConvertToImageFrame"]->isEnabled() && m_actionList.contains("itemConvertToImageFrame"))
			menuConvertTo->addAction(m_ScMW->scrActions["itemConvertToImageFrame"]);
		if (m_ScMW->scrActions["itemConvertToOutlines"]->isEnabled() && m_actionList.contains("itemConvertToOutlines"))
			menuConvertTo->addAction(m_ScMW->scrActions["itemConvertToOutlines"]);
		if (m_ScMW->scrActions["itemConvertToPolygon"]->isEnabled() && m_actionList.contains("itemConvertToPolygon"))
			menuConvertTo->addAction(m_ScMW->scrActions["itemConvertToPolygon"]);
		if (m_ScMW->scrActions["itemConvertToTextFrame"]->isEnabled() && m_actionList.contains("itemConvertToTextFrame"))
			menuConvertTo->addAction(m_ScMW->scrActions["itemConvertToTextFrame"]);
		if (m_ScMW->scrActions["itemConvertToSymbolFrame"]->isEnabled() && !currItem->isSymbol())
			menuConvertTo->addAction(m_ScMW->scrActions["itemConvertToSymbolFrame"]);
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
		addAction(m_ScMW->scrActions["editCut"]);
	if (!(currItem->isSingleSel))
		addAction(m_ScMW->scrActions["editCopy"]);
	if ((m_doc->appMode == modeEdit) && (ScMimeData::clipboardHasScribusText()||ScMimeData::clipboardHasPlainText()) && (currItem->itemType() == PageItem::TextFrame))
		addAction(m_ScMW->scrActions["editPaste"]);
	if (!currItem->locked() && (m_doc->appMode != modeEdit) && (!(currItem->isSingleSel)))
		addAction(m_ScMW->scrActions["itemDelete"]);
	//-->
	
	//<-- Add Contents Actions
	if (itemsAreSameType)
	{
		if (m_actionList.contains("editCopyContents"))
			menuEditContents->addAction(m_ScMW->scrActions["editCopyContents"]);
		if (m_actionList.contains("editPasteContents"))
			menuEditContents->addAction(m_ScMW->scrActions["editPasteContents"]);
		if (currItem->asImageFrame() && m_actionList.contains("editPasteContentsAbs"))
			menuEditContents->addAction(m_ScMW->scrActions["editPasteContentsAbs"]);
	}
	if (m_actionList.contains("editClearContents"))
		menuEditContents->addAction(m_ScMW->scrActions["editClearContents"]);
	if (menuEditContents->actions().count()>0)
	{
		QAction *act = addMenu(menuEditContents);
		act->setText( ScribusView::tr("Contents"));
	}
	//-->
	
	//<-- Add Welding Menu
	addSeparator();
	if (selectedItemCount > 0 && m_doc->appMode != modeEdit)
	{
		PageItem *currItem;
		for (int a = 0; a < m_Sel.count(); ++a)
		{
			currItem = m_Sel.itemAt(a);
			if (currItem->isWelded())
			{
				addAction(m_ScMW->scrActions["itemsUnWeld"]);
				break;
			}
		}
	}
	if (selectedItemCount == 2 && m_doc->appMode != modeEdit)
		addAction(m_ScMW->scrActions["itemWeld"]);
//	{
//		menuWeld->addAction(m_AP->scrActions["itemWeld17"]);
//		menuWeld->addAction(m_AP->scrActions["itemWeld71"]);
//		menuWeld->addAction(m_AP->scrActions["itemWeld13"]);
//		menuWeld->addAction(m_AP->scrActions["itemWeld31"]);
//		QAction *act = addMenu(menuWeld);
//		act->setText( ScribusView::tr("Weld to last..."));
//	}
	//-->
	
	//<-- Add Properties
	addSeparator();
	addAction(m_ScMW->scrActions["toolsProperties"]);
	//-->

}

void ContextMenu::createMenuItems_NoSelection(double mx, double my)
{
	int selectedItemCount=m_Sel.count();
	if (selectedItemCount!=0)
		return;
	
	if (ScMimeData::clipboardHasScribusElem() || ScMimeData::clipboardHasScribusFragment() )
	{
		m_doc->view()->dragX = mx;
		m_doc->view()->dragY = my;
		addAction( ScribusView::tr("&Paste") , m_doc->view(), SLOT(PasteToPage()));
	}
	if (m_ScMW->scrRecentPasteActions.count()>0)
	{
		m_doc->view()->dragX = mx;
		m_doc->view()->dragY = my;
		QMenu* menuPasteRecent = new QMenu(this);
		QAction *act = addMenu(menuPasteRecent);
		act->setText( ScribusView::tr("Paste Recent"));
		
		QMap<QString, QPointer<ScrAction> > scrRecentPasteActions;
		ScrAction *recentPasteAction;
		foreach (recentPasteAction, m_ScMW->scrRecentPasteActions)
			menuPasteRecent->addAction(recentPasteAction);
		addSeparator();
	}
	QAction *act = addAction( tr("Paste File..."));
	connect(act, SIGNAL(triggered()), dynamic_cast<QObject*>(m_doc->view()->m_canvasMode), SLOT(importToPage()));
	addSeparator();

	addAction(m_ScMW->scrActions["editUndoAction"]);
	addAction(m_ScMW->scrActions["editRedoAction"]);
	addSeparator();
	addAction(m_ScMW->scrActions["viewShowMargins"]);
	addAction(m_ScMW->scrActions["viewShowFrames"]);
	addAction(m_ScMW->scrActions["viewShowLayerMarkers"]);
	addAction(m_ScMW->scrActions["viewShowImages"]);
	addAction(m_ScMW->scrActions["viewShowGrid"]);
	addAction(m_ScMW->scrActions["viewShowGuides"]);
	addAction(m_ScMW->scrActions["viewShowColumnBorders"]);
	addAction(m_ScMW->scrActions["viewShowBaseline"]);
	addAction(m_ScMW->scrActions["viewShowTextChain"]);
	addAction(m_ScMW->scrActions["viewShowRulers"]);
	addAction(m_ScMW->scrActions["viewRulerMode"]);
	addSeparator();
	addAction(m_ScMW->scrActions["viewSnapToGrid"]);
	addAction(m_ScMW->scrActions["viewSnapToGuides"]);
	addAction(m_ScMW->scrActions["viewSnapToElements"]);
	
	onAPage = (m_doc->OnPage(mx, my) != -1);
	if (onAPage)
	{
		addSeparator();
		addAction(m_ScMW->scrActions["pageApplyMasterPage"]);
		addAction(m_ScMW->scrActions["pageManageGuides"]);
		addAction(m_ScMW->scrActions["pageManageMargins"]);
		if (m_ScMW->scrActions["pageDelete"]->isEnabled())
		{
			addSeparator();
			pageDeletePrimaryString=m_ScMW->scrActions["pageDelete"]->text();
			m_ScMW->scrActions["pageDelete"]->setText(tr("Delete Page"));
			addAction(m_ScMW->scrActions["pageDelete"]);
		}
	}
}

ContextMenu::~ContextMenu()
{
	if (onAPage && m_ScMW->scrActions["pageDelete"]->isEnabled())
		m_ScMW->scrActions["pageDelete"]->setText(pageDeletePrimaryString);
}
