/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "propertiespalette_group.h"

#include <QButtonGroup>
#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QEvent>
#include <QFocusEvent>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QImage>
#include <QKeyEvent>
#include <QLabel>
#include <QListView>
#include <QTransform>
#include <QMenu>
#include <QMessageBox>
#include <QObject>
#include <QPixmap>
#include <QPoint>
#include <QPushButton>
#include <QRadioButton>
#include <QRegExp>
#include <QSpacerItem>
#include <QSpinBox>
#include <QStackedWidget>
#include <QTimer>
#include <QToolBox>
#include <QToolTip>
#include <QVBoxLayout>
#include <QValidator>
#include <QWidget>

#if defined(_MSC_VER)
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include "commonstrings.h"
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

#include "text/nlsconfig.h"
#include "ui/autoform.h"
#include "ui/nodeeditpalette.h"
#include "ui/propertiespalette_utils.h"
#include "ui/transparencypalette.h"

PropertiesPalette_Group::PropertiesPalette_Group( QWidget* parent) : QWidget(parent)
{
	m_ScMW = 0;
	m_doc  = 0;
	m_haveDoc   = false;
	m_haveItem  = false;
	m_unitRatio = 1.0;

	m_tmpSelection = new Selection(this, false);
	m_tmpSelection->clear();

	setupUi(this);

	textFlowDisabled->setIcon(QIcon(loadIcon("flow-none.png")));
	textFlowUsesFrameShape->setIcon(QIcon(loadIcon("flow-frame.png")));
	textFlowUsesBoundingBox->setIcon(QIcon(loadIcon("flow-bounding.png")));
	textFlowUsesContourLine->setIcon(QIcon(loadIcon("flow-contour.png")));
	textFlowUsesImageClipping->setIcon(QIcon(loadIcon("flow-contour.png")));

	textFlowDisabled->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
	textFlowUsesFrameShape->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
	textFlowUsesBoundingBox->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
	textFlowUsesContourLine->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
	textFlowUsesImageClipping->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));

	transPalWidget->hideSelectionButtons();

	languageChange();

	connect(textFlowBtnGroup, SIGNAL(buttonClicked(int)), this, SLOT(handleTextFlow()));

	connect(customShape  , SIGNAL(FormSel(int, int, qreal *)), this, SLOT(handleNewShape(int, int, qreal *)));
	connect(editShape, SIGNAL(clicked())                 , this, SLOT(handleShapeEdit()));
	connect(transPalWidget , SIGNAL(editGradient())            , this, SLOT(handleGradientEdit()));
	connect(transPalWidget , SIGNAL(NewSpecial(double, double, double, double, double, double, double, double, double, double)), this, SLOT(handleSpecialGradient(double, double, double, double, double, double, double, double )));
}

PageItem* PropertiesPalette_Group::currentItemFromSelection()
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

void PropertiesPalette_Group::setMainWindow(ScribusMainWindow *mw)
{
	m_ScMW = mw;

	connect(this->transPalWidget, SIGNAL(gradientChanged()), m_ScMW, SLOT(updtGradMaskGroup()));
	connect(this                , SIGNAL(DocChanged())     , m_ScMW, SLOT(slotDocCh()));
	connect(m_ScMW, SIGNAL(UpdateRequest(int)), this, SLOT(handleUpdateRequest(int)));
}

void PropertiesPalette_Group::setDoc(ScribusDoc *d)
{
	if((d == (ScribusDoc*) m_doc) || (m_ScMW && m_ScMW->scriptIsRunning()))
		return;

	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}
	
	disconnect(this->transPalWidget, SIGNAL(NewTrans(double)), 0, 0);
	disconnect(this->transPalWidget, SIGNAL(NewBlend(int)), 0, 0);
	disconnect(this->transPalWidget, SIGNAL(NewGradient(int)), 0, 0);
	disconnect(this->transPalWidget, SIGNAL(NewPattern(QString)), 0, 0);
	disconnect(this->transPalWidget, SIGNAL(NewPatternProps(double, double, double, double, double, double, double, bool, bool)), 0, 0);

	m_doc  = d;
	m_item = NULL;
	m_unitRatio   = m_doc->unitRatio();
	m_unitIndex   = m_doc->unitIndex();
	int precision = unitGetPrecisionFromIndex(m_unitIndex);
	double maxXYWHVal =  16777215 * m_unitRatio;
	double minXYVal   = -16777215 * m_unitRatio;

	transPalWidget->setDocument(m_doc);
	transPalWidget->setCurrentItem(NULL);

	m_haveDoc  = true;
	m_haveItem = false;

	connect(this->transPalWidget, SIGNAL(NewTrans(double))   , this, SLOT(handleGroupTransparency(double)));
	connect(this->transPalWidget, SIGNAL(NewBlend(int))      , this, SLOT(handleGroupBlending(int)));
	connect(this->transPalWidget, SIGNAL(NewGradient(int))   , this, SLOT(handleGroupGradMask(int)));
	connect(this->transPalWidget, SIGNAL(NewPattern(QString)), this, SLOT(handleGroupPatternMask(QString)));
	connect(this->transPalWidget, SIGNAL(NewPatternProps(double, double, double, double, double, double, double, bool, bool)), this, SLOT(handleGroupPatternMaskProps(double, double, double, double, double, double, double, bool, bool)));

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
}

void PropertiesPalette_Group::unsetDoc()
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

	transPalWidget->setDocument(NULL);
	transPalWidget->setCurrentItem(NULL);

	setEnabled(false);
}

void PropertiesPalette_Group::unsetItem()
{
	m_haveItem = false;
	m_item     = NULL;
	transPalWidget->setCurrentItem(NULL);
	handleSelectionChanged();
}

void PropertiesPalette_Group::handleSelectionChanged()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem* currItem = currentItemFromSelection();
	if (m_doc->m_Selection->count() > 1)
	{
		bool widgetEnabled = false;
		if (m_haveItem && m_item)
		{
			if ((m_item->isGroup()) && (!m_item->isSingleSel))
				widgetEnabled = true;
		}
		setEnabled(widgetEnabled);
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
			if (currItem->asOSGFrame())
			{
				setEnabled(false);
			}
			break;
		case PageItem::Arc:
		case PageItem::TextFrame:
		case PageItem::Line:
		case PageItem::ItemType1:
		case PageItem::ItemType3:
		case PageItem::Polygon:
		case PageItem::PolyLine:
		case PageItem::PathText:
		case PageItem::RegularPolygon:
			setEnabled(false);
			break;
		case PageItem::Symbol:
			setEnabled(true);
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

void PropertiesPalette_Group::handleUpdateRequest(int updateFlags)
{
	if (updateFlags & reqColorsUpdate)
		updateColorList();
}

void PropertiesPalette_Group::setCurrentItem(PageItem *item)
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

	bool   mirrorX, mirrorY;
	double patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY;
	m_item->patternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY);
	m_item->patternFlip(mirrorX, mirrorY);

	transPalWidget->setCurrentItem(m_item);

	if ((m_item->isGroup()) && (!m_item->isSingleSel))
	{
		setEnabled(true);
		if (m_item->FrameType == 0)
			customShape->setIcon(customShape->getIconPixmap(0));
		if (m_item->FrameType == 1)
			customShape->setIcon(customShape->getIconPixmap(1));
		if (m_item->FrameType > 3)
			customShape->setIcon(customShape->getIconPixmap(m_item->FrameType-2));
		transPalWidget->updateFromItem();
		transPalWidget->setActPattern(m_item->patternMask(), patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY, mirrorX, mirrorY);
		customShape->setEnabled(true);
		editShape->setEnabled(true);
		shapeGroup->setEnabled(true);
		textFlowDisabled->setEnabled(true);
		textFlowUsesFrameShape->setEnabled(true);
		textFlowUsesBoundingBox->setEnabled(true);
		textFlowUsesContourLine->setEnabled(true);
		textFlowUsesImageClipping->setEnabled(false);
	}
	else
	{
		setEnabled(false);
	}

	m_haveItem = true;

	if (m_item->asOSGFrame())
	{
		setEnabled(false);
	}
	if (m_item->asSymbolFrame())
	{
		setEnabled(true);
		if (m_item->FrameType == 0)
			customShape->setIcon(customShape->getIconPixmap(0));
		if (m_item->FrameType == 1)
			customShape->setIcon(customShape->getIconPixmap(1));
		if (m_item->FrameType > 3)
			customShape->setIcon(customShape->getIconPixmap(m_item->FrameType-2));
		transPalWidget->updateFromItem();
		transPalWidget->setActPattern(m_item->patternMask(), patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY, mirrorX, mirrorY);
		customShape->setEnabled(true);
		editShape->setEnabled(true);
		shapeGroup->setEnabled(true);
		textFlowDisabled->setEnabled(true);
		textFlowUsesFrameShape->setEnabled(true);
		textFlowUsesBoundingBox->setEnabled(true);
		textFlowUsesContourLine->setEnabled(true);
		textFlowUsesImageClipping->setEnabled(false);
	}
}

void PropertiesPalette_Group::displayTextFlowMode(PageItem::TextFlowMode mode)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning() || !m_haveItem)
		return;
	if (m_item->isGroup())
	{
		if (mode == PageItem::TextFlowDisabled)
			textFlowDisabled->setChecked(true);
		else if (mode == PageItem::TextFlowUsesFrameShape)
			textFlowUsesFrameShape->setChecked(true);
		else if (mode == PageItem::TextFlowUsesBoundingBox)
			textFlowUsesBoundingBox->setChecked(true);
		else if (mode == PageItem::TextFlowUsesContourLine)
			textFlowUsesContourLine->setChecked(true);
		else if (mode == PageItem::TextFlowUsesImageClipping)
			textFlowUsesImageClipping->setChecked(true);
		if ((m_item->asImageFrame()) && (m_item->imageClip.size() != 0))
			textFlowUsesImageClipping->setEnabled(true);
		else
			textFlowUsesImageClipping->setEnabled(false);
	}
}

const VGradient PropertiesPalette_Group::getMaskGradientGroup()
{
	return transPalWidget->gradEdit->gradient();
}

void PropertiesPalette_Group::updateColorList()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	transPalWidget->setColors(m_doc->PageColors);
	transPalWidget->setPatterns(&m_doc->docPatterns);
	transPalWidget->setGradients(&m_doc->docGradients);
}

void PropertiesPalette_Group::updateColorSpecialGradient()
{
	if (!m_haveDoc)
		return;
	if(m_doc->m_Selection->isEmpty())
		return;
	double ratio = m_doc->unitRatio();
	PageItem *currItem = m_doc->m_Selection->itemAt(0);
	if (currItem)
	{
		switch (m_ScMW->view->editStrokeGradient)
		{
		case 0:
		case 1:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			break;
		default:
			if (currItem->isGroup())
				transPalWidget->setSpecialGradient(currItem->GrMaskStartX * ratio, currItem->GrMaskStartY * ratio, currItem->GrMaskEndX * ratio, currItem->GrMaskEndY * ratio, currItem->GrMaskFocalX * ratio, currItem->GrMaskFocalY * ratio, currItem->GrMaskScale, currItem->GrMaskSkew);
		}
	}
}

void PropertiesPalette_Group::handleGroupTransparency(double trans)
{
	if ((m_haveDoc) && (m_haveItem))
	{
		m_item->setFillTransparency(trans);
		m_item->update();
		emit DocChanged();
	}
}

void PropertiesPalette_Group::handleGroupBlending(int blend)
{
	if ((m_haveDoc) && (m_haveItem))
	{
		m_item->setFillBlendmode(blend);
		m_item->update();
		emit DocChanged();
	}
}

void PropertiesPalette_Group::handleGroupGradMask(int typ)
{
	if ((m_haveDoc) && (m_haveItem))
	{
		m_item->GrMask = typ;
		if ((typ > 0) && (typ < 7))
			m_item->updateGradientVectors();
		m_item->update();
		emit DocChanged();
	}
}

void PropertiesPalette_Group::handleGroupPatternMask(QString pattern)
{
	if ((m_haveDoc) && (m_haveItem))
	{
		m_item->setPatternMask(pattern);
		m_item->update();
		emit DocChanged();
	}
}

void PropertiesPalette_Group::handleGroupPatternMaskProps(double imageScaleX, double imageScaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY)
{
	if ((m_haveDoc) && (m_haveItem))
	{
		m_item->setMaskTransform(imageScaleX, imageScaleY, offsetX, offsetY, rotation, skewX, skewY);
		m_item->setMaskFlip(mirrorX, mirrorY);
		m_item->update();
		emit DocChanged();
	}
}

void PropertiesPalette_Group::handleNewShape(int frameType, int count, qreal *points)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		if ((m_item->itemType() == PageItem::PolyLine) || (m_item->itemType() == PageItem::PathText))
			return;

		m_doc->item_setFrameShape(m_item, frameType, count, points);

		//ScribusDoc::changed() is called in item_setFrameShape()
		//Hope this is enough
		//emit DocChanged();
		emit shapeChanged(frameType);
	}
}

void PropertiesPalette_Group::handleTextFlow()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		PageItem::TextFlowMode mode = PageItem::TextFlowDisabled;
		if (m_item->isGroup())
		{
			if (textFlowDisabled->isChecked())
				mode = PageItem::TextFlowDisabled;
			if (textFlowUsesFrameShape->isChecked())
				mode = PageItem::TextFlowUsesFrameShape;
			if (textFlowUsesBoundingBox->isChecked())
				mode = PageItem::TextFlowUsesBoundingBox;
			if (textFlowUsesContourLine->isChecked())
				mode = PageItem::TextFlowUsesContourLine;
			if (textFlowUsesImageClipping->isChecked())
				mode = PageItem::TextFlowUsesImageClipping;
		}
		m_item->setTextFlowMode(mode);
		m_ScMW->view->DrawNew();
		emit DocChanged();
	}
}

void PropertiesPalette_Group::handleGradientEdit()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		m_ScMW->view->editStrokeGradient = 2;
		if (transPalWidget->gradEditButton->isChecked())
			m_ScMW->view->requestMode(modeEditGradientVectors);
		else
			m_ScMW->view->requestMode(modeNormal);
	}
}

void PropertiesPalette_Group::handleSpecialGradient(double x1, double y1, double x2, double y2, double fx, double fy, double sg, double sk)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		QRectF upRect;
		m_item->GrMaskStartX = x1 / m_unitRatio;
		m_item->GrMaskStartY = y1 / m_unitRatio;
		m_item->GrMaskEndX = x2 / m_unitRatio;
		m_item->GrMaskEndY = y2 / m_unitRatio;
		m_item->GrMaskFocalX = fx / m_unitRatio;
		m_item->GrMaskFocalY = fy / m_unitRatio;
		m_item->GrMaskScale = sg;
		m_item->GrMaskSkew = sk;
		if ((m_item->GrMask == 1) || (m_item->GrMask == 4))
		{
			m_item->GrMaskFocalX = m_item->GrMaskStartX;
			m_item->GrMaskFocalY = m_item->GrMaskStartY;
		}
		m_item->update();
		upRect = QRectF(QPointF(m_item->GrMaskStartX, m_item->GrMaskStartY), QPointF(m_item->GrMaskEndX, m_item->GrMaskEndY));
		double radEnd = distance(m_item->GrMaskEndX - m_item->GrMaskStartX, m_item->GrMaskEndY - m_item->GrMaskStartY);
		double rotEnd = xy2Deg(m_item->GrMaskEndX - m_item->GrMaskStartX, m_item->GrMaskEndY - m_item->GrMaskStartY);
		QTransform m;
		m.translate(m_item->GrMaskStartX, m_item->GrMaskStartY);
		m.rotate(rotEnd);
		m.rotate(-90);
		m.rotate(m_item->GrMaskSkew);
		m.translate(radEnd * m_item->GrMaskScale, 0);
		QPointF shP = m.map(QPointF(0,0));
		upRect |= QRectF(shP, QPointF(m_item->GrMaskEndX, m_item->GrMaskEndY)).normalized();
		upRect |= QRectF(shP, QPointF(m_item->GrMaskStartX, m_item->GrMaskStartY)).normalized();
		upRect |= QRectF(shP, QPointF(0, 0)).normalized();
		upRect |= QRectF(shP, QPointF(m_item->width(), m_item->height())).normalized();
		upRect.translate(m_item->xPos(), m_item->yPos());
		m_doc->regionsChanged()->update(upRect.adjusted(-10.0, -10.0, 10.0, 10.0));
		emit DocChanged();
	}
}

void PropertiesPalette_Group::handleShapeEdit()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		m_tmpSelection->clear();
		m_tmpSelection->copy(*m_doc->m_Selection, true);
		m_doc->m_Selection->clear();
		m_doc->m_Selection->addItem(m_item);
		m_item->isSingleSel = true;
		m_item->update();
		m_ScMW->view->requestMode(modeEditClip);
		connect(m_ScMW->nodePalette, SIGNAL(paletteClosed()), this, SLOT(handleShapeEditEnded()));
		emit shapeEditStarted();
	}
}

void PropertiesPalette_Group::handleShapeEditEnded()
{
	disconnect(m_ScMW->nodePalette, SIGNAL(paletteClosed()), this, SLOT(handleShapeEditEnded()));
	if ((m_haveDoc) && (m_haveItem))
	{
		if (m_tmpSelection->count() > 0)
		{
			m_doc->m_Selection->copy(*m_tmpSelection, false);
			m_doc->m_Selection->connectItemToGUI();
		}
		m_tmpSelection->clear();
	}
}

void PropertiesPalette_Group::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}


void PropertiesPalette_Group::languageChange()
{
	editShape->setText( tr("&Edit..."));
	shapeGroup->setTitle( tr("Shape"));

	textFlowGroup->setTitle( tr("Text &Flow Around Frame"));
	textFlowDisabled->setText( tr("Disabled"));
	textFlowUsesFrameShape->setText( tr("Use Frame &Shape"));
	textFlowUsesBoundingBox->setText( tr("Use &Bounding Box"));
	textFlowUsesContourLine->setText( tr("&Use Contour Line"));
	textFlowUsesImageClipping->setText( tr("Use Image Clip Path"));

	textFlowDisabled->setToolTip( tr("Disable text flow from lower frames around object"));
	textFlowUsesFrameShape->setToolTip( tr("Use the frame shape for text flow of text frames below the object."));
	textFlowUsesBoundingBox->setToolTip(  "<qt>" + tr("Use the bounding box, which is always rectangular, instead of the frame's shape for text flow of text frames below the object. ") + "</qt>" );
	textFlowUsesContourLine->setToolTip(  "<qt>" + tr("When chosen, the contour line can be edited with the Edit Shape Tool on the palette further above. When edited via the shape palette, this becomes a second separate line originally based on the frame's shape for text flow of text frames below the object.") + "</qt>" );
	textFlowUsesImageClipping->setToolTip(  "<qt>" + tr("Use the clipping path of the image") + "</qt>" );
}

void PropertiesPalette_Group::unitChange()
{
	if (!m_doc)
		return;

	double oldRatio = m_unitRatio;
	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();

	transPalWidget->blockSignals(true);
	transPalWidget->unitChange(oldRatio, m_unitRatio, m_doc->unitIndex());
	transPalWidget->blockSignals(false);
}