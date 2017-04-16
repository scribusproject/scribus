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

PropertyWidget_FontFeatures::PropertyWidget_FontFeatures(QWidget* parent) : QFrame(parent)
{
	m_item = NULL;
	m_ScMW = NULL;
	setupUi(this);

	setFrameStyle(QFrame::Box | QFrame::Plain);
	setLineWidth(1);
	layout()->setAlignment( Qt::AlignTop );
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
	CommonCheck->setChecked(true);
	NormalCaRadio->setChecked(true);
	NormalRadio->setChecked(true);
	DefaultStyleRadio->setChecked(true);
	DefaultWidthRadio->setChecked(true);
	DefaultFractionsRadio->setChecked(true);
	ContextualCheck->setChecked(true);
	DiscretinoryCheck->setChecked(false);
	HistoricalCheck->setChecked(false);
	SubscriptRadio->setChecked(false);
	SuperscriptRaido->setChecked(false);
	SmallRadio->setChecked(false);
	SmallFromCRadio->setChecked(false);
	PetiteRadio->setChecked(false);
	PetiteCapRadio->setChecked(false);
	UnicaseRadio->setChecked(false);
	TiltingRadio->setChecked(false);
	LininRadio->setChecked(false);
	OldStyleRadio->setChecked(false);
	ProportionalRadio->setChecked(false);
	TabularRadio->setChecked(false);
	DiagonalRadio->setChecked(false);
	StackedRadio->setChecked(false);
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
			DiscretinoryCheck->setChecked(true);
		else if (fontFeatures[i] == "+hlig")
			HistoricalCheck->setChecked(true);
		else if (fontFeatures[i] == "+subs")
			SubscriptRadio->setChecked(true);
		else if (fontFeatures[i] == "+sups")
			SuperscriptRaido->setChecked(true);
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
			LininRadio->setChecked(true);
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

void PropertyWidget_FontFeatures::handlefontfeatures()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
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

	if (SubscriptRadio->isChecked())
		font_feature << "+subs";
	if (SuperscriptRaido->isChecked())
		font_feature <<"+sups";


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
	if (LininRadio->isChecked())
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
	connect(ContextualCheck, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(CommonCheck, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(DiscretinoryCheck, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(HistoricalCheck, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(NormalRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(SubscriptRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(SuperscriptRaido, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(NormalCaRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(SmallRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(SmallFromCRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(PetiteRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(PetiteCapRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(UnicaseRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(TiltingRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(DefaultStyleRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(LininRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(OldStyleRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(DefaultWidthRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(ProportionalRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(TabularRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(DefaultFractionsRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(DiagonalRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(StackedRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(OrdinalCheck, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(SlashedZeroCheck, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(StyleSet01, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(StyleSet02, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(StyleSet03, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(StyleSet04, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(StyleSet05, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(StyleSet06, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(StyleSet07, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(StyleSet08, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(StyleSet09, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(StyleSet10, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(StyleSet11, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(StyleSet12, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(StyleSet13, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(StyleSet14, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(StyleSet15, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(StyleSet16, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(StyleSet17, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(StyleSet18, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(StyleSet19, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	connect(StyleSet20, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
}

void PropertyWidget_FontFeatures::disconnectSignals()
{
	disconnect(ContextualCheck, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(CommonCheck, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(DiscretinoryCheck, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(HistoricalCheck, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(NormalRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(SubscriptRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(SuperscriptRaido, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(NormalCaRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(SmallRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(SmallFromCRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(PetiteRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(PetiteCapRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(UnicaseRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(TiltingRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(DefaultStyleRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(LininRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(OldStyleRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(DefaultWidthRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(ProportionalRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(TabularRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(DefaultFractionsRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(DiagonalRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(StackedRadio, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(OrdinalCheck, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(SlashedZeroCheck, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(StyleSet01, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(StyleSet02, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(StyleSet03, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(StyleSet04, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(StyleSet05, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(StyleSet06, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(StyleSet07, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(StyleSet08, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(StyleSet09, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(StyleSet10, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(StyleSet11, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(StyleSet12, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(StyleSet13, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(StyleSet14, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(StyleSet15, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(StyleSet16, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(StyleSet17, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(StyleSet18, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(StyleSet19, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
	disconnect(StyleSet20, SIGNAL(clicked()), this, SLOT(handlefontfeatures()));
}

void PropertyWidget_FontFeatures::disableAllFeatures()
{
	ContextualCheck->hide();
	CommonCheck->hide();
	DiscretinoryCheck->hide();
	HistoricalCheck->hide();
	line->hide();
	
	NormalCaRadio->setChecked(true);
	NormalCaRadio->hide();
	SmallRadio->hide();
	SmallFromCRadio->hide();
	PetiteRadio->hide();
	PetiteCapRadio->hide();
	UnicaseRadio->hide();
	TiltingRadio->hide();
	line_3->hide();
	
	DefaultStyleRadio->setChecked(true);
	DefaultStyleRadio->hide();
	LininRadio->hide();
	OldStyleRadio->hide();
	line_5->hide();
	
	DefaultWidthRadio->setChecked(true);
	DefaultWidthRadio->hide();
	ProportionalRadio->hide();
	TabularRadio->hide();
	line_2->hide();
	
	DefaultFractionsRadio->setChecked(true);
	DefaultFractionsRadio->hide();
	DiagonalRadio->hide();
	StackedRadio->hide();
	line_4->hide();
	
	NormalRadio->setChecked(true);
	NormalRadio->hide();
	SubscriptRadio->hide();
	SuperscriptRaido->hide();
	OrdinalCheck->hide();
	line_6->hide();
	
	SlashedZeroCheck->setChecked(false);
	SlashedZeroCheck->hide();
	line_7->hide();

	StyleSet01->hide();
	StyleSet02->hide();
	StyleSet03->hide();
	StyleSet04->hide();
	StyleSet05->hide();
	StyleSet06->hide();
	StyleSet07->hide();
	StyleSet08->hide();
	StyleSet09->hide();
	StyleSet10->hide();
	StyleSet11->hide();
	StyleSet12->hide();
	StyleSet13->hide();
	StyleSet14->hide();
	StyleSet15->hide();
	StyleSet16->hide();
	StyleSet17->hide();
	StyleSet18->hide();
	StyleSet19->hide();
	StyleSet20->hide();
	groupBox_7->hide();

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
	disableAllFeatures();

	line->show();
	line_2->show();
	line_3->show();
	line_4->show();
	line_5->show();
	line_6->show();
	line_7->show();

	NormalCaRadio->show();
	DefaultStyleRadio->show();
	DefaultWidthRadio->show();
	DefaultFractionsRadio->show();
	NormalRadio->show();

	for (int i = 0; i < fontFeatures.count(); i++)
	{
		if (fontFeatures[i] == "clig")
			ContextualCheck->hide();
		else if (fontFeatures[i] == "liga")
			CommonCheck->show();
		else if (fontFeatures[i] == "dlig")
			DiscretinoryCheck->show();
		else if (fontFeatures[i] == "hlig")
			HistoricalCheck->show();
		else if (fontFeatures[i] == "subs")
			SubscriptRadio->show();
		else if (fontFeatures[i] == "sups")
			SuperscriptRaido->show();
		else if (fontFeatures[i] == "smcp")
			SmallRadio->show();
		else if (fontFeatures[i] == "c2sc")
			SmallFromCRadio->show();
		else if (fontFeatures[i] == "pcap")
			PetiteRadio->show();
		else if (fontFeatures[i] == "c2pc")
			PetiteCapRadio->show();
		else if (fontFeatures[i] == "unic")
			UnicaseRadio->show();
		else if (fontFeatures[i] == "titl")
			TiltingRadio->show();
		else if (fontFeatures[i] == "lnum")
			LininRadio->show();
		else if (fontFeatures[i] == "onum")
			OldStyleRadio->show();
		else if (fontFeatures[i] == "pnum")
			ProportionalRadio->show();
		else if (fontFeatures[i] == "tnum")
			TabularRadio->show();
		else if (fontFeatures[i] == "frac")
			DiagonalRadio->show();
		else if (fontFeatures[i] == "afrc")
			StackedRadio->show();
		else if (fontFeatures[i] == "ordn")
			OrdinalCheck->show();
		else if (fontFeatures[i] == "zero")
			SlashedZeroCheck->show();
		else if (fontFeatures[i] == "ss01")
		{
			groupBox_7->show();
			StyleSet01->show();
		}
		else if (fontFeatures[i] == "ss02")
		{
			groupBox_7->show();
			StyleSet02->show();
		}
		else if (fontFeatures[i] == "ss03")
		{
			groupBox_7->show();
			StyleSet03->show();
		}
		else if (fontFeatures[i] == "ss04")
		{
			groupBox_7->show();
			StyleSet04->show();
		}
		else if (fontFeatures[i] == "ss05")
		{
			groupBox_7->show();
			StyleSet05->show();
		}
		else if (fontFeatures[i] == "ss06")
		{
			groupBox_7->show();
			StyleSet06->show();
		}
		else if (fontFeatures[i] == "ss07")
		{
			groupBox_7->show();
			StyleSet07->show();
		}
		else if (fontFeatures[i] == "ss08")
		{
			groupBox_7->show();
			StyleSet08->show();
		}
		else if (fontFeatures[i] == "ss09")
		{
			groupBox_7->show();
			StyleSet09->show();
		}
		else if (fontFeatures[i] == "ss10")
		{
			groupBox_7->show();
			StyleSet10->show();
		}
		else if (fontFeatures[i] == "ss11")
		{
			groupBox_7->show();
			StyleSet11->show();
		}
		else if (fontFeatures[i] == "ss12")
		{
			groupBox_7->show();
			StyleSet12->show();
		}
		else if (fontFeatures[i] == "ss13")
		{
			groupBox_7->show();
			StyleSet13->show();
		}
		else if (fontFeatures[i] == "ss14")
		{
			groupBox_7->show();
			StyleSet14->show();
		}
		else if (fontFeatures[i] == "ss15")
		{
			groupBox_7->show();
			StyleSet15->show();
		}
		else if (fontFeatures[i] == "ss16")
		{
			groupBox_7->show();
			StyleSet16->show();
		}
		else if (fontFeatures[i] == "ss17")
		{
			groupBox_7->show();
			StyleSet17->show();
		}
		else if (fontFeatures[i] == "ss18")
		{
			groupBox_7->show();
			StyleSet18->show();
		}
		else if (fontFeatures[i] == "ss19")
		{
			groupBox_7->show();
			StyleSet19->show();
		}
		else if (fontFeatures[i] == "ss20")
		{
			groupBox_7->show();
			StyleSet20->show();
		}
	}

	// Hide Default features when their relative ones aren't found in the font.
	if (SmallRadio->isHidden()
			&& SmallFromCRadio->isHidden() && PetiteRadio->isHidden()
			&& PetiteCapRadio->isHidden() && UnicaseRadio->isHidden()
			&& TiltingRadio->isHidden())
		NormalCaRadio->hide();

	if (LininRadio->isHidden() && OldStyleRadio->isHidden())
		DefaultStyleRadio->hide();

	if (ProportionalRadio->isHidden() && TabularRadio->isHidden())
		DefaultWidthRadio->hide();

	if (DiagonalRadio->isHidden() && StackedRadio->isHidden())
		DefaultFractionsRadio->hide();

	if (SubscriptRadio->isHidden() && SuperscriptRaido->isHidden() && OrdinalCheck->isHidden())
		NormalRadio->hide();

	// Hide Lines
	if (ContextualCheck->isHidden() && CommonCheck->isHidden()
			&& DiscretinoryCheck->isHidden() && HistoricalCheck->isHidden())
		line->hide();
	if (NormalCaRadio->isHidden() && SmallRadio->isHidden()
			&& SmallFromCRadio->isHidden() && PetiteRadio->isHidden()
			&& PetiteCapRadio->isHidden() && UnicaseRadio->isHidden()
			&& TiltingRadio->isHidden())
		line_3->hide();
	if (DefaultStyleRadio->isHidden() && LininRadio->isHidden()
			&& OldStyleRadio->isHidden())
		line_5->hide();
	if (DefaultWidthRadio->isHidden() && ProportionalRadio->isHidden()
			&& TabularRadio->isHidden())
		line_2->hide();
	if (DefaultFractionsRadio->isHidden() && DiagonalRadio->isHidden()
			&& StackedRadio->isHidden())
		line_4->hide();
	if (NormalRadio->isHidden() && SubscriptRadio->isHidden()
			&& SuperscriptRaido->isHidden() && OrdinalCheck->isHidden())
		line_6->hide();
	if (SlashedZeroCheck->isHidden() )
		line_7->hide();

	// Do not trigger item relayout unnecessarily,
	// that can hurt text typing speed
	if (oldFlags != featureFlags())
		emit needsRelayout();
}

quint64 PropertyWidget_FontFeatures::featureFlags()
{
	quint64 flags = 0;

	if (ContextualCheck->isVisible())
		flags |= 1;
	if (CommonCheck->isVisible())
		flags |= ((quint64) 1 << 1);
	if (DiscretinoryCheck->isVisible())
		flags |= ((quint64) 1 << 2);
	if (HistoricalCheck->isVisible())
		flags |= ((quint64) 1 << 3);
	if (SubscriptRadio->isVisible())
		flags |= ((quint64) 1 << 4);
	if (SuperscriptRaido->isVisible())
		flags |= ((quint64) 1 << 5);
	if (SmallRadio->isVisible())
		flags |= ((quint64) 1 << 6);
	if (SmallFromCRadio->isVisible())
		flags |= ((quint64) 1 << 7);
	if (PetiteRadio->isVisible())
		flags |= ((quint64) 1 << 8);
	if (PetiteCapRadio->isVisible())
		flags |= ((quint64) 1 << 9);
	if (UnicaseRadio->isVisible())
		flags |= ((quint64) 1 << 10);
	if (TiltingRadio->isVisible())
		flags |= ((quint64) 1 << 11);
	if (LininRadio->isVisible())
		flags |= ((quint64) 1 << 12);
	if (OldStyleRadio->isVisible())
		flags |= ((quint64) 1 << 13);
	if (ProportionalRadio->isVisible())
		flags |= ((quint64) 1 << 14);
	if (TabularRadio->isVisible())
		flags |= ((quint64) 1 << 15);
	if (DiagonalRadio->isVisible())
		flags |= ((quint64) 1 << 16);
	if (StackedRadio->isVisible())
		flags |= ((quint64) 1 << 17);
	if (OrdinalCheck->isVisible())
		flags |= ((quint64) 1 << 18);
	if (SlashedZeroCheck->isVisible())
		flags |= ((quint64) 1 << 19);
	if (StyleSet01->isVisible())
		flags |= ((quint64) 1 << 20);
	if (StyleSet02->isVisible())
		flags |= ((quint64) 1 << 21);
	if (StyleSet03->isVisible())
		flags |= ((quint64) 1 << 22);
	if (StyleSet04->isVisible())
		flags |= ((quint64) 1 << 23);
	if (StyleSet05->isVisible())
		flags |= ((quint64) 1 << 24);
	if (StyleSet06->isVisible())
		flags |= ((quint64) 1 << 25);
	if (StyleSet07->isVisible())
		flags |= ((quint64) 1 << 26);
	if (StyleSet08->isVisible())
		flags |= ((quint64) 1 << 27);
	if (StyleSet09->isVisible())
		flags |= ((quint64) 1 << 28);
	if (StyleSet10->isVisible())
		flags |= ((quint64) 1 << 29);
	if (StyleSet11->isVisible())
		flags |= ((quint64) 1 << 30);
	if (StyleSet12->isVisible())
		flags |= ((quint64) 1 << 31);
	if (StyleSet13->isVisible())
		flags |= ((quint64) 1 << 32);
	if (StyleSet14->isVisible())
		flags |= ((quint64) 1 << 33);
	if (StyleSet15->isVisible())
		flags |= ((quint64) 1 << 34);
	if (StyleSet16->isVisible())
		flags |= ((quint64) 1 << 35);
	if (StyleSet17->isVisible())
		flags |= ((quint64) 1 << 36);
	if (StyleSet18->isVisible())
		flags |= ((quint64) 1 << 37);
	if (StyleSet19->isVisible())
		flags |= ((quint64) 1 << 38);
	if (StyleSet20->isVisible())
		flags |= ((quint64) 1 << 39);

	return flags;
}

void PropertyWidget_FontFeatures::configureWidgets(void)
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
