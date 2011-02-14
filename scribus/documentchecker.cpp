/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
	begin				: 2005
	copyright			: (C) 2005 by Franz Schmid
	email				: Franz.Schmid@altmuehlnet.de
	copyright			: (C) 2005 by Craig Bradney
	email				: cbradney@zip.com.au
***************************************************************************/

/***************************************************************************
*																		 *
*   ScMW program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or	 *
*   (at your option) any later version.								   *
*																		 *
***************************************************************************/

#include "commonstrings.h"
#include "documentchecker.h"
#include "page.h"
#include "pageitem.h"
#include "pdf_analyzer.h"
#include "sccolor.h"
#include "sclayer.h"
#include "scribusdoc.h"
#include "scribusstructs.h"
#include "text/nlsconfig.h"
#include "util.h"
#include "util_formats.h"

#include <QList>


bool isPartFilledImageFrame(PageItem * currItem)
{
    //qDebug() << "X" << currItem->width() << currItem->imageXScale() / 72.0 * currItem->pixm.imgInfo.xres * currItem->pixm.width();
    //qDebug() << "Y" << currItem->height() << currItem->imageYScale() / 72.0 * currItem->pixm.imgInfo.yres * currItem->pixm.height();
	return (currItem->height() > currItem->imageYScale() / 72.0 * currItem->pixm.imgInfo.yres * currItem->pixm.height()
			|| currItem->width() > currItem->imageXScale() / 72.0 * currItem->pixm.imgInfo.xres * currItem->pixm.width());
}


bool DocumentChecker::checkDocument(ScribusDoc *currDoc)
{
	QString chstr;
	struct CheckerPrefs checkerSettings;
	checkerSettings.ignoreErrors = currDoc->checkerProfiles()[currDoc->curCheckProfile()].ignoreErrors;
	checkerSettings.autoCheck = currDoc->checkerProfiles()[currDoc->curCheckProfile()].autoCheck;
	checkerSettings.checkGlyphs = currDoc->checkerProfiles()[currDoc->curCheckProfile()].checkGlyphs;
	checkerSettings.checkOrphans = currDoc->checkerProfiles()[currDoc->curCheckProfile()].checkOrphans;
	checkerSettings.checkOverflow = currDoc->checkerProfiles()[currDoc->curCheckProfile()].checkOverflow;
	checkerSettings.checkPictures = currDoc->checkerProfiles()[currDoc->curCheckProfile()].checkPictures;
	checkerSettings.checkResolution = currDoc->checkerProfiles()[currDoc->curCheckProfile()].checkResolution;
	checkerSettings.checkPartFilledImageFrames = currDoc->checkerProfiles()[currDoc->curCheckProfile()].checkPartFilledImageFrames;
	checkerSettings.checkTransparency = currDoc->checkerProfiles()[currDoc->curCheckProfile()].checkTransparency;
	checkerSettings.minResolution = currDoc->checkerProfiles()[currDoc->curCheckProfile()].minResolution;
	checkerSettings.maxResolution = currDoc->checkerProfiles()[currDoc->curCheckProfile()].maxResolution;
	checkerSettings.checkAnnotations = currDoc->checkerProfiles()[currDoc->curCheckProfile()].checkAnnotations;
	checkerSettings.checkRasterPDF = currDoc->checkerProfiles()[currDoc->curCheckProfile()].checkRasterPDF;
	checkerSettings.checkForGIF = currDoc->checkerProfiles()[currDoc->curCheckProfile()].checkForGIF;
	checkerSettings.ignoreOffLayers = currDoc->checkerProfiles()[currDoc->curCheckProfile()].ignoreOffLayers;
	checkerSettings.checkOffConflictLayers = currDoc->checkerProfiles()[currDoc->curCheckProfile()].checkOffConflictLayers;
	checkerSettings.checkNotCMYKOrSpot = currDoc->checkerProfiles()[currDoc->curCheckProfile()].checkNotCMYKOrSpot;
	checkerSettings.checkDeviceColorsAndOutputIntend = currDoc->checkerProfiles()[currDoc->curCheckProfile()].checkDeviceColorsAndOutputIntend;
	checkerSettings.checkFontNotEmbedded = currDoc->checkerProfiles()[currDoc->curCheckProfile()].checkFontNotEmbedded;
	checkerSettings.checkFontIsOpenType = currDoc->checkerProfiles()[currDoc->curCheckProfile()].checkFontIsOpenType;
	currDoc->docItemErrors.clear();
	currDoc->masterItemErrors.clear();
	currDoc->docLayerErrors.clear();
	errorCodes itemError;
	errorCodes layerError;
	int Lnr;
	ScLayer ll;
	ll.ID = 0;
	Lnr = 0;
	uint layerCount= currDoc->layerCount();
	for (uint la = 0; la < layerCount; ++la)
	{
		layerError.clear();
		currDoc->Layers.levelToLayer(ll, Lnr);
		if ((ll.isViewable != ll.isPrintable) && (checkerSettings.checkOffConflictLayers))
			layerError.insert(OffConflictLayers, 0);
		if ((!ll.isViewable) && (checkerSettings.ignoreOffLayers))
			continue;
		if ((!ll.isPrintable) && (checkerSettings.ignoreOffLayers))
			continue;
		if ((ll.transparency != 1.0) && (checkerSettings.checkTransparency))
			layerError.insert(Transparency, 0);
		if ((ll.blendMode != 0) && (checkerSettings.checkTransparency))
			layerError.insert(BlendMode, 1);
		Lnr++;
		if (layerError.count() != 0)
			currDoc->docLayerErrors.insert(ll.ID, layerError);
	}
	QList<PageItem*> allItems;
	uint masterItemsCount = currDoc->MasterItems.count();
	for (uint i = 0; i < masterItemsCount; ++i)
	{
		PageItem* currItem = currDoc->MasterItems.at(i);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (!currItem->printEnabled())
				continue;
			if (!(currDoc->layerPrintable(currItem->LayerID)) && (checkerSettings.ignoreOffLayers))
				continue;
			itemError.clear();
			if (((currItem->isAnnotation()) || (currItem->isBookmark)) && (checkerSettings.checkAnnotations))
				itemError.insert(PDFAnnotField, 0);
			if (((currItem->fillTransparency() != 0.0) || (currItem->lineTransparency() != 0.0) || (currItem->fillBlendmode() != 0) || (currItem->lineBlendmode() != 0)) && (checkerSettings.checkTransparency))
				itemError.insert(Transparency, 0);
			if ((currItem->GrType != 0) && (checkerSettings.checkTransparency))
			{
				if (currItem->GrType == 9)
				{
					if (currItem->GrCol1transp != 1.0)
						itemError.insert(Transparency, 0);
					else if (currItem->GrCol2transp != 1.0)
						itemError.insert(Transparency, 0);
					else if (currItem->GrCol3transp != 1.0)
						itemError.insert(Transparency, 0);
					else if (currItem->GrCol4transp != 1.0)
						itemError.insert(Transparency, 0);
				}
				else if (currItem->GrType == 11)
				{
					for (int grow = 0; grow < currItem->meshGradientArray.count(); grow++)
					{
						for (int gcol = 0; gcol < currItem->meshGradientArray[grow].count(); gcol++)
						{
							if (currItem->meshGradientArray[grow][gcol].transparency != 1.0)
								itemError.insert(Transparency, 0);
						}
					}
				}
				else
				{
					QList<VColorStop*> colorStops = currItem->fill_gradient.colorStops();
					for( int offset = 0 ; offset < colorStops.count() ; offset++ )
					{
						if (colorStops[offset]->opacity != 1.0)
						{
							itemError.insert(Transparency, 0);
							break;
						}
					}
				}
			}
			if ((currItem->GrTypeStroke != 0) && (checkerSettings.checkTransparency))
			{
				QList<VColorStop*> colorStops = currItem->stroke_gradient.colorStops();
				for( int offset = 0 ; offset < colorStops.count() ; offset++ )
				{
					if (colorStops[offset]->opacity != 1.0)
					{
						itemError.insert(Transparency, 0);
						break;
					}
				}
			}
			if ((currItem->GrMask > 0) && (checkerSettings.checkTransparency))
				itemError.insert(Transparency, 0);
			if ((currItem->OwnPage == -1) && (checkerSettings.checkOrphans))
				itemError.insert(ObjectNotOnPage, 0);
	#ifdef HAVE_OSG
			if (currItem->asImageFrame() && !currItem->asOSGFrame())
	#else
			if (currItem->asImageFrame())
	#endif
			{

				// check image vs. frame sizes
				if (checkerSettings.checkPartFilledImageFrames && isPartFilledImageFrame(currItem))
				{
					itemError.insert(PartFilledImageFrame, 0);
				}

				if ((!currItem->PictureIsAvailable) && (checkerSettings.checkPictures))
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
					QString ext = fi.suffix().toLower();
					if (extensionIndicatesPDF(ext) && (checkerSettings.checkRasterPDF))
						itemError.insert(PlacedPDF, 0);
					if ((ext == "gif") && (checkerSettings.checkForGIF))
						itemError.insert(ImageIsGIF, 0);

					if (extensionIndicatesPDF(ext))
					{
						PDFAnalyzer analyst(currItem->Pfile);
						QList<PDFColorSpace> usedColorSpaces;
						bool hasTransparency = false;
						QList<PDFFont> usedFonts;
						int pageNum = qMin(qMax(1, currItem->pixm.imgInfo.actualPageNumber), currItem->pixm.imgInfo.numberOfPages) - 1;
						QList<PDFImage> imgs;
						bool succeeded = analyst.inspectPDF(pageNum, usedColorSpaces, hasTransparency, usedFonts, imgs);
						if (succeeded)
						{
							if (checkerSettings.checkNotCMYKOrSpot || checkerSettings.checkDeviceColorsAndOutputIntend)
							{
								eColorSpaceType currPrintProfCS = ColorSpace_Unknown;
								if (currDoc->HasCMS)
								{
									ScColorProfile printerProf = currDoc->DocPrinterProf;
									currPrintProfCS = printerProf.colorSpace();
								}
								if (checkerSettings.checkNotCMYKOrSpot)
								{
									for (int i=0; i<usedColorSpaces.size(); ++i)
									{
										if (usedColorSpaces[i] == CS_DeviceRGB || usedColorSpaces[i] == CS_ICCBased || usedColorSpaces[i] == CS_CalGray
											|| usedColorSpaces[i] == CS_CalRGB || usedColorSpaces[i] == CS_Lab)
										{
											itemError.insert(NotCMYKOrSpot, 0);
											break;
										}
									}
								}
								if (checkerSettings.checkDeviceColorsAndOutputIntend && currDoc->HasCMS)
								{
									for (int i=0; i<usedColorSpaces.size(); ++i)
									{
										if (currPrintProfCS == ColorSpace_Cmyk && (usedColorSpaces[i] == CS_DeviceRGB || usedColorSpaces[i] == CS_DeviceGray))
										{
											itemError.insert(DeviceColorAndOutputIntend, 0);
											break;
										}
										else if (currPrintProfCS == ColorSpace_Rgb && (usedColorSpaces[i] == CS_DeviceCMYK || usedColorSpaces[i] == CS_DeviceGray))
										{
											itemError.insert(DeviceColorAndOutputIntend, 0);
											break;
										}
									}
								}
							}
							if (checkerSettings.checkTransparency && hasTransparency)
								itemError.insert(Transparency, 0);
							if (checkerSettings.checkFontNotEmbedded || checkerSettings.checkFontIsOpenType)
							{
								for (int i=0; i<usedFonts.size(); ++i)
								{
									PDFFont currentFont = usedFonts[i];
									if (!currentFont.isEmbedded && checkerSettings.checkFontNotEmbedded)
										itemError.insert(FontNotEmbedded, 0);
									if (currentFont.isEmbedded && currentFont.isOpenType && checkerSettings.checkFontIsOpenType)
										itemError.insert(EmbeddedFontIsOpenType, 0);
								}
							}
							if (checkerSettings.checkResolution)
							{
								for (int i=0; i<imgs.size(); ++i)
								{
									if ((imgs[i].dpiX < checkerSettings.minResolution) || (imgs[i].dpiY < checkerSettings.minResolution))
										itemError.insert(ImageDPITooLow, 0);
									if ((imgs[i].dpiX > checkerSettings.maxResolution) || (imgs[i].dpiY > checkerSettings.maxResolution))
										itemError.insert(ImageDPITooHigh, 0);
								}
							}
						}
					}
				}
			}
			if ((currItem->asTextFrame()) || (currItem->asPathText()))
			{
	#ifndef NLS_PROTO
				if ( currItem->frameOverflows() && (checkerSettings.checkOverflow) && (!((currItem->isAnnotation()) && ((currItem->annotation().Type() == 5) || (currItem->annotation().Type() == 6)))))
					itemError.insert(TextOverflow, 0);
				if (currItem->isAnnotation())
				{
					ScFace::FontFormat fformat = currItem->itemText.defaultStyle().charStyle().font().format();
					if (!(fformat == ScFace::SFNT || fformat == ScFace::TTCF))
						itemError.insert(WrongFontInAnnotation, 0);
				}
				for (int e = currItem->firstInFrame(); e <= currItem->lastInFrame(); ++e)
				{
					uint chr = currItem->itemText.text(e).unicode();
					if ((chr == 13) || (chr == 32) || (chr == 29) || (chr == 28) || (chr == 27) || (chr == 26) || (chr == 25))
						continue;
					if ((currItem->itemText.charStyle(e).effects() & ScStyle_SmallCaps) || (currItem->itemText.charStyle(e).effects() & ScStyle_AllCaps))
					{
						chstr = currItem->itemText.text(e);
						if (chstr.toUpper() != currItem->itemText.text(e))
							chstr = chstr.toUpper();
						chr = chstr[0].unicode();
					}
					if (chr == 9)
					{
						for (int t1 = 0; t1 < currItem->itemText.paragraphStyle(e).tabValues().count(); t1++)
						{
							if (currItem->itemText.paragraphStyle(e).tabValues()[t1].tabFillChar.isNull())
								continue;
							chstr = QString(currItem->itemText.paragraphStyle(e).tabValues()[t1].tabFillChar);
							if ((currItem->itemText.charStyle(e).effects() & ScStyle_SmallCaps) || (currItem->itemText.charStyle(e).effects() & ScStyle_AllCaps))
							{
								if (chstr.toUpper() != QString(currItem->itemText.paragraphStyle(e).tabValues()[t1].tabFillChar))
									chstr = chstr.toUpper();
							}
							chr = chstr[0].unicode();
							if ((!currItem->itemText.charStyle(e).font().canRender(chr)) && (checkerSettings.checkGlyphs))
								itemError.insert(MissingGlyph, 0);
						}
						for (int t1 = 0; t1 < currItem->itemText.defaultStyle().tabValues().count(); t1++)
						{
							if (currItem->itemText.defaultStyle().tabValues()[t1].tabFillChar.isNull())
								continue;
							chstr = QString(currItem->itemText.defaultStyle().tabValues()[t1].tabFillChar);
							if ((currItem->itemText.charStyle(e).effects() & ScStyle_SmallCaps) || (currItem->itemText.charStyle(e).effects() & ScStyle_AllCaps))
							{
								if (chstr.toUpper() != QString(currItem->itemText.defaultStyle().tabValues()[t1].tabFillChar))
									chstr = chstr.toUpper();
							}
							chr = chstr[0].unicode();
							if ((!currItem->itemText.charStyle(e).font().canRender(chr)) && (checkerSettings.checkGlyphs))
								itemError.insert(MissingGlyph, 0);
						}
						continue;
					}
					if ((chr == 30) || (chr == 23))
					{
						for (int numco = 0x30; numco < 0x3A; ++numco)
						{
							if ((!currItem->itemText.charStyle(e).font().canRender(numco)) && (checkerSettings.checkGlyphs))
								itemError.insert(MissingGlyph, 0);
						}
						continue;
					}
					if ((!currItem->itemText.charStyle(e).font().canRender(chr)) && (checkerSettings.checkGlyphs))
						itemError.insert(MissingGlyph, 0);
				}
	#endif
			}
			if (((currItem->fillColor() != CommonStrings::None) || (currItem->lineColor() != CommonStrings::None)) && (checkerSettings.checkNotCMYKOrSpot))
			{
				bool rgbUsed = false;
				if ((currItem->fillColor() != CommonStrings::None))
				{
					ScColor tmpC = currDoc->PageColors[currItem->fillColor()];
					if (tmpC.getColorModel() == colorModelRGB)
						rgbUsed = true;
				}
				if ((currItem->lineColor() != CommonStrings::None))
				{
					ScColor tmpC = currDoc->PageColors[currItem->lineColor()];
					if (tmpC.getColorModel() == colorModelRGB)
						rgbUsed = true;
				}
				if (rgbUsed)
					itemError.insert(NotCMYKOrSpot, 0);
			}
			if (itemError.count() != 0)
				currDoc->masterItemErrors.insert(currItem, itemError);
		}
		allItems.clear();
	}
	allItems.clear();
	uint docItemsCount = currDoc->DocItems.count();
	for (uint i = 0; i < docItemsCount; ++i)
	{
		PageItem* currItem = currDoc->DocItems.at(i);
		if (currItem->isGroup())
			allItems = currItem->getItemList();
		else
			allItems.append(currItem);
		for (int ii = 0; ii < allItems.count(); ii++)
		{
			currItem = allItems.at(ii);
			if (!currItem->printEnabled())
				continue;
			if (!(currDoc->layerPrintable(currItem->LayerID)) && (checkerSettings.ignoreOffLayers))
				continue;
			itemError.clear();
			if (((currItem->fillTransparency() != 0.0) || (currItem->lineTransparency() != 0.0) || (currItem->fillBlendmode() != 0) || (currItem->lineBlendmode() != 0)) && (checkerSettings.checkTransparency))
				itemError.insert(Transparency, 0);
			if ((currItem->GrType != 0) && (checkerSettings.checkTransparency))
			{
				if (currItem->GrType == 9)
				{
					if (currItem->GrCol1transp != 1.0)
						itemError.insert(Transparency, 0);
					else if (currItem->GrCol2transp != 1.0)
						itemError.insert(Transparency, 0);
					else if (currItem->GrCol3transp != 1.0)
						itemError.insert(Transparency, 0);
					else if (currItem->GrCol4transp != 1.0)
						itemError.insert(Transparency, 0);
				}
				else if (currItem->GrType == 11)
				{
					for (int grow = 0; grow < currItem->meshGradientArray.count(); grow++)
					{
						for (int gcol = 0; gcol < currItem->meshGradientArray[grow].count(); gcol++)
						{
							if (currItem->meshGradientArray[grow][gcol].transparency != 1.0)
								itemError.insert(Transparency, 0);
						}
					}
				}
				else
				{
					QList<VColorStop*> colorStops = currItem->fill_gradient.colorStops();
					for( int offset = 0 ; offset < colorStops.count() ; offset++ )
					{
						if (colorStops[offset]->opacity != 1.0)
						{
							itemError.insert(Transparency, 0);
							break;
						}
					}
				}
			}
			if ((currItem->GrTypeStroke != 0) && (checkerSettings.checkTransparency))
			{
				QList<VColorStop*> colorStops = currItem->stroke_gradient.colorStops();
				for( int offset = 0 ; offset < colorStops.count() ; offset++ )
				{
					if (colorStops[offset]->opacity != 1.0)
					{
						itemError.insert(Transparency, 0);
						break;
					}
				}
			}
			if ((currItem->GrMask > 0) && (checkerSettings.checkTransparency))
				itemError.insert(Transparency, 0);
			if (((currItem->isAnnotation()) || (currItem->isBookmark)) && (checkerSettings.checkAnnotations))
				itemError.insert(PDFAnnotField, 0);
			if ((currItem->OwnPage == -1) && (checkerSettings.checkOrphans))
				itemError.insert(ObjectNotOnPage, 0);
	#ifdef HAVE_OSG
			if (currItem->asImageFrame() && !currItem->asOSGFrame())
	#else
			if (currItem->asImageFrame())
	#endif
			{

				// check image vs. frame sizes
				if (checkerSettings.checkPartFilledImageFrames && isPartFilledImageFrame(currItem))
				{
					itemError.insert(PartFilledImageFrame, 0);
				}

				if ((!currItem->PictureIsAvailable) && (checkerSettings.checkPictures))
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
					QString ext = fi.suffix().toLower();
					if (extensionIndicatesPDF(ext) && (checkerSettings.checkRasterPDF))
						itemError.insert(PlacedPDF, 0);
					if ((ext == "gif") && (checkerSettings.checkForGIF))
						itemError.insert(ImageIsGIF, 0);

					if (extensionIndicatesPDF(ext))
					{
						PDFAnalyzer analyst(currItem->Pfile);
						QList<PDFColorSpace> usedColorSpaces;
						bool hasTransparency = false;
						QList<PDFFont> usedFonts;
						int pageNum = qMin(qMax(1, currItem->pixm.imgInfo.actualPageNumber), currItem->pixm.imgInfo.numberOfPages) - 1;
						QList<PDFImage> imgs;
						bool succeeded = analyst.inspectPDF(pageNum, usedColorSpaces, hasTransparency, usedFonts, imgs);
						if (succeeded)
						{
							if (checkerSettings.checkNotCMYKOrSpot || checkerSettings.checkDeviceColorsAndOutputIntend)
							{
								int currPrintProfCS = -1;
								if (currDoc->HasCMS)
								{
									ScColorProfile printerProf = currDoc->DocPrinterProf;
									currPrintProfCS = static_cast<int>(printerProf.colorSpace());
								}
								if (checkerSettings.checkNotCMYKOrSpot)
								{
									for (int i=0; i<usedColorSpaces.size(); ++i)
									{
										if (usedColorSpaces[i] == CS_DeviceRGB || usedColorSpaces[i] == CS_ICCBased || usedColorSpaces[i] == CS_CalGray
											|| usedColorSpaces[i] == CS_CalRGB || usedColorSpaces[i] == CS_Lab)
										{
											itemError.insert(NotCMYKOrSpot, 0);
											break;
										}
									}
								}
								if (checkerSettings.checkDeviceColorsAndOutputIntend && currDoc->HasCMS)
								{
									for (int i=0; i<usedColorSpaces.size(); ++i)
									{
										if (currPrintProfCS == ColorSpace_Cmyk && (usedColorSpaces[i] == CS_DeviceRGB || usedColorSpaces[i] == CS_DeviceGray))
										{
											itemError.insert(DeviceColorAndOutputIntend, 0);
											break;
										}
										else if (currPrintProfCS == ColorSpace_Rgb && (usedColorSpaces[i] == CS_DeviceCMYK || usedColorSpaces[i] == CS_DeviceGray))
										{
											itemError.insert(DeviceColorAndOutputIntend, 0);
											break;
										}
									}
								}
							}
							if (checkerSettings.checkTransparency && hasTransparency)
								itemError.insert(Transparency, 0);
							if (checkerSettings.checkFontNotEmbedded || checkerSettings.checkFontIsOpenType)
							{
								for (int i=0; i<usedFonts.size(); ++i)
								{
									PDFFont currentFont = usedFonts[i];
									if (!currentFont.isEmbedded && checkerSettings.checkFontNotEmbedded)
										itemError.insert(FontNotEmbedded, 0);
									if (currentFont.isEmbedded && currentFont.isOpenType && checkerSettings.checkFontIsOpenType)
										itemError.insert(EmbeddedFontIsOpenType, 0);
								}
							}
							if (checkerSettings.checkResolution)
							{
								for (int i=0; i<imgs.size(); ++i)
								{
									if ((imgs[i].dpiX < checkerSettings.minResolution) || (imgs[i].dpiY < checkerSettings.minResolution))
										itemError.insert(ImageDPITooLow, 0);
									if ((imgs[i].dpiX > checkerSettings.maxResolution) || (imgs[i].dpiY > checkerSettings.maxResolution))
										itemError.insert(ImageDPITooHigh, 0);
								}
							}
						}
					}
				}
			}
			if ((currItem->asTextFrame()) || (currItem->asPathText()))
			{
	#ifndef NLS_PROTO
				if ( currItem->frameOverflows() && (checkerSettings.checkOverflow) && (!((currItem->isAnnotation()) && ((currItem->annotation().Type() == 5) || (currItem->annotation().Type() == 6)))))
					itemError.insert(TextOverflow, 0);
				if (currItem->isAnnotation())
				{
					ScFace::FontFormat fformat = currItem->itemText.defaultStyle().charStyle().font().format();
					if (!(fformat == ScFace::SFNT || fformat == ScFace::TTCF))
						itemError.insert(WrongFontInAnnotation, 0);
				}
				for (int e = currItem->firstInFrame(); e <= currItem->lastInFrame(); ++e)
				{
					uint chr = currItem->itemText.text(e).unicode();
					if ((chr == 13) || (chr == 32) || (chr == 29) || (chr == 28) || (chr == 27) || (chr == 26) || (chr == 25))
						continue;
					if ((currItem->itemText.charStyle(e).effects() & ScStyle_SmallCaps) || (currItem->itemText.charStyle(e).effects() & ScStyle_AllCaps))
					{
						chstr = currItem->itemText.text(e,1);
						if (chstr.toUpper() != currItem->itemText.text(e,1))
							chstr = chstr.toUpper();
						chr = chstr[0].unicode();
					}
					if (chr == 9)
					{
						for (int t1 = 0; t1 < currItem->itemText.paragraphStyle(e).tabValues().count(); t1++)
						{
							if (currItem->itemText.paragraphStyle(e).tabValues()[t1].tabFillChar.isNull())
								continue;
							chstr = QString(currItem->itemText.paragraphStyle(e).tabValues()[t1].tabFillChar);
							if ((currItem->itemText.charStyle(e).effects() & ScStyle_SmallCaps) || (currItem->itemText.charStyle(e).effects() & ScStyle_AllCaps))
							{
								if (chstr.toUpper() != QString(currItem->itemText.paragraphStyle(e).tabValues()[t1].tabFillChar))
									chstr = chstr.toUpper();
							}
							chr = chstr[0].unicode();
							if ((!currItem->itemText.charStyle(e).font().canRender(chr)) && (checkerSettings.checkGlyphs))
								itemError.insert(MissingGlyph, 0);
						}
						for (int t1 = 0; t1 < currItem->itemText.defaultStyle().tabValues().count(); t1++)
						{
							if (currItem->itemText.defaultStyle().tabValues()[t1].tabFillChar.isNull())
								continue;
							chstr = QString(currItem->itemText.defaultStyle().tabValues()[t1].tabFillChar);
							if ((currItem->itemText.charStyle(e).effects() & ScStyle_SmallCaps) || (currItem->itemText.charStyle(e).effects() & ScStyle_AllCaps))
							{
								if (chstr.toUpper() != QString(currItem->itemText.defaultStyle().tabValues()[t1].tabFillChar))
									chstr = chstr.toUpper();
							}
							chr = chstr[0].unicode();
							if ((!currItem->itemText.charStyle(e).font().canRender(chr)) && (checkerSettings.checkGlyphs))
								itemError.insert(MissingGlyph, 0);
						}
						continue;
					}
					if ((chr == 30) || (chr == 23))
					{
						for (uint numco = 0x30; numco < 0x3A; ++numco)
						{
							if ((!currItem->itemText.charStyle(e).font().canRender(numco)) && (checkerSettings.checkGlyphs))
								itemError.insert(MissingGlyph, 0);
						}
						continue;
					}
					if ((!currItem->itemText.charStyle(e).font().canRender(chr)) && (checkerSettings.checkGlyphs))
						itemError.insert(MissingGlyph, 0);
				}
	#endif
			}
			if (((currItem->fillColor() != CommonStrings::None) || (currItem->lineColor() != CommonStrings::None)) && (checkerSettings.checkNotCMYKOrSpot))
			{
				bool rgbUsed = false;
				if ((currItem->fillColor() != CommonStrings::None))
				{
					ScColor tmpC = currDoc->PageColors[currItem->fillColor()];
					if (tmpC.getColorModel() == colorModelRGB)
						rgbUsed = true;
				}
				if ((currItem->lineColor() != CommonStrings::None))
				{
					ScColor tmpC = currDoc->PageColors[currItem->lineColor()];
					if (tmpC.getColorModel() == colorModelRGB)
						rgbUsed = true;
				}
				if (rgbUsed)
					itemError.insert(NotCMYKOrSpot, 0);
			}
			if (itemError.count() != 0)
				currDoc->docItemErrors.insert(currItem, itemError);
		}
		allItems.clear();
	}
	return ((currDoc->docItemErrors.count() != 0) || (currDoc->masterItemErrors.count() != 0) || (currDoc->docLayerErrors.count() != 0));
}
