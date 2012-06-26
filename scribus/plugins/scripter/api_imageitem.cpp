/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "api_imageitem.h"
#include "utils.h"
#include "selection.h"
#include "undomanager.h"
#include "sctextstream.h"
#include "ui/propertiespalette.h"
#include "ui/propertiespalette_image.h"

ImageAPI::ImageAPI(PageItem_ImageFrame* im) : ItemAPI(im)
{
	qDebug() << "ImageitemAPI loaded";
    setObjectName("ImageItemAPI");
	item = im;
}


double ImageAPI::xScale()
{
	return item->imageXScale();
}

void ImageAPI::setXScale(double value)
{
	item->setImageXScale(value);
}


double ImageAPI::yScale()
{
	return item->imageYScale();
}

void ImageAPI::setYScale(double value)
{
	item->setImageYScale(value);
}


double ImageAPI::xOffset()
{
	return item->imageXOffset();
}

void ImageAPI::setXOffset(double value)
{
	item->setImageXOffset(value);
}


double ImageAPI::yOffset()
{
	return item->imageYOffset();
}

void ImageAPI::setYOffset(double value)
{
	item->setImageYOffset(value);
}

void ImageAPI::setGrayscale()
{
    if (!checkHaveDocument())
        RAISE("No document open");
    if (item == NULL)
        return;
    if (! item->asImageFrame())
    {
        RAISE("Specified item not an image frame.");
        return;
    }

    ImageEffect ef;
    ef.effectCode = ScImage::EF_GRAYSCALE;

    item->effectsInUse.append(ef);
    item->pixm.applyEffect(item->effectsInUse, ScCore->primaryMainWindow()->doc->PageColors, false);

    ScCore->primaryMainWindow()->doc->updatePic();
}

void ImageAPI::load(QString filename)
{
	if (!checkHaveDocument())
		RAISE("No document open");
	if (item == NULL)
		return;
	if (!item->asImageFrame())
	{
		RAISE("Target is not an image frame.");
		return;
	}
	ScCore->primaryMainWindow()->doc->loadPict(filename, item);
}

void ImageAPI::scale(double x, double y)
{
	if (!checkHaveDocument())
		RAISE("No document open");
	if (item == NULL)
		return;
	if (! item->asImageFrame())
	{
		RAISE("Specified item not an image frame.");
		return;
	}

    Selection tempSelection(*ScCore->primaryMainWindow()->doc->m_Selection);
	bool hadOrigSelection = (tempSelection.count() != 0);

	ScCore->primaryMainWindow()->doc->m_Selection->clear();
    ScCore->primaryMainWindow()->view->Deselect();
    ScCore->primaryMainWindow()->view->SelectItem(item);

    ScCore->primaryMainWindow()->doc->itemSelection_SetImageScale(x, y);
	ScCore->primaryMainWindow()->doc->updatePic();

	// Now restore the selection.
	ScCore->primaryMainWindow()->view->Deselect();
	if (hadOrigSelection)
		*ScCore->primaryMainWindow()->doc->m_Selection=tempSelection;
}

void ImageAPI::offset(double x, double y)
{
	if (!checkHaveDocument())
		RAISE("No document open");
	if (item == NULL)
		return;
	if (! item->asImageFrame())
	{
		RAISE("Specified item not an image frame.");
		return;
	}

	// Grab the old selection - but use it only where is there any
	Selection tempSelection(*ScCore->primaryMainWindow()->doc->m_Selection);
	bool hadOrigSelection = (tempSelection.count() != 0);

	ScCore->primaryMainWindow()->doc->m_Selection->clear();
	// Clear the selection
	ScCore->primaryMainWindow()->view->Deselect();
	// Select the item, which will also select its group if
	// there is one.
	ScCore->primaryMainWindow()->view->SelectItem(item);

	// offset
	double newOffsetX = x / ((item->imageXScale() != 0.0) ? item->imageXScale() : 1);
	double newOffsetY = y / ((item->imageYScale() != 0.0) ? item->imageYScale() : 1);
	ScCore->primaryMainWindow()->doc->itemSelection_SetImageOffset(newOffsetX, newOffsetY);    //CB why when this is done above?
	ScCore->primaryMainWindow()->doc->updatePic();

	// Now restore the selection.
	ScCore->primaryMainWindow()->view->Deselect();
	if (hadOrigSelection)
		*ScCore->primaryMainWindow()->doc->m_Selection=tempSelection;
}

void ImageAPI::setBrightness(double n)
{
	if (!checkHaveDocument())
		RAISE("No document open");
	if (item == NULL)
		return ;
	if (! item->asImageFrame())
	{
		RAISE("Specified item not an image frame.");
		return;
	}

	ImageEffect ef;
	ef.effectCode = ScImage::EF_BRIGHTNESS;
	ScTextStream fp(&ef.effectParameters, QIODevice::WriteOnly);
	fp << n;

	item->effectsInUse.append(ef);
	item->pixm.applyEffect(item->effectsInUse, ScCore->primaryMainWindow()->doc->PageColors, false);

	ScCore->primaryMainWindow()->doc->updatePic();
}

void ImageAPI::scaleToFrame(bool scaletoframe, bool Proportional)
{
	bool scaleToFrame = false;
	bool proportional = true;
	
	scaleToFrame = scaletoframe;
	proportional = Proportional;
	if (!checkHaveDocument())
		RAISE("No document open.");
	if (item == NULL)
		return;
	if (! item->asImageFrame())
	{
		RAISE("Specified item not an image frame.");
		return;
	}
	// Set the item to scale if appropriate. ScaleType 1 is free
	// scale, 0 is scale to frame.
	item->ScaleType = scaleToFrame == 0;
	// Now, if the user has chosen to set the proportional mode,
	// set it. 1 is proportional, 0 is free aspect.
	if (proportional != -1)
		item->AspectRatio = proportional > 0;
	// Force the braindead app to notice the changes

	//FIXME emit or something so we dont need this
	ScCore->primaryMainWindow()->propertiesPalette->imagePal->displayScaleAndOffset(item->imageXScale(), item->imageYScale(), item->imageXOffset(), item->imageYOffset());
	item->AdjustPictScale();
	//ScCore->primaryMainWindow()->view->AdjustPictScale(item);

	item->update();
}

ImageAPI::~ImageAPI()
{
	qDebug() << "ImageitemAPI deleted";
}


