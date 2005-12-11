#include "edit1format.h"
#include "edit1format.moc"
#include "tabruler.h"
#include "units.h"

#include <qmessagebox.h>
#include <qtooltip.h>
#include <qcolordialog.h>
#include <qcursor.h>

#include "sccombobox.h"
#include "commonstrings.h"
#include "scribusdoc.h"
#include "styleselect.h"
#include "scribusdoc.h"
#include "scribusstructs.h"
#include "scpaths.h"
#include "scribus.h"
#include "util.h"
#include "loremipsum.h"
#include "prefsmanager.h"
#include "sampleitem.h"


EditStyle::EditStyle( QWidget* parent, struct ParagraphStyle *vor, QValueList<ParagraphStyle> v, bool neu, double au, int dEin, ScribusDoc *doc)
		: QDialog( parent, "EditST", true, 0)
{
	qApp->setOverrideCursor(QCursor(waitCursor));
	parentDoc = doc;
	setCaption( tr( "Edit Style" ) );
	setIcon(loadIcon("AppIcon.png"));
	AutoVal = au;
	DocsEin = dEin;

	sampleItem = new SampleItem(parent);
	sampleItem->setLoremIpsum(2);

	EditStyleLayout = new QVBoxLayout( this, 10, 5, "EditStyleLayout");

	TextLabel1 = new QLabel( tr( "&Name:" ), this, "TextLabel1" );
	EditStyleLayout->addWidget( TextLabel1 );

	Name = new QLineEdit( this, "Name" );
	Name->setMinimumSize( QSize( 200, 22 ) );
	Name->setText( vor->Vname );
	OldName = vor->Vname;
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
	FontC->setCurrentFont(vor->Font);
	GroupFontLayout->addWidget( FontC );
	layout7 = new QHBoxLayout( 0, 0, 5, "layout7");
	SizeC = new MSpinBox( 1, 2048, GroupFont, 1 );
	SizeC->setMinimumSize( QSize( 70, 22 ) );
	SizeC->setSuffix( tr( " pt" ) );
	SizeC->setValue(vor->FontSize / 10.0);
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
	fontHScale->setValue( vor->scaleH / 10.0 );
	fontHScale->setSuffix( tr( " %" ) );
	layout7->addWidget( fontHScale );
	pixmapLabel3_2 = new QLabel( "", GroupFont, "pixmapLabel3_2" );
	pixmapLabel3_2->setMinimumSize( QSize( 22, 22 ) );
	pixmapLabel3_2->setMaximumSize( QSize( 22, 22 ) );
	pixmapLabel3_2->setPixmap( loadIcon("textscalev.png") );
	layout7->addWidget( pixmapLabel3_2 );

	fontVScale = new MSpinBox( 10, 400, GroupFont, 1 );
	fontVScale->setValue( vor->scaleV / 10.0 );
	fontVScale->setSuffix( tr( " %" ) );
	layout7->addWidget( fontVScale );
	GroupFontLayout->addLayout( layout7 );

	layout9a = new QHBoxLayout( 0, 0, 0, "layout9");
	EffeS = new StyleSelect(GroupFont);
	EffeS->setStyle(vor->FontEffect);
	EffeS->ShadowVal->Xoffset->setValue(vor->txtShadowX / 10.0);
	EffeS->ShadowVal->Yoffset->setValue(vor->txtShadowY / 10.0);
	EffeS->OutlineVal->LWidth->setValue(vor->txtOutline / 10.0);
	EffeS->UnderlineVal->LPos->setValue(vor->txtUnderPos / 10.0);
	EffeS->UnderlineVal->LWidth->setValue(vor->txtUnderWidth / 10.0);
	EffeS->StrikeVal->LPos->setValue(vor->txtStrikePos / 10.0);
	EffeS->StrikeVal->LWidth->setValue(vor->txtStrikeWidth / 10.0);
	layout9a->addWidget( EffeS );
	QSpacerItem* spacer1 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout9a->addItem( spacer1 );
	GroupFontLayout->addLayout( layout9a, Qt::AlignLeft );

	layout9b = new QHBoxLayout( 0, 0, 0, "layout9");
	AligS = new AlignSelect(GroupFont);
	AligS->setStyle(vor->textAlignment);
	layout9b->addWidget( AligS );
	QSpacerItem* spacer2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout9a->addItem( spacer2 );
	GroupFontLayout->addLayout( layout9b, Qt::AlignLeft );

	layout5 = new QHBoxLayout( 0, 0, 5, "layout5");
	FillIcon = new QLabel( "", GroupFont, "FillIcon" );
	FillIcon->setPixmap(loadIcon("fill.png"));
	layout5->addWidget( FillIcon );
	TxFill = new ScComboBox( false, GroupFont, "TxFill" );
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
	TxStroke = new ScComboBox( false, GroupFont, "TxStroke" );
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
	TxFill->insertItem( tr("None"));
	TxStroke->insertItem( tr("None"));
	for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
	{
		pm.fill(doc->PageColors[it.key()].getRawRGBColor());
		TxFill->insertItem(pm, it.key());
		TxStroke->insertItem(pm, it.key());
	}
	StrokeIcon->setEnabled(false);
	PM1->setEnabled(false);
	TxStroke->setEnabled(false);
	TxFill->setCurrentText(vor->FColor);
	TxStroke->setCurrentText(vor->SColor);
	PM2->setValue(vor->FShade);
	PM1->setValue(vor->SShade);

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
	linespacingLabel = new QToolButton(AbstandV, "linespacingLabel" );
	linespacingLabel->setText("");
	linespacingLabel->setPixmap( loadIcon("linespacing.png") );
	linespacingLabel->setPopup(lineSpacingPop);
	linespacingLabel->setPopupDelay(400);
	linespacingLabel->setAutoRaise(true);
	AbstandVLayout->addWidget( linespacingLabel, 0, 0 );
	LineSpVal = new MSpinBox( 1, 300, AbstandV, 1 );
	LineSpVal->setSuffix( tr( " pt" ) );
	LineSpVal->setValue(vor->LineSpa);
	AbstandVLayout->addWidget( LineSpVal, 0, 1 );
	pixmapLabel2 = new QLabel( AbstandV, "pixmapLabel2" );
	pixmapLabel2->setMinimumSize( QSize( 22, 22 ) );
	pixmapLabel2->setMaximumSize( QSize( 22, 22 ) );
	pixmapLabel2->setPixmap( loadIcon("textbase.png") );
	AbstandVLayout->addWidget( pixmapLabel2, 0, 2 );
	fontBase = new MSpinBox( -100, 100, AbstandV, 1 );
	fontBase->setValue( vor->baseOff / 10.0 );
	fontBase->setSuffix( tr( " %" ) );
	AbstandVLayout->addWidget( fontBase, 0, 3 );
	pixmapLabel3_3 = new QLabel( AbstandV, "pixmapLabel3_3" );
	pixmapLabel3_3->setMinimumSize( QSize( 22, 22 ) );
	pixmapLabel3_3->setMaximumSize( QSize( 22, 22 ) );
	pixmapLabel3_3->setPixmap( loadIcon("textkern.png") );
	AbstandVLayout->addWidget( pixmapLabel3_3, 0, 4 );
	fontKern = new MSpinBox( -300, 300, AbstandV, 1 );
	fontKern->setValue( vor->kernVal / 10.0 );
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
	DropLines->setValue(vor->DropLin);
	CapLabel = new QLabel( DropLines, tr("&Lines:"), DropCaps, "CapLabel" );
	DropCapsLayout->addWidget( CapLabel, 0, 0 );
	DropCapsLayout->addWidget( DropLines, 0, 1 );
	DropDist = new MSpinBox( -3000, 3000, DropCaps, 1);
	DropDist->setSuffix( tr( " pt" ) );
	DropDist->setValue(vor->DropDist);
	CapLabel2 = new QLabel( DropLines, tr("Distance from Text:"), DropCaps, "CapLabel2" );
	DropCapsLayout->addWidget( CapLabel2, 1, 0 );
	DropCapsLayout->addWidget( DropDist, 1, 1 );
	DropCaps->setChecked(vor->Drop);
	layout8->addWidget( DropCaps );
	layout9->addLayout( layout8 );
	EditStyleLayout->addLayout( layout9 );

	for (uint al = 0; al < lineSpacingPop->count(); ++al)
	{
		lineSpacingPop->setItemChecked(lineSpacingPop->idAt(al), false);
	}
	if (vor->BaseAdj)
	{
		LineSpVal->setEnabled(false);
		lineSpacingPop->setItemChecked(lineSpacingPop->idAt(2), true);
	}
	else
	{
		lineSpacingPop->setItemChecked(lineSpacingPop->idAt(vor->LineSpaMode), true);
		if (vor->LineSpaMode > 0)
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
	TabList = new Tabruler(GroupBox10, true, DocsEin, vor->TabValues, -1);
	TabList->setLeftIndentData(vor->Indent);
	TabList->setLeftIndent();
	TabList->setFirstLineData(vor->First);
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
	previewBgColor->setText(tr("Background"));
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
	QToolTip::add( LineSpVal, "<qt>" + tr( "Line Spacing" ) + "</qt>" );
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
	BelowV->setValue(vor->gapAfter * parentDoc->unitRatio());
	AboveV->setValue(vor->gapBefore * parentDoc->unitRatio());
	DropDist->setValue(vor->DropDist * parentDoc->unitRatio());
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
			if (Name->text() == allV[x].Vname)
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
				if (Name->text() == allV[x].Vname)
				{
					QMessageBox::information(this, CommonStrings::trWarning, mess, CommonStrings::tr_OK,0, 0, 0, QMessageBox::Ok);
					Name->selectAll();
					Name->setFocus();
					return;
				}
			}
		}
	}
	werte->FontEffect = EffeS->getStyle();
	werte->textAlignment = AligS->getStyle();
	for (uint al = 0; al < lineSpacingPop->count(); ++al)
	{
		if (lineSpacingPop->isItemChecked(lineSpacingPop->idAt(al)))
		{
			werte->LineSpaMode = al;
			if (al == 2)
				werte->BaseAdj = true;
			else
				werte->BaseAdj = false;
			break;
		}
	}
	werte->LineSpa = LineSpVal->value();
	werte->Indent = QMAX(TabList->getLeftIndent(), 0.0);
	werte->First = TabList->getFirstLine();
	werte->gapBefore = AboveV->value() / parentDoc->unitRatio();
	werte->gapAfter = BelowV->value() / parentDoc->unitRatio();
	werte->Vname = Name->text();
	werte->Font = FontC->currentFont();
	werte->FontSize = qRound(SizeC->value() * 10.0);
	werte->Drop = DropCaps->isChecked();
	werte->DropLin = DropLines->value();
	werte->DropDist = DropDist->value() / parentDoc->unitRatio();
	werte->FColor = TxFill->currentText();
	werte->FShade = PM2->getValue();
	werte->SColor = TxStroke->currentText();
	werte->SShade = PM1->getValue();
	werte->TabValues = TabList->getTabVals();
	werte->txtShadowX = qRound(EffeS->ShadowVal->Xoffset->value() * 10.0);
	werte->txtShadowY = qRound(EffeS->ShadowVal->Yoffset->value() * 10.0);
	werte->txtOutline = qRound(EffeS->OutlineVal->LWidth->value() * 10.0);
	werte->txtStrikePos = qRound(EffeS->StrikeVal->LPos->value() * 10.0);
	werte->txtStrikeWidth = qRound(EffeS->StrikeVal->LWidth->value() * 10.0);
	werte->txtUnderPos = qRound(EffeS->UnderlineVal->LPos->value() * 10.0);
	werte->txtUnderWidth = qRound(EffeS->UnderlineVal->LWidth->value() * 10.0);
	werte->scaleH = qRound(fontHScale->value() * 10.0);
	werte->scaleV = qRound(fontVScale->value() * 10.0);
	werte->baseOff = qRound(fontBase->value() * 10.0);
	werte->kernVal = qRound(fontKern->value() * 10.0);
	accept();
}

void EditStyle::updatePreview()
{
	if (!previewCaption->isChecked())
		return;
	qApp->setOverrideCursor(QCursor(waitCursor));

	ParagraphStyle tmpStyle;
	tmpStyle.Vname = Name->text() + " (preview temporary)";
	tmpStyle.FontEffect = EffeS->getStyle();
	tmpStyle.textAlignment = AligS->getStyle();
	for (uint al = 0; al < lineSpacingPop->count(); ++al)
	{
		if (lineSpacingPop->isItemChecked(lineSpacingPop->idAt(al)))
		{
			tmpStyle.LineSpaMode = al;
			if (al == 2)
				tmpStyle.BaseAdj = true;
			else
				tmpStyle.BaseAdj = false;
			break;
		}
	}
	tmpStyle.LineSpa = LineSpVal->value();
	tmpStyle.Indent = TabList->getLeftIndent();
	tmpStyle.First = TabList->getFirstLine();
	tmpStyle.gapBefore = AboveV->value() / parentDoc->unitRatio();
	tmpStyle.gapAfter = BelowV->value() / parentDoc->unitRatio();
	tmpStyle.Font = FontC->currentFont();
	tmpStyle.FontSize = qRound(SizeC->value() * 10.0);
	tmpStyle.Drop = DropCaps->isChecked();
	tmpStyle.DropLin = DropLines->value();
	tmpStyle.DropDist = DropDist->value() / parentDoc->unitRatio();
	tmpStyle.FColor = TxFill->currentText();
	tmpStyle.FShade = PM2->getValue();
	tmpStyle.SColor = TxStroke->currentText();
	tmpStyle.SShade = PM1->getValue();
	tmpStyle.TabValues = TabList->getTabVals();
	tmpStyle.txtShadowX = qRound(EffeS->ShadowVal->Xoffset->value() * 10.0);
	tmpStyle.txtShadowY = qRound(EffeS->ShadowVal->Yoffset->value() * 10.0);
	tmpStyle.txtOutline = qRound(EffeS->OutlineVal->LWidth->value() * 10.0);
	tmpStyle.txtStrikePos = qRound(EffeS->StrikeVal->LPos->value() * 10.0);
	tmpStyle.txtStrikeWidth = qRound(EffeS->StrikeVal->LWidth->value() * 10.0);
	tmpStyle.txtUnderPos = qRound(EffeS->UnderlineVal->LPos->value() * 10.0);
	tmpStyle.txtUnderWidth = qRound(EffeS->UnderlineVal->LWidth->value() * 10.0);
	tmpStyle.scaleH = qRound(fontHScale->value() * 10.0);
	tmpStyle.scaleV = qRound(fontVScale->value() * 10.0);
	tmpStyle.baseOff = qRound(fontBase->value() * 10.0);
	tmpStyle.kernVal = qRound(fontKern->value() * 10.0);
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
