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

#include "contextmenu.h"

#include <QApplication>
#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QTextEdit>
#include <QWidget>
#include <QWidgetAction>

#include "appmodes.h"
#include "canvas.h"
#include "canvasmode.h"
#include "pageitem_textframe.h"
#include "scmimedata.h"
#include "scraction.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "undomanager.h"


ContextMenu::ContextMenu(Selection & sel, ScribusMainWindow *actionsParent, ScribusDoc* doc, QWidget * parent) :
	QMenu(parent),
	m_Sel(sel),
	m_ScMW(actionsParent),
	m_doc(doc),
	onAPage(false)
{
	if (m_Sel.count()>0)
	{
		processSelection();
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
	
	PageItem* currItem=nullptr;
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
	assert(m_doc!=nullptr && currItem!=nullptr);
	
	QMenu *menuConvertTo = new QMenu(this);
	QMenu *menuEditContent = new QMenu(this);
	QMenu *menuInfo = new QMenu(this);
	QMenu *menuLayer = new QMenu(this);
	QMenu *menuLevel = new QMenu(this);
	QMenu *menuPDF = new QMenu(this);
	QMenu *menuMark = new QMenu(this);
	QMenu *menuResolution = new QMenu(this);
	QMenu *menuLocking = new QMenu(this);
	QMenu *menuSendTo = new QMenu(this);
	QMenu *menuScrapbook = new QMenu(this);
	QMenu *menuEdit = new QMenu(this);
	QMenu *menuImage = new QMenu(this);
//	QMenu *menuWeld = new QMenu(this);

	//<-- Add Info
	//Test new method with image frames first
	if (selectedItemCount==1 && currItem->isImageFrame())
	{
		QAction *act = addMenu(menuInfo);
		act->setText( tr("In&fo"));
		if (currItem->isImageFrame())
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
			printCT->setText( tr("Print: "));
			infoGroupLayout->addWidget( printCT, row, 0, Qt::AlignRight );
			if (currItem->printEnabled())
				printT->setText( tr("Enabled"));
			else
				printT->setText( tr("Disabled"));
			infoGroupLayout->addWidget( printT, row, 1 ); // </a.l.e>
					
			QWidgetAction* MenAct = new QWidgetAction(this);
			MenAct->setDefaultWidget(infoGroup);
			menuInfo->addAction(MenAct);
	
	// Qt4				menuInfo->insertItem(infoGroup);
//			currItem->createContextMenu(menuInfo, 5);
			QAction *act = addMenu(menuInfo);
			act->setText( tr("In&fo"));
		}
		else
		{
			delete infoGroupLayout;
			delete infoGroup;
		}
	}
	//-->

	//<-- Add Contents Actions
	//This part only for text frames
	if (m_actionList.contains("fileImportText"))
	{
		addSeparator();
		menuEditContent->addAction(m_ScMW->scrActions["fileImportText"]);
		menuEditContent->addAction(m_ScMW->scrActions["fileImportAppendText"]);
		menuEditContent->addAction(m_ScMW->scrActions["toolsEditWithStoryEditor"]);
		menuEditContent->addAction(m_ScMW->scrActions["insertSampleText"]);
		menuEditContent->addSeparator();
	}
	else //enable this for, eg, text on a path
		if (m_actionList.contains("toolsEditWithStoryEditor"))
		{
			addSeparator();
			menuEditContent->addAction(m_ScMW->scrActions["toolsEditWithStoryEditor"]);
		}
	if (m_actionList.contains("fileImportImage"))
		menuEditContent->addAction(m_ScMW->scrActions["fileImportImage"]);
	if (selectedItemCount==1 && currItem->isImageFrame())
	{
		if (QApplication::clipboard()->mimeData()->hasImage())
			menuEditContent->addAction(m_ScMW->scrActions["editPasteImageFromClipboard"]);
	}
	if (itemsAreSameType)
	{
		if (m_actionList.contains("editCopyContents"))
			menuEditContent->addAction(m_ScMW->scrActions["editCopyContents"]);
		if (m_actionList.contains("editPasteContents"))
			menuEditContent->addAction(m_ScMW->scrActions["editPasteContents"]);
		if (currItem->isImageFrame() && m_actionList.contains("editPasteContentsAbs"))
			menuEditContent->addAction(m_ScMW->scrActions["editPasteContentsAbs"]);
	}
	if (m_actionList.contains("editClearContents"))
		menuEditContent->addAction(m_ScMW->scrActions["editClearContents"]);
	if (m_actionList.contains("editTruncateContents"))
		menuEditContent->addAction(m_ScMW->scrActions["editTruncateContents"]);
	if (menuEditContent->actions().count()>0)
	{
		QAction *act = addMenu(menuEditContent);
		act->setText( tr("Content"));
	}
	//-->

	//<-- Item specific actions
	if (itemsAreSameType)
	{
		if (m_actionList.contains("editEditRenderSource"))
		{
			addSeparator();
#ifdef HAVE_OSG
			if (currItem->isOSGFrame())
			{
				QAction *act = addAction( tr("Edit 3D Annotation..."));
				connect(act, SIGNAL(triggered()), m_ScMW, SLOT(callImageEditor()));
			}
			else
				addAction(m_ScMW->scrActions["editEditRenderSource"]);
#else
			addAction(m_ScMW->scrActions["editEditRenderSource"]);
#endif
		}
		if (m_doc->appMode == modeEdit)
		{
			//add actions for marks in edit mode
			addSeparator();
			QAction *act2 = addMenu(menuMark);
			act2->setText( tr("Insert Mark"));
			if (m_actionList.contains("insertMarkAnchor"))
			{
				menuMark->addAction(m_ScMW->scrActions["insertMarkAnchor"]);
				if (!currItem->isNoteFrame())
				{
					menuMark->addAction(m_ScMW->scrActions["insertMarkNote"]);
				//	menuMark->addAction(m_AP->scrActions["insertMarkIndex"]);
				}
				menuMark->addAction(m_ScMW->scrActions["insertMarkItem"]);
				menuMark->addAction(m_ScMW->scrActions["insertMark2Mark"]);
			}
			menuMark->addAction(m_ScMW->scrActions["insertMarkVariableText"]);
			if (currItem->itemText.cursorPosition() < currItem->itemText.length())
			{
				if (currItem->itemText.hasMark(currItem->itemText.cursorPosition()))
					addAction(m_ScMW->scrActions["editMark"]);
			}
		}
		if (!m_doc->marksList().isEmpty())
		{
			addSeparator();
			addAction(m_ScMW->scrActions["itemUpdateMarks"]);
		}





		addSeparator();



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

		
		if (m_actionList.contains("itemPreviewLow"))
		{
			if (m_actionList.contains("itemImageIsVisible"))
				menuResolution->addAction(m_ScMW->scrActions["itemImageIsVisible"]);
			menuResolution->addSeparator();
			if (m_actionList.contains("itemPreviewFull"))
				menuResolution->addAction(m_ScMW->scrActions["itemPreviewFull"]);
			if (m_actionList.contains("itemPreviewNormal"))
				menuResolution->addAction(m_ScMW->scrActions["itemPreviewNormal"]);
			if (m_actionList.contains("itemPreviewLow"))
				menuResolution->addAction(m_ScMW->scrActions["itemPreviewLow"]);
			if (menuResolution->actions().count()>0)
			{
				QAction *act = addMenu(menuResolution);
				act->setText( tr("Preview Settings"));
			}
		}
		
		if (m_actionList.contains("styleImageEffects"))
			menuImage->addAction(m_ScMW->scrActions["styleImageEffects"]);
		if (m_actionList.contains("editEditWithImageEditor"))
			menuImage->addAction(m_ScMW->scrActions["editEditWithImageEditor"]);
		if (selectedItemCount==1 && currItem->isImageFrame())
		{
			if (currItem->imageIsAvailable)
			{
				if (m_actionList.contains("itemExtendedImageProperties"))
					menuImage->addAction(m_ScMW->scrActions["itemExtendedImageProperties"]);
				if (m_actionList.contains("itemAdjustFrameToImage"))
				{
					if (currItem->imageIsAvailable)
						menuImage->addAction(m_ScMW->scrActions["itemToggleInlineImage"]);
				}
				if (m_actionList.contains("itemImageInfo"))
					menuImage->addAction(m_ScMW->scrActions["itemImageInfo"]);
				if (m_actionList.contains("itemUpdateImage"))
					menuImage->addAction(m_ScMW->scrActions["itemUpdateImage"]);
				if (m_actionList.contains("itemAdjustFrameToImage"))
					menuImage->addAction(m_ScMW->scrActions["itemAdjustFrameToImage"]);
				if (m_actionList.contains("itemAdjustImageToFrame"))
					menuImage->addAction(m_ScMW->scrActions["itemAdjustImageToFrame"]);
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


				if (menuImage->actions().count()>0)
				{
					QAction *act = addMenu(menuImage);
					act->setText( tr("Image"));
				}
			}
		}
		
		if ((selectedItemCount==1) && currItem->isTextFrame())
		{
			if (currItem->itemText.length() > 0)
				m_ScMW->scrActions["itemAdjustFrameHeightToText"]->setEnabled(true);
		}
	}
	//-->




	addSeparator();

	
	if (selectedItemCount>0)
	{
		//<-- Item Locking
		menuLocking->addAction(m_ScMW->scrActions["itemLock"]);
		menuLocking->addAction(m_ScMW->scrActions["itemLockSize"]);
		QAction *actL = addMenu(menuLocking);
		actL->setText( tr("Locking"));
		//-->

		//<-- Send To
		QAction *actST = addMenu(menuSendTo);
		actST->setText( tr("Send to"));
		QAction *actScr = menuSendTo->addMenu(menuScrapbook);
		actScr->setText( tr("Scrapbook"));
		menuSendTo->addAction(m_ScMW->scrActions["itemSendToPattern"]);
		menuSendTo->addAction(m_ScMW->scrActions["itemSendToInline"]);

		QStringList scrapNames = m_ScMW->scrapbookNames();
		scrapNames.removeAt(1);
		for (int i = 0; i < scrapNames.count(); i++)
		{
			ScrAction *act = new ScrAction(ScrAction::DataInt, QString(), QString(), scrapNames[i], QKeySequence(), this, i);
			menuScrapbook->addAction(act);
			connect(act, SIGNAL(triggeredData(int)), m_ScMW, SLOT(PutScrap(int)));
		}

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
			act->setText( tr("Send to La&yer"));
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

	//<-- Add Level Items
	if (!currItem->locked())
	{
		menuLevel->addAction(m_ScMW->scrActions["itemRaise"]);
		menuLevel->addAction(m_ScMW->scrActions["itemLower"]);
		menuLevel->addAction(m_ScMW->scrActions["itemRaiseToTop"]);
		menuLevel->addAction(m_ScMW->scrActions["itemLowerToBottom"]);
		if (menuLevel->actions().count()>0)
		{
			QAction *act = addMenu(menuLevel);
			act->setText( tr("Le&vel"));
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
			act->setText( tr("Conve&rt to"));
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
		act->setText( tr("&PDF Options"));
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
	//<<-- Edit Menu
	addSeparator();
	//<-- Add Copy/Paste Actions
	if (!currItem->locked() && !(currItem->isSingleSel))
		menuEdit->addAction(m_ScMW->scrActions["editCut"]);
	if (!(currItem->isSingleSel))
		menuEdit->addAction(m_ScMW->scrActions["editCopy"]);
	if (((m_doc->appMode == modeEdit && currItem->itemType() == PageItem::TextFrame) || (m_doc->appMode == modeEditTable && currItem->itemType() == PageItem::Table)) && (ScMimeData::clipboardHasScribusText()||ScMimeData::clipboardHasPlainText()) )
		menuEdit->addAction(m_ScMW->scrActions["editPaste"]);
	if (!currItem->locked() && (m_doc->appMode != modeEdit)  && (m_doc->appMode != modeEditTable) && (!(currItem->isSingleSel)))
		menuEdit->addAction(m_ScMW->scrActions["itemDelete"]);

//<<#14678
	if ((ScMimeData::clipboardHasScribusElem() || ScMimeData::clipboardHasScribusFragment()) &&
		!m_doc->inAnEditMode())
	{
		FPoint fp = m_doc->view()->m_canvas->globalToCanvas(QCursor::pos());
		m_doc->view()->dragX = fp.x();
		m_doc->view()->dragY = fp.y();
		menuEdit->addAction( tr("&Paste Here") , m_doc->view(), SLOT(PasteToPage()));
	}
//>>#14678



	//-->
	//<-- Add undo
	UndoManager * const undoManager(UndoManager::instance());
	if (undoManager->hasUndoActions())
		menuEdit->addAction(m_ScMW->scrActions["editUndoAction"]);
	if (undoManager->hasRedoActions())
		menuEdit->addAction(m_ScMW->scrActions["editRedoAction"]);
	QAction *actEdit = addMenu(menuEdit);
	actEdit->setText( tr("Edit"));
	//-->
	//-->
	
	//<-- Add Welding Menu
	addSeparator();
	if (selectedItemCount > 0 && m_doc->appMode != modeEdit)
	{
		PageItem *currItem2;
		for (int i = 0; i < m_Sel.count(); ++i)
		{
			currItem2 = m_Sel.itemAt(i);
			if (currItem2->isWelded())
			{
				addAction(m_ScMW->scrActions["itemsUnWeld"]);
				break;
			}
		}
	}
	if (selectedItemCount == 2 && m_doc->appMode != modeEdit)
		addAction(m_ScMW->scrActions["itemWeld"]);
	//-->
	
	//<-- Add Properties
	addSeparator();

	addAction(m_ScMW->scrActions["toolsProperties"]);
	addAction(m_ScMW->scrActions["toolsContent"]);
	//-->
}

void ContextMenu::createMenuItems_NoSelection(double mx, double my)
{
	int selectedItemCount = m_Sel.count();
	if (selectedItemCount != 0)
		return;
	bool layerLocked = m_doc->layerLocked(m_doc->activeLayer());
	
	if (!layerLocked && (ScMimeData::clipboardHasScribusElem() || ScMimeData::clipboardHasScribusFragment()))
	{
		m_doc->view()->dragX = mx;
		m_doc->view()->dragY = my;
		addAction( tr("&Paste Here") , m_doc->view(), SLOT(PasteToPage()));
	}
	if (!layerLocked && m_ScMW->scrRecentPasteActions.count() > 0)
	{
		m_doc->view()->dragX = mx;
		m_doc->view()->dragY = my;
		QMenu* menuPasteRecent = new QMenu(this);
		QAction *act = addMenu(menuPasteRecent);
		act->setText( tr("Paste Recent"));
		
		ScrAction *recentPasteAction;
		foreach (recentPasteAction, m_ScMW->scrRecentPasteActions)
			menuPasteRecent->addAction(recentPasteAction);
		addSeparator();
	}
	if (!layerLocked)
	{
		QAction *act = addAction( tr("Paste File..."));
		connect(act, SIGNAL(triggered()), dynamic_cast<QObject*>(m_doc->view()->m_canvasMode), SLOT(importToPage()));
		addSeparator();
	}

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
		addAction(m_ScMW->scrActions["pageManageProperties"]);
		if (m_ScMW->scrActions["pageDelete"]->isEnabled())
		{
			addSeparator();
			pageDeletePrimaryString = m_ScMW->scrActions["pageDelete"]->text();
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
