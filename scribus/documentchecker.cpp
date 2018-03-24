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
#include "pageitem.h"
#include "pdf_analyzer.h"
#include "sccolor.h"
#include "sclayer.h"
#include "scpage.h"
#include "scribusdoc.h"
#include "scribusstructs.h"
#include "text/textlayoutpainter.h"
#include "util.h"
#include "util_formats.h"

#include <QList>


class MissingGlyphsPainter: public TextLayoutPainter
{
	errorCodes& m_itemError;
//	const TextLayout& m_textLayout;

public:
	MissingGlyphsPainter(errorCodes& itemError, const TextLayout& textLayout)
		: m_itemError(itemError)
//		, m_textLayout(textLayout)
	{ }

	void drawGlyph(const GlyphCluster& gc)
	{
		if (gc.isEmpty())
		{
			int pos = gc.firstChar();
			m_itemError.insert(MissingGlyph, pos);
		}
	}
	void drawGlyphOutline(const GlyphCluster& gc, bool)
	{
		drawGlyph(gc);
	}
	void drawLine(QPointF, QPointF) { }
	void drawRect(QRectF) { }
	void drawObject(PageItem*) { }
};

bool isPartFilledImageFrame(PageItem * currItem)
{
	double imageRealWidth  = currItem->imageXScale() * currItem->pixm.imgInfo.lowResScale * currItem->pixm.width();
	double imageRealHeight = currItem->imageYScale() * currItem->pixm.imgInfo.lowResScale * currItem->pixm.height();
//	qDebug() << "X" << currItem->width() << imageRealWidth;
//	qDebug() << "Y" << currItem->height() << imageRealHeight;
	return ((currItem->width() - imageRealWidth) > 0.05 || (currItem->height() - imageRealHeight) > 0.05);
}


bool DocumentChecker::checkDocument(ScribusDoc *currDoc)
{
	struct CheckerPrefs checkerSettings;
	checkerSettings=currDoc->checkerProfiles()[currDoc->curCheckProfile()];
	currDoc->pageErrors.clear();
	currDoc->docItemErrors.clear();
	currDoc->masterItemErrors.clear();
	currDoc->docLayerErrors.clear();

	checkPages(currDoc, checkerSettings);
	checkLayers(currDoc, checkerSettings);
	//update all marks references and check if that changes anything in doc
	currDoc->setNotesChanged(currDoc->updateMarks(true));

	checkItems(currDoc, checkerSettings);

	return (currDoc->hasPreflightErrors());
}

void DocumentChecker::checkPages(ScribusDoc *currDoc, struct CheckerPrefs checkerSettings)
{
	errorCodes pageError;
	for (int i=0; i < currDoc->DocPages.count(); ++i )
	{
		pageError.clear();
		if (checkerSettings.checkAppliedMasterDifferentSide)
		{
			bool error = false;
			int masterPageNumber = -1, masterPageLocation = -1;
			PageLocation pageLoc = currDoc->locationOfPage(i);
			masterPageNumber = currDoc->MasterNames.value(currDoc->DocPages[i]->MPageNam, -1);
			if (masterPageNumber >= 0)
				masterPageLocation = currDoc->MasterPages[masterPageNumber]->LeftPg;
			if (currDoc->pagePositioning() == singlePage)
			{
				if ((pageLoc != LeftPage) || (masterPageLocation != 0))
					error = true;
			}
			else
			{
				if (pageLoc==LeftPage && masterPageLocation==1)
					error = false;
				else if (pageLoc==RightPage && masterPageLocation==0)
					error = false;
				else if (pageLoc==MiddlePage && masterPageLocation==2)
					error = false;
				else
					error = true;
			}
			if (error)
				pageError.insert(AppliedMasterDifferentSide, 0);
		}
		if (pageError.count() != 0)
			currDoc->pageErrors.insert(i, pageError);
	}
}

void DocumentChecker::checkLayers(ScribusDoc *currDoc, struct CheckerPrefs checkerSettings)
{
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
}

void DocumentChecker::checkItems(ScribusDoc *currDoc, struct CheckerPrefs checkerSettings)
{
	errorCodes itemError;

	QList<PageItem*> allItems;
	uint masterItemsCount = currDoc->MasterItems.count();
	for (uint i = 0; i < masterItemsCount; ++i)
	{
		PageItem* currItem = currDoc->MasterItems.at(i);
		if (currItem->isGroup())
			allItems = currItem->getAllChildren();
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
			if ((currItem->hasSoftShadow() || (currItem->fillTransparency() != 0.0) || (currItem->lineTransparency() != 0.0) || (currItem->fillBlendmode() != 0) || (currItem->lineBlendmode() != 0)) && (checkerSettings.checkTransparency))
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
				else if (currItem->GrType == 12)
				{
					for (int grow = 0; grow < currItem->meshGradientPatches.count(); grow++)
					{
						meshGradientPatch patch = currItem->meshGradientPatches[grow];
						if (currItem->meshGradientPatches[grow].TL.transparency != 1.0)
							itemError.insert(Transparency, 0);
						if (currItem->meshGradientPatches[grow].TR.transparency != 1.0)
							itemError.insert(Transparency, 0);
						if (currItem->meshGradientPatches[grow].BR.transparency != 1.0)
							itemError.insert(Transparency, 0);
						if (currItem->meshGradientPatches[grow].BL.transparency != 1.0)
							itemError.insert(Transparency, 0);
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

				if ((!currItem->imageIsAvailable) && (checkerSettings.checkPictures))
					itemError.insert(MissingImage, 0);
				else
				{
					if (currItem->imageIsAvailable)
					{
						if (checkerSettings.checkTransparency && currItem->pixm.hasSmoothAlpha())
							itemError.insert(Transparency, 0);
					}
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
							if (checkerSettings.checkNotCMYKOrSpot || checkerSettings.checkDeviceColorsAndOutputIntent)
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
								if (checkerSettings.checkDeviceColorsAndOutputIntent && currDoc->HasCMS)
								{
									for (int i=0; i<usedColorSpaces.size(); ++i)
									{
										if (currPrintProfCS == ColorSpace_Cmyk && (usedColorSpaces[i] == CS_DeviceRGB || usedColorSpaces[i] == CS_DeviceGray))
										{
											itemError.insert(DeviceColorsAndOutputIntent, 0);
											break;
										}
										else if (currPrintProfCS == ColorSpace_Rgb && (usedColorSpaces[i] == CS_DeviceCMYK || usedColorSpaces[i] == CS_DeviceGray))
										{
											itemError.insert(DeviceColorsAndOutputIntent, 0);
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
				if ( currItem->frameOverflows() && (checkerSettings.checkOverflow) && (!((currItem->isAnnotation()) && ((currItem->annotation().Type() == Annotation::Combobox) || (currItem->annotation().Type() == Annotation::Listbox)))))
					itemError.insert(TextOverflow, 0);

				if (checkerSettings.checkEmptyTextFrames && (currItem->itemText.length() == 0 || currItem->frameUnderflows()))
				{
					bool isLinkAnnotation = (currItem->isAnnotation() && (currItem->annotation().Type() == Annotation::Link));
					if (!isLinkAnnotation)
						itemError.insert(EmptyTextFrame, 0);
				}
				
				if (currItem->isAnnotation())
				{
					ScFace::FontFormat fformat = currItem->itemText.defaultStyle().charStyle().font().format();
					if (!(fformat == ScFace::SFNT || fformat == ScFace::TTCF))
						itemError.insert(WrongFontInAnnotation, 0);
				}

				if (checkerSettings.checkGlyphs)
				{
					if (currItem->invalid)
						currItem->layout();
					MissingGlyphsPainter p(itemError, currItem->textLayout);
					currItem->textLayout.render(&p);
				}
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
			allItems = currItem->getAllChildren();
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
			if ((currItem->hasSoftShadow() || (currItem->fillTransparency() != 0.0) || (currItem->lineTransparency() != 0.0) || (currItem->fillBlendmode() != 0) || (currItem->lineBlendmode() != 0)) && (checkerSettings.checkTransparency))
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
				else if (currItem->GrType == 12)
				{
					for (int grow = 0; grow < currItem->meshGradientPatches.count(); grow++)
					{
						meshGradientPatch patch = currItem->meshGradientPatches[grow];
						if (currItem->meshGradientPatches[grow].TL.transparency != 1.0)
							itemError.insert(Transparency, 0);
						if (currItem->meshGradientPatches[grow].TR.transparency != 1.0)
							itemError.insert(Transparency, 0);
						if (currItem->meshGradientPatches[grow].BR.transparency != 1.0)
							itemError.insert(Transparency, 0);
						if (currItem->meshGradientPatches[grow].BL.transparency != 1.0)
							itemError.insert(Transparency, 0);
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

				if ((!currItem->imageIsAvailable) && (checkerSettings.checkPictures))
					itemError.insert(MissingImage, 0);
				else
				{
					if (currItem->imageIsAvailable)
					{
						if (checkerSettings.checkTransparency && currItem->pixm.hasSmoothAlpha())
							itemError.insert(Transparency, 0);
					}
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
							if (checkerSettings.checkNotCMYKOrSpot || checkerSettings.checkDeviceColorsAndOutputIntent)
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
								if (checkerSettings.checkDeviceColorsAndOutputIntent && currDoc->HasCMS)
								{
									for (int i=0; i<usedColorSpaces.size(); ++i)
									{
										if (currPrintProfCS == ColorSpace_Cmyk && (usedColorSpaces[i] == CS_DeviceRGB || usedColorSpaces[i] == CS_DeviceGray))
										{
											itemError.insert(DeviceColorsAndOutputIntent, 0);
											break;
										}
										else if (currPrintProfCS == ColorSpace_Rgb && (usedColorSpaces[i] == CS_DeviceCMYK || usedColorSpaces[i] == CS_DeviceGray))
										{
											itemError.insert(DeviceColorsAndOutputIntent, 0);
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
				if ( currItem->frameOverflows() && (checkerSettings.checkOverflow) && (!((currItem->isAnnotation()) && ((currItem->annotation().Type() == Annotation::Combobox) || (currItem->annotation().Type() == Annotation::Listbox)))))
					itemError.insert(TextOverflow, 0);

				if (checkerSettings.checkEmptyTextFrames && (currItem->itemText.length()==0 || currItem->frameUnderflows()))
				{
					bool isLinkAnnotation = (currItem->isAnnotation() && (currItem->annotation().Type() == Annotation::Link));
					if (!isLinkAnnotation)
						itemError.insert(EmptyTextFrame, 0);
				}

				if (currItem->isAnnotation())
				{
					ScFace::FontFormat fformat = currItem->itemText.defaultStyle().charStyle().font().format();
					if (!(fformat == ScFace::SFNT || fformat == ScFace::TTCF))
						itemError.insert(WrongFontInAnnotation, 0);
				}
				if (checkerSettings.checkGlyphs)
				{
					if (currItem->invalid)
						currItem->layout();
					MissingGlyphsPainter p(itemError, currItem->textLayout);
					currItem->textLayout.render(&p);
				}
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
}
