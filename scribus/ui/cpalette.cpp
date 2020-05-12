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
#include "commonstrings.h"
#include "gradienteditor.h"
#include "iconmanager.h"
#include "insertTable.h"
#include "pageitem.h"
#include "sccolorengine.h"
#include "sclistwidgetdelegate.h"
#include "scpage.h"
#include "scpainter.h"
#include "scpattern.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "scrspinbox.h"
#include "selection.h"
#include "undomanager.h"
#include "units.h"
#include "util.h"
#include "util_math.h"

ColorPalette::ColorPalette(QWidget* parent) : QWidget(parent)
{
	undoManager = UndoManager::instance();
	CGradDia = new GradientVectorDialog(this->parentWidget());
	CGradDia->hide();

	setupUi(this);
	fillModeCombo->addItem( tr("Solid") );
	fillModeCombo->addItem( tr("Gradient") );
	fillModeCombo->addItem( tr("Hatch") );
	fillShade->setDecimals(0);
	strokeShade->setDecimals(0);
	color1Alpha->setDecimals(0);
	color2Alpha->setDecimals(0);
	color3Alpha->setDecimals(0);
	color4Alpha->setDecimals(0);
	color1Shade->setDecimals(0);
	color2Shade->setDecimals(0);
	color3Shade->setDecimals(0);
	color4Shade->setDecimals(0);
	shadeMeshPoint->setDecimals(0);
	strokeModeCombo->addItem( tr("Solid") );
	strokeModeCombo->addItem( tr("Gradient") );

	colorPoint1->setPixmapType(ColorCombo::fancyPixmaps);
	colorPoint2->setPixmapType(ColorCombo::fancyPixmaps);
	colorPoint3->setPixmapType(ColorCombo::fancyPixmaps);
	colorPoint4->setPixmapType(ColorCombo::fancyPixmaps);
	colorMeshPoint->setPixmapType(ColorCombo::fancyPixmaps);
	colorListFill->setPixmapType(ColorListBox::fancyPixmap);
	colorListStroke->setPixmapType(ColorListBox::fancyPixmap);
	hatchLineColor->setPixmapType(ColorCombo::fancyPixmaps);
	hatchBackground->setPixmapType(ColorCombo::fancyPixmaps);

/*  Setting a delegate to dispaly only icons for the patterns */
/*	ScListWidgetDelegate* delegateF = new ScListWidgetDelegate(patternBox, patternBox);
	delegateF->setIconOnly(true);
	patternBox->setItemDelegate(delegateF);
	ScListWidgetDelegate* delegateS = new ScListWidgetDelegate(patternBoxStroke, patternBoxStroke);
	delegateS->setIconOnly(true);
	patternBoxStroke->setItemDelegate(delegateS);
*/
	gradEdit->layout()->setAlignment(Qt::AlignTop);
	gradEditStroke->layout()->setAlignment(Qt::AlignTop);
	gradientTypeStroke->setCurrentIndex(0);
	
	tabFillStroke->setCurrentIndex(0);
	setCurrentItem(nullptr);
	hatchAngle->setDecimals(0);
	hatchAngle->setNewUnit(6);
	hatchAngle->setValues(-360, 360, 1, 0);
	hatchDist->setDecimals(0);
	hatchDist->setNewUnit(0);
	hatchDist->setValues(1, 1000, 0, 1);
	/*editFillColorSelector->setChecked(true);
	editFillColorSelectorButton();*/
}

void ColorPalette::connectSignals()
{
	connect(CGradDia, SIGNAL(NewSpecial(double,double,double,double,double,double,double,double,double,double)), this, SIGNAL(NewSpecial(double,double,double,double,double,double,double,double,double,double)));
	connect(CGradDia, SIGNAL(paletteShown(bool)), this, SLOT(setActiveGradDia(bool)));
	connect(CGradDia, SIGNAL(editGradient(int)), this, SIGNAL(editGradient(int)));
	connect(CGradDia, SIGNAL(createNewMesh()), this, SLOT(createNewMeshGradient()));
	connect(CGradDia, SIGNAL(resetMesh()), this, SLOT(resetMeshGradient()));
	connect(CGradDia, SIGNAL(meshToShape()), this, SLOT(meshGradientToShape()));
	connect(CGradDia, SIGNAL(reset1Control()), this, SLOT(resetOneControlPoint()));
	connect(CGradDia, SIGNAL(resetAllControl()), this, SLOT(resetAllControlPoints()));
	connect(CGradDia, SIGNAL(removePatch()), this, SLOT(handleRemovePatch()));
	connect(CGradDia, SIGNAL(snapToMGrid(bool)), this, SLOT(snapToPatchGrid(bool)));

	connect(colorPoint1    , SIGNAL(activated(int)), this, SLOT(setGradientColors()));
	connect(colorPoint2    , SIGNAL(activated(int)), this, SLOT(setGradientColors()));
	connect(colorPoint3    , SIGNAL(activated(int)), this, SLOT(setGradientColors()));
	connect(colorPoint4    , SIGNAL(activated(int)), this, SLOT(setGradientColors()));
	connect(color1Alpha    , SIGNAL(valueChanged(double)), this, SLOT(setGradientColors()));
	connect(color2Alpha    , SIGNAL(valueChanged(double)), this, SLOT(setGradientColors()));
	connect(color3Alpha    , SIGNAL(valueChanged(double)), this, SLOT(setGradientColors()));
	connect(color4Alpha    , SIGNAL(valueChanged(double)), this, SLOT(setGradientColors()));
	connect(color1Shade    , SIGNAL(valueChanged(double)), this, SLOT(setGradientColors()));
	connect(color2Shade    , SIGNAL(valueChanged(double)), this, SLOT(setGradientColors()));
	connect(color3Shade    , SIGNAL(valueChanged(double)), this, SLOT(setGradientColors()));
	connect(color4Shade    , SIGNAL(valueChanged(double)), this, SLOT(setGradientColors()));
	connect(colorListFill  , SIGNAL(currentRowChanged(int)), this, SLOT(selectColorF(int)));
	connect(colorListStroke, SIGNAL(currentRowChanged(int)), this, SLOT(selectColorS(int)));
	connect(colorMeshPoint , SIGNAL(activated(int)), this, SLOT(updateMeshPoint()));
	connect(editMeshColors , SIGNAL(clicked()), this, SLOT(editMeshPointColor()));
	connect(editPatternProps      , SIGNAL(clicked()) , this, SLOT(changePatternProps()));
	connect(editPatternPropsStroke, SIGNAL(clicked()), this, SLOT(changePatternPropsStroke()));
	connect(fillModeCombo , SIGNAL(currentIndexChanged(int)), this, SLOT(slotGrad(int)));
	connect(fillShade     , SIGNAL(valueChanged(double)), this, SIGNAL(NewBrushShade(double)));
	connect(followsPath   , SIGNAL(clicked()), this, SLOT(toggleStrokePattern()));
	connect(gradientTypeStroke , SIGNAL(activated(int)), this, SLOT(slotGradTypeStroke(int)));
	connect(gradEdit      , SIGNAL(gradientChanged()), this, SLOT(handleFillGradient()));
	connect(gradEditButton, SIGNAL(clicked()), this, SLOT(editGradientVector()));
	connect(gradEditButtonStroke, SIGNAL(clicked()), this, SLOT(editGradientVectorStroke()));
	connect(gradEditStroke, SIGNAL(gradientChanged()), this, SLOT(handleStrokeGradient()));
	connect(gradientType  , SIGNAL(activated(int)), this, SLOT(slotGradType(int)));
	connect(gradientTypeStroke , SIGNAL(activated(int)), this, SLOT(slotGradTypeStroke(int)));
	connect(namedGradient , SIGNAL(activated(const QString &)), this, SLOT(setNamedGradient(const QString &)));
	connect(namedGradientStroke, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradientStroke(const QString &)));
	connect(overPrintCombo     , SIGNAL(activated(int)), this, SIGNAL(NewOverprint(int)));
	connect(patternBox         , SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPattern(QListWidgetItem*)));
	connect(patternBoxStroke   , SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPatternS(QListWidgetItem*)));
	connect(shadeMeshPoint     , SIGNAL(valueChanged(double)), this, SLOT(updateMeshPoint()));
	connect(strokeShade        , SIGNAL(valueChanged(double)), this, SIGNAL(NewPenShade(double)));
	connect(strokeModeCombo    , SIGNAL(currentIndexChanged(int)), this, SLOT(slotGradStroke(int)));
	connect(tabFillStroke      , SIGNAL(currentChanged(int)), this, SLOT(fillStrokeSelector(int)));
	connect(transparencyMeshPoint, SIGNAL(valueChanged(double)), this, SLOT(updateMeshPoint()));
	connect(hatchAngle, SIGNAL(valueChanged(double)), this, SLOT(changeHatchProps()));
	connect(hatchDist, SIGNAL(valueChanged(double)), this, SLOT(changeHatchProps()));
	connect(hatchType, SIGNAL(activated(int)), this, SLOT(changeHatchProps()));
	connect(hatchBackground, SIGNAL(activated(int)), this, SLOT(changeHatchProps()));
	connect(hatchLineColor, SIGNAL(activated(int)), this, SLOT(changeHatchProps()));
	connect(gradientExtend  , SIGNAL(activated(int)), this, SLOT(handleGradientExtend(int)));
	connect(GradientExtendS  , SIGNAL(activated(int)), this, SLOT(handleStrokeGradientExtend(int)));
}

void ColorPalette::disconnectSignals()
{
	disconnect(CGradDia, SIGNAL(NewSpecial(double,double,double,double,double,double,double,double,double,double)), this, SIGNAL(NewSpecial(double,double,double,double,double,double,double,double,double,double)));
	disconnect(CGradDia, SIGNAL(paletteShown(bool)), this, SLOT(setActiveGradDia(bool)));
	disconnect(CGradDia, SIGNAL(editGradient(int)), this, SIGNAL(editGradient(int)));
	disconnect(CGradDia, SIGNAL(createNewMesh()), this, SLOT(createNewMeshGradient()));
	disconnect(CGradDia, SIGNAL(resetMesh()), this, SLOT(resetMeshGradient()));
	disconnect(CGradDia, SIGNAL(meshToShape()), this, SLOT(meshGradientToShape()));
	disconnect(CGradDia, SIGNAL(reset1Control()), this, SLOT(resetOneControlPoint()));
	disconnect(CGradDia, SIGNAL(resetAllControl()), this, SLOT(resetAllControlPoints()));
	disconnect(CGradDia, SIGNAL(removePatch()), this, SLOT(handleRemovePatch()));
	disconnect(CGradDia, SIGNAL(snapToMGrid(bool)), this, SLOT(snapToPatchGrid(bool)));

	disconnect(colorPoint1    , SIGNAL(activated(int)), this, SLOT(setGradientColors()));
	disconnect(colorPoint2    , SIGNAL(activated(int)), this, SLOT(setGradientColors()));
	disconnect(colorPoint3    , SIGNAL(activated(int)), this, SLOT(setGradientColors()));
	disconnect(colorPoint4    , SIGNAL(activated(int)), this, SLOT(setGradientColors()));
	disconnect(color1Alpha    , SIGNAL(valueChanged(double)), this, SLOT(setGradientColors()));
	disconnect(color2Alpha    , SIGNAL(valueChanged(double)), this, SLOT(setGradientColors()));
	disconnect(color3Alpha    , SIGNAL(valueChanged(double)), this, SLOT(setGradientColors()));
	disconnect(color4Alpha    , SIGNAL(valueChanged(double)), this, SLOT(setGradientColors()));
	disconnect(color1Shade    , SIGNAL(valueChanged(double)), this, SLOT(setGradientColors()));
	disconnect(color2Shade    , SIGNAL(valueChanged(double)), this, SLOT(setGradientColors()));
	disconnect(color3Shade    , SIGNAL(valueChanged(double)), this, SLOT(setGradientColors()));
	disconnect(color4Shade    , SIGNAL(valueChanged(double)), this, SLOT(setGradientColors()));
	disconnect(colorListFill  , SIGNAL(currentRowChanged(int)), this, SLOT(selectColorF(int)));
	disconnect(colorListStroke, SIGNAL(currentRowChanged(int)), this, SLOT(selectColorS(int)));
	disconnect(colorMeshPoint , SIGNAL(activated(int)), this, SLOT(updateMeshPoint()));
	disconnect(editMeshColors , SIGNAL(clicked()), this, SLOT(editMeshPointColor()));
	disconnect(editPatternProps      , SIGNAL(clicked()) , this, SLOT(changePatternProps()));
	disconnect(editPatternPropsStroke, SIGNAL(clicked()), this, SLOT(changePatternPropsStroke()));
	disconnect(fillModeCombo , SIGNAL(currentIndexChanged(int)), this, SLOT(slotGrad(int)));
	disconnect(fillShade     , SIGNAL(valueChanged(double)), this, SIGNAL(NewBrushShade(double)));
	disconnect(followsPath   , SIGNAL(clicked()), this, SLOT(toggleStrokePattern()));
	disconnect(gradientTypeStroke , SIGNAL(activated(int)), this, SLOT(slotGradTypeStroke(int)));
	disconnect(gradEdit      , SIGNAL(gradientChanged()), this, SLOT(handleFillGradient()));
	disconnect(gradEditButton, SIGNAL(clicked()), this, SLOT(editGradientVector()));
	disconnect(gradEditButtonStroke, SIGNAL(clicked()), this, SLOT(editGradientVectorStroke()));
	disconnect(gradEditStroke, SIGNAL(gradientChanged()), this, SLOT(handleStrokeGradient()));
	disconnect(gradientType  , SIGNAL(activated(int)), this, SLOT(slotGradType(int)));
	disconnect(gradientTypeStroke , SIGNAL(activated(int)), this, SLOT(slotGradTypeStroke(int)));
	disconnect(namedGradient , SIGNAL(activated(const QString &)), this, SLOT(setNamedGradient(const QString &)));
	disconnect(namedGradientStroke, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradientStroke(const QString &)));
	disconnect(overPrintCombo     , SIGNAL(activated(int)), this, SIGNAL(NewOverprint(int)));
	disconnect(patternBox         , SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPattern(QListWidgetItem*)));
	disconnect(patternBoxStroke   , SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPatternS(QListWidgetItem*)));
	disconnect(shadeMeshPoint, SIGNAL(valueChanged(double)), this, SLOT(updateMeshPoint()));
	disconnect(strokeModeCombo    , SIGNAL(currentIndexChanged(int)), this, SLOT(slotGradStroke(int)));
	disconnect(strokeShade    , SIGNAL(valueChanged(double)), this, SIGNAL(NewPenShade(double)));
	disconnect(tabFillStroke      , SIGNAL(currentChanged(int)), this, SLOT(fillStrokeSelector(int)));
	disconnect(transparencyMeshPoint, SIGNAL(valueChanged(double)), this, SLOT(updateMeshPoint()));
	disconnect(hatchAngle, SIGNAL(valueChanged(double)), this, SLOT(changeHatchProps()));
	disconnect(hatchDist, SIGNAL(valueChanged(double)), this, SLOT(changeHatchProps()));
	disconnect(hatchType, SIGNAL(activated(int)), this, SLOT(changeHatchProps()));
	disconnect(hatchBackground, SIGNAL(activated(int)), this, SLOT(changeHatchProps()));
	disconnect(hatchLineColor, SIGNAL(activated(int)), this, SLOT(changeHatchProps()));
	disconnect(gradientExtend  , SIGNAL(activated(int)), this, SLOT(handleGradientExtend(int)));
	disconnect(GradientExtendS  , SIGNAL(activated(int)), this, SLOT(handleStrokeGradientExtend(int)));
}

void ColorPalette::setCurrentItem(PageItem* item)
{
	if ((item == nullptr) || (currentItem != item))
	{
		editStrokeGradient = 0;
		CGradDia->hide();
		editMeshColors->setEnabled(true);
		gradEditButton->setEnabled(true);
	}

	currentItem = item;
	disconnectSignals();
	
	if (!currentItem)
		return;

	showOverprint(currentItem->doOverprint ? 1 : 0);
	showColorValues(currentItem->lineColor(), currentItem->fillColor(), currentItem->lineShade(), currentItem->fillShade());
	showGradient(currentItem->gradientType());
	showGradientStroke(currentItem->strokeGradientType());
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
	/*if (!currentItem->strokePattern().isEmpty())
		strokeModeCombo->setCurrentIndex(2);
	else if (currentItem->GrTypeStroke > 0)
		strokeModeCombo->setCurrentIndex(1);
	else
		strokeModeCombo->setCurrentIndex(0);*/
	if (currentItem->gradientType() == 12)
		setMeshPatchPoint();
	else
		setMeshPoint();
	if (CGradDia && gradEditButton->isChecked())
	{
		if (tabFillStroke->currentIndex() == 0)
			setGradientVectorValues();
		else
			setGradientVectorStrokeValues();
	}
	editMeshColors->setEnabled(!CGradDia->isVisible());
	gradEditButton->setEnabled(!editMeshColors->isChecked());
	fillModeCombo->setEnabled(!gradEditButton->isChecked() && !editMeshColors->isChecked());
	gradientType->setEnabled(!gradEditButton->isChecked() && !editMeshColors->isChecked());
	strokeModeCombo->setEnabled(!gradEditButtonStroke->isChecked() && !editMeshColors->isChecked());
	gradientTypeStroke->setEnabled(!gradEditButtonStroke->isChecked() && !editMeshColors->isChecked());

	double patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY, patternSpace;
	bool mirrorX, mirrorY;
	currentItem->patternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY);
	currentItem->patternFlip(mirrorX, mirrorY);
	setActPattern(currentItem->pattern(), patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY, mirrorX, mirrorY);
	currentItem->strokePatternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY, patternSpace);
	currentItem->strokePatternFlip(mirrorX, mirrorY);
	setActPatternStroke(currentItem->strokePattern(), patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY, mirrorX, mirrorY, patternSpace, currentItem->isStrokePatternToPath());

	connectSignals();
}

void ColorPalette::updateFromItem()
{
	setCurrentItem(currentItem);
}

void ColorPalette::setDocument(ScribusDoc* doc)
{
	this->disconnect(SIGNAL(NewPen(QString)));
	this->disconnect(SIGNAL(NewBrush(QString)));
	this->disconnect(SIGNAL(NewPenShade(double)));
	this->disconnect(SIGNAL(NewBrushShade(double)));
	this->disconnect(SIGNAL(NewGradient(int)));
	this->disconnect(SIGNAL(NewGradientS(int)));
	this->disconnect(SIGNAL(NewPattern(QString)));
	this->disconnect(SIGNAL(NewPatternProps(double,double,double,double,double,double,double,bool,bool)));
	this->disconnect(SIGNAL(NewOverprint(int)));
	this->disconnect(SIGNAL(NewPatternS(QString)));
	this->disconnect(SIGNAL(NewPatternTypeS(bool)));
	this->disconnect(SIGNAL(NewPatternPropsS(double,double,double,double,double,double,double,double,bool,bool)));
	disconnect(displayAllColors, SIGNAL(clicked()), this, SLOT(toggleColorDisplay()));

	if (currentDoc)
	{
		disconnect(currentDoc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(currentDoc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
		disconnect(currentDoc->scMW()     , SIGNAL(UpdateRequest(int)), this, SLOT(handleUpdateRequest(int)));
	}

	ScribusDoc* oldDoc = currentDoc;
	currentDoc = doc;

	if (doc == nullptr)
	{
		colorListStroke->cList = nullptr;
		colorListFill->cList = nullptr;
		disconnectSignals();
	}
	else
	{
		colorListStroke->cList = &doc->PageColors;
		colorListFill->cList = &doc->PageColors;
		gradEdit->setColors(doc->PageColors);
		currentUnit = doc->unitIndex();

		updateColorList();
		updateCList();

		connect(this, SIGNAL(NewPen(QString))      , doc, SLOT(itemSelection_SetItemPen(QString)));
		connect(this, SIGNAL(NewBrush(QString))    , doc, SLOT(itemSelection_SetItemBrush(QString)));
		connect(this, SIGNAL(NewPenShade(double))     , this, SLOT(handleStrokeShade(double)));
		connect(this, SIGNAL(NewBrushShade(double))   , this, SLOT(handleFillShade(double)));
		connect(this, SIGNAL(NewGradient(int))     , doc, SLOT(itemSelection_SetItemGradFill(int)));
		connect(this, SIGNAL(NewGradientS(int))    , doc, SLOT(itemSelection_SetItemGradStroke(int)));
		connect(this, SIGNAL(NewPattern(QString))  , doc, SLOT(itemSelection_SetItemPatternFill(QString)));
		connect(this, SIGNAL(NewPatternProps(double,double,double,double,double,double,double,bool,bool)), doc, SLOT(itemSelection_SetItemPatternProps(double,double,double,double,double,double,double,bool,bool)));
		connect(this, SIGNAL(NewOverprint(int))    , this, SLOT(handleOverprint(int)));
		connect(this, SIGNAL(NewPatternS(QString)) , doc, SLOT(itemSelection_SetItemStrokePattern(QString)));
		connect(this, SIGNAL(NewPatternTypeS(bool)), doc, SLOT(itemSelection_SetItemStrokePatternType(bool)));
		connect(this, SIGNAL(NewPatternPropsS(double,double,double,double,double,double,double,double,bool,bool)), doc, SLOT(itemSelection_SetItemStrokePatternProps(double,double,double,double,double,double,double,double,bool,bool)));
		connect(displayAllColors, SIGNAL(clicked()), this, SLOT(toggleColorDisplay()));

		connect(currentDoc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		connect(currentDoc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
		connect(currentDoc->scMW()     , SIGNAL(UpdateRequest(int)), this, SLOT(handleUpdateRequest(int)));
	}

	if (oldDoc != currentDoc)
	{
		showGradient(0);
	}
}

PageItem* ColorPalette::currentItemFromSelection()
{
	PageItem *currentItem = nullptr;
	if (currentDoc)
		currentItem = currentDoc->m_Selection->itemAt(0);
	return currentItem;
}

void ColorPalette::enablePatterns(bool enable)
{
	if (enable)
	{
		if (fillModeCombo->count() < 4)
			fillModeCombo->addItem( tr("Pattern") );
		if (strokeModeCombo->count() < 3)
			strokeModeCombo->addItem( tr("Pattern") );
	}
	else
	{
		if (fillModeCombo->count() == 4)
			fillModeCombo->removeItem(3);
		if (strokeModeCombo->count() == 3)
			strokeModeCombo->removeItem(2);
	}
}

void ColorPalette::handleSelectionChanged()
{
	if (currentDoc && !updatesBlocked())
	{
		PageItem* currentItem = currentItemFromSelection();
		setCurrentItem(currentItem);
	}
}

void ColorPalette::handleUpdateRequest(int updateFlags)
{
	if (updateFlags & reqColorsUpdate)
		updateColorList();
}

void ColorPalette::updateColorList()
{
	if (!currentDoc)
		return;
	
	if (currentItem)
		disconnectSignals();

	this->setColors(currentDoc->PageColors);
	this->setGradients(&currentDoc->docGradients);
	this->setPatterns(&currentDoc->docPatterns);

	if (currentItem)
		setCurrentItem(currentItem);
}

void ColorPalette::updateCList()
{
	bool sigBlocked1 = colorListStroke->blockSignals(true);
	bool sigBlocked2 = colorListFill->blockSignals(true);

	if (displayAllColors->isChecked())
	{
		if (currentDoc != nullptr)
			currentDoc->getUsedColors(colorList);
	}
	colorListFill->setColors(colorList, true);
	colorListStroke->setColors(colorList, true);
	gradEditStroke->setColors(colorList);
	gradEdit->setColors(colorList);
	colorPoint1->setColors(colorList, true);
	colorPoint2->setColors(colorList, true);
	colorPoint3->setColors(colorList, true);
	colorPoint4->setColors(colorList, true);
	colorMeshPoint->setColors(colorList, true);
	colorListFill->clearSelection();
	colorListStroke->clearSelection();
	hatchLineColor->setColors(colorList, false);
	hatchBackground->setColors(colorList, true);

	colorListStroke->blockSignals(sigBlocked1);
	colorListFill->blockSignals(sigBlocked2);
}

void ColorPalette::toggleColorDisplay()
{
	if (!currentDoc)
		return;
	colorListStroke->cList = &currentDoc->PageColors;
	colorListFill->cList = &currentDoc->PageColors;
	colorList = currentDoc->PageColors;
	updateColorList();
}

void ColorPalette::showOverprint(int val)
{
	bool sigBlocked = overPrintCombo->blockSignals(true);
	overPrintCombo->setCurrentIndex(val);
	overPrintCombo->blockSignals(sigBlocked);
}

void ColorPalette::handleFillShade(double val)
{
	if (!currentDoc)
		return;
	blockUpdates(true);
	currentDoc->itemSelection_SetItemBrushShade(static_cast<int>(val));
	blockUpdates(false);
}

void ColorPalette::handleStrokeShade(double val)
{
	if (!currentDoc)
		return;
	blockUpdates(true);
	currentDoc->itemSelection_SetItemPenShade(static_cast<int>(val));
	blockUpdates(false);
}

void ColorPalette::handleOverprint(int val)
{
	if (!currentDoc)
		return;
	bool setter = true;
	if (val == 0)
		setter = false;
	currentDoc->itemSelection_SetOverprint(setter);
}

void ColorPalette::handleFillGradient()
{
	if (!currentDoc)
		return;
	VGradient gradient(gradEdit->gradient());
	blockUpdates(true);
	currentDoc->updateManager()->setUpdatesDisabled();
	currentDoc->itemSelection_SetFillGradient(gradient);
	currentDoc->updateManager()->setUpdatesEnabled();
	blockUpdates(false);
}

void ColorPalette::handleStrokeGradient()
{
	if (!currentDoc)
		return;
	VGradient gradient(gradEditStroke->gradient());
	blockUpdates(true);
	currentDoc->updateManager()->setUpdatesDisabled();
	currentDoc->itemSelection_SetLineGradient(gradient);
	currentDoc->updateManager()->setUpdatesEnabled();
	blockUpdates(false);
}

void ColorPalette::handleStrokeGradientExtend(int val)
{
	if (!currentDoc)
		return;
	if (val == 0)
		currentItem->setStrokeGradientExtend(VGradient::none);
	else
		currentItem->setStrokeGradientExtend(VGradient::pad);
	currentItem->update();
	currentDoc->regionsChanged()->update(QRect());
}

void ColorPalette::handleGradientExtend(int val)
{
	if (!currentDoc)
		return;
	if (val == 0)
		currentItem->setGradientExtend(VGradient::none);
	else
		currentItem->setGradientExtend(VGradient::pad);
	currentItem->update();
	currentDoc->regionsChanged()->update(QRect());
}

void ColorPalette::showColorValues(const QString& stroke, const QString& fill, int sShade, int fShade)
{
	bool sigBlocked1 = fillShade->blockSignals(true);
	bool sigBlocked2 = strokeShade->blockSignals(true);
	bool sigBlocked3 = colorListStroke->blockSignals(true);
	bool sigBlocked4 = colorListFill->blockSignals(true);
	
	strokeShade->setValue(sShade);
	fillShade->setValue(fShade);
	if ((stroke != CommonStrings::None) && (!stroke.isEmpty()))
		colorListStroke->setCurrentColor(stroke);
	else
		colorListStroke->setCurrentRow(0);
	if ((fill != CommonStrings::None) && (!fill.isEmpty()))
		colorListFill->setCurrentColor(fill);
	else
		colorListFill->setCurrentRow(0);

	fillShade->blockSignals(sigBlocked1);
	strokeShade->blockSignals(sigBlocked2);
	colorListStroke->blockSignals(sigBlocked3);
	colorListFill->blockSignals(sigBlocked4);
}

void ColorPalette::selectColorS(int row)
{
	QString colorName;
	QVariant varValue = colorListStroke->data(row, Qt::UserRole);
	if (varValue.canConvert<ColorPixmapValue>())
	{
		ColorPixmapValue value(varValue.value<ColorPixmapValue>());
		colorName = value.m_name;
	}
	else
	{
		colorName = varValue.toString();
	}
	if (colorName.isEmpty())
		return;
	emit NewPen(colorName);
}

void ColorPalette::selectColorF(int row)
{
	QString colorName;
	QVariant varValue = colorListFill->data(row, Qt::UserRole);
	if (varValue.canConvert<ColorPixmapValue>())
	{
		ColorPixmapValue value(varValue.value<ColorPixmapValue>());
		colorName = value.m_name;
	}
	else
	{
		colorName = varValue.toString();
	}
	if (colorName.isEmpty())
		return;
	emit NewBrush(colorName);
}

void ColorPalette::setColors(const ColorList& newColorList)
{
	colorList.clear();
	colorList = newColorList;
	updateCList();
}

void ColorPalette::fillStrokeSelector(int /*index*/)
{
	if (gradEditButton->isChecked() || editMeshColors->isChecked())
	{
		editStrokeGradient = 0;
		CGradDia->hide();
		editMeshColors->setEnabled(true);
		editMeshColors->setChecked(false);
		gradEditButton->setEnabled(true);
		gradEditButton->setChecked(false);
		emit editGradient(editStrokeGradient);
	}
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

void ColorPalette::updateGradientList()
{
	bool sigBlocked1 = namedGradient->blockSignals(true);
	bool sigBlocked2 = namedGradientStroke->blockSignals(true);

	namedGradient->clear();
	namedGradient->setIconSize(QSize(48, 12));
	namedGradient->addItem( tr("Custom"));
	namedGradientStroke->clear();
	namedGradientStroke->setIconSize(QSize(48, 12));
	namedGradientStroke->addItem( tr("Custom"));
	QStringList patK = gradientList->keys();
	patK.sort();
	for (int i = 0; i < patK.count(); i++)
	{
		VGradient gr = gradientList->value(patK[i]);
		QImage pixm(48, 12, QImage::Format_ARGB32_Premultiplied);
		QPainter pb;
		QBrush b(QColor(205,205,205), IconManager::instance().loadPixmap("testfill.png"));
		pb.begin(&pixm);
		pb.fillRect(0, 0, 48, 12, b);
		pb.end();
		ScPainter *p = new ScPainter(&pixm, 48, 12);
		p->setPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->setFillMode(2);
		p->fill_gradient = gr;
		p->setGradient(VGradient::linear, FPoint(0,6), FPoint(48, 6), FPoint(0,0), 1, 0);
		p->drawRect(0, 0, 48, 12);
		p->end();
		delete p;
		QPixmap pm;
		pm = QPixmap::fromImage(pixm);
		namedGradient->addItem(pm, patK[i]);
		namedGradientStroke->addItem(pm, patK[i]);
	}
	namedGradient->blockSignals(sigBlocked1);
	namedGradientStroke->blockSignals(sigBlocked2);
}

void ColorPalette::setGradients(QHash<QString, VGradient> *docGradients)
{
	gradientList = docGradients;
	updateGradientList();
}

void ColorPalette::setGradientColors()
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
	UndoTransaction trans;
	if (UndoManager::undoEnabled())
		trans = undoManager->beginTransaction(Um::Selection,Um::IFill,Um::GradVal,"",Um::IFill);
	currentItem->set4ColorShade(static_cast<int>(color1Shade->value()), static_cast<int>(color2Shade->value()), static_cast<int>(color3Shade->value()), static_cast<int>(color4Shade->value()));
	currentItem->set4ColorTransparency(t1, t2, t3, t4);
	currentItem->set4ColorColors(color1, color2, color3, color4);
	if (trans)
		trans.commit();
	currentItem->update();
}

void ColorPalette::setNamedGradient(const QString &name)
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
		emit NewGradient(13);
	else if (gradientType->currentIndex() == 3)
	{
		setGradientColors();
		emit NewGradient(9);
	}
	else if (gradientType->currentIndex() == 4)
		emit NewGradient(10);
	else if (gradientType->currentIndex() == 5)
		emit NewGradient(11);
}

void ColorPalette::setNamedGradientStroke(const QString &name)
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

void ColorPalette::updatePatternList()
{
	disconnect(patternBox, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPattern(QListWidgetItem*)));
	patternBox->clear();
	patternBox->setIconSize(QSize(48, 48));
	patternBoxStroke->clear();
	patternBoxStroke->setIconSize(QSize(48, 48));
	QStringList patK = patternList->keys();
	patK.sort();
	for (int a = 0; a < patK.count(); a++)
	{
		ScPattern sp = patternList->value(patK[a]);
		QPixmap pm;
		if (sp.getPattern()->width() >= sp.getPattern()->height())
			pm=QPixmap::fromImage(sp.getPattern()->scaledToWidth(48, Qt::SmoothTransformation));
		else
			pm=QPixmap::fromImage(sp.getPattern()->scaledToHeight(48, Qt::SmoothTransformation));
		QPixmap pm2(48, 48);
		pm2.fill(palette().color(QPalette::Base));
		QPainter p;
		p.begin(&pm2);
		p.drawPixmap(24 - pm.width() / 2, 24 - pm.height() / 2, pm);
		p.end();
		QListWidgetItem *item = new QListWidgetItem(pm2, patK[a], patternBox);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		QListWidgetItem *itemS = new QListWidgetItem(pm2, patK[a], patternBoxStroke);
		itemS->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	}
	patternBox->clearSelection();
	patternBoxStroke->clearSelection();
	connect(patternBox, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPattern(QListWidgetItem*)));
}

void ColorPalette::hideEditedPatterns(QStringList names)
{
	for (int i = 0; i < names.count(); i++)
	{
		QList<QListWidgetItem*> items = patternBox->findItems(names[i], Qt::MatchExactly);
		if (items.count() > 0)
			items[0]->setHidden(true);
		items = patternBoxStroke->findItems(names[i], Qt::MatchExactly);
		if (items.count() > 0)
			items[0]->setHidden(true);
	}
}

void ColorPalette::setPatterns(QHash<QString, ScPattern> *docPatterns)
{
	patternList = docPatterns;
	updatePatternList();
}

void ColorPalette::selectPattern(QListWidgetItem *c)
{
	if (c == nullptr)
		return;
	emit NewPattern(c->text());
}

void ColorPalette::selectPatternS(QListWidgetItem *c)
{
	if (c == nullptr)
		return;
	emit NewPatternS(c->text());
}

void ColorPalette::setActPatternStroke(const QString& pattern, double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY, double space, bool pathF)
{
	bool sigBlocked = patternBoxStroke->blockSignals(true);
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
	patternBoxStroke->blockSignals(sigBlocked);
}

void ColorPalette::setActPattern(const QString& pattern, double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY)
{
	bool sigBlocked = patternBox->blockSignals(true);
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
	patternBox->blockSignals(sigBlocked);
}

void ColorPalette::slotGradStroke(int number)
{
	if (number == 1)
	{
		bool sigBlocked = namedGradientStroke->blockSignals(true);
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
		namedGradientStroke->blockSignals(sigBlocked);
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

void ColorPalette::showGradient(int number)
{
	bool sigBlocked = fillModeCombo->blockSignals(true);
	if (number==-1)
	{
		fillModeCombo->setCurrentIndex(0);
	}
	if (number == 0)
		fillModeCombo->setCurrentIndex(0);
	else if (((number > 0) && (number < 8)) || ((number >= 9) && (number <= 13)))
	{
		if ((number == 5) || (number == 7) || (number == 13))
		{
			stackedWidget_2->setCurrentIndex(0);
			if ((number == 5) || (number == 7))
				gradientType->setCurrentIndex(1);
			if (number == 13)
				gradientType->setCurrentIndex(2);
		}
		else if (number == 9)
		{
			stackedWidget_2->setCurrentIndex(1);
			gradientType->setCurrentIndex(3);
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
			gradientType->setCurrentIndex(4);
		}
		else if (number == 11)
		{
			stackedWidget_2->setCurrentIndex(2);
			if ((currentItem->selectedMeshPointX > -1) && (currentItem->selectedMeshPointY > -1l))
			{
				MeshPoint mp = currentItem->meshGradientArray[currentItem->selectedMeshPointX][currentItem->selectedMeshPointY];
				setCurrentComboItem(colorMeshPoint, mp.colorName);
				shadeMeshPoint->setValue(mp.shade);
				transparencyMeshPoint->setValue(mp.transparency * 100);
			}
			gradientType->setCurrentIndex(5);
		}
		else if (number == 12)
		{
			stackedWidget_2->setCurrentIndex(2);
			gradientType->setCurrentIndex(6);
		}
		else
		{
			stackedWidget_2->setCurrentIndex(0);
			gradientType->setCurrentIndex(0);
		}
		fillModeCombo->setCurrentIndex(1);
		if (currentItem->getGradientExtend() == VGradient::none)
			gradientExtend->setCurrentIndex(0);
		else
			gradientExtend->setCurrentIndex(1);
	}
	else if (number == 14)
	{
		fillModeCombo->setCurrentIndex(2);
		hatchAngle->setValue(currentItem->hatchAngle);
		hatchDist->setValue(currentItem->hatchDistance * unitGetRatioFromIndex(currentUnit));
		hatchType->setCurrentIndex(currentItem->hatchType);
		if ((currentItem->hatchForeground != CommonStrings::None) && (!currentItem->hatchForeground.isEmpty()))
			setCurrentComboItem(hatchLineColor, currentItem->hatchForeground);
		if (currentItem->hatchUseBackground)
		{
			if (!currentItem->hatchBackground.isEmpty())
				setCurrentComboItem(hatchBackground, currentItem->hatchBackground);
		}
		else
			setCurrentComboItem(hatchBackground, CommonStrings::None);
	}
	else
	{
		if (patternList->count() == 0)
		{
			fillModeCombo->setCurrentIndex(0);
			emit NewGradient(0);
		}
		else
			fillModeCombo->setCurrentIndex(3);
	}
	fillModeStack->setCurrentIndex( fillModeCombo->currentIndex() );
	fillModeCombo->blockSignals(sigBlocked);
}

void ColorPalette::showGradientStroke(int number)
{
	bool sigBlocked = strokeModeCombo->blockSignals(true);
	if (number==-1 || number == 0)
		strokeModeCombo->setCurrentIndex(0);
	else if (number > 0 && number < 8)
	{
		if (number == 7)
			gradientTypeStroke->setCurrentIndex(1);
		else
			gradientTypeStroke->setCurrentIndex(0);
		strokeModeCombo->setCurrentIndex(1);
		if (currentItem->getStrokeGradientExtend() == VGradient::none)
			GradientExtendS->setCurrentIndex(0);
		else
			GradientExtendS->setCurrentIndex(1);
	}
	else
	{
		if (patternList->count() == 0)
		{
			strokeModeCombo->setCurrentIndex(0);
			emit NewGradient(0);
		}
		else
			strokeModeCombo->setCurrentIndex(2);
	}
	strokeModeStack->setCurrentIndex( strokeModeCombo->currentIndex() );
	strokeModeCombo->blockSignals(sigBlocked);
}

void ColorPalette::slotGrad(int number)
{
	if (number == 1)
	{
		bool sigBlocked1 = gradEdit->blockSignals(true);
		bool sigBlocked2 = gradientType->blockSignals(true);
		bool sigBlocked3 = namedGradient->blockSignals(true);
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
			stackedWidget_2->setCurrentIndex(0);
			emit NewGradient(13);
		}
		else if (gradientType->currentIndex() == 3)
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
		else if (gradientType->currentIndex() == 4)
		{
			stackedWidget_2->setCurrentIndex(0);
			emit NewGradient(10);
		}
		else if (gradientType->currentIndex() == 5)
		{
			stackedWidget_2->setCurrentIndex(2);
			if ((currentItem->selectedMeshPointX > -1) && (currentItem->selectedMeshPointY > -1l))
			{
				MeshPoint mp = currentItem->meshGradientArray[currentItem->selectedMeshPointX][currentItem->selectedMeshPointY];
				setCurrentComboItem(colorMeshPoint, mp.colorName);
				shadeMeshPoint->setValue(mp.shade);
				transparencyMeshPoint->setValue(mp.transparency * 100);
			}
			emit NewGradient(11);
		}
		else if (gradientType->currentIndex() == 6)
		{
			stackedWidget_2->setCurrentIndex(2);
			emit NewGradient(12);
		}
		gradEdit->blockSignals(sigBlocked1);
		gradientType->blockSignals(sigBlocked2);
		namedGradient->blockSignals(sigBlocked3);
	}
	else if (number == 2)
	{
		hatchAngle->setValue(currentItem->hatchAngle);
		hatchDist->setValue(currentItem->hatchDistance * unitGetRatioFromIndex(currentUnit));
		hatchType->setCurrentIndex(currentItem->hatchType);
		if ((currentItem->hatchForeground != CommonStrings::None) && (!currentItem->hatchForeground.isEmpty()))
			setCurrentComboItem(hatchLineColor, currentItem->hatchForeground);
		if (currentItem->hatchUseBackground)
		{
			if (!currentItem->hatchBackground.isEmpty())
				setCurrentComboItem(hatchBackground, currentItem->hatchBackground);
		}
		else
			setCurrentComboItem(hatchBackground, CommonStrings::None);
		emit NewGradient(14);
	}
	else if (number == 3)
		emit NewGradient(8);
	else
		emit NewGradient(0);
	fillModeStack->setCurrentIndex(number);
}

void ColorPalette::slotGradType(int type)
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
		stackedWidget_2->setCurrentIndex(0);
		emit NewGradient(13);
	}
	else if (type == 3)
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
	else if (type == 4)
	{
		stackedWidget_2->setCurrentIndex(0);
		emit NewGradient(10);
	}
	else if (type == 5)
	{
		stackedWidget_2->setCurrentIndex(2);
		if ((currentItem->selectedMeshPointX > -1) && (currentItem->selectedMeshPointY > -1l))
		{
			MeshPoint mp = currentItem->meshGradientArray[currentItem->selectedMeshPointX][currentItem->selectedMeshPointY];
			setCurrentComboItem(colorMeshPoint, mp.colorName);
			shadeMeshPoint->setValue(mp.shade);
			transparencyMeshPoint->setValue(mp.transparency * 100);
		}
		emit NewGradient(11);
	}
	else if (type == 6)
	{
		stackedWidget_2->setCurrentIndex(2);
		emit NewGradient(12);
	}
}

void ColorPalette::slotGradTypeStroke(int type)
{
	if (type == 0)
		emit NewGradientS(6);
	else
		emit NewGradientS(7);
}

void ColorPalette::editMeshPointColor()
{
	if (editMeshColors->isChecked())
	{
		if (currentItem->gradientType() == 11)
			editStrokeGradient = 6;
		else if (currentItem->gradientType() == 12)
			editStrokeGradient = 8;
		else
			editStrokeGradient = 0;
		gradEditButton->setEnabled(false);
	}
	else
	{
		editStrokeGradient = 0;
		gradEditButton->setEnabled(true);
	}
	fillModeCombo->setEnabled(!gradEditButton->isChecked() && !editMeshColors->isChecked());
	gradientType->setEnabled(!gradEditButton->isChecked() && !editMeshColors->isChecked());
	strokeModeCombo->setEnabled(!gradEditButtonStroke->isChecked() && !editMeshColors->isChecked());
	gradientTypeStroke->setEnabled(!gradEditButtonStroke->isChecked() && !editMeshColors->isChecked());
	emit editGradient(editStrokeGradient);
}

void ColorPalette::createNewMeshGradient()
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

void ColorPalette::resetMeshGradient()
{
	currentItem->resetGradientMesh();
	currentItem->update();
	currentDoc->regionsChanged()->update(QRect());
}

void ColorPalette::meshGradientToShape()
{
	currentItem->meshToShape();
	currentItem->update();
	currentDoc->regionsChanged()->update(QRect());
}

void ColorPalette::resetOneControlPoint()
{
	int grow = currentItem->selectedMeshPointX;
	int gcol = currentItem->selectedMeshPointY;
	int cont = currentItem->selectedMeshControlPoint;
	MeshPoint tmp;
	if (currentItem->gradientType() == 12)
	{
		if ((grow == -1) || (gcol == 0))
			return;
		ScItemState<QPair<MeshPoint,MeshPoint> > *ss = nullptr;
		if (UndoManager::undoEnabled())
		{
			ss = new ScItemState<QPair<MeshPoint,MeshPoint> >(Um::GradPos);
			ss->set("MOVE_MESH_PATCH");
			ss->set("ARRAY", false);
			ss->set("X", grow);
			ss->set("Y", gcol);
		}
		switch (gcol)
		{
			case 1:
				tmp = currentItem->meshGradientPatches[grow].TL;
				if (cont == 2)
					tmp.controlBottom = tmp.gridPoint;
				if (cont == 4)
					tmp.controlRight  = tmp.gridPoint;
				if (UndoManager::undoEnabled())
					ss->setItem(qMakePair(currentItem->meshGradientPatches[grow].TL,tmp));
				currentItem->meshGradientPatches[grow].TL = tmp;
				break;
			case 2:
				tmp = currentItem->meshGradientPatches[grow].TR;
				if (cont == 2)
					tmp.controlBottom = tmp.gridPoint;
				if (cont == 3)
					tmp.controlLeft   = tmp.gridPoint;
				if (UndoManager::undoEnabled())
					ss->setItem(qMakePair(currentItem->meshGradientPatches[grow].TR,tmp));
				currentItem->meshGradientPatches[grow].TR = tmp;
				break;
			case 3:
				tmp = currentItem->meshGradientPatches[grow].BR;
				if (cont == 1)
					tmp.controlTop  = tmp.gridPoint;
				if (cont == 3)
					tmp.controlLeft = tmp.gridPoint;
				if (UndoManager::undoEnabled())
					ss->setItem(qMakePair(currentItem->meshGradientPatches[grow].BR,tmp));
				currentItem->meshGradientPatches[grow].BR = tmp;
				break;
			case 4:
				tmp = currentItem->meshGradientPatches[grow].BL;
				if (cont == 1)
					tmp.controlTop   = tmp.gridPoint;
				if (cont == 4)
					tmp.controlRight = tmp.gridPoint;
				if (UndoManager::undoEnabled())
					ss->setItem(qMakePair(currentItem->meshGradientPatches[grow].BL,tmp));
				currentItem->meshGradientPatches[grow].BL = tmp;
				break;
		}
		if (UndoManager::undoEnabled())
			undoManager->action(currentItem,ss);
	}
	else
	{
		if ((grow == -1) || (gcol == -1))
			return;

		tmp = currentItem->meshGradientArray[grow][gcol];
		if (cont == 1)
			tmp.controlTop    = tmp.gridPoint;
		else if (cont == 2)
			tmp.controlBottom = tmp.gridPoint;
		else if (cont == 3)
			tmp.controlLeft   = tmp.gridPoint;
		else if (cont == 4)
			tmp.controlRight  = tmp.gridPoint;

		if (UndoManager::undoEnabled())
		{
			ScItemState<QPair<MeshPoint,MeshPoint> > *ss = new ScItemState<QPair<MeshPoint,MeshPoint> >(Um::GradPos);
			ss->set("MOVE_MESH_PATCH");
			ss->set("ARRAY", true);
			ss->set("X", grow);
			ss->set("Y", gcol);
			ss->setItem(qMakePair(currentItem->meshGradientArray[grow][gcol],tmp));
			undoManager->action(currentItem,ss);
		}
		currentItem->meshGradientArray[grow][gcol] = tmp;
	}
	currentItem->update();
	currentDoc->regionsChanged()->update(QRect());
}

void ColorPalette::resetAllControlPoints()
{
	int grow = currentItem->selectedMeshPointX;
	int gcol = currentItem->selectedMeshPointY;
	MeshPoint tmp;
	if (currentItem->gradientType() == 12)
	{
		if ((grow == -1) || (gcol == 0))
			return;
		ScItemState<QPair<MeshPoint,MeshPoint> > *ss = nullptr;
		if (UndoManager::undoEnabled())
		{
			ss = new ScItemState<QPair<MeshPoint,MeshPoint> >(Um::GradPos);
			ss->set("MOVE_MESH_PATCH");
			ss->set("ARRAY", false);
			ss->set("X", grow);
			ss->set("Y", gcol);
		}
		switch (gcol)
		{
			case 1:
				tmp = currentItem->meshGradientPatches[grow].TL;
				tmp.controlTop    = tmp.gridPoint;
				tmp.controlLeft   = tmp.gridPoint;
				tmp.controlRight  = tmp.gridPoint;
				tmp.controlBottom = tmp.gridPoint;
				if (UndoManager::undoEnabled())
					ss->setItem(qMakePair(currentItem->meshGradientPatches[grow].TL,tmp));
				currentItem->meshGradientPatches[grow].TL = tmp;
		break;
			case 2:
				tmp = currentItem->meshGradientPatches[grow].TR;
				tmp.controlTop    = tmp.gridPoint;
				tmp.controlLeft   = tmp.gridPoint;
				tmp.controlRight  = tmp.gridPoint;
				tmp.controlBottom = tmp.gridPoint;
				if (UndoManager::undoEnabled())
					ss->setItem(qMakePair(currentItem->meshGradientPatches[grow].TR,tmp));
				currentItem->meshGradientPatches[grow].TR = tmp;
		break;
			case 3:
				tmp = currentItem->meshGradientPatches[grow].BR;
				tmp.controlTop    = tmp.gridPoint;
				tmp.controlLeft   = tmp.gridPoint;
				tmp.controlRight  = tmp.gridPoint;
				tmp.controlBottom = tmp.gridPoint;
				if (UndoManager::undoEnabled())
					ss->setItem(qMakePair(currentItem->meshGradientPatches[grow].BR,tmp));
				currentItem->meshGradientPatches[grow].BR = tmp;
		break;
			case 4:
				tmp = currentItem->meshGradientPatches[grow].BL;
				tmp.controlTop    = tmp.gridPoint;
				tmp.controlLeft   = tmp.gridPoint;
				tmp.controlRight  = tmp.gridPoint;
				tmp.controlBottom = tmp.gridPoint;
				if (UndoManager::undoEnabled())
					ss->setItem(qMakePair(currentItem->meshGradientPatches[grow].BL,tmp));
				currentItem->meshGradientPatches[grow].BL = tmp;
		break;
		}
		if (UndoManager::undoEnabled())
			undoManager->action(currentItem,ss);
	}
	else
	{
		if ((grow == -1) || (gcol == -1))
			return;
		tmp = currentItem->meshGradientArray[grow][gcol];
		tmp.controlTop    = tmp.gridPoint;
		tmp.controlLeft   = tmp.gridPoint;
		tmp.controlRight  = tmp.gridPoint;
		tmp.controlBottom = tmp.gridPoint;
		if (UndoManager::undoEnabled())
		{
			ScItemState<QPair<MeshPoint,MeshPoint> > *ss = new ScItemState<QPair<MeshPoint,MeshPoint> >(Um::GradPos);
			ss->set("MOVE_MESH_PATCH");
			ss->set("ARRAY", true);
			ss->set("X", grow);
			ss->set("Y", gcol);
			ss->setItem(qMakePair(currentItem->meshGradientArray[grow][gcol],tmp));
			undoManager->action(currentItem,ss);
		}
		currentItem->meshGradientArray[grow][gcol] = tmp;
	}
	currentItem->update();
	currentDoc->regionsChanged()->update(QRect());
}

void ColorPalette::editGradientVector()
{
	if (gradEditButton->isChecked())
	{
		setGradientVectorValues();
		CGradDia->show();
	}
	else
	{
		CGradDia->hide();
		editMeshColors->setEnabled(true);
	}
	if (currentItem->gradientType() == 9)
		editStrokeGradient = 3;
	else if (currentItem->gradientType() == 10)
		editStrokeGradient = 4;
	else if (currentItem->gradientType() == 11)
		editStrokeGradient = 5;
	else if (currentItem->gradientType() == 12)
		editStrokeGradient = 9;
	else
		editStrokeGradient = 0;
	fillModeCombo->setEnabled(!gradEditButton->isChecked() && !editMeshColors->isChecked());
	gradientType->setEnabled(!gradEditButton->isChecked() && !editMeshColors->isChecked());
	emit editGradient(editStrokeGradient);
}

void ColorPalette::editGradientVectorStroke()
{
	if (gradEditButtonStroke->isChecked())
	{
		setGradientVectorStrokeValues();
		CGradDia->show();
	}
	else
	{
		CGradDia->hide();
	}
	editStrokeGradient = 1;
	strokeModeCombo->setEnabled(!gradEditButtonStroke->isChecked() && !editMeshColors->isChecked());
	gradientTypeStroke->setEnabled(!gradEditButtonStroke->isChecked() && !editMeshColors->isChecked());
	emit editGradient(editStrokeGradient);
}

void ColorPalette::setGradientVectorValues()
{
	if (!gradEditButton->isChecked())
		return;
	CGradDia->unitChange(currentDoc->unitIndex());
	CGradDia->setValues(currentItem->gradientStartX(), currentItem->gradientStartY(), currentItem->gradientEndX(), currentItem->gradientEndY(), currentItem->GrFocalX, currentItem->GrFocalY, currentItem->GrScale, currentItem->GrSkew, 0, 0);
	if (currentItem->gradientType() == 6)
		CGradDia->selectLinear();
	else if (currentItem->gradientType() == 7)
		CGradDia->selectRadial();
	else if (currentItem->gradientType() == 9)
	{
		CGradDia->setValues(currentItem->GrControl1.x(), currentItem->GrControl1.y(), currentItem->GrControl2.x(), currentItem->GrControl2.y(), currentItem->GrControl3.x(), currentItem->GrControl3.y(), currentItem->GrControl4.x(), currentItem->GrControl4.y(), 0, 0);
		CGradDia->selectFourColor();
	}
	else if (currentItem->gradientType() == 10)
	{
		CGradDia->setValues(currentItem->GrControl1.x(), currentItem->GrControl1.y(), currentItem->GrControl2.x(), currentItem->GrControl2.y(), currentItem->GrControl3.x(), currentItem->GrControl3.y(), currentItem->GrControl4.x(), currentItem->GrControl4.y(), currentItem->GrControl5.x(), currentItem->GrControl5.y());
		CGradDia->selectDiamond();
	}
	else if (currentItem->gradientType() == 11)
	{
		CGradDia->selectMesh();
		editMeshColors->setEnabled(false);
	}
	else if (currentItem->gradientType() == 12)
	{
		CGradDia->selectPatchMesh();
		editMeshColors->setEnabled(false);
	}
	else if (currentItem->gradientType() == 13)
		CGradDia->selectConical();
}

void ColorPalette::setGradientVectorStrokeValues()
{
	if (!gradEditButtonStroke->isChecked())
		return;
	CGradDia->unitChange(currentDoc->unitIndex());
	CGradDia->setValues(currentItem->GrStrokeStartX, currentItem->GrStrokeStartY, currentItem->GrStrokeEndX, currentItem->GrStrokeEndY, currentItem->GrStrokeFocalX, currentItem->GrStrokeFocalY, currentItem->GrStrokeScale, currentItem->GrStrokeSkew, 0, 0);
	if (currentItem->strokeGradientType() == 6)
		CGradDia->selectLinear();
	else
		CGradDia->selectRadial();
}

void ColorPalette::setActiveGradDia(bool active)
{
	if (active)
		return;
	if (editStrokeGradient == 1)
		gradEditButtonStroke->setChecked(false);
	else
		gradEditButton->setChecked(false);
	emit editGradient(editStrokeGradient);
	fillModeCombo->setEnabled(!gradEditButton->isChecked() && !editMeshColors->isChecked());
	gradientType->setEnabled(!gradEditButton->isChecked() && !editMeshColors->isChecked());
	strokeModeCombo->setEnabled(!gradEditButtonStroke->isChecked() && !editMeshColors->isChecked());
	gradientTypeStroke->setEnabled(!gradEditButtonStroke->isChecked() && !editMeshColors->isChecked());
	editMeshColors->setEnabled(true);
}

void ColorPalette::setSpecialGradient(double x1, double y1, double x2, double y2, double fx, double fy, double sg, double sk, double cx, double cy)
{
	if (CGradDia)
		CGradDia->setValues(x1, y1, x2, y2, fx, fy, sg, sk, cx, cy);
}

void ColorPalette::setMeshPoint()
{
	if ((currentItem->selectedMeshPointX > -1) && (currentItem->selectedMeshPointY > -1))
	{
		colorMeshPoint->setEnabled(true);
		shadeMeshPoint->setEnabled(true);
		transparencyMeshPoint->setEnabled(true);
		MeshPoint mp = currentItem->meshGradientArray[currentItem->selectedMeshPointX][currentItem->selectedMeshPointY];
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

void ColorPalette::endPatchAdd()
{
	CGradDia->endPAddButton();
}

void ColorPalette::snapToPatchGrid(bool val)
{
	currentItem->setSnapToPatchGrid(val);
}

void ColorPalette::handleRemovePatch()
{
	if ((currentItem->selectedMeshPointX > -1) && (currentItem->meshGradientPatches.count() > 1))
	{
		if (UndoManager::undoEnabled())
		{
			ScItemState<meshGradientPatch> *ss = new ScItemState<meshGradientPatch>(Um::RemoveMeshPatch,"",Um::ILine);
			ss->set("REMOVE_MESH_PATCH");
			ss->setItem(currentItem->meshGradientPatches.takeAt(currentItem->selectedMeshPointX));
			ss->set("POS", currentItem->selectedMeshPointX);
			undoManager->action(currentItem,ss);
		}
		currentItem->selectedMeshPointX = -1;
		CGradDia->changebuttonRemovePatch((currentItem->selectedMeshPointX > -1) && (currentItem->meshGradientPatches.count() > 1));
		currentItem->update();
		currentDoc->regionsChanged()->update(QRect());
		editStrokeGradient = 9;
		emit editGradient(editStrokeGradient);
	}
}

void ColorPalette::updateMeshPoint()
{
	QString color = colorMeshPoint->currentText();
	if (color == CommonStrings::tr_NoneColor)
		color = CommonStrings::None;
	double t = transparencyMeshPoint->value() / 100.0;
	currentItem->setMeshPointColor(currentItem->selectedMeshPointX, currentItem->selectedMeshPointY, color, static_cast<int>(shadeMeshPoint->value()), t, currentDoc->view()->editStrokeGradient == 8);
	currentItem->update();
	currentDoc->regionsChanged()->update(QRect());
}

void ColorPalette::setMeshPatchPoint()
{
	if ((currentItem->selectedMeshPointX > -1) && (currentItem->selectedMeshPointY > 0))
	{
		colorMeshPoint->setEnabled(true);
		shadeMeshPoint->setEnabled(true);
		transparencyMeshPoint->setEnabled(true);
		meshGradientPatch patch = currentItem->meshGradientPatches[currentItem->selectedMeshPointX];
		MeshPoint mp;
		switch (currentItem->selectedMeshPointY)
		{
			case 1:
				mp = patch.TL;
				break;
			case 2:
				mp = patch.TR;
				break;
			case 3:
				mp = patch.BR;
				break;
			case 4:
				mp = patch.BL;
				break;
		}
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

void ColorPalette::setMeshPatch()
{
	CGradDia->changebuttonRemovePatch((currentItem->selectedMeshPointX > -1) && (currentItem->meshGradientPatches.count() > 1));
}

void ColorPalette::changePatternProps()
{
	PatternPropsDialog *dia = new PatternPropsDialog(this, currentUnit, false);
	dia->spinXscaling->setValue(m_Pattern_scaleX);
	dia->spinYscaling->setValue(m_Pattern_scaleY);
	if (m_Pattern_scaleX == m_Pattern_scaleY)
		dia->keepScaleRatio->setChecked(true);
	dia->spinXoffset->setValue(m_Pattern_offsetX);
	dia->spinYoffset->setValue(m_Pattern_offsetY);
	dia->spinAngle->setValue(m_Pattern_rotation);
	double asina = atan(m_Pattern_skewX);
	dia->spinXSkew->setValue(asina / (M_PI / 180.0));
	double asinb = atan(m_Pattern_skewY);
	dia->spinYSkew->setValue(asinb / (M_PI / 180.0));
	dia->FlipH->setChecked(m_Pattern_mirrorX);
	dia->FlipV->setChecked(m_Pattern_mirrorY);
	connect(dia, SIGNAL(NewPatternProps(double,double,double,double,double,double,double,bool,bool)), this, SIGNAL(NewPatternProps(double,double,double,double,double,double,double,bool,bool)));
	dia->exec();
	m_Pattern_scaleX = dia->spinXscaling->value();
	m_Pattern_scaleY = dia->spinYscaling->value();
	m_Pattern_offsetX = dia->spinXoffset->value();
	m_Pattern_offsetY = dia->spinYoffset->value();
	m_Pattern_rotation = dia->spinAngle->value();
	double skewX = dia->spinXSkew->value();
	double a;
	if (skewX == 90.0)
		a = 1.0;
	else if (skewX == 180.0)
		a = 0.0;
	else if (skewX == 270.0)
		a = -1.0;
	else if (skewX == 360.0)
		a = 0.0;
	else
		a = tan(M_PI / 180.0 * skewX);
	m_Pattern_skewX = tan(a);
	skewX = dia->spinYSkew->value();
	if (skewX == 90.0)
		a = 1.0;
	else if (skewX == 180.0)
		a = 0.0;
	else if (skewX == 270.0)
		a = -1.0;
	else if (skewX == 360.0)
		a = 0.0;
	else
		a = tan(M_PI / 180.0 * skewX);
	m_Pattern_skewY = tan(a);
	m_Pattern_mirrorX = dia->FlipH->isChecked();
	m_Pattern_mirrorY = dia->FlipV->isChecked();
	delete dia;
	fillModeCombo->setCurrentIndex(3);
	emit NewGradient(8);
}

void ColorPalette::changePatternPropsStroke()
{
	PatternPropsDialog *dia = new PatternPropsDialog(this, currentUnit, true);
	dia->spinXscaling->setValue(m_Pattern_scaleXS);
	dia->spinYscaling->setValue(m_Pattern_scaleYS);
	if (m_Pattern_scaleXS == m_Pattern_scaleYS)
		dia->keepScaleRatio->setChecked(true);
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
	connect(dia, SIGNAL(NewPatternPropsS(double,double,double,double,double,double,double,double,bool,bool)), this, SIGNAL(NewPatternPropsS(double,double,double,double,double,double,double,double,bool,bool)));
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

void ColorPalette::changeHatchProps()
{
	QString color1 = hatchLineColor->currentText();
	if (color1 == CommonStrings::tr_NoneColor)
		color1 = CommonStrings::None;
	QString color2 = hatchBackground->currentText();
	if (color2 == CommonStrings::tr_NoneColor)
		color2 = CommonStrings::None;
	bool useB = (color2 != CommonStrings::None);
	double angle = hatchAngle->value();
	double dist = hatchDist->value() / unitGetRatioFromIndex(currentUnit);
	currentItem->setHatchParameters(hatchType->currentIndex(), dist, angle, useB, color2, color1);
	currentItem->update();
	currentDoc->regionsChanged()->update(QRect());
}

void ColorPalette::toggleStrokePattern()
{
	emit NewPatternTypeS(followsPath->isChecked());
}

void ColorPalette::unitChange(double, double, int unitIndex)
{
	if (CGradDia)
		CGradDia->unitChange(unitIndex);
	hatchDist->setNewUnit(unitIndex);
	currentUnit = unitIndex;
}

void ColorPalette::languageChange()
{
	// Needed to avoid issues if an item is selected and patterns are available
	if (currentItem)
		disconnectSignals();

	// Save fill tab state
	int oldFillModeComboIndex = fillModeCombo->currentIndex();
	int oldGradientTypeIndex = gradientType->currentIndex();
	int oldGradientExtendIndex = gradientExtend->currentIndex();
	int oldHatchTypeIndex = hatchType->currentIndex();

	// Save stroke tab state
	int oldStrokeModeComboIndex = strokeModeCombo->currentIndex();
	int oldGradientTypeStrokeIndex = gradientTypeStroke->currentIndex();
	int oldGradientExtendSIndex = GradientExtendS->currentIndex();

	// Save properties outside of tabs
	int oldOverPrintComboIndex = overPrintCombo->currentIndex();

	// Retranslate UI
	retranslateUi(this);

	fillModeCombo->clear();
	fillModeCombo->addItem( tr("Solid"));
	fillModeCombo->addItem( tr("Gradient"));
	fillModeCombo->addItem( tr("Hatch"));

	strokeModeCombo->clear();
	strokeModeCombo->addItem( tr("Solid"));
	strokeModeCombo->addItem( tr("Gradient"));

	if (currentDoc)
		enablePatterns(patternList->count() != 0);

	// Restore properties
	fillModeCombo->setCurrentIndex(oldFillModeComboIndex);
	gradientType->setCurrentIndex(oldGradientTypeIndex);
	gradientExtend->setCurrentIndex(oldGradientExtendIndex);
	hatchType->setCurrentIndex(oldHatchTypeIndex);

	strokeModeCombo->setCurrentIndex(oldStrokeModeComboIndex);
	gradientTypeStroke->setCurrentIndex(oldGradientTypeStrokeIndex);
	GradientExtendS->setCurrentIndex(oldGradientExtendSIndex);

	overPrintCombo->setCurrentIndex(oldOverPrintComboIndex);

	// Reconnect signals if necessary
	if (currentItem)
		connectSignals();
}
