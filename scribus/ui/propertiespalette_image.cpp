/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/


#include "propertiespalette_image.h"

#if defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <QLocale>
#include <QSignalBlocker>

#include "extimageprops.h"
#include "iconmanager.h"
#include "localemgr.h"
#include "pageitem.h"
#include "propertiespalette_utils.h"
#include "propertiespalette.h"
#include "scribuscore.h"
#include "scribusapp.h"
#include "scribusview.h"
#include "selection.h"
#include "units.h"
#include "undomanager.h"

PropertiesPalette_Image::PropertiesPalette_Image( QWidget* parent) : QWidget(parent)
{
	userActionSniffer = new UserActionSniffer(this);
	connect(userActionSniffer, SIGNAL(actionStart()), this, SLOT(spinboxStartUserAction()));
	connect(userActionSniffer, SIGNAL(actionEnd()), this, SLOT(spinboxFinishUserAction()));

	setupUi(this);

	imagePageNumber->setMinimum(0);
	imagePageNumber->setSpecialValueText( tr( "Auto" ));
	imagePageNumber->setDecimals(0);
	imagePageNumber->setSuffix("");
	installSniffer(imagePageNumber);
	
	imageRotation->setWrapping( true );
	imageRotation->setNewUnit(6);
	installSniffer(imageRotation);

	installSniffer(imageXScaleSpinBox);
	installSniffer(imageYScaleSpinBox);
	installSniffer(imgDpiX);
	installSniffer(imgDpiY);

	keepImageWHRatioButton->setCheckable( true );
	keepImageWHRatioButton->setAutoRaise( true );

	sectionImageDimensions->expand();
	sectionImageDimensions->setCanSaveState(true);
	sectionImageDimensions->restorePreferences();

	sectionImageEffects->collapse();
	sectionImageEffects->setCanSaveState(true);
	sectionImageEffects->restorePreferences();

	sectionImageColorManagement->collapse();
	sectionImageColorManagement->setCanSaveState(true);
	sectionImageColorManagement->restorePreferences();

	sectionImagePDF->collapse();
	sectionImagePDF->setCanSaveState(true);
	sectionImagePDF->restorePreferences();

	sectionImageLayers->collapse();
	sectionImageLayers->setCanSaveState(true);
	sectionImageLayers->restorePreferences();

	sectionImageClippingPaths->collapse();
	sectionImageClippingPaths->setCanSaveState(true);
	sectionImageClippingPaths->restorePreferences();

	iconSetChange();
	languageChange();

	connect(imagePageNumber    , SIGNAL(valueChanged(double)), this, SLOT(handleImagePageNumber()));
	connect(imageXScaleSpinBox , SIGNAL(valueChanged(double)), this, SLOT(handleXScale()));
	connect(imageYScaleSpinBox , SIGNAL(valueChanged(double)), this, SLOT(handleYScale()));
	connect(imageXOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(handleLocalXY()));
	connect(imageYOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(handleLocalXY()));
	connect(imageRotation      , SIGNAL(valueChanged(double)), this, SLOT(handleLocalRotation()));
	connect(imgDpiX            , SIGNAL(valueChanged(double)), this, SLOT(handleDpiX()));
	connect(imgDpiY            , SIGNAL(valueChanged(double)), this, SLOT(handleDpiY()));
	connect(keepImageWHRatioButton , SIGNAL(clicked())       , this, SLOT(handleImageAspectRatio()));
	connect(checkBoxAutoFit    , SIGNAL(toggled(bool))		 , this, SLOT(handleScaling()));
	connect(keepImageWHRatioButton, SIGNAL(toggled(bool))	 , this, SLOT(handleScaling()));
	connect(imgEffectsButton   , SIGNAL(clicked())           , this, SLOT(handleImageEffects()));
	connect(inputProfiles      , SIGNAL(textActivated(QString)), this, SLOT(handleProfile(QString)));
	connect(renderIntent       , SIGNAL(activated(int))      , this, SLOT(handleIntent()));
	connect(compressionMethod  , SIGNAL(activated(int))      , this, SLOT(handleCompressionMethod()));
	connect(compressionQuality , SIGNAL(activated(int))      , this, SLOT(handleCompressionQuality()));
	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(ScQApp, SIGNAL(localeChanged()), this, SLOT(localeChange()));
	connect(ScQApp, SIGNAL(labelVisibilityChanged(bool)), this, SLOT(toggleLabelVisibility(bool)));

}

void PropertiesPalette_Image::updateSpinBoxConstants()
{
	if (!m_haveDoc)
		return;
	if (m_doc->m_Selection->count() == 0)
		return;
	imageXOffsetSpinBox->setConstants(&m_doc->constants());
	imageYOffsetSpinBox->setConstants(&m_doc->constants());
}

void PropertiesPalette_Image::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}

void PropertiesPalette_Image::setMainWindow(ScribusMainWindow* mw)
{
	m_ScMW = mw;

	connect(m_ScMW, SIGNAL(UpdateRequest(int)), this  , SLOT(handleUpdateRequest(int)));
}

void PropertiesPalette_Image::setDoc(ScribusDoc *d)
{
	if ((d == (ScribusDoc*) m_doc) || (m_ScMW && m_ScMW->scriptIsRunning()))
		return;

	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}

	m_doc  = d;
	m_item = nullptr;
	m_unitRatio   = m_doc->unitRatio();
	m_unitIndex   = m_doc->unitIndex();
	int precision = unitGetPrecisionFromIndex(m_unitIndex);
	double maxXYWHVal =  16777215 * m_unitRatio;

	m_haveDoc  = true;
	m_haveItem = false;

	imageXOffsetSpinBox->setValues( -16777215, maxXYWHVal, precision, 0);
	imageYOffsetSpinBox->setValues( -16777215, maxXYWHVal, precision, 0);

	imageRotation->setValues( 0, 359.99, 1, 0);

	imageXScaleSpinBox->setValues( 1, 30000, 2, 1);
	imageYScaleSpinBox->setValues( 1, 30000, 2, 1);
	imgDpiX->setValues( 1, 30000, 2, 1);
	imgDpiY->setValues( 1, 30000, 2, 1);
	updateSpinBoxConstants();

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
}

void PropertiesPalette_Image::unsetDoc()
{
	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}

	m_haveDoc  = false;
	m_haveItem = false;
	m_doc   = nullptr;
	m_item  = nullptr;

	setEnabled(false);
}

void PropertiesPalette_Image::unsetItem()
{
	m_haveItem = false;
	m_item     = nullptr;
	handleSelectionChanged();
}

PageItem* PropertiesPalette_Image::currentItemFromSelection()
{
	PageItem *currentItem = nullptr;

	if (m_doc)
	{
		if (m_doc->m_Selection->count() > 1)
		{
			currentItem = m_doc->m_Selection->itemAt(0);
		}
		else if (m_doc->m_Selection->count() == 1)
		{
			currentItem = m_doc->m_Selection->itemAt(0);
		}
	}

	return currentItem;
}

void PropertiesPalette_Image::installSniffer(ScrSpinBox *spinBox)
{
	const QList<QObject*>& list = spinBox->children();
	if (!list.isEmpty())
	{
		QListIterator<QObject*> it(list);
		QObject *obj;
		while (it.hasNext())
		{
			obj = it.next();
			obj->installEventFilter(userActionSniffer);
		}
	}
}

void PropertiesPalette_Image::installSniffer(QSpinBox *spinBox)
{
	const QList<QObject*>& list = spinBox->children();
	if (!list.isEmpty())
	{
		QListIterator<QObject*> it(list);
		QObject *obj;
		while (it.hasNext())
		{
			obj = it.next();
			obj->installEventFilter(userActionSniffer);
		}
	}
}

void PropertiesPalette_Image::updateProfileList()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if (m_haveDoc)
	{
		if (ScCore->haveCMS() && m_doc->cmsSettings().CMSinUse)
			sectionImageColorManagement->show();
		else
		{
			sectionImageColorManagement->hide();
			return;
		}

		inputProfiles->blockSignals(true);
		renderIntent->blockSignals(true);

		inputProfiles->clear();
		if (m_haveItem)
		{
			if (m_item->pixm.imgInfo.colorspace == ColorSpaceCMYK)
			{
				ScProfileInfoMap::Iterator itPend = ScCore->InputProfilesCMYK.end();
				for (auto itP = ScCore->InputProfilesCMYK.begin(); itP != itPend; ++itP)
				{
					inputProfiles->addItem(itP.key());
					if (itP.key() == m_item->ImageProfile)
						inputProfiles->setCurrentIndex(inputProfiles->count() - 1);
				}
				if (!ScCore->InputProfilesCMYK.contains(m_item->ImageProfile))
				{
					inputProfiles->addItem(m_item->ImageProfile);
					inputProfiles->setCurrentIndex(inputProfiles->count() - 1);
				}
				else
				{
					if (!m_item->EmbeddedProfile.isEmpty())
						inputProfiles->addItem(m_item->EmbeddedProfile);
				}
			}
			else
			{
				ScProfileInfoMap::Iterator itPend = ScCore->InputProfiles.end();
				for (auto itP = ScCore->InputProfiles.begin(); itP != itPend; ++itP)
				{
					inputProfiles->addItem(itP.key());
					if (itP.key() == m_item->ImageProfile)
						inputProfiles->setCurrentIndex(inputProfiles->count() - 1);
				}
				if (!ScCore->InputProfiles.contains(m_item->ImageProfile))
				{
					inputProfiles->addItem(m_item->ImageProfile);
					inputProfiles->setCurrentIndex(inputProfiles->count() - 1);
				}
				else
				{
					if (!m_item->EmbeddedProfile.isEmpty())
						inputProfiles->addItem(m_item->EmbeddedProfile);
				}
			}
			renderIntent->setCurrentIndex(m_item->ImageIntent);
		}

		inputProfiles->blockSignals(false);
		renderIntent->blockSignals(false);
	}
}

void PropertiesPalette_Image::showCMSOptions()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if (m_haveItem)
		updateProfileList();
	else if (m_doc)
		sectionImageColorManagement->setVisible(ScCore->haveCMS() && m_doc->cmsSettings().CMSinUse);
}

void PropertiesPalette_Image::showImageRotation(double rot)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	double rrR = rot;
	if (rot > 0)
		rrR = 360 - rrR;
	imageRotation->showValue(fabs(rrR));
}

void PropertiesPalette_Image::showScaleAndOffset(double scx, double scy, double x, double y)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	if (m_haveItem)
	{
		imageXOffsetSpinBox->showValue(x * m_unitRatio * m_item->imageXScale());
		imageYOffsetSpinBox->showValue(y * m_unitRatio * m_item->imageYScale());
		imageXScaleSpinBox->showValue(scx * 100 / 72.0 * m_item->pixm.imgInfo.xres);
		imageYScaleSpinBox->showValue(scy * 100 / 72.0 * m_item->pixm.imgInfo.yres);
		imgDpiX->showValue(qRound(720.0 / m_item->imageXScale()) / 10.0);
		imgDpiY->showValue(qRound(720.0 / m_item->imageYScale()) / 10.0);
	}
	else
	{
		imageXOffsetSpinBox->showValue(x * m_unitRatio);
		imageYOffsetSpinBox->showValue(y * m_unitRatio);
		imageXScaleSpinBox->showValue(scx * 100);
		imageYScaleSpinBox->showValue(scy * 100);
		imgDpiX->showValue(72);
		imgDpiY->showValue(72);
	}

}

void PropertiesPalette_Image::handleSelectionChanged()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem* currItem = currentItemFromSelection();
	if (m_doc->m_Selection->count() > 1)
	{
		setEnabled(false);
	}
	else
	{
		int itemType = currItem ? (int) currItem->itemType() : -1;
		m_haveItem = (itemType != -1);

		switch (itemType)
		{
		case -1:
			setEnabled(false);
			break;
		case PageItem::ImageFrame:
		case PageItem::LatexFrame:
		case PageItem::OSGFrame:
			setEnabled(currItem->asOSGFrame() == nullptr);
			break;
		case PageItem::TextFrame:
			setEnabled(false);
			break;
		case PageItem::Line:
			setEnabled(false);
			break;
		case PageItem::Arc:
		case PageItem::ItemType1:
		case PageItem::ItemType3:
		case PageItem::Polygon:
		case PageItem::RegularPolygon:
			setEnabled(false);
			break;
		case PageItem::PolyLine:
			setEnabled(false);
			break;
		case PageItem::PathText:
			setEnabled(false);
			break;
		case PageItem::Symbol:
			setEnabled(false);
			break;
		}
	}
	if (currItem)
	{
		setCurrentItem(currItem);
	}
	updateGeometry();
}

void PropertiesPalette_Image::handleUpdateRequest(int updateFlags)
{
	if (updateFlags & reqCmsOptionsUpdate)
		showCMSOptions();
}

void PropertiesPalette_Image::setCurrentItem(PageItem *item)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	//CB We shouldn't really need to process this if our item is the same one
	//maybe we do if the item has been changed by scripter.. but that should probably
	//set some status if so.
	//FIXME: This won't work until when a canvas deselect happens, m_item must be nullptr.
	//if (m_item == i)
	//	return;

	if (!m_doc)
		setDoc(item->doc());

	m_haveItem = false;
	m_item = item;

	imageLayers->setCurrentItem(m_item, m_ScMW->view);
	clippingPaths->setCurrentItem(m_item, m_ScMW->view);

	if (m_item->isImageFrame())
	{
		ImageInfoRecord *info = &m_item->pixm.imgInfo;

		imagePageNumber->blockSignals(true);
		if (m_item->imageIsAvailable)
			imagePageNumber->setMaximum(info->numberOfPages);
		imagePageNumber->setValue(info->actualPageNumber);
		imagePageNumberLabel->setVisible(info->numberOfPages > 1);
		imagePageNumber->blockSignals(false);

		compressionMethod->setCurrentIndex(m_item->OverrideCompressionMethod ? m_item->CompressionMethodIndex + 1 : 0);
		compressionQuality->setCurrentIndex(m_item->OverrideCompressionQuality ? m_item->CompressionQualityIndex + 1 : 0);

		imageXScaleSpinBox->blockSignals(true);
		imageYScaleSpinBox->blockSignals(true);
		imageXOffsetSpinBox->blockSignals(true);
		imageYOffsetSpinBox->blockSignals(true);
		imgDpiX->blockSignals(true);
		imgDpiY->blockSignals(true);
		keepImageWHRatioButton->blockSignals(true);
		checkBoxAutoFit->blockSignals(true);
		imageRotation->blockSignals(true);

		sectionImageEffects->setVisible(m_item->imageIsAvailable && m_item->isRaster);

		if (m_item->imageIsAvailable && info->valid)
		{
			sectionImageLayers->setVisible(info->layerInfo.count() != 0);
			sectionImageClippingPaths->setVisible(info->PDSpathData.count() != 0);
		}
		else
		{
			sectionImageLayers->setVisible(false);
			sectionImageClippingPaths->setVisible(false);
		}

		bool setter = m_item->ScaleType;
		checkBoxAutoFit->setChecked(!setter);

		if ((m_item->isLatexFrame()) || (m_item->isOSGFrame()))
		{
			checkBoxAutoFit->setEnabled(false);
			imageXScaleSpinBox->setEnabled(false);
			imageYScaleSpinBox->setEnabled(false);
			imgDpiX->setEnabled(false);
			imgDpiY->setEnabled(false);
		}
		else
		{
			imageXScaleSpinBox->setEnabled(setter);
			imageYScaleSpinBox->setEnabled(setter);
			imgDpiX->setEnabled(setter);
			imgDpiY->setEnabled(setter);
			checkBoxAutoFit->setEnabled(true);
			keepImageWHRatioButton->setChecked(m_item->AspectRatio);
		}

		imageXScaleSpinBox->blockSignals(false);
		imageYScaleSpinBox->blockSignals(false);
		imageXOffsetSpinBox->blockSignals(false);
		imageYOffsetSpinBox->blockSignals(false);
		imgDpiX->blockSignals(false);
		imgDpiY->blockSignals(false);
		keepImageWHRatioButton->blockSignals(false);
		checkBoxAutoFit->blockSignals(false);
		imageRotation->blockSignals(false);
	}
	m_haveItem = true;

	showScaleAndOffset(m_item->imageXScale(), m_item->imageYScale(), m_item->imageXOffset(), m_item->imageYOffset());

	double rrR = m_item->imageRotation();
	if (m_item->imageRotation() > 0)
		rrR = 360 - rrR;
	imageRotation->showValue(fabs(rrR));

	if (m_item->isImageFrame())
	{
		updateProfileList();
	}
	if (m_item->isOSGFrame() || m_item->isSymbol())
	{
		setEnabled(false);
	}
	updateSpinBoxConstants();
}

void PropertiesPalette_Image::handleLocalXY()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_doc->itemSelection_SetImageOffset(imageXOffsetSpinBox->value() / m_unitRatio / m_item->imageXScale(), imageYOffsetSpinBox->value() / m_unitRatio / m_item->imageYScale());
}

void PropertiesPalette_Image::handleLocalScale()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if (m_haveDoc && m_haveItem)
	{
		//CB Don't pass in the scale to the offset change as its taken from the new scale
		m_doc->itemSelection_SetImageScaleAndOffset(imageXScaleSpinBox->value() / 100.0 / m_item->pixm.imgInfo.xres * 72.0, imageYScaleSpinBox->value() / 100.0 / m_item->pixm.imgInfo.yres * 72.0, imageXOffsetSpinBox->value() / m_unitRatio, imageYOffsetSpinBox->value() / m_unitRatio);
		imgDpiX->showValue(qRound(720.0 / m_item->imageXScale()) / 10.0);
		imgDpiY->showValue(qRound(720.0 / m_item->imageYScale()) / 10.0);
	}
}

void PropertiesPalette_Image::handleLocalDpi()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if (m_haveDoc && m_haveItem)
	{
		//CB Don't pass in the scale to the offset change as its taken from the new scale
		m_doc->itemSelection_SetImageScaleAndOffset(72.0 / imgDpiX->value(), 72.0 / imgDpiY->value(), imageXOffsetSpinBox->value() / m_unitRatio, imageYOffsetSpinBox->value() / m_unitRatio);
		imageXScaleSpinBox->showValue(m_item->imageXScale() * 100 / 72.0 * m_item->pixm.imgInfo.xres);
		imageYScaleSpinBox->showValue(m_item->imageYScale() * 100 / 72.0 * m_item->pixm.imgInfo.yres);
	}
}

void PropertiesPalette_Image::handleLocalRotation()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if (m_haveDoc && m_haveItem)
	{
		m_doc->itemSelection_SetImageRotation(360 - imageRotation->value());
		if (checkBoxAutoFit->isChecked())
		{
			m_item->adjustPictScale();
			m_item->update();
		}
	}
}

void PropertiesPalette_Image::handleScaling()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	if (checkBoxAutoFit == sender())
	{
		imageXScaleSpinBox->setEnabled(!checkBoxAutoFit->isChecked());
		imageYScaleSpinBox->setEnabled(!checkBoxAutoFit->isChecked());
		imgDpiX->setEnabled(!checkBoxAutoFit->isChecked());
		imgDpiY->setEnabled(!checkBoxAutoFit->isChecked());
	}

	if (m_haveDoc && m_haveItem)
	{
		m_item->setImageScalingMode(!checkBoxAutoFit->isChecked(), keepImageWHRatioButton->isChecked());
		m_doc->changed();
		m_doc->changedPagePreview();
		emit UpdtGui(PageItem::ImageFrame);
	}
}

void PropertiesPalette_Image::handleXScale()
{
	bool xscaleBlocked = imageXScaleSpinBox->blockSignals(true);
	bool yscaleBlocked = imageYScaleSpinBox->blockSignals(true);
	if (keepImageWHRatioButton->isChecked())
		imageYScaleSpinBox->setValue(imageXScaleSpinBox->value());
	handleLocalScale();
	imageXScaleSpinBox->blockSignals(xscaleBlocked);
	imageYScaleSpinBox->blockSignals(yscaleBlocked);
}

void PropertiesPalette_Image::handleYScale()
{
	bool xscaleBlocked = imageXScaleSpinBox->blockSignals(true);
	bool yscaleBlocked = imageYScaleSpinBox->blockSignals(true);
	if (keepImageWHRatioButton->isChecked())
		imageXScaleSpinBox->setValue(imageYScaleSpinBox->value());
	handleLocalScale();
	imageXScaleSpinBox->blockSignals(xscaleBlocked);
	imageYScaleSpinBox->blockSignals(yscaleBlocked);
}

void PropertiesPalette_Image::handleDpiX()
{
	bool dpiXBlocked = imgDpiX->blockSignals(true);
	bool dpiYBlocked = imgDpiY->blockSignals(true);
	if (keepImageWHRatioButton->isChecked())
		imgDpiY->setValue(imgDpiX->value());
	handleLocalDpi();
	imgDpiX->blockSignals(dpiXBlocked);
	imgDpiY->blockSignals(dpiYBlocked);
}

void PropertiesPalette_Image::handleDpiY()
{
	bool dpiXBlocked = imgDpiX->blockSignals(true);
	bool dpiYBlocked = imgDpiY->blockSignals(true);
	if (keepImageWHRatioButton->isChecked())
		imgDpiX->setValue(imgDpiY->value());
	handleLocalDpi();
	imgDpiX->blockSignals(dpiXBlocked);
	imgDpiY->blockSignals(dpiYBlocked);
}

void PropertiesPalette_Image::handleImageAspectRatio()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	bool xscaleBlocked = imageXScaleSpinBox->blockSignals(true);
	bool yscaleBlocked = imageYScaleSpinBox->blockSignals(true);
	bool dpiXBlocked = imgDpiX->blockSignals(true);
	bool dpiYBlocked = imgDpiY->blockSignals(true);
	if (keepImageWHRatioButton->isChecked())
	{
		double maxXY = qMax(imageXScaleSpinBox->value(), imageYScaleSpinBox->value());
		imageXScaleSpinBox->setValue(maxXY);
		imageYScaleSpinBox->setValue(maxXY);
		handleLocalScale();

		double minXY = qMin(imgDpiX->value(), imgDpiY->value());
		imgDpiX->setValue(minXY);
		imgDpiY->setValue(minXY);
		handleLocalDpi();
	}
	imageXScaleSpinBox->blockSignals(xscaleBlocked);
	imageYScaleSpinBox->blockSignals(yscaleBlocked);
	imgDpiX->blockSignals(dpiXBlocked);
	imgDpiY->blockSignals(dpiYBlocked);
}

void PropertiesPalette_Image::handleImageEffects()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_ScMW->ImageEffects();
}

void PropertiesPalette_Image::handleImagePageNumber()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	bool reallynew = (m_item->pixm.imgInfo.actualPageNumber != imagePageNumber->value());
	if (UndoManager::undoEnabled())
	{
		SimpleState *ss = new SimpleState(Um::PageNmbr.arg(static_cast<int>(imagePageNumber->value())),"",Um::IImageFrame);
		ss->set("IMAGE_NBR");
		ss->set("OLD", m_item->pixm.imgInfo.actualPageNumber);
		ss->set("NEW", imagePageNumber->value());
		UndoManager::instance()->action(m_item,ss);
	}
	m_item->pixm.imgInfo.actualPageNumber = static_cast<int>(imagePageNumber->value());
	if (reallynew)
		m_item->loadImage(m_item->externalFile(), true);
	m_item->update();
}

void PropertiesPalette_Image::handleProfile(const QString& prn)
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_doc->itemSelection_SetColorProfile(inputProfiles->currentText());
}

void PropertiesPalette_Image::handleIntent()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_doc->itemSelection_SetRenderIntent(renderIntent->currentIndex());
}
 
void PropertiesPalette_Image::handleCompressionMethod()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_doc->itemSelection_SetCompressionMethod(compressionMethod->currentIndex() - 1);
}

void PropertiesPalette_Image::handleCompressionQuality()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_doc->itemSelection_SetCompressionQuality(compressionQuality->currentIndex() - 1);
}


void PropertiesPalette_Image::iconSetChange()
{
	IconManager& im = IconManager::instance();

	xscaleLabel->setPixmap(im.loadPixmap("scale-width"));
	yscaleLabel->setPixmap(im.loadPixmap("scale-height"));
	imageRotationLabel->setPixmap(im.loadPixmap("object-rotation"));
}

void PropertiesPalette_Image::languageChange()
{
	retranslateUi(this);

	imagePageNumber->setSpecialValueText( tr( "Auto" ));

	QSignalBlocker renderIntentBlocker(renderIntent);
	int oldRenderI = renderIntent->currentIndex();
	renderIntent->clear();
	renderIntent->addItem( tr("Perceptual"));
	renderIntent->addItem( tr("Relative Colorimetric"));
	renderIntent->addItem( tr("Saturation"));
	renderIntent->addItem( tr("Absolute Colorimetric"));
	renderIntent->setCurrentIndex(oldRenderI);

	QSignalBlocker compressionMethodBlocker(compressionMethod);
	int oldCompressionMethod = compressionMethod->currentIndex();
	compressionMethod->clear();
	compressionMethod->addItem( tr( "Global" ) );
	compressionMethod->addItem( tr( "Automatic" ) );
	compressionMethod->addItem( tr( "Lossy - JPEG" ) );
	compressionMethod->addItem( tr( "Lossless - Zip" ) );
	compressionMethod->addItem( tr( "None" ) );
	compressionMethod->setCurrentIndex(oldCompressionMethod);

	QSignalBlocker compressionQualityBlocker(compressionQuality);
	int oldCompressionQuality = compressionQuality->currentIndex();
	compressionQuality->clear();
	compressionQuality->addItem( tr( "Global" ) );
	compressionQuality->addItem( tr( "Maximum" ) );
	compressionQuality->addItem( tr( "High" ) );
	compressionQuality->addItem( tr( "Medium" ) );
	compressionQuality->addItem( tr( "Low" ) );
	compressionQuality->addItem( tr( "Minimum" ) );
	compressionQuality->setCurrentIndex(oldCompressionQuality);

	QString pctSuffix = tr(" %");
	imageXScaleSpinBox->setSuffix(pctSuffix);
	imageYScaleSpinBox->setSuffix(pctSuffix);

	QString ptSuffix = tr(" pt");
	QString suffix   = (m_haveDoc) ? unitGetSuffixFromIndex(m_doc->unitIndex()) : ptSuffix;
	imageXOffsetSpinBox->setSuffix(suffix);
	imageYOffsetSpinBox->setSuffix(suffix);

	QString dpiSuffix = tr(" dpi");
	imgDpiX->setSuffix(dpiSuffix);
	imgDpiY->setSuffix(dpiSuffix);
}

void PropertiesPalette_Image::unitChange()
{
	if (!m_doc)
		return;

	QSignalBlocker imageXOffsetSpinBoxBlocker(imageXOffsetSpinBox);
	QSignalBlocker imageYOffsetSpinBoxBlocker(imageYOffsetSpinBox);

	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();

	imageXOffsetSpinBox->setNewUnit(m_unitIndex);
	imageYOffsetSpinBox->setNewUnit(m_unitIndex);
}

void PropertiesPalette_Image::localeChange()
{
	const QLocale& l(LocaleManager::instance().userPreferredLocale());
	imagePageNumber->setLocale(l);
	imageXScaleSpinBox->setLocale(l);
	imageYScaleSpinBox->setLocale(l);
	imageXOffsetSpinBox->setLocale(l);
	imageYOffsetSpinBox->setLocale(l);
	imageRotation->setLocale(l);
	imgDpiX->setLocale(l);
	imgDpiY->setLocale(l);
}

void PropertiesPalette_Image::toggleLabelVisibility(bool v)
{
	imagePageNumberLabel->setLabelVisibility(v);
	imageRotationLabel->setLabelVisibility(v);
	imageScaleLabel->setLabelVisibility(v);
}

bool PropertiesPalette_Image::userActionOn()
{
	return m_userActionOn;
}

void PropertiesPalette_Image::spinboxStartUserAction()
{
	m_userActionOn = true;
}

void PropertiesPalette_Image::spinboxFinishUserAction()
{
	m_userActionOn = false;

	for (int i = 0; i < m_doc->m_Selection->count(); ++i)
		m_doc->m_Selection->itemAt(i)->checkChanges(true);
	if (m_ScMW->view->groupTransactionStarted())
	{
		m_ScMW->view->endGroupTransaction();
	}
}


