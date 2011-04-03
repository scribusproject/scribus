/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/


#include "propertiespalette_image.h"

#if defined(_MSC_VER)
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include "commonstrings.h"
#include "sccolorengine.h"
#include "pageitem.h"
#include "propertiespalette_utils.h"
#include "sccombobox.h"
#include "scribus.h"
#include "scribuscore.h"
#include "scraction.h"
#include "scribusview.h"
#include "selection.h"
#include "units.h"
#include "undomanager.h"
#include "util.h"
#include "util_icon.h"
#include "util_math.h"

PropertiesPalette_Image::PropertiesPalette_Image( QWidget* parent) : QWidget(parent)
{
	m_ScMW = 0;
	m_doc  = 0;
	m_haveDoc    = false;
	m_haveItem   = false;
	m_unitRatio  = 1.0;

	m_userActionOn    = false;
	userActionSniffer = new UserActionSniffer(this);
	connect(userActionSniffer, SIGNAL(actionStart()), this, SLOT(spinboxStartUserAction()));
	connect(userActionSniffer, SIGNAL(actionEnd()), this, SLOT(spinboxFinishUserAction()));

	setupUi(this);
	setSizePolicy( QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

	imagePageNumber->setMinimum(0);
	imagePageNumber->setSpecialValueText(tr( "Auto" ));
	imagePageNumberLabel->setBuddy(imagePageNumber);
	
//	freeScale = new QRadioButton( "&Free Scaling", this );
//	freeScale->setChecked( true );
//	pageLayout_4->addWidget( freeScale );

	xposImgLabel->setBuddy(imageXOffsetSpinBox);
	yposImgLabel->setBuddy(imageYOffsetSpinBox);
	
	imageRotation->setWrapping( true );
	installSniffer(imageRotation);
	imageRotationLabel->setBuddy(imageRotation);

	freeScale->setChecked( true );
	
	installSniffer(imageXScaleSpinBox);
	xscaleLabel->setBuddy(imageXScaleSpinBox);
	installSniffer(imageYScaleSpinBox);
	yscaleLabel->setBuddy(imageYScaleSpinBox);

	keepImageWHRatioButton->setCheckable( true );
	keepImageWHRatioButton->setAutoRaise( true );

	installSniffer(imgDpiX);
	imgDPIXLabel->setBuddy(imgDpiX);

	installSniffer(imgDpiY);
	imgDPIYLabel->setBuddy(imgDpiY);

	keepImageDPIRatioButton->setCheckable( true );
	keepImageDPIRatioButton->setAutoRaise( true );

	frameScale->setText( "&To Frame Size" );

	cbProportional->setEnabled( false );
	cbProportional->setText( "P&roportional" );
	cbProportional->setChecked( true );

	languageChange();

	connect(imagePageNumber    , SIGNAL(valueChanged(int))   , this, SLOT(handleImagePageNumber()));
	connect(imageXScaleSpinBox , SIGNAL(valueChanged(double)), this, SLOT(handleXScale()));
	connect(imageYScaleSpinBox , SIGNAL(valueChanged(double)), this, SLOT(handleYScale()));
	connect(imageXOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(handleLocalXY()));
	connect(imageYOffsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(handleLocalXY()));
	connect(imageRotation      , SIGNAL(valueChanged(double)), this, SLOT(handleLocalRotation()));
	connect(imgDpiX            , SIGNAL(valueChanged(double)), this, SLOT(handleDpiX()));
	connect(imgDpiY            , SIGNAL(valueChanged(double)), this, SLOT(handleDpiY()));
	connect(keepImageWHRatioButton , SIGNAL(clicked())       , this, SLOT(handleImageWHRatio()));
	connect(keepImageDPIRatioButton, SIGNAL(clicked())       , this, SLOT(handleImageDPIRatio()));
	connect(freeScale          , SIGNAL(clicked())           , this, SLOT(handleScaling()));
	connect(frameScale         , SIGNAL(clicked())           , this, SLOT(handleScaling()));
	connect(cbProportional     , SIGNAL(clicked())           , this, SLOT(handleScaling()));
	connect(imgEffectsButton   , SIGNAL(clicked())           , this, SLOT(handleImageEffects()));
	connect(imgExtProperties   , SIGNAL(clicked())           , this, SLOT(handleExtImgProperties()));
	connect(inputProfiles      , SIGNAL(activated(const QString&)), this, SLOT(handleProfile(const QString&)));
	connect(renderIntent       , SIGNAL(activated(int))      , this, SLOT(handleIntent()));
	connect(compressionMethod  , SIGNAL(activated(int))      , this, SLOT(handleCompressionMethod()));
	connect(compressionQuality , SIGNAL(activated(int))      , this, SLOT(handleCompressionQuality()));
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

	connect(this  , SIGNAL(DocChanged())      , m_ScMW, SLOT(slotDocCh()));
	connect(m_ScMW, SIGNAL(UpdateRequest(int)), this  , SLOT(handleUpdateRequest(int)));
}

void PropertiesPalette_Image::setDoc(ScribusDoc *d)
{
	if((d == (ScribusDoc*) m_doc) || (m_ScMW && m_ScMW->scriptIsRunning()))
		return;

	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}

	m_doc  = d;
	m_item = NULL;
	m_unitRatio   = m_doc->unitRatio();
	m_unitIndex   = m_doc->unitIndex();
	int precision = unitGetPrecisionFromIndex(m_unitIndex);
	double maxXYWHVal =  16777215 * m_unitRatio;
	double minXYVal   = -16777215 * m_unitRatio;

	m_haveDoc  = true;
	m_haveItem = false;

	imageXOffsetSpinBox->setValues( -16777215, maxXYWHVal, precision, 0);
	imageYOffsetSpinBox->setValues( -16777215, maxXYWHVal, precision, 0);

	imageRotation->setValues( 0, 359.99, 1, 0);

	imageXScaleSpinBox->setValues( 1, 30000, 2, 1);
	imageYScaleSpinBox->setValues( 1, 30000, 2, 1);
	imgDpiX->setValues( 1, 30000, 2, 1);
	imgDpiY->setValues( 1, 30000, 2, 1);

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
	m_doc   = NULL;
	m_item  = NULL;

	setEnabled(false);
}

void PropertiesPalette_Image::unsetItem()
{
	m_haveItem = false;
	m_item     = NULL;
	handleSelectionChanged();
}

PageItem* PropertiesPalette_Image::currentItemFromSelection()
{
	PageItem *currentItem = NULL;

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
	const QList<QObject*> list = spinBox->children();
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
			colorMgmtGroup->show();
		else
		{
			colorMgmtGroup->hide();
			return;
		}

		inputProfiles->blockSignals(true);
		renderIntent->blockSignals(true);

		inputProfiles->clear();
		if (m_haveItem)
		{
			if (m_item->pixm.imgInfo.colorspace == ColorSpaceCMYK)
			{
				ProfilesL::Iterator itP;
				ProfilesL::Iterator itPend = ScCore->InputProfilesCMYK.end();
				for (itP = ScCore->InputProfilesCMYK.begin(); itP != itPend; ++itP)
				{
					inputProfiles->addItem(itP.key());
					if (itP.key() == m_item->IProfile)
						inputProfiles->setCurrentIndex(inputProfiles->count()-1);
				}
				if (!ScCore->InputProfilesCMYK.contains(m_item->IProfile))
				{
					inputProfiles->addItem(m_item->IProfile);
					inputProfiles->setCurrentIndex(inputProfiles->count()-1);
				}
				else
				{
					if (!m_item->EmProfile.isEmpty())
						inputProfiles->addItem(m_item->EmProfile);
				}
			}
			else
			{
				ProfilesL::Iterator itP;
				ProfilesL::Iterator itPend=ScCore->InputProfiles.end();
				for (itP = ScCore->InputProfiles.begin(); itP != itPend; ++itP)
				{
					inputProfiles->addItem(itP.key());
					if (itP.key() == m_item->IProfile)
						inputProfiles->setCurrentIndex(inputProfiles->count()-1);
				}
				if (!ScCore->InputProfiles.contains(m_item->IProfile))
				{
					inputProfiles->addItem(m_item->IProfile);
					inputProfiles->setCurrentIndex(inputProfiles->count()-1);
				}
				else
				{
					if (!m_item->EmProfile.isEmpty())
						inputProfiles->addItem(m_item->EmProfile);
				}
			}
			renderIntent->setCurrentIndex(m_item->IRender);
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
		colorMgmtGroup->setVisible(ScCore->haveCMS() && m_doc->cmsSettings().CMSinUse);
}

void PropertiesPalette_Image::displayImageRotation(double rot)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	double rrR = rot;
	if (rot > 0)
		rrR = 360 - rrR;
	imageRotation->showValue(fabs(rrR));
}

void PropertiesPalette_Image::displayScaleAndOffset(double scx, double scy, double x, double y)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	keepImageWHRatioButton->blockSignals(true);
	keepImageDPIRatioButton->blockSignals(true);
	if (fabs(scx - scy) > 0.0002)
	{
		keepImageWHRatioButton->setChecked(false);
		keepImageDPIRatioButton->setChecked(false);
	}
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
	keepImageWHRatioButton->blockSignals(false);
	keepImageDPIRatioButton->blockSignals(false);
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
			setEnabled(currItem->asOSGFrame() == NULL);
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
	repaint();
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
	//CB We shouldnt really need to process this if our item is the same one
	//maybe we do if the item has been changed by scripter.. but that should probably
	//set some status if so.
	//FIXME: This wont work until when a canvas deselect happens, m_item must be NULL.
	//if (m_item == i)
	//	return;

	if (!m_doc)
		setDoc(item->doc());

	m_haveItem = false;
	m_item = item;

	if (m_item->asImageFrame())
	{
		imagePageNumber->blockSignals(true);
		imagePageNumber->setMaximum(m_item->pixm.imgInfo.numberOfPages);
		imagePageNumber->setValue(m_item->pixm.imgInfo.actualPageNumber);
		compressionMethod->setCurrentIndex(m_item->OverrideCompressionMethod ? m_item->CompressionMethodIndex + 1 : 0);
		compressionQuality->setCurrentIndex(m_item->OverrideCompressionQuality ? m_item->CompressionQualityIndex + 1 : 0);
		imagePageNumber->blockSignals(false);

		imageXScaleSpinBox->blockSignals(true);
		imageYScaleSpinBox->blockSignals(true);
		imageXOffsetSpinBox->blockSignals(true);
		imageYOffsetSpinBox->blockSignals(true);
		imageRotation->blockSignals(true);

		imgEffectsButton->setShown(m_item->PictureIsAvailable && m_item->isRaster);
		imgExtProperties->setShown(m_item->PictureIsAvailable && m_item->pixm.imgInfo.valid);
		bool setter = m_item->ScaleType;
		freeScale->setChecked(setter);
		frameScale->setChecked(!setter);
		if ((m_item->asLatexFrame()) || (m_item->asOSGFrame()))
		{
			freeScale->setEnabled(false);
			frameScale->setEnabled(false);
			cbProportional->setEnabled(false);
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
			cbProportional->setEnabled(!setter);
			cbProportional->setChecked(m_item->AspectRatio);
			freeScale->setEnabled(true);
			frameScale->setEnabled(true);
		}
//CB Why do we need this? Setting it too much here
// 		if (setter == true)
// 		{
// 			keepImageWHRatioButton->setChecked(setter);
// 			keepImageDPIRatioButton->setChecked(setter);
// 		}
		//imageXOffsetSpinBox->setEnabled(setter);
		//imageYOffsetSpinBox->setEnabled(setter);
		//imageRotation->setEnabled(setter);

		imageXScaleSpinBox->blockSignals(false);
		imageYScaleSpinBox->blockSignals(false);
		imageXOffsetSpinBox->blockSignals(false);
		imageYOffsetSpinBox->blockSignals(false);
		imageRotation->blockSignals(false);
	}
	m_haveItem = true;

	displayScaleAndOffset(m_item->imageXScale(), m_item->imageYScale(), m_item->imageXOffset(), m_item->imageYOffset());
	double rrR = m_item->imageRotation();
	if (m_item->imageRotation() > 0)
		rrR = 360 - rrR;
	imageRotation->showValue(fabs(rrR));

	if (m_item->asImageFrame())
	{
		updateProfileList();
	}
	if (m_item->asOSGFrame())
	{
		setEnabled(false);
	}
	if (m_item->asSymbolFrame())
	{
		setEnabled(false);
	}
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
	if ((m_haveDoc) && (m_haveItem))
	{
		//CB Dont pass in the scale to the offset change as its taken from the new scale
		m_doc->itemSelection_SetImageScaleAndOffset(imageXScaleSpinBox->value() / 100.0 / m_item->pixm.imgInfo.xres * 72.0, imageYScaleSpinBox->value() / 100.0 / m_item->pixm.imgInfo.yres * 72.0, imageXOffsetSpinBox->value() / m_unitRatio, imageYOffsetSpinBox->value() / m_unitRatio);
		imgDpiX->showValue(qRound(720.0 / m_item->imageXScale()) / 10.0);
		imgDpiY->showValue(qRound(720.0 / m_item->imageYScale()) / 10.0);
	}
}

void PropertiesPalette_Image::handleLocalDpi()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		//CB Dont pass in the scale to the offset change as its taken from the new scale
		m_doc->itemSelection_SetImageScaleAndOffset(72.0 / imgDpiX->value(), 72.0 / imgDpiY->value(), imageXOffsetSpinBox->value() / m_unitRatio, imageYOffsetSpinBox->value() / m_unitRatio);
		
		imageXScaleSpinBox->showValue(m_item->imageXScale() * 100 / 72.0 * m_item->pixm.imgInfo.xres);
		imageYScaleSpinBox->showValue(m_item->imageYScale() * 100 / 72.0 * m_item->pixm.imgInfo.yres);
	}
}

void PropertiesPalette_Image::handleLocalRotation()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
		m_doc->itemSelection_SetImageRotation(360 - imageRotation->value());
}

void PropertiesPalette_Image::handleScaling()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	if (freeScale == sender())
	{
		frameScale->setChecked(false);
		freeScale->setChecked(true);
		cbProportional->setEnabled(false);
//		imageXOffsetSpinBox->setEnabled(true);
//		imageYOffsetSpinBox->setEnabled(true);
		imageXScaleSpinBox->setEnabled(true);
		imageYScaleSpinBox->setEnabled(true);
		imgDpiX->setEnabled(true);
		imgDpiY->setEnabled(true);
//		imageRotation->setEnabled(true);
		keepImageWHRatioButton->setEnabled(true);
		keepImageDPIRatioButton->setEnabled(true);
	}
	if (frameScale == sender())
	{
		frameScale->setChecked(true);
		freeScale->setChecked(false);
		cbProportional->setEnabled(true);
//		imageXOffsetSpinBox->setEnabled(false);
//		imageYOffsetSpinBox->setEnabled(false);
		imageXScaleSpinBox->setEnabled(false);
		imageYScaleSpinBox->setEnabled(false);
		imgDpiX->setEnabled(false);
		imgDpiY->setEnabled(false);
//		imageRotation->setEnabled(false);
		keepImageWHRatioButton->setEnabled(false);
		keepImageDPIRatioButton->setEnabled(false);
	}

	if ((m_haveDoc) && (m_haveItem))
	{
		m_item->setImageScalingMode(freeScale->isChecked(), cbProportional->isChecked());
		emit UpdtGui(PageItem::ImageFrame);
		emit DocChanged();
	}
}

void PropertiesPalette_Image::handleXScale()
{
	imageXScaleSpinBox->blockSignals(true);
	imageYScaleSpinBox->blockSignals(true);
	if (keepImageWHRatioButton->isChecked())
		imageYScaleSpinBox->setValue(imageXScaleSpinBox->value());
	handleLocalScale();
	imageXScaleSpinBox->blockSignals(false);
	imageYScaleSpinBox->blockSignals(false);
}

void PropertiesPalette_Image::handleYScale()
{
	imageXScaleSpinBox->blockSignals(true);
	imageYScaleSpinBox->blockSignals(true);
	if (keepImageWHRatioButton->isChecked())
		imageXScaleSpinBox->setValue(imageYScaleSpinBox->value());
	handleLocalScale();
	imageXScaleSpinBox->blockSignals(false);
	imageYScaleSpinBox->blockSignals(false);
}

void PropertiesPalette_Image::handleDpiX()
{
	imgDpiX->blockSignals(true);
	imgDpiY->blockSignals(true);
	if (keepImageDPIRatioButton->isChecked())
		imgDpiY->setValue(imgDpiX->value());
	handleLocalDpi();
	imgDpiX->blockSignals(false);
	imgDpiY->blockSignals(false);
}

void PropertiesPalette_Image::handleDpiY()
{
	imgDpiX->blockSignals(true);
	imgDpiY->blockSignals(true);
	if (keepImageDPIRatioButton->isChecked())
		imgDpiX->setValue(imgDpiY->value());
	handleLocalDpi();
	imgDpiX->blockSignals(false);
	imgDpiY->blockSignals(false);
}

void PropertiesPalette_Image::handleImageDPIRatio()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	imgDpiX->blockSignals(true);
	imgDpiY->blockSignals(true);
	if (keepImageDPIRatioButton->isChecked())
	{
		imgDpiY->setValue(imgDpiX->value());
		handleLocalDpi();
		keepImageWHRatioButton->setChecked(true);
	}
	else
		keepImageWHRatioButton->setChecked(false);
	imgDpiX->blockSignals(false);
	imgDpiY->blockSignals(false);
}

void PropertiesPalette_Image::handleImageWHRatio()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	imageXScaleSpinBox->blockSignals(true);
	imageYScaleSpinBox->blockSignals(true);
	if (keepImageWHRatioButton->isChecked())
	{
		imageYScaleSpinBox->setValue(imageXScaleSpinBox->value());
		handleLocalScale();
		keepImageDPIRatioButton->setChecked(true);
	}
	else
		keepImageDPIRatioButton->setChecked(false);
	imageXScaleSpinBox->blockSignals(false);
	imageYScaleSpinBox->blockSignals(false);
}

void PropertiesPalette_Image::handleImageEffects()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_ScMW->ImageEffects();
}

void PropertiesPalette_Image::handleExtImgProperties()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_ScMW->view->editExtendedImageProperties();
	emit DocChanged();
}

void PropertiesPalette_Image::handleImagePageNumber()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	bool reallynew = (m_item->pixm.imgInfo.actualPageNumber != imagePageNumber->value());
	m_item->pixm.imgInfo.actualPageNumber = imagePageNumber->value();
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

void PropertiesPalette_Image::languageChange()
{
	imgDPIXLabel->setText( tr("Actual X-DPI:"));
	imgDPIYLabel->setText( tr("Actual Y-DPI:"));
	xposImgLabel->setText( tr("&X-Pos:"));
	yposImgLabel->setText( tr("&Y-Pos:"));
	xscaleLabel->setText( tr("X-Sc&ale:"));
	yscaleLabel->setText( tr("Y-Scal&e:"));
	freeScale->setText( tr("&Free Scaling"));
	imagePageNumberLabel->setText( tr("&Page Number:"));
	imageRotationLabel->setText( tr("Rotation:"));

	cbProportional->setText( tr("P&roportional"));
	imgEffectsButton->setText( tr("Image Effects"));
	imgExtProperties->setText( tr("Extended Properties"));

	inputProfLabel->setText( tr("Input Profile:"));
	renderIntentLabel->setText( tr("Rendering Intent:"));
	compressionMethodLabel->setText( tr("Method:"));
	compressionQualityLabel->setText( tr("Quality:"));

	int oldMonitorI=renderIntent->currentIndex();
	renderIntent->clear();
	renderIntent->addItem( tr("Perceptual"));
	renderIntent->addItem( tr("Relative Colorimetric"));
	renderIntent->addItem( tr("Saturation"));
	renderIntent->addItem( tr("Absolute Colorimetric"));
	renderIntent->setCurrentIndex(oldMonitorI);

	int oldCompressionMethod=compressionMethod->currentIndex();
	compressionMethod->clear();
	compressionMethod->addItem( tr( "Global" ) );
	compressionMethod->addItem( tr( "Automatic" ) );
	compressionMethod->addItem( tr( "Lossy - JPEG" ) );
	compressionMethod->addItem( tr( "Lossless - Zip" ) );
	compressionMethod->addItem( tr( "None" ) );
	compressionMethod->setCurrentIndex(oldCompressionMethod);

	int oldCompressionQuality=compressionQuality->currentIndex();
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
	imgDpiX->setSuffix("");
	imgDpiY->setSuffix("");

	QString ptSuffix = tr(" pt");
	QString suffix   = (m_haveDoc) ? unitGetSuffixFromIndex(m_doc->unitIndex()) : ptSuffix;

	imageXOffsetSpinBox->setSuffix(suffix);
	imageYOffsetSpinBox->setSuffix(suffix);

	imageXOffsetSpinBox->setToolTip( tr("Horizontal offset of image within frame"));
	imageYOffsetSpinBox->setToolTip( tr("Vertical offset of image within frame"));
	imageXScaleSpinBox->setToolTip( tr("Resize the image horizontally"));
	imageYScaleSpinBox->setToolTip( tr("Resize the image vertically"));
	keepImageWHRatioButton->setToolTip( tr("Keep the X and Y scaling the same"));
	freeScale->setToolTip( tr("Allow the image to be a different size to the frame"));
	frameScale->setToolTip( tr("Make the image fit within the size of the frame"));
	imgDpiX->setToolTip( tr("Effective horizontal DPI of the image after scaling"));
	imgDpiY->setToolTip( tr("Effective vertical DPI of the image after scaling"));
	cbProportional->setToolTip( tr("Use image proportions rather than those of the frame"));
	inputProfiles->setToolTip( tr("Source profile of the image"));
	renderIntent->setToolTip( tr("Rendering intent for the image"));
	compressionMethod->setToolTip( tr("Compression method used in PDF export for the image"));
	compressionQuality->setToolTip( tr("Compression quality used in PDF export for the image"));
}

void PropertiesPalette_Image::unitChange()
{
	if (!m_doc)
		return;

	double oldRatio = m_unitRatio;
	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();

	imageXOffsetSpinBox->blockSignals(true);
	imageXOffsetSpinBox->setNewUnit( m_unitIndex );
	imageXOffsetSpinBox->blockSignals(false);

	imageYOffsetSpinBox->blockSignals(true);
	imageYOffsetSpinBox->setNewUnit( m_unitIndex );
	imageYOffsetSpinBox->blockSignals(false);
}

bool PropertiesPalette_Image::userActionOn()
{
	return m_userActionOn;;
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


