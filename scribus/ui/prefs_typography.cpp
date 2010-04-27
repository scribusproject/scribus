/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "prefs_typography.h"
#include "prefsstructs.h"
#include "scribusdoc.h"

Prefs_Typography::Prefs_Typography(QWidget* parent, ScribusDoc* doc)
	: Prefs_Pane(parent)
{
	setupUi(this);
	languageChange();
}

Prefs_Typography::~Prefs_Typography()
{
}

void Prefs_Typography::languageChange()
{
	QString autoText=tr( "Auto" );
	underlineDisplacementSpinBox->setSpecialValueText(autoText);
	underlineLineWidthSpinBox->setSpecialValueText(autoText);
	strikeoutDisplacementSpinBox->setSpecialValueText(autoText);
	strikeoutLineWidthSpinBox->setSpecialValueText(autoText);

	subscriptDisplacementSpinBox->setToolTip( tr( "Displacement below the baseline of the normal font on a line" ) );
	subscriptScalingSpinBox->setToolTip( tr( "Relative size of the subscript compared to the normal font" ) );
	superscriptDisplacementSpinBox->setToolTip( tr( "Displacement above the baseline of the font on a line" ) );
	superscriptScalingSpinBox->setToolTip( tr( "Relative size of the superscript compared to the normal font" ) );
	underlineDisplacementSpinBox->setToolTip( tr( "Displacement below the baseline of the normal font expressed as a percentage of the fonts descender" ) );
	underlineLineWidthSpinBox->setToolTip( tr( "Line width expressed as a percentage of the font size" ) );
	strikeoutDisplacementSpinBox->setToolTip( tr( "Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender" ) );
	strikeoutLineWidthSpinBox->setToolTip( tr( "Line width expressed as a percentage of the font size" ) );
	smallcapsScalingSpinBox->setToolTip( tr( "Relative size of the small caps font compared to the normal font" ) );
	automaticLineSpacingSpinBox->setToolTip( tr( "Percentage increase over the font size for the line spacing" ) );
}

void Prefs_Typography::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	subscriptDisplacementSpinBox->setValue(prefsData->typoPrefs.valueSubScript);
	subscriptScalingSpinBox->setValue(prefsData->typoPrefs.scalingSubScript);
	superscriptDisplacementSpinBox->setValue(prefsData->typoPrefs.valueSuperScript);
	superscriptScalingSpinBox->setValue(prefsData->typoPrefs.scalingSuperScript);
	underlineDisplacementSpinBox->setValue(prefsData->typoPrefs.valueUnderlinePos / 10.0);
	underlineLineWidthSpinBox->setValue(prefsData->typoPrefs.valueUnderlineWidth / 10.0);
	strikeoutDisplacementSpinBox->setValue(prefsData->typoPrefs.valueStrikeThruPos / 10.0);
	strikeoutLineWidthSpinBox->setValue(prefsData->typoPrefs.valueStrikeThruWidth / 10.0);
	smallcapsScalingSpinBox->setValue(prefsData->typoPrefs.valueSmallCaps);
	automaticLineSpacingSpinBox->setValue(prefsData->typoPrefs.autoLineSpacing);
}

void Prefs_Typography::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->typoPrefs.valueSubScript=subscriptDisplacementSpinBox->value();
	prefsData->typoPrefs.scalingSubScript=subscriptScalingSpinBox->value();
	prefsData->typoPrefs.valueSuperScript=superscriptDisplacementSpinBox->value();
	prefsData->typoPrefs.scalingSuperScript=superscriptScalingSpinBox->value();
	prefsData->typoPrefs.valueUnderlinePos=underlineDisplacementSpinBox->value() * 10.0;
	prefsData->typoPrefs.valueUnderlineWidth=underlineLineWidthSpinBox->value() * 10.0;
	prefsData->typoPrefs.valueStrikeThruPos=strikeoutDisplacementSpinBox->value() * 10.0;
	prefsData->typoPrefs.valueStrikeThruWidth=strikeoutLineWidthSpinBox->value() * 10.0;
	prefsData->typoPrefs.valueSmallCaps=smallcapsScalingSpinBox->value();
	prefsData->typoPrefs.autoLineSpacing=automaticLineSpacingSpinBox->value();
}

