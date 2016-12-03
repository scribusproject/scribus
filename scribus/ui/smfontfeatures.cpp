/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smfontfeatures.h"
#include "util.h"
#include <QListWidget>
#include <QListWidgetItem>
#include <QAction>

SMFontFeatures::SMFontFeatures(QWidget *parent)
	: QWidget(parent),
	  m_hasParent(false),
	  m_useParentValue(false),
	  m_pValue("")
{
	setupUi(this);
	setMenu = new StylisticSetsMenu(this);
	for (int i = 1; i <= 20 ; i++)
	{
		QAction *action = new QAction("set " + QString::number(i),setMenu);
		action->setCheckable(true);
		setMenu->addAction(action);
	}
	connect(this->commandLinkButton,SIGNAL(clicked(bool)), this, SLOT(showStyleSetsList()));

	//capture changed signal
	connect(this->CommonCheck, SIGNAL(toggled(bool)), this, SLOT(slotChange()));
	connect(this->ContextualCheck, SIGNAL(toggled(bool)), this, SLOT(slotChange()));
	connect(this->DiscretinoryCheck, SIGNAL(toggled(bool)), this, SLOT(slotChange()));
	connect(this->HistoricalCheck, SIGNAL(toggled(bool)), this, SLOT(slotChange()));
	connect(this->SlashedZeroCheck, SIGNAL(toggled(bool)), this, SLOT(slotChange()));
	connect(this->capitalsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChange()));
	connect(this->positionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChange()));
	connect(this->numeralComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChange()));
	connect(this->widthComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChange()));
	connect(this->fractionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChange()));
	connect(this->setMenu, SIGNAL(triggered(QAction*)), this, SLOT(slotChange()));
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
			DiscretinoryCheck->setChecked(true);
		else if (fontFeatures[i] == "+hlig")
			HistoricalCheck->setChecked(true);
		// position comboBox
		else if (fontFeatures[i] == "+subs")
			setCurrentComboItem(positionComboBox, "Subscript");
		else if (fontFeatures[i] == "+sups")
			setCurrentComboItem(positionComboBox, "Superscript");
		else if (fontFeatures[i] == "+ordn")
			setCurrentComboItem(positionComboBox, "Ordinal");
		// Capitals ComboBox
		else if (fontFeatures[i] == "+smcp")
			setCurrentComboItem(capitalsComboBox, "Small Capitals");
		else if (fontFeatures[i] == "+c2sc")
			setCurrentComboItem(capitalsComboBox, "Small Capitals from Capitals");
		else if (fontFeatures[i] == "+pcap")
			setCurrentComboItem(capitalsComboBox, "Petite Capitals");
		else if (fontFeatures[i] == "+c2pc")
			setCurrentComboItem(capitalsComboBox, "Petite Capitals from Capitals");
		else if (fontFeatures[i] == "+unic")
			setCurrentComboItem(capitalsComboBox, "Unicase");
		else if (fontFeatures[i] == "+titl")
			setCurrentComboItem(capitalsComboBox, "Titling");
		// Numerals style
		else if (fontFeatures[i] == "+lnum")
			setCurrentComboItem(numeralComboBox, "Lining");
		else if (fontFeatures[i] == "+onum")
			setCurrentComboItem(numeralComboBox, "Old Style");
		// Numerals width
		else if (fontFeatures[i] == "+pnum")
			setCurrentComboItem(widthComboBox, "Proportional");
		else if (fontFeatures[i] == "+tnum")
			setCurrentComboItem(widthComboBox, "Tabular");
		// Numerals Fraction
		else if (fontFeatures[i] == "+frac")
			setCurrentComboItem(fractionComboBox, "Diagonal Fractions");
		else if (fontFeatures[i] == "+afrc")
			setCurrentComboItem(fractionComboBox, "Stacked Fractions");
		// Numerals Zero
		else if (fontFeatures[i] == "+zero")
			SlashedZeroCheck->setChecked(true);
		// Style Sets
		else if (fontFeatures[i] == "+ss01")
			setMenu->actions().at(0)->setChecked(true);
		else if (fontFeatures[i] == "+ss02")
			setMenu->actions().at(1)->setChecked(true);
		else if (fontFeatures[i] == "+ss03")
			setMenu->actions().at(2)->setChecked(true);
		else if (fontFeatures[i] == "+ss04")
			setMenu->actions().at(3)->setChecked(true);
		else if (fontFeatures[i] == "+ss05")
			setMenu->actions().at(4)->setChecked(true);
		else if (fontFeatures[i] == "+ss06")
			setMenu->actions().at(5)->setChecked(true);
		else if (fontFeatures[i] == "+ss07")
			setMenu->actions().at(6)->setChecked(true);
		else if (fontFeatures[i] == "+ss08")
			setMenu->actions().at(7)->setChecked(true);
		else if (fontFeatures[i] == "+ss09")
			setMenu->actions().at(8)->setChecked(true);
		else if (fontFeatures[i] == "+ss10")
			setMenu->actions().at(9)->setChecked(true);
		else if (fontFeatures[i] == "+ss11")
			setMenu->actions().at(10)->setChecked(true);
		else if (fontFeatures[i] == "+ss12")
			setMenu->actions().at(11)->setChecked(true);
		else if (fontFeatures[i] == "+ss13")
			setMenu->actions().at(12)->setChecked(true);
		else if (fontFeatures[i] == "+ss14")
			setMenu->actions().at(13)->setChecked(true);
		else if (fontFeatures[i] == "+ss15")
			setMenu->actions().at(14)->setChecked(true);
		else if (fontFeatures[i] == "+ss16")
			setMenu->actions().at(15)->setChecked(true);
		else if (fontFeatures[i] == "+ss17")
			setMenu->actions().at(16)->setChecked(true);
		else if (fontFeatures[i] == "+ss18")
			setMenu->actions().at(17)->setChecked(true);
		else if (fontFeatures[i] == "+ss19")
			setMenu->actions().at(18)->setChecked(true);
		else if (fontFeatures[i] == "+ss20")
			setMenu->actions().at(19)->setChecked(true);
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
	if (DiscretinoryCheck->isChecked())
		font_feature << "+dlig";
	if (HistoricalCheck->isChecked())
		font_feature << "+hlig";
	//Position
	QString vlaue = positionComboBox->currentText();
	if (vlaue == "Subscript")
		font_feature << "+subs";
	else if (vlaue == "Superscript")
		font_feature <<"+sups";
	//Capitals
	vlaue = capitalsComboBox->currentText();
	if (vlaue == "Small Capitals")
		font_feature << "+smcp";
	else if (vlaue == "Small Capitals from Capitals")
		font_feature << "+c2sc";
	else if (vlaue == "Petite Capitals")
		font_feature << "+pcap";
	else if (vlaue == "Petite Capitals from Capitals")
		font_feature << "+c2pc";
	else if (vlaue == "Unicase")
		font_feature << "+unic";
	else if (vlaue == "Titling")
		font_feature << "+titl";
	//Numerals style
	vlaue = numeralComboBox->currentText();
	if (vlaue == "Lining")
		font_feature << "+lnum";
	else if (vlaue == "Old Style")
		font_feature << "+onum";
	//Numerals width
	vlaue = widthComboBox->currentText();
	if (vlaue == "Proportional")
		font_feature << "+pnum";
	else if (vlaue == "Tabular")
		font_feature << "+tnum";
	//Numerals Fraction
	vlaue = fractionComboBox->currentText();
	if (vlaue == "Diagonal Fractions")
		font_feature << "+frac";
	else if (vlaue == "Stacked Fractions")
		font_feature << "+afrc";
	// Numerals Zero
	if (SlashedZeroCheck->isChecked())
		font_feature << "+zero";

	// Stylistic sets
	if (setMenu->actions().at(0)->isChecked())
		font_feature << "+ss01";
	if (setMenu->actions().at(1)->isChecked())
		font_feature << "+ss02";
	if (setMenu->actions().at(2)->isChecked())
		font_feature << "+ss03";
	if (setMenu->actions().at(3)->isChecked())
		font_feature << "+ss04";
	if (setMenu->actions().at(4)->isChecked())
		font_feature << "+ss05";
	if (setMenu->actions().at(5)->isChecked())
		font_feature << "+ss06";
	if (setMenu->actions().at(6)->isChecked())
		font_feature << "+ss07";
	if (setMenu->actions().at(7)->isChecked())
		font_feature << "+ss08";
	if (setMenu->actions().at(8)->isChecked())
		font_feature << "+ss09";
	if (setMenu->actions().at(9)->isChecked())
		font_feature << "+ss10";
	if (setMenu->actions().at(10)->isChecked())
		font_feature << "+ss11";
	if (setMenu->actions().at(11)->isChecked())
		font_feature << "+ss12";
	if (setMenu->actions().at(12)->isChecked())
		font_feature << "+ss13";
	if (setMenu->actions().at(13)->isChecked())
		font_feature << "+ss14";
	if (setMenu->actions().at(14)->isChecked())
		font_feature << "+ss15";
	if (setMenu->actions().at(15)->isChecked())
		font_feature << "+ss16";
	if (setMenu->actions().at(16)->isChecked())
		font_feature << "+ss17";
	if (setMenu->actions().at(17)->isChecked())
		font_feature << "+ss18";
	if (setMenu->actions().at(18)->isChecked())
		font_feature << "+ss19";
	if (setMenu->actions().at(19)->isChecked())
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
	connect(DiscretinoryCheck, SIGNAL(clicked()), this, SLOT(slotDiscretinoryCheck()));
	connect(HistoricalCheck, SIGNAL(clicked()), this, SLOT(slotHistoricalCheck()));
	connect(capitalsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotCapitalsComboBox()));
	connect(positionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotPositionComboBox()));
	connect(numeralComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotNumeralComboBox()));
	connect(widthComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotWidthComboBox()));
	connect(fractionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotFractionComboBox()));
	connect(SlashedZeroCheck, SIGNAL(clicked()), this, SLOT(slotSlashedZeroCheck()));
	connect(setMenu, SIGNAL(triggered(QAction*)), this, SLOT(slotSetMenu()));
}

void SMFontFeatures::disconnectSignals()
{
	disconnect(ContextualCheck, SIGNAL(clicked()), this, SLOT(slotContextualCheck()));
	disconnect(CommonCheck, SIGNAL(clicked()), this, SLOT(slotCommonCheck()));
	disconnect(DiscretinoryCheck, SIGNAL(clicked()), this, SLOT(slotDiscretinoryCheck()));
	disconnect(HistoricalCheck, SIGNAL(clicked()), this, SLOT(slotHistoricalCheck()));
	disconnect(capitalsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotCapitalsComboBox()));
	disconnect(positionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotPositionComboBox()));
	disconnect(numeralComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotNumeralComboBox()));
	disconnect(widthComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotWidthComboBox()));
	disconnect(fractionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotFractionComboBox()));
	disconnect(SlashedZeroCheck, SIGNAL(clicked()), this, SLOT(slotSlashedZeroCheck()));
	disconnect(setMenu, SIGNAL(triggered(QAction*)), this, SLOT(slotSetMenu()));
}

void SMFontFeatures::hideAllFontFeatures()
{
	statusLabel->hide();
	ligaturesGroupBox->hide();
	CommonCheck->setChecked(false);
	CommonCheck->setDisabled(true);
	DiscretinoryCheck->setChecked(false);
	DiscretinoryCheck->setDisabled(true);
	ContextualCheck->setDisabled(true);
	HistoricalCheck->setDisabled(true);

	capitalsLabel->hide();
	removeComboItem(capitalsComboBox, "Small Capitals");
	removeComboItem(capitalsComboBox, "Small Capitals from Captials");
	removeComboItem(capitalsComboBox, "Petite Capitals");
	removeComboItem(capitalsComboBox, "Petite Capitals from Captials");
	removeComboItem(capitalsComboBox, "Unicase");
	removeComboItem(capitalsComboBox, "Titling");
	capitalsComboBox->hide();


	positionLabel->hide();
	removeComboItem(positionComboBox, "Subscript");
	removeComboItem(positionComboBox, "Superscript");
	removeComboItem(positionComboBox, "Ordinals");
	positionComboBox->hide();
	capitalsGroupBox->hide();

	numeralsGroupBox->hide();
	styleLabel->hide();
	removeComboItem(numeralComboBox, "Lining");
	removeComboItem(numeralComboBox, "Old Style");
	numeralComboBox->hide();

	widthLabel->hide();
	removeComboItem(widthComboBox, "Proportional");
	removeComboItem(widthComboBox, "Tabular");
	widthComboBox->hide();

	fractionLabel->hide();
	removeComboItem(fractionComboBox, "Diagonal Fractions");
	removeComboItem(fractionComboBox, "Stacked Fractions");
	fractionComboBox->hide();

	SlashedZeroCheck->hide();
	stylisticSetsLabel->hide();
	commandLinkButton->hide();
	stylisticSetsLabel->hide();
	for (int i = 0; i < setMenu->actions().count(); i++)
	{
		setMenu->actions().at(i)->setVisible(false);
	}
}

void SMFontFeatures::enableFontFeatures(QStringList fontFeatures)
{
	hideAllFontFeatures();
	capitalsLabel->show();
	capitalsComboBox->show();
	numeralsGroupBox->show();
	numeralComboBox->show();
	positionComboBox->show();
	positionLabel->show();
	fractionLabel->show();
	fractionComboBox->show();
	capitalsGroupBox->show();

	widthComboBox->show();
	styleLabel->show();
	widthLabel->show();
	stylisticSetsLabel->show();
	commandLinkButton->show();

	for (int i = 0; i < fontFeatures.count(); i++)
	{
		// Ligatures
		if (fontFeatures[i] == "clig")
		{
			ContextualCheck->setDisabled(false);
			ContextualCheck->setChecked(true);
		}
		else if (fontFeatures[i] == "liga")
		{
			CommonCheck->setDisabled(false);
			CommonCheck->setChecked(true);
		}
		else if (fontFeatures[i] == "dlig")
			DiscretinoryCheck->setDisabled(false);
		else if (fontFeatures[i] == "hlig")
			HistoricalCheck->setDisabled(false);
		// position comboBox
		else if (fontFeatures[i] == "subs")
			positionComboBox->addItem("Subscript");
		else if (fontFeatures[i] == "sups")
			positionComboBox->addItem("Superscript");
		else if (fontFeatures[i] == "ordn")
			positionComboBox->addItem("Ordinals");
		// Capitals ComboBox
		else if (fontFeatures[i] == "smcp")
			capitalsComboBox->addItem("Small Capitals");
		else if (fontFeatures[i] == "c2sc")
			capitalsComboBox->addItem("Small Capitals from Captials");
		else if (fontFeatures[i] == "pcap")
			capitalsComboBox->addItem("Petite Capitals");
		else if (fontFeatures[i] == "c2pc")
			capitalsComboBox->addItem("Petite Capitals from Capitals");
		else if (fontFeatures[i] == "unic")
			capitalsComboBox->addItem("Unicase");
		else if (fontFeatures[i] == "titl")
			capitalsComboBox->addItem("Titling");
		// Numerals style
		else if (fontFeatures[i] == "lnum")
			numeralComboBox->addItem("Lining");
		else if (fontFeatures[i] == "onum")
			numeralComboBox->addItem("Old Style");
		// Numerals width
		else if (fontFeatures[i] == "pnum")
			widthComboBox->addItem("Proportional");
		else if (fontFeatures[i] == "tnum")
			widthComboBox->addItem("Tabular");
		// Numerals Fraction
		else if (fontFeatures[i] == "frac")
			fractionComboBox->addItem("Diagonal Fractions");
		else if (fontFeatures[i] == "afrc")
			fractionComboBox->addItem("Stacked Fractions");
		// Numerals Zero
		else if (fontFeatures[i] == "zero")
			SlashedZeroCheck->show();
		// Style Sets
		else if (fontFeatures[i] == "ss01")
			setMenu->actions().at(0)->setVisible(true);
		else if (fontFeatures[i] == "ss02")
			setMenu->actions().at(1)->setVisible(true);
		else if (fontFeatures[i] == "ss03")
			setMenu->actions().at(2)->setVisible(true);
		else if (fontFeatures[i] == "ss04")
			setMenu->actions().at(3)->setVisible(true);
		else if (fontFeatures[i] == "ss05")
			setMenu->actions().at(4)->setVisible(true);
		else if (fontFeatures[i] == "ss06")
			setMenu->actions().at(5)->setVisible(true);
		else if (fontFeatures[i] == "ss07")
			setMenu->actions().at(6)->setVisible(true);
		else if (fontFeatures[i] == "ss08")
			setMenu->actions().at(7)->setVisible(true);
		else if (fontFeatures[i] == "ss09")
			setMenu->actions().at(8)->setVisible(true);
		else if (fontFeatures[i] == "ss10")
			setMenu->actions().at(9)->setVisible(true);
		else if (fontFeatures[i] == "ss11")
			setMenu->actions().at(10)->setVisible(true);
		else if (fontFeatures[i] == "ss12")
			setMenu->actions().at(11)->setVisible(true);
		else if (fontFeatures[i] == "ss13")
			setMenu->actions().at(12)->setVisible(true);
		else if (fontFeatures[i] == "ss14")
			setMenu->actions().at(13)->setVisible(true);
		else if (fontFeatures[i] == "ss15")
			setMenu->actions().at(14)->setVisible(true);
		else if (fontFeatures[i] == "ss16")
			setMenu->actions().at(15)->setVisible(true);
		else if (fontFeatures[i] == "ss17")
			setMenu->actions().at(16)->setVisible(true);
		else if (fontFeatures[i] == "ss18")
			setMenu->actions().at(17)->setVisible(true);
		else if (fontFeatures[i] == "ss19")
			setMenu->actions().at(18)->setVisible(true);
		else if (fontFeatures[i] == "ss20")
			setMenu->actions().at(19)->setVisible(true);
	}

	if (positionComboBox->count() < 2)
	{
		positionComboBox->hide();
		positionLabel->hide();
	}
	if (capitalsComboBox->count() < 2)
	{
		capitalsComboBox->hide();
		capitalsLabel->hide();
	}
	if (numeralComboBox->count() < 2)
	{
		numeralComboBox->hide();
		styleLabel->hide();
	}
	if (widthComboBox->count() < 2)
	{
		widthComboBox->hide();
		widthLabel->hide();
	}
	if (fractionComboBox->count() < 2)
	{
		fractionComboBox->hide();
		fractionLabel->hide();
	}
	if (ContextualCheck->isEnabled() || CommonCheck->isEnabled()
			|| DiscretinoryCheck->isEnabled() || HistoricalCheck->isEnabled())
		ligaturesGroupBox->show();
	if (positionComboBox->isHidden() && capitalsComboBox->isHidden())
		capitalsGroupBox->hide();
	if (numeralComboBox->isHidden() && fractionComboBox->isHidden()
			&& widthComboBox->isHidden() && SlashedZeroCheck->isHidden())
		numeralsGroupBox->hide();

	int visibleCount = 0;
	for (int i = 0; i < setMenu->actions().size(); ++i)
		if (setMenu->actions().at(i)->isVisible())
			visibleCount ++;

	if (visibleCount == 0)
	{
		stylisticSetsLabel->hide();
		commandLinkButton->hide();
	}
	if (ligaturesGroupBox->isHidden() && capitalsGroupBox->isHidden()
			&& numeralsGroupBox && commandLinkButton->isHidden())
	{
		statusLabel->show();
	}
}

void SMFontFeatures::showStyleSetsList()
{
	setMenu->popup(commandLinkButton->mapToGlobal(QPoint(0,0)));
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

void SMFontFeatures::slotDiscretinoryCheck()
{
	if (m_hasParent)
	{
		QFont f(font());
		f.setBold(true);
		DiscretinoryCheck->setFont(f);
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
		for (int i = 0; i < setMenu->actions().count(); i++)
		{
			if (setMenu->actions().at(i)->isChecked())
				setMenu->actions().at(i)->setFont(f);
		}
	}
}

void SMFontFeatures::resetFontFeatures()
{
	disconnectSignals();
	ContextualCheck->setChecked(false);
	CommonCheck->setChecked(false);
	DiscretinoryCheck->setChecked(false);
	HistoricalCheck->setChecked(false);
	capitalsComboBox->setCurrentIndex(0);
	positionComboBox->setCurrentIndex(0);
	numeralComboBox->setCurrentIndex(0);
	widthComboBox->setCurrentIndex(0);
	fractionComboBox->setCurrentIndex(0);
	SlashedZeroCheck->setChecked(false);

	for (int i=0; i < setMenu->actions().count(); i++)
		setMenu->actions().at(i)->setChecked(false);
	connectSignals();
}
