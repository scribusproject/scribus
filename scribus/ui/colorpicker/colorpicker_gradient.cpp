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

#include "colorpicker_gradient.h"

#include "util_gui.h"
#include "ui/gradienteditor.h"
#include "scribus.h"
#include "scribusview.h"
#include "selection.h"

/* ********************************************************************************* *
 *
 * Constructor + Setup
 *
 * ********************************************************************************* */

ColorPickerGradient::ColorPickerGradient(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);

	sectionVector->collapse();

	listColor1Swatches = new ColorListBox(ColorListBox::fancyPixmap);
	listColor2Swatches = new ColorListBox(ColorListBox::fancyPixmap);
	listColor3Swatches = new ColorListBox(ColorListBox::fancyPixmap);
	listColor4Swatches = new ColorListBox(ColorListBox::fancyPixmap);
	listColorMesh = new ColorListBox(ColorListBox::fancyPixmap);

	buttonColor1->setContextWidget(listColor1Swatches);
	buttonColor2->setContextWidget(listColor2Swatches);
	buttonColor3->setContextWidget(listColor3Swatches);
	buttonColor4->setContextWidget(listColor4Swatches);
	buttonColorMesh->setContextWidget(listColorMesh);

	numberColor1Alpha->setDecimals(0);
	numberColor2Alpha->setDecimals(0);
	numberColor3Alpha->setDecimals(0);
	numberColor4Alpha->setDecimals(0);
	numberColorMeshAlpha->setDecimals(0);
	numberColor1Shade->setDecimals(0);
	numberColor2Shade->setDecimals(0);
	numberColor3Shade->setDecimals(0);
	numberColor4Shade->setDecimals(0);
	numberColorMeshShade->setDecimals(0);

	gradientSelector = new QComboBox(); // dummy selector

	languageChange();
	connectSlots();

	connect(properties,				&ColorPickerGradientProperties::currentChanged,	this, &ColorPickerGradient::updateSize);
	connect(stackGradientPicker,	&StackedContainer::currentChanged,				this, &ColorPickerGradient::updateSize);
	connect(sectionGradient,		&SectionContainer::sizeChanged,					this, &ColorPickerGradient::updateSize);
	connect(sectionSwatches,		&SectionContainer::sizeChanged,					this, &ColorPickerGradient::updateSize);
	connect(sectionVector,			&SectionContainer::sizeChanged,					this, &ColorPickerGradient::updateSize);
}

void ColorPickerGradient::connectSlots()
{	
	connect(sectionVector,			&SectionContainer::collapsedStateChanged,				this, &ColorPickerGradient::handleGradientEditProperties);
	connect(swatches,				&ColorPickerGradientSwatches::gradientChanged,			this, &ColorPickerGradient::updateGradientFromSwatches);
	connect(gradientSelector,		&QComboBox::currentIndexChanged,						this, &ColorPickerGradient::updateGradientTypeFromSelector);
	connect(checkboxLumAlpha,		&QCheckBox::stateChanged,								this, &ColorPickerGradient::gradientChanged);
	connect(gradientEditor,			&GradientEditor::gradientChanged,						this, &ColorPickerGradient::updateGradientFromEditor);
	connect(gradientEditor,			&GradientEditor::repeatMethodChanged,					this, &ColorPickerGradient::updateRepeatMethod);
	connect(properties,				&ColorPickerGradientProperties::gradientVectorChanged,	this, &ColorPickerGradient::gradientVectorChanged);
	connect(properties,				&ColorPickerGradientProperties::gradientEditMode,		this,	&ColorPickerGradient::setGradientEditMode);

	connect(numberColor1Alpha,		&ScrSpinBox::valueChanged,								this, &ColorPickerGradient::updateFourColors);
	connect(numberColor2Alpha,		&ScrSpinBox::valueChanged,								this, &ColorPickerGradient::updateFourColors);
	connect(numberColor3Alpha,		&ScrSpinBox::valueChanged,								this, &ColorPickerGradient::updateFourColors);
	connect(numberColor4Alpha,		&ScrSpinBox::valueChanged,								this, &ColorPickerGradient::updateFourColors);
	connect(numberColor1Shade,		&ScrSpinBox::valueChanged,								this, &ColorPickerGradient::updateFourColors);
	connect(numberColor2Shade,		&ScrSpinBox::valueChanged,								this, &ColorPickerGradient::updateFourColors);
	connect(numberColor3Shade,		&ScrSpinBox::valueChanged,								this, &ColorPickerGradient::updateFourColors);
	connect(numberColor4Shade,		&ScrSpinBox::valueChanged,								this, &ColorPickerGradient::updateFourColors);
	connect(listColor1Swatches,		&ColorListBox::itemSelectionChanged,					this, &ColorPickerGradient::updateFourColors);
	connect(listColor2Swatches,		&ColorListBox::itemSelectionChanged,					this, &ColorPickerGradient::updateFourColors);
	connect(listColor3Swatches,		&ColorListBox::itemSelectionChanged,					this, &ColorPickerGradient::updateFourColors);
	connect(listColor4Swatches,		&ColorListBox::itemSelectionChanged,					this, &ColorPickerGradient::updateFourColors);

	connect(numberColorMeshAlpha,	&ScrSpinBox::valueChanged,								this, &ColorPickerGradient::updateMeshColor);
	connect(numberColorMeshShade,	&ScrSpinBox::valueChanged,								this, &ColorPickerGradient::updateMeshColor);
	connect(listColorMesh,			&ColorListBox::itemSelectionChanged,					this, &ColorPickerGradient::updateMeshColor);
}

void ColorPickerGradient::disconnectSlots()
{	
	disconnect(sectionVector,		&SectionContainer::collapsedStateChanged,				this, &ColorPickerGradient::handleGradientEditProperties);
	disconnect(swatches,			&ColorPickerGradientSwatches::gradientChanged,			this, &ColorPickerGradient::updateGradientFromSwatches);
	disconnect(gradientSelector,	&QComboBox::currentIndexChanged,						this, &ColorPickerGradient::updateGradientTypeFromSelector);
	disconnect(checkboxLumAlpha,	&QCheckBox::stateChanged,								this, &ColorPickerGradient::gradientChanged);
	disconnect(gradientEditor,		&GradientEditor::gradientChanged,						this, &ColorPickerGradient::updateGradientFromEditor);
	disconnect(gradientEditor,		&GradientEditor::repeatMethodChanged,					this, &ColorPickerGradient::updateRepeatMethod);
	disconnect(properties,			&ColorPickerGradientProperties::gradientVectorChanged,	this, &ColorPickerGradient::gradientVectorChanged);
	disconnect(properties,			&ColorPickerGradientProperties::gradientEditMode,		this, &ColorPickerGradient::setGradientEditMode);

	disconnect(numberColor1Alpha,	&ScrSpinBox::valueChanged,								this, &ColorPickerGradient::updateFourColors);
	disconnect(numberColor2Alpha,	&ScrSpinBox::valueChanged,								this, &ColorPickerGradient::updateFourColors);
	disconnect(numberColor3Alpha,	&ScrSpinBox::valueChanged,								this, &ColorPickerGradient::updateFourColors);
	disconnect(numberColor4Alpha,	&ScrSpinBox::valueChanged,								this, &ColorPickerGradient::updateFourColors);
	disconnect(numberColor1Shade,	&ScrSpinBox::valueChanged,								this, &ColorPickerGradient::updateFourColors);
	disconnect(numberColor2Shade,	&ScrSpinBox::valueChanged,								this, &ColorPickerGradient::updateFourColors);
	disconnect(numberColor3Shade,	&ScrSpinBox::valueChanged,								this, &ColorPickerGradient::updateFourColors);
	disconnect(numberColor4Shade,	&ScrSpinBox::valueChanged,								this, &ColorPickerGradient::updateFourColors);
	disconnect(listColor1Swatches,	&ColorListBox::itemSelectionChanged,					this, &ColorPickerGradient::updateFourColors);
	disconnect(listColor2Swatches,	&ColorListBox::itemSelectionChanged,					this, &ColorPickerGradient::updateFourColors);
	disconnect(listColor3Swatches,	&ColorListBox::itemSelectionChanged,					this, &ColorPickerGradient::updateFourColors);
	disconnect(listColor4Swatches,	&ColorListBox::itemSelectionChanged,					this, &ColorPickerGradient::updateFourColors);

	disconnect(numberColorMeshAlpha,	&ScrSpinBox::valueChanged,							this, &ColorPickerGradient::updateMeshColor);
	disconnect(numberColorMeshShade,	&ScrSpinBox::valueChanged,							this, &ColorPickerGradient::updateMeshColor);
	disconnect(listColorMesh,			&ColorListBox::itemSelectionChanged,				this, &ColorPickerGradient::updateMeshColor);
}

/* ********************************************************************************* *
 *
 * Members
 *
 * ********************************************************************************* */

void ColorPickerGradient::setGradientList(const GradientList &list)
{
	swatches->setGradients(list, true);
}

void ColorPickerGradient::setColorList(const ColorList &list)
{
	QSignalBlocker sig1(gradientEditor);
	QSignalBlocker sig2(listColor1Swatches);
	QSignalBlocker sig3(listColor2Swatches);
	QSignalBlocker sig4(listColor3Swatches);
	QSignalBlocker sig5(listColor4Swatches);
	QSignalBlocker sig6(listColorMesh);

	colList = list;
	gradientEditor->setColors(colList);
	listColor1Swatches->setColors(colList, true);
	listColor2Swatches->setColors(colList, true);
	listColor3Swatches->setColors(colList, true);
	listColor4Swatches->setColors(colList, true);
	listColorMesh->setColors(colList, true);
}

Context ColorPickerGradient::context() const
{
	return m_context;
}

void ColorPickerGradient::setContext(Context config)
{
	m_context = config;

	disconnectSlots();

	// reset controls
	gradientSelector->clear();

	switch (m_context)
	{
	case Context::Line:
		gradientSelector->addItem(tr("Linear"),        Gradient_Linear);
		gradientSelector->addItem(tr("Radial"),        Gradient_Radial);
		gradientSelector->setCurrentIndex(0);
		checkboxLumAlpha->setVisible(false);
		gradientEditor->setExtendVisible(true);
		break;
	case Context::FillMask:
		gradientSelector->addItem(tr("Linear"),        Gradient_Linear);
		gradientSelector->addItem(tr("Radial"),        Gradient_Radial);
		gradientSelector->setCurrentIndex(0);
		checkboxLumAlpha->setVisible(true);
		gradientEditor->setExtendVisible(false);
		break;
	case Context::Fill:
		gradientSelector->addItem(tr("Linear"),        Gradient_Linear);
		gradientSelector->addItem(tr("Radial"),        Gradient_Radial);
		gradientSelector->addItem(tr("Conical"),       Gradient_Conical);
		gradientSelector->addItem(tr("Diamond"),       Gradient_Diamond);
		gradientSelector->addItem(tr("4 Colors"),      Gradient_4Colors);
		gradientSelector->addItem(tr("Mesh"),          Gradient_Mesh);
		gradientSelector->addItem(tr("Patch Mesh"),    Gradient_PatchMesh);
		gradientSelector->setCurrentIndex(0);
		checkboxLumAlpha->setVisible(false);
		gradientEditor->setExtendVisible(true);
		break;
	default:
	case Context::Simple:
		break;
	}

	connectSlots();

	updateSize();
}

void ColorPickerGradient::setSelector(QComboBox *selector)
{
	disconnect(gradientSelector, &QComboBox::currentIndexChanged, this, &ColorPickerGradient::updateGradientTypeFromSelector);
	gradientSelector = selector;
	connect(gradientSelector, &QComboBox::currentIndexChanged, this, &ColorPickerGradient::updateGradientTypeFromSelector);
}

void ColorPickerGradient::setGradientData(const CPGradientData& gradient)
{
	m_gradient = gradient;

	// update UI + controls
	setCurrentGradientType(gradient.Type);
	setCurrentGradientName(gradient.Name);
	setCurrentGradient(gradient.Gradient);
	setCurrentFourColors(
				gradient.Color1Name, gradient.Color2Name, gradient.Color3Name, gradient.Color4Name,
				gradient.Color1Shade, gradient.Color2Shade, gradient.Color3Shade, gradient.Color4Shade,
				gradient.Color1Alpha, gradient.Color2Alpha, gradient.Color3Alpha, gradient.Color4Alpha
				);
	setCurrentRepeatMethod(gradient.repeatMethod);

}

const CPGradientData& ColorPickerGradient::gradientData() const
{
	return m_gradient;
}

void ColorPickerGradient::setGradientVectorData(const CPGradientVectorData& data)
{
	QSignalBlocker sig(properties);
	properties->setGradientVectorData(data);
	//	properties->setConfig(type(), isMask());
}

const CPGradientVectorData& ColorPickerGradient::gradientVectorData() const
{
	return properties->gradientVectorData();
}

void ColorPickerGradient::setGradientMeshData(const CPColorData& data)
{
	m_gradientMesh = data;
	setCurrentMeshPoint(m_gradientMesh.Name, m_gradientMesh.Shade, m_gradientMesh.Opacity);
}

const CPColorData& ColorPickerGradient::gradientMeshData() const
{
	return m_gradientMesh;
}

int ColorPickerGradient::type() const
{
	int index = qvariant_cast<int>(gradientSelector->currentData().toInt());

	if (isMask())
	{
		switch (index)
		{
		case Gradient_Linear:
			if (checkboxLumAlpha->isChecked())
				index = GradMask_LinearLumAlpha;
			else
				index = GradMask_Linear;
			break;
		case Gradient_Radial:
			if (checkboxLumAlpha->isChecked())
				index = GradMask_RadialLumAlpha;
			else
				index = GradMask_Radial;
			break;
		}
	}

	return index;
}

GradientEdit ColorPickerGradient::gradientEditMode() const
{
	int etype = type();
	GradientEdit m_gradType;

	if (isMask())
	{
		switch (etype)
		{
		default:
		case GradMask_Linear:
		case GradMask_LinearLumAlpha:
		case GradMask_Radial:
		case GradMask_RadialLumAlpha:
			m_gradType = GradientEdit::Gradient_Mask;
			break;
		}
	}
	else if (m_context == Context::Line)
		m_gradType = GradientEdit::Gradient_Stroke;
	else
	{
		switch (etype)
		{
		default:
		case Gradient_Linear:
		case Gradient_Radial:
		case Gradient_Conical:
			m_gradType = GradientEdit::Gradient;
			break;
		case Gradient_4Colors:
			m_gradType = GradientEdit::FourColors;
			break;
		case Gradient_Diamond:
			m_gradType = GradientEdit::Diamond;
			break;
		case Gradient_Mesh:
		case Gradient_PatchMesh:
			m_gradType = m_gradientMeshEdit;
			break;
		}
	}

	return m_gradType;
}

void ColorPickerGradient::setGradientEditMode(GradientEdit mode)
{
	m_gradientMeshEdit = mode;

	handleGradientEditProperties();
}

void ColorPickerGradient::setGradientEditPropertiesEnabled(bool enabled)
{
	QSignalBlocker sig(sectionVector);
	sectionVector->setIsCollapsed(!enabled);
	//	handleGradientEditProperties(); // break m_ScMW->view->requestMode();
	updateSize();
}

bool ColorPickerGradient::gradientEditPropertiesEnabled() const
{
	return !sectionVector->isCollapsed();
}

QString ColorPickerGradient::toolTipText() const
{
	QString name = m_gradient.Name.isEmpty() ? CommonStrings::trCustom : m_gradient.Name;
	return QString( tr("Gradient: %1") ).arg(name);
}

void ColorPickerGradient::disableGradientEditProperties()
{
	if (gradientEditPropertiesEnabled())
	{
		setGradientEditPropertiesEnabled(false);
		handleGradientEditProperties();
	}

}

void ColorPickerGradient::setDoc(ScribusDoc *doc)
{
	m_doc = doc;
	unitChange();
}

/* ********************************************************************************* *
 *
 * Slots
 *
 * ********************************************************************************* */

void ColorPickerGradient::languageChange()
{
	sectionVector->setText( tr("Vector"));
	sectionGradient->setText( tr("Gradient"));
	sectionSwatches->setText( tr("Gradient Swatches"));

	retranslateUi(this);

}

void ColorPickerGradient::unitChange()
{
	if (!m_doc)
		return;

	currentUnit = m_doc->unitIndex();
	properties->unitChange(currentUnit);
}

void ColorPickerGradient::updateGradientFromSwatches(QString name, VGradient gradient)
{
	// use local gradient if gradient name is "custom"
	if (name.isEmpty())
		gradient = this->gradientData().Gradient;

	gradientEditor->setGradientEditable(name.isEmpty());

	m_gradient.Gradient = gradient;
	m_gradient.Name = name;
	updateGradient();
}

void ColorPickerGradient::updateGradientFromEditor()
{
	m_gradient.Gradient = gradientEditor->gradient();
	m_gradient.Name = "";
	updateGradient();
}

void ColorPickerGradient::updateGradientTypeFromSelector()
{
	VGradient::VGradientType gradType = VGradient::linear;

	switch (type())
	{
	default:
	case Gradient_Linear:
		gradType = VGradient::linear;
		break;
	case Gradient_Radial:
		gradType = VGradient::radial;
		break;
	case Gradient_Diamond:
		gradType = VGradient::diamond;
		break;
	case Gradient_4Colors:
		gradType = VGradient::fourcolor;
		break;
	case Gradient_Mesh:
		gradType = VGradient::mesh;
		break;
	case Gradient_Conical:
	case Gradient_PatchMesh:
		gradType = VGradient::freemesh;
		break;
	}

	int gtype = type();

	m_gradient.Gradient.setType(gradType);
	m_gradient.Type = gtype;

	updateUI();
	updateGradient();

	emit properties->gradientEditMode(properties->gradientEditType());

}

void ColorPickerGradient::updateFourColors()
{
	QString color1 = listColor1Swatches->currentColor();
	if (color1 == CommonStrings::tr_NoneColor)
		color1 = CommonStrings::None;
	QString color2 = listColor2Swatches->currentColor();
	if (color2 == CommonStrings::tr_NoneColor)
		color2 = CommonStrings::None;
	QString color3 = listColor3Swatches->currentColor();
	if (color3 == CommonStrings::tr_NoneColor)
		color3 = CommonStrings::None;
	QString color4 = listColor4Swatches->currentColor();
	if (color4 == CommonStrings::tr_NoneColor)
		color4 = CommonStrings::None;

	m_gradient.Color1Name = color1;
	m_gradient.Color2Name = color2;
	m_gradient.Color3Name = color3;
	m_gradient.Color4Name = color4;
	m_gradient.Color1Alpha = numberColor1Alpha->value() / 100.0;
	m_gradient.Color2Alpha = numberColor2Alpha->value() / 100.0;
	m_gradient.Color3Alpha = numberColor3Alpha->value() / 100.0;
	m_gradient.Color4Alpha = numberColor4Alpha->value() / 100.0;
	m_gradient.Color1Shade = numberColor1Shade->value();
	m_gradient.Color2Shade = numberColor2Shade->value();
	m_gradient.Color3Shade = numberColor3Shade->value();
	m_gradient.Color4Shade = numberColor4Shade->value();

	// render color buttons
	buttonColor1->setBrush(colorBrush(buttonColor1->circleSize(), m_gradient.Color1Name, m_gradient.Color1Shade, m_gradient.Color1Alpha));
	buttonColor1->setToolTip(m_gradient.Color1Name);
	buttonColor2->setBrush(colorBrush(buttonColor2->circleSize(), m_gradient.Color2Name, m_gradient.Color2Shade, m_gradient.Color2Alpha));
	buttonColor2->setToolTip(m_gradient.Color2Name);
	buttonColor3->setBrush(colorBrush(buttonColor3->circleSize(), m_gradient.Color3Name, m_gradient.Color3Shade, m_gradient.Color3Alpha));
	buttonColor3->setToolTip(m_gradient.Color3Name);
	buttonColor4->setBrush(colorBrush(buttonColor4->circleSize(), m_gradient.Color4Name, m_gradient.Color4Shade, m_gradient.Color4Alpha));
	buttonColor4->setToolTip(m_gradient.Color4Name);

	updateGradient();
}

void ColorPickerGradient::updateMeshColor()
{
	QString colorMesh = listColorMesh->currentColor();
	if (colorMesh == CommonStrings::tr_NoneColor)
		colorMesh = CommonStrings::None;

	m_gradientMesh.Name = colorMesh;
	m_gradientMesh.Shade = numberColorMeshShade->value();
	m_gradientMesh.Opacity = numberColorMeshAlpha->value() / 100.0;

	// render color button
	buttonColorMesh->setBrush(colorBrush(buttonColorMesh->circleSize(), m_gradientMesh.Name, m_gradientMesh.Shade, m_gradientMesh.Opacity));
	buttonColorMesh->setToolTip(m_gradientMesh.Name);

	if (gradientEditPropertiesEnabled() && (type() == Gradient_PatchMesh || type() == Gradient_Mesh))
		updateGradientMesh();
}

void ColorPickerGradient::updateRepeatMethod()
{
	m_gradient.repeatMethod = gradientEditor->repeatMethod();
	updateGradient();
}

void ColorPickerGradient::updateSize()
{
	int w = this->width();
	adjustSize();
	resize(w, sizeHint().height());
	emit sizeChanged();
}

void ColorPickerGradient::handleGradientEditProperties()
{
	if (!m_doc)
		return;

	ScribusMainWindow *m_ScMW = m_doc->scMW();
	if (!m_ScMW || !m_ScMW->view)
		return;

	GradientEdit type = gradientEditMode();
	bool editingOn = gradientEditPropertiesEnabled();

	m_ScMW->view->editStrokeGradient = type;

	// Reference: void PropertiesPalette::toggleGradientEdit(int stroke)

	if (editingOn)
	{
		if ((type == GradientEdit::Mesh_Point) /*|| (type == GradientEdit::Mesh_Color)*/ || (type == GradientEdit::Mesh_ControlPoints))
			m_ScMW->view->requestMode(modeEditMeshGradient);
		else if (/*(type == GradientEdit::PatchMesh_Color) ||*/ (type == GradientEdit::PatchMesh_Point) || (type == GradientEdit::PatchMesh_ControlPoints) || (type == GradientEdit::PatchMesh_Polygon))
			m_ScMW->view->requestMode(modeEditMeshPatch);
		else
			m_ScMW->view->requestMode(modeEditGradientVectors);
	}
	else
		m_ScMW->view->requestMode(modeNormal);

	PageItem *currentItem = m_doc->m_Selection->itemAt(0);

	if (currentItem)
	{
		m_ScMW->view->RefreshGradient(currentItem);

		if ( (type == GradientEdit::Mesh_Point || type == GradientEdit::PatchMesh_Point) && (currentItem->selectedMeshPointX > -1) && (currentItem->selectedMeshPointY > -1))
			enableMeshColor(true);
		else
			enableMeshColor(false);
	}
}

/* ********************************************************************************* *
 *
 * Methods
 *
 * ********************************************************************************* */

void ColorPickerGradient::setCurrentGradientType(int type)
{
	if (isMask())
	{
		QSignalBlocker sigLumAlpha(checkboxLumAlpha);

		switch (type)
		{
		default:
		case GradMask_Linear:
			type = Gradient_Linear;
			checkboxLumAlpha->setChecked(false);
			break;
		case GradMask_LinearLumAlpha:
			type = Gradient_Linear;
			checkboxLumAlpha->setChecked(true);
			break;
		case GradMask_Radial:
			type = Gradient_Radial;
			checkboxLumAlpha->setChecked(false);
			break;
		case GradMask_RadialLumAlpha:
			type = Gradient_Radial;
			checkboxLumAlpha->setChecked(true);
			break;
		}
	}

	QSignalBlocker sigType(gradientSelector);

	int index = gradientSelector->findData(type);
	if ( index != -1 )
		gradientSelector->setCurrentIndex(index);
	else
		gradientSelector->setCurrentIndex(0);

	updateUI();
}

void ColorPickerGradient::setCurrentGradientName(QString name)
{
	QSignalBlocker sigGradientEditor(gradientEditor);
	gradientEditor->setGradientEditable(name.isEmpty());

	QSignalBlocker sigSwatches(swatches);
	swatches->setCurrentGradient(name);
}

void ColorPickerGradient::setCurrentGradient(const VGradient& gradient)
{
	QSignalBlocker sigGradientEditor(gradientEditor);
	gradientEditor->setGradient(gradient);
}

void ColorPickerGradient::setCurrentFourColors(QString color1, QString color2, QString color3, QString color4, double color1Shade, double color2Shade, double color3Shade, double color4Shade, double color1Alpha, double color2Alpha, double color3Alpha, double color4Alpha)
{

	QSignalBlocker sigColor1Alpha(numberColor1Alpha);
	QSignalBlocker sigColor2Alpha(numberColor2Alpha);
	QSignalBlocker sigColor3Alpha(numberColor3Alpha);
	QSignalBlocker sigColor4Alpha(numberColor4Alpha);
	numberColor1Alpha->setValue(color1Alpha * 100.0);
	numberColor2Alpha->setValue(color2Alpha * 100.0);
	numberColor3Alpha->setValue(color3Alpha * 100.0);
	numberColor4Alpha->setValue(color4Alpha * 100.0);
	m_gradient.Color1Alpha = color1Alpha;
	m_gradient.Color2Alpha = color2Alpha;
	m_gradient.Color3Alpha = color3Alpha;
	m_gradient.Color4Alpha = color4Alpha;

	QSignalBlocker sigColor1Shade(numberColor1Shade);
	QSignalBlocker sigColor2Shade(numberColor2Shade);
	QSignalBlocker sigColor3Shade(numberColor3Shade);
	QSignalBlocker sigColor4Shade(numberColor4Shade);
	numberColor1Shade->setValue(color1Shade);
	numberColor2Shade->setValue(color2Shade);
	numberColor3Shade->setValue(color3Shade);
	numberColor4Shade->setValue(color4Shade);
	m_gradient.Color1Shade = color1Shade;
	m_gradient.Color2Shade = color2Shade;
	m_gradient.Color3Shade = color3Shade;
	m_gradient.Color4Shade = color4Shade;

	QSignalBlocker sigListColor1(listColor1Swatches);
	QSignalBlocker sigListColor2(listColor2Swatches);
	QSignalBlocker sigListColor3(listColor3Swatches);
	QSignalBlocker sigListColor4(listColor4Swatches);
	listColor1Swatches->setCurrentColor(color1);
	listColor2Swatches->setCurrentColor(color2);
	listColor3Swatches->setCurrentColor(color3);
	listColor4Swatches->setCurrentColor(color4);
	m_gradient.Color1Name = color1;
	m_gradient.Color2Name = color2;
	m_gradient.Color3Name = color3;
	m_gradient.Color4Name = color4;

	QSignalBlocker sigButtonColor1(buttonColor1);
	QSignalBlocker sigButtonColor2(buttonColor2);
	QSignalBlocker sigButtonColor3(buttonColor3);
	QSignalBlocker sigButtonColor4(buttonColor4);
	buttonColor1->setBrush(colorBrush(buttonColor1->circleSize(), color1, m_gradient.Color1Shade, m_gradient.Color1Alpha));
	buttonColor1->setToolTip(color1);
	buttonColor2->setBrush(colorBrush(buttonColor2->circleSize(), color2, m_gradient.Color2Shade, m_gradient.Color2Alpha));
	buttonColor2->setToolTip(color2);
	buttonColor3->setBrush(colorBrush(buttonColor3->circleSize(), color3, m_gradient.Color3Shade, m_gradient.Color3Alpha));
	buttonColor3->setToolTip(color3);
	buttonColor4->setBrush(colorBrush(buttonColor4->circleSize(), color4, m_gradient.Color4Shade, m_gradient.Color4Alpha));
	buttonColor4->setToolTip(color4);

}

void ColorPickerGradient::setCurrentMeshPoint(QString colorName, double shade, double alpha)
{

	QSignalBlocker sigMeshListColor(listColorMesh);
	listColorMesh->setCurrentColor(colorName);
	m_gradientMesh.Name = colorName;

	QSignalBlocker sigMeshShade(numberColorMeshShade);
	numberColorMeshShade->setValue(shade);
	m_gradientMesh.Shade = shade;

	QSignalBlocker sigMeshAlpha(numberColorMeshAlpha);
	numberColorMeshAlpha->setValue(alpha * 100);
	m_gradientMesh.Opacity = alpha;

	QSignalBlocker sigMeshColor(buttonColorMesh);
	buttonColorMesh->setBrush(colorBrush(buttonColorMesh->circleSize(), m_gradientMesh.Name, m_gradientMesh.Shade, m_gradientMesh.Opacity));
	buttonColorMesh->setToolTip(m_gradientMesh.Name);
}

void ColorPickerGradient::setCurrentRepeatMethod(VGradient::VGradientRepeatMethod method)
{
	gradientEditor->setRepeatMethod(method);
}

QBrush ColorPickerGradient::colorBrush(QSize size, QString colorName, double shade, double opacity) const
{
	if (!m_doc || colorName == CommonStrings::tr_NoneColor || colorName == CommonStrings::None)
		return renderEmptyPattern(size, devicePixelRatio());

	ScColor sColor(0, 0, 0);

	shade = qBound(0.0, shade, 100.0);
	opacity = qBound(0.0, opacity, 1.0);

	if (m_doc->PageColors.contains(colorName))
		sColor = m_doc->PageColors.value(colorName);

	QColor qColorShade = ScColorEngine::getDisplayColor(sColor, m_doc, shade);
	QColor qColor = ScColorEngine::getDisplayColor(sColor, m_doc, 100.0);

	return renderColor(size, devicePixelRatio(), qColor, qColorShade, opacity);
}

void ColorPickerGradient::updateGradient()
{
	m_gradient.Type = type();
	setGradientData(m_gradient);
	emit gradientChanged();
}

void ColorPickerGradient::updateGradientMesh()
{
	setGradientMeshData(m_gradientMesh);
	emit gradientMeshChanged();
}

void ColorPickerGradient::updateUI()
{
	int gtype = type();

	properties->setConfig(gtype, isMask());

	switch (gtype)
	{
	default:
	case Gradient_Conical:
	case Gradient_Linear:
	case Gradient_Radial:
	case Gradient_Diamond:
		stackGradientPicker->setCurrentWidget(pageNormal);
		sectionSwatches->setVisible(true);
		break;
	case Gradient_Mesh:
	case Gradient_PatchMesh:
		stackGradientPicker->setCurrentWidget(pageMesh);
		sectionSwatches->setVisible(false);
		break;
	case Gradient_4Colors:
		stackGradientPicker->setCurrentWidget(pageFourColors);
		sectionSwatches->setVisible(false);
		break;
	}

	updateSize();

}

void ColorPickerGradient::enableMeshColor(bool enable)
{
	buttonColorMesh->setEnabled(enable);
	numberColorMeshShade->setEnabled(enable);
	numberColorMeshAlpha->setEnabled(enable);
}

bool ColorPickerGradient::isMask() const
{
	return (m_context == Context::FillMask || m_context == Context::LineMask);
}

