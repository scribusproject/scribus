/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
	begin                : 2005
	copyright            : (C) 2005 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
	copyright            : (C) 2005 by Craig Bradney
	email                : cbradney@zip.com.au
***************************************************************************/

/***************************************************************************
*                                                                         *
*   ScMW program is free software; you can redistribute it and/or modify  *
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
#include "text/nlsconfig.h"

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
	checkerSettings.maxResolution = currDoc->checkerProfiles[currDoc->curCheckProfile].maxResolution;
	checkerSettings.checkAnnotations = currDoc->checkerProfiles[currDoc->curCheckProfile].checkAnnotations;
	checkerSettings.checkRasterPDF = currDoc->checkerProfiles[currDoc->curCheckProfile].checkRasterPDF;
	checkerSettings.checkForGIF = currDoc->checkerProfiles[currDoc->curCheckProfile].checkForGIF;
	currDoc->docItemErrors.clear();
	currDoc->masterItemErrors.clear();
	errorCodes itemError;
	for (uint d = 0; d < currDoc->MasterItems.count(); ++d)
	{
		currItem = currDoc->MasterItems.at(d);
		if (!currItem->printEnabled())
			continue;
		itemError.clear();
		if (((currItem->isAnnotation()) || (currItem->isBookmark)) && (checkerSettings.checkAnnotations))
			itemError.insert(PDFAnnotField, 0);
		if (((currItem->fillTransparency() != 0.0) || (currItem->lineTransparency() != 0.0)) && (checkerSettings.checkTransparency))
			itemError.insert(Transparency, 0);
		if ((currItem->GrType != 0) && (checkerSettings.checkTransparency))
		{
			QPtrVector<VColorStop> colorStops = currItem->fill_gradient.colorStops();
			for( uint offset = 0 ; offset < colorStops.count() ; offset++ )
			{
				if (colorStops[offset]->opacity != 1.0)
				{
					itemError.insert(Transparency, 0);
					break;
				}
			}
		}
		if ((currItem->OwnPage == -1) && (checkerSettings.checkOrphans))
			itemError.insert(ObjectNotOnPage, 0);
		if (currItem->asImageFrame())
		{
		 	if ((!currItem->PicAvail) && (checkerSettings.checkPictures))
				itemError.insert(MissingImage, 0);
			else
			{
				if  (((qRound(72.0 / currItem->imageXScale()) < checkerSettings.minResolution) || (qRound(72.0 / currItem->imageYScale()) < checkerSettings.minResolution))
				          && (currItem->isRaster) && (checkerSettings.checkResolution))
					itemError.insert(ImageDPITooLow, 0);
				if  (((qRound(72.0 / currItem->imageXScale()) > checkerSettings.maxResolution) || (qRound(72.0 / currItem->imageYScale()) > checkerSettings.maxResolution))
				          && (currItem->isRaster) && (checkerSettings.checkResolution))
					itemError.insert(ImageDPITooHigh, 0);
				QFileInfo fi = QFileInfo(currItem->Pfile);
				QString ext = fi.extension(false).lower();
				if ((ext == "pdf") && (checkerSettings.checkRasterPDF))
					itemError.insert(PlacedPDF, 0);
				if ((ext == "gif") && (checkerSettings.checkForGIF))
					itemError.insert(ImageIsGIF, 0);
			}
		}
		if ((currItem->asTextFrame()) || (currItem->asPathText()))
		{
#ifndef NLS_PROTO
			if ( currItem->frameOverflows() && (checkerSettings.checkOverflow))
				itemError.insert(TextOverflow, 0);
			for (uint e = 0; e < currItem->itemText.length(); ++e)
			{
				uint chr = currItem->itemText.text(e).unicode();
				if ((chr == 13) || (chr == 32) || (chr == 29) || (chr == 28) || (chr == 27) || (chr == 26) || (chr == 25))
					continue;
				if ((currItem->itemText.charStyle(e).cstyle & 64) || (currItem->itemText.charStyle(e).cstyle & 32))
				{
					chx = currItem->itemText.text(e);
					if (chx.upper() != currItem->itemText.text(e))
						chx = chx.upper();
					chr = chx[0].unicode();
				}
				if (chr == 9)
				{
					for (uint t1 = 0; t1 < currDoc->docParagraphStyles[currItem->itemText.at(e)->cab].tabValues().count(); t1++)
					{
						if (currDoc->docParagraphStyles[currItem->itemText.at(e)->cab].tabValues()[t1].tabFillChar.isNull())
							continue;
						chx = QString(currDoc->docParagraphStyles[currItem->itemText.at(e)->cab].tabValues()[t1].tabFillChar);
						if ((currItem->itemText.charStyle(e).cstyle & 64) || (currItem->itemText.charStyle(e).cstyle & 32))
						{
							if (chx.upper() != QString(currDoc->docParagraphStyles[currItem->itemText.at(e)->cab].tabValues()[t1].tabFillChar))
								chx = chx.upper();
						}
						chr = chx[0].unicode();
						if ((!currItem->itemText.at(e)->cfont->canRender(chr)) && (checkerSettings.checkGlyphs))
							itemError.insert(MissingGlyph, 0);
					}
					for (uint t1 = 0; t1 < currItem->TabValues.count(); t1++)
					{
						if (currItem->TabValues[t1].tabFillChar.isNull())
							continue;
						chx = QString(currItem->TabValues[t1].tabFillChar);
						if ((currItem->itemText.charStyle(e).cstyle & 64) || (currItem->itemText.charStyle(e).cstyle & 32))
						{
							if (chx.upper() != QString(currItem->TabValues[t1].tabFillChar))
								chx = chx.upper();
						}
						chr = chx[0].unicode();
						if ((!currItem->itemText.at(e)->cfont->canRender(chr)) && (checkerSettings.checkGlyphs))
							itemError.insert(MissingGlyph, 0);
					}
					continue;
				}
				if (chr == 30)
				{
					for (uint numco = 0x30; numco < 0x3A; ++numco)
					{
						if ((!currItem->itemText.at(e)->cfont->canRender(numco)) && (checkerSettings.checkGlyphs))
							itemError.insert(MissingGlyph, 0);
					}
					continue;
				}
				if ((!currItem->itemText.at(e)->cfont->canRender(chr)) && (checkerSettings.checkGlyphs))
					itemError.insert(MissingGlyph, 0);
			}
#endif
		}
		if (itemError.count() != 0)
			currDoc->masterItemErrors.insert(currItem->ItemNr, itemError);
	}
	for (uint d = 0; d < currDoc->DocItems.count(); ++d)
	{
		currItem = currDoc->DocItems.at(d);
		if (!currItem->printEnabled())
			continue;
		itemError.clear();
		if (((currItem->fillTransparency() != 0.0) || (currItem->lineTransparency() != 0.0)) && (checkerSettings.checkTransparency))
			itemError.insert(Transparency, 0);
		if ((currItem->GrType != 0) && (checkerSettings.checkTransparency))
		{
			QPtrVector<VColorStop> colorStops = currItem->fill_gradient.colorStops();
			for( uint offset = 0 ; offset < colorStops.count() ; offset++ )
			{
				if (colorStops[offset]->opacity != 1.0)
				{
					itemError.insert(Transparency, 0);
					break;
				}
			}
		}
		if (((currItem->isAnnotation()) || (currItem->isBookmark)) && (checkerSettings.checkAnnotations))
			itemError.insert(PDFAnnotField, 0);
		if ((currItem->OwnPage == -1) && (checkerSettings.checkOrphans))
			itemError.insert(ObjectNotOnPage, 0);
		if (currItem->asImageFrame())
		{
		 	if ((!currItem->PicAvail) && (checkerSettings.checkPictures))
				itemError.insert(MissingImage, 0);
			else
			{
				if  (((qRound(72.0 / currItem->imageYScale()) < checkerSettings.minResolution) || (qRound(72.0 / currItem->imageYScale()) < checkerSettings.minResolution))
				           && (currItem->isRaster) && (checkerSettings.checkResolution))
					itemError.insert(ImageDPITooLow, 0);
				if  (((qRound(72.0 / currItem->imageXScale()) > checkerSettings.maxResolution) || (qRound(72.0 / currItem->imageYScale()) > checkerSettings.maxResolution))
				          && (currItem->isRaster) && (checkerSettings.checkResolution))
					itemError.insert(ImageDPITooHigh, 0);
				QFileInfo fi = QFileInfo(currItem->Pfile);
				QString ext = fi.extension(false).lower();
				if ((ext == "pdf") && (checkerSettings.checkRasterPDF))
					itemError.insert(PlacedPDF, 0);
				if ((ext == "gif") && (checkerSettings.checkForGIF))
					itemError.insert(ImageIsGIF, 0);
			}
		}
		if ((currItem->asTextFrame()) || (currItem->asPathText()))
		{
#ifndef NLS_PROTO
			if ( currItem->frameOverflows() && (checkerSettings.checkOverflow))
				itemError.insert(TextOverflow, 0);
			for (uint e = 0; e < currItem->itemText.count(); ++e)
			{
				uint chr = currItem->itemText.text(e).unicode();
				if ((chr == 13) || (chr == 32) || (chr == 29) || (chr == 28) || (chr == 27) || (chr == 26) || (chr == 25))
					continue;
				if ((currItem->itemText.charStyle(e).cstyle & 64) || (currItem->itemText.charStyle(e).cstyle & 32))
				{
					chx = currItem->itemText.text(e,1);
					if (chx.upper() != currItem->itemText.at(e)->ch)
						chx = chx.upper();
					chr = chx[0].unicode();
				}
				if (chr == 9)
				{
					for (uint t1 = 0; t1 < currDoc->docParagraphStyles[currItem->itemText.at(e)->cab].tabValues().count(); t1++)
					{
						if (currDoc->docParagraphStyles[currItem->itemText.at(e)->cab].tabValues()[t1].tabFillChar.isNull())
							continue;
						chx = QString(currDoc->docParagraphStyles[currItem->itemText.at(e)->cab].tabValues()[t1].tabFillChar);
						if ((currItem->itemText.charStyle(e).cstyle & 64) || (currItem->itemText.charStyle(e).cstyle & 32))
						{
							if (chx.upper() != QString(currDoc->docParagraphStyles[currItem->itemText.at(e)->cab].tabValues()[t1].tabFillChar))
								chx = chx.upper();
						}
						chr = chx[0].unicode();
						if ((!currItem->itemText.at(e)->cfont->canRender(chr)) && (checkerSettings.checkGlyphs))
							itemError.insert(MissingGlyph, 0);
					}
					for (uint t1 = 0; t1 < currItem->TabValues.count(); t1++)
					{
						if (currItem->TabValues[t1].tabFillChar.isNull())
							continue;
						chx = QString(currItem->TabValues[t1].tabFillChar);
						if ((currItem->itemText.charStyle(e).cstyle & 64) || (currItem->itemText.charStyle(e).cstyle & 32))
						{
							if (chx.upper() != QString(currItem->TabValues[t1].tabFillChar))
								chx = chx.upper();
						}
						chr = chx[0].unicode();
						if ((!currItem->itemText.at(e)->cfont->canRender(chr)) && (checkerSettings.checkGlyphs))
							itemError.insert(MissingGlyph, 0);
					}
					continue;
				}
				if (chr == 30)
				{
					for (uint numco = 0x30; numco < 0x3A; ++numco)
					{
						if ((!currItem->itemText.at(e)->cfont->canRender(numco)) && (checkerSettings.checkGlyphs))
							itemError.insert(MissingGlyph, 0);
					}
					continue;
				}
				if ((!currItem->itemText.at(e)->cfont->canRender(chr)) && (checkerSettings.checkGlyphs))
					itemError.insert(MissingGlyph, 0);
			}
#endif
		}
		if (itemError.count() != 0)
			currDoc->docItemErrors.insert(currItem->ItemNr, itemError);
	}
}
