/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "propertywidget_fontfeatures.h"
#include "appmodes.h"
#include "pageitem_table.h"
#include "iconmanager.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "selection.h"

quint64 PropertyWidget_FontFeatures::FeatureCommonLigatures          = 1;
quint64 PropertyWidget_FontFeatures::FeatureContextualLigatures      = ((quint64) 1 << 1);
quint64 PropertyWidget_FontFeatures::FeatureDiscretionaryLigatures   = ((quint64) 1 << 2);
quint64 PropertyWidget_FontFeatures::FeatureHistoricalLigatures      = ((quint64) 1 << 3);
quint64 PropertyWidget_FontFeatures::FeatureSmallCaps                = ((quint64) 1 << 4);
quint64 PropertyWidget_FontFeatures::FeatureSmallCapsFromCaps        = ((quint64) 1 << 5);
quint64 PropertyWidget_FontFeatures::FeaturePetiteCaps               = ((quint64) 1 << 6);
quint64 PropertyWidget_FontFeatures::FeaturePetiteCapsFromCaps       = ((quint64) 1 << 7);
quint64 PropertyWidget_FontFeatures::FeatureUnicaseCaps              = ((quint64) 1 << 8);
quint64 PropertyWidget_FontFeatures::FeatureTiltingCaps              = ((quint64) 1 << 9);
quint64 PropertyWidget_FontFeatures::FeatureLiningNumerals           = ((quint64) 1 << 10);
quint64 PropertyWidget_FontFeatures::FeatureOldStyleNumerals         = ((quint64) 1 << 11);
quint64 PropertyWidget_FontFeatures::FeatureProportionalNumeralWidth = ((quint64) 1 << 12);
quint64 PropertyWidget_FontFeatures::FeatureTabularNumeralWidth      = ((quint64) 1 << 13);
quint64 PropertyWidget_FontFeatures::FeatureDiagonalFractions        = ((quint64) 1 << 14);
quint64 PropertyWidget_FontFeatures::FeatureStackedFractions         = ((quint64) 1 << 15);
quint64 PropertyWidget_FontFeatures::FeatureSubscript                = ((quint64) 1 << 16);
quint64 PropertyWidget_FontFeatures::FeatureSuperscript              = ((quint64) 1 << 17);
quint64 PropertyWidget_FontFeatures::FeatureOrdinals                 = ((quint64) 1 << 18);
quint64 PropertyWidget_FontFeatures::FeatureSlashedZero              = ((quint64) 1 << 19);
quint64 PropertyWidget_FontFeatures::FeatureStyleSet01               = ((quint64) 1 << 20);
quint64 PropertyWidget_FontFeatures::FeatureStyleSet02               = ((quint64) 1 << 21);
quint64 PropertyWidget_FontFeatures::FeatureStyleSet03               = ((quint64) 1 << 22);
quint64 PropertyWidget_FontFeatures::FeatureStyleSet04               = ((quint64) 1 << 23);
quint64 PropertyWidget_FontFeatures::FeatureStyleSet05               = ((quint64) 1 << 24);
quint64 PropertyWidget_FontFeatures::FeatureStyleSet06               = ((quint64) 1 << 25);
quint64 PropertyWidget_FontFeatures::FeatureStyleSet07               = ((quint64) 1 << 26);
quint64 PropertyWidget_FontFeatures::FeatureStyleSet08               = ((quint64) 1 << 27);
quint64 PropertyWidget_FontFeatures::FeatureStyleSet09               = ((quint64) 1 << 28);
quint64 PropertyWidget_FontFeatures::FeatureStyleSet10               = ((quint64) 1 << 29);
quint64 PropertyWidget_FontFeatures::FeatureStyleSet11               = ((quint64) 1 << 30);
quint64 PropertyWidget_FontFeatures::FeatureStyleSet12               = ((quint64) 1 << 31);
quint64 PropertyWidget_FontFeatures::FeatureStyleSet13               = ((quint64) 1 << 32);
quint64 PropertyWidget_FontFeatures::FeatureStyleSet14               = ((quint64) 1 << 33);
quint64 PropertyWidget_FontFeatures::FeatureStyleSet15               = ((quint64) 1 << 34);
quint64 PropertyWidget_FontFeatures::FeatureStyleSet16               = ((quint64) 1 << 35);
quint64 PropertyWidget_FontFeatures::FeatureStyleSet17               = ((quint64) 1 << 36);
quint64 PropertyWidget_FontFeatures::FeatureStyleSet18               = ((quint64) 1 << 37);
quint64 PropertyWidget_FontFeatures::FeatureStyleSet19               = ((quint64) 1 << 38);
quint64 PropertyWidget_FontFeatures::FeatureStyleSet20               = ((quint64) 1 << 39);

PropertyWidget_FontFeatures::PropertyWidget_FontFeatures(QWidget* parent) : QFrame(parent)
{
	m_item = NULL;
	m_ScMW = NULL;
	setupUi(this);

	setFrameStyle(QFrame::Box | QFrame::Plain);
	setLineWidth(1);
	layout()->setAlignment( Qt::AlignTop );

	initWidgets();
	languageChange();
}

void PropertyWidget_FontFeatures::setMainWindow(ScribusMainWindow *mw)
{
	m_ScMW = mw;
}

void PropertyWidget_FontFeatures::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}

void PropertyWidget_FontFeatures::languageChange()
{
	retranslateUi(this);
}

void PropertyWidget_FontFeatures::showFontFeatures(QString s, QStringList availableFeatures)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	enableFeatures(availableFeatures);

	QStringList fontFeatures = s.split(',');
	ContextualCheck->setChecked(true);
	CommonCheck->setChecked(true);
	for (int i = 0; i < fontFeatures.count(); i++)
	{
		if (fontFeatures[i] == "-clig")
			ContextualCheck->setChecked(false);
		else if (fontFeatures[i] == "-liga")
			CommonCheck->setChecked(false);
		else if (fontFeatures[i] == "+dlig")
			DiscretionaryCheck->setChecked(true);
		else if (fontFeatures[i] == "+hlig")
			HistoricalCheck->setChecked(true);
		else if (fontFeatures[i] == "+subs")
			SubscriptRadio->setChecked(true);
		else if (fontFeatures[i] == "+sups")
			SuperscriptRadio->setChecked(true);
		else if (fontFeatures[i] == "+smcp")
			SmallRadio->setChecked(true);
		else if (fontFeatures[i] == "+c2sc")
			SmallFromCRadio->setChecked(true);
		else if (fontFeatures[i] == "+pcap")
			PetiteRadio->setChecked(true);
		else if (fontFeatures[i] == "+c2pc")
			PetiteCapRadio->setChecked(true);
		else if (fontFeatures[i] == "+unic")
			UnicaseRadio->setChecked(true);
		else if (fontFeatures[i] == "+titl")
			TiltingRadio->setChecked(true);
		else if (fontFeatures[i] == "+lnum")
			LiningRadio->setChecked(true);
		else if (fontFeatures[i] == "+onum")
			OldStyleRadio->setChecked(true);
		else if (fontFeatures[i] == "+pnum")
			ProportionalRadio->setChecked(true);
		else if (fontFeatures[i] == "+tnum")
			TabularRadio->setChecked(true);
		else if (fontFeatures[i] == "+frac")
			DiagonalRadio->setChecked(true);
		else if (fontFeatures[i] == "+afrc")
			StackedRadio->setChecked(true);
		else if (fontFeatures[i] == "+ordn")
			OrdinalCheck->setChecked(true);
		else if (fontFeatures[i] == "+zero")
			SlashedZeroCheck->setChecked(true);
		else if (fontFeatures[i] == "+ss01")
			StyleSet01->setChecked(true);
		else if (fontFeatures[i] == "+ss02")
			StyleSet02->setChecked(true);
		else if (fontFeatures[i] == "+ss03")
			StyleSet03->setChecked(true);
		else if (fontFeatures[i] == "+ss04")
			StyleSet04->setChecked(true);
		else if (fontFeatures[i] == "+ss05")
			StyleSet05->setChecked(true);
		else if (fontFeatures[i] == "+ss06")
			StyleSet06->setChecked(true);
		else if (fontFeatures[i] == "+ss07")
			StyleSet07->setChecked(true);
		else if (fontFeatures[i] == "+ss08")
			StyleSet08->setChecked(true);
		else if (fontFeatures[i] == "+ss09")
			StyleSet09->setChecked(true);
		else if (fontFeatures[i] == "+ss10")
			StyleSet10->setChecked(true);
		else if (fontFeatures[i] == "+ss11")
			StyleSet11->setChecked(true);
		else if (fontFeatures[i] == "+ss12")
			StyleSet12->setChecked(true);
		else if (fontFeatures[i] == "+ss13")
			StyleSet13->setChecked(true);
		else if (fontFeatures[i] == "+ss14")
			StyleSet14->setChecked(true);
		else if (fontFeatures[i] == "+ss15")
			StyleSet15->setChecked(true);
		else if (fontFeatures[i] == "+ss16")
			StyleSet16->setChecked(true);
		else if (fontFeatures[i] == "+ss17")
			StyleSet17->setChecked(true);
		else if (fontFeatures[i] == "+ss18")
			StyleSet18->setChecked(true);
		else if (fontFeatures[i] == "+ss19")
			StyleSet19->setChecked(true);
		else if (fontFeatures[i] == "+ss20")
			StyleSet20->setChecked(true);
	}
}

void PropertyWidget_FontFeatures::handleFontFeatures()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	// Ligatures
	QStringList font_feature ;
	if (!CommonCheck->isChecked())
		font_feature << "-liga";
	if (!ContextualCheck->isChecked())
		font_feature << "-clig";
	if (DiscretionaryCheck->isChecked())
		font_feature << "+dlig";
	if (HistoricalCheck->isChecked())
		font_feature << "+hlig";

	//Capitals
	if (SmallRadio->isChecked())
		font_feature << "+smcp";
	if (SmallFromCRadio->isChecked())
		font_feature << "+c2sc";
	if (PetiteRadio->isChecked())
		font_feature << "+pcap";
	if (PetiteCapRadio->isChecked())
		font_feature << "+c2pc";
	if (UnicaseRadio->isChecked())
		font_feature << "+unic";
	if (TiltingRadio->isChecked())
		font_feature << "+titl";

	//Numeric
	if (LiningRadio->isChecked())
		font_feature << "+lnum";
	if (OldStyleRadio->isChecked())
		font_feature << "+onum";
	if (ProportionalRadio->isChecked())
		font_feature << "+pnum";
	if (TabularRadio->isChecked())
		font_feature << "+tnum";
	if (DiagonalRadio->isChecked())
		font_feature << "+frac";
	if (StackedRadio->isChecked())
		font_feature <<"+afrc";

	//Position
	if (SubscriptRadio->isChecked())
		font_feature << "+subs";
	if (SuperscriptRadio->isChecked())
		font_feature << "+sups";
	if (OrdinalCheck->isChecked())
		font_feature << "+ordn";

	if (SlashedZeroCheck->isChecked())
		font_feature << "+zero";

	// Stylistic sets
	if (StyleSet01->isChecked())
		font_feature << "+ss01";
	if (StyleSet02->isChecked())
		font_feature << "+ss02";
	if (StyleSet03->isChecked())
		font_feature << "+ss03";
	if (StyleSet04->isChecked())
		font_feature << "+ss04";
	if (StyleSet05->isChecked())
		font_feature << "+ss05";
	if (StyleSet06->isChecked())
		font_feature << "+ss06";
	if (StyleSet07->isChecked())
		font_feature << "+ss07";
	if (StyleSet08->isChecked())
		font_feature << "+ss08";
	if (StyleSet09->isChecked())
		font_feature << "+ss09";
	if (StyleSet10->isChecked())
		font_feature << "+ss10";
	if (StyleSet11->isChecked())
		font_feature << "+ss11";
	if (StyleSet12->isChecked())
		font_feature << "+ss12";
	if (StyleSet13->isChecked())
		font_feature << "+ss13";
	if (StyleSet14->isChecked())
		font_feature << "+ss14";
	if (StyleSet15->isChecked())
		font_feature << "+ss15";
	if (StyleSet16->isChecked())
		font_feature << "+ss16";
	if (StyleSet17->isChecked())
		font_feature << "+ss17";
	if (StyleSet18->isChecked())
		font_feature << "+ss18";
	if (StyleSet19->isChecked())
		font_feature << "+ss19";
	if (StyleSet20->isChecked())
		font_feature << "+ss20";

	Selection tempSelection(this, false);
	tempSelection.addItem(m_item, true);
	m_doc->itemSelection_SetFontFeatures(font_feature.join(","), &tempSelection);
}

void PropertyWidget_FontFeatures::setDoc(ScribusDoc *d)
{
	if((d == (ScribusDoc*) m_doc) || (m_ScMW && m_ScMW->scriptIsRunning()))
		return;

	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}

	m_doc  = d;
	m_item = NULL;

	if (m_doc.isNull())
	{
		disconnectSignals();
		return;
	}

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
}

void PropertyWidget_FontFeatures::handleSelectionChanged()
{
	if (!m_doc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem* currItem = currentItemFromSelection();
	setCurrentItem(currItem);
	updateGeometry();
	repaint();
}

void PropertyWidget_FontFeatures::updateCharStyle(const CharStyle& charStyle)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	showFontFeatures(charStyle.fontFeatures(), charStyle.font().fontFeatures());
}

void PropertyWidget_FontFeatures::updateStyle(const ParagraphStyle& newCurrent)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	const CharStyle& charStyle = newCurrent.charStyle();
	showFontFeatures(charStyle.fontFeatures(), charStyle.font().fontFeatures());
}

void PropertyWidget_FontFeatures::connectSignals()
{
	connect(CommonCheck, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(ContextualCheck, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(DiscretionaryCheck, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(HistoricalCheck, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);

	connect(NormalCapRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(SmallRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(SmallFromCRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(PetiteRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(PetiteCapRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(UnicaseRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(TiltingRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);

	connect(DefaultStyleRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(LiningRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(OldStyleRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);

	connect(DefaultWidthRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(ProportionalRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(TabularRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);

	connect(DefaultFractionsRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(DiagonalRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(StackedRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);

	connect(DefaultPosRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(SubscriptRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(SuperscriptRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(OrdinalCheck, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);

	connect(SlashedZeroCheck, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);

	connect(StyleSet01, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(StyleSet02, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(StyleSet03, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(StyleSet04, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(StyleSet05, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(StyleSet06, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(StyleSet07, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(StyleSet08, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(StyleSet09, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(StyleSet10, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(StyleSet11, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(StyleSet12, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(StyleSet13, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(StyleSet14, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(StyleSet15, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(StyleSet16, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(StyleSet17, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(StyleSet18, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(StyleSet19, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(StyleSet20, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
}

void PropertyWidget_FontFeatures::disconnectSignals()
{
	disconnect(CommonCheck, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(ContextualCheck, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(DiscretionaryCheck, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(HistoricalCheck, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));

	disconnect(NormalCapRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(SmallRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(SmallFromCRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(PetiteRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(PetiteCapRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(UnicaseRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(TiltingRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));

	disconnect(DefaultStyleRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(LiningRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(OldStyleRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));

	disconnect(DefaultWidthRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(ProportionalRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(TabularRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));

	disconnect(DefaultFractionsRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(DiagonalRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(StackedRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));

	disconnect(DefaultPosRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(SubscriptRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(SuperscriptRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(OrdinalCheck, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));

	disconnect(SlashedZeroCheck, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));

	disconnect(StyleSet01, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(StyleSet02, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(StyleSet03, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(StyleSet04, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(StyleSet05, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(StyleSet06, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(StyleSet07, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(StyleSet08, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(StyleSet09, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(StyleSet10, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(StyleSet11, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(StyleSet12, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(StyleSet13, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(StyleSet14, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(StyleSet15, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(StyleSet16, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(StyleSet17, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(StyleSet18, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(StyleSet19, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(StyleSet20, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
}

void PropertyWidget_FontFeatures::disableAllFeatures()
{
	CommonCheck->setChecked(true);
	ContextualCheck->setChecked(true);
	DiscretionaryCheck->setChecked(false);
	HistoricalCheck->setChecked(false);
	
	NormalCapRadio->setChecked(true);
	DefaultStyleRadio->setChecked(true);
	DefaultWidthRadio->setChecked(true);
	DefaultFractionsRadio->setChecked(true);
	DefaultPosRadio->setChecked(true);
	SlashedZeroCheck->setChecked(false);

	StyleSet01->setChecked(false);
	StyleSet02->setChecked(false);
	StyleSet03->setChecked(false);
	StyleSet04->setChecked(false);
	StyleSet05->setChecked(false);
	StyleSet06->setChecked(false);
	StyleSet07->setChecked(false);
	StyleSet08->setChecked(false);
	StyleSet09->setChecked(false);
	StyleSet10->setChecked(false);
	StyleSet11->setChecked(false);
	StyleSet12->setChecked(false);
	StyleSet13->setChecked(false);
	StyleSet14->setChecked(false);
	StyleSet15->setChecked(false);
	StyleSet16->setChecked(false);
	StyleSet17->setChecked(false);
	StyleSet18->setChecked(false);
	StyleSet19->setChecked(false);
	StyleSet20->setChecked(false);
}

void PropertyWidget_FontFeatures::enableFeatures(QStringList fontFeatures)
{
	quint64 oldFlags = featureFlags();
	quint64 newFlags = featureFlagsFromList(fontFeatures);

	disableAllFeatures();

	CommonCheck->setVisible(newFlags & FeatureCommonLigatures);
	ContextualCheck->setVisible(newFlags & FeatureContextualLigatures);
	DiscretionaryCheck->setVisible(newFlags & FeatureDiscretionaryLigatures);
	HistoricalCheck->setVisible(newFlags & FeatureHistoricalLigatures);

	SmallRadio->setVisible(newFlags & FeatureSmallCaps);
	SmallFromCRadio->setVisible(newFlags & FeatureSmallCapsFromCaps);
	PetiteRadio->setVisible(newFlags & FeaturePetiteCaps);
	PetiteCapRadio->setVisible(newFlags & FeaturePetiteCapsFromCaps);
	UnicaseRadio->setVisible(newFlags & FeatureUnicaseCaps);
	TiltingRadio->setVisible(newFlags & FeatureTiltingCaps);

	LiningRadio->setVisible(newFlags & FeatureLiningNumerals);
	OldStyleRadio->setVisible(newFlags & FeatureOldStyleNumerals);

	ProportionalRadio->setVisible(newFlags & FeatureProportionalNumeralWidth);
	TabularRadio->setVisible(newFlags & FeatureTabularNumeralWidth);

	DiagonalRadio->setVisible(newFlags & FeatureDiagonalFractions);
	StackedRadio->setVisible(newFlags & FeatureStackedFractions);

	SubscriptRadio->setVisible(newFlags & FeatureSubscript);
	SuperscriptRadio->setVisible(newFlags & FeatureSuperscript);
	OrdinalCheck->setVisible(newFlags & FeatureOrdinals);

	SlashedZeroCheck->setVisible(newFlags & FeatureSlashedZero);

	StyleSet01->setVisible(newFlags & FeatureStyleSet01);
	StyleSet02->setVisible(newFlags & FeatureStyleSet02);
	StyleSet03->setVisible(newFlags & FeatureStyleSet03);
	StyleSet04->setVisible(newFlags & FeatureStyleSet04);
	StyleSet05->setVisible(newFlags & FeatureStyleSet05);
	StyleSet06->setVisible(newFlags & FeatureStyleSet06);
	StyleSet07->setVisible(newFlags & FeatureStyleSet07);
	StyleSet08->setVisible(newFlags & FeatureStyleSet08);
	StyleSet09->setVisible(newFlags & FeatureStyleSet09);
	StyleSet10->setVisible(newFlags & FeatureStyleSet10);
	StyleSet11->setVisible(newFlags & FeatureStyleSet11);
	StyleSet12->setVisible(newFlags & FeatureStyleSet12);
	StyleSet13->setVisible(newFlags & FeatureStyleSet13);
	StyleSet14->setVisible(newFlags & FeatureStyleSet14);
	StyleSet15->setVisible(newFlags & FeatureStyleSet15);
	StyleSet16->setVisible(newFlags & FeatureStyleSet16);
	StyleSet17->setVisible(newFlags & FeatureStyleSet17);
	StyleSet18->setVisible(newFlags & FeatureStyleSet18);
	StyleSet19->setVisible(newFlags & FeatureStyleSet19);
	StyleSet20->setVisible(newFlags & FeatureStyleSet20);

	quint64 featureStyleSetMask = 0;
	featureStyleSetMask += FeatureStyleSet01 + FeatureStyleSet02 + FeatureStyleSet03 + FeatureStyleSet04 + FeatureStyleSet05;
	featureStyleSetMask += FeatureStyleSet06 + FeatureStyleSet07 + FeatureStyleSet08 + FeatureStyleSet09 + FeatureStyleSet10;
	featureStyleSetMask += FeatureStyleSet11 + FeatureStyleSet12 + FeatureStyleSet13 + FeatureStyleSet14 + FeatureStyleSet15;
	featureStyleSetMask += FeatureStyleSet16 + FeatureStyleSet17 + FeatureStyleSet18 + FeatureStyleSet19 + FeatureStyleSet20;
	groupBox_7->setVisible(newFlags & featureStyleSetMask);

	// Hide Default features when their relative ones aren't found in the font.
	quint64 featureCapsMask = 0;
	featureCapsMask += FeatureSmallCaps + FeatureSmallCapsFromCaps;
	featureCapsMask += FeaturePetiteCaps + FeaturePetiteCapsFromCaps;
	featureCapsMask += FeatureUnicaseCaps + FeatureTiltingCaps;
	NormalCapRadio->setVisible(newFlags & featureCapsMask);

	quint64 featureNumeralStyleMask = FeatureLiningNumerals + FeatureOldStyleNumerals;
	DefaultStyleRadio->setVisible(newFlags & featureNumeralStyleMask);

	quint64 featureNumeralWidthMask = FeatureProportionalNumeralWidth + FeatureTabularNumeralWidth;
	DefaultWidthRadio->setVisible(newFlags & featureNumeralWidthMask);
	
	quint64 featureFractionMask = FeatureDiagonalFractions + FeatureStackedFractions;
	DefaultFractionsRadio->setVisible(newFlags & featureFractionMask);

	quint64 featureGlyphPositionMask = FeatureSubscript + FeatureSuperscript + FeatureOrdinals;
	DefaultPosRadio->setVisible(newFlags & featureGlyphPositionMask);

	// Hide Lines
	quint64 featureLigaturesMask = FeatureCommonLigatures + FeatureContextualLigatures;
	featureLigaturesMask += FeatureDiscretionaryLigatures + FeatureHistoricalLigatures;
	line->setVisible(newFlags & featureLigaturesMask);

	line_2->setVisible(newFlags & featureCapsMask);
	line_3->setVisible(newFlags & featureNumeralStyleMask);
	line_4->setVisible(newFlags & featureNumeralWidthMask);
	line_5->setVisible(newFlags & featureFractionMask);
	line_6->setVisible(newFlags & featureGlyphPositionMask);
	line_7->setVisible(SlashedZeroCheck->isVisible());

	// Do not trigger item relayout unnecessarily,
	// that can hurt text typing speed
	if (oldFlags != featureFlags())
		emit needsRelayout();
}

quint64 PropertyWidget_FontFeatures::featureFlags()
{
	quint64 flags = 0;

	if (CommonCheck->isVisible())
		flags |= FeatureCommonLigatures;
	if (ContextualCheck->isVisible())
		flags |= FeatureContextualLigatures;
	if (DiscretionaryCheck->isVisible())
		flags |= FeatureDiscretionaryLigatures;
	if (HistoricalCheck->isVisible())
		flags |= FeatureHistoricalLigatures;

	if (SmallRadio->isVisible())
		flags |= FeatureSmallCaps;
	if (SmallFromCRadio->isVisible())
		flags |= FeatureSmallCapsFromCaps;
	if (PetiteRadio->isVisible())
		flags |= FeaturePetiteCaps;
	if (PetiteCapRadio->isVisible())
		flags |= FeaturePetiteCapsFromCaps;
	if (UnicaseRadio->isVisible())
		flags |= FeatureUnicaseCaps;
	if (TiltingRadio->isVisible())
		flags |= FeatureTiltingCaps;

	if (LiningRadio->isVisible())
		flags |= FeatureLiningNumerals;
	if (OldStyleRadio->isVisible())
		flags |= FeatureOldStyleNumerals;

	if (ProportionalRadio->isVisible())
		flags |= FeatureProportionalNumeralWidth;
	if (TabularRadio->isVisible())
		flags |= FeatureTabularNumeralWidth;

	if (DiagonalRadio->isVisible())
		flags |= FeatureDiagonalFractions;
	if (StackedRadio->isVisible())
		flags |= FeatureStackedFractions;

	if (SubscriptRadio->isVisible())
		flags |= FeatureSubscript;
	if (SuperscriptRadio->isVisible())
		flags |= FeatureSuperscript;
	if (OrdinalCheck->isVisible())
		flags |= FeatureOrdinals;

	if (SlashedZeroCheck->isVisible())
		flags |= FeatureSlashedZero;

	if (StyleSet01->isVisible())
		flags |= FeatureStyleSet01;
	if (StyleSet02->isVisible())
		flags |= FeatureStyleSet02;
	if (StyleSet03->isVisible())
		flags |= FeatureStyleSet03;
	if (StyleSet04->isVisible())
		flags |= FeatureStyleSet04;
	if (StyleSet05->isVisible())
		flags |= FeatureStyleSet05;
	if (StyleSet06->isVisible())
		flags |= FeatureStyleSet06;
	if (StyleSet07->isVisible())
		flags |= FeatureStyleSet07;
	if (StyleSet08->isVisible())
		flags |= FeatureStyleSet08;
	if (StyleSet09->isVisible())
		flags |= FeatureStyleSet09;
	if (StyleSet10->isVisible())
		flags |= FeatureStyleSet10;
	if (StyleSet11->isVisible())
		flags |= FeatureStyleSet11;
	if (StyleSet12->isVisible())
		flags |= FeatureStyleSet12;
	if (StyleSet13->isVisible())
		flags |= FeatureStyleSet13;
	if (StyleSet14->isVisible())
		flags |= FeatureStyleSet14;
	if (StyleSet15->isVisible())
		flags |= FeatureStyleSet15;
	if (StyleSet16->isVisible())
		flags |= FeatureStyleSet16;
	if (StyleSet17->isVisible())
		flags |= FeatureStyleSet17;
	if (StyleSet18->isVisible())
		flags |= FeatureStyleSet18;
	if (StyleSet19->isVisible())
		flags |= FeatureStyleSet19;
	if (StyleSet20->isVisible())
		flags |= FeatureStyleSet20;

	return flags;
}

quint64 PropertyWidget_FontFeatures::featureFlagsFromList(QStringList features)
{
	quint64 flags = 0;

	for (int i = 0; i < features.count(); i++)
	{
		const QString& feature = features.at(i);

		if (feature == "liga")
			flags |= FeatureCommonLigatures;
		else if (feature == "clig")
			flags |= FeatureContextualLigatures;
		else if (feature == "dlig")
			flags |= FeatureDiscretionaryLigatures;
		else if (feature == "hlig")
			flags |= FeatureHistoricalLigatures;

		else if (feature == "smcp")
			flags |= FeatureSmallCaps;
		else if (feature == "c2sc")
			flags |= FeatureSmallCapsFromCaps;
		else if (feature == "pcap")
			flags |= FeaturePetiteCaps;
		else if (feature == "c2pc")
			flags |= FeaturePetiteCapsFromCaps;
		else if (feature == "unic")
			flags |= FeatureUnicaseCaps;
		else if (feature == "titl")
			flags |= FeatureTiltingCaps;

		else if (feature == "lnum")
			flags |= FeatureLiningNumerals;
		else if (feature == "onum")
			flags |= FeatureOldStyleNumerals;

		else if (feature == "pnum")
			flags |= FeatureProportionalNumeralWidth;
		else if (feature == "tnum")
			flags |= FeatureTabularNumeralWidth;

		else if (feature == "frac")
			flags |= FeatureDiagonalFractions;
		else if (feature == "afrc")
			flags |= FeatureStackedFractions;

		else if (feature == "subs")
			flags |= FeatureSubscript;
		else if (feature == "sups")
			flags |= FeatureSuperscript;
		else if (feature == "ordn")
			flags |= FeatureOrdinals;

		else if (feature == "zero")
			flags |= FeatureSlashedZero;

		else if (feature == "ss01")
			flags |= FeatureStyleSet01;
		else if (feature == "ss02")
			flags |= FeatureStyleSet02;
		else if (feature == "ss03")
			flags |= FeatureStyleSet03;
		else if (feature == "ss04")
			flags |= FeatureStyleSet04;
		else if (feature == "ss05")
			flags |= FeatureStyleSet05;
		else if (feature == "ss06")
			flags |= FeatureStyleSet06;
		else if (feature == "ss07")
			flags |= FeatureStyleSet07;
		else if (feature == "ss08")
			flags |= FeatureStyleSet08;
		else if (feature == "ss09")
			flags |= FeatureStyleSet09;
		else if (feature == "ss10")
			flags |= FeatureStyleSet10;
		else if (feature == "ss11")
			flags |= FeatureStyleSet11;
		else if (feature == "ss12")
			flags |= FeatureStyleSet12;
		else if (feature == "ss13")
			flags |= FeatureStyleSet13;
		else if (feature == "ss14")
			flags |= FeatureStyleSet14;
		else if (feature == "ss15")
			flags |= FeatureStyleSet15;
		else if (feature == "ss16")
			flags |= FeatureStyleSet16;
		else if (feature == "ss17")
			flags |= FeatureStyleSet17;
		else if (feature == "ss18")
			flags |= FeatureStyleSet18;
		else if (feature == "ss19")
			flags |= FeatureStyleSet19;
		else if (feature == "ss20")
			flags |= FeatureStyleSet20;
	}

	return flags;
}

void PropertyWidget_FontFeatures::configureWidgets()
{
	bool enabled = false;
	if (m_item && m_doc)
	{
		if (m_item->asPathText() || m_item->asTextFrame() || m_item->asTable())
			enabled = true;
		if ((m_item->isGroup()) && (!m_item->isSingleSel))
			enabled = false;
		if (m_item->asOSGFrame() || m_item->asSymbolFrame())
			enabled = false;
		if (m_doc->m_Selection->count() > 1)
			enabled = false;
	}
	setEnabled(enabled);
}

void PropertyWidget_FontFeatures::initWidgets()
{
	CommonCheck->setChecked(true);
	ContextualCheck->setChecked(true);
	DiscretionaryCheck->setChecked(false);
	HistoricalCheck->setChecked(false);
	
	NormalCapRadio->setChecked(true);
	SmallRadio->setChecked(false);
	SmallFromCRadio->setChecked(false);
	PetiteRadio->setChecked(false);
	PetiteCapRadio->setChecked(false);
	UnicaseRadio->setChecked(false);
	TiltingRadio->setChecked(false);

	DefaultStyleRadio->setChecked(true);
	LiningRadio->setChecked(false);
	OldStyleRadio->setChecked(false);

	DefaultWidthRadio->setChecked(true);
	ProportionalRadio->setChecked(false);
	TabularRadio->setChecked(false);
	
	DefaultFractionsRadio->setChecked(true);
	DiagonalRadio->setChecked(false);
	StackedRadio->setChecked(false);

	DefaultPosRadio->setChecked(true);
	SubscriptRadio->setChecked(false);
	SuperscriptRadio->setChecked(false);
	OrdinalCheck->setChecked(false);

	SlashedZeroCheck->setChecked(false);

	StyleSet01->setChecked(false);
	StyleSet02->setChecked(false);
	StyleSet03->setChecked(false);
	StyleSet04->setChecked(false);
	StyleSet05->setChecked(false);
	StyleSet06->setChecked(false);
	StyleSet07->setChecked(false);
	StyleSet08->setChecked(false);
	StyleSet09->setChecked(false);
	StyleSet10->setChecked(false);
	StyleSet11->setChecked(false);
	StyleSet12->setChecked(false);
	StyleSet13->setChecked(false);
	StyleSet14->setChecked(false);
	StyleSet15->setChecked(false);
	StyleSet16->setChecked(false);
	StyleSet17->setChecked(false);
	StyleSet18->setChecked(false);
	StyleSet19->setChecked(false);
	StyleSet20->setChecked(false);
}

void PropertyWidget_FontFeatures::setCurrentItem(PageItem *item)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	if (item && m_doc.isNull())
		setDoc(item->doc());

	m_item = item;

	disconnectSignals();
	configureWidgets();

	if (m_item)
	{
		if (m_item->asTextFrame() || m_item->asPathText() || m_item->asTable())
		{
			ParagraphStyle parStyle =  m_item->itemText.defaultStyle();
			if (m_doc->appMode == modeEdit)
				m_item->currentTextProps(parStyle);
			else if (m_doc->appMode == modeEditTable)
				m_item->asTable()->activeCell().textFrame()->currentTextProps(parStyle);
			updateStyle(parStyle);
		}
		connectSignals();
	}
}
