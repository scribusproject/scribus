/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QButtonGroup>

#include "propertywidget_fontfeatures.h"
#include "iconmanager.h"
#include "localemgr.h"
#include "appmodes.h"
#include "pageitem_table.h"
#include "scribus.h"
#include "scribusapp.h"
#include "scribusdoc.h"
#include "selection.h"

#include "fonts/fontfeatures.h"


PropertyWidget_FontFeatures::PropertyWidget_FontFeatures(QWidget* parent) : QFrame(parent)
{
	setupUi(this);

	layout()->setAlignment(Qt::AlignTop);

	initWidgets();
	iconSetChange();
	languageChange();
	localeChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(ScQApp, SIGNAL(localeChanged()), this, SLOT(localeChange()));
	connect(ScQApp, SIGNAL(labelVisibilityChanged(bool)), this, SLOT(toggleLabelVisibility(bool)));
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

bool PropertyWidget_FontFeatures::eventFilter(QObject *obj, QEvent *e)
{
	if (e->type() == QEvent::Enter)
	{
		handleFontPreview(obj);
		return true;
	}
	if (e->type() == QEvent::Leave)
	{
		fontPreview->setPixmap(QPixmap());
		return true;
	}

	return QWidget::eventFilter(obj, e);
}


void PropertyWidget_FontFeatures::iconSetChange()
{
	IconManager& im = IconManager::instance();

	// Literal Cases
	SmallRadio->setIcon(im.loadIcon("ff-smcp"));
	SmallFromCRadio->setIcon(im.loadIcon("ff-c2sc"));
	AllSmallCapsRadio->setIcon(im.loadIcon("ff-c2sc-smcp"));
	PetiteRadio->setIcon(im.loadIcon("ff-pcap"));
	PetiteCapRadio->setIcon(im.loadIcon("ff-p2pc"));
	AllPetiteCapsRadio->setIcon(im.loadIcon("ff-p2pc-pcap"));
	UnicaseRadio->setIcon(im.loadIcon("ff-unic"));
	TitlingRadio->setIcon(im.loadIcon("ff-titl"));
	// Number Style
	LiningRadio->setIcon(im.loadIcon("ff-lnum"));
	OldStyleRadio->setIcon(im.loadIcon("ff-onum"));
	// Number Width
	ProportionalRadio->setIcon(im.loadIcon("ff-pnum"));
	TabularRadio->setIcon(im.loadIcon("ff-tnum"));
	// Number Fractions
	DiagonalRadio->setIcon(im.loadIcon("ff-frac"));
	StackedRadio->setIcon(im.loadIcon("ff-afrc"));
	// Number Position
	SubscriptRadio->setIcon(im.loadIcon("ff-subs"));
	SuperscriptRadio->setIcon(im.loadIcon("ff-sups"));
	OrdinalCheck->setIcon(im.loadIcon("ff-ordn"));

}

void PropertyWidget_FontFeatures::languageChange()
{
	retranslateUi(this);
}

void PropertyWidget_FontFeatures::localeChange()
{
	//const QLocale& l(LocaleManager::instance().userPreferredLocale());
}

void PropertyWidget_FontFeatures::toggleLabelVisibility(bool v)
{
	// Nothing to do!
}

void PropertyWidget_FontFeatures::showFontFeatures(const QString& s, const QStringList& availableFeatures)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;

	enableFeatures(availableFeatures);

	QStringList fontFeatures = s.split(',');
	quint64 featureFlags = FontFeatures::textFlagsFromList(fontFeatures);

	ContextualCheck->setChecked(true);
	CommonCheck->setChecked(true);
	SlashedZeroCheck->setEnabled(true);

	if ((featureFlags & FontFeatures::CommonLigatures) == 0)
		CommonCheck->setChecked(false);
	if ((featureFlags & FontFeatures::ContextualLigatures) == 0)
		ContextualCheck->setChecked(false);
	if (featureFlags & FontFeatures::DiscretionaryLigatures)
		DiscretionaryCheck->setChecked(true);
	if (featureFlags & FontFeatures::HistoricalLigatures)
		HistoricalCheck->setChecked(true);

	if ((featureFlags & FontFeatures::AllSmallCapsMask) == FontFeatures::AllSmallCapsMask)
		AllSmallCapsRadio->setChecked(true);
	else if (featureFlags & FontFeatures::SmallCaps)
		SmallRadio->setChecked(true);
	else if (featureFlags & FontFeatures::SmallCapsFromCaps)
		SmallFromCRadio->setChecked(true);
	if ((featureFlags & FontFeatures::AllPetiteCapsMask) == FontFeatures::AllPetiteCapsMask)
		AllPetiteCapsRadio->setChecked(true);
	else if (featureFlags & FontFeatures::PetiteCaps)
		PetiteRadio->setChecked(true);
	else if (featureFlags & FontFeatures::PetiteCapsFromCaps)
		PetiteCapRadio->setChecked(true);
	if (featureFlags & FontFeatures::UnicaseCaps)
		UnicaseRadio->setChecked(true);
	if (featureFlags & FontFeatures::TiltingCaps)
		TitlingRadio->setChecked(true);

	if (featureFlags & FontFeatures::LiningNumerals)
		LiningRadio->setChecked(true);
	if (featureFlags & FontFeatures::OldStyleNumerals)
		OldStyleRadio->setChecked(true);

	if (featureFlags & FontFeatures::ProportionalNumeralWidth)
		ProportionalRadio->setChecked(true);
	if (featureFlags & FontFeatures::TabularNumeralWidth)
		TabularRadio->setChecked(true);

	if (featureFlags & FontFeatures::DiagonalFractions)
		DiagonalRadio->setChecked(true);
	if (featureFlags & FontFeatures::StackedFractions)
		StackedRadio->setChecked(true);

	if (featureFlags & FontFeatures::Subscript)
		SubscriptRadio->setChecked(true);
	if (featureFlags & FontFeatures::Superscript)
		SuperscriptRadio->setChecked(true);
	if (featureFlags & FontFeatures::Ordinals)
		OrdinalCheck->setChecked(true);

	if (featureFlags & FontFeatures::SlashedZero)
		SlashedZeroCheck->setChecked(true);

	if (featureFlags & FontFeatures::StyleSet01)
		StyleSet01->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet02)
		StyleSet02->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet03)
		StyleSet03->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet04)
		StyleSet04->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet05)
		StyleSet05->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet06)
		StyleSet06->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet07)
		StyleSet07->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet08)
		StyleSet08->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet09)
		StyleSet09->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet10)
		StyleSet10->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet11)
		StyleSet11->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet12)
		StyleSet12->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet13)
		StyleSet13->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet14)
		StyleSet14->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet15)
		StyleSet15->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet16)
		StyleSet16->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet17)
		StyleSet17->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet18)
		StyleSet18->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet19)
		StyleSet19->setChecked(true);
	if (featureFlags & FontFeatures::StyleSet20)
		StyleSet20->setChecked(true);

	if (OldStyleRadio->isChecked())
	{
		SlashedZeroCheck->setChecked(false);
		SlashedZeroCheck->setEnabled(false);
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
	if (SmallRadio->isChecked() || AllSmallCapsRadio->isChecked())
		font_feature << "+smcp";
	if (SmallFromCRadio->isChecked() || AllSmallCapsRadio->isChecked())
		font_feature << "+c2sc";
	if (PetiteRadio->isChecked() || AllPetiteCapsRadio->isChecked())
		font_feature << "+pcap";
	if (PetiteCapRadio->isChecked() || AllPetiteCapsRadio->isChecked())
		font_feature << "+c2pc";
	if (UnicaseRadio->isChecked())
		font_feature << "+unic";
	if (TitlingRadio->isChecked())
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
	if (SlashedZeroCheck->isChecked())
		font_feature << "+zero";

	//Position
	if (SubscriptRadio->isChecked())
		font_feature << "+subs";
	if (SuperscriptRadio->isChecked())
		font_feature << "+sups";
	if (OrdinalCheck->isChecked())
		font_feature << "+ordn";

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

void PropertyWidget_FontFeatures::handleFontPreview(QObject *obj)
{
	IconManager& im = IconManager::instance();
	int h = 48;

	if (obj == (QObject*)CommonCheck)
		fontPreview->setPixmap(im.loadPixmap("ff-preview-liga", QSize(-1, h)));
	else if (obj == (QObject*)ContextualCheck)
		fontPreview->setPixmap(im.loadPixmap("ff-preview-clig", QSize(-1, h)));
	else if (obj == (QObject*)DiscretionaryCheck)
		fontPreview->setPixmap(im.loadPixmap("ff-preview-dlig", QSize(-1, h)));
	else if (obj == (QObject*)HistoricalCheck)
		fontPreview->setPixmap(im.loadPixmap("ff-preview-hlig", QSize(-1, h)));
	else if (obj == (QObject*)AllSmallCapsRadio)
		fontPreview->setPixmap(im.loadPixmap("ff-preview-c2sc-smcp", QSize(-1, h)));
	else if (obj == (QObject*)SmallRadio)
		fontPreview->setPixmap(im.loadPixmap("ff-preview-smcp", QSize(-1, h)));
	else if (obj == (QObject*)SmallFromCRadio)
		fontPreview->setPixmap(im.loadPixmap("ff-preview-c2sc", QSize(-1, h)));
	else if (obj == (QObject*)AllPetiteCapsRadio)
		fontPreview->setPixmap(im.loadPixmap("ff-preview-p2pc-pcap", QSize(-1, h)));
	else if (obj == (QObject*)PetiteRadio)
		fontPreview->setPixmap(im.loadPixmap("ff-preview-pcap", QSize(-1, h)));
	else if (obj == (QObject*)PetiteCapRadio)
		fontPreview->setPixmap(im.loadPixmap("ff-preview-p2pc", QSize(-1, h)));
	else if (obj == (QObject*)UnicaseRadio)
		fontPreview->setPixmap(im.loadPixmap("ff-preview-unic", QSize(-1, h)));
	// else if (obj == (QObject*)TitlingRadio)
	// 	fontPreview->setPixmap(im.loadPixmap("", QSize(-1, h)));
	else if (obj == (QObject*)LiningRadio)
		fontPreview->setPixmap(im.loadPixmap("ff-preview-lnum", QSize(-1, h)));
	else if (obj == (QObject*)OldStyleRadio)
		fontPreview->setPixmap(im.loadPixmap("ff-preview-onum", QSize(-1, h)));
	else if (obj == (QObject*)ProportionalRadio)
		fontPreview->setPixmap(im.loadPixmap("ff-preview-pnum", QSize(-1, h)));
	else if (obj == (QObject*)TabularRadio)
		fontPreview->setPixmap(im.loadPixmap("ff-preview-tnum", QSize(-1, h)));
	else if (obj == (QObject*)DiagonalRadio)
		fontPreview->setPixmap(im.loadPixmap("ff-preview-frac", QSize(-1, h)));
	else if (obj == (QObject*)StackedRadio)
		fontPreview->setPixmap(im.loadPixmap("ff-preview-afrc", QSize(-1, h)));
	else if (obj == (QObject*)SubscriptRadio)
		fontPreview->setPixmap(im.loadPixmap("ff-preview-subs", QSize(-1, h)));
	else if (obj == (QObject*)SuperscriptRadio)
		fontPreview->setPixmap(im.loadPixmap("ff-preview-sups", QSize(-1, h)));
	else if (obj == (QObject*)OrdinalCheck)
		fontPreview->setPixmap(im.loadPixmap("ff-preview-ordn", QSize(-1, h)));
	else if (obj == (QObject*)SlashedZeroCheck)
		fontPreview->setPixmap(im.loadPixmap("ff-preview-zero", QSize(-1, h)));
	else
		fontPreview->setPixmap(QPixmap());
}

void PropertyWidget_FontFeatures::setDoc(ScribusDoc *d)
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
	update();
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
	connect(AllSmallCapsRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(PetiteRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(PetiteCapRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(AllPetiteCapsRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(UnicaseRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);
	connect(TitlingRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()), Qt::UniqueConnection);

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
	disconnect(AllSmallCapsRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(PetiteRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(PetiteCapRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(AllPetiteCapsRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(UnicaseRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));
	disconnect(TitlingRadio, SIGNAL(clicked()), this, SLOT(handleFontFeatures()));

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

void PropertyWidget_FontFeatures::enableFeatures(const QStringList& fontFeatures)
{
	quint64 oldFlags = featureFlags();
	quint64 newFlags = FontFeatures::fontFlagsFromList(fontFeatures);

	disableAllFeatures();

	// Ligatures
	CommonCheck->setVisible(newFlags & FontFeatures::CommonLigatures);
	labelCommonLigatures->setVisible(newFlags & FontFeatures::CommonLigatures);
	ContextualCheck->setVisible(newFlags & FontFeatures::ContextualLigatures);
	labelContextualLigatures->setVisible(newFlags & FontFeatures::ContextualLigatures);
	DiscretionaryCheck->setVisible(newFlags & FontFeatures::DiscretionaryLigatures);
	labelDiscretionaryLigatures->setVisible(newFlags & FontFeatures::DiscretionaryLigatures);
	HistoricalCheck->setVisible(newFlags & FontFeatures::HistoricalLigatures);
	labelHistoricalLigatures->setVisible(newFlags & FontFeatures::HistoricalLigatures);

	// Capitals
	NormalCapRadio->setVisible(newFlags & FontFeatures::CapsMask);
	labelCapitals->setVisible(newFlags & FontFeatures::CapsMask);
	SmallRadio->setVisible(newFlags & FontFeatures::SmallCaps);
	SmallFromCRadio->setVisible(newFlags & FontFeatures::SmallCapsFromCaps);
	AllSmallCapsRadio->setVisible((newFlags & FontFeatures::AllSmallCapsMask) == FontFeatures::AllSmallCapsMask);
	PetiteRadio->setVisible(newFlags & FontFeatures::PetiteCaps);
	PetiteCapRadio->setVisible(newFlags & FontFeatures::PetiteCapsFromCaps);
	AllPetiteCapsRadio->setVisible((newFlags & FontFeatures::AllPetiteCapsMask) == FontFeatures::AllPetiteCapsMask);
	UnicaseRadio->setVisible(newFlags & FontFeatures::UnicaseCaps);
	TitlingRadio->setVisible(newFlags & FontFeatures::TiltingCaps);

	// Numeral Style
	DefaultStyleRadio->setVisible(newFlags & FontFeatures::NumeralStyleMask);
	labelNumeralStyle->setVisible(newFlags & FontFeatures::NumeralStyleMask);
	LiningRadio->setVisible(newFlags & FontFeatures::LiningNumerals);
	OldStyleRadio->setVisible(newFlags & FontFeatures::OldStyleNumerals);

	// Numeral Width
	DefaultWidthRadio->setVisible(newFlags & FontFeatures::NumeralWidthMask);
	labelNumeralWidth->setVisible(newFlags & FontFeatures::NumeralWidthMask);
	ProportionalRadio->setVisible(newFlags & FontFeatures::ProportionalNumeralWidth);
	TabularRadio->setVisible(newFlags & FontFeatures::TabularNumeralWidth);

	// Numeral Fractions
	DefaultFractionsRadio->setVisible(newFlags & FontFeatures::NumeralFractionsMask);
	labelFraction->setVisible(newFlags & FontFeatures::NumeralFractionsMask);
	DiagonalRadio->setVisible(newFlags & FontFeatures::DiagonalFractions);
	StackedRadio->setVisible(newFlags & FontFeatures::StackedFractions);

	// Numeral Positions
	DefaultPosRadio->setVisible(newFlags & FontFeatures::GlyphPositionMask);
	LabelNumeralPosition->setVisible(newFlags & FontFeatures::GlyphPositionMask);
	SubscriptRadio->setVisible(newFlags & FontFeatures::Subscript);
	SuperscriptRadio->setVisible(newFlags & FontFeatures::Superscript);
	OrdinalCheck->setVisible(newFlags & FontFeatures::Ordinals);

	// Numeral Slash Zero
	SlashedZeroCheck->setVisible(newFlags & FontFeatures::SlashedZero);
	labelSlashedZero->setVisible(newFlags & FontFeatures::SlashedZero);

	// Stylistic Sets
	StyleSet01->setVisible(newFlags & FontFeatures::StyleSet01);
	StyleSet02->setVisible(newFlags & FontFeatures::StyleSet02);
	StyleSet03->setVisible(newFlags & FontFeatures::StyleSet03);
	StyleSet04->setVisible(newFlags & FontFeatures::StyleSet04);
	StyleSet05->setVisible(newFlags & FontFeatures::StyleSet05);
	StyleSet06->setVisible(newFlags & FontFeatures::StyleSet06);
	StyleSet07->setVisible(newFlags & FontFeatures::StyleSet07);
	StyleSet08->setVisible(newFlags & FontFeatures::StyleSet08);
	StyleSet09->setVisible(newFlags & FontFeatures::StyleSet09);
	StyleSet10->setVisible(newFlags & FontFeatures::StyleSet10);
	StyleSet11->setVisible(newFlags & FontFeatures::StyleSet11);
	StyleSet12->setVisible(newFlags & FontFeatures::StyleSet12);
	StyleSet13->setVisible(newFlags & FontFeatures::StyleSet13);
	StyleSet14->setVisible(newFlags & FontFeatures::StyleSet14);
	StyleSet15->setVisible(newFlags & FontFeatures::StyleSet15);
	StyleSet16->setVisible(newFlags & FontFeatures::StyleSet16);
	StyleSet17->setVisible(newFlags & FontFeatures::StyleSet17);
	StyleSet18->setVisible(newFlags & FontFeatures::StyleSet18);
	StyleSet19->setVisible(newFlags & FontFeatures::StyleSet19);
	StyleSet20->setVisible(newFlags & FontFeatures::StyleSet20);

	groupBox_7->setVisible(newFlags & FontFeatures::StyleSetsMask);

	// Hide Headers
	labelLigatures->setVisible(newFlags & FontFeatures::LigaturesMask);
	labelLettercase->setVisible(
		(newFlags & FontFeatures::CapsMask) ||
		(newFlags & FontFeatures::GlyphPositionMask)
	);
	LabelNumbers->setVisible(
		(newFlags & FontFeatures::NumeralStyleMask) ||
		(newFlags & FontFeatures::NumeralWidthMask) ||
		(newFlags & FontFeatures::NumeralFractionsMask) ||
		(newFlags & FontFeatures::SlashedZero)
	);
	fontPreview->setVisible(
		(newFlags & FontFeatures::LigaturesMask) ||
		(newFlags & FontFeatures::CapsMask) ||
		(newFlags & FontFeatures::NumeralStyleMask) ||
		(newFlags & FontFeatures::NumeralWidthMask) ||
		(newFlags & FontFeatures::NumeralFractionsMask) ||
		(newFlags & FontFeatures::GlyphPositionMask) ||
		(newFlags & FontFeatures::SlashedZero) ||
		(newFlags & FontFeatures::StyleSetsMask)
	);

	// Do not trigger item relayout unnecessarily,
	// that can hurt text typing speed
	if (oldFlags != featureFlags())
		emit needsRelayout();
}

quint64 PropertyWidget_FontFeatures::featureFlags() const
{
	quint64 flags = 0;

	if (CommonCheck->isVisible())
		flags |= FontFeatures::CommonLigatures;
	if (ContextualCheck->isVisible())
		flags |= FontFeatures::ContextualLigatures;
	if (DiscretionaryCheck->isVisible())
		flags |= FontFeatures::DiscretionaryLigatures;
	if (HistoricalCheck->isVisible())
		flags |= FontFeatures::HistoricalLigatures;

	if (SmallRadio->isVisible())
		flags |= FontFeatures::SmallCaps;
	if (SmallFromCRadio->isVisible())
		flags |= FontFeatures::SmallCapsFromCaps;
	if (PetiteRadio->isVisible())
		flags |= FontFeatures::PetiteCaps;
	if (PetiteCapRadio->isVisible())
		flags |= FontFeatures::PetiteCapsFromCaps;
	if (UnicaseRadio->isVisible())
		flags |= FontFeatures::UnicaseCaps;
	if (TitlingRadio->isVisible())
		flags |= FontFeatures::TiltingCaps;

	if (LiningRadio->isVisible())
		flags |= FontFeatures::LiningNumerals;
	if (OldStyleRadio->isVisible())
		flags |= FontFeatures::OldStyleNumerals;

	if (ProportionalRadio->isVisible())
		flags |= FontFeatures::ProportionalNumeralWidth;
	if (TabularRadio->isVisible())
		flags |= FontFeatures::TabularNumeralWidth;

	if (DiagonalRadio->isVisible())
		flags |= FontFeatures::DiagonalFractions;
	if (StackedRadio->isVisible())
		flags |= FontFeatures::StackedFractions;

	if (SubscriptRadio->isVisible())
		flags |= FontFeatures::Subscript;
	if (SuperscriptRadio->isVisible())
		flags |= FontFeatures::Superscript;
	if (OrdinalCheck->isVisible())
		flags |= FontFeatures::Ordinals;

	if (SlashedZeroCheck->isVisible())
		flags |= FontFeatures::SlashedZero;

	if (StyleSet01->isVisible())
		flags |= FontFeatures::StyleSet01;
	if (StyleSet02->isVisible())
		flags |= FontFeatures::StyleSet02;
	if (StyleSet03->isVisible())
		flags |= FontFeatures::StyleSet03;
	if (StyleSet04->isVisible())
		flags |= FontFeatures::StyleSet04;
	if (StyleSet05->isVisible())
		flags |= FontFeatures::StyleSet05;
	if (StyleSet06->isVisible())
		flags |= FontFeatures::StyleSet06;
	if (StyleSet07->isVisible())
		flags |= FontFeatures::StyleSet07;
	if (StyleSet08->isVisible())
		flags |= FontFeatures::StyleSet08;
	if (StyleSet09->isVisible())
		flags |= FontFeatures::StyleSet09;
	if (StyleSet10->isVisible())
		flags |= FontFeatures::StyleSet10;
	if (StyleSet11->isVisible())
		flags |= FontFeatures::StyleSet11;
	if (StyleSet12->isVisible())
		flags |= FontFeatures::StyleSet12;
	if (StyleSet13->isVisible())
		flags |= FontFeatures::StyleSet13;
	if (StyleSet14->isVisible())
		flags |= FontFeatures::StyleSet14;
	if (StyleSet15->isVisible())
		flags |= FontFeatures::StyleSet15;
	if (StyleSet16->isVisible())
		flags |= FontFeatures::StyleSet16;
	if (StyleSet17->isVisible())
		flags |= FontFeatures::StyleSet17;
	if (StyleSet18->isVisible())
		flags |= FontFeatures::StyleSet18;
	if (StyleSet19->isVisible())
		flags |= FontFeatures::StyleSet19;
	if (StyleSet20->isVisible())
		flags |= FontFeatures::StyleSet20;

	return flags;
}

void PropertyWidget_FontFeatures::configureWidgets()
{
	bool enabled = false;
	if (m_item && m_doc)
	{
		if (m_item->isPathText() || m_item->isTextFrame() || m_item->isTable())
			enabled = true;
		if ((m_item->isGroup()) && (!m_item->isSingleSel))
			enabled = false;
		if (m_item->isOSGFrame() || m_item->isSymbol())
			enabled = false;
		if (m_doc->m_Selection->count() > 1)
			enabled = false;
	}
	setEnabled(enabled);
}

void PropertyWidget_FontFeatures::initWidgets()
{
	fontPreview->setFixedHeight(50);

	CommonCheck->installEventFilter(this);
	ContextualCheck->installEventFilter(this);
	DiscretionaryCheck->installEventFilter(this);
	HistoricalCheck->installEventFilter(this);
	AllSmallCapsRadio->installEventFilter(this);
	SmallRadio->installEventFilter(this);
	SmallFromCRadio->installEventFilter(this);
	AllPetiteCapsRadio->installEventFilter(this);
	PetiteRadio->installEventFilter(this);
	PetiteCapRadio->installEventFilter(this);
	UnicaseRadio->installEventFilter(this);
	TitlingRadio->installEventFilter(this);
	LiningRadio->installEventFilter(this);
	OldStyleRadio->installEventFilter(this);
	ProportionalRadio->installEventFilter(this);
	TabularRadio->installEventFilter(this);
	DiagonalRadio->installEventFilter(this);
	StackedRadio->installEventFilter(this);
	SubscriptRadio->installEventFilter(this);
	SuperscriptRadio->installEventFilter(this);
	OrdinalCheck->installEventFilter(this);
	SlashedZeroCheck->installEventFilter(this);

	QButtonGroup * groupCapitals = new QButtonGroup();
	groupCapitals->setExclusive(true);
	groupCapitals->addButton(NormalCapRadio);
	groupCapitals->addButton(SmallRadio);
	groupCapitals->addButton(SmallFromCRadio);
	groupCapitals->addButton(AllSmallCapsRadio);
	groupCapitals->addButton(PetiteRadio);
	groupCapitals->addButton(PetiteCapRadio);
	groupCapitals->addButton(AllPetiteCapsRadio);
	groupCapitals->addButton(UnicaseRadio);
	groupCapitals->addButton(TitlingRadio);

	QButtonGroup * groupFractions = new QButtonGroup();
	groupFractions->setExclusive(true);
	groupFractions->addButton(DefaultFractionsRadio);
	groupFractions->addButton(DiagonalRadio);
	groupFractions->addButton(StackedRadio);

	QButtonGroup * groupNumeralStyle = new QButtonGroup();
	groupNumeralStyle->setExclusive(true);
	groupNumeralStyle->addButton(DefaultStyleRadio);
	groupNumeralStyle->addButton(LiningRadio);
	groupNumeralStyle->addButton(OldStyleRadio);

	QButtonGroup * groupNumeralWidth = new QButtonGroup();
	groupNumeralWidth->setExclusive(true);
	groupNumeralWidth->addButton(DefaultWidthRadio);
	groupNumeralWidth->addButton(ProportionalRadio);
	groupNumeralWidth->addButton(TabularRadio);


	CommonCheck->setChecked(true);
	ContextualCheck->setChecked(true);
	DiscretionaryCheck->setChecked(false);
	HistoricalCheck->setChecked(false);
	
	NormalCapRadio->setChecked(true);
	// SmallRadio->setChecked(false);
	// SmallFromCRadio->setChecked(false);
	// AllSmallCapsRadio->setChecked(false);
	// PetiteRadio->setChecked(false);
	// PetiteCapRadio->setChecked(false);
	// AllPetiteCapsRadio->setChecked(false);
	// UnicaseRadio->setChecked(false);
	// TitlingRadio->setChecked(false);

	DefaultStyleRadio->setChecked(true);
	// LiningRadio->setChecked(false);
	// OldStyleRadio->setChecked(false);

	DefaultWidthRadio->setChecked(true);
	// ProportionalRadio->setChecked(false);
	// TabularRadio->setChecked(false);
	
	DefaultFractionsRadio->setChecked(true);
	// DiagonalRadio->setChecked(false);
	// StackedRadio->setChecked(false);

	DefaultPosRadio->setChecked(true);
	// SubscriptRadio->setChecked(false);
	// SuperscriptRadio->setChecked(false);
	// OrdinalCheck->setChecked(false);

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
		if (m_item->isTextFrame() || m_item->isPathText() || m_item->isTable())
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
