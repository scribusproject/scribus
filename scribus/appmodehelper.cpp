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
#include "scribusdoc.h"
#include "scribuscore.h"

AppModeHelper::AppModeHelper(QObject *parent) :
    QObject(parent)
{
	scrActions=NULL;
}

void AppModeHelper::setScrActions(QMap<QString, QPointer<ScrAction> > *scra)
{
	scrActions=scra;
}

void AppModeHelper::setFrameEditMode(bool b)
{
	bool b2=!b;
	(*scrActions)["toolsSelect"]->setEnabled(b2);
	(*scrActions)["toolsRotate"]->setEnabled(b2);
	(*scrActions)["toolsEditContents"]->setEnabled(b2);
	(*scrActions)["toolsEditWithStoryEditor"]->setEnabled(b2);
	(*scrActions)["toolsZoom"]->setEnabled(b2);
	(*scrActions)["toolsInsertTextFrame"]->setEnabled(b2);
	(*scrActions)["toolsInsertImageFrame"]->setEnabled(b2);
	(*scrActions)["toolsInsertTable"]->setEnabled(b2);
	(*scrActions)["toolsInsertShape"]->setEnabled(b2);
	(*scrActions)["toolsInsertLine"]->setEnabled(b2);
	(*scrActions)["toolsInsertBezier"]->setEnabled(b2);
	(*scrActions)["toolsInsertFreehandLine"]->setEnabled(b2);
	(*scrActions)["toolsInsertCalligraphicLine"]->setEnabled(b2);
	(*scrActions)["toolsInsertPolygon"]->setEnabled(b2);
	(*scrActions)["toolsInsertArc"]->setEnabled(b2);
	(*scrActions)["toolsInsertSpiral"]->setEnabled(b2);
	(*scrActions)["toolsInsertRenderFrame"]->setEnabled(b2);
	(*scrActions)["toolsLinkTextFrame"]->setEnabled(b2);
	(*scrActions)["toolsUnlinkTextFrame"]->setEnabled(b2);
	(*scrActions)["toolsUnlinkTextFrameWithTextCopy"]->setEnabled(b2);
	(*scrActions)["toolsUnlinkTextFrameWithTextCut"]->setEnabled(b2);
	(*scrActions)["toolsMeasurements"]->setEnabled(b2);
	(*scrActions)["toolsCopyProperties"]->setEnabled(b2);
	(*scrActions)["toolsEyeDropper"]->setEnabled(b2);
	(*scrActions)["toolsPDFPushButton"]->setEnabled(b2);
	(*scrActions)["toolsPDFRadioButton"]->setEnabled(b2);
	(*scrActions)["toolsPDFTextField"]->setEnabled(b2);
	(*scrActions)["toolsPDFCheckBox"]->setEnabled(b2);
	(*scrActions)["toolsPDFComboBox"]->setEnabled(b2);
	(*scrActions)["toolsPDFListBox"]->setEnabled(b2);
	(*scrActions)["toolsPDFAnnotText"]->setEnabled(b2);
	(*scrActions)["toolsPDFAnnotLink"]->setEnabled(b2);
#ifdef HAVE_OSG
	(*scrActions)["toolsPDFAnnot3D"]->setEnabled(b2);
#endif
	(*scrActions)["itemDelete"]->setEnabled(b2);
}

void AppModeHelper::setSymbolEditMode(bool b, ScribusDoc* doc)
{
	bool b2=!b;
	(*scrActions)["pageInsert"]->setEnabled(b2);
	(*scrActions)["pageImport"]->setEnabled(b2);
	(*scrActions)["pageCopy"]->setEnabled(b2);
	(*scrActions)["pageApplyMasterPage"]->setEnabled(b2);
	(*scrActions)["pageCopyToMasterPage"]->setEnabled(b2);
	(*scrActions)["editMasterPages"]->setEnabled(b2);
	(*scrActions)["fileNew"]->setEnabled(b2);
	(*scrActions)["fileNewFromTemplate"]->setEnabled(b2);
	(*scrActions)["fileOpen"]->setEnabled(b2);
	(*scrActions)["fileClose"]->setEnabled(b2);
	(*scrActions)["fileSave"]->setEnabled(b2);
	(*scrActions)["pageDelete"]->setEnabled(b2);
	(*scrActions)["pageMove"]->setEnabled(b2);
	if (b2)
	{
		(*scrActions)["fileSave"]->setEnabled(!doc->isConverted);
		bool setter = doc->DocPages.count() > 1 ? true : false;
		(*scrActions)["pageDelete"]->setEnabled(setter);
		(*scrActions)["pageMove"]->setEnabled(setter);
	}
	(*scrActions)["fileRevert"]->setEnabled(b2);
	(*scrActions)["fileDocSetup150"]->setEnabled(b2);
	(*scrActions)["filePrint"]->setEnabled(b2);
	(*scrActions)["fileCollect"]->setEnabled(b2);
	(*scrActions)["fileSaveAs"]->setEnabled(b2);
	(*scrActions)["fileExportAsEPS"]->setEnabled(b2);
	(*scrActions)["fileExportAsPDF"]->setEnabled(b2);
	(*scrActions)["PrintPreview"]->setEnabled(b2);
	(*scrActions)["toolsPDFPushButton"]->setEnabled(b2);
	(*scrActions)["toolsPDFRadioButton"]->setEnabled(b2);
	(*scrActions)["toolsPDFTextField"]->setEnabled(b2);
	(*scrActions)["toolsPDFCheckBox"]->setEnabled(b2);
	(*scrActions)["toolsPDFComboBox"]->setEnabled(b2);
	(*scrActions)["toolsPDFListBox"]->setEnabled(b2);
	(*scrActions)["toolsPDFAnnotText"]->setEnabled(b2);
#ifdef HAVE_OSG
	(*scrActions)["toolsPDFAnnot3D"]->setEnabled(b2);
#endif
}

void AppModeHelper::setInlineEditMode(bool b, ScribusDoc *doc)
{
	bool b2=!b;
	(*scrActions)["pageInsert"]->setEnabled(b2);
	(*scrActions)["pageImport"]->setEnabled(b2);
	(*scrActions)["pageDelete"]->setEnabled(b2);
	(*scrActions)["pageCopy"]->setEnabled(b2);
	(*scrActions)["pageMove"]->setEnabled(b2);
	(*scrActions)["pageApplyMasterPage"]->setEnabled(b2);
	(*scrActions)["pageCopyToMasterPage"]->setEnabled(b2);
	(*scrActions)["editMasterPages"]->setEnabled(b2);
	(*scrActions)["fileNew"]->setEnabled(b2);
	(*scrActions)["fileNewFromTemplate"]->setEnabled(b2);
	(*scrActions)["fileOpen"]->setEnabled(b2);
	(*scrActions)["fileSave"]->setEnabled(b2);
	if (b2)
	{
		(*scrActions)["fileSave"]->setEnabled(!doc->isConverted);
		if ( ScCore->haveGS() || ScCore->isWinGUI() )
			(*scrActions)["PrintPreview"]->setEnabled(true);
		bool setter = doc->DocPages.count() > 1 ? true : false;
		(*scrActions)["pageDelete"]->setEnabled(setter);
		(*scrActions)["pageMove"]->setEnabled(setter);
	}
	(*scrActions)["fileRevert"]->setEnabled(b2);
	(*scrActions)["fileDocSetup150"]->setEnabled(b2);
	(*scrActions)["filePrint"]->setEnabled(b2);
	(*scrActions)["fileCollect"]->setEnabled(b2);
	(*scrActions)["fileSaveAs"]->setEnabled(b2);
	(*scrActions)["fileExportAsEPS"]->setEnabled(b2);
	(*scrActions)["fileExportAsPDF"]->setEnabled(b2);
	(*scrActions)["PrintPreview"]->setEnabled(b2);
	(*scrActions)["toolsPDFPushButton"]->setEnabled(b2);
	(*scrActions)["toolsPDFRadioButton"]->setEnabled(b2);
	(*scrActions)["toolsPDFTextField"]->setEnabled(b2);
	(*scrActions)["toolsPDFCheckBox"]->setEnabled(b2);
	(*scrActions)["toolsPDFComboBox"]->setEnabled(b2);
	(*scrActions)["toolsPDFListBox"]->setEnabled(b2);
	(*scrActions)["toolsPDFAnnotText"]->setEnabled(b2);
#ifdef HAVE_OSG
	(*scrActions)["toolsPDFAnnot3D"]->setEnabled(b2);
#endif
}

void AppModeHelper::setMasterPageEditMode(bool b, ScribusDoc* doc)
{
	bool b2=!b;
	(*scrActions)["pageInsert"]->setEnabled(b2);
	(*scrActions)["pageImport"]->setEnabled(b2);
	(*scrActions)["pageDelete"]->setEnabled(b2);
	(*scrActions)["pageCopy"]->setEnabled(b2);
	(*scrActions)["pageMove"]->setEnabled(b2);
	(*scrActions)["pageApplyMasterPage"]->setEnabled(b2);
	(*scrActions)["pageCopyToMasterPage"]->setEnabled(b2);
	(*scrActions)["editMasterPages"]->setEnabled(b2);
	(*scrActions)["fileNew"]->setEnabled(b2);
	(*scrActions)["fileNewFromTemplate"]->setEnabled(b2);
	(*scrActions)["fileOpen"]->setEnabled(b2);
	(*scrActions)["fileClose"]->setEnabled(!b2);
	(*scrActions)["fileSave"]->setEnabled(b2);
	if (b2)
	{
		(*scrActions)["fileSave"]->setEnabled(!doc->isConverted);
		if ( ScCore->haveGS() || ScCore->isWinGUI() )
			(*scrActions)["PrintPreview"]->setEnabled(true);
		bool setter = doc->DocPages.count() > 1 ? true : false;
		(*scrActions)["pageDelete"]->setEnabled(setter);
		(*scrActions)["pageMove"]->setEnabled(setter);
	}

	(*scrActions)["fileRevert"]->setEnabled(b2);
	(*scrActions)["fileDocSetup150"]->setEnabled(b2);
	(*scrActions)["filePrint"]->setEnabled(b2);
	(*scrActions)["PrintPreview"]->setEnabled(b2);
	(*scrActions)["toolsPDFPushButton"]->setEnabled(b2);
	(*scrActions)["toolsPDFRadioButton"]->setEnabled(b2);
	(*scrActions)["toolsPDFTextField"]->setEnabled(b2);
	(*scrActions)["toolsPDFCheckBox"]->setEnabled(b2);
	(*scrActions)["toolsPDFComboBox"]->setEnabled(b2);
	(*scrActions)["toolsPDFListBox"]->setEnabled(b2);
	(*scrActions)["toolsPDFAnnotText"]->setEnabled(b2);
#ifdef HAVE_OSG
	(*scrActions)["toolsPDFAnnot3D"]->setEnabled(b2);
#endif
	(*scrActions)["viewPreviewMode"]->setEnabled(b2);
}

void AppModeHelper::changeLayer(ScribusDoc *doc, bool clipScrapHaveData)
{
	bool setter = !doc->layerLocked( doc->activeLayer() );
	(*scrActions)["editPaste"]->setEnabled(clipScrapHaveData && setter);
	(*scrActions)["editSelectAll"]->setEnabled(setter);
	(*scrActions)["editSelectAllOnLayer"]->setEnabled(setter);
	(*scrActions)["editDeselectAll"]->setEnabled(false);
	(*scrActions)["insertFrame"]->setEnabled(setter);
	(*scrActions)["toolsSelect"]->setEnabled(setter);
	(*scrActions)["toolsInsertTextFrame"]->setEnabled(setter);
	(*scrActions)["toolsInsertImageFrame"]->setEnabled(setter);
	(*scrActions)["toolsInsertTable"]->setEnabled(setter);
	(*scrActions)["toolsInsertShape"]->setEnabled(setter);
	(*scrActions)["toolsInsertLine"]->setEnabled(setter);
	(*scrActions)["toolsInsertBezier"]->setEnabled(setter);
	(*scrActions)["toolsInsertFreehandLine"]->setEnabled(setter);
	(*scrActions)["toolsInsertCalligraphicLine"]->setEnabled(setter);
	(*scrActions)["toolsInsertPolygon"]->setEnabled(setter);
	(*scrActions)["toolsInsertArc"]->setEnabled(setter);
	(*scrActions)["toolsInsertSpiral"]->setEnabled(setter);
	(*scrActions)["toolsInsertRenderFrame"]->setEnabled(setter);
	bool setter2=doc->masterPageMode() ? false : setter;
	(*scrActions)["toolsPDFPushButton"]->setEnabled(setter2);
	(*scrActions)["toolsPDFRadioButton"]->setEnabled(setter2);
	(*scrActions)["toolsPDFTextField"]->setEnabled(setter2);
	(*scrActions)["toolsPDFCheckBox"]->setEnabled(setter2);
	(*scrActions)["toolsPDFComboBox"]->setEnabled(setter2);
	(*scrActions)["toolsPDFListBox"]->setEnabled(setter2);
	(*scrActions)["toolsPDFAnnotText"]->setEnabled(setter2);
	(*scrActions)["toolsPDFAnnotLink"]->setEnabled(setter);
}
