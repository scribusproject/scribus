/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "appmodehelper.h"
#include "appmodes.h"
#include "canvasmode.h"
#include "pageitem.h"
#include "pageitem_table.h"
#include "pluginmanager.h"
#include "prefsmanager.h"
#include "scmimedata.h"
#include "scribus.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "ui/charselect.h"
#include "ui/hruler.h"
#include "ui/modetoolbar.h"
#include "ui/outlinepalette.h"
#include "ui/pageselector.h"
#include "ui/propertiespalette.h"
#include "ui/scmwmenumanager.h"
#include "ui/scrspinbox.h"
#include "ui/vruler.h"
#include "undomanager.h"
#include "util_icon.h"


AppModeHelper::AppModeHelper(QObject *parent) :
    QObject(parent)
{
	a_actMgr=NULL;
	a_scrActions=NULL;
	a_scrRecentFileActions=NULL;
	a_scrWindowsActions=NULL;
	a_scrScrapActions=NULL;
	a_scrLayersActions=NULL;
	a_scrRecentPasteActions=NULL;
}

void AppModeHelper::setup(ActionManager* am,
						  QMap<QString, QPointer<ScrAction> > *scrA,
						  QMap<QString, QPointer<ScrAction> > *scrRFA,
						  QMap<QString, QPointer<ScrAction> > *sWA,
						  QMap<QString, QPointer<ScrAction> > *scSA,
						  QMap<QString, QPointer<ScrAction> > *scLA,
						  QMap<QString, QPointer<ScrAction> > *scRPA)
{
	a_actMgr=am;
	a_scrActions=scrA;
	a_scrRecentFileActions=scrRFA;
	a_scrWindowsActions=sWA;
	a_scrScrapActions=scSA;
	a_scrLayersActions=scLA;
	a_scrRecentPasteActions=scRPA;

}

void AppModeHelper::resetApplicationMode(ScribusMainWindow* scmw, int newMode)
{
	a_actMgr->disconnectModeActions();
	setModeActionsPerMode(newMode);
	a_actMgr->connectModeActions();
	return;
}

void AppModeHelper::setApplicationMode(ScribusMainWindow* scmw, ScribusDoc* doc, int newMode)
{
	//Application modes start at 1 (appmodes.h), canvasmodes start at 1000 (canvasmode.h)
	assert(newMode < submodeFirstSubmode);

	//If no doc and we end here, just reset the tool actions
	if (doc==NULL)
	{
		resetApplicationMode(scmw, newMode);
		return;
	}

	//disconnect the tools actions so we dont fire them off
	a_actMgr->disconnectModeActions();
	setModeActionsPerMode(newMode);


	int oldMode = doc->appMode;
	PageItem *currItem=0;
	if (!doc->m_Selection->isEmpty())
		currItem = doc->m_Selection->itemAt(0);

	if (oldMode == modeEditClip && newMode != modeEditClip)
		scmw->NoFrameEdit();
	else if (oldMode != modeEditClip && newMode == modeEditClip)
		scmw->ToggleFrameEdit();

	//Ugly hack but I have absolutly no idea about how to do this in another way
	if(UndoManager::undoEnabled() && currItem && oldMode != newMode && (newMode == modeEditMeshPatch || newMode == modeEditMeshGradient || newMode == modeEditGradientVectors || oldMode == modeEditMeshPatch || oldMode == modeEditMeshGradient || oldMode == modeEditGradientVectors || oldMode == modeEditPolygon || newMode == modeEditPolygon || oldMode == modeEditArc || newMode == modeEditArc || oldMode == modeEditSpiral || newMode == modeEditSpiral))
	{
		SimpleState *ss = new SimpleState(Um::Mode);
		ss->set("CHANGE_MODE","change_mode");
		ss->set("OLD",oldMode);
		ss->set("NEW",newMode);
		UndoManager::instance()->action(currItem,ss);
	}
	doc->appMode = newMode ;
	//disable text action which work only text frame in edit mode
	if ((newMode != modeEdit) || (!currItem) || !currItem->isTextFrame())
		enableTextActions(false);
	if (newMode != modeDrawShapes)
		doc->SubMode = -1;
	if (newMode != modeNormal && newMode != modeStoryEditor)
		scmw->activateWindow();


	switch (oldMode)
	{
		case modeEdit:
			{
				if (newMode!=modeEdit)
					a_actMgr->restoreActionShortcutsPostEditMode();
				scmw->zoomSpinBox->setFocusPolicy(Qt::ClickFocus);
				scmw->pageSelector->setFocusPolicy(Qt::ClickFocus);
				(*a_scrActions)["editClearContents"]->setEnabled(false);
				(*a_scrActions)["editTruncateContents"]->setEnabled(false);
				scmw->charPalette->setEnabled(false, 0);
				if (currItem != 0)
				{
					currItem->update();
					if (currItem->asTextFrame())
						enableTextActions(false);
					//		scrMenuMgr->setMenuEnabled("Item", true);
					setTextEditMode(false);
				}
				doc->view()->horizRuler->textMode(false);
				doc->view()->horizRuler->update();
			}
			break;
		case modeEditTable:
			if (newMode != modeEditTable)
			{
				scmw->outlinePalette->setEnabled(true);
				scmw->charPalette->setEnabled(false, 0);
				enableTextActions(false);
				(*a_scrActions)["insertSampleText"]->setEnabled(false);
				(*a_scrActions)["toolsEditWithStoryEditor"]->setEnabled(false);
				a_actMgr->restoreActionShortcutsPostEditMode();
			}
			break;
		case modeEditArc:
			setSpecialEditMode(false);
			break;
		case modeEditSpiral:
			setSpecialEditMode(false);
			break;
		case modeEditPolygon:
			setSpecialEditMode(false);
			break;
		case modeLinkFrames:
			doc->regionsChanged()->update(QRect());
			break;
		case modeUnlinkFrames:
			doc->regionsChanged()->update(QRect());
			break;
	}

	switch (newMode)
	{
		case modeNormal:
			{
				scmw->propertiesPalette->setGradientEditMode(false);
				scmw->outlinePalette->setEnabled(true);
			}
			break;
		case modeDrawShapes:
			{
				doc->SubMode = scmw->modeToolBar->SubMode;
				doc->ShapeValues = scmw->modeToolBar->ShapeVals;
				doc->ValCount = scmw->modeToolBar->ValCount;
				scmw->emitUpdateRequest(reqCustomShapeUpdate);
			}
			break;
		case modeStoryEditor:
			{
				scmw->slotStoryEditor(oldMode == modeEditTable);
				scmw->slotSelect();
			}
			break;
		case modeDrawImage:
			break;
		case modeDrawText:
			break;
		case modeMagnifier:
			break;
		case modeEdit:
			{
				if (oldMode!=modeEdit)
					a_actMgr->saveActionShortcutsPreEditMode();
				if (currItem != 0)
				{
					currItem->itemText.setCursorPosition(0);
					scmw->setTBvals(currItem);
				}
				(*a_scrActions)["editPaste"]->setEnabled(false);
				scmw->charPalette->setEnabled(true, currItem);
				if (currItem!=NULL && currItem->asTextFrame())
				{
					enableTextActions(true, currItem->currentCharStyle().font().scName());
					currItem->asTextFrame()->togleEditModeActions();
				}
				if (ScMimeData::clipboardHasScribusData())
				{
					bool textFrameEditMode = ((currItem != NULL) && (currItem->asTextFrame()));
					(*a_scrActions)["editPaste"]->setEnabled( textFrameEditMode || (currItem == NULL) );
				}
				setTextEditMode(true);

				if (currItem != 0)
				{
					(*a_scrActions)["editCut"]->setEnabled(currItem->HasSel);
					(*a_scrActions)["editCopy"]->setEnabled(currItem->HasSel);
					(*a_scrActions)["editClearContents"]->setEnabled(currItem->HasSel);
					(*a_scrActions)["editTruncateContents"]->setEnabled(currItem->HasSel);
					(*a_scrActions)["editSearchReplace"]->setEnabled(true);
				}
			}
			break;
		case modeDrawLine:
			break;
		case modeRotation:
			break;
		case modeLinkFrames:
			{
				doc->regionsChanged()->update(QRect());
				doc->ElemToLink = doc->m_Selection->itemAt(0);
			}
			break;
		case modeImportImage:
			break;
		case modeUnlinkFrames:
			doc->regionsChanged()->update(QRect());
			break;
		case modeDrawRegularPolygon:
			break;
		case modeDrawBezierLine:
			{
				if ((doc->m_Selection->count() != 0) && (!PrefsManager::instance()->appPrefs.uiPrefs.stickyTools))
					doc->view()->Deselect(true);
				doc->view()->FirstPoly = true;
			}
			break;
		case modeInsertPDFButton:
			break;
		case modeInsertPDFTextfield:
			break;
		case modeInsertPDFCheckbox:
			break;
		case modeInsertPDFCombobox:
			break;
		case modeInsertPDFListbox:
			break;
		case modeInsertPDFTextAnnotation:
			break;
		case modeInsertPDFLinkAnnotation:
			break;
		case modeDrawFreehandLine:
			break;
		case modeDrawTable:
			break;
		case modeDrawTable2:
			break;
		case modePanning:
			break;
		case modeMeasurementTool:
			break;
		case modeEditGradientVectors:
			{
				scmw->propertiesPalette->setGradientEditMode(true);
			}
			break;
		case modeEyeDropper:
			break;
		case modeCopyProperties:
			{
				if (!doc->m_Selection->isEmpty())
				{
					doc->ElemToLink = doc->m_Selection->itemAt(0);
					doc->view()->Deselect(true);
					(*a_scrActions)["toolsCopyProperties"]->setEnabled(true);
				}
			}
			break;
		case modeEditClip:
			break;
		case modeDrawLatex:
			break;
		case modeImportObject:
			break;
		case modeInsertPDF3DAnnotation:
			break;
		case modeEditMeshGradient:
			break;
		case modeDrawCalligraphicLine:
			break;
		case modeDrawArc:
			break;
		case modeEditArc:
			setSpecialEditMode(true);
			break;
		case modeEditPolygon:
			setSpecialEditMode(true);
			break;
		case modeDrawSpiral:
			break;
		case modeEditSpiral:
			setSpecialEditMode(true);
			scmw->outlinePalette->setEnabled(false);
			break;
		case modeEditTable:
			{
				if (oldMode != modeEditTable)
				{
					scmw->outlinePalette->setEnabled(false);
					scmw->charPalette->setEnabled(true, currItem);
					(*a_scrActions)["insertSampleText"]->setEnabled(true);
					(*a_scrActions)["toolsEditWithStoryEditor"]->setEnabled(true);
					PageItem *i2 = currItem->asTable()->activeCell().textFrame();
					enableTextActions(true, i2->currentCharStyle().font().scName());
					a_actMgr->saveActionShortcutsPreEditMode();
				}
			}
			break;
		case modeEditMeshPatch:
			break;
		case modeEditWeldPoint:
			break;
		case modeInsertPDFRadioButton:
			break;
		default:
			//No doc open?
			break;
	}
	emit AppModeChanged(oldMode, newMode);
	a_actMgr->connectModeActions();
	PluginManager::instance().enablePluginActionsForSelection(scmw);
}

void AppModeHelper::setModeActionsPerMode(int newMode)
{
	//set the actions state based on incoming mode
	(*a_scrActions)["toolsSelect"]->setChecked(newMode==modeNormal);
	(*a_scrActions)["toolsInsertTextFrame"]->setChecked(newMode==modeDrawText);
	(*a_scrActions)["toolsInsertImageFrame"]->setChecked(newMode==modeDrawImage);
	(*a_scrActions)["toolsInsertTable"]->setChecked(newMode==modeDrawTable2);
	(*a_scrActions)["toolsInsertShape"]->setChecked(newMode==modeDrawShapes);
	(*a_scrActions)["toolsInsertPolygon"]->setChecked(newMode==modeDrawRegularPolygon);
	(*a_scrActions)["toolsInsertArc"]->setChecked(newMode==modeDrawArc);
	(*a_scrActions)["toolsInsertSpiral"]->setChecked(newMode==modeDrawSpiral);
	(*a_scrActions)["toolsInsertLine"]->setChecked(newMode==modeDrawLine);
	(*a_scrActions)["toolsInsertBezier"]->setChecked(newMode==modeDrawBezierLine);
	(*a_scrActions)["toolsInsertFreehandLine"]->setChecked(newMode==modeDrawFreehandLine);
	(*a_scrActions)["toolsInsertCalligraphicLine"]->setChecked(newMode == modeDrawCalligraphicLine);
	(*a_scrActions)["toolsInsertRenderFrame"]->setChecked(newMode==modeDrawLatex);
	(*a_scrActions)["toolsRotate"]->setChecked(newMode==modeRotation);
	(*a_scrActions)["toolsZoom"]->setChecked(newMode==modeMagnifier);
	(*a_scrActions)["toolsEditContents"]->setChecked(newMode==modeEdit);
	(*a_scrActions)["toolsEditWithStoryEditor"]->setChecked(newMode==modeStoryEditor);
	(*a_scrActions)["toolsLinkTextFrame"]->setChecked(newMode==modeLinkFrames);
	(*a_scrActions)["toolsUnlinkTextFrame"]->setChecked(newMode==modeUnlinkFrames);
//	(*a_scrActions)["toolsUnlinkTextFrameWithTextCopy"]->setChecked(newMode==modeUnlinkFrames);
//	(*a_scrActions)["toolsUnlinkTextFrameWithTextCut"]->setChecked(newMode==modeUnlinkFrames);
	(*a_scrActions)["toolsEyeDropper"]->setChecked(newMode==modeEyeDropper);
	(*a_scrActions)["toolsMeasurements"]->setChecked(newMode==modeMeasurementTool);
	(*a_scrActions)["toolsCopyProperties"]->setChecked(newMode==modeCopyProperties);
	(*a_scrActions)["toolsPDFPushButton"]->setChecked(newMode==modeInsertPDFButton);
	(*a_scrActions)["toolsPDFRadioButton"]->setChecked(newMode==modeInsertPDFRadioButton);
	(*a_scrActions)["toolsPDFTextField"]->setChecked(newMode==modeInsertPDFTextfield);
	(*a_scrActions)["toolsPDFCheckBox"]->setChecked(newMode==modeInsertPDFCheckbox);
	(*a_scrActions)["toolsPDFComboBox"]->setChecked(newMode==modeInsertPDFCombobox);
	(*a_scrActions)["toolsPDFListBox"]->setChecked(newMode==modeInsertPDFListbox);
	(*a_scrActions)["toolsPDFAnnotText"]->setChecked(newMode==modeInsertPDFTextAnnotation);
	(*a_scrActions)["toolsPDFAnnotLink"]->setChecked(newMode==modeInsertPDFLinkAnnotation);
#ifdef HAVE_OSG
	(*a_scrActions)["toolsPDFAnnot3D"]->setChecked(newMode==modeInsertPDF3DAnnotation);
#endif
}

void AppModeHelper::setActionGroupEnabled(QMap<QString, QPointer<ScrAction> >*ag, bool enabled)
{
	if (ag!=NULL)
		for( QMap<QString, QPointer<ScrAction> >::Iterator it = ag->begin(); it!=ag->end(); ++it )
			(*it)->setEnabled(enabled);
}

void AppModeHelper::setTextEditMode(bool b)
{
	bool b2=!b;

	(*a_scrActions)["itemDuplicate"]->setEnabled(b2);
	(*a_scrActions)["itemMulDuplicate"]->setEnabled(b2);
	(*a_scrActions)["itemTransform"]->setEnabled(b2);
	(*a_scrActions)["itemDelete"]->setEnabled(b2);
	(*a_scrActions)["itemRaise"]->setEnabled(b2);
	(*a_scrActions)["itemLower"]->setEnabled(b2);
	(*a_scrActions)["itemRaiseToTop"]->setEnabled(b2);
	(*a_scrActions)["itemLowerToBottom"]->setEnabled(b2);
	//scmw->scrMenuMgr->setMenuEnabled("itemSendToScrapbook", b2);
	(*a_scrActions)["itemSendToPattern"]->setEnabled(b2);
	(*a_scrActions)["itemSendToInline"]->setEnabled(b2);
	(*a_scrActions)["itemAdjustFrameToImage"]->setEnabled(b2);
	(*a_scrActions)["itemAdjustImageToFrame"]->setEnabled(b2);
	(*a_scrActions)["itemExtendedImageProperties"]->setEnabled(b2);
	(*a_scrActions)["itemUpdateImage"]->setEnabled(b2);
	(*a_scrActions)["itemPreviewFull"]->setEnabled(b2);
	(*a_scrActions)["itemPreviewNormal"]->setEnabled(b2);
	(*a_scrActions)["itemPreviewLow"]->setEnabled(b2);
	(*a_scrActions)["itemAttributes"]->setEnabled(b2);
}

void AppModeHelper::setSpecialEditMode(bool b)
{
	bool b2=!b;

	(*a_scrActions)["editCut"]->setEnabled(b2);
	(*a_scrActions)["editCopy"]->setEnabled(b2);
	(*a_scrActions)["editPaste"]->setEnabled(b2);
	(*a_scrActions)["editCopyContents"]->setEnabled(b2);
	(*a_scrActions)["editPasteContents"]->setEnabled(b2);
	(*a_scrActions)["editPasteContentsAbs"]->setEnabled(b2);
	(*a_scrActions)["editSelectAll"]->setEnabled(b2);
	(*a_scrActions)["editSelectAllOnLayer"]->setEnabled(b2);
	(*a_scrActions)["editDeselectAll"]->setEnabled(b2);
	(*a_scrActions)["itemDelete"]->setEnabled(b2);
	(*a_scrActions)["itemSendToPattern"]->setEnabled(b2);
	(*a_scrActions)["itemSendToInline"]->setEnabled(b2);
	(*a_scrActions)["itemConvertToTextFrame"]->setEnabled(b2);
	(*a_scrActions)["itemConvertToImageFrame"]->setEnabled(b2);
	(*a_scrActions)["itemConvertToPolygon"]->setEnabled(b2);
	(*a_scrActions)["itemConvertToBezierCurve"]->setEnabled(b2);
	(*a_scrActions)["itemConvertToOutlines"]->setEnabled(b2);
	(*a_scrActions)["itemConvertToSymbolFrame"]->setEnabled(b2);
	(*a_scrActions)["pageInsert"]->setEnabled(b2);
	(*a_scrActions)["pageImport"]->setEnabled(b2);
	(*a_scrActions)["pageDelete"]->setEnabled(b2);
	(*a_scrActions)["pageCopy"]->setEnabled(b2);
	(*a_scrActions)["pageCopyToMasterPage"]->setEnabled(b2);

	setActionGroupEnabled(a_scrLayersActions, b2);
	setActionGroupEnabled(a_scrRecentFileActions, b2);
	setActionGroupEnabled(a_scrRecentPasteActions, b2);
	setActionGroupEnabled(a_scrScrapActions, b2);
	setActionGroupEnabled(a_scrWindowsActions, b2);
}

void AppModeHelper::setFrameEditMode(bool b)
{
	bool b2=!b;

	(*a_scrActions)["itemShapeEdit"]->setChecked(b);

	//CB Enable/Disable undo in frame edit mode
	if (b2)
	{
		(*a_scrActions)["editUndoAction"]->setEnabled(UndoManager::instance()->hasUndoActions());
		(*a_scrActions)["editRedoAction"]->setEnabled(UndoManager::instance()->hasRedoActions());
	}
	else
	{
		(*a_scrActions)["editUndoAction"]->setEnabled(false);
		(*a_scrActions)["editRedoAction"]->setEnabled(false);
	}

	(*a_scrActions)["insertFrame"]->setEnabled(b2);
	(*a_scrActions)["toolsSelect"]->setEnabled(b2);
	(*a_scrActions)["toolsRotate"]->setEnabled(b2);
	(*a_scrActions)["toolsEditContents"]->setEnabled(b2);
	(*a_scrActions)["toolsEditWithStoryEditor"]->setEnabled(b2);
	(*a_scrActions)["toolsZoom"]->setEnabled(b2);
	(*a_scrActions)["toolsInsertTextFrame"]->setEnabled(b2);
	(*a_scrActions)["toolsInsertImageFrame"]->setEnabled(b2);
	(*a_scrActions)["toolsInsertTable"]->setEnabled(b2);
	(*a_scrActions)["toolsInsertShape"]->setEnabled(b2);
	(*a_scrActions)["toolsInsertLine"]->setEnabled(b2);
	(*a_scrActions)["toolsInsertBezier"]->setEnabled(b2);
	(*a_scrActions)["toolsInsertFreehandLine"]->setEnabled(b2);
	(*a_scrActions)["toolsInsertCalligraphicLine"]->setEnabled(b2);
	(*a_scrActions)["toolsInsertPolygon"]->setEnabled(b2);
	(*a_scrActions)["toolsInsertArc"]->setEnabled(b2);
	(*a_scrActions)["toolsInsertSpiral"]->setEnabled(b2);
	(*a_scrActions)["toolsInsertRenderFrame"]->setEnabled(b2);
	(*a_scrActions)["toolsLinkTextFrame"]->setEnabled(b2);
	(*a_scrActions)["toolsUnlinkTextFrame"]->setEnabled(b2);
	(*a_scrActions)["toolsUnlinkTextFrameWithTextCopy"]->setEnabled(b2);
	(*a_scrActions)["toolsUnlinkTextFrameWithTextCut"]->setEnabled(b2);
	(*a_scrActions)["toolsMeasurements"]->setEnabled(b2);
	(*a_scrActions)["toolsCopyProperties"]->setEnabled(b2);
	(*a_scrActions)["toolsEyeDropper"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFPushButton"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFRadioButton"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFTextField"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFCheckBox"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFComboBox"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFListBox"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFAnnotText"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFAnnotLink"]->setEnabled(b2);
#ifdef HAVE_OSG
	(*a_scrActions)["toolsPDFAnnot3D"]->setEnabled(b2);
#endif
	(*a_scrActions)["itemDelete"]->setEnabled(b2);
	(*a_scrActions)["itemLock"]->setEnabled(b2);
	(*a_scrActions)["itemConvertToTextFrame"]->setEnabled(b2);
	(*a_scrActions)["itemConvertToImageFrame"]->setEnabled(b2);
	(*a_scrActions)["itemConvertToPolygon"]->setEnabled(b2);
	(*a_scrActions)["itemConvertToBezierCurve"]->setEnabled(b2);
	(*a_scrActions)["itemConvertToOutlines"]->setEnabled(b2);
	(*a_scrActions)["itemConvertToSymbolFrame"]->setEnabled(b2);
	(*a_scrActions)["pageImport"]->setEnabled(b2);
	(*a_scrActions)["pageCopyToMasterPage"]->setEnabled(b2);
	(*a_scrActions)["itemLowerToBottom"]->setEnabled(b2);
	(*a_scrActions)["itemRaiseToTop"]->setEnabled(b2);
	(*a_scrActions)["itemLower"]->setEnabled(b2);
	(*a_scrActions)["itemRaise"]->setEnabled(b2);

	setActionGroupEnabled(a_scrLayersActions, b2);
}

void AppModeHelper::setSymbolEditMode(bool b, ScribusDoc* doc)
{
	bool b2=!b;
	(*a_scrActions)["pageInsert"]->setEnabled(b2);
	(*a_scrActions)["pageImport"]->setEnabled(b2);
	(*a_scrActions)["pageCopy"]->setEnabled(b2);
	(*a_scrActions)["pageApplyMasterPage"]->setEnabled(b2);
	(*a_scrActions)["pageCopyToMasterPage"]->setEnabled(b2);
	(*a_scrActions)["editMasterPages"]->setEnabled(b2);
	(*a_scrActions)["fileNew"]->setEnabled(b2);
	(*a_scrActions)["fileNewFromTemplate"]->setEnabled(b2);
	(*a_scrActions)["fileOpen"]->setEnabled(b2);
	(*a_scrActions)["fileClose"]->setEnabled(b2);
	(*a_scrActions)["fileSave"]->setEnabled(b2);
	(*a_scrActions)["pageDelete"]->setEnabled(b2);
	(*a_scrActions)["pageMove"]->setEnabled(b2);
	if (b2)
	{
		(*a_scrActions)["fileSave"]->setEnabled(!doc->isConverted);
		bool setter = doc->DocPages.count() > 1 ? true : false;
		(*a_scrActions)["pageDelete"]->setEnabled(setter);
		(*a_scrActions)["pageMove"]->setEnabled(setter);
	}
	(*a_scrActions)["fileRevert"]->setEnabled(b2);
	(*a_scrActions)["fileDocSetup150"]->setEnabled(b2);
	(*a_scrActions)["filePrint"]->setEnabled(b2);
	(*a_scrActions)["fileCollect"]->setEnabled(b2);
	(*a_scrActions)["fileSaveAs"]->setEnabled(b2);
	(*a_scrActions)["fileExportAsEPS"]->setEnabled(b2);
	(*a_scrActions)["fileExportAsPDF"]->setEnabled(b2);
	if ( ScCore->haveGS() || ScCore->isWinGUI() )
		(*a_scrActions)["PrintPreview"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFPushButton"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFRadioButton"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFTextField"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFCheckBox"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFComboBox"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFListBox"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFAnnotText"]->setEnabled(b2);
#ifdef HAVE_OSG
	(*a_scrActions)["toolsPDFAnnot3D"]->setEnabled(b2);
#endif
}

void AppModeHelper::setInlineEditMode(bool b, ScribusDoc *doc)
{
	bool b2=!b;
	(*a_scrActions)["pageInsert"]->setEnabled(b2);
	(*a_scrActions)["pageImport"]->setEnabled(b2);
	(*a_scrActions)["pageDelete"]->setEnabled(b2);
	(*a_scrActions)["pageCopy"]->setEnabled(b2);
	(*a_scrActions)["pageMove"]->setEnabled(b2);
	(*a_scrActions)["pageApplyMasterPage"]->setEnabled(b2);
	(*a_scrActions)["pageCopyToMasterPage"]->setEnabled(b2);
	(*a_scrActions)["editMasterPages"]->setEnabled(b2);
	(*a_scrActions)["fileNew"]->setEnabled(b2);
	(*a_scrActions)["fileNewFromTemplate"]->setEnabled(b2);
	(*a_scrActions)["fileOpen"]->setEnabled(b2);
	(*a_scrActions)["fileSave"]->setEnabled(b2);
	if (b2)
	{
		(*a_scrActions)["fileSave"]->setEnabled(!doc->isConverted);
		if ( ScCore->haveGS() || ScCore->isWinGUI() )
			(*a_scrActions)["PrintPreview"]->setEnabled(true);
		bool setter = doc->DocPages.count() > 1 ? true : false;
		(*a_scrActions)["pageDelete"]->setEnabled(setter);
		(*a_scrActions)["pageMove"]->setEnabled(setter);
	}
	(*a_scrActions)["fileRevert"]->setEnabled(b2);
	(*a_scrActions)["fileDocSetup150"]->setEnabled(b2);
	(*a_scrActions)["filePrint"]->setEnabled(b2);
	(*a_scrActions)["fileCollect"]->setEnabled(b2);
	(*a_scrActions)["fileSaveAs"]->setEnabled(b2);
	(*a_scrActions)["fileExportAsEPS"]->setEnabled(b2);
	(*a_scrActions)["fileExportAsPDF"]->setEnabled(b2);
	if ( ScCore->haveGS() || ScCore->isWinGUI() )
		(*a_scrActions)["PrintPreview"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFPushButton"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFRadioButton"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFTextField"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFCheckBox"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFComboBox"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFListBox"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFAnnotText"]->setEnabled(b2);
#ifdef HAVE_OSG
	(*a_scrActions)["toolsPDFAnnot3D"]->setEnabled(b2);
#endif
}

void AppModeHelper::setMasterPageEditMode(bool b, ScribusDoc* doc)
{
	bool b2=!b;
	(*a_scrActions)["pageInsert"]->setEnabled(b2);
	(*a_scrActions)["pageImport"]->setEnabled(b2);
	(*a_scrActions)["pageDelete"]->setEnabled(b2);
	(*a_scrActions)["pageCopy"]->setEnabled(b2);
	(*a_scrActions)["pageMove"]->setEnabled(b2);
	(*a_scrActions)["pageApplyMasterPage"]->setEnabled(b2);
	(*a_scrActions)["pageCopyToMasterPage"]->setEnabled(b2);
	(*a_scrActions)["editMasterPages"]->setEnabled(b2);
	(*a_scrActions)["fileNew"]->setEnabled(b2);
	(*a_scrActions)["fileNewFromTemplate"]->setEnabled(b2);
	(*a_scrActions)["fileOpen"]->setEnabled(b2);
	(*a_scrActions)["fileClose"]->setEnabled(!b2);
	(*a_scrActions)["fileSave"]->setEnabled(b2);
	(*a_scrActions)["fileExportAsEPS"]->setEnabled(b2);
	(*a_scrActions)["fileExportAsPDF"]->setEnabled(b2);
	if (b2)
	{
		(*a_scrActions)["fileSave"]->setEnabled(!doc->isConverted);
		if ( ScCore->haveGS() || ScCore->isWinGUI() )
			(*a_scrActions)["PrintPreview"]->setEnabled(true);
		bool setter = doc->DocPages.count() > 1 ? true : false;
		(*a_scrActions)["pageDelete"]->setEnabled(setter);
		(*a_scrActions)["pageMove"]->setEnabled(setter);
	}

	(*a_scrActions)["fileRevert"]->setEnabled(b2);
	(*a_scrActions)["fileDocSetup150"]->setEnabled(b2);
	(*a_scrActions)["filePrint"]->setEnabled(b2);
	if ( ScCore->haveGS() || ScCore->isWinGUI() )
		(*a_scrActions)["PrintPreview"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFPushButton"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFRadioButton"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFTextField"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFCheckBox"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFComboBox"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFListBox"]->setEnabled(b2);
	(*a_scrActions)["toolsPDFAnnotText"]->setEnabled(b2);
#ifdef HAVE_OSG
	(*a_scrActions)["toolsPDFAnnot3D"]->setEnabled(b2);
#endif
	//(*a_scrActions)["viewPreviewMode"]->setEnabled(b2);
}

void AppModeHelper::changeLayer(ScribusDoc *doc, bool clipScrapHaveData)
{
	bool setter = !doc->layerLocked( doc->activeLayer() );
	(*a_scrActions)["editPaste"]->setEnabled(clipScrapHaveData && setter);
	(*a_scrActions)["editSelectAll"]->setEnabled(setter);
	(*a_scrActions)["editSelectAllOnLayer"]->setEnabled(setter);
	(*a_scrActions)["editDeselectAll"]->setEnabled(false);
	(*a_scrActions)["insertFrame"]->setEnabled(setter);
	(*a_scrActions)["toolsSelect"]->setEnabled(setter);
	(*a_scrActions)["toolsInsertTextFrame"]->setEnabled(setter);
	(*a_scrActions)["toolsInsertImageFrame"]->setEnabled(setter);
	(*a_scrActions)["toolsInsertTable"]->setEnabled(setter);
	(*a_scrActions)["toolsInsertShape"]->setEnabled(setter);
	(*a_scrActions)["toolsInsertLine"]->setEnabled(setter);
	(*a_scrActions)["toolsInsertBezier"]->setEnabled(setter);
	(*a_scrActions)["toolsInsertFreehandLine"]->setEnabled(setter);
	(*a_scrActions)["toolsInsertCalligraphicLine"]->setEnabled(setter);
	(*a_scrActions)["toolsInsertPolygon"]->setEnabled(setter);
	(*a_scrActions)["toolsInsertArc"]->setEnabled(setter);
	(*a_scrActions)["toolsInsertSpiral"]->setEnabled(setter);
	(*a_scrActions)["toolsInsertRenderFrame"]->setEnabled(setter);
	bool setter2=doc->masterPageMode() ? false : setter;
	(*a_scrActions)["toolsPDFPushButton"]->setEnabled(setter2);
	(*a_scrActions)["toolsPDFRadioButton"]->setEnabled(setter2);
	(*a_scrActions)["toolsPDFTextField"]->setEnabled(setter2);
	(*a_scrActions)["toolsPDFCheckBox"]->setEnabled(setter2);
	(*a_scrActions)["toolsPDFComboBox"]->setEnabled(setter2);
	(*a_scrActions)["toolsPDFListBox"]->setEnabled(setter2);
	(*a_scrActions)["toolsPDFAnnotText"]->setEnabled(setter2);
	(*a_scrActions)["toolsPDFAnnotLink"]->setEnabled(setter);
}

void AppModeHelper::mainWindowHasNewDoc(ScribusDoc *doc, bool clipScrapHaveData)
{
	(*a_scrActions)["filePrint"]->setEnabled(true);
	(*a_scrActions)["fileSave"]->setEnabled(!doc->isConverted);
	(*a_scrActions)["fileClose"]->setEnabled(true);
	(*a_scrActions)["fileDocSetup150"]->setEnabled(true);
	(*a_scrActions)["fileRevert"]->setEnabled(false);
	(*a_scrActions)["fileCollect"]->setEnabled(true);
	(*a_scrActions)["fileSaveAs"]->setEnabled(true);
	(*a_scrActions)["fileExportAsEPS"]->setEnabled(true);
	(*a_scrActions)["fileExportAsPDF"]->setEnabled(true);
	(*a_scrActions)["fileImportVector"]->setEnabled(true);
	(*a_scrActions)["pageImport"]->setEnabled(true);

	if ( ScCore->haveGS() || ScCore->isWinGUI() )
		(*a_scrActions)["PrintPreview"]->setEnabled(true);

	if ((*a_scrActions)["SaveAsDocumentTemplate"])
		(*a_scrActions)["SaveAsDocumentTemplate"]->setEnabled(true);

	(*a_scrActions)["editCut"]->setEnabled(false);
	(*a_scrActions)["editCopy"]->setEnabled(false);
	(*a_scrActions)["editPaste"]->setEnabled(clipScrapHaveData);
	(*a_scrActions)["editCopyContents"]->setEnabled(false);
	(*a_scrActions)["editPasteContents"]->setEnabled(false);
	(*a_scrActions)["editPasteContentsAbs"]->setEnabled(false);
	(*a_scrActions)["editSelectAll"]->setEnabled(true);
	(*a_scrActions)["editSelectAllOnLayer"]->setEnabled(true);
	(*a_scrActions)["editDeselectAll"]->setEnabled(false);
	(*a_scrActions)["editReplaceColors"]->setEnabled(true);
	(*a_scrActions)["editStyles"]->setEnabled(true);
	(*a_scrActions)["editMarks"]->setEnabled(true);
	(*a_scrActions)["editNotesStyles"]->setEnabled(true);
	(*a_scrActions)["editMasterPages"]->setEnabled(true);
	(*a_scrActions)["editJavascripts"]->setEnabled(true);

	(*a_scrActions)["viewFitInWindow"]->setEnabled(true);
	(*a_scrActions)["viewFitWidth"]->setEnabled(true);
	(*a_scrActions)["viewFit50"]->setEnabled(true);
	(*a_scrActions)["viewFit75"]->setEnabled(true);
	(*a_scrActions)["viewFit100"]->setEnabled(true);
	(*a_scrActions)["viewFit200"]->setEnabled(true);
	(*a_scrActions)["viewFit400"]->setEnabled(true);

	(*a_scrActions)["viewSnapToGrid"]->setChecked(doc->SnapGrid);
	(*a_scrActions)["viewSnapToGuides"]->setChecked(doc->SnapGuides);
	(*a_scrActions)["viewSnapToElements"]->setChecked(doc->SnapElement);
	(*a_scrActions)["viewShowRulers"]->setEnabled(true);

	(*a_scrActions)["insertFrame"]->setEnabled(true);

	(*a_scrActions)["toolsSelect"]->setEnabled(true);
	(*a_scrActions)["toolsZoom"]->setEnabled(true);
	(*a_scrActions)["toolsInsertTextFrame"]->setEnabled(true);
	(*a_scrActions)["toolsInsertImageFrame"]->setEnabled(true);
	(*a_scrActions)["toolsInsertTable"]->setEnabled(true);
	(*a_scrActions)["toolsInsertShape"]->setEnabled(true);
	(*a_scrActions)["toolsInsertLine"]->setEnabled(true);
	(*a_scrActions)["toolsInsertBezier"]->setEnabled(true);
	(*a_scrActions)["toolsInsertFreehandLine"]->setEnabled(true);
	(*a_scrActions)["toolsInsertCalligraphicLine"]->setEnabled(true);
	(*a_scrActions)["toolsInsertPolygon"]->setEnabled(true);
	(*a_scrActions)["toolsInsertArc"]->setEnabled(true);
	(*a_scrActions)["toolsInsertSpiral"]->setEnabled(true);
	(*a_scrActions)["toolsInsertRenderFrame"]->setEnabled(true);
	(*a_scrActions)["toolsMeasurements"]->setEnabled(true);
	(*a_scrActions)["toolsEyeDropper"]->setEnabled(true);
	(*a_scrActions)["toolsPDFPushButton"]->setEnabled(true);
	(*a_scrActions)["toolsPDFRadioButton"]->setEnabled(true);
	(*a_scrActions)["toolsPDFTextField"]->setEnabled(true);
	(*a_scrActions)["toolsPDFCheckBox"]->setEnabled(true);
	(*a_scrActions)["toolsPDFComboBox"]->setEnabled(true);
	(*a_scrActions)["toolsPDFListBox"]->setEnabled(true);
	(*a_scrActions)["toolsPDFAnnotText"]->setEnabled(true);
	(*a_scrActions)["toolsPDFAnnotLink"]->setEnabled(true);
#ifdef HAVE_OSG
	(*a_scrActions)["toolsPDFAnnot3D"]->setEnabled(true);
#endif
	(*a_scrActions)["toolsPreflightVerifier"]->setEnabled(true);
	bool setter = doc->DocPages.count() > 1 ? true : false;
	(*a_scrActions)["pageDelete"]->setEnabled(setter);
	(*a_scrActions)["pageMove"]->setEnabled(setter);
	(*a_scrActions)["pageInsert"]->setEnabled(true);
	(*a_scrActions)["pageImport"]->setEnabled(true);
	(*a_scrActions)["pageCopy"]->setEnabled(true);
	(*a_scrActions)["pageApplyMasterPage"]->setEnabled(true);
	(*a_scrActions)["pageCopyToMasterPage"]->setEnabled(true);
	(*a_scrActions)["pageManageGuides"]->setEnabled(true);
	(*a_scrActions)["pageManageMargins"]->setEnabled(true);
}

void AppModeHelper::mainWindowSwitchWin(ScribusDoc *doc)
{
	(*a_scrActions)["fileClose"]->setEnabled(true);
	if (doc->masterPageMode() || doc->symbolEditMode() || doc->inlineEditMode())
	{
		(*a_scrActions)["pageInsert"]->setEnabled(false);
		(*a_scrActions)["pageDelete"]->setEnabled(false);
		(*a_scrActions)["pageCopy"]->setEnabled(false);
		(*a_scrActions)["pageMove"]->setEnabled(false);
		(*a_scrActions)["pageApplyMasterPage"]->setEnabled(false);
		(*a_scrActions)["pageCopyToMasterPage"]->setEnabled(false);
		(*a_scrActions)["editMasterPages"]->setEnabled(false);
		(*a_scrActions)["fileNew"]->setEnabled(false);
		(*a_scrActions)["fileNewFromTemplate"]->setEnabled(false);
		if (doc->symbolEditMode() || doc->inlineEditMode())
		{
			(*a_scrActions)["fileCollect"]->setEnabled(false);
			(*a_scrActions)["fileSaveAs"]->setEnabled(false);
			(*a_scrActions)["fileExportAsEPS"]->setEnabled(false);
			(*a_scrActions)["fileExportAsPDF"]->setEnabled(false);
			(*a_scrActions)["fileSave"]->setEnabled(false);
		}
		else
			(*a_scrActions)["fileSave"]->setEnabled(!doc->isConverted);
		(*a_scrActions)["fileOpen"]->setEnabled(false);
		(*a_scrActions)["fileRevert"]->setEnabled(false);
		(*a_scrActions)["toolsPDFPushButton"]->setEnabled(false);
		(*a_scrActions)["toolsPDFRadioButton"]->setEnabled(false);
		(*a_scrActions)["toolsPDFTextField"]->setEnabled(false);
		(*a_scrActions)["toolsPDFCheckBox"]->setEnabled(false);
		(*a_scrActions)["toolsPDFComboBox"]->setEnabled(false);
		(*a_scrActions)["toolsPDFListBox"]->setEnabled(false);
		(*a_scrActions)["toolsPDFAnnotText"]->setEnabled(false);
#ifdef HAVE_OSG
		(*a_scrActions)["toolsPDFAnnot3D"]->setEnabled(false);
#endif
	}
	else
	{
		(*a_scrActions)["editMasterPages"]->setEnabled(true);
		(*a_scrActions)["fileNew"]->setEnabled(true);
		(*a_scrActions)["fileNewFromTemplate"]->setEnabled(true);
		(*a_scrActions)["fileOpen"]->setEnabled(true);
		(*a_scrActions)["fileClose"]->setEnabled(true);
		(*a_scrActions)["fileSave"]->setEnabled(!doc->isConverted);
		(*a_scrActions)["fileRevert"]->setEnabled(false);

		if (!doc->isModified())
		{
			bool setter = doc->DocPages.count() > 1 ? true : false;
			(*a_scrActions)["pageDelete"]->setEnabled(setter);
			(*a_scrActions)["pageMove"]->setEnabled(setter);
		}
		(*a_scrActions)["pageInsert"]->setEnabled(true);
		(*a_scrActions)["pageImport"]->setEnabled(true);
		(*a_scrActions)["pageCopy"]->setEnabled(true);
		(*a_scrActions)["pageApplyMasterPage"]->setEnabled(true);
		(*a_scrActions)["pageCopyToMasterPage"]->setEnabled(true);
		(*a_scrActions)["pageManageGuides"]->setEnabled(true);
		(*a_scrActions)["pageManageMargins"]->setEnabled(true);

		(*a_scrActions)["fileSaveAs"]->setEnabled(true);
		(*a_scrActions)["fileCollect"]->setEnabled(true);
		(*a_scrActions)["toolsPDFPushButton"]->setEnabled(true);
		(*a_scrActions)["toolsPDFRadioButton"]->setEnabled(true);
		(*a_scrActions)["toolsPDFTextField"]->setEnabled(true);
		(*a_scrActions)["toolsPDFCheckBox"]->setEnabled(true);
		(*a_scrActions)["toolsPDFComboBox"]->setEnabled(true);
		(*a_scrActions)["toolsPDFListBox"]->setEnabled(true);
		(*a_scrActions)["toolsPDFAnnotText"]->setEnabled(true);
#ifdef HAVE_OSG
		(*a_scrActions)["toolsPDFAnnot3D"]->setEnabled(true);
#endif
	}

	(*a_scrActions)["viewSnapToGrid"]->setChecked(doc->SnapGrid);
	(*a_scrActions)["viewSnapToGuides"]->setChecked(doc->SnapGuides);
	(*a_scrActions)["viewSnapToElements"]->setChecked(doc->SnapElement);
}

void AppModeHelper::mainWindowCloseLastDoc()
{
	if ( ScCore->haveGS() || ScCore->isWinGUI() )
		(*a_scrActions)["PrintPreview"]->setEnabled(false);
	if ((*a_scrActions)["SaveAsDocumentTemplate"])
		(*a_scrActions)["SaveAsDocumentTemplate"]->setEnabled(false);
	(*a_scrActions)["editClearContents"]->setEnabled(false);
	(*a_scrActions)["editCopy"]->setEnabled(false);
	(*a_scrActions)["editCut"]->setEnabled(false);
	(*a_scrActions)["editDeselectAll"]->setEnabled(false);
	(*a_scrActions)["editEditRenderSource"]->setEnabled(false);
	(*a_scrActions)["editEditWithImageEditor"]->setEnabled(false);
	(*a_scrActions)["editJavascripts"]->setEnabled(false);
	(*a_scrActions)["editMarks"]->setEnabled(false);
	(*a_scrActions)["editMasterPages"]->setEnabled(false);
	(*a_scrActions)["editNotesStyles"]->setEnabled(false);
	(*a_scrActions)["editPaste"]->setEnabled(false);
	(*a_scrActions)["editRedoAction"]->setEnabled(false);
	(*a_scrActions)["editReplaceColors"]->setEnabled(false);
	(*a_scrActions)["editSearchReplace"]->setEnabled(false);
	(*a_scrActions)["editSelectAll"]->setEnabled(false);
	(*a_scrActions)["editSelectAllOnLayer"]->setEnabled(false);
	(*a_scrActions)["editStyles"]->setEnabled(false);
	(*a_scrActions)["editUndoAction"]->setEnabled(false);
	(*a_scrActions)["extrasDeHyphenateText"]->setEnabled(false);
	(*a_scrActions)["extrasHyphenateText"]->setEnabled(false);
	(*a_scrActions)["fileClose"]->setEnabled(false);
	(*a_scrActions)["fileCollect"]->setEnabled(false);
	(*a_scrActions)["fileDocSetup150"]->setEnabled(false);
	(*a_scrActions)["fileExportAsEPS"]->setEnabled(false);
	(*a_scrActions)["fileExportAsPDF"]->setEnabled(false);
	(*a_scrActions)["fileExportText"]->setEnabled(false);
	(*a_scrActions)["fileImportAppendText"]->setEnabled(false);
	(*a_scrActions)["fileImportImage"]->setEnabled(false);
	(*a_scrActions)["fileImportText"]->setEnabled(false);
	(*a_scrActions)["fileImportText2"]->setEnabled(false);
	(*a_scrActions)["fileImportVector"]->setEnabled(false);
	(*a_scrActions)["filePrint"]->setEnabled(false);
	(*a_scrActions)["fileRevert"]->setEnabled(false);
	(*a_scrActions)["fileSave"]->setEnabled(false);
	(*a_scrActions)["fileSaveAs"]->setEnabled(false);
	(*a_scrActions)["insertFrame"]->setEnabled(false);
	(*a_scrActions)["insertSampleText"]->setEnabled(false);
	(*a_scrActions)["itemAdjustFrameToImage"]->setEnabled(false);
	(*a_scrActions)["itemAdjustImageToFrame"]->setEnabled(false);
	(*a_scrActions)["itemAttributes"]->setEnabled(false);
	(*a_scrActions)["itemDelete"]->setEnabled(false);
	(*a_scrActions)["itemDuplicate"]->setEnabled(false);
	(*a_scrActions)["itemExtendedImageProperties"]->setEnabled(false);
	(*a_scrActions)["itemLower"]->setEnabled(false);
	(*a_scrActions)["itemLowerToBottom"]->setEnabled(false);
	(*a_scrActions)["itemMulDuplicate"]->setEnabled(false);
	(*a_scrActions)["itemPDFAnnotationProps"]->setEnabled(false);
	(*a_scrActions)["itemPDFFieldProps"]->setEnabled(false);
	(*a_scrActions)["itemPreviewFull"]->setEnabled(false);
	(*a_scrActions)["itemPreviewLow"]->setEnabled(false);
	(*a_scrActions)["itemPreviewNormal"]->setEnabled(false);
	(*a_scrActions)["itemRaise"]->setEnabled(false);
	(*a_scrActions)["itemRaiseToTop"]->setEnabled(false);
	(*a_scrActions)["itemSendToInline"]->setEnabled(false);
	(*a_scrActions)["itemSendToPattern"]->setEnabled(false);
	(*a_scrActions)["itemToggleInlineImage"]->setEnabled(false);
	(*a_scrActions)["itemTransform"]->setEnabled(false);
	(*a_scrActions)["itemUpdateImage"]->setEnabled(false);
	(*a_scrActions)["pageApplyMasterPage"]->setEnabled(false);
	(*a_scrActions)["pageCopy"]->setEnabled(false);
	(*a_scrActions)["pageCopyToMasterPage"]->setEnabled(false);
	(*a_scrActions)["pageDelete"]->setEnabled(false);
	(*a_scrActions)["pageImport"]->setEnabled(false);
	(*a_scrActions)["pageImport"]->setEnabled(false);
	(*a_scrActions)["pageInsert"]->setEnabled(false);
	(*a_scrActions)["pageManageGuides"]->setEnabled(false);
	(*a_scrActions)["pageManageMargins"]->setEnabled(false);
	(*a_scrActions)["pageMove"]->setEnabled(false);
	(*a_scrActions)["toolsCopyProperties"]->setEnabled(false);
	(*a_scrActions)["toolsEditContents"]->setEnabled(false);
	(*a_scrActions)["toolsEditWithStoryEditor"]->setEnabled(false);
	(*a_scrActions)["toolsEyeDropper"]->setEnabled(false);
	(*a_scrActions)["toolsInsertArc"]->setEnabled(false);
	(*a_scrActions)["toolsInsertBezier"]->setEnabled(false);
	(*a_scrActions)["toolsInsertCalligraphicLine"]->setEnabled(false);
	(*a_scrActions)["toolsInsertFreehandLine"]->setEnabled(false);
	(*a_scrActions)["toolsInsertImageFrame"]->setEnabled(false);
	(*a_scrActions)["toolsInsertLine"]->setEnabled(false);
	(*a_scrActions)["toolsInsertPolygon"]->setEnabled(false);
	(*a_scrActions)["toolsInsertRenderFrame"]->setEnabled(false);
	(*a_scrActions)["toolsInsertShape"]->setEnabled(false);
	(*a_scrActions)["toolsInsertSpiral"]->setEnabled(false);
	(*a_scrActions)["toolsInsertTable"]->setEnabled(false);
	(*a_scrActions)["toolsInsertTextFrame"]->setEnabled(false);
	(*a_scrActions)["toolsLinkTextFrame"]->setEnabled(false);
	(*a_scrActions)["toolsMeasurements"]->setEnabled(false);
	(*a_scrActions)["toolsPDFAnnotLink"]->setEnabled(false);
	(*a_scrActions)["toolsPDFAnnotText"]->setEnabled(false);
	(*a_scrActions)["toolsPDFCheckBox"]->setEnabled(false);
	(*a_scrActions)["toolsPDFComboBox"]->setEnabled(false);
	(*a_scrActions)["toolsPDFListBox"]->setEnabled(false);
	(*a_scrActions)["toolsPDFPushButton"]->setEnabled(false);
	(*a_scrActions)["toolsPDFRadioButton"]->setEnabled(false);
	(*a_scrActions)["toolsPDFTextField"]->setEnabled(false);
	(*a_scrActions)["toolsPreflightVerifier"]->setEnabled(false);
	(*a_scrActions)["toolsRotate"]->setEnabled(false);
	(*a_scrActions)["toolsSelect"]->setEnabled(false);
	(*a_scrActions)["toolsUnlinkTextFrame"]->setEnabled(false);
	(*a_scrActions)["toolsUnlinkTextFrameWithTextCopy"]->setEnabled(false);
	(*a_scrActions)["toolsUnlinkTextFrameWithTextCut"]->setEnabled(false);
	(*a_scrActions)["toolsZoom"]->setEnabled(false);
	(*a_scrActions)["viewFit100"]->setEnabled(false);
	(*a_scrActions)["viewFit200"]->setEnabled(false);
	(*a_scrActions)["viewFit400"]->setEnabled(false);
	(*a_scrActions)["viewFit50"]->setEnabled(false);
	(*a_scrActions)["viewFit75"]->setEnabled(false);
	(*a_scrActions)["viewFitInWindow"]->setEnabled(false);
	(*a_scrActions)["viewFitWidth"]->setEnabled(false);
	(*a_scrActions)["viewShowRulers"]->setEnabled(false);
	(*a_scrActions)["viewSnapToElements"]->setChecked(false);
	(*a_scrActions)["viewSnapToGrid"]->setChecked(false);
	(*a_scrActions)["viewSnapToGuides"]->setChecked(false);

#ifdef HAVE_OSG
	(*a_scrActions)["toolsPDFAnnot3D"]->setEnabled(false);
#endif
	(*a_scrActions)["itemConvertToBezierCurve"]->setEnabled(false);
	(*a_scrActions)["itemConvertToImageFrame"]->setEnabled(false);
	(*a_scrActions)["itemConvertToOutlines"]->setEnabled(false);
	(*a_scrActions)["itemConvertToPolygon"]->setEnabled(false);
	(*a_scrActions)["itemConvertToTextFrame"]->setEnabled(false);
	(*a_scrActions)["itemConvertToSymbolFrame"]->setEnabled(false);
	(*a_scrActions)["itemLock"]->setEnabled(false);
	(*a_scrActions)["itemLockSize"]->setEnabled(false);

	(*a_scrActions)["tableInsertRows"]->setEnabled(false);
	(*a_scrActions)["tableInsertColumns"]->setEnabled(false);
	(*a_scrActions)["tableDeleteRows"]->setEnabled(false);
	(*a_scrActions)["tableDeleteColumns"]->setEnabled(false);
	(*a_scrActions)["tableMergeCells"]->setEnabled(false);
	(*a_scrActions)["tableSplitCells"]->setEnabled(false); // Not implemented.
	(*a_scrActions)["tableSetRowHeights"]->setEnabled(false);
	(*a_scrActions)["tableSetColumnWidths"]->setEnabled(false);
	(*a_scrActions)["tableDistributeRowsEvenly"]->setEnabled(false);
	(*a_scrActions)["tableDistributeColumnsEvenly"]->setEnabled(false);
	(*a_scrActions)["tableDistributeRowsEvenly"]->setEnabled(false);
	(*a_scrActions)["tableDistributeColumnsEvenly"]->setEnabled(false);
	(*a_scrActions)["tableAdjustFrameToTable"]->setEnabled(false);
	(*a_scrActions)["tableAdjustTableToFrame"]->setEnabled(false);
}

void AppModeHelper::setPreviewMode(bool b)
{
	bool b2=!b;
	(*a_scrActions)["viewShowMargins"]->setEnabled(b2);
	(*a_scrActions)["viewShowFrames"]->setEnabled(b2);
	(*a_scrActions)["viewShowLayerMarkers"]->setEnabled(b2);
	(*a_scrActions)["viewShowGrid"]->setEnabled(b2);
	(*a_scrActions)["viewShowGuides"]->setEnabled(b2);
	(*a_scrActions)["viewShowColumnBorders"]->setEnabled(b2);
	(*a_scrActions)["viewShowBaseline"]->setEnabled(b2);
	(*a_scrActions)["viewShowTextChain"]->setEnabled(b2);
	(*a_scrActions)["viewShowTextControls"]->setEnabled(b2);
}

void AppModeHelper::enableTextActions(bool enabled, const QString& fontName)
{
	(*a_scrActions)["insertGlyph"]->setEnabled(enabled);
	a_actMgr->enableUnicodeActions(a_scrActions, enabled, fontName);
	if (!enabled)
	{
		(*a_scrActions)["insertMarkVariableText"]->setEnabled(false);
		(*a_scrActions)["insertMarkAnchor"]->setEnabled(false);
		(*a_scrActions)["insertMarkItem"]->setEnabled(false);
		(*a_scrActions)["insertMark2Mark"]->setEnabled(false);
		(*a_scrActions)["insertMarkNote"]->setEnabled(false);
		(*a_scrActions)["editMark"]->setEnabled(false);
	}
}

void AppModeHelper::setStartupActionsEnabled(bool enabled)
{
	(*a_scrActions)["fileDocSetup150"]->setEnabled(false);
	(*a_scrActions)["filePrint"]->setEnabled(false);
	(*a_scrActions)["fileSave"]->setEnabled(false);
	(*a_scrActions)["fileSaveAs"]->setEnabled(false);
	(*a_scrActions)["fileRevert"]->setEnabled(false);
	(*a_scrActions)["fileCollect"]->setEnabled(false);
	(*a_scrActions)["fileClose"]->setEnabled(false);
	(*a_scrActions)["PrintPreview"]->setEnabled(false);
	(*a_scrActions)["SaveAsDocumentTemplate"]->setEnabled(false);
//	scrMenuMgr->setMenuEnabled("FileImport", false);
//	scrMenuMgr->setMenuEnabled("FileExport", false);
	(*a_scrActions)["fileExportAsPDF"]->setEnabled(false);
	(*a_scrActions)["fileExportText"]->setEnabled(false);
	(*a_scrActions)["fileExportAsEPS"]->setEnabled(false);
	(*a_scrActions)["fileImportText"]->setEnabled(false);
	(*a_scrActions)["fileImportText2"]->setEnabled(false);
	(*a_scrActions)["fileImportImage"]->setEnabled(false);
	(*a_scrActions)["fileImportAppendText"]->setEnabled(false);
	(*a_scrActions)["fileImportVector"]->setEnabled(false);
	(*a_scrActions)["pageInsert"]->setEnabled(false);
	(*a_scrActions)["pageImport"]->setEnabled(false);
	(*a_scrActions)["pageDelete"]->setEnabled(false);
	(*a_scrActions)["pageImport"]->setEnabled(false);
	(*a_scrActions)["pageMove"]->setEnabled(false);
	(*a_scrActions)["pageCopy"]->setEnabled(false);
	(*a_scrActions)["pageApplyMasterPage"]->setEnabled(false);
	(*a_scrActions)["pageCopyToMasterPage"]->setEnabled(false);
	(*a_scrActions)["pageManageGuides"]->setEnabled(false);
	(*a_scrActions)["pageManageMargins"]->setEnabled(false);
	(*a_scrActions)["editUndoAction"]->setEnabled(false);
	(*a_scrActions)["editRedoAction"]->setEnabled(false);
	(*a_scrActions)["editCut"]->setEnabled(false);
	(*a_scrActions)["editCopy"]->setEnabled(false);
	(*a_scrActions)["editPaste"]->setEnabled(false);
//	scrMenuMgr->setMenuEnabled("EditPasteRecent", false);
	(*a_scrActions)["editClearContents"]->setEnabled(false);
	(*a_scrActions)["editTruncateContents"]->setEnabled(false);
	(*a_scrActions)["editSelectAll"]->setEnabled(false);
	(*a_scrActions)["editSelectAllOnLayer"]->setEnabled(false);
	(*a_scrActions)["editDeselectAll"]->setEnabled(false);
	(*a_scrActions)["editReplaceColors"]->setEnabled(false);
	(*a_scrActions)["editStyles"]->setEnabled(false);
	(*a_scrActions)["editMarks"]->setEnabled(false);
	(*a_scrActions)["editNotesStyles"]->setEnabled(false);
	(*a_scrActions)["editSearchReplace"]->setEnabled(false);
	(*a_scrActions)["editMasterPages"]->setEnabled(false);
	(*a_scrActions)["editJavascripts"]->setEnabled(false);
	(*a_scrActions)["editEditWithImageEditor"]->setEnabled(false);
	(*a_scrActions)["editEditRenderSource"]->setEnabled(false);
	(*a_scrActions)["toolsPreflightVerifier"]->setEnabled(false);
	(*a_scrActions)["extrasHyphenateText"]->setEnabled(false);
	(*a_scrActions)["extrasDeHyphenateText"]->setEnabled(false);
	(*a_scrActions)["viewFitInWindow"]->setEnabled(false);
	(*a_scrActions)["viewFitWidth"]->setEnabled(false);
	(*a_scrActions)["viewFit50"]->setEnabled(false);
	(*a_scrActions)["viewFit75"]->setEnabled(false);
	(*a_scrActions)["viewFit100"]->setEnabled(false);
	(*a_scrActions)["viewFit200"]->setEnabled(false);
	(*a_scrActions)["viewFit400"]->setEnabled(false);
	(*a_scrActions)["viewSnapToGuides"]->setChecked(false);
	(*a_scrActions)["viewSnapToElements"]->setChecked(false);
	(*a_scrActions)["viewSnapToGrid"]->setChecked(false);
	(*a_scrActions)["viewShowRulers"]->setEnabled(false);
//	scrMenuMgr->setMenuEnabled("Insert", false);
	(*a_scrActions)["insertFrame"]->setEnabled(false);
	(*a_scrActions)["insertSampleText"]->setEnabled(false);
	(*a_scrActions)["itemDuplicate"]->setEnabled(false);
	(*a_scrActions)["itemMulDuplicate"]->setEnabled(false);
	(*a_scrActions)["itemTransform"]->setEnabled(false);
	(*a_scrActions)["itemDelete"]->setEnabled(false);
	(*a_scrActions)["itemRaise"]->setEnabled(false);
	(*a_scrActions)["itemLower"]->setEnabled(false);
	(*a_scrActions)["itemRaiseToTop"]->setEnabled(false);
	(*a_scrActions)["itemLowerToBottom"]->setEnabled(false);
//	scrMenuMgr->setMenuEnabled("itemSendToScrapbook", false);
	(*a_scrActions)["itemSendToPattern"]->setEnabled(false);
	(*a_scrActions)["itemSendToInline"]->setEnabled(false);
	(*a_scrActions)["itemAdjustFrameToImage"]->setEnabled(false);
	(*a_scrActions)["itemAdjustImageToFrame"]->setEnabled(false);
	(*a_scrActions)["itemExtendedImageProperties"]->setEnabled(false);
	(*a_scrActions)["itemUpdateImage"]->setEnabled(false);
	(*a_scrActions)["itemPreviewFull"]->setEnabled(false);
	(*a_scrActions)["itemPreviewNormal"]->setEnabled(false);
	(*a_scrActions)["itemPreviewLow"]->setEnabled(false);
	(*a_scrActions)["itemAttributes"]->setEnabled(false);
	(*a_scrActions)["itemPDFAnnotationProps"]->setEnabled(false);
	(*a_scrActions)["itemPDFFieldProps"]->setEnabled(false);
	(*a_scrActions)["itemConvertToBezierCurve"]->setEnabled(false);
	(*a_scrActions)["itemConvertToImageFrame"]->setEnabled(false);
	(*a_scrActions)["itemConvertToOutlines"]->setEnabled(false);
	(*a_scrActions)["itemConvertToPolygon"]->setEnabled(false);
	(*a_scrActions)["itemConvertToTextFrame"]->setEnabled(false);
	(*a_scrActions)["itemConvertToSymbolFrame"]->setEnabled(false);
	(*a_scrActions)["toolsSelect"]->setEnabled(false);
	(*a_scrActions)["toolsRotate"]->setEnabled(false);
	(*a_scrActions)["toolsEditContents"]->setEnabled(false);
	(*a_scrActions)["toolsEditWithStoryEditor"]->setEnabled(false);
	(*a_scrActions)["toolsZoom"]->setEnabled(false);
	(*a_scrActions)["toolsInsertTextFrame"]->setEnabled(false);
	(*a_scrActions)["toolsInsertImageFrame"]->setEnabled(false);
	(*a_scrActions)["toolsInsertShape"]->setEnabled(false);
	(*a_scrActions)["toolsInsertLine"]->setEnabled(false);
	(*a_scrActions)["toolsInsertBezier"]->setEnabled(false);
	(*a_scrActions)["toolsInsertFreehandLine"]->setEnabled(false);
	(*a_scrActions)["toolsInsertCalligraphicLine"]->setEnabled(false);
	(*a_scrActions)["toolsInsertPolygon"]->setEnabled(false);
	(*a_scrActions)["toolsInsertArc"]->setEnabled(false);
	(*a_scrActions)["toolsInsertSpiral"]->setEnabled(false);
	(*a_scrActions)["toolsInsertRenderFrame"]->setEnabled(false);
	(*a_scrActions)["toolsInsertTable"]->setEnabled(false);
	(*a_scrActions)["toolsLinkTextFrame"]->setEnabled(false);
	(*a_scrActions)["toolsUnlinkTextFrame"]->setEnabled(false);
	(*a_scrActions)["toolsUnlinkTextFrameWithTextCopy"]->setEnabled(false);
	(*a_scrActions)["toolsUnlinkTextFrameWithTextCut"]->setEnabled(false);
	(*a_scrActions)["toolsMeasurements"]->setEnabled(false);
	(*a_scrActions)["toolsCopyProperties"]->setEnabled(false);
	(*a_scrActions)["toolsEyeDropper"]->setEnabled(false);
	(*a_scrActions)["toolsPDFPushButton"]->setEnabled(false);
	(*a_scrActions)["toolsPDFRadioButton"]->setEnabled(false);
	(*a_scrActions)["toolsPDFTextField"]->setEnabled(false);
	(*a_scrActions)["toolsPDFCheckBox"]->setEnabled(false);
	(*a_scrActions)["toolsPDFComboBox"]->setEnabled(false);
	(*a_scrActions)["toolsPDFListBox"]->setEnabled(false);
	(*a_scrActions)["toolsPDFAnnotText"]->setEnabled(false);
	(*a_scrActions)["toolsPDFAnnotLink"]->setEnabled(false);
#ifdef HAVE_OSG
	(*a_scrActions)["toolsPDFAnnot3D"]->setEnabled(false);
#endif

	(*a_scrActions)["tableInsertRows"]->setEnabled(false);
	(*a_scrActions)["tableInsertColumns"]->setEnabled(false);
	(*a_scrActions)["tableDeleteRows"]->setEnabled(false);
	(*a_scrActions)["tableDeleteColumns"]->setEnabled(false);
	(*a_scrActions)["tableMergeCells"]->setEnabled(false);
	(*a_scrActions)["tableSplitCells"]->setEnabled(false); // Not implemented.
	(*a_scrActions)["tableSetRowHeights"]->setEnabled(false);
	(*a_scrActions)["tableSetColumnWidths"]->setEnabled(false);
	(*a_scrActions)["tableDistributeRowsEvenly"]->setEnabled(false);
	(*a_scrActions)["tableDistributeColumnsEvenly"]->setEnabled(false);
	(*a_scrActions)["tableDistributeRowsEvenly"]->setEnabled(false);
	(*a_scrActions)["tableDistributeColumnsEvenly"]->setEnabled(false);
	(*a_scrActions)["tableAdjustFrameToTable"]->setEnabled(false);
	(*a_scrActions)["tableAdjustTableToFrame"]->setEnabled(false);
}

