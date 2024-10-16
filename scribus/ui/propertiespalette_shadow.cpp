/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "propertiespalette_shadow.h"

#if defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "commonstrings.h"
#include "pageitem.h"
#include "scribus.h"
#include "scribusapp.h"
#include "scribusdoc.h"
#include "selection.h"
#include "units.h"
#include "iconmanager.h"

PropertiesPalette_Shadow::PropertiesPalette_Shadow( QWidget* parent)
	: QWidget(parent)
{
	setupUi(this);

	switchHasShadow->setChecked(false);

	numberXOffset->setNewUnit(0);
	numberXOffset->setDecimals(2);
	numberXOffset->setMinimum(-200.00);
	numberXOffset->setMaximum(200.00);
	numberXOffset->setValue(5.0);

	numberYOffset->setNewUnit(0);
	numberYOffset->setDecimals(2);
	numberYOffset->setMinimum(-200.00);
	numberYOffset->setMaximum(200.00);
	numberYOffset->setValue(5.0);

	numberBlurRadius->setNewUnit(0);
	numberBlurRadius->setDecimals(2);
	numberBlurRadius->setMinimum(-200.00);
	numberBlurRadius->setMaximum(200.00);
	numberBlurRadius->setValue(2.0);

	buttonColor->setContext(Context::DropShadow);
	buttonColor->setMenuContextType(ColorButton::Floating);

	numberBlurRadius->setNewUnit(7);
	numberOpacity->setDecimals(1);
	numberOpacity->setMinimum(0.0);
	numberOpacity->setMaximum(100.0);
	numberOpacity->setValue(100.0);

	QStringList modes;
	modes << tr( "Normal");
	comboboxBlendMode->addItems(modes);

	buttonEraseShadow->setCheckable(true);
	buttonInheritOpacity->setCheckable(true);

	languageChange();
	iconSetChange();

	connectSlots();
	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(ScQApp, SIGNAL(localeChanged()), this, SLOT(localeChange()));
	connect(ScQApp, SIGNAL(labelVisibilityChanged(bool)), this, SLOT(toggleLabelVisibility(bool)));
}

void PropertiesPalette_Shadow::setMainWindow(ScribusMainWindow* mw)
{
	m_ScMW = mw;
//	connect(m_ScMW, SIGNAL(UpdateRequest(int)), this, SLOT(handleUpdateRequest(int)));
}

void PropertiesPalette_Shadow::setDoc(ScribusDoc *d)
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
	double maxXYWHVal =  200 * m_unitRatio;
	double minXYVal   = -200 * m_unitRatio;

	m_haveDoc = true;
	m_haveItem = false;

	disconnectSlots();

	numberXOffset->setNewUnit(m_unitIndex);
	numberXOffset->setDecimals(precision);
	numberXOffset->setMinimum(minXYVal);
	numberXOffset->setMaximum(maxXYWHVal);
	numberXOffset->setValue(minXYVal);

	numberYOffset->setNewUnit(m_unitIndex);
	numberYOffset->setDecimals(precision);
	numberYOffset->setMinimum(minXYVal);
	numberYOffset->setMaximum(maxXYWHVal);
	numberYOffset->setValue(minXYVal);

	numberBlurRadius->setNewUnit(m_unitIndex);
	numberBlurRadius->setDecimals(precision);
	numberBlurRadius->setMinimum(0.0);
	numberBlurRadius->setMaximum(200.00);
	numberBlurRadius->setValue(5);

	numberOpacity->setDecimals(0);
	numberOpacity->setMinimum(0);
	numberOpacity->setMaximum(100);
	numberOpacity->setValue(100);

	buttonColor->setDoc(m_doc);

//	updateColorList();
	connectSlots();
	setEnabled(true);

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));

}

void PropertiesPalette_Shadow::connectSlots()
{
	connect(numberXOffset, &ScrSpinBox::valueChanged, this, &PropertiesPalette_Shadow::handleNewValues);
	connect(numberYOffset, &ScrSpinBox::valueChanged, this, &PropertiesPalette_Shadow::handleNewValues);
	connect(numberBlurRadius, &ScrSpinBox::valueChanged, this, &PropertiesPalette_Shadow::handleNewValues);
	connect(numberOpacity, &ScrSpinBox::valueChanged, this, &PropertiesPalette_Shadow::handleNewValues);

	connect(switchHasShadow, &Switch::toggled, this, &PropertiesPalette_Shadow::handleNewValues);

	connect(comboboxBlendMode, &QComboBox::currentIndexChanged, this, &PropertiesPalette_Shadow::handleNewValues);

	connect(buttonColor, &ColorButton::colorChanged, this, &PropertiesPalette_Shadow::handleNewValues);
	connect(buttonEraseShadow, &QToolButton::toggled, this, &PropertiesPalette_Shadow::handleNewValues);
	connect(buttonInheritOpacity, &QToolButton::toggled, this, &PropertiesPalette_Shadow::handleInheritOpacity);
}

void PropertiesPalette_Shadow::disconnectSlots()
{
	disconnect(numberXOffset, &ScrSpinBox::valueChanged, this, &PropertiesPalette_Shadow::handleNewValues);
	disconnect(numberYOffset, &ScrSpinBox::valueChanged, this, &PropertiesPalette_Shadow::handleNewValues);
	disconnect(numberBlurRadius, &ScrSpinBox::valueChanged, this, &PropertiesPalette_Shadow::handleNewValues);
	disconnect(numberOpacity, &ScrSpinBox::valueChanged, this, &PropertiesPalette_Shadow::handleNewValues);

	disconnect(switchHasShadow, &Switch::toggled, this, &PropertiesPalette_Shadow::handleNewValues);

	disconnect(comboboxBlendMode, &QComboBox::currentIndexChanged, this, &PropertiesPalette_Shadow::handleNewValues);

	disconnect(buttonColor, &ColorButton::colorChanged, this, &PropertiesPalette_Shadow::handleNewValues);
	disconnect(buttonEraseShadow, &QToolButton::toggled, this, &PropertiesPalette_Shadow::handleNewValues);
	disconnect(buttonInheritOpacity, &QToolButton::toggled, this, &PropertiesPalette_Shadow::handleInheritOpacity);
}

void PropertiesPalette_Shadow::unsetDoc()
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
	buttonColor->unsetDoc();
	setEnabled(false);
}

void PropertiesPalette_Shadow::unsetItem()
{
	m_haveItem = false;
	m_item     = nullptr;
	handleSelectionChanged();
}

//void PropertiesPalette_Shadow::handleUpdateRequest(int updateFlags)
//{
//	if (updateFlags & reqColorsUpdate)
//		updateColorList();
//}

PageItem* PropertiesPalette_Shadow::currentItemFromSelection()
{
	PageItem *currentItem = nullptr;
	if (m_doc)
	{
		if (m_doc->m_Selection->count() > 1)
			currentItem = m_doc->m_Selection->itemAt(0);
		else if (m_doc->m_Selection->count() == 1)
			currentItem = m_doc->m_Selection->itemAt(0);
	}
	return currentItem;
}

void PropertiesPalette_Shadow::setCurrentItem(PageItem *item)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if (!m_doc)
		setDoc(item->doc());
	m_haveItem = false;
	m_item = item;

	disconnectSlots();
	switchHasShadow->setChecked(item->hasSoftShadow());
	numberXOffset->setValue(item->softShadowXOffset() * m_unitRatio);
	numberYOffset->setValue(item->softShadowYOffset() * m_unitRatio);
	numberBlurRadius->setValue(item->softShadowBlurRadius() * m_unitRatio);
	numberOpacity->setValue(qRound(100 - (item->softShadowOpacity() * 100)));
	comboboxBlendMode->setCurrentIndex(item->softShadowBlendMode());
	buttonEraseShadow->setChecked(item->softShadowErasedByObject());
	buttonInheritOpacity->setChecked(item->softShadowHasObjectTransparency());

	double opacity = item->softShadowHasObjectTransparency() ? item->fillTransparency() : item->softShadowOpacity();
	buttonColor->setColor(item->softShadowColor(), item->softShadowShade(), opacity);
	buttonColor->updatePreview();
	buttonColor->updateFloatingContext();

	labelErase->setVisible(item->itemType() != PageItem::Line);

	m_haveItem = true;
//	updateSpinBoxConstants();

	connectSlots();
}

void PropertiesPalette_Shadow::handleSelectionChanged()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem* currItem = currentItemFromSelection();
	if (currItem)
		setCurrentItem(currItem);
	updateGeometry();
}

void PropertiesPalette_Shadow::unitChange()
{
	if (!m_haveDoc)
		return;
	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();

	disconnectSlots();

	numberXOffset->setNewUnit(m_unitIndex);
	numberYOffset->setNewUnit(m_unitIndex);
	numberBlurRadius->setNewUnit(m_unitIndex);

	connectSlots();
}

void PropertiesPalette_Shadow::localeChange()
{
//	numberXOffset->localeChange();
//	numberYOffset->localeChange();
	//	numberBlurRadius->localeChange();
}

void PropertiesPalette_Shadow::toggleLabelVisibility(bool v)
{
	labelColor->setLabelVisibility(v);
	labelBlendmode->setLabelVisibility(v);
	labelHasShadow->setLabelVisibility(v);
	labelErase->setLabelVisibility(v);
}

void PropertiesPalette_Shadow::handleNewValues()
{
	if (!m_haveItem || !m_haveDoc)
		return;
	double x = numberXOffset->value() / m_unitRatio;
	double y = numberYOffset->value() / m_unitRatio;
	double r = numberBlurRadius->value() / m_unitRatio;
	QString color = buttonColor->colorData().Name;
	if (color == CommonStrings::tr_NoneColor)
		color = CommonStrings::None;
	int b = comboboxBlendMode->currentIndex();
	double o = (100 - numberOpacity->value()) / 100.0;
	int s = buttonColor->colorData().Shade;

	m_doc->itemSelection_SetSoftShadow(switchHasShadow->isChecked(), color, x, y, r, s, o, b, buttonEraseShadow->isChecked(), buttonInheritOpacity->isChecked());

	buttonColor->updatePreview();
}

void PropertiesPalette_Shadow::handleInheritOpacity()
{
		numberOpacity->setEnabled(!buttonInheritOpacity->isChecked());
		handleNewValues();
}

void PropertiesPalette_Shadow::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void PropertiesPalette_Shadow::languageChange()
{
	labelXOffset->setText(tr( "X:"));
	labelYOffset->setText(tr( "Y:"));
	labelBlurRadius->setText(tr( "Radius:"));
	labelColor->setText(tr( "Color"));
	labelOpacity->setText(tr( "Opacity:"));
	comboboxBlendMode->languageChange();
	labelBlendmode->setText(tr( "Blendmode"));
	labelErase->setText(tr( "Erase Fill"));

	buttonEraseShadow->setToolTip(tr( "Content covers Drop Shadow"));
	buttonInheritOpacity->setToolTip(tr( "Inherit Object Transparency"));
}

void PropertiesPalette_Shadow::iconSetChange()
{
	IconManager &im = IconManager::instance();

	QIcon iconErase;
	iconErase.addPixmap(im.loadPixmap("erase-shadow-on"), QIcon::Normal, QIcon::On);
	iconErase.addPixmap(im.loadPixmap("erase-shadow-off"), QIcon::Normal, QIcon::Off);
	buttonEraseShadow->setIcon(iconErase);

	QIcon iconTransparency;
	iconTransparency.addPixmap(im.loadPixmap("inherit-opacity-on"), QIcon::Normal, QIcon::On);
	iconTransparency.addPixmap(im.loadPixmap("inherit-opacity-off"), QIcon::Normal, QIcon::Off);
	buttonInheritOpacity->setIcon(iconTransparency);

}

void PropertiesPalette_Shadow::updateSpinBoxConstants()
{
	if (!m_haveDoc)
		return;
	if (m_doc->m_Selection->count() == 0)
		return;
}
