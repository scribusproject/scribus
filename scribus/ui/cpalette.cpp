/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          cpalette.cpp  -  description
                             -------------------
    begin                : Wed Apr 25 2001
    copyright            : (C) 2001 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "cpalette.h"

#include <QAbstractItemView>
#include <QEvent>
#include <QFont>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPixmap>
#include <QRect>
#include <QSpacerItem>
#include <QSpinBox>
#include <QStandardItem>
#include <QToolButton>
#include <QToolTip>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QDebug>

#include "colorlistbox.h"
#include "sccombobox.h"
#include "scribusdoc.h"
#include "scrspinbox.h"
#include "gradienteditor.h"
#include "insertTable.h"
#include "units.h"
#include "page.h"
#include "pageitem.h"
#include "util_icon.h"
#include "commonstrings.h"
#include "sccolorengine.h"
#include "scpainter.h"
#include "scpattern.h"
#include "scribus.h"
#include "selection.h"
#include "util.h"
#include "util_math.h"

Cpalette::Cpalette(QWidget* parent) : QWidget(parent)
{
	m_blockUpdates = 0;
	currentItem = NULL;
	patternList = NULL;
	CGradDia = NULL;
	CGradDia = new GradientVectorDialog(this->parentWidget());
	CGradDia->hide();

	setupUi(this);
	fillModeCombo->addItem( tr("Solid") );
	fillModeCombo->addItem( tr("Gradient") );
	strokeModeCombo->addItem( tr("Solid") );
	strokeModeCombo->addItem( tr("Gradient") );

	gradEdit->layout()->setAlignment(Qt::AlignTop);
	gradEditStroke->layout()->setAlignment(Qt::AlignTop);
	gradientTypeStroke->setCurrentIndex(0);
	/*editLineColorSelector->setIcon(QIcon(loadIcon("16/color-stroke.png")));
	editFillColorSelector->setIcon(QIcon(loadIcon("16/color-fill.png")));
	connect(editLineColorSelector, SIGNAL(clicked()), this, SLOT(editLineColorSelectorButton()));
	connect(editFillColorSelector, SIGNAL(clicked()), this, SLOT(editFillColorSelectorButton()));*/
	connect(tabFillStroke, SIGNAL(currentChanged(int)), this, SLOT(fillStrokeSelector(int)));
	connect(patternBox, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPattern(QListWidgetItem*)));
	connect(fillShade, SIGNAL(valueChanged(int)), this, SIGNAL(NewBrushShade(int)));
	connect(strokeShade, SIGNAL(valueChanged(int)), this, SIGNAL(NewPenShade(int)));
	connect(colorListStroke, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(selectColorS(QListWidgetItem*)));
	connect(colorListFill, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(selectColorF(QListWidgetItem*)));
	connect(gradEditButton, SIGNAL(clicked()), this, SLOT(editGradientVector()));
	connect(editMeshColors, SIGNAL(clicked()), this, SLOT(editMeshPointColor()));
	connect(displayAllColors, SIGNAL(clicked()), this, SLOT(toggleColorDisplay()));
	connect(fillModeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(slotGrad(int)));
	connect(CGradDia, SIGNAL(NewSpecial(double, double, double, double, double, double, double, double, double, double)), this, SIGNAL(NewSpecial(double, double, double, double, double, double, double, double, double, double)));
	connect(CGradDia, SIGNAL(paletteShown(bool)), this, SLOT(setActiveGradDia(bool)));
	connect(CGradDia, SIGNAL(editGradient(int)), this, SIGNAL(editGradient(int)));
	connect(CGradDia, SIGNAL(createNewMesh()), this, SLOT(createNewMeshGradient()));
	connect(CGradDia, SIGNAL(resetMesh()), this, SLOT(resetMeshGradient()));
	connect(CGradDia, SIGNAL(meshToShape()), this, SLOT(meshGradientToShape()));
	connect(CGradDia, SIGNAL(reset1Control()), this, SLOT(resetOneControlPoint()));
	connect(CGradDia, SIGNAL(resetAllControl()), this, SLOT(resetAllControlPoints()));
	connect(gradientType, SIGNAL(activated(int)), this, SLOT(slotGradType(int)));
	connect(gradEdit, SIGNAL(gradientChanged()) , this, SLOT(handleFillGradient()));
	connect(editPatternProps, SIGNAL(clicked()) , this, SLOT(changePatternProps()));
	connect(namedGradient, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradient(const QString &)));
	connect(overPrintCombo, SIGNAL(activated(int)), this, SIGNAL(NewOverprint(int)));
	connect(patternBoxStroke, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPatternS(QListWidgetItem*)));
	connect(strokeModeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(slotGradStroke(int)));
	connect(editPatternPropsStroke, SIGNAL(clicked()), this, SLOT(changePatternPropsStroke()));
	connect(namedGradientStroke, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradientStroke(const QString &)));
	connect(gradientTypeStroke, SIGNAL(activated(int)), this, SLOT(slotGradTypeStroke(int)));
	connect(gradEditStroke, SIGNAL(gradientChanged()) , this, SLOT(handleStrokeGradient()));
	connect(gradEditButtonStroke, SIGNAL(clicked()), this, SLOT(editGradientVectorStroke()));
	connect(followsPath, SIGNAL(clicked()), this, SLOT(toggleStrokePattern()));
	connect(colorPoint1, SIGNAL(activated(int)), this, SLOT(setGradientColors()));
	connect(colorPoint2, SIGNAL(activated(int)), this, SLOT(setGradientColors()));
	connect(colorPoint3, SIGNAL(activated(int)), this, SLOT(setGradientColors()));
	connect(colorPoint4, SIGNAL(activated(int)), this, SLOT(setGradientColors()));
	connect(color1Alpha, SIGNAL(valueChanged(int)), this, SLOT(setGradientColors()));
	connect(color2Alpha, SIGNAL(valueChanged(int)), this, SLOT(setGradientColors()));
	connect(color3Alpha, SIGNAL(valueChanged(int)), this, SLOT(setGradientColors()));
	connect(color4Alpha, SIGNAL(valueChanged(int)), this, SLOT(setGradientColors()));
	connect(color1Shade, SIGNAL(valueChanged(int)), this, SLOT(setGradientColors()));
	connect(color2Shade, SIGNAL(valueChanged(int)), this, SLOT(setGradientColors()));
	connect(color3Shade, SIGNAL(valueChanged(int)), this, SLOT(setGradientColors()));
	connect(color4Shade, SIGNAL(valueChanged(int)), this, SLOT(setGradientColors()));
	connect(colorMeshPoint, SIGNAL(activated(int)), this, SLOT(updateMeshPoint()));
	connect(shadeMeshPoint, SIGNAL(valueChanged(int)), this, SLOT(updateMeshPoint()));
	connect(transparencyMeshPoint, SIGNAL(valueChanged(int)), this, SLOT(updateMeshPoint()));
	tabFillStroke->setCurrentIndex(0);
	updateFromItem();
	/*editFillColorSelector->setChecked(true);
	editFillColorSelectorButton();*/
}

void Cpalette::setCurrentItem(PageItem* item)
{
	if ((item == NULL) || (currentItem != item))
	{
		editStrokeGradient = 0;
		CGradDia->hide();
		editMeshColors->setEnabled(true);
		gradEditButton->setEnabled(true);
	}
	currentItem = item;
}

void Cpalette::setDocument(ScribusDoc* doc)
{
	disconnect(this, SIGNAL(NewPen(QString)), 0, 0);
	disconnect(this, SIGNAL(NewBrush(QString)), 0, 0);
	disconnect(this, SIGNAL(NewPenShade(int)), 0, 0);
	disconnect(this, SIGNAL(NewBrushShade(int)), 0, 0);
	disconnect(this, SIGNAL(NewGradient(int)), 0, 0);
	disconnect(this, SIGNAL(NewGradientS(int)), 0, 0);
	disconnect(this, SIGNAL(NewPattern(QString)), 0, 0);
	disconnect(this, SIGNAL(NewPatternProps(double, double, double, double, double, double, double, bool, bool)), 0, 0);
	disconnect(this, SIGNAL(NewOverprint(int)), 0, 0);
	disconnect(this, SIGNAL(NewPatternS(QString)), 0, 0);
	disconnect(this, SIGNAL(NewPatternTypeS(bool)), 0, 0);
	disconnect(this, SIGNAL(NewPatternPropsS(double, double, double, double, double, double, double, double, bool, bool)), 0, 0);

	if (currentDoc)
	{
		disconnect(currentDoc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(currentDoc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
		disconnect(currentDoc->scMW()     , SIGNAL(UpdateRequest(int)), this, SLOT(handleUpdateRequest(int)));
	}

	ScribusDoc* oldDoc = currentDoc;
	currentDoc = doc;

	if (doc == NULL)
	{
		colorListStroke->cList = NULL;
		colorListFill->cList = NULL;
	}
	else
	{
		colorListStroke->cList = &doc->PageColors;
		colorListFill->cList = &doc->PageColors;
		gradEdit->setColors(doc->PageColors);
		currentUnit = doc->unitIndex();

		updateColorList();

		connect(this, SIGNAL(NewPen(QString))      , doc, SLOT(itemSelection_SetItemPen(QString)));
		connect(this, SIGNAL(NewBrush(QString))    , doc, SLOT(itemSelection_SetItemBrush(QString)));
		connect(this, SIGNAL(NewPenShade(int))     , this, SLOT(handleStrokeShade(int)));
		connect(this, SIGNAL(NewBrushShade(int))   , this, SLOT(handleFillShade(int)));
		connect(this, SIGNAL(NewGradient(int))     , doc, SLOT(itemSelection_SetItemGradFill(int)));
		connect(this, SIGNAL(NewGradientS(int))    , doc, SLOT(itemSelection_SetItemGradStroke(int)));
		connect(this, SIGNAL(NewPattern(QString))  , doc, SLOT(itemSelection_SetItemPatternFill(QString)));
		connect(this, SIGNAL(NewPatternProps(double, double, double, double, double, double, double, bool, bool)), doc, SLOT(itemSelection_SetItemPatternProps(double, double, double, double, double, double, double, bool, bool)));
		connect(this, SIGNAL(NewOverprint(int))    , this, SLOT(handleOverprint(int)));
		connect(this, SIGNAL(NewPatternS(QString)) , doc, SLOT(itemSelection_SetItemStrokePattern(QString)));
		connect(this, SIGNAL(NewPatternTypeS(bool)), doc, SLOT(itemSelection_SetItemStrokePatternType(bool)));
		connect(this, SIGNAL(NewPatternPropsS(double, double, double, double, double, double, double, double, bool, bool)), doc, SLOT(itemSelection_SetItemStrokePatternProps(double, double, double, double, double, double, double, double, bool, bool)));

		connect(currentDoc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		connect(currentDoc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
		connect(currentDoc->scMW()     , SIGNAL(UpdateRequest(int)), this, SLOT(handleUpdateRequest(int)));
	}

	if (oldDoc != currentDoc)
	{
		displayGradient(0);
	}
}

void Cpalette::updateFromItem()
{
	if (currentItem == NULL)
		return;
	if (!currentDoc)
		return;
	disconnect(namedGradient, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradient(const QString &)));
	disconnect(namedGradientStroke, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradientStroke(const QString &)));
	disconnect(strokeModeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(slotGradStroke(int)));
	disconnect(gradientTypeStroke, SIGNAL(activated(int)), this, SLOT(slotGradTypeStroke(int)));
	disconnect(gradEdit      , SIGNAL(gradientChanged()), this, SLOT(handleFillGradient()));
	disconnect(gradEditStroke, SIGNAL(gradientChanged()), this, SLOT(handleStrokeGradient()));
	disconnect(gradientType, SIGNAL(activated(int)), this, SLOT(slotGradType(int)));
	disconnect(colorPoint1, SIGNAL(activated(int)), this, SLOT(setGradientColors()));
	disconnect(colorPoint2, SIGNAL(activated(int)), this, SLOT(setGradientColors()));
	disconnect(colorPoint3, SIGNAL(activated(int)), this, SLOT(setGradientColors()));
	disconnect(colorPoint4, SIGNAL(activated(int)), this, SLOT(setGradientColors()));
	disconnect(color1Alpha, SIGNAL(valueChanged(int)), this, SLOT(setGradientColors()));
	disconnect(color2Alpha, SIGNAL(valueChanged(int)), this, SLOT(setGradientColors()));
	disconnect(color3Alpha, SIGNAL(valueChanged(int)), this, SLOT(setGradientColors()));
	disconnect(color4Alpha, SIGNAL(valueChanged(int)), this, SLOT(setGradientColors()));
	disconnect(color1Shade, SIGNAL(valueChanged(int)), this, SLOT(setGradientColors()));
	disconnect(color2Shade, SIGNAL(valueChanged(int)), this, SLOT(setGradientColors()));
	disconnect(color3Shade, SIGNAL(valueChanged(int)), this, SLOT(setGradientColors()));
	disconnect(color4Shade, SIGNAL(valueChanged(int)), this, SLOT(setGradientColors()));
	disconnect(colorMeshPoint, SIGNAL(activated(int)), this, SLOT(updateMeshPoint()));
	disconnect(shadeMeshPoint, SIGNAL(valueChanged(int)), this, SLOT(updateMeshPoint()));
	disconnect(transparencyMeshPoint, SIGNAL(valueChanged(int)), this, SLOT(updateMeshPoint()));
	updateCList();
	displayOverprint(currentItem->doOverprint ? 1 : 0);
	displayColorValues(currentItem->lineColor(), currentItem->fillColor(), currentItem->lineShade(), currentItem->fillShade());
	displayGradient(currentItem->GrType);
	gradEdit->setGradient(currentItem->fill_gradient);
	gradEditStroke->setGradient(currentItem->stroke_gradient);
	if (!currentItem->gradient().isEmpty())
	{
		setCurrentComboItem(namedGradient, currentItem->gradient());
		gradEdit->setGradientEditable(false);
	}
	else
	{
		namedGradient->setCurrentIndex(0);
		gradEdit->setGradientEditable(true);
	}
	if (!currentItem->strokeGradient().isEmpty())
	{
		setCurrentComboItem(namedGradientStroke, currentItem->strokeGradient());
		gradEditStroke->setGradientEditable(false);
	}
	else
	{
		namedGradientStroke->setCurrentIndex(0);
		gradEditStroke->setGradientEditable(true);
	}
	if (currentItem->GrTypeStroke > 0)
	{
		if (currentItem->GrTypeStroke == 6)
			gradientTypeStroke->setCurrentIndex(0);
		else
			gradientTypeStroke->setCurrentIndex(1);
	}
	/*if (patternList->count() == 0)
	{
		tabWidgetStroke->setTabEnabled(2, false);
		tabWidget->setTabEnabled(2, false);
	}
	else
	{
		tabWidgetStroke->setTabEnabled(2, true);
		tabWidget->setTabEnabled(2, true);
	}*/
	enablePatterns(patternList->count() != 0);
	if (!currentItem->strokePattern().isEmpty())
		strokeModeCombo->setCurrentIndex(2);
	else if (currentItem->GrTypeStroke > 0)
		strokeModeCombo->setCurrentIndex(1);
	else
		strokeModeCombo->setCurrentIndex(0);
	setMeshPoint();
	editMeshColors->setEnabled(!CGradDia->isVisible());
	gradEditButton->setEnabled(true);

	double patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY, patternSpace;
	bool mirrorX, mirrorY;
	currentItem->patternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY);
	currentItem->patternFlip(mirrorX, mirrorY);
	setActPattern(currentItem->pattern(), patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY, mirrorX, mirrorY);
	currentItem->strokePatternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY, patternSpace);
	currentItem->strokePatternFlip(mirrorX, mirrorY);
	setActPatternStroke(currentItem->strokePattern(), patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY, mirrorX, mirrorY, patternSpace, currentItem->isStrokePatternToPath());

	connect(namedGradient, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradient(const QString &)));
	connect(namedGradientStroke, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradientStroke(const QString &)));
	connect(strokeModeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(slotGradStroke(int)));
	connect(gradientTypeStroke, SIGNAL(activated(int)), this, SLOT(slotGradTypeStroke(int)));
	connect(gradEdit      , SIGNAL(gradientChanged()), this, SLOT(handleFillGradient()));
	connect(gradEditStroke, SIGNAL(gradientChanged()), this, SLOT(handleStrokeGradient()));
	connect(gradientType, SIGNAL(activated(int)), this, SLOT(slotGradType(int)));
	connect(colorPoint1, SIGNAL(activated(int)), this, SLOT(setGradientColors()));
	connect(colorPoint2, SIGNAL(activated(int)), this, SLOT(setGradientColors()));
	connect(colorPoint3, SIGNAL(activated(int)), this, SLOT(setGradientColors()));
	connect(colorPoint4, SIGNAL(activated(int)), this, SLOT(setGradientColors()));
	connect(color1Alpha, SIGNAL(valueChanged(int)), this, SLOT(setGradientColors()));
	connect(color2Alpha, SIGNAL(valueChanged(int)), this, SLOT(setGradientColors()));
	connect(color3Alpha, SIGNAL(valueChanged(int)), this, SLOT(setGradientColors()));
	connect(color4Alpha, SIGNAL(valueChanged(int)), this, SLOT(setGradientColors()));
	connect(color1Shade, SIGNAL(valueChanged(int)), this, SLOT(setGradientColors()));
	connect(color2Shade, SIGNAL(valueChanged(int)), this, SLOT(setGradientColors()));
	connect(color3Shade, SIGNAL(valueChanged(int)), this, SLOT(setGradientColors()));
	connect(color4Shade, SIGNAL(valueChanged(int)), this, SLOT(setGradientColors()));
	connect(colorMeshPoint, SIGNAL(activated(int)), this, SLOT(updateMeshPoint()));
	connect(shadeMeshPoint, SIGNAL(valueChanged(int)), this, SLOT(updateMeshPoint()));
	connect(transparencyMeshPoint, SIGNAL(valueChanged(int)), this, SLOT(updateMeshPoint()));
}

PageItem* Cpalette::currentItemFromSelection()
{
	PageItem *currentItem = NULL;

	if (currentDoc)
	{
		if (currentDoc->m_Selection->count() > 1)
		{
			uint lowestItem = 999999;
			for (int a=0; a < currentDoc->m_Selection->count(); ++a)
			{
				currentItem = currentDoc->m_Selection->itemAt(a);
				lowestItem = qMin(lowestItem, currentItem->ItemNr);
			}
			currentItem = currentDoc->Items->at(lowestItem);
		}
		else if (currentDoc->m_Selection->count() == 1)
		{
			currentItem = currentDoc->m_Selection->itemAt(0);
		}
	}

	return currentItem;
}

void Cpalette::enablePatterns(bool enable)
{
	if (enable)
	{
		if (fillModeCombo->count() < 3)
			fillModeCombo->addItem( tr("Pattern") );
		if (strokeModeCombo->count() < 3)
			strokeModeCombo->addItem( tr("Pattern") );
	}
	else
	{
		if (fillModeCombo->count() == 3)
			fillModeCombo->removeItem(2);
		if (strokeModeCombo->count() == 3)
			strokeModeCombo->removeItem(2);
	}
}

void Cpalette::handleSelectionChanged()
{
	if (currentDoc && !updatesBlocked())
	{
		PageItem* currentItem = currentItemFromSelection();
		setCurrentItem(currentItem);
		updateFromItem();
	}
}

void Cpalette::handleUpdateRequest(int updateFlags)
{
	if (updateFlags & reqColorsUpdate)
		updateColorList();
}

void Cpalette::updateColorList()
{
	if (currentDoc)
	{
		this->setColors(currentDoc->PageColors);
		this->setGradients(&currentDoc->docGradients);
		this->setPatterns(&currentDoc->docPatterns);
	}
}

void Cpalette::updateCList()
{
	disconnect(colorListStroke, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(selectColorS(QListWidgetItem*)));
	disconnect(colorListFill, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(selectColorF(QListWidgetItem*)));
	colorListStroke->clear();
	colorListStroke->addItem(CommonStrings::tr_NoneColor);
	colorListFill->clear();
	colorListFill->addItem(CommonStrings::tr_NoneColor);
	if (displayAllColors->isChecked())
	{
		if (currentDoc != NULL)
			currentDoc->getUsedColors(colorList);
	}
	gradEditStroke->setColors(colorList);
	gradEdit->setColors(colorList);
	colorPoint1->updateBox(colorList, ColorCombo::fancyPixmaps, true);
	colorPoint2->updateBox(colorList, ColorCombo::fancyPixmaps, true);
	colorPoint3->updateBox(colorList, ColorCombo::fancyPixmaps, true);
	colorPoint4->updateBox(colorList, ColorCombo::fancyPixmaps, true);
	colorMeshPoint->updateBox(colorList, ColorCombo::fancyPixmaps, true);
	colorListFill->insertItems(colorList, ColorListBox::fancyPixmap);
	colorListStroke->insertItems(colorList, ColorListBox::fancyPixmap);
	if (colorListFill->currentItem())
		colorListFill->currentItem()->setSelected(false);
	if (colorListStroke->currentItem())
		colorListStroke->currentItem()->setSelected(false);
	connect(colorListStroke, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(selectColorS(QListWidgetItem*)));
	connect(colorListFill, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(selectColorF(QListWidgetItem*)));
}

void Cpalette::toggleColorDisplay()
{
	if (currentDoc != NULL)
	{
		colorListStroke->cList = &currentDoc->PageColors;
		colorListFill->cList = &currentDoc->PageColors;
		colorList = currentDoc->PageColors;
		updateFromItem();
	}
}

void Cpalette::displayOverprint(int val)
{
	disconnect(overPrintCombo, SIGNAL(activated(int)), this, SIGNAL(NewOverprint(int)));
	overPrintCombo->setCurrentIndex(val);
	connect(overPrintCombo, SIGNAL(activated(int)), this, SIGNAL(NewOverprint(int)));
}

void Cpalette::handleFillShade(int val)
{
	if (currentDoc)
	{
		blockUpdates(true);
		currentDoc->itemSelection_SetItemBrushShade(val);
		blockUpdates(false);
	}
}

void Cpalette::handleStrokeShade(int val)
{
	if (currentDoc)
	{
		blockUpdates(true);
		currentDoc->itemSelection_SetItemPenShade(val);
		blockUpdates(false);
	}
}

void Cpalette::handleOverprint(int val)
{
	if (currentDoc)
	{
		bool setter = true;
		if (val == 0)
			setter = false;
		currentDoc->itemSelection_SetOverprint(setter);
	}
}

void Cpalette::handleFillGradient()
{
	if (currentDoc)
	{
		VGradient gradient(gradEdit->gradient());
		blockUpdates(true);
		currentDoc->updateManager()->setUpdatesDisabled();
		currentDoc->itemSelection_SetFillGradient(gradient);
		currentDoc->updateManager()->setUpdatesEnabled();
		blockUpdates(false);
	}
}

void Cpalette::handleStrokeGradient()
{
	if (currentDoc)
	{
		VGradient gradient(gradEditStroke->gradient());
		blockUpdates(true);
		currentDoc->updateManager()->setUpdatesDisabled();
		currentDoc->itemSelection_SetLineGradient(gradient);
		currentDoc->updateManager()->setUpdatesEnabled();
		blockUpdates(false);
	}
}

void Cpalette::displayColorValues(QString stroke, QString fill, int sShade, int fShade)
{
	disconnect(fillShade, SIGNAL(valueChanged(int)), this, SIGNAL(NewBrushShade(int)));
	disconnect(strokeShade, SIGNAL(valueChanged(int)), this, SIGNAL(NewPenShade(int)));
	disconnect(colorListStroke, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(selectColorS(QListWidgetItem*)));
	disconnect(colorListFill, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(selectColorF(QListWidgetItem*)));
	strokeShade->setValue(sShade);
	fillShade->setValue(fShade);
	if ((stroke != CommonStrings::None) && (!stroke.isEmpty()))
	{
		QList<QListWidgetItem *> cCol = colorListStroke->findItems(stroke, Qt::MatchExactly);
		if (cCol.count() != 0)
			colorListStroke->setCurrentItem(cCol[0]);
	}
	else
		colorListStroke->setCurrentRow(0);
	if ((fill != CommonStrings::None) && (!fill.isEmpty()))
	{
		QList<QListWidgetItem *> cCol = colorListFill->findItems(fill, Qt::MatchExactly);
		if (cCol.count() != 0)
			colorListFill->setCurrentItem(cCol[0]);
	}
	else
		colorListFill->setCurrentRow(0);
	connect(fillShade, SIGNAL(valueChanged(int)), this, SIGNAL(NewBrushShade(int)));
	connect(strokeShade, SIGNAL(valueChanged(int)), this, SIGNAL(NewPenShade(int)));
	connect(colorListStroke, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(selectColorS(QListWidgetItem*)));
	connect(colorListFill, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(selectColorF(QListWidgetItem*)));
}

void Cpalette::selectColorS(QListWidgetItem *item)
{
	ColorPixmapItem* c = dynamic_cast<ColorPixmapItem*>(item);
	if (c != NULL)	
		sFarbe = c->colorName();
	else if (! item->data(Qt::DisplayRole).toString().isEmpty()) 
		sFarbe = item->data(Qt::DisplayRole).toString();
	else
		return;
	emit NewPen(sFarbe);
}

void Cpalette::selectColorF(QListWidgetItem *item)
{
	ColorPixmapItem* c = dynamic_cast<ColorPixmapItem*>(item);
	if (c != NULL)	
		sFarbe = c->colorName();
	else if (! item->data(Qt::DisplayRole).toString().isEmpty()) 
		sFarbe = item->data(Qt::DisplayRole).toString();
	else
		return;
	emit NewBrush(sFarbe);
}

void Cpalette::setColors(ColorList newColorList)
{
	colorList.clear();
	colorList = newColorList;
	updateCList();
}

void Cpalette::fillStrokeSelector(int /*index*/)
{
	updateFromItem();
}

/*void Cpalette::editLineColorSelectorButton()
{
	if (editLineColorSelector->isChecked())
	{
		stackedWidget->setCurrentIndex(0);
		editFillColorSelector->setChecked(false);
	}
	updateFromItem();
}

void Cpalette::editFillColorSelectorButton()
{
	if (editFillColorSelector->isChecked())
	{
		stackedWidget->setCurrentIndex(1);
		editLineColorSelector->setChecked(false);
	}
	updateFromItem();
}*/

void Cpalette::updateGradientList()
{
	disconnect(namedGradient, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradient(const QString &)));
	disconnect(namedGradientStroke, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradientStroke(const QString &)));
	namedGradient->clear();
	namedGradient->setIconSize(QSize(48, 12));
	namedGradient->addItem( tr("Custom"));
	namedGradientStroke->clear();
	namedGradientStroke->setIconSize(QSize(48, 12));
	namedGradientStroke->addItem( tr("Custom"));
	for (QMap<QString, VGradient>::Iterator it = gradientList->begin(); it != gradientList->end(); ++it)
	{
		QImage pixm(48, 12, QImage::Format_ARGB32_Premultiplied);
		QPainter pb;
		QBrush b(QColor(205,205,205), loadIcon("testfill.png"));
		pb.begin(&pixm);
		pb.fillRect(0, 0, 48, 12, b);
		pb.end();
		ScPainter *p = new ScPainter(&pixm, 48, 12);
		p->setPen(Qt::black);
		p->setLineWidth(1);
		p->setFillMode(2);
		p->fill_gradient = it.value();
		p->setGradient(VGradient::linear, FPoint(0,6), FPoint(48, 6), FPoint(0,0), 1, 0);
		p->drawRect(0, 0, 48, 12);
		p->end();
		delete p;
		QPixmap pm;
		pm = QPixmap::fromImage(pixm);
		namedGradient->addItem(pm, it.key());
		namedGradientStroke->addItem(pm, it.key());
	}
	connect(namedGradient, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradient(const QString &)));
	connect(namedGradientStroke, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradientStroke(const QString &)));
}

void Cpalette::setGradients(QMap<QString, VGradient> *docGradients)
{
	gradientList = docGradients;
	updateGradientList();
}

void Cpalette::setGradientColors()
{
	QString color1 = colorPoint1->currentText();
	if (color1 == CommonStrings::tr_NoneColor)
		color1 = CommonStrings::None;
	QString color2 = colorPoint2->currentText();
	if (color2 == CommonStrings::tr_NoneColor)
		color2 = CommonStrings::None;
	QString color3 = colorPoint3->currentText();
	if (color3 == CommonStrings::tr_NoneColor)
		color3 = CommonStrings::None;
	QString color4 = colorPoint4->currentText();
	if (color4 == CommonStrings::tr_NoneColor)
		color4 = CommonStrings::None;
	double t1 = color1Alpha->value() / 100.0;
	double t2 = color2Alpha->value() / 100.0;
	double t3 = color3Alpha->value() / 100.0;
	double t4 = color4Alpha->value() / 100.0;
	currentItem->set4ColorShade(color1Shade->value(), color2Shade->value(), color3Shade->value(), color4Shade->value());
	currentItem->set4ColorTransparency(t1, t2, t3, t4);
	currentItem->set4ColorColors(color1, color2, color3, color4);
	currentItem->update();
}

void Cpalette::setNamedGradient(const QString &name)
{
	if (namedGradient->currentIndex() == 0)
	{
		gradEdit->setGradient(currentItem->fill_gradient);
		currentItem->setGradient("");
		gradEdit->setGradientEditable(true);
	}
	else
	{
		gradEdit->setGradient(gradientList->value(name));
		gradEdit->setGradientEditable(false);
		currentItem->setGradient(name);
	}
	if (gradientType->currentIndex() == 0)
		emit NewGradient(6);
	else if (gradientType->currentIndex() == 1)
		emit NewGradient(7);
	else if (gradientType->currentIndex() == 2)
	{
		setGradientColors();
		emit NewGradient(9);
	}
	else if (gradientType->currentIndex() == 3)
		emit NewGradient(10);
	else if (gradientType->currentIndex() == 4)
		emit NewGradient(11);
}

void Cpalette::setNamedGradientStroke(const QString &name)
{
	if (namedGradientStroke->currentIndex() == 0)
	{
		gradEditStroke->setGradient(currentItem->stroke_gradient);
		currentItem->setStrokeGradient("");
		gradEditStroke->setGradientEditable(true);
	}
	else
	{
		gradEditStroke->setGradient(gradientList->value(name));
		gradEditStroke->setGradientEditable(false);
		currentItem->setStrokeGradient(name);
	}
	if (gradientTypeStroke->currentIndex() == 0)
		emit NewGradientS(6);
	else
		emit NewGradientS(7);
}

void Cpalette::updatePatternList()
{
	disconnect(patternBox, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPattern(QListWidgetItem*)));
	patternBox->clear();
	patternBox->setIconSize(QSize(48, 48));
	patternBoxStroke->clear();
	patternBoxStroke->setIconSize(QSize(48, 48));
	for (QMap<QString, ScPattern>::Iterator it = patternList->begin(); it != patternList->end(); ++it)
	{
		QPixmap pm;
		if (it.value().getPattern()->width() >= it.value().getPattern()->height())
			pm=QPixmap::fromImage(it.value().getPattern()->scaledToWidth(48, Qt::SmoothTransformation));
		else
			pm=QPixmap::fromImage(it.value().getPattern()->scaledToHeight(48, Qt::SmoothTransformation));
		QPixmap pm2(48, 48);
		pm2.fill(palette().color(QPalette::Base));
		QPainter p;
		p.begin(&pm2);
		p.drawPixmap(24 - pm.width() / 2, 24 - pm.height() / 2, pm);
		p.end();
		QListWidgetItem *item = new QListWidgetItem(pm2, it.key(), patternBox);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		QListWidgetItem *itemS = new QListWidgetItem(pm2, it.key(), patternBoxStroke);
		itemS->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	}
	patternBox->clearSelection();
	patternBoxStroke->clearSelection();
	connect(patternBox, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPattern(QListWidgetItem*)));
}

void Cpalette::hideEditedPatterns(QStringList names)
{
	for (int a = 0; a < names.count(); a++)
	{
		QList<QListWidgetItem*> items = patternBox->findItems(names[a], Qt::MatchExactly);
		if (items.count() > 0)
			items[0]->setHidden(true);
		items = patternBoxStroke->findItems(names[a], Qt::MatchExactly);
		if (items.count() > 0)
			items[0]->setHidden(true);
	}
}

void Cpalette::setPatterns(QMap<QString, ScPattern> *docPatterns)
{
	patternList = docPatterns;
	updatePatternList();
}

void Cpalette::selectPattern(QListWidgetItem *c)
{
	if (c == NULL)
		return;
	emit NewPattern(c->text());
}

void Cpalette::selectPatternS(QListWidgetItem *c)
{
	if (c == NULL)
		return;
	emit NewPatternS(c->text());
}

void Cpalette::setActPatternStroke(QString pattern, double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY, double space, bool pathF)
{
	disconnect(patternBoxStroke, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPatternS(QListWidgetItem*)));
	QList<QListWidgetItem*> itl = patternBoxStroke->findItems(pattern, Qt::MatchExactly);
	if (itl.count() != 0)
	{
		QListWidgetItem *it = itl[0];
		patternBoxStroke->setCurrentItem(it);
	}
	else
		patternBoxStroke->clearSelection();
	m_Pattern_scaleXS = scaleX;
	m_Pattern_scaleYS = scaleX;
	m_Pattern_offsetXS = offsetX;
	m_Pattern_offsetYS = offsetY;
	m_Pattern_rotationS = rotation;
	m_Pattern_skewXS = skewX;
	m_Pattern_skewYS = skewY;
	m_Pattern_mirrorXS = mirrorX;
	m_Pattern_mirrorYS = mirrorY;
	m_Pattern_spaceS = space;
	followsPath->setChecked(pathF);
	connect(patternBoxStroke, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPatternS(QListWidgetItem*)));
}

void Cpalette::setActPattern(QString pattern, double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY)
{
	disconnect(patternBox, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPattern(QListWidgetItem*)));
	QList<QListWidgetItem*> itl = patternBox->findItems(pattern, Qt::MatchExactly);
	if (itl.count() != 0)
	{
		QListWidgetItem *it = itl[0];
		patternBox->setCurrentItem(it);
	}
	else
		patternBox->clearSelection();
	m_Pattern_scaleX = scaleX;
	m_Pattern_scaleY = scaleX;
	m_Pattern_offsetX = offsetX;
	m_Pattern_offsetY = offsetY;
	m_Pattern_rotation = rotation;
	m_Pattern_skewX = skewX;
	m_Pattern_skewY = skewY;
	m_Pattern_mirrorX = mirrorX;
	m_Pattern_mirrorY = mirrorY;
	connect(patternBox, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPattern(QListWidgetItem*)));
}

void Cpalette::slotGradStroke(int number)
{
	if (number == 1)
	{
		disconnect(namedGradientStroke, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradientStroke(const QString &)));
		if (!currentItem->strokeGradient().isEmpty())
		{
			setCurrentComboItem(namedGradientStroke, currentItem->strokeGradient());
			gradEditStroke->setGradient(gradientList->value(currentItem->strokeGradient()));
			gradEditStroke->setGradientEditable(false);
		}
		else
		{
			namedGradientStroke->setCurrentIndex(0);
			gradEditStroke->setGradient(currentItem->stroke_gradient);
			gradEditStroke->setGradientEditable(true);
		}
		emit NewPatternS("");
		if (gradientTypeStroke->currentIndex() == 0)
			emit NewGradientS(6);
		else
			emit NewGradientS(7);
		connect(namedGradientStroke, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradientStroke(const QString &)));
	}
	else if (number == 2)
	{
		emit NewGradientS(8);
		if (patternBoxStroke->currentItem())
			emit NewPatternS(patternBoxStroke->currentItem()->text());
	}
	else
	{
		emit NewGradientS(0);
		emit NewPatternS("");
	}
	strokeModeStack->setCurrentIndex(number);
}

void Cpalette::displayGradient(int number)
{
	disconnect(fillModeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(slotGrad(int)));
	if (number==-1)
	{
		fillModeCombo->setCurrentIndex(0);
	}
	if (number == 0)
		fillModeCombo->setCurrentIndex(0);
	else if (((number > 0) && (number < 8)) || (number == 9) || (number == 10) || (number == 11))
	{
		if ((number == 5) || (number == 7))
		{
			stackedWidget_2->setCurrentIndex(0);
			gradientType->setCurrentIndex(1);
		}
		else if (number == 9)
		{
			stackedWidget_2->setCurrentIndex(1);
			gradientType->setCurrentIndex(2);
			if ((currentItem->GrColorP1 != CommonStrings::None) && (!currentItem->GrColorP1.isEmpty()))
				setCurrentComboItem(colorPoint1, currentItem->GrColorP1);
			else
				colorPoint1->setCurrentIndex(0);
			if ((currentItem->GrColorP2 != CommonStrings::None) && (!currentItem->GrColorP2.isEmpty()))
				setCurrentComboItem(colorPoint2, currentItem->GrColorP2);
			else
				colorPoint2->setCurrentIndex(0);
			if ((currentItem->GrColorP3 != CommonStrings::None) && (!currentItem->GrColorP3.isEmpty()))
				setCurrentComboItem(colorPoint3, currentItem->GrColorP3);
			else
				colorPoint3->setCurrentIndex(0);
			if ((currentItem->GrColorP4 != CommonStrings::None) && (!currentItem->GrColorP4.isEmpty()))
				setCurrentComboItem(colorPoint4, currentItem->GrColorP4);
			else
				colorPoint4->setCurrentIndex(0);
			color1Alpha->setValue(qRound(currentItem->GrCol1transp * 100));
			color2Alpha->setValue(qRound(currentItem->GrCol2transp * 100));
			color3Alpha->setValue(qRound(currentItem->GrCol3transp * 100));
			color4Alpha->setValue(qRound(currentItem->GrCol4transp * 100));
			color1Shade->setValue(currentItem->GrCol1Shade);
			color2Shade->setValue(currentItem->GrCol2Shade);
			color3Shade->setValue(currentItem->GrCol3Shade);
			color4Shade->setValue(currentItem->GrCol4Shade);
		}
		else if (number == 10)
		{
			stackedWidget_2->setCurrentIndex(0);
			gradientType->setCurrentIndex(3);
		}
		else if (number == 11)
		{
			stackedWidget_2->setCurrentIndex(2);
			if ((currentItem->selectedMeshPointX > -1) && (currentItem->selectedMeshPointY > -1l))
			{
				meshPoint mp = currentItem->meshGradientArray[currentItem->selectedMeshPointX][currentItem->selectedMeshPointY];
				setCurrentComboItem(colorMeshPoint, mp.colorName);
				shadeMeshPoint->setValue(mp.shade);
				transparencyMeshPoint->setValue(mp.transparency * 100);
			}
			gradientType->setCurrentIndex(4);
		}
		else
		{
			stackedWidget_2->setCurrentIndex(0);
			gradientType->setCurrentIndex(0);
		}
		fillModeCombo->setCurrentIndex(1);
	}
	else
	{
		if (patternList->count() == 0)
		{
			fillModeCombo->setCurrentIndex(0);
			emit NewGradient(0);
		}
		else
			fillModeCombo->setCurrentIndex(2);
	}
	fillModeStack->setCurrentIndex( fillModeCombo->currentIndex() );
	connect(fillModeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(slotGrad(int)));
}

void Cpalette::slotGrad(int number)
{
	if (number == 1)
	{
		disconnect(namedGradient, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradient(const QString &)));
		disconnect(gradientType, SIGNAL(activated(int)), this, SLOT(slotGradType(int)));
		disconnect(gradEdit, SIGNAL(gradientChanged()) , this, SLOT(handleFillGradient()));
		if (!currentItem->gradient().isEmpty())
		{
			setCurrentComboItem(namedGradient, currentItem->gradient());
			gradEdit->setGradient(gradientList->value(currentItem->gradient()));
			gradEdit->setGradientEditable(false);
		}
		else
		{
			namedGradient->setCurrentIndex(0);
			gradEdit->setGradient(currentItem->fill_gradient);
			gradEdit->setGradientEditable(true);
		}
		if (gradientType->currentIndex() == 0)
		{
			stackedWidget_2->setCurrentIndex(0);
			emit NewGradient(6);
		}
		else if (gradientType->currentIndex() == 1)
		{
			stackedWidget_2->setCurrentIndex(0);
			emit NewGradient(7);
		}
		else if (gradientType->currentIndex() == 2)
		{
			stackedWidget_2->setCurrentIndex(1);
			if ((currentItem->GrColorP1 != CommonStrings::None) && (!currentItem->GrColorP1.isEmpty()))
				setCurrentComboItem(colorPoint1, currentItem->GrColorP1);
			else
				colorPoint1->setCurrentIndex(0);
			if ((currentItem->GrColorP2 != CommonStrings::None) && (!currentItem->GrColorP2.isEmpty()))
				setCurrentComboItem(colorPoint2, currentItem->GrColorP2);
			else
				colorPoint2->setCurrentIndex(0);
			if ((currentItem->GrColorP3 != CommonStrings::None) && (!currentItem->GrColorP3.isEmpty()))
				setCurrentComboItem(colorPoint3, currentItem->GrColorP3);
			else
				colorPoint3->setCurrentIndex(0);
			if ((currentItem->GrColorP4 != CommonStrings::None) && (!currentItem->GrColorP4.isEmpty()))
				setCurrentComboItem(colorPoint4, currentItem->GrColorP4);
			else
				colorPoint4->setCurrentIndex(0);
			color1Alpha->setValue(qRound(currentItem->GrCol1transp * 100));
			color2Alpha->setValue(qRound(currentItem->GrCol2transp * 100));
			color3Alpha->setValue(qRound(currentItem->GrCol3transp * 100));
			color4Alpha->setValue(qRound(currentItem->GrCol4transp * 100));
			color1Shade->setValue(currentItem->GrCol1Shade);
			color2Shade->setValue(currentItem->GrCol2Shade);
			color3Shade->setValue(currentItem->GrCol3Shade);
			color4Shade->setValue(currentItem->GrCol4Shade);
			emit NewGradient(9);
		}
		else if (gradientType->currentIndex() == 3)
		{
			stackedWidget_2->setCurrentIndex(0);
			emit NewGradient(10);
		}
		else if (gradientType->currentIndex() == 4)
		{
			stackedWidget_2->setCurrentIndex(2);
			if ((currentItem->selectedMeshPointX > -1) && (currentItem->selectedMeshPointY > -1l))
			{
				meshPoint mp = currentItem->meshGradientArray[currentItem->selectedMeshPointX][currentItem->selectedMeshPointY];
				setCurrentComboItem(colorMeshPoint, mp.colorName);
				shadeMeshPoint->setValue(mp.shade);
				transparencyMeshPoint->setValue(mp.transparency * 100);
			}
			emit NewGradient(11);
		}
		connect(namedGradient, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradient(const QString &)));
		connect(gradientType, SIGNAL(activated(int)), this, SLOT(slotGradType(int)));
		connect(gradEdit, SIGNAL(gradientChanged()) , this, SLOT(handleFillGradient()));
	}
	else if (number == 2)
		emit NewGradient(8);
	else
		emit NewGradient(0);
	fillModeStack->setCurrentIndex(number);
}

void Cpalette::slotGradType(int type)
{
	if (type == 0)
	{
		stackedWidget_2->setCurrentIndex(0);
		emit NewGradient(6);
	}
	else if (type == 1)
	{
		stackedWidget_2->setCurrentIndex(0);
		emit NewGradient(7);
	}
	else if (type == 2)
	{
		stackedWidget_2->setCurrentIndex(1);
		if ((currentItem->GrColorP1 != CommonStrings::None) && (!currentItem->GrColorP1.isEmpty()))
			setCurrentComboItem(colorPoint1, currentItem->GrColorP1);
		else
			colorPoint1->setCurrentIndex(0);
		if ((currentItem->GrColorP2 != CommonStrings::None) && (!currentItem->GrColorP2.isEmpty()))
			setCurrentComboItem(colorPoint2, currentItem->GrColorP2);
		else
			colorPoint2->setCurrentIndex(0);
		if ((currentItem->GrColorP3 != CommonStrings::None) && (!currentItem->GrColorP3.isEmpty()))
			setCurrentComboItem(colorPoint3, currentItem->GrColorP3);
		else
			colorPoint3->setCurrentIndex(0);
		if ((currentItem->GrColorP4 != CommonStrings::None) && (!currentItem->GrColorP4.isEmpty()))
			setCurrentComboItem(colorPoint4, currentItem->GrColorP4);
		else
			colorPoint4->setCurrentIndex(0);
		color1Alpha->setValue(qRound(currentItem->GrCol1transp * 100));
		color2Alpha->setValue(qRound(currentItem->GrCol2transp * 100));
		color3Alpha->setValue(qRound(currentItem->GrCol3transp * 100));
		color4Alpha->setValue(qRound(currentItem->GrCol4transp * 100));
		color1Shade->setValue(currentItem->GrCol1Shade);
		color2Shade->setValue(currentItem->GrCol2Shade);
		color3Shade->setValue(currentItem->GrCol3Shade);
		color4Shade->setValue(currentItem->GrCol4Shade);
		emit NewGradient(9);
	}
	else if (type == 3)
	{
		stackedWidget_2->setCurrentIndex(0);
		emit NewGradient(10);
	}
	else if (type == 4)
	{
		stackedWidget_2->setCurrentIndex(2);
		if ((currentItem->selectedMeshPointX > -1) && (currentItem->selectedMeshPointY > -1l))
		{
			meshPoint mp = currentItem->meshGradientArray[currentItem->selectedMeshPointX][currentItem->selectedMeshPointY];
			setCurrentComboItem(colorMeshPoint, mp.colorName);
			shadeMeshPoint->setValue(mp.shade);
			transparencyMeshPoint->setValue(mp.transparency * 100);
		}
		emit NewGradient(11);
	}
}

void Cpalette::slotGradTypeStroke(int type)
{
	if (type == 0)
		emit NewGradientS(6);
	else
		emit NewGradientS(7);
}

void Cpalette::editMeshPointColor()
{
	if (editMeshColors->isChecked())
	{
		if (currentItem->GrType == 11)
			editStrokeGradient = 6;
		else
			editStrokeGradient = 0;
		gradEditButton->setEnabled(false);
	}
	else
	{
		editStrokeGradient = 0;
		gradEditButton->setEnabled(true);
	}
	emit editGradient(editStrokeGradient);
}

void Cpalette::createNewMeshGradient()
{
	InsertTable* dia = new InsertTable(this, 255, 255);
	dia->setWindowTitle( tr( "Create Mesh" ) );
	if (dia->exec())
	{
		currentItem->createGradientMesh(dia->Rows->value(), dia->Cols->value());
		currentItem->update();
		currentDoc->regionsChanged()->update(QRect());
	}
	delete dia;
}

void Cpalette::resetMeshGradient()
{
	currentItem->resetGradientMesh();
	currentItem->update();
	currentDoc->regionsChanged()->update(QRect());
}

void Cpalette::meshGradientToShape()
{
	currentItem->meshToShape();
	currentItem->update();
	currentDoc->regionsChanged()->update(QRect());
}

void Cpalette::resetOneControlPoint()
{
	int grow = currentItem->selectedMeshPointX;
	int gcol = currentItem->selectedMeshPointY;
	int cont = currentItem->selectedMeshControlPoint;
	if ((grow == -1) || (gcol == -1))
		return;
	if (cont == 1)
		currentItem->meshGradientArray[grow][gcol].controlTop    = currentItem->meshGradientArray[grow][gcol].gridPoint;
	else if (cont == 2)
		currentItem->meshGradientArray[grow][gcol].controlBottom = currentItem->meshGradientArray[grow][gcol].gridPoint;
	else if (cont == 3)
		currentItem->meshGradientArray[grow][gcol].controlLeft   = currentItem->meshGradientArray[grow][gcol].gridPoint;
	else if (cont == 4)
		currentItem->meshGradientArray[grow][gcol].controlRight  = currentItem->meshGradientArray[grow][gcol].gridPoint;
	currentItem->update();
	currentDoc->regionsChanged()->update(QRect());
}

void Cpalette::resetAllControlPoints()
{
	int grow = currentItem->selectedMeshPointX;
	int gcol = currentItem->selectedMeshPointY;
	if ((grow == -1) || (gcol == -1))
		return;
	currentItem->meshGradientArray[grow][gcol].controlTop    = currentItem->meshGradientArray[grow][gcol].gridPoint;
	currentItem->meshGradientArray[grow][gcol].controlLeft   = currentItem->meshGradientArray[grow][gcol].gridPoint;
	currentItem->meshGradientArray[grow][gcol].controlRight  = currentItem->meshGradientArray[grow][gcol].gridPoint;
	currentItem->meshGradientArray[grow][gcol].controlBottom = currentItem->meshGradientArray[grow][gcol].gridPoint;
	currentItem->update();
	currentDoc->regionsChanged()->update(QRect());
}

void Cpalette::editGradientVector()
{
	if (gradEditButton->isChecked())
	{
		CGradDia->unitChange(currentDoc->unitIndex());
		CGradDia->setValues(currentItem->GrStartX, currentItem->GrStartY, currentItem->GrEndX, currentItem->GrEndY, currentItem->GrFocalX, currentItem->GrFocalY, currentItem->GrScale, currentItem->GrSkew, 0, 0);
		if (currentItem->GrType == 6)
			CGradDia->selectLinear();
		else if (currentItem->GrType == 7)
			CGradDia->selectRadial();
		else if (currentItem->GrType == 9)
		{
			CGradDia->setValues(currentItem->GrControl1.x(), currentItem->GrControl1.y(), currentItem->GrControl2.x(), currentItem->GrControl2.y(), currentItem->GrControl3.x(), currentItem->GrControl3.y(), currentItem->GrControl4.x(), currentItem->GrControl4.y(), 0, 0);
			CGradDia->selectFourColor();
		}
		else if (currentItem->GrType == 10)
		{
			CGradDia->setValues(currentItem->GrControl1.x(), currentItem->GrControl1.y(), currentItem->GrControl2.x(), currentItem->GrControl2.y(), currentItem->GrControl3.x(), currentItem->GrControl3.y(), currentItem->GrControl4.x(), currentItem->GrControl4.y(), currentItem->GrControl5.x(), currentItem->GrControl5.y());
			CGradDia->selectDiamond();
		}
		else if (currentItem->GrType == 11)
		{
			CGradDia->selectMesh();
			editMeshColors->setEnabled(false);
		}
		CGradDia->show();
	}
	else
	{
		CGradDia->hide();
		editMeshColors->setEnabled(true);
	}
	if (currentItem->GrType == 9)
		editStrokeGradient = 3;
	else if (currentItem->GrType == 10)
		editStrokeGradient = 4;
	else if (currentItem->GrType == 11)
		editStrokeGradient = 5;
	else
		editStrokeGradient = 0;
	emit editGradient(editStrokeGradient);
}

void Cpalette::editGradientVectorStroke()
{
	if (gradEditButtonStroke->isChecked())
	{
		CGradDia->unitChange(currentDoc->unitIndex());
		CGradDia->setValues(currentItem->GrStrokeStartX, currentItem->GrStrokeStartY, currentItem->GrStrokeEndX, currentItem->GrStrokeEndY, currentItem->GrStrokeFocalX, currentItem->GrStrokeFocalY, currentItem->GrStrokeScale, currentItem->GrStrokeSkew, 0, 0);
		if (currentItem->GrTypeStroke == 6)
			CGradDia->selectLinear();
		else
			CGradDia->selectRadial();
		CGradDia->show();
	}
	else
	{
		CGradDia->hide();
	}
	editStrokeGradient = 1;
	emit editGradient(editStrokeGradient);
}

void Cpalette::setActiveGradDia(bool active)
{
	if (!active)
	{
		if (editStrokeGradient == 1)
			gradEditButtonStroke->setChecked(false);
		else
			gradEditButton->setChecked(false);
		emit editGradient(editStrokeGradient);
		editMeshColors->setEnabled(true);
	}
}

void Cpalette::setSpecialGradient(double x1, double y1, double x2, double y2, double fx, double fy, double sg, double sk, double cx, double cy)
{
	if (CGradDia)
		CGradDia->setValues(x1, y1, x2, y2, fx, fy, sg, sk, cx, cy);
}

void Cpalette::setMeshPoint()
{
	if ((currentItem->selectedMeshPointX > -1) && (currentItem->selectedMeshPointY > -1l))
	{
		colorMeshPoint->setEnabled(true);
		shadeMeshPoint->setEnabled(true);
		transparencyMeshPoint->setEnabled(true);
		meshPoint mp = currentItem->meshGradientArray[currentItem->selectedMeshPointX][currentItem->selectedMeshPointY];
		setCurrentComboItem(colorMeshPoint, mp.colorName);
		shadeMeshPoint->setValue(mp.shade);
		transparencyMeshPoint->setValue(mp.transparency * 100);
	}
	else
	{
		colorMeshPoint->setEnabled(false);
		shadeMeshPoint->setEnabled(false);
		transparencyMeshPoint->setEnabled(false);
	}
}

void Cpalette::setMeshControlPoint()
{
}

void Cpalette::updateMeshPoint()
{
	QString color = colorMeshPoint->currentText();
	if (color == CommonStrings::tr_NoneColor)
		color = CommonStrings::None;
	double t = transparencyMeshPoint->value() / 100.0;
	currentItem->setMeshPointColor(currentItem->selectedMeshPointX, currentItem->selectedMeshPointY, color, shadeMeshPoint->value(), t);
	currentItem->update();
}

void Cpalette::changePatternProps()
{
	PatternPropsDialog *dia = new PatternPropsDialog(this, currentUnit, false);
	dia->spinXscaling->setValue(m_Pattern_scaleX);
	dia->spinYscaling->setValue(m_Pattern_scaleY);
	dia->spinXoffset->setValue(m_Pattern_offsetX);
	dia->spinYoffset->setValue(m_Pattern_offsetY);
	dia->spinAngle->setValue(m_Pattern_rotation);
	double asina = atan(m_Pattern_skewX);
	dia->spinXSkew->setValue(asina / (M_PI / 180.0));
	double asinb = atan(m_Pattern_skewY);
	dia->spinYSkew->setValue(asinb / (M_PI / 180.0));
	dia->FlipH->setChecked(m_Pattern_mirrorX);
	dia->FlipV->setChecked(m_Pattern_mirrorY);
	connect(dia, SIGNAL(NewPatternProps(double, double, double, double, double, double, double, bool, bool)), this, SIGNAL(NewPatternProps(double, double, double, double, double, double, double, bool, bool)));
	dia->exec();
	m_Pattern_scaleX = dia->spinXscaling->value();
	m_Pattern_scaleY = dia->spinYscaling->value();
	m_Pattern_offsetX = dia->spinXoffset->value();
	m_Pattern_offsetY = dia->spinYoffset->value();
	m_Pattern_rotation = dia->spinAngle->value();
	double skewX = dia->spinXSkew->value();
	double a;
	if (skewX == 90)
		a = 1;
	else if (skewX == 180)
		a = 0;
	else if (skewX == 270)
		a = -1;
	else if (skewX == 360)
		a = 0;
	else
		a = tan(M_PI / 180.0 * skewX);
	m_Pattern_skewX = tan(a);
	skewX = dia->spinYSkew->value();
	if (skewX == 90)
		a = 1;
	else if (skewX == 180)
		a = 0;
	else if (skewX == 270)
		a = -1;
	else if (skewX == 360)
		a = 0;
	else
		a = tan(M_PI / 180.0 * skewX);
	m_Pattern_skewY = tan(a);
	m_Pattern_mirrorX = dia->FlipH->isChecked();
	m_Pattern_mirrorY = dia->FlipV->isChecked();
	delete dia;
	fillModeCombo->setCurrentIndex(2);
	emit NewGradient(8);
}

void Cpalette::changePatternPropsStroke()
{
	PatternPropsDialog *dia = new PatternPropsDialog(this, currentUnit, true);
	dia->spinXscaling->setValue(m_Pattern_scaleXS);
	dia->spinYscaling->setValue(m_Pattern_scaleYS);
	dia->spinXoffset->setValue(m_Pattern_offsetXS);
	dia->spinYoffset->setValue(m_Pattern_offsetYS);
	dia->spinAngle->setValue(m_Pattern_rotationS);
	dia->spinSpacing->setValue(m_Pattern_spaceS * 100.0);
	double asina = atan(m_Pattern_skewXS);
	dia->spinXSkew->setValue(asina / (M_PI / 180.0));
	double asinb = atan(m_Pattern_skewYS);
	dia->spinYSkew->setValue(asinb / (M_PI / 180.0));
	dia->FlipH->setChecked(m_Pattern_mirrorXS);
	dia->FlipV->setChecked(m_Pattern_mirrorYS);
	connect(dia, SIGNAL(NewPatternPropsS(double, double, double, double, double, double, double, double, bool, bool)), this, SIGNAL(NewPatternPropsS(double, double, double, double, double, double, double, double, bool, bool)));
	dia->exec();
	m_Pattern_scaleXS = dia->spinXscaling->value();
	m_Pattern_scaleYS = dia->spinYscaling->value();
	m_Pattern_offsetXS = dia->spinXoffset->value();
	m_Pattern_offsetYS = dia->spinYoffset->value();
	m_Pattern_rotationS = dia->spinAngle->value();
	double skewX = dia->spinXSkew->value();
	double a;
	if (skewX == 90)
		a = 1;
	else if (skewX == 180)
		a = 0;
	else if (skewX == 270)
		a = -1;
	else if (skewX == 360)
		a = 0;
	else
		a = tan(M_PI / 180.0 * skewX);
	m_Pattern_skewXS = tan(a);
	skewX = dia->spinYSkew->value();
	if (skewX == 90)
		a = 1;
	else if (skewX == 180)
		a = 0;
	else if (skewX == 270)
		a = -1;
	else if (skewX == 360)
		a = 0;
	else
		a = tan(M_PI / 180.0 * skewX);
	m_Pattern_skewYS = tan(a);
	m_Pattern_spaceS = dia->spinSpacing->value() / 100.0;
	m_Pattern_mirrorXS = dia->FlipH->isChecked();
	m_Pattern_mirrorYS = dia->FlipV->isChecked();
	delete dia;
}

void Cpalette::toggleStrokePattern()
{
	emit NewPatternTypeS(followsPath->isChecked());
}

void Cpalette::unitChange(double, double, int unitIndex)
{
	if (CGradDia)
		CGradDia->unitChange(unitIndex);
	currentUnit = unitIndex;
}

void Cpalette::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}
