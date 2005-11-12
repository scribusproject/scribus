/***************************************************************************
	begin                : 2005
	copyright            : (C) 2005 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
	copyright            : (C) 2005 by Craig Bradney
	email                : cbradney@zip.com.au
***************************************************************************/

/***************************************************************************
*                                                                         *
*   ScApp program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "documentchecker.h"
#include "pageitem.h"
#include "scribusstructs.h"
#include "scribusdoc.h"
#include "page.h"

void DocumentChecker::checkDocument(ScribusDoc *currDoc)
{
	PageItem* currItem;
	QString chx;
	struct checkerPrefs checkerSettings;
	checkerSettings.ignoreErrors = currDoc->checkerProfiles[currDoc->curCheckProfile].ignoreErrors;
	checkerSettings.autoCheck = currDoc->checkerProfiles[currDoc->curCheckProfile].autoCheck;
	checkerSettings.checkGlyphs = currDoc->checkerProfiles[currDoc->curCheckProfile].checkGlyphs;
	checkerSettings.checkOrphans = currDoc->checkerProfiles[currDoc->curCheckProfile].checkOrphans;
	checkerSettings.checkOverflow = currDoc->checkerProfiles[currDoc->curCheckProfile].checkOverflow;
	checkerSettings.checkPictures = currDoc->checkerProfiles[currDoc->curCheckProfile].checkPictures;
	checkerSettings.checkResolution = currDoc->checkerProfiles[currDoc->curCheckProfile].checkResolution;
	checkerSettings.checkTransparency = currDoc->checkerProfiles[currDoc->curCheckProfile].checkTransparency;
	checkerSettings.minResolution = currDoc->checkerProfiles[currDoc->curCheckProfile].minResolution;
	checkerSettings.checkAnnotations = currDoc->checkerProfiles[currDoc->curCheckProfile].checkAnnotations;
	checkerSettings.checkRasterPDF = currDoc->checkerProfiles[currDoc->curCheckProfile].checkRasterPDF;
	currDoc->docItemErrors.clear();
	currDoc->masterItemErrors.clear();
	errorCodes itemError;
	for (uint d = 0; d < currDoc->MasterItems.count(); ++d)
	{
		currItem = currDoc->MasterItems.at(d);
		itemError.clear();
		if (((currItem->isAnnotation) || (currItem->isBookmark)) && (checkerSettings.checkAnnotations))
			itemError.insert(7, 0);
		if (((currItem->fillTransparency() != 0.0) || (currItem->lineTransparency() != 0.0)) && (checkerSettings.checkTransparency))
			itemError.insert(6, 0);
		if ((currItem->GrType != 0) && (checkerSettings.checkTransparency))
		{
			QPtrVector<VColorStop> colorStops = currItem->fill_gradient.colorStops();
			for( uint offset = 0 ; offset < colorStops.count() ; offset++ )
			{
				if (colorStops[offset]->opacity != 1.0)
				{
					itemError.insert(6, 0);
					break;
				}
			}
		}
		if ((currItem->OwnPage == -1) && (checkerSettings.checkOrphans))
			itemError.insert(3, 0);
		if (currItem->asImageFrame())
		{
		 	if ((!currItem->PicAvail) && (checkerSettings.checkPictures))
				itemError.insert(4, 0);
			else
			{
				if  (((qRound(72.0 / currItem->LocalScX) < checkerSettings.minResolution) || (qRound(72.0 / currItem->LocalScY) < checkerSettings.minResolution))
				          && (currItem->isRaster) && (checkerSettings.checkResolution))
					itemError.insert(5, 0);
				QFileInfo fi = QFileInfo(currItem->Pfile);
				QString ext = fi.extension(false).lower();
				if ((ext == "pdf") && (checkerSettings.checkRasterPDF))
					itemError.insert(8, 0);
			}
		}
		if ((currItem->asTextFrame()) || (currItem->asPathText()))
		{
			if ((currItem->itemText.count() > currItem->MaxChars) && (checkerSettings.checkOverflow))
				itemError.insert(2, 0);
			for (uint e = 0; e < currItem->itemText.count(); ++e)
			{
				uint chr = currItem->itemText.at(e)->ch[0].unicode();
				if ((chr == 13) || (chr == 32) || (chr == 29) || (chr == 28) || (chr == 27) || (chr == 26) || (chr == 25))
					continue;
				if ((currItem->itemText.at(e)->cstyle & 64) || (currItem->itemText.at(e)->cstyle & 32))
				{
					chx = currItem->itemText.at(e)->ch;
					if (chx.upper() != currItem->itemText.at(e)->ch)
						chx = chx.upper();
					chr = chx[0].unicode();
				}
				if (chr == 9)
				{
					for (uint t1 = 0; t1 < currDoc->docParagraphStyles[currItem->itemText.at(e)->cab].TabValues.count(); t1++)
					{
						if (currDoc->docParagraphStyles[currItem->itemText.at(e)->cab].TabValues[t1].tabFillChar.isNull())
							continue;
						chx = QString(currDoc->docParagraphStyles[currItem->itemText.at(e)->cab].TabValues[t1].tabFillChar);
						if ((currItem->itemText.at(e)->cstyle & 64) || (currItem->itemText.at(e)->cstyle & 32))
						{
							if (chx.upper() != QString(currDoc->docParagraphStyles[currItem->itemText.at(e)->cab].TabValues[t1].tabFillChar))
								chx = chx.upper();
						}
						chr = chx[0].unicode();
						if ((!currItem->itemText.at(e)->cfont->CharWidth.contains(chr)) && (checkerSettings.checkGlyphs))
							itemError.insert(1, 0);
					}
					for (uint t1 = 0; t1 < currItem->TabValues.count(); t1++)
					{
						if (currItem->TabValues[t1].tabFillChar.isNull())
							continue;
						chx = QString(currItem->TabValues[t1].tabFillChar);
						if ((currItem->itemText.at(e)->cstyle & 64) || (currItem->itemText.at(e)->cstyle & 32))
						{
							if (chx.upper() != QString(currItem->TabValues[t1].tabFillChar))
								chx = chx.upper();
						}
						chr = chx[0].unicode();
						if ((!currItem->itemText.at(e)->cfont->CharWidth.contains(chr)) && (checkerSettings.checkGlyphs))
							itemError.insert(1, 0);
					}
					continue;
				}
				if (chr == 30)
				{
					for (uint numco = 0x30; numco < 0x3A; ++numco)
					{
						if ((!currItem->itemText.at(e)->cfont->CharWidth.contains(numco)) && (checkerSettings.checkGlyphs))
							itemError.insert(1, 0);
					}
					continue;
				}
				if ((!currItem->itemText.at(e)->cfont->CharWidth.contains(chr)) && (checkerSettings.checkGlyphs))
					itemError.insert(1, 0);
			}
		}
		if (itemError.count() != 0)
			currDoc->masterItemErrors.insert(currItem->ItemNr, itemError);
	}
	for (uint d = 0; d < currDoc->DocItems.count(); ++d)
	{
		currItem = currDoc->DocItems.at(d);
		itemError.clear();
		if (((currItem->fillTransparency() != 0.0) || (currItem->lineTransparency() != 0.0)) && (checkerSettings.checkTransparency))
			itemError.insert(6, 0);
		if ((currItem->GrType != 0) && (checkerSettings.checkTransparency))
		{
			QPtrVector<VColorStop> colorStops = currItem->fill_gradient.colorStops();
			for( uint offset = 0 ; offset < colorStops.count() ; offset++ )
			{
				if (colorStops[offset]->opacity != 1.0)
				{
					itemError.insert(6, 0);
					break;
				}
			}
		}
		if (((currItem->isAnnotation) || (currItem->isBookmark)) && (checkerSettings.checkAnnotations))
			itemError.insert(7, 0);
		if ((currItem->OwnPage == -1) && (checkerSettings.checkOrphans))
			itemError.insert(3, 0);
		if (currItem->asImageFrame())
		{
		 	if ((!currItem->PicAvail) && (checkerSettings.checkPictures))
				itemError.insert(4, 0);
			else
			{
				if  (((qRound(72.0 / currItem->LocalScX) < checkerSettings.minResolution) || (qRound(72.0 / currItem->LocalScY) < checkerSettings.minResolution))
				           && (currItem->isRaster) && (checkerSettings.checkResolution))
					itemError.insert(5, 0);
				QFileInfo fi = QFileInfo(currItem->Pfile);
				QString ext = fi.extension(false).lower();
				if ((ext == "pdf") && (checkerSettings.checkRasterPDF))
					itemError.insert(8, 0);
			}
		}
		if ((currItem->asTextFrame()) || (currItem->asPathText()))
		{
			if ((currItem->itemText.count() > currItem->MaxChars) && (checkerSettings.checkOverflow))
				itemError.insert(2, 0);
			for (uint e = 0; e < currItem->itemText.count(); ++e)
			{
				uint chr = currItem->itemText.at(e)->ch[0].unicode();
				if ((chr == 13) || (chr == 32) || (chr == 29) || (chr == 28) || (chr == 27) || (chr == 26) || (chr == 25))
					continue;
				if ((currItem->itemText.at(e)->cstyle & 64) || (currItem->itemText.at(e)->cstyle & 32))
				{
					chx = currItem->itemText.at(e)->ch;
					if (chx.upper() != currItem->itemText.at(e)->ch)
						chx = chx.upper();
					chr = chx[0].unicode();
				}
				if (chr == 9)
				{
					for (uint t1 = 0; t1 < currDoc->docParagraphStyles[currItem->itemText.at(e)->cab].TabValues.count(); t1++)
					{
						if (currDoc->docParagraphStyles[currItem->itemText.at(e)->cab].TabValues[t1].tabFillChar.isNull())
							continue;
						chx = QString(currDoc->docParagraphStyles[currItem->itemText.at(e)->cab].TabValues[t1].tabFillChar);
						if ((currItem->itemText.at(e)->cstyle & 64) || (currItem->itemText.at(e)->cstyle & 32))
						{
							if (chx.upper() != QString(currDoc->docParagraphStyles[currItem->itemText.at(e)->cab].TabValues[t1].tabFillChar))
								chx = chx.upper();
						}
						chr = chx[0].unicode();
						if ((!currItem->itemText.at(e)->cfont->CharWidth.contains(chr)) && (checkerSettings.checkGlyphs))
							itemError.insert(1, 0);
					}
					for (uint t1 = 0; t1 < currItem->TabValues.count(); t1++)
					{
						if (currItem->TabValues[t1].tabFillChar.isNull())
							continue;
						chx = QString(currItem->TabValues[t1].tabFillChar);
						if ((currItem->itemText.at(e)->cstyle & 64) || (currItem->itemText.at(e)->cstyle & 32))
						{
							if (chx.upper() != QString(currItem->TabValues[t1].tabFillChar))
								chx = chx.upper();
						}
						chr = chx[0].unicode();
						if ((!currItem->itemText.at(e)->cfont->CharWidth.contains(chr)) && (checkerSettings.checkGlyphs))
							itemError.insert(1, 0);
					}
					continue;
				}
				if (chr == 30)
				{
					for (uint numco = 0x30; numco < 0x3A; ++numco)
					{
						if ((!currItem->itemText.at(e)->cfont->CharWidth.contains(numco)) && (checkerSettings.checkGlyphs))
							itemError.insert(1, 0);
					}
					continue;
				}
				if ((!currItem->itemText.at(e)->cfont->CharWidth.contains(chr)) && (checkerSettings.checkGlyphs))
					itemError.insert(1, 0);
			}
		}
		if (itemError.count() != 0)
			currDoc->docItemErrors.insert(currItem->ItemNr, itemError);
	}
}
