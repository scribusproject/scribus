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

#include "colorpicker.h"

#include <QButtonGroup>
#include "iconmanager.h"
#include "scribus.h"
#include "scribusapp.h"
#include "selection.h"
#include "ui/widgets/popup_menu.h"

/* ********************************************************************************* *
 *
 * Constructor + Setup
 *
 * ********************************************************************************* */

ColorPicker::ColorPicker(Context context, QWidget *parent) : QWidget(parent)
{
	setupUi(this);

	comboOverprint = new QComboBox();
	checkboxUsedColors = new QCheckBox();

	buttonGroup = new QButtonGroup();
	buttonGroup->addButton(buttonSolid, (int)Mode::Solid);
	buttonGroup->addButton(buttonGradient, (int)Mode::Gradient);
	buttonGroup->addButton(buttonHatch, (int)Mode::Hatch);
	buttonGroup->addButton(buttonPattern, (int)Mode::Pattern);

	gradientEdit->setSelector(comboType); // required; gradientEdit has no gradient type selector
	comboType->setVisible(false);
	buttonPattern->setEnabled(false); // Button will enabled only if there are document patterns
	buttonPreview->setFixedSize(32, 32);

	initContextMenu();
	languageChange();
	iconSetChange();
	setContext(context);

	stack->setCurrentWidget(colorEdit);

	// Picker
	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(buttonGroup, &QButtonGroup::buttonClicked, this, &ColorPicker::changedTab);
	connect(comboOverprint, &QComboBox::activated, this, &ColorPicker::updateOverPrint);
	connect(checkboxUsedColors, &QCheckBox::toggled, this, &ColorPicker::toggleColorLists);
	connect(stack, &StackedContainer::currentChanged, this, &ColorPicker::updateSize);
	// Color
	connect(colorEdit, &ColorPickerColor::colorChanged, this, &ColorPicker::updateColor);
	connect(colorEdit, &ColorPickerColor::sizeChanged, this, &ColorPicker::updateSize);
	// Gradient
	connect(gradientEdit, &ColorPickerGradient::gradientChanged, this, &ColorPicker::updateGradient);
	connect(gradientEdit, &ColorPickerGradient::gradientVectorChanged, this, &ColorPicker::updateGradientVector);
	connect(gradientEdit, &ColorPickerGradient::gradientMeshChanged, this, &ColorPicker::updateGradientMesh);
	connect(gradientEdit, &ColorPickerGradient::sizeChanged, this, &ColorPicker::updateSize);
	connect(gradientEdit->Properties(), &ColorPickerGradientProperties::createNewMesh, this,	&ColorPicker::createNewMesh);
	connect(gradientEdit->Properties(), &ColorPickerGradientProperties::resetMesh, this,		&ColorPicker::resetMesh);
	connect(gradientEdit->Properties(), &ColorPickerGradientProperties::meshToShape, this,	&ColorPicker::meshToShape);
	connect(gradientEdit->Properties(), &ColorPickerGradientProperties::reset1Control, this,	&ColorPicker::reset1Control);
	connect(gradientEdit->Properties(), &ColorPickerGradientProperties::resetAllControl, this,&ColorPicker::resetAllControl);
	connect(gradientEdit->Properties(), &ColorPickerGradientProperties::removePatch, this,	&ColorPicker::removePatch);
	connect(gradientEdit->Properties(), &ColorPickerGradientProperties::snapToMGrid, this,	&ColorPicker::snapToMGrid);	
	// Hatch
	connect(hatchEdit, &ColorPickerHatch::hatchChanged, this, &ColorPicker::updateHatch);
	connect(hatchEdit, &ColorPickerHatch::sizeChanged, this, &ColorPicker::updateSize);
	// Pattern
	connect(patternEdit, &ColorPickerPattern::patternChanged, this, &ColorPicker::updatePattern);
	connect(patternEdit, &ColorPickerPattern::sizeChanged, this, &ColorPicker::updateSize);

}

/* ********************************************************************************* *
 *
 * Members
 *
 * ********************************************************************************* */

void ColorPicker::setGeneralData(const CPGeneralData& data)
{
	m_generalData = data;

	QSignalBlocker sigOverprint(comboOverprint);
	comboOverprint->setCurrentIndex(m_generalData.overprint ? 1 : 0);
}

const CPGeneralData& ColorPicker::generalData() const
{
	return m_generalData;
}

void ColorPicker::setColorData(const CPColorData& data)
{
	QSignalBlocker sig(colorEdit);
	colorEdit->setColorData(data);
}

const CPColorData& ColorPicker::colorData() const
{
	return colorEdit->colorData();
}

void ColorPicker::setGradientData(const CPGradientData& data)
{
	QSignalBlocker sig(gradientEdit);
	gradientEdit->setGradientData(data);
}

const CPGradientData& ColorPicker::gradientData() const
{
	return gradientEdit->gradientData();
}

void ColorPicker::setGradientVectorData(const CPGradientVectorData& data)
{
	QSignalBlocker sig(gradientEdit);
	gradientEdit->setGradientVectorData(data);
}

const CPGradientVectorData& ColorPicker::gradientVectorData() const
{
	return gradientEdit->gradientVectorData();
}

void ColorPicker::setGradientMeshData(const CPColorData&  data)
{
	QSignalBlocker sig(gradientEdit);
	gradientEdit->setGradientMeshData(data);
}

const CPColorData& ColorPicker::gradientMeshData() const
{
	return gradientEdit->gradientMeshData();
}

void ColorPicker::setPatternData(const CPPatternData& data)
{
	QSignalBlocker sig(patternEdit);
	patternEdit->setPatternData(data);
}

const CPPatternData& ColorPicker::patternData() const
{
	return patternEdit->patternData();
}

void ColorPicker::setHatchData(const CPHatchData& data)
{
	QSignalBlocker sig(hatchEdit);
	hatchEdit->setHatchData(data);
}

const CPHatchData& ColorPicker::hatchData() const
{
	return hatchEdit->hatchData();
}

void ColorPicker::setColorButton(ColorButton *button)
{
	if (m_colorButton != button)
		gradientEdit->disableGradientEditProperties();

	m_colorButton = button;

	if (m_colorButton)
		m_colorButton->installEventFilter(this);
}

ColorButton *ColorPicker::colorButton()
{
	return m_colorButton;
}

QString ColorPicker::toolTipText() const
{
	switch (m_mode)
	{
	default:
	case Mode::Solid:
		return colorEdit->toolTipText();
		break;
	case Mode::Gradient:
		return gradientEdit->toolTipText();
		break;
	case Mode::Hatch:
		return hatchEdit->toolTipText();
		break;
	case Mode::Pattern:
		return patternEdit->toolTipText();
		break;
	}

	return QString();
}

void ColorPicker::setColorList(const ColorList &list)
{
	colorList = list;

	if (checkboxUsedColors->isChecked() && m_doc != nullptr)
		m_doc->getUsedColors(colorList);

	colorEdit->setColorList(colorList, true);
	gradientEdit->setColorList(colorList);
	hatchEdit->setColorList(colorList);
}

void ColorPicker::setGradientList(const GradientList &list)
{
	gradientEdit->setGradientList(list);
}

void ColorPicker::setPatternList(QHash<QString, ScPattern> *docPatterns)
{
	patternEdit->setPatternList(docPatterns);
}

PageItem *ColorPicker::currentItemFromSelection()
{
	PageItem *currentItem = nullptr;
	if (m_doc)
		currentItem = m_doc->m_Selection->itemAt(0);

	return currentItem;
}

bool ColorPicker::eventFilter(QObject *object, QEvent *event)
{
	const ColorButton *colButton = qobject_cast<ColorButton*>(object);
	if (!colButton)
		return false;

	if (colButton == m_colorButton)
	{
		switch (event->type())
		{
		case QEvent::EnabledChange:
		case QEvent::Hide:
		case QEvent::Close:
			gradientEdit->disableGradientEditProperties();
			closeParent();
			return true;
			break;
		default:
			return false;
		}
	}

	return false;
}

void ColorPicker::initContextMenu()
{
	QVBoxLayout *layoutMenu = new QVBoxLayout();
	layoutMenu->addWidget(comboOverprint);
	layoutMenu->addWidget(checkboxUsedColors);

	QWidget *menu = new QWidget();
	menu->setLayout(layoutMenu);
	menu->adjustSize();

	contextMenu = new PopupMenu(menu);
	buttonMenu->setMenu(contextMenu);
	buttonMenu->setPopupMode(QToolButton::InstantPopup);	
}

Context ColorPicker::context() const
{
	return m_context;
}

void ColorPicker::setContext(Context config)
{
	if (m_context == config)
		return;

	m_context = config;

	setUpdatesEnabled(false);

	colorEdit->setContext(m_context);
	gradientEdit->setContext(m_context);
	hatchEdit->setContext(m_context);
	patternEdit->setContext(m_context);

	switch (m_context)
	{
	default:
	case Context::Simple:
	case Context::LineMask:
	case Context::Text:
	case Context::TextBackground:
	case Context::TextOutline:
		buttonSolid->setVisible(false);
		buttonGradient->setVisible(false);
		buttonHatch->setVisible(false);
		buttonPattern->setVisible(false);
		buttonMenu->setVisible(false);
		// menu
		checkboxUsedColors->setVisible(false);
		comboOverprint->setVisible(false);
		break;
	case Context::DropShadow:
		buttonSolid->setVisible(false);
		buttonGradient->setVisible(false);
		buttonHatch->setVisible(false);
		buttonPattern->setVisible(false);
		buttonMenu->setVisible(true);
		// menu
		checkboxUsedColors->setVisible(true);
		comboOverprint->setVisible(false);
		break;
	case Context::Fill:		
		buttonSolid->setVisible(true);
		buttonGradient->setVisible(true);
		buttonHatch->setVisible(true);
		buttonPattern->setVisible(true);
		buttonMenu->setVisible(true);
		// menu
		checkboxUsedColors->setVisible(true);
		comboOverprint->setVisible(true);
		break;
	case Context::FillMask:
		buttonSolid->setVisible(true);
		buttonGradient->setVisible(true);
		buttonHatch->setVisible(false);
		buttonPattern->setVisible(true);
		buttonMenu->setVisible(true);
		// menu
		checkboxUsedColors->setVisible(true);
		comboOverprint->setVisible(false);
		break;
	case Context::Line:
		buttonSolid->setVisible(true);
		buttonGradient->setVisible(true);
		buttonHatch->setVisible(false);
		buttonPattern->setVisible(true);
		buttonMenu->setVisible(true);
		// menu
		checkboxUsedColors->setVisible(true);
		comboOverprint->setVisible(true);
		break;
	}

	setUpdatesEnabled(true);

	updateSize();
}

Mode ColorPicker::mode() const
{
	return m_mode;
}

bool ColorPicker::gradientEditPropertiesEnabled() const
{
	return gradientEdit->gradientEditPropertiesEnabled() && gradientEdit->isVisible();
}

void ColorPicker::setGradientEditPropertiesEnabled(bool enabled)
{
	gradientEdit->setGradientEditPropertiesEnabled(enabled);
}

GradientEdit ColorPicker::gradientEditMode() const
{
	return gradientEdit->gradientEditMode();
}

int ColorPicker::type() const
{
	return m_type;
}

void ColorPicker::setType(int type)
{
	m_type = type;
}

/* ********************************************************************************* *
 *
 * Methods
 *
 * ********************************************************************************* */

void ColorPicker::loadLists()
{
	if (!m_doc)
		return;

	// colors
	setColorList(m_doc->PageColors);

	// gradients
	GradientList lGradient;
	lGradient.addGradients(m_doc->docGradients);
	setGradientList(lGradient);

	// patterns
	setPatternList(&m_doc->docPatterns);
}


void ColorPicker::unsetDoc()
{
	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc, SIGNAL(docChanged()), this, SLOT(handleSelectionChanged()));
	}

	setDoc(nullptr);
}

void ColorPicker::updateUiByMode(Mode mode)
{
	if (m_mode == mode)
		return;

	m_mode = mode;

	switch (m_mode)
	{
	case Mode::Solid:
		buttonSolid->setChecked(true);
		stack->setCurrentWidget(colorEdit);
		comboType->setVisible(false);

		break;
	case Mode::Gradient:
		buttonGradient->setChecked(true);
		stack->setCurrentWidget(gradientEdit);
		comboType->setVisible(true);

		break;
	case Mode::Hatch:
		buttonHatch->setChecked(true);
		stack->setCurrentWidget(hatchEdit);
		comboType->setVisible(false);

		break;
	case Mode::Pattern:
		buttonPattern->setChecked(true);
		stack->setCurrentWidget(patternEdit);
		comboType->setVisible(false);

		break;
	}

}

void ColorPicker::updateUiByType(int type)
{
	setType(type);

	// set UI mode
	if (m_context == Context::FillMask)
	{
		switch (type)
		{
		case GradMask_None:
			updateUiByMode(Mode::Solid);
			break;
		case GradMask_Linear:
		case GradMask_LinearLumAlpha:
		case GradMask_Radial:
		case GradMask_RadialLumAlpha:
			updateUiByMode(Mode::Gradient);
			break;
		case GradMask_Pattern:
		case GradMask_PatternInverted:
		case GradMask_PatternLumAlpha:
		case GradMask_PatternLumAlphaInverted:
			updateUiByMode(Mode::Pattern);
			break;
		}
	}
	else
	{
		switch (type)
		{
		case Gradient_None:
			updateUiByMode(Mode::Solid);
			break;
		case Gradient_Linear:
		case Gradient_Conical:
		case Gradient_Radial:
		case Gradient_Diamond:
		case Gradient_4Colors:
		case Gradient_Mesh:
		case Gradient_PatchMesh:
			updateUiByMode(Mode::Gradient);
			break;
		case Gradient_Pattern:
			updateUiByMode(Mode::Pattern);
			break;
		case Gradient_Hatch:
			updateUiByMode(Mode::Hatch);
			break;
		}
	}
}

void ColorPicker::updatePreview()
{
	buttonPreview->setToolTip(toolTipText());
	buttonPreview->setContext(context());
	buttonPreview->setDoc(m_doc);
	buttonPreview->setType(type());
	buttonPreview->setColorData(colorData());
	buttonPreview->setGradientData(gradientData());
	buttonPreview->setHatchData(hatchData());
	buttonPreview->setPatternData(patternData());
	buttonPreview->updatePreview();
}

void ColorPicker::hideEditedPatterns(QStringList names)
{
	patternEdit->hideEditedPatterns(names);
}

void ColorPicker::enablePatterns()
{
	bool enable = false;

	if (m_doc)
		enable = !m_doc->docPatterns.empty();

	buttonPattern->setEnabled(enable);

	if (!enable && (mode() == Mode::Pattern))
		updateUiByMode(Mode::Solid);

}

void ColorPicker::closeParent()
{
	QWidget *widget = static_cast<QWidget*>(parent());
	if (widget)
		widget->close();
}


/* ********************************************************************************* *
 *
 * Slots
 *
 * ********************************************************************************* */

void ColorPicker::setDoc(ScribusDoc *doc)
{
	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
		disconnect(m_doc->scMW()     , SIGNAL(UpdateRequest(int)), this, SLOT(handleUpdateRequest(int)));
	}

	m_doc = doc;
	colorEdit->setDoc(m_doc);
	gradientEdit->setDoc(m_doc);
	hatchEdit->setDoc(m_doc);
	patternEdit->setDoc(m_doc);

	if (m_doc)
	{
		loadLists();
		enablePatterns();

		// Connect automatic selection handling only if the context handles the items within the color picker
		if ( (context() == Context::Fill) || (context() == Context::FillMask) || (context() == Context::Line) || (context() == Context::LineMask
			|| context() == Context::Text || context() == Context::TextBackground || context() == Context::TextOutline))
		{
			connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
			connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
			connect(m_doc->scMW()     , SIGNAL(UpdateRequest(int)), this, SLOT(handleUpdateRequest(int)));
		}
	}
}

void ColorPicker::languageChange()
{
	colorEdit->languageChange();
	gradientEdit->languageChange();
	hatchEdit->languageChange();
	patternEdit->languageChange();

	int oldOverPrintComboIndex = comboOverprint->currentIndex();
	QSignalBlocker sigOverPrint(comboOverprint);
	comboOverprint->clear();
	comboOverprint->addItem( tr("Knockout") );
	comboOverprint->addItem( tr("Overprint") );
	comboOverprint->setCurrentIndex(oldOverPrintComboIndex);

	checkboxUsedColors->setText( tr("Display only used colors") );
}

void ColorPicker::iconSetChange()
{
	IconManager &iconManager = IconManager::instance();

	buttonSolid->setIcon( iconManager.loadIcon("colorpicker-solid"));
	buttonGradient->setIcon( iconManager.loadIcon("colorpicker-gradient"));
	buttonHatch->setIcon( iconManager.loadIcon("colorpicker-hatch"));
	buttonPattern->setIcon( iconManager.loadIcon("colorpicker-pattern"));
	buttonMenu->setIcon(iconManager.loadIcon("context-menu"));
}

void ColorPicker::unitChange()
{
	hatchEdit->unitChange();
	patternEdit->unitChange();
}

void ColorPicker::parentClosed()
{
	gradientEdit->disableGradientEditProperties();
}

void ColorPicker::endPatchAdd()
{
	gradientEdit->Properties()->endPAddButton();
}

void ColorPicker::enableButtonRemovePatch(bool enable)
{
	gradientEdit->Properties()->changebuttonRemovePatch(enable);
}

void ColorPicker::enableMeshColor(bool enable)
{
	gradientEdit->enableMeshColor(enable);
}

void ColorPicker::toggleColorLists()
{
	if (m_doc)
		loadLists();

	emit colorListChanged();
}

void ColorPicker::updateColor()
{
	setType(colorEdit->type());
	updatePreview();
	emit changed();
	emit colorChanged();
}

void ColorPicker::updateGradient()
{
	setType(gradientEdit->type());
	updatePreview();
	emit changed();
	emit gradientChanged();
}

void ColorPicker::updateGradientVector()
{
	setType(gradientEdit->type());
	emit gradientVectorChanged();
}

void ColorPicker::updateGradientMesh()
{
	setType(gradientEdit->type());
	emit gradientMeshChanged();
}

void ColorPicker::updateHatch()
{
	setType(hatchEdit->type());
	updatePreview();
	emit changed();
	emit hatchChanged();
}

void ColorPicker::updatePattern()
{
	setType(patternEdit->type());
	updatePreview();
	emit changed();
	emit patternChanged();
}

void ColorPicker::updateSize()
{
	setUpdatesEnabled(false);

	stack->adjustSize();
	adjustSize();

	QWidget *parentWidget = qobject_cast<QWidget*>(parent());
	if (parentWidget)
		parentWidget->adjustSize();

	setUpdatesEnabled(true);

	emit sizeChanged();
}

void ColorPicker::updateOverPrint(int val)
{
	if (!m_doc)
		return;

	bool setter = (val == 0) ? false : true;
	m_generalData.overprint = val;
	m_doc->itemSelection_SetOverprint(setter);
}

void ColorPicker::changedTab()
{
	Mode fillMode = static_cast<Mode>(buttonGroup->checkedId());
	updateUiByMode(fillMode);

	switch (fillMode)
	{
	case Mode::Solid:
		updateColor();
		gradientEdit->disableGradientEditProperties();
		break;
	case Mode::Gradient:
		updateGradient();
		updateGradientMesh();
		gradientEdit->handleGradientEditProperties();
		break;
	case Mode::Hatch:
		updateHatch();
		gradientEdit->disableGradientEditProperties();
		break;
	case Mode::Pattern:
		updatePattern();
		gradientEdit->disableGradientEditProperties();
		break;
	}

}

void ColorPicker::handleSelectionChanged()
{
	if (m_doc)
		enablePatterns();
}

void ColorPicker::handleUpdateRequest(int updateFlags)
{
	if (updateFlags & reqColorsUpdate)
		loadLists();
}

/* ********************************************************************************* *
 *
 * Events
 *
 * ********************************************************************************* */

void ColorPicker::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
		languageChange();

	QWidget::changeEvent(e);
}
