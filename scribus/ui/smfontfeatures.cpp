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
	: QWidget(parent),
	  m_hasParent(false),
	  m_useParentValue(false),
	  m_pValue("")
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

	QStringList capitalFeatures  = comboboxFeatures(capitalsComboBox);
	QStringList positionFeatures = comboboxFeatures(positionComboBox);
	QStringList numeralFeatures  = comboboxFeatures(numeralComboBox);
	QStringList widthFeatures    = comboboxFeatures(widthComboBox);
	QStringList fractionFeatures = comboboxFeatures(fractionComboBox);

	int oldCapitalIndex  = capitalsComboBox->currentIndex();
	int oldPositionIndex = positionComboBox->currentIndex();
	int oldNumeralIndex  = numeralComboBox->currentIndex();
	int oldWidthIndex    = widthComboBox->currentIndex();
	int oldFractionIndex = fractionComboBox->currentIndex();

	retranslateUi(this);

	setupCapitalCombo(FontFeatures::textFlagsFromList(capitalFeatures));
	setupPositionCombo(FontFeatures::textFlagsFromList(positionFeatures));
	setupNumeralStyleCombo(FontFeatures::textFlagsFromList(numeralFeatures));
	setupNumeralWidthCombo(FontFeatures::textFlagsFromList(widthFeatures));
	setupNumeralFractionCombo(FontFeatures::textFlagsFromList(fractionFeatures));

	capitalsComboBox->setCurrentIndex(oldCapitalIndex);
	positionComboBox->setCurrentIndex(oldPositionIndex);
	numeralComboBox->setCurrentIndex(oldNumeralIndex);
	widthComboBox->setCurrentIndex(oldWidthIndex);
	fractionComboBox->setCurrentIndex(oldFractionIndex);
}

QStringList SMFontFeatures::comboboxFeatures(QComboBox* combo)
{
	QStringList features;
	for (int i = 0; i < combo->count(); ++i)
	{
		QVariant varVal = combo->itemData(i);
		QString value = varVal.toString();
		if (value.length() > 0)
			features.append(value);
	}
	return features;
}

void SMFontFeatures::setFontFeatures(QString s, QStringList fontFeaturesList)
{
	disconnectSignals();
	m_hasParent = false;
	m_pValue = "";

	resetFontFeatures();
	enableFontFeatures(fontFeaturesList);
	QStringList fontFeatures = s.split(',');
	for (int i = 0; i < fontFeatures.count(); i++)
	{
		// Ligatures
		if (fontFeatures[i] == "-clig")
			ContextualCheck->setChecked(false);
		else if (fontFeatures[i] == "-liga")
			CommonCheck->setChecked(false);
		else if (fontFeatures[i] == "+dlig")
			DiscretionaryCheck->setChecked(true);
		else if (fontFeatures[i] == "+hlig")
			HistoricalCheck->setChecked(true);
		// position comboBox
		else if (fontFeatures[i] == "+subs" || 
		         fontFeatures[i] == "+sups" ||
		         fontFeatures[i] == "+ordn")
		{
			setCurrentComboItemFromData(positionComboBox, fontFeatures[i]);
		}
		// Capitals ComboBox
		else if (fontFeatures[i] == "+smcp" || 
		         fontFeatures[i] == "+c2sc" || 
		         fontFeatures[i] == "+pcap" || 
		         fontFeatures[i] == "+c2pc" || 
		         fontFeatures[i] == "+unic" || 
		         fontFeatures[i] == "+titl")
		{
			setCurrentComboItemFromData(capitalsComboBox, fontFeatures[i]);
		}
		// Numerals style
		else if (fontFeatures[i] == "+lnum" || 
		         fontFeatures[i] == "+onum")
		{
			setCurrentComboItemFromData(numeralComboBox, fontFeatures[i]);
		}
		// Numerals width
		else if (fontFeatures[i] == "+pnum" || 
		         fontFeatures[i] == "+tnum")
		{
			setCurrentComboItemFromData(widthComboBox, fontFeatures[i]);
		}
		// Numerals Fraction
		else if (fontFeatures[i] == "+frac" || 
		         fontFeatures[i] == "+afrc")
		{
			setCurrentComboItemFromData(fractionComboBox, fontFeatures[i]);
		}
		// Numerals Zero
		else if (fontFeatures[i] == "+zero")
			SlashedZeroCheck->setChecked(true);
		// Style Sets
		else if (fontFeatures[i] == "+ss01")
			styleSetsMenu->actions().at(0)->setChecked(true);
		else if (fontFeatures[i] == "+ss02")
			styleSetsMenu->actions().at(1)->setChecked(true);
		else if (fontFeatures[i] == "+ss03")
			styleSetsMenu->actions().at(2)->setChecked(true);
		else if (fontFeatures[i] == "+ss04")
			styleSetsMenu->actions().at(3)->setChecked(true);
		else if (fontFeatures[i] == "+ss05")
			styleSetsMenu->actions().at(4)->setChecked(true);
		else if (fontFeatures[i] == "+ss06")
			styleSetsMenu->actions().at(5)->setChecked(true);
		else if (fontFeatures[i] == "+ss07")
			styleSetsMenu->actions().at(6)->setChecked(true);
		else if (fontFeatures[i] == "+ss08")
			styleSetsMenu->actions().at(7)->setChecked(true);
		else if (fontFeatures[i] == "+ss09")
			styleSetsMenu->actions().at(8)->setChecked(true);
		else if (fontFeatures[i] == "+ss10")
			styleSetsMenu->actions().at(9)->setChecked(true);
		else if (fontFeatures[i] == "+ss11")
			styleSetsMenu->actions().at(10)->setChecked(true);
		else if (fontFeatures[i] == "+ss12")
			styleSetsMenu->actions().at(11)->setChecked(true);
		else if (fontFeatures[i] == "+ss13")
			styleSetsMenu->actions().at(12)->setChecked(true);
		else if (fontFeatures[i] == "+ss14")
			styleSetsMenu->actions().at(13)->setChecked(true);
		else if (fontFeatures[i] == "+ss15")
			styleSetsMenu->actions().at(14)->setChecked(true);
		else if (fontFeatures[i] == "+ss16")
			styleSetsMenu->actions().at(15)->setChecked(true);
		else if (fontFeatures[i] == "+ss17")
			styleSetsMenu->actions().at(16)->setChecked(true);
		else if (fontFeatures[i] == "+ss18")
			styleSetsMenu->actions().at(17)->setChecked(true);
		else if (fontFeatures[i] == "+ss19")
			styleSetsMenu->actions().at(18)->setChecked(true);
		else if (fontFeatures[i] == "+ss20")
			styleSetsMenu->actions().at(19)->setChecked(true);
	}
}

void SMFontFeatures::setFontFeatures(QString val, QStringList fontFeatures, bool isParentVal)
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
	QStringList font_feature ;
	if (!ContextualCheck->isChecked())
		font_feature << "-clig";
	if (!CommonCheck->isChecked())
		font_feature << "-liga";
	if (DiscretionaryCheck->isChecked())
		font_feature << "+dlig";
	if (HistoricalCheck->isChecked())
		font_feature << "+hlig";
	//Position
	QVariant varVal = positionComboBox->currentData();
	QString  value = varVal.toString();
	if (value.length() > 0)
		font_feature << value;
	//Capitals
	varVal = capitalsComboBox->currentData();
	value = varVal.toString();
	if (value.length() > 0)
		font_feature << value;
	//Numerals style
	varVal = numeralComboBox->currentData();
	value = varVal.toString();
	if (value.length() > 0)
		font_feature << value;
	//Numerals width
	varVal = widthComboBox->currentData();
	value = varVal.toString();
	if (value.length() > 0)
		font_feature << value;
	//Numerals Fraction
	varVal = fractionComboBox->currentData();
	value = varVal.toString();
	if (value.length() > 0)
		font_feature << value;
	// Numerals Zero
	if (SlashedZeroCheck->isChecked())
		font_feature << "+zero";

	// Stylistic sets
	if (styleSetsMenu->actions().at(0)->isChecked())
		font_feature << "+ss01";
	if (styleSetsMenu->actions().at(1)->isChecked())
		font_feature << "+ss02";
	if (styleSetsMenu->actions().at(2)->isChecked())
		font_feature << "+ss03";
	if (styleSetsMenu->actions().at(3)->isChecked())
		font_feature << "+ss04";
	if (styleSetsMenu->actions().at(4)->isChecked())
		font_feature << "+ss05";
	if (styleSetsMenu->actions().at(5)->isChecked())
		font_feature << "+ss06";
	if (styleSetsMenu->actions().at(6)->isChecked())
		font_feature << "+ss07";
	if (styleSetsMenu->actions().at(7)->isChecked())
		font_feature << "+ss08";
	if (styleSetsMenu->actions().at(8)->isChecked())
		font_feature << "+ss09";
	if (styleSetsMenu->actions().at(9)->isChecked())
		font_feature << "+ss10";
	if (styleSetsMenu->actions().at(10)->isChecked())
		font_feature << "+ss11";
	if (styleSetsMenu->actions().at(11)->isChecked())
		font_feature << "+ss12";
	if (styleSetsMenu->actions().at(12)->isChecked())
		font_feature << "+ss13";
	if (styleSetsMenu->actions().at(13)->isChecked())
		font_feature << "+ss14";
	if (styleSetsMenu->actions().at(14)->isChecked())
		font_feature << "+ss15";
	if (styleSetsMenu->actions().at(15)->isChecked())
		font_feature << "+ss16";
	if (styleSetsMenu->actions().at(16)->isChecked())
		font_feature << "+ss17";
	if (styleSetsMenu->actions().at(17)->isChecked())
		font_feature << "+ss18";
	if (styleSetsMenu->actions().at(18)->isChecked())
		font_feature << "+ss19";
	if (styleSetsMenu->actions().at(19)->isChecked())
		font_feature << "+ss20";
	return font_feature.join(",");
}

void SMFontFeatures::setParentValue(QString val)
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

void SMFontFeatures::enableFontFeatures(QStringList fontFeatures)
{
	int featureFlags = FontFeatures::fontFlagsFromList(fontFeatures);

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
			&& numeralsGroupBox && stylisticSetsButton->isHidden())
	{
		statusLabel->show();
	}
}

void SMFontFeatures::setupCapitalCombo(quint64 featureFlags)
{
	QSignalBlocker blocker(capitalsComboBox);

	capitalsComboBox->clear();
	capitalsComboBox->addItem( tr("Default Capitals"));

	if (featureFlags & FontFeatures::SmallCaps)
		capitalsComboBox->addItem(tr("Small Capitals"), QVariant("+smcp"));
	if (featureFlags & FontFeatures::SmallCapsFromCaps)
		capitalsComboBox->addItem(tr("Small Capitals from Capitals"), QVariant("+c2sc"));
	if (featureFlags & FontFeatures::PetiteCaps)
		capitalsComboBox->addItem(tr("Petite Capitals"), QVariant("+pcap"));
	if (featureFlags & FontFeatures::PetiteCapsFromCaps)
		capitalsComboBox->addItem(tr("Petite Capitals from Capitals"), QVariant("+c2pc"));
	if (featureFlags & FontFeatures::UnicaseCaps)
		capitalsComboBox->addItem(tr("Unicase"), QVariant("+unic"));
	if (featureFlags & FontFeatures::TiltingCaps)
		capitalsComboBox->addItem(tr("Titling"), QVariant("+titl"));
}

void SMFontFeatures::setupPositionCombo(quint64 featureFlags)
{
	QSignalBlocker blocker(positionComboBox);

	positionComboBox->clear();
	positionComboBox->addItem( tr("Default Position"));

	if (featureFlags & FontFeatures::Subscript)
		positionComboBox->addItem(tr("Subscript"), QVariant("+subs"));
	if (featureFlags & FontFeatures::Superscript)
		positionComboBox->addItem(tr("Superscript"), QVariant("+sups"));
	if (featureFlags & FontFeatures::Ordinals)
		positionComboBox->addItem(tr("Ordinals"), QVariant("+ordn"));
}

void SMFontFeatures::setupNumeralStyleCombo(quint64 featureFlags)
{
	QSignalBlocker blocker(numeralComboBox);

	numeralComboBox->clear();
	numeralComboBox->addItem( tr("Default Numerals"));

	if (featureFlags & FontFeatures::LiningNumerals)
		numeralComboBox->addItem(tr("Lining"), QVariant("+lnum"));
	if (featureFlags & FontFeatures::OldStyleNumerals)
		numeralComboBox->addItem(tr("Old Style"), QVariant("+onum"));
}

void SMFontFeatures::setupNumeralWidthCombo(quint64 featureFlags)
{
	QSignalBlocker blocker(widthComboBox);

	widthComboBox->clear();
	widthComboBox->addItem( tr("Default Numeral Width"));

	if (featureFlags & FontFeatures::ProportionalNumeralWidth)
		widthComboBox->addItem(tr("Proportional"), QVariant("+pnum"));
	if (featureFlags & FontFeatures::TabularNumeralWidth)
		widthComboBox->addItem(tr("Tabular"), QVariant("+tnum"));
}

void SMFontFeatures::setupNumeralFractionCombo(quint64 featureFlags)
{
	QSignalBlocker blocker(fractionComboBox);

	fractionComboBox->clear();
	fractionComboBox->addItem( tr("No Fractions"));

	if (featureFlags & FontFeatures::DiagonalFractions)
		fractionComboBox->addItem(tr("Diagonal Fractions"), QVariant("+frac"));
	if (featureFlags & FontFeatures::StackedFractions)
		fractionComboBox->addItem(tr("Stacked Fractions"), QVariant("+afrc"));
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
