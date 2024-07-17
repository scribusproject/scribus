/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
For general Scribus copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#include "colorpicker_gradient_properties.h"
#include "units.h"
#include "iconmanager.h"
#include "scribusapp.h"

ColorPickerGradientProperties::ColorPickerGradientProperties(QWidget* parent) : StackedContainer(parent)
{
	m_unitRatio = 1.0;

	setupUi(this);

	gSk->setNewUnit(6);
	gSk->setValues(-89, 89, 2, 0);
	gSk_2->setNewUnit(6);
	gSk_2->setValues(-89, 89, 2, 0);
	gSc->setSuffix(" %");
	gSc->setValue( 100 );

	connectSlots();

	// Mesh
	connect(buttonNewGrid, SIGNAL(clicked()), this, SIGNAL(createNewMesh()));
	connect(buttonResetGrid, SIGNAL(clicked()), this, SIGNAL(resetMesh()));
	connect(buttonPoLIne, SIGNAL(clicked()), this, SIGNAL(meshToShape()));
	connect(editPoints, SIGNAL(clicked()), this, SLOT(handleEditButton()));
	connect(editControlPoints, SIGNAL(clicked()), this, SLOT(handleEditControlButton()));
	connect(resetControlPoint, SIGNAL(clicked()), this, SIGNAL(reset1Control()));
	connect(resetAllControlPoints, SIGNAL(clicked()), this, SIGNAL(resetAllControl()));	
	// Patch Mesh
	connect(editPPoint, SIGNAL(clicked()), this, SLOT(handlePEditButton()));
	connect(editPControlPoints, SIGNAL(clicked()), this, SLOT(handlePEditControlButton()));	
	connect(buttonAddPatch, SIGNAL(clicked()), this, SLOT(handlePAddButton()));
	connect(buttonRemovePatch, SIGNAL(clicked()), this, SIGNAL(removePatch()));
	connect(resetPControlPoint, SIGNAL(clicked()), this, SIGNAL(reset1Control()));
	connect(resetAllPControlPoints, SIGNAL(clicked()), this, SIGNAL(resetAllControl()));
	connect(snapToGrid, SIGNAL(clicked()), this, SLOT(handleSnapToGridBox()));

	iconSetChange();
	languageChange();
	selectLinear();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
}

void ColorPickerGradientProperties::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
		languageChange();

	QWidget::changeEvent(e);
}

void ColorPickerGradientProperties::selectLinear()
{
	this->setCurrentIndex(0);
	m_gradientEditType = GradientEdit::Gradient;
}

void ColorPickerGradientProperties::selectRadial()
{
	this->setCurrentIndex(1);
	labelSkew2->show();
	gSk_2->show();
	m_gradientEditType = GradientEdit::Gradient;
}

void ColorPickerGradientProperties::selectConical()
{
	this->setCurrentIndex(1);
	labelSkew2->hide();
	gSk_2->hide();
	m_gradientEditType = GradientEdit::Gradient;
}

void ColorPickerGradientProperties::selectFourColor()
{
	this->setCurrentIndex(2);
	m_gradientEditType = GradientEdit::FourColors;
}

void ColorPickerGradientProperties::selectDiamond()
{
	this->setCurrentIndex(3);
	m_gradientEditType = GradientEdit::Diamond;
}

void ColorPickerGradientProperties::selectMesh()
{
	QSignalBlocker sig(this);

	this->setCurrentIndex(4);
	editPoints->setChecked(true);
	m_gradientEditType = GradientEdit::Mesh_Point;
}

void ColorPickerGradientProperties::selectPatchMesh()
{
	QSignalBlocker sig(this);

	this->setCurrentIndex(5);
	editPPoint->setChecked(true);
	snapToGrid->setChecked(false);
	snapToGrid->setEnabled(true);
	m_gradientEditType = GradientEdit::PatchMesh_Point;
}

void ColorPickerGradientProperties::setConfig(int type, bool isMask)
{
	int t = type;

	// normalize type based on mask mode
	if (isMask)
	{
		switch (t)
		{
		default:
		case GradMask_Linear:
		case GradMask_LinearLumAlpha:
			t = Gradient_Linear;
			break;
		case GradMask_Radial:
		case GradMask_RadialLumAlpha:
			t = Gradient_Radial;
			break;
		case GradMask_Pattern:
		case GradMask_PatternInverted:
		case GradMask_PatternLumAlphaInverted:
		case GradMask_PatternLumAlpha:
			// not handled
			break;
		}
	}

	if (m_type == type)
		return;

	m_type = t;

	switch (t)
	{
	default:
	case Gradient_Linear:
		selectLinear();
		break;
	case Gradient_Radial:
		selectRadial();
		break;
	case Gradient_Diamond:
		selectDiamond();
		break;
	case Gradient_4Colors:
		selectFourColor();
		break;
	case Gradient_Conical:
		selectConical();
		break;
	case Gradient_Mesh:
		selectMesh();
		break;
	case Gradient_PatchMesh:
		selectPatchMesh();
		break;
	}
}

GradientEdit ColorPickerGradientProperties::gradientEditType() const
{
	return m_gradientEditType;
}

void ColorPickerGradientProperties::languageChange()
{
	retranslateUi(this);
}

void ColorPickerGradientProperties::handleEditButton()
{
	if (editPoints->isChecked())
	{
		resetAllControlPoints->setEnabled(false);
		resetControlPoint->setEnabled(false);
		m_gradientEditType = GradientEdit::Mesh_Point;
		emit gradientEditMode(gradientEditType());
	}
}

void ColorPickerGradientProperties::handleEditControlButton()
{
	if (editControlPoints->isChecked())
	{
		resetAllControlPoints->setEnabled(true);
		resetControlPoint->setEnabled(true);
		m_gradientEditType = GradientEdit::Mesh_ControlPoints;
		emit gradientEditMode(gradientEditType());
	}
}

void ColorPickerGradientProperties::handlePEditButton()
{
	if (editPPoint->isChecked())
	{
		snapToGrid->setEnabled(true);
		resetAllPControlPoints->setEnabled(false);
		resetPControlPoint->setEnabled(false);
		m_gradientEditType = GradientEdit::PatchMesh_Point;
		emit gradientEditMode(gradientEditType());
	}
}

void ColorPickerGradientProperties::handlePEditControlButton()
{
	if (editPControlPoints->isChecked())
	{
		snapToGrid->setEnabled(false);
		resetAllPControlPoints->setEnabled(true);
		resetPControlPoint->setEnabled(true);
		m_gradientEditType = GradientEdit::PatchMesh_ControlPoints;
		emit gradientEditMode(gradientEditType());
	}
}

void ColorPickerGradientProperties::handleSnapToGridBox()
{
	emit snapToMGrid(snapToGrid->isChecked());
}

void ColorPickerGradientProperties::handlePAddButton()
{
	editPPoint->setChecked(true);
	editPPoint->setEnabled(false);
	editPControlPoints->setEnabled(false);
	resetAllPControlPoints->setEnabled(false);
	resetPControlPoint->setEnabled(false);
	buttonAddPatch->setEnabled(false);
	m_gradientEditType = GradientEdit::PatchMesh_Polygon;
	emit gradientEditMode(gradientEditType());
}

void ColorPickerGradientProperties::endPAddButton()
{
	editPPoint->setChecked(true);
	editPPoint->setEnabled(true);
	editPControlPoints->setEnabled(true);
	resetAllPControlPoints->setEnabled(false);
	resetPControlPoint->setEnabled(false);
	buttonAddPatch->setChecked(false);
	buttonAddPatch->setEnabled(true);
	m_gradientEditType = GradientEdit::PatchMesh_Point;
	emit gradientEditMode(gradientEditType());
}

void ColorPickerGradientProperties::changebuttonRemovePatch(bool val)
{
	buttonRemovePatch->setEnabled(val);
}

void ColorPickerGradientProperties::setGradientVectorData(const CPGradientVectorData& data)
{
	disconnectSlots();

	m_data = data;
	// Linear
	gX1->setValue(data.StartX * m_unitRatio);
	gX2->setValue(data.EndX * m_unitRatio);
	gY1->setValue(data.StartY * m_unitRatio);
	gY2->setValue(data.EndY * m_unitRatio);
	gSk->setValue(data.Skew);
	// Radial
	gX1_2->setValue(data.StartX * m_unitRatio);
	gX2_2->setValue(data.EndX * m_unitRatio);
	gY1_2->setValue(data.StartY * m_unitRatio);
	gY2_2->setValue(data.EndY * m_unitRatio);
	gFX->setValue(data.FocalX * m_unitRatio);
	gFY->setValue(data.FocalY * m_unitRatio);
	gSc->setValue(data.Scale * 100.0);
	gSk_2->setValue(data.Skew);
	// Four Colors
	gC1X->setValue(data.C1X * m_unitRatio);
	gC1Y->setValue(data.C1Y * m_unitRatio);
	gC2X->setValue(data.C2X * m_unitRatio);
	gC2Y->setValue(data.C2Y * m_unitRatio);
	gC3X->setValue(data.C3X * m_unitRatio);
	gC3Y->setValue(data.C3Y * m_unitRatio);
	gC4X->setValue(data.C4X * m_unitRatio);
	gC4Y->setValue(data.C4Y * m_unitRatio);
	// Diamond
	gC1XD->setValue(data.C1X * m_unitRatio);
	gC1YD->setValue(data.C1Y * m_unitRatio);
	gC2XD->setValue(data.C2X * m_unitRatio);
	gC2YD->setValue(data.C2Y * m_unitRatio);
	gC3XD->setValue(data.C3X * m_unitRatio);
	gC3YD->setValue(data.C3Y * m_unitRatio);
	gC4XD->setValue(data.C4X * m_unitRatio);
	gC4YD->setValue(data.C4Y * m_unitRatio);
	gC5XD->setValue(data.C5X * m_unitRatio);
	gC5YD->setValue(data.C5Y * m_unitRatio);

	connectSlots();
}

void ColorPickerGradientProperties::changeSpecialLinear()
{
	m_data.StartX = gX1->value() / m_unitRatio;
	m_data.StartY = gY1->value() / m_unitRatio;
	m_data.EndX = gX2->value() / m_unitRatio;
	m_data.EndY = gY2->value() / m_unitRatio;
	m_data.FocalX = gFX->value() / m_unitRatio;
	m_data.FocalY = gFY->value() / m_unitRatio;
	m_data.Scale = 1;
	m_data.Skew = gSk->value();

	emit gradientVectorChanged();
}

void ColorPickerGradientProperties::changeSpecialRadial()
{
	m_data.StartX = gX1_2->value() / m_unitRatio;
	m_data.StartY = gY1_2->value() / m_unitRatio;
	m_data.EndX = gX2_2->value() / m_unitRatio;
	m_data.EndY = gY2_2->value() / m_unitRatio;
	m_data.FocalX = gFX->value() / m_unitRatio;
	m_data.FocalY = gFY->value() / m_unitRatio;
	m_data.Scale = gSc->value() / 100.0;
	m_data.Skew = gSk_2->value();

	emit gradientVectorChanged();
}

void ColorPickerGradientProperties::changeSpecialFourColors()
{
	m_data.C1X = gC1X->value() / m_unitRatio;
	m_data.C1Y = gC1Y->value() / m_unitRatio;
	m_data.C2X = gC2X->value() / m_unitRatio;
	m_data.C2Y = gC2Y->value() / m_unitRatio;
	m_data.C3X = gC3X->value() / m_unitRatio;
	m_data.C3Y = gC3Y->value() / m_unitRatio;
	m_data.C4X = gC4X->value() / m_unitRatio;
	m_data.C4Y = gC4Y->value() / m_unitRatio;

	emit gradientVectorChanged();
}

void ColorPickerGradientProperties::changeSpecialDiamond()
{
	m_data.C1X = gC1XD->value() / m_unitRatio;
	m_data.C1Y = gC1YD->value() / m_unitRatio;
	m_data.C2X = gC2XD->value() / m_unitRatio;
	m_data.C2Y = gC2YD->value() / m_unitRatio;
	m_data.C3X = gC3XD->value() / m_unitRatio;
	m_data.C3Y = gC3YD->value() / m_unitRatio;
	m_data.C4X = gC4XD->value() / m_unitRatio;
	m_data.C4Y = gC4YD->value() / m_unitRatio;
	m_data.C5X = gC5XD->value() / m_unitRatio;
	m_data.C5Y = gC5YD->value() / m_unitRatio;

	emit gradientVectorChanged();
}

void ColorPickerGradientProperties::iconSetChange()
{
	IconManager& im = IconManager::instance();
	editPoints->setIcon(im.loadIcon("MoveNode.png"));
	editControlPoints->setIcon(im.loadIcon("MoveKontrol.png"));
	resetControlPoint->setIcon(im.loadIcon("Reset1Node.png"));
	resetAllControlPoints->setIcon(im.loadIcon("ResetNode.png"));
	editPPoint->setIcon(im.loadIcon("MoveNode.png"));
	editPControlPoints->setIcon(im.loadIcon("MoveKontrol.png"));
	resetPControlPoint->setIcon(im.loadIcon("Reset1Node.png"));
	resetAllPControlPoints->setIcon(im.loadIcon("ResetNode.png"));
}

void ColorPickerGradientProperties::unitChange(int unitIndex)
{
	m_unitRatio = unitGetRatioFromIndex(unitIndex);

	disconnectSlots();

	gX1->setNewUnit(unitIndex);
	gY1->setNewUnit(unitIndex);
	gX2->setNewUnit(unitIndex);
	gY2->setNewUnit(unitIndex);

	gX1_2->setNewUnit(unitIndex);
	gY1_2->setNewUnit(unitIndex);
	gX2_2->setNewUnit(unitIndex);
	gY2_2->setNewUnit(unitIndex);
	gFX->setNewUnit(unitIndex);
	gFY->setNewUnit(unitIndex);

	gC1X->setNewUnit(unitIndex);
	gC1Y->setNewUnit(unitIndex);
	gC2X->setNewUnit(unitIndex);
	gC2Y->setNewUnit(unitIndex);
	gC3X->setNewUnit(unitIndex);
	gC3Y->setNewUnit(unitIndex);
	gC4X->setNewUnit(unitIndex);
	gC4Y->setNewUnit(unitIndex);

	gC1XD->setNewUnit(unitIndex);
	gC1YD->setNewUnit(unitIndex);
	gC2XD->setNewUnit(unitIndex);
	gC2YD->setNewUnit(unitIndex);
	gC3XD->setNewUnit(unitIndex);
	gC3YD->setNewUnit(unitIndex);
	gC4XD->setNewUnit(unitIndex);
	gC4YD->setNewUnit(unitIndex);
	gC5XD->setNewUnit(unitIndex);
	gC5YD->setNewUnit(unitIndex);

	connectSlots();
}

void ColorPickerGradientProperties::connectSlots()
{
	// Linear
	connect(gX1,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialLinear()));
	connect(gX2,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialLinear()));
	connect(gY1,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialLinear()));
	connect(gY2,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialLinear()));
	connect(gSk,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialLinear()));
	// Radial
	connect(gX1_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialRadial()));
	connect(gX2_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialRadial()));
	connect(gY1_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialRadial()));
	connect(gY2_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialRadial()));
	connect(gFX,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialRadial()));
	connect(gFY,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialRadial()));
	connect(gSk_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialRadial()));
	connect(gSc,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialRadial()));
	// Four Colors
	connect(gC1X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialFourColors()));
	connect(gC1Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialFourColors()));
	connect(gC2X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialFourColors()));
	connect(gC2Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialFourColors()));
	connect(gC3X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialFourColors()));
	connect(gC3Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialFourColors()));
	connect(gC4X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialFourColors()));
	connect(gC4Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialFourColors()));
	// Diamond
	connect(gC1XD,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialDiamond()));
	connect(gC1YD,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialDiamond()));
	connect(gC2XD,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialDiamond()));
	connect(gC2YD,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialDiamond()));
	connect(gC3XD,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialDiamond()));
	connect(gC3YD,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialDiamond()));
	connect(gC4XD,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialDiamond()));
	connect(gC4YD,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialDiamond()));
	connect(gC5XD,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialDiamond()));
	connect(gC5YD,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialDiamond()));
}

void ColorPickerGradientProperties::disconnectSlots()
{
	// Linear
	disconnect(gX1,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialLinear()));
	disconnect(gX2,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialLinear()));
	disconnect(gY1,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialLinear()));
	disconnect(gY2,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialLinear()));
	disconnect(gSk,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialLinear()));
	// Radial
	disconnect(gX1_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialRadial()));
	disconnect(gX2_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialRadial()));
	disconnect(gY1_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialRadial()));
	disconnect(gY2_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialRadial()));
	disconnect(gFX,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialRadial()));
	disconnect(gFY,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialRadial()));
	disconnect(gSk_2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecialRadial()));
	disconnect(gSc,   SIGNAL(valueChanged(double)), this, SLOT(changeSpecialRadial()));
	// Four Colors
	disconnect(gC1X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialFourColors()));
	disconnect(gC1Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialFourColors()));
	disconnect(gC2X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialFourColors()));
	disconnect(gC2Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialFourColors()));
	disconnect(gC3X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialFourColors()));
	disconnect(gC3Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialFourColors()));
	disconnect(gC4X,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialFourColors()));
	disconnect(gC4Y,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialFourColors()));
	// Diamond
	disconnect(gC1XD,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialDiamond()));
	disconnect(gC1YD,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialDiamond()));
	disconnect(gC2XD,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialDiamond()));
	disconnect(gC2YD,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialDiamond()));
	disconnect(gC3XD,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialDiamond()));
	disconnect(gC3YD,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialDiamond()));
	disconnect(gC4XD,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialDiamond()));
	disconnect(gC4YD,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialDiamond()));
	disconnect(gC5XD,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialDiamond()));
	disconnect(gC5YD,  SIGNAL(valueChanged(double)), this, SLOT(changeSpecialDiamond()));
}
