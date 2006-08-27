/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "edit1format.h"
#include "edit1format.moc"
#include "tabruler.h"
#include "units.h"

#include <qmessagebox.h>
#include <qtooltip.h>
#include <qcolordialog.h>
#include <qcursor.h>

#include "colorcombo.h"
#include "commonstrings.h"
#include "scribusdoc.h"
#include "styleselect.h"
#include "scribusstructs.h"
#include "sctextstruct.h"
#include "scpaths.h"
#include "scribus.h"
#include "util.h"
#include "loremipsum.h"
#include "prefsmanager.h"
#include "sampleitem.h"


EditStyle::EditStyle( QWidget* parent, ParagraphStyle *vor, QValueList<ParagraphStyle> v, bool neu, double au, int dEin, ScribusDoc *doc)
		: QDialog( parent, "EditST", true, 0)
{
	qApp->setOverrideCursor(QCursor(waitCursor));
	parentDoc = doc;
	setCaption( tr( "Edit Style" ) );
	setIcon(loadIcon("AppIcon.png"));
	AutoVal = au;
	DocsEin = dEin;

	sampleItem = new SampleItem(doc);
	sampleItem->setLoremIpsum(2);
	sampleItem->cleanupTemporary();

	EditStyleLayout = new QVBoxLayout( this, 10, 5, "EditStyleLayout");

	TextLabel1 = new QLabel( tr( "&Name:" ), this, "TextLabel1" );
	EditStyleLayout->addWidget( TextLabel1 );

	Name = new QLineEdit( this, "Name" );
	Name->setMinimumSize( QSize( 200, 22 ) );
	Name->setText( vor->name() );
	OldName = vor->name();
	IsNew = neu;
	TextLabel1->setBuddy(Name);
	EditStyleLayout->addWidget( Name );

	layout9 = new QHBoxLayout( 0, 0, 5, "layout9");

	GroupFont = new QGroupBox( tr( "Character" ), this, "GroupFont" );
	GroupFont->setColumnLayout(0, Qt::Vertical );
	GroupFont->layout()->setSpacing( 5 );
	GroupFont->layout()->setMargin( 10 );
	GroupFontLayout = new QVBoxLayout( GroupFont->layout() );
	GroupFontLayout->setAlignment( Qt::AlignLeft );
	FontC = new FontComboH(GroupFont);
	FontC->setCurrentFont(vor->charStyle().font().scName());
	GroupFontLayout->addWidget( FontC );
	layout7 = new QHBoxLayout( 0, 0, 5, "layout7");
	SizeC = new MSpinBox( 1, 2048, GroupFont, 1 );
	SizeC->setMinimumSize( QSize( 70, 22 ) );
	SizeC->setSuffix( tr( " pt" ) );
	SizeC->setValue(vor->charStyle().fontSize() / 10.0);
	TextF2 = new QLabel( "" ,GroupFont, "TextF2" );
	TextF2->setPixmap(loadIcon("Zeichen.xpm"));
	TextF2->setMinimumSize( QSize( 22, 22 ) );
	TextF2->setMaximumSize( QSize( 22, 22 ) );
	layout7->addWidget( TextF2 );
	layout7->addWidget( SizeC );
	pixmapLabel3 = new QLabel( "", GroupFont, "pixmapLabel3" );
	pixmapLabel3->setMinimumSize( QSize( 22, 22 ) );
	pixmapLabel3->setMaximumSize( QSize( 22, 22 ) );
	pixmapLabel3->setPixmap( loadIcon("textscaleh.png") );
	layout7->addWidget( pixmapLabel3 );
	fontHScale = new MSpinBox( 10, 400, GroupFont, 1 );
	fontHScale->setValue( vor->charStyle().scaleH() / 10.0 );
	fontHScale->setSuffix( tr( " %" ) );
	layout7->addWidget( fontHScale );
	pixmapLabel3_2 = new QLabel( "", GroupFont, "pixmapLabel3_2" );
	pixmapLabel3_2->setMinimumSize( QSize( 22, 22 ) );
	pixmapLabel3_2->setMaximumSize( QSize( 22, 22 ) );
	pixmapLabel3_2->setPixmap( loadIcon("textscalev.png") );
	layout7->addWidget( pixmapLabel3_2 );

	fontVScale = new MSpinBox( 10, 400, GroupFont, 1 );
	fontVScale->setValue( vor->charStyle().scaleV() / 10.0 );
	fontVScale->setSuffix( tr( " %" ) );
	layout7->addWidget( fontVScale );
	GroupFontLayout->addLayout( layout7 );

	layout9a = new QHBoxLayout( 0, 0, 0, "layout9");
	EffeS = new StyleSelect(GroupFont);
	EffeS->setStyle(vor->charStyle().effects());
	EffeS->ShadowVal->Xoffset->setValue(vor->charStyle().shadowXOffset() / 10.0);
	EffeS->ShadowVal->Yoffset->setValue(vor->charStyle().shadowYOffset() / 10.0);
	EffeS->OutlineVal->LWidth->setValue(vor->charStyle().outlineWidth() / 10.0);
	EffeS->UnderlineVal->LPos->setValue(vor->charStyle().underlineOffset() / 10.0);
	EffeS->UnderlineVal->LWidth->setValue(vor->charStyle().underlineWidth() / 10.0);
	EffeS->StrikeVal->LPos->setValue(vor->charStyle().strikethruOffset() / 10.0);
	EffeS->StrikeVal->LWidth->setValue(vor->charStyle().strikethruWidth() / 10.0);
	layout9a->addWidget( EffeS );
	QSpacerItem* spacer1 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout9a->addItem( spacer1 );
	GroupFontLayout->addLayout( layout9a, Qt::AlignLeft );

	layout9b = new QHBoxLayout( 0, 0, 0, "layout9");
	AligS = new AlignSelect(GroupFont);
	AligS->setStyle(vor->alignment());
	layout9b->addWidget( AligS );
	QSpacerItem* spacer2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout9a->addItem( spacer2 );
	GroupFontLayout->addLayout( layout9b, Qt::AlignLeft );

	layout5 = new QHBoxLayout( 0, 0, 5, "layout5");
	FillIcon = new QLabel( "", GroupFont, "FillIcon" );
	FillIcon->setPixmap(loadIcon("fill.png"));
	layout5->addWidget( FillIcon );
	TxFill = new ColorCombo( false, GroupFont, "TxFill" );
	layout5->addWidget( TxFill );
	pixmapLabel3_20 = new QLabel( GroupFont, "pixmapLabel3_20" );
	pixmapLabel3_20->setMinimumSize( QSize( 22, 22 ) );
	pixmapLabel3_20->setMaximumSize( QSize( 22, 22 ) );
	pixmapLabel3_20->setPixmap( loadIcon("shade.png") );
	layout5->addWidget( pixmapLabel3_20 );
	PM2 = new ShadeButton(GroupFont);
	layout5->addWidget( PM2 );
	QSpacerItem* spacer3 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout5->addItem( spacer3 );
	GroupFontLayout->addLayout( layout5 );

	layout6 = new QHBoxLayout( 0, 0, 5, "layout6");
	StrokeIcon = new QLabel( "", GroupFont, "StrokeIcon" );
	StrokeIcon->setPixmap(loadIcon("Stiftalt.xpm"));
	layout6->addWidget( StrokeIcon );
	TxStroke = new ColorCombo( false, GroupFont, "TxStroke" );
	layout6->addWidget( TxStroke );
	pixmapLabel3_19 = new QLabel( "", GroupFont, "pixmapLabel3_19" );
	pixmapLabel3_19->setMinimumSize( QSize( 22, 22 ) );
	pixmapLabel3_19->setMaximumSize( QSize( 22, 22 ) );
	pixmapLabel3_19->setPixmap( loadIcon("shade.png") );
	layout6->addWidget( pixmapLabel3_19 );
	PM1 = new ShadeButton(GroupFont);
	layout6->addWidget( PM1 );
	QSpacerItem* spacer4 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout6->addItem( spacer4 );
	GroupFontLayout->addLayout( layout6 );

	TxFill->clear();
	TxStroke->clear();
	ColorList::Iterator it;
	QPixmap pm = QPixmap(15, 15);
	TxFill->insertItem(CommonStrings::NoneColor);
	TxStroke->insertItem(CommonStrings::NoneColor);
	for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
	{
		TxFill->insertSmallItem(doc->PageColors[it.key()], it.key());
		TxStroke->insertSmallItem(doc->PageColors[it.key()], it.key());
	}
	StrokeIcon->setEnabled(false);
	PM1->setEnabled(false);
	TxStroke->setEnabled(false);
	TxFill->setCurrentText(vor->charStyle().fillColor());
	TxStroke->setCurrentText(vor->charStyle().strokeColor());
	PM2->setValue(vor->charStyle().fillShade());
	PM1->setValue(vor->charStyle().strokeShade());

	layout9->addWidget( GroupFont );

	layout8 = new QVBoxLayout( 0, 0, 5, "layout8");
	AbstandV = new QGroupBox( tr("Distances"), this, "AbstandV" );
	AbstandV->setColumnLayout(0, Qt::Vertical );
	AbstandV->layout()->setSpacing( 0 );
	AbstandV->layout()->setMargin( 0 );
	AbstandVLayout = new QGridLayout( AbstandV->layout() );
	AbstandVLayout->setAlignment( Qt::AlignTop );
	AbstandVLayout->setSpacing( 5 );
	AbstandVLayout->setMargin( 10 );
	lineSpacingPop = new QPopupMenu();
	lineSpacingPop->insertItem( tr("Fixed Linespacing"));
	lineSpacingPop->insertItem( tr("Automatic Linespacing"));
	lineSpacingPop->insertItem( tr("Align to Baseline Grid"));
	linespacingButton = new QToolButton(AbstandV, "linespacingButton" );
	linespacingButton->setText("");
	linespacingButton->setPixmap( loadIcon("linespacing.png") );
	linespacingButton->setPopup(lineSpacingPop);
	linespacingButton->setPopupDelay(400);
	linespacingButton->setAutoRaise(true);
	AbstandVLayout->addWidget( linespacingButton, 0, 0 );
	LineSpVal = new MSpinBox( 1, 300, AbstandV, 1 );
	LineSpVal->setSuffix( tr( " pt" ) );
	LineSpVal->setValue(vor->lineSpacing());
	AbstandVLayout->addWidget( LineSpVal, 0, 1 );
	pixmapLabel2 = new QLabel( AbstandV, "pixmapLabel2" );
	pixmapLabel2->setMinimumSize( QSize( 22, 22 ) );
	pixmapLabel2->setMaximumSize( QSize( 22, 22 ) );
	pixmapLabel2->setPixmap( loadIcon("textbase.png") );
	AbstandVLayout->addWidget( pixmapLabel2, 0, 2 );
	fontBase = new MSpinBox( -100, 100, AbstandV, 1 );
	fontBase->setValue( vor->charStyle().baselineOffset() / 10.0 );
	fontBase->setSuffix( tr( " %" ) );
	AbstandVLayout->addWidget( fontBase, 0, 3 );
	pixmapLabel3_3 = new QLabel( AbstandV, "pixmapLabel3_3" );
	pixmapLabel3_3->setMinimumSize( QSize( 22, 22 ) );
	pixmapLabel3_3->setMaximumSize( QSize( 22, 22 ) );
	pixmapLabel3_3->setPixmap( loadIcon("textkern.png") );
	AbstandVLayout->addWidget( pixmapLabel3_3, 0, 4 );
	fontKern = new MSpinBox( -300, 300, AbstandV, 1 );
	fontKern->setValue( vor->charStyle().tracking() / 10.0 );
	fontKern->setSuffix( tr( " %" ) );
	AbstandVLayout->addWidget( fontKern, 0, 5 );

	TextLabel1_2_2 = new QLabel( "", AbstandV, "TextLabel1_2_2" );
	TextLabel1_2_2->setPixmap( loadIcon("above.png") );
	AbstandVLayout->addWidget( TextLabel1_2_2, 1, 0 );
	AboveV = new MSpinBox( 0, 300, AbstandV, 1 );
	AboveV->setSuffix( tr( " pt" ) );
	AbstandVLayout->addWidget( AboveV, 1, 1 );

	TextLabel1_2_3 = new QLabel( "", AbstandV, "TextLabel1_2_3" );
	TextLabel1_2_3->setPixmap( loadIcon("below.png") );
	AbstandVLayout->addWidget( TextLabel1_2_3, 1, 2 );
	BelowV = new MSpinBox( 0, 300, AbstandV, 1 );
	BelowV->setSuffix( tr( " pt" ) );
	AbstandVLayout->addWidget( BelowV, 1, 3 );
	layout8->addWidget( AbstandV );

	DropCaps = new QGroupBox( tr("Drop Caps"),  this, "groupCaps" );
	DropCaps->setCheckable( true );
	DropCaps->setColumnLayout(0, Qt::Vertical );
	DropCaps->layout()->setSpacing( 5 );
	DropCaps->layout()->setMargin( 10 );
	DropCapsLayout = new QGridLayout( DropCaps->layout() );
	DropCapsLayout->setAlignment( Qt::AlignTop );
	DropLines = new QSpinBox( DropCaps, "DropLines" );
	DropLines->setMinValue( 2 );
	DropLines->setMaxValue( 20 );
	DropLines->setValue(vor->dropCapLines());
	CapLabel = new QLabel( DropLines, tr("&Lines:"), DropCaps, "CapLabel" );
	DropCapsLayout->addWidget( CapLabel, 0, 0 );
	DropCapsLayout->addWidget( DropLines, 0, 1 );
	DropDist = new MSpinBox( -3000, 3000, DropCaps, 1);
	DropDist->setSuffix( tr( " pt" ) );
	DropDist->setValue(vor->dropCapOffset());
	CapLabel2 = new QLabel( DropLines, tr("Distance from Text:"), DropCaps, "CapLabel2" );
	DropCapsLayout->addWidget( CapLabel2, 1, 0 );
	DropCapsLayout->addWidget( DropDist, 1, 1 );
	DropCaps->setChecked(vor->hasDropCap());
	layout8->addWidget( DropCaps );
	layout9->addLayout( layout8 );
	EditStyleLayout->addLayout( layout9 );

	for (uint al = 0; al < lineSpacingPop->count(); ++al)
	{
		lineSpacingPop->setItemChecked(lineSpacingPop->idAt(al), false);
	}
	if (vor->useBaselineGrid())
	{
		LineSpVal->setEnabled(false);
		lineSpacingPop->setItemChecked(lineSpacingPop->idAt(2), true);
	}
	else
	{
		lineSpacingPop->setItemChecked(lineSpacingPop->idAt(vor->lineSpacingMode()), true);
		if (vor->lineSpacingMode() > 0)
			LineSpVal->setEnabled(false);
		else
			LineSpVal->setEnabled(true);
	}

	GroupBox10 = new QGroupBox( tr("Tabulators and Indentation"), this, "GroupBox10" );
	GroupBox10->setColumnLayout(0, Qt::Vertical );
	GroupBox10->layout()->setSpacing( 0 );
	GroupBox10->layout()->setMargin( 0 );
	GroupBox10Layout = new QVBoxLayout(GroupBox10->layout());
	GroupBox10Layout->setAlignment( Qt::AlignTop );
	GroupBox10Layout->setSpacing( 5 );
	GroupBox10Layout->setMargin( 10 );
	TabList = new Tabruler(GroupBox10, true, DocsEin, vor->tabValues(), -1);
	TabList->setLeftIndentData(vor->leftMargin());
	TabList->setLeftIndent();
	TabList->setFirstLineData(vor->firstIndent());
	TabList->setFirstLine();
	GroupBox10Layout->addWidget( TabList );
	EditStyleLayout->addWidget( GroupBox10 );

	// Label for holding "style preview" bitmap 12/30/2004 petr vanek
	layoutPreview = new QVBoxLayout; // paragraphs and check
	layoutPreview->setSpacing(6);
	layoutPreview->setMargin(0);
	layoutPrevSet = new QHBoxLayout; // all preview items
	layoutPrevSet->setSpacing(6);
	layoutPrevSet->setMargin(0);

	previewCaption = new QCheckBox( tr("Preview of the Paragraph Style"), this, "previewCaption" );
	previewCaption->setChecked(PrefsManager::instance()->appPrefs.haveStylePreview);
	QSpacerItem* spacerBg = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	previewBgColor = new QPushButton(this, "previewBgColor");
	previewBgColor->setText( tr("Background"));
	layoutPrevSet->addWidget(previewCaption);
	layoutPrevSet->addItem(spacerBg);
	layoutPrevSet->addWidget(previewBgColor);

	layoutPreview->addLayout(layoutPrevSet);

	previewText = new QLabel(this, "previewText");
	previewText->setMinimumSize(640, 200);
	previewText->setMaximumSize(640, 200);
	previewText->setAlignment( static_cast<int>( QLabel::AlignVCenter | QLabel::AlignLeft ) );
	previewText->setFrameShape(QFrame::Box);
	previewText->setPaletteBackgroundColor(paletteBackgroundColor());
	layoutPreview->addWidget(previewText);

	EditStyleLayout->addLayout(layoutPreview);

	Layout17 = new QHBoxLayout;
	Layout17->setSpacing( 6 );
	Layout17->setMargin( 0 );
	QSpacerItem* spacer2a = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout17->addItem( spacer2a );
	OkButton = new QPushButton( CommonStrings::tr_OK, this, "OkButton" );
	Layout17->addWidget( OkButton );
	Cancel = new QPushButton( CommonStrings::tr_Cancel, this, "Cancel" );
	OkButton->setDefault( true );
	Layout17->addWidget( Cancel );
	EditStyleLayout->addLayout(Layout17);

	werte = vor;
	allV = v;
	// tooltips
	QToolTip::add(previewBgColor, "<qt>" + tr("Select for easier reading of light coloured text styles") + "</qt>");
	QToolTip::add( Name, "<qt>" + tr( "Name of your paragraph style" ) + "</qt>" );
	QToolTip::add( FontC, "<qt>" + tr( "Font of selected text or object" ) + "</qt>" );
	QToolTip::add( SizeC, "<qt>" + tr( "Font Size" ) + "</qt>" );
	QToolTip::add( TxFill, "<qt>" + tr( "Color of text fill" ) + "</qt>" );
	QToolTip::add( TxStroke, "<qt>" + tr( "Color of text stroke" ) + "</qt>" );
	//	QToolTip::add( DropCaps, "<qt>" + tr( "Provides an oversized first letter for a paragraph. Used for stylistic effect" ) + "</qt>" );
	QToolTip::add( DropLines, "<qt>" + tr( "Determines the overall height, in line numbers, of the Drop Caps" ) );
	QToolTip::add( DropDist, "<qt>" + tr( "Determines the gap between the DropCaps and the Text" ) );
	//	QToolTip::add( BaseGrid, "<qt>" + tr( "Align text to baseline grid" ) + "</qt>" );
	QToolTip::add( AboveV, "<qt>" + tr( "Spacing above the paragraph" ) + "</qt>" );
	QToolTip::add( BelowV, "<qt>" + tr( "Spacing below the paragraph" ) + "</qt>" );
	QToolTip::add( fontKern, tr("Manual Tracking"));
	QToolTip::add( fontBase, tr("Offset to baseline of characters"));
	QToolTip::add( LineSpVal, "<qt>" + tr( "Line Spacing" ) + "</qt>" );
	QToolTip::add( linespacingButton, tr("Click to select the line spacing mode"));
	QToolTip::add( previewText, "<qt>" + tr( "Toggles sample text of this paragraph style" ) + "</qt>");

	// signals and slots connections
	connect( Cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( OkButton, SIGNAL( clicked() ), this, SLOT( Verlassen() ) );
	connect( DropCaps, SIGNAL( toggled(bool) ), this, SLOT( updatePreview() ) );
	connect(SizeC, SIGNAL(valueChanged(int)), this, SLOT(FontChange()));
	connect(fontVScale, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
	connect(fontHScale, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
	connect(EffeS, SIGNAL(State(int)), this, SLOT(ColorChange()));
	connect(EffeS->ShadowVal->Xoffset, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
	connect(EffeS->ShadowVal->Yoffset, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
	connect(EffeS->OutlineVal->LWidth, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
	connect(EffeS->UnderlineVal->LPos, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
	connect(EffeS->UnderlineVal->LWidth, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
	connect(EffeS->StrikeVal->LPos, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
	connect(EffeS->StrikeVal->LWidth, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
	// preview generators
//	connect(FontC, SIGNAL(activated(const QString &)), this, SLOT(updatePreview()));
	connect(FontC, SIGNAL(fontSelected(QString )), this, SLOT(updatePreview()));
	connect(LineSpVal, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
	connect(fontBase, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
	connect(fontKern, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
	connect(TabList, SIGNAL(tabrulerChanged()), this, SLOT(updatePreview()));
	connect(AboveV, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
	connect(BelowV, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
	connect(AligS, SIGNAL(State(int)), this, SLOT(updatePreview()));
	connect(TxStroke, SIGNAL(activated(const QString &)), this, SLOT(updatePreview()));
	connect(PM1, SIGNAL(clicked()), this, SLOT(updatePreview()));
	connect(TxFill, SIGNAL(activated(const QString &)), this, SLOT(updatePreview()));
	connect(PM2, SIGNAL(clicked()), this, SLOT(updatePreview()));
	connect(DropLines, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
	connect(DropDist, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
	connect(previewCaption, SIGNAL( clicked() ), this, SLOT( togglePreview() ) );
	connect(previewBgColor, SIGNAL(clicked()), this, SLOT(setPreviewBackground()));
	connect(lineSpacingPop, SIGNAL(activated(int)), this, SLOT(toggleLsp(int )));
	connect(fontKern, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));

	AboveV->setDecimals(10);
	BelowV->setDecimals(10);
	/* PFJ - 29.02.04 - Altered switch so only case 2 is tested */
	QString ein = unitGetSuffixFromIndex(dEin);
	if (dEin == 2)
	{
		AboveV->setDecimals(10000);
		BelowV->setDecimals(10000);
		DropDist->setDecimals(10000);
	}
	AboveV->setSuffix(ein);
	BelowV->setSuffix(ein);
	DropDist->setSuffix(ein);
	BelowV->setValue(vor->gapAfter() * parentDoc->unitRatio());
	AboveV->setValue(vor->gapBefore() * parentDoc->unitRatio());
	DropDist->setValue(vor->dropCapOffset() * parentDoc->unitRatio());
	ColorChange();
	togglePreview();
	qApp->restoreOverrideCursor();
}

EditStyle::~EditStyle()
{
	delete sampleItem;
}

void EditStyle::toggleLsp(int id)
{
	for (uint al = 0; al < lineSpacingPop->count(); ++al)
	{
		lineSpacingPop->setItemChecked(lineSpacingPop->idAt(al), false);
	}
	lineSpacingPop->setItemChecked(id, true);
	if (lineSpacingPop->indexOf(id) > 0)
		LineSpVal->setEnabled(false);
	else
		LineSpVal->setEnabled(true);
	updatePreview();
}

void EditStyle::togglePreview()
{
	if (previewCaption->isChecked())
	{
		previewText->show();
		previewText->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
		previewBgColor->show();
		updatePreview();
	}
	else
	{
		previewText->hide();
		previewBgColor->hide();
		previewText->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));
	}
	PrefsManager::instance()->appPrefs.haveStylePreview = previewCaption->isChecked();
	layout()->activate();
	resize(minimumSizeHint());
}

void EditStyle::ColorChange()
{
	int s = EffeS->getStyle();
	bool enabled;
	if ((s & 4) || (s & 256))
		enabled = true;
	else
		enabled = false;
	StrokeIcon->setEnabled(enabled);
	PM1->setEnabled(enabled);
	TxStroke->setEnabled(enabled);
	updatePreview();
}

void EditStyle::FontChange()
{
	double val = SizeC->value();
	LineSpVal->setValue((val  * AutoVal / 100) + val);
	updatePreview();
}

void EditStyle::Verlassen()
{
	QString mess=tr("Name of the style is not unique");
	if (IsNew)
	{
		for (uint x=0; x<allV.count()-1; ++x)
		{
			if (Name->text() == allV[x].name())
			{
				QMessageBox::information(this, CommonStrings::trWarning, mess, CommonStrings::tr_OK,0, 0, 0, QMessageBox::Ok);
				Name->selectAll();
				Name->setFocus();
				return;
			}
		}
	}
	else
	{
		if (OldName != Name->text())
		{
			for (uint x=0; x<allV.count(); ++x)
			{
				if (Name->text() == allV[x].name())
				{
					QMessageBox::information(this, CommonStrings::trWarning, mess, CommonStrings::tr_OK,0, 0, 0, QMessageBox::Ok);
					Name->selectAll();
					Name->setFocus();
					return;
				}
			}
		}
	}
	copyStyleSettings(*werte);
	accept();
}

void EditStyle::copyStyleSettings(ParagraphStyle& parstyle)
{
	CharStyle charstyle;
	charstyle.setEffects(static_cast<StyleFlag>(EffeS->getStyle()));
	parstyle.setAlignment(AligS->getStyle());
	for (uint al = 0; al < lineSpacingPop->count(); ++al)
	{
		if (lineSpacingPop->isItemChecked(lineSpacingPop->idAt(al)))
		{
			parstyle.setLineSpacingMode(static_cast<ParagraphStyle::LineSpacingMode>(al));
			parstyle.setUseBaselineGrid(al == 2);
			break;
		}
	}
	parstyle.setLineSpacing(LineSpVal->value());
	parstyle.setLeftMargin(QMAX(TabList->getLeftIndent(), 0.0));
	parstyle.setFirstIndent(TabList->getFirstLine());
	parstyle.setGapBefore(AboveV->value() / parentDoc->unitRatio());
	parstyle.setGapAfter(BelowV->value() / parentDoc->unitRatio());
	parstyle.setName(Name->text());
	charstyle.setFont(PrefsManager::instance()->appPrefs.AvailFonts[FontC->currentFont()]);
	charstyle.setFontSize(qRound(SizeC->value() * 10.0));
	parstyle.setHasDropCap(DropCaps->isChecked());
	parstyle.setDropCapLines(DropLines->value());
	parstyle.setDropCapOffset(DropDist->value() / parentDoc->unitRatio());
	charstyle.setFillColor(TxFill->currentText());
	charstyle.setFillShade(PM2->getValue());
	charstyle.setStrokeColor(TxStroke->currentText());
	charstyle.setStrokeShade(PM1->getValue());
	parstyle.tabValues() = TabList->getTabVals();
	charstyle.setShadowXOffset(qRound(EffeS->ShadowVal->Xoffset->value() * 10.0));
	charstyle.setShadowYOffset(qRound(EffeS->ShadowVal->Yoffset->value() * 10.0));
	charstyle.setOutlineWidth(qRound(EffeS->OutlineVal->LWidth->value() * 10.0));
	charstyle.setStrikethruOffset(qRound(EffeS->StrikeVal->LPos->value() * 10.0));
	charstyle.setStrikethruWidth(qRound(EffeS->StrikeVal->LWidth->value() * 10.0));
	charstyle.setUnderlineOffset(qRound(EffeS->UnderlineVal->LPos->value() * 10.0));
	charstyle.setUnderlineWidth(qRound(EffeS->UnderlineVal->LWidth->value() * 10.0));
	charstyle.setScaleH(qRound(fontHScale->value() * 10.0));
	charstyle.setScaleV(qRound(fontVScale->value() * 10.0));
	charstyle.setBaselineOffset(qRound(fontBase->value() * 10.0));
	charstyle.setTracking(qRound(fontKern->value() * 10.0));
	parstyle.charStyle().applyCharStyle(charstyle);
}


void EditStyle::updatePreview()
{
	if (!previewCaption->isChecked())
		return;
	qApp->setOverrideCursor(QCursor(waitCursor));

	ParagraphStyle tmpStyle;
	copyStyleSettings(tmpStyle);
	tmpStyle.setName(Name->text() + " (preview temporary)");
	sampleItem->setStyle(tmpStyle);
	QPixmap pm = sampleItem->getSample(previewText->width(), previewText->height());
	previewText->setPixmap(pm);
	qApp->restoreOverrideCursor();
}

void EditStyle::setPreviewBackground()
{
	QColor bg;
	bg = QColorDialog::getColor(previewText->paletteBackgroundColor(), this);
	if (bg.isValid())
	{
		previewText->setPaletteBackgroundColor(bg);
		sampleItem->setBgColor(bg);
		updatePreview();
	}
}
