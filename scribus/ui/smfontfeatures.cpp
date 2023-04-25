/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smfontfeatures.h"
#include "util.h"

#include "fonts/fontfeatures.h"

#include <QAction>
#include <QListWidget>
#include <QListWidgetItem>
#include <QSignalBlocker>

SMFontFeatures::SMFontFeatures(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);
	styleSetsMenu = new StylisticSetsMenu(this);
	for (int i = 1; i <= 20 ; i++)
	{
		QAction *action = new QAction("set " + QString::number(i), styleSetsMenu);
		action->setCheckable(true);
		styleSetsMenu->addAction(action);
	}
	stylisticSetsButton->setMenu(styleSetsMenu);

	//capture changed signal
	connect(this->CommonCheck, SIGNAL(toggled(bool)), this, SLOT(slotChange()));
	connect(this->ContextualCheck, SIGNAL(toggled(bool)), this, SLOT(slotChange()));
	connect(this->DiscretionaryCheck, SIGNAL(toggled(bool)), this, SLOT(slotChange()));
	connect(this->HistoricalCheck, SIGNAL(toggled(bool)), this, SLOT(slotChange()));
	connect(this->SlashedZeroCheck, SIGNAL(toggled(bool)), this, SLOT(slotChange()));
	connect(this->capitalsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChange()));
	connect(this->positionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChange()));
	connect(this->numeralComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChange()));
	connect(this->widthComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChange()));
	connect(this->fractionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChange()));
	connect(this->styleSetsMenu, SIGNAL(triggered(QAction*)), this, SLOT(slotChange()));
}

void SMFontFeatures::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
		languageChange();
	else
		QWidget::changeEvent(e);
}

void SMFontFeatures::languageChange()
{
	QSignalBlocker blocker01(this->CommonCheck);
	QSignalBlocker blocker02(this->ContextualCheck);
	QSignalBlocker blocker03(this->DiscretionaryCheck);
	QSignalBlocker blocker04(this->HistoricalCheck);
	QSignalBlocker blocker05(this->SlashedZeroCheck);
	QSignalBlocker blocker06(this->capitalsComboBox);
	QSignalBlocker blocker07(this->positionComboBox);
	QSignalBlocker blocker08(this->numeralComboBox);
	QSignalBlocker blocker09(this->widthComboBox);
	QSignalBlocker blocker10(this->fractionComboBox);
	QSignalBlocker blocker11(this->styleSetsMenu);

	quint64 capitalFeatures  = comboboxFeatures(capitalsComboBox);
	quint64 positionFeatures = comboboxFeatures(positionComboBox);
	quint64 numeralFeatures  = comboboxFeatures(numeralComboBox);
	quint64 widthFeatures    = comboboxFeatures(widthComboBox);
	quint64 fractionFeatures = comboboxFeatures(fractionComboBox);

	int oldCapitalIndex  = capitalsComboBox->currentIndex();
	int oldPositionIndex = positionComboBox->currentIndex();
	int oldNumeralIndex  = numeralComboBox->currentIndex();
	int oldWidthIndex    = widthComboBox->currentIndex();
	int oldFractionIndex = fractionComboBox->currentIndex();

	retranslateUi(this);

	setupCapitalCombo(capitalFeatures);
	setupPositionCombo(positionFeatures);
	setupNumeralStyleCombo(numeralFeatures);
	setupNumeralWidthCombo(widthFeatures);
	setupNumeralFractionCombo(fractionFeatures);

	capitalsComboBox->setCurrentIndex(oldCapitalIndex);
	positionComboBox->setCurrentIndex(oldPositionIndex);
	numeralComboBox->setCurrentIndex(oldNumeralIndex);
	widthComboBox->setCurrentIndex(oldWidthIndex);
	fractionComboBox->setCurrentIndex(oldFractionIndex);
}

quint64 SMFontFeatures::comboboxFeatures(QComboBox* combo) const
{
	quint64 features = 0;
	for (int i = 0; i < combo->count(); ++i)
	{
		QVariant varVal = combo->itemData(i);
		quint64 value = varVal.toULongLong();
		features |= value;
	}
	return features;
}

void SMFontFeatures::setFontFeatures(const QString& s, QStringList fontFeaturesList)
{
	disconnectSignals();
	m_hasParent = false;
	m_pValue.clear();

	resetFontFeatures();
	enableFontFeatures(fontFeaturesList);
	SlashedZeroCheck->setEnabled(true);

	QStringList fontFeatures = s.split(',');
	quint64 featureFlags = FontFeatures::textFlagsFromList(fontFeatures);

	// Ligatures
	if ((featureFlags & FontFeatures::CommonLigatures) == 0)
		CommonCheck->setChecked(false);
	if ((featureFlags & FontFeatures::ContextualLigatures) == 0)
		ContextualCheck->setChecked(false);
	if (featureFlags & FontFeatures::DiscretionaryLigatures)
		DiscretionaryCheck->setChecked(true);
	if (featureFlags & FontFeatures::HistoricalLigatures)
		HistoricalCheck->setChecked(true);

	// Position comboBox
	if (featureFlags & FontFeatures::GlyphPositionMask)
		setCurrentComboItemFromData(positionComboBox, featureFlags & FontFeatures::GlyphPositionMask);

	// Capitals ComboBox
	if (featureFlags & FontFeatures::CapsMask)
		setCurrentComboItemFromData(capitalsComboBox, featureFlags & FontFeatures::CapsMask);

	// Numerals style
	if (featureFlags & FontFeatures::NumeralStyleMask)
		setCurrentComboItemFromData(numeralComboBox, featureFlags & FontFeatures::NumeralStyleMask);

	// Numerals width
	if (featureFlags & FontFeatures::NumeralWidthMask)
		setCurrentComboItemFromData(widthComboBox, featureFlags & FontFeatures::NumeralWidthMask);

	// Numerals Fraction
	if (featureFlags & FontFeatures::NumeralFractionsMask)
		setCurrentComboItemFromData(fractionComboBox, featureFlags & FontFeatures::NumeralFractionsMask);

	// Numerals Zero
	if (featureFlags & FontFeatures::SlashedZero)
		SlashedZeroCheck->setChecked(true);

	// Style Sets
	if (featureFlags & FontFeatures::StyleSet01)
		styleSetsMenu->actions().at(0)->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet02)
		styleSetsMenu->actions().at(1)->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet03)
		styleSetsMenu->actions().at(2)->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet04)
		styleSetsMenu->actions().at(3)->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet05)
		styleSetsMenu->actions().at(4)->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet06)
		styleSetsMenu->actions().at(5)->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet07)
		styleSetsMenu->actions().at(6)->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet08)
		styleSetsMenu->actions().at(7)->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet09)
		styleSetsMenu->actions().at(8)->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet10)
		styleSetsMenu->actions().at(9)->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet11)
		styleSetsMenu->actions().at(10)->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet12)
		styleSetsMenu->actions().at(11)->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet13)
		styleSetsMenu->actions().at(12)->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet14)
		styleSetsMenu->actions().at(13)->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet15)
		styleSetsMenu->actions().at(14)->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet16)
		styleSetsMenu->actions().at(15)->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet17)
		styleSetsMenu->actions().at(16)->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet18)
		styleSetsMenu->actions().at(17)->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet19)
		styleSetsMenu->actions().at(18)->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet20)
		styleSetsMenu->actions().at(19)->setChecked(true);

	if (numeralComboBox->currentData().toULongLong() == FontFeatures::OldStyleNumerals)
	{
		SlashedZeroCheck->setChecked(false);
		SlashedZeroCheck->setEnabled(false);
	}
}

void SMFontFeatures::setFontFeatures(const QString& val, QStringList fontFeatures, bool isParentVal)
{
	disconnectSignals();
	m_hasParent = true;
	m_pValue = val;

	if (isParentVal)
		setFontFeatures(val, fontFeatures);
	connectSignals();
}

QString SMFontFeatures::fontFeatures()
{
	quint64 featureFlags = 0;

	// Ligatures
	if (CommonCheck->isChecked())
		featureFlags |= FontFeatures::CommonLigatures;
	if (ContextualCheck->isChecked())
		featureFlags |= FontFeatures::ContextualLigatures;
	if (DiscretionaryCheck->isChecked())
		featureFlags |= FontFeatures::DiscretionaryLigatures;
	if (HistoricalCheck->isChecked())
		featureFlags |= FontFeatures::HistoricalLigatures;

	//Position
	QVariant varVal = positionComboBox->currentData();
	quint64  value = varVal.toULongLong();
	featureFlags |= value;

	//Capitals
	varVal = capitalsComboBox->currentData();
	value = varVal.toULongLong();
	featureFlags |= value;

	//Numerals style
	varVal = numeralComboBox->currentData();
	value = varVal.toULongLong();
	featureFlags |= value;

	//Numerals width
	varVal = widthComboBox->currentData();
	value = varVal.toULongLong();
	featureFlags |= value;

	//Numerals Fraction
	varVal = fractionComboBox->currentData();
	value = varVal.toULongLong();
	featureFlags |= value;

	// Numerals Zero
	if (SlashedZeroCheck->isChecked())
		featureFlags |= FontFeatures::SlashedZero;

	// Stylistic sets
	if (styleSetsMenu->actions().at(0)->isChecked())
		featureFlags |= FontFeatures::StyleSet01;
	if (styleSetsMenu->actions().at(1)->isChecked())
		featureFlags |= FontFeatures::StyleSet02;
	if (styleSetsMenu->actions().at(2)->isChecked())
		featureFlags |= FontFeatures::StyleSet03;
	if (styleSetsMenu->actions().at(3)->isChecked())
		featureFlags |= FontFeatures::StyleSet04;
	if (styleSetsMenu->actions().at(4)->isChecked())
		featureFlags |= FontFeatures::StyleSet05;
	if (styleSetsMenu->actions().at(5)->isChecked())
		featureFlags |= FontFeatures::StyleSet06;
	if (styleSetsMenu->actions().at(6)->isChecked())
		featureFlags |= FontFeatures::StyleSet07;
	if (styleSetsMenu->actions().at(7)->isChecked())
		featureFlags |= FontFeatures::StyleSet08;
	if (styleSetsMenu->actions().at(8)->isChecked())
		featureFlags |= FontFeatures::StyleSet09;
	if (styleSetsMenu->actions().at(9)->isChecked())
		featureFlags |= FontFeatures::StyleSet10;
	if (styleSetsMenu->actions().at(10)->isChecked())
		featureFlags |= FontFeatures::StyleSet11;
	if (styleSetsMenu->actions().at(11)->isChecked())
		featureFlags |= FontFeatures::StyleSet12;
	if (styleSetsMenu->actions().at(12)->isChecked())
		featureFlags |= FontFeatures::StyleSet13;
	if (styleSetsMenu->actions().at(13)->isChecked())
		featureFlags |= FontFeatures::StyleSet14;
	if (styleSetsMenu->actions().at(14)->isChecked())
		featureFlags |= FontFeatures::StyleSet15;
	if (styleSetsMenu->actions().at(15)->isChecked())
		featureFlags |= FontFeatures::StyleSet16;
	if (styleSetsMenu->actions().at(16)->isChecked())
		featureFlags |= FontFeatures::StyleSet17;
	if (styleSetsMenu->actions().at(17)->isChecked())
		featureFlags |= FontFeatures::StyleSet18;
	if (styleSetsMenu->actions().at(18)->isChecked())
		featureFlags |= FontFeatures::StyleSet19;
	if (styleSetsMenu->actions().at(19)->isChecked())
		featureFlags |= FontFeatures::StyleSet20;

	QStringList font_features = FontFeatures::textFeaturesFromFlags(featureFlags);
	return font_features.join(",");
}

void SMFontFeatures::setParentValue(const QString& val)
{
	m_hasParent = true;
	m_pValue = val;
}

bool SMFontFeatures::useParentValue()
{
	bool ret = m_useParentValue;
	m_useParentValue = false;
	return ret;
}

void SMFontFeatures::connectSignals()
{
	connect(ContextualCheck, SIGNAL(clicked()), this, SLOT(slotContextualCheck()));
	connect(CommonCheck, SIGNAL(clicked()), this, SLOT(slotCommonCheck()));
	connect(DiscretionaryCheck, SIGNAL(clicked()), this, SLOT(slotDiscretionaryCheck()));
	connect(HistoricalCheck, SIGNAL(clicked()), this, SLOT(slotHistoricalCheck()));
	connect(capitalsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotCapitalsComboBox()));
	connect(positionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotPositionComboBox()));
	connect(numeralComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotNumeralComboBox()));
	connect(widthComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotWidthComboBox()));
	connect(fractionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotFractionComboBox()));
	connect(SlashedZeroCheck, SIGNAL(clicked()), this, SLOT(slotSlashedZeroCheck()));
	connect(styleSetsMenu, SIGNAL(triggered(QAction*)), this, SLOT(slotSetMenu()));
}

void SMFontFeatures::disconnectSignals()
{
	disconnect(ContextualCheck, SIGNAL(clicked()), this, SLOT(slotContextualCheck()));
	disconnect(CommonCheck, SIGNAL(clicked()), this, SLOT(slotCommonCheck()));
	disconnect(DiscretionaryCheck, SIGNAL(clicked()), this, SLOT(slotDiscretionaryCheck()));
	disconnect(HistoricalCheck, SIGNAL(clicked()), this, SLOT(slotHistoricalCheck()));
	disconnect(capitalsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotCapitalsComboBox()));
	disconnect(positionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotPositionComboBox()));
	disconnect(numeralComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotNumeralComboBox()));
	disconnect(widthComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotWidthComboBox()));
	disconnect(fractionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotFractionComboBox()));
	disconnect(SlashedZeroCheck, SIGNAL(clicked()), this, SLOT(slotSlashedZeroCheck()));
	disconnect(styleSetsMenu, SIGNAL(triggered(QAction*)), this, SLOT(slotSetMenu()));
}

void SMFontFeatures::hideAllFontFeatures()
{
	statusLabel->hide();
	ligaturesGroupBox->hide();
	CommonCheck->setChecked(false);
	CommonCheck->setDisabled(true);
	DiscretionaryCheck->setChecked(false);
	DiscretionaryCheck->setDisabled(true);
	ContextualCheck->setDisabled(true);
	HistoricalCheck->setDisabled(true);

	capitalsLabel->hide();
	capitalsComboBox->hide();
	setupCapitalCombo(FontFeatures::NoFeatures);

	positionLabel->hide();
	positionComboBox->hide();
	setupPositionCombo(FontFeatures::NoFeatures);
	capitalsGroupBox->hide();

	numeralsGroupBox->hide();
	styleLabel->hide();
	numeralComboBox->hide();
	setupNumeralStyleCombo(FontFeatures::NoFeatures);

	widthLabel->hide();
	widthComboBox->hide();
	setupNumeralWidthCombo(FontFeatures::NoFeatures);

	fractionLabel->hide();
	fractionComboBox->hide();
	setupNumeralFractionCombo(FontFeatures::NoFeatures);

	SlashedZeroCheck->hide();

	stylisticSetsLabel->hide();
	stylisticSetsButton->hide();
	for (int i = 0; i < styleSetsMenu->actions().count(); i++)
	{
		styleSetsMenu->actions().at(i)->setVisible(false);
	}
}

void SMFontFeatures::enableFontFeatures(const QStringList& fontFeatures)
{
	quint64 featureFlags = FontFeatures::fontFlagsFromList(fontFeatures);

	hideAllFontFeatures();

	// Capitals
	setupCapitalCombo(featureFlags);
	capitalsComboBox->setVisible(capitalsComboBox->count() > 1);
	capitalsLabel->setVisible(capitalsComboBox->count() > 1);

	// Positions
	setupPositionCombo(featureFlags);
	positionComboBox->setVisible(positionComboBox->count() > 1);
	positionLabel->setVisible(positionComboBox->count() > 1);

	// Numeral Style
	setupNumeralStyleCombo(featureFlags);
	numeralComboBox->setVisible(numeralComboBox->count() > 1);
	styleLabel->setVisible(numeralComboBox->count() > 1);

	// Numeral Width
	setupNumeralWidthCombo(featureFlags);
	widthComboBox->setVisible(widthComboBox->count() > 1);
	widthLabel->setVisible(widthComboBox->count() > 1);

	// Numeral Fractions
	setupNumeralFractionCombo(featureFlags);
	fractionComboBox->setVisible(fractionComboBox->count() > 1);
	fractionLabel->setVisible(fractionComboBox->count() > 1);

	// Ligatures
	CommonCheck->setEnabled(featureFlags & FontFeatures::CommonLigatures);
	CommonCheck->setChecked(featureFlags & FontFeatures::CommonLigatures);

	ContextualCheck->setEnabled(featureFlags & FontFeatures::ContextualLigatures);
	ContextualCheck->setChecked(featureFlags & FontFeatures::ContextualLigatures);

	DiscretionaryCheck->setEnabled(featureFlags & FontFeatures::DiscretionaryLigatures);
	HistoricalCheck->setEnabled(featureFlags & FontFeatures::HistoricalLigatures);

	// Numerals Zero
	SlashedZeroCheck->setVisible(featureFlags & FontFeatures::SlashedZero);

	// Style Sets
	styleSetsMenu->actions().at(0)->setVisible(featureFlags & FontFeatures::StyleSet01);
	styleSetsMenu->actions().at(1)->setVisible(featureFlags & FontFeatures::StyleSet02);
	styleSetsMenu->actions().at(2)->setVisible(featureFlags & FontFeatures::StyleSet03);
	styleSetsMenu->actions().at(3)->setVisible(featureFlags & FontFeatures::StyleSet04);
	styleSetsMenu->actions().at(4)->setVisible(featureFlags & FontFeatures::StyleSet05);
	styleSetsMenu->actions().at(5)->setVisible(featureFlags & FontFeatures::StyleSet06);
	styleSetsMenu->actions().at(6)->setVisible(featureFlags & FontFeatures::StyleSet07);
	styleSetsMenu->actions().at(7)->setVisible(featureFlags & FontFeatures::StyleSet08);
	styleSetsMenu->actions().at(8)->setVisible(featureFlags & FontFeatures::StyleSet09);
	styleSetsMenu->actions().at(9)->setVisible(featureFlags & FontFeatures::StyleSet10);
	styleSetsMenu->actions().at(10)->setVisible(featureFlags & FontFeatures::StyleSet11);
	styleSetsMenu->actions().at(11)->setVisible(featureFlags & FontFeatures::StyleSet12);
	styleSetsMenu->actions().at(12)->setVisible(featureFlags & FontFeatures::StyleSet13);
	styleSetsMenu->actions().at(13)->setVisible(featureFlags & FontFeatures::StyleSet14);
	styleSetsMenu->actions().at(14)->setVisible(featureFlags & FontFeatures::StyleSet15);
	styleSetsMenu->actions().at(15)->setVisible(featureFlags & FontFeatures::StyleSet16);
	styleSetsMenu->actions().at(16)->setVisible(featureFlags & FontFeatures::StyleSet17);
	styleSetsMenu->actions().at(17)->setVisible(featureFlags & FontFeatures::StyleSet18);
	styleSetsMenu->actions().at(18)->setVisible(featureFlags & FontFeatures::StyleSet19);
	styleSetsMenu->actions().at(19)->setVisible(featureFlags & FontFeatures::StyleSet20);

	ligaturesGroupBox->setVisible(featureFlags & FontFeatures::LigaturesMask);
	capitalsGroupBox->setVisible(featureFlags & (FontFeatures::CapsMask | FontFeatures::GlyphPositionMask));
	
	quint64 numeralsMask = FontFeatures::NumeralStyleMask | FontFeatures::NumeralWidthMask | FontFeatures::NumeralFractionsMask;
	numeralsMask |= FontFeatures::SlashedZero;
	numeralsGroupBox->setVisible(featureFlags & numeralsMask);

	stylisticSetsLabel->setVisible(featureFlags & FontFeatures::StyleSetsMask);
	stylisticSetsButton->setVisible(featureFlags & FontFeatures::StyleSetsMask);

	if (ligaturesGroupBox->isHidden() && capitalsGroupBox->isHidden()
			&& numeralsGroupBox->isHidden() && stylisticSetsButton->isHidden())
	{
		statusLabel->show();
	}
}

void SMFontFeatures::setupCapitalCombo(quint64 featureFlags)
{
	QSignalBlocker blocker(capitalsComboBox);

	capitalsComboBox->clear();
	capitalsComboBox->addItem( tr("Default Capitals"), QVariant(0));

	if (featureFlags & FontFeatures::SmallCaps)
		capitalsComboBox->addItem(tr("Small Capitals"), QVariant(FontFeatures::SmallCaps));
	if (featureFlags & FontFeatures::SmallCapsFromCaps)
		capitalsComboBox->addItem(tr("Small Capitals from Capitals"), QVariant(FontFeatures::SmallCapsFromCaps));
	if ((featureFlags & FontFeatures::AllSmallCapsMask) == FontFeatures::AllSmallCapsMask)
		capitalsComboBox->addItem(tr("All Small Capitals"), QVariant(FontFeatures::AllSmallCapsMask));
	if (featureFlags & FontFeatures::PetiteCaps)
		capitalsComboBox->addItem(tr("Petite Capitals"), QVariant(FontFeatures::PetiteCaps));
	if (featureFlags & FontFeatures::PetiteCapsFromCaps)
		capitalsComboBox->addItem(tr("Petite Capitals from Capitals"), QVariant(FontFeatures::PetiteCapsFromCaps));
	if ((featureFlags & FontFeatures::AllPetiteCapsMask) == FontFeatures::AllPetiteCapsMask)
		capitalsComboBox->addItem(tr("All Petite Capitals"), QVariant(FontFeatures::AllPetiteCapsMask));
	if (featureFlags & FontFeatures::UnicaseCaps)
		capitalsComboBox->addItem(tr("Unicase"), QVariant(FontFeatures::UnicaseCaps));
	if (featureFlags & FontFeatures::TiltingCaps)
		capitalsComboBox->addItem(tr("Titling"), QVariant(FontFeatures::TiltingCaps));
}

void SMFontFeatures::setupPositionCombo(quint64 featureFlags)
{
	QSignalBlocker blocker(positionComboBox);

	positionComboBox->clear();
	positionComboBox->addItem( tr("Default Position"), QVariant(0));

	if (featureFlags & FontFeatures::Subscript)
		positionComboBox->addItem(tr("Subscript"), QVariant(FontFeatures::Subscript));
	if (featureFlags & FontFeatures::Superscript)
		positionComboBox->addItem(tr("Superscript"), QVariant(FontFeatures::Superscript));
	if (featureFlags & FontFeatures::Ordinals)
		positionComboBox->addItem(tr("Ordinals"), QVariant(FontFeatures::Ordinals));
}

void SMFontFeatures::setupNumeralStyleCombo(quint64 featureFlags)
{
	QSignalBlocker blocker(numeralComboBox);

	numeralComboBox->clear();
	numeralComboBox->addItem( tr("Default Numerals"), QVariant(0));

	if (featureFlags & FontFeatures::LiningNumerals)
		numeralComboBox->addItem(tr("Lining"), QVariant(FontFeatures::LiningNumerals));
	if (featureFlags & FontFeatures::OldStyleNumerals)
		numeralComboBox->addItem(tr("Old Style"), QVariant(FontFeatures::OldStyleNumerals));
}

void SMFontFeatures::setupNumeralWidthCombo(quint64 featureFlags)
{
	QSignalBlocker blocker(widthComboBox);

	widthComboBox->clear();
	widthComboBox->addItem( tr("Default Numeral Width"), QVariant(0));

	if (featureFlags & FontFeatures::ProportionalNumeralWidth)
		widthComboBox->addItem(tr("Proportional"), QVariant(FontFeatures::ProportionalNumeralWidth));
	if (featureFlags & FontFeatures::TabularNumeralWidth)
		widthComboBox->addItem(tr("Tabular"), QVariant(FontFeatures::TabularNumeralWidth));
}

void SMFontFeatures::setupNumeralFractionCombo(quint64 featureFlags)
{
	QSignalBlocker blocker(fractionComboBox);

	fractionComboBox->clear();
	fractionComboBox->addItem( tr("No Fractions"), QVariant(0));

	if (featureFlags & FontFeatures::DiagonalFractions)
		fractionComboBox->addItem(tr("Diagonal Fractions"), QVariant(FontFeatures::DiagonalFractions));
	if (featureFlags & FontFeatures::StackedFractions)
		fractionComboBox->addItem(tr("Stacked Fractions"), QVariant(FontFeatures::StackedFractions));
}

void SMFontFeatures::slotChange()
{
	emit changed();
}

void SMFontFeatures::slotContextualCheck()
{
	if (m_hasParent)
	{
		QFont f(font());
		f.setBold(true);
		ContextualCheck->setFont(f);
	}
}

void SMFontFeatures::slotCommonCheck()
{
	if (m_hasParent)
	{
		QFont f(font());
		f.setBold(true);
		CommonCheck->setFont(f);
	}
}

void SMFontFeatures::slotDiscretionaryCheck()
{
	if (m_hasParent)
	{
		QFont f(font());
		f.setBold(true);
		DiscretionaryCheck->setFont(f);
	}
}

void SMFontFeatures::slotHistoricalCheck()
{
	if (m_hasParent)
	{
		QFont f(font());
		f.setBold(true);
		HistoricalCheck->setFont(f);
	}
}

void SMFontFeatures::slotCapitalsComboBox()
{
	if (m_hasParent)
	{
		QFont f(font());
		f.setBold(true);
		capitalsComboBox->setFont(f);
	}
}

void SMFontFeatures::slotPositionComboBox()
{
	if (m_hasParent)
	{
		QFont f(font());
		f.setBold(true);
		positionComboBox->setFont(f);
	}
}

void SMFontFeatures::slotNumeralComboBox()
{
	if (m_hasParent)
	{
		QFont f(font());
		f.setBold(true);
		numeralComboBox->setFont(f);
	}

	// Old Style numerals do not support slashed zero
	bool oldStyleNumeral = (numeralComboBox->currentData().toULongLong() == FontFeatures::OldStyleNumerals);
	if (oldStyleNumeral)
		SlashedZeroCheck->setChecked(false);
	SlashedZeroCheck->setEnabled(!oldStyleNumeral);
}

void SMFontFeatures::slotWidthComboBox()
{
	if (m_hasParent)
	{
		QFont f(font());
		f.setBold(true);
		widthComboBox->setFont(f);
	}
}

void SMFontFeatures::slotFractionComboBox()
{
	if (m_hasParent)
	{
		QFont f(font());
		f.setBold(true);
		fractionComboBox->setFont(f);
	}
}

void SMFontFeatures::slotSlashedZeroCheck()
{
	if (m_hasParent)
	{
		QFont f(font());
		f.setBold(true);
		SlashedZeroCheck->setFont(f);
	}
}

void SMFontFeatures::slotSetMenu()
{
	if (m_hasParent)
	{
		QFont f(font());
		f.setBold(true);
		for (int i = 0; i < styleSetsMenu->actions().count(); i++)
		{
			if (styleSetsMenu->actions().at(i)->isChecked())
				styleSetsMenu->actions().at(i)->setFont(f);
		}
	}
}

void SMFontFeatures::resetFontFeatures()
{
	disconnectSignals();
	ContextualCheck->setChecked(false);
	CommonCheck->setChecked(false);
	DiscretionaryCheck->setChecked(false);
	HistoricalCheck->setChecked(false);
	capitalsComboBox->setCurrentIndex(0);
	positionComboBox->setCurrentIndex(0);
	numeralComboBox->setCurrentIndex(0);
	widthComboBox->setCurrentIndex(0);
	fractionComboBox->setCurrentIndex(0);
	SlashedZeroCheck->setChecked(false);

	for (int i=0; i < styleSetsMenu->actions().count(); i++)
		styleSetsMenu->actions().at(i)->setChecked(false);
	connectSignals();
}
