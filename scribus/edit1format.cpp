#include "edit1format.h"
#include "edit1format.moc"
#include "tabruler.h"
extern QPixmap loadIcon(QString nam);
extern double UmReFaktor;
#include <qmessagebox.h>
#include <qtooltip.h>

EditStyle::EditStyle( QWidget* parent, struct StVorL *vor, QValueList<StVorL> v, bool neu, preV *Prefs, double au, int dEin, ScribusDoc *doc)
		: QDialog( parent, "EditST", true, 0)
{
	setCaption( tr( "Edit Style" ) );
	setIcon(loadIcon("AppIcon.png"));
	AutoVal = au;
	DocsEin = dEin;
	EditStyleLayout = new QGridLayout( this );
	EditStyleLayout->setSpacing( 6 );
	EditStyleLayout->setMargin( 10 );

	TextLabel1 = new QLabel( this, "TextLabel1" );
	TextLabel1->setText( tr( "Name:" ) );
	EditStyleLayout->addMultiCellWidget( TextLabel1, 0, 0, 0, 1 );

	Name = new QLineEdit( this, "Name" );
	Name->setMinimumSize( QSize( 200, 22 ) );
	Name->setText( vor->Vname );
	OldName = vor->Vname;
	IsNew = neu;
	EditStyleLayout->addMultiCellWidget( Name, 1, 1, 0, 1 );

	GroupFont = new QGroupBox( this, "GroupFont" );
	GroupFont->setTitle( tr( "Character" ) );
	GroupFont->setColumnLayout(0, Qt::Vertical );
	GroupFont->layout()->setSpacing( 5 );
	GroupFont->layout()->setMargin( 10 );
	GroupFontLayout = new QGridLayout( GroupFont->layout() );
	GroupFontLayout->setAlignment( Qt::AlignTop );
	TextF1 = new QLabel( GroupFont, "TextF1" );
	TextF1->setMinimumSize( QSize( 50, 22 ) );
	TextF1->setText( tr( "Face:" ) );
	GroupFontLayout->addWidget( TextF1, 0, 0 );
	FontC = new FontCombo(GroupFont, Prefs);
	for (int fc=0; fc<FontC->count(); ++fc)
	{
		if (FontC->text(fc) == vor->Font)
		{
			FontC->setCurrentItem(fc);
			break;
		}
	}
	GroupFontLayout->addMultiCellWidget( FontC, 0, 0, 1, 4 );
	TextF2 = new QLabel( GroupFont, "TextF2" );
	TextF2->setMinimumSize( QSize( 50, 22 ) );
	TextF2->setText( tr( "Size:" ) );
	GroupFontLayout->addWidget( TextF2, 1, 0 );
	SizeC = new MSpinBox( GroupFont, 1 );
	SizeC->setMinimumSize( QSize( 70, 22 ) );
	SizeC->setSuffix( tr( " pt" ) );
	SizeC->setMinValue(1);
	SizeC->setMaxValue(1024);
	SizeC->setValue(vor->FontSize / 10.0);
	GroupFontLayout->addWidget( SizeC, 1, 1 );
	EffeLabel = new QLabel( GroupFont, "EffeLabel" );
	EffeLabel->setText( tr("Effect:"));
	GroupFontLayout->addWidget( EffeLabel, 1, 2 );
	EffeS = new StyleSelect(GroupFont);
	EffeS->setStyle(vor->FontEffect);
	GroupFontLayout->addMultiCellWidget( EffeS, 1, 1, 3, 4, Qt::AlignLeft );
	AligLabel = new QLabel( GroupFont, "AligLabel" );
	AligLabel->setText( tr("Alignment:"));
	GroupFontLayout->addWidget( AligLabel, 2, 0 );
	AligS = new AlignSelect(GroupFont);
	AligS->setStyle(vor->Ausri);
	GroupFontLayout->addWidget( AligS, 2, 1, Qt::AlignLeft );

	DropCaps = new QCheckBox( GroupFont, "DropCaps" );
	DropCaps->setText( tr( "Drop Caps" ) );
	DropCaps->setChecked(vor->Drop);
	GroupFontLayout->addWidget( DropCaps, 2, 2 );
	CapLabel = new QLabel( GroupFont, "CapLabel" );
	CapLabel->setText( tr("Lines:"));
	GroupFontLayout->addWidget( CapLabel, 2, 3, Qt::AlignRight );
	DropLines = new QSpinBox( GroupFont, "DropLines" );
	DropLines->setMinValue( 2 );
	DropLines->setMaxValue( 20 );
	DropLines->setValue(vor->DropLin);
	GroupFontLayout->addWidget( DropLines, 2, 4 );
	bool enable = vor->Drop ? true : false;
	DropLines->setEnabled(enable);
	CapLabel->setEnabled(enable);

	FillIcon = new QLabel( GroupFont, "FillIcon" );
	FillIcon->setText( tr("Fill Color:"));
	GroupFontLayout->addWidget( FillIcon, 3, 0 );
	TxFill = new QComboBox( true, GroupFont, "TxFill" );
	TxFill->setEditable(false);
	GroupFontLayout->addWidget( TxFill, 3, 1 );
	PM2 = new ShadeButton(GroupFont);
	GroupFontLayout->addWidget( PM2, 3, 2, Qt::AlignLeft );

	StrokeIcon = new QLabel( GroupFont, "StrokeIcon" );
	StrokeIcon->setText( tr("Stroke Color:"));
	GroupFontLayout->addWidget( StrokeIcon, 4, 0 );
	TxStroke = new QComboBox( true, GroupFont, "TxStroke" );
	TxStroke->setEditable(false);
	GroupFontLayout->addWidget( TxStroke, 4, 1 );
	PM1 = new ShadeButton(GroupFont);
	GroupFontLayout->addWidget( PM1, 4, 2, Qt::AlignLeft );

	TxFill->clear();
	TxStroke->clear();
	CListe::Iterator it;
	QPixmap pm = QPixmap(15, 15);
	TxFill->insertItem( tr("None"));
	TxStroke->insertItem( tr("None"));
	for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
	{
		pm.fill(doc->PageColors[it.key()].getRGBColor());
		TxFill->insertItem(pm, it.key());
		TxStroke->insertItem(pm, it.key());
	}
	StrokeIcon->setEnabled(false);
	TxStroke->setEnabled(false);
	TxFill->setCurrentText(vor->FColor);
	TxStroke->setCurrentText(vor->SColor);
	PM2->setValue(vor->FShade);
	PM1->setValue(vor->SShade);

	EditStyleLayout->addWidget( GroupFont, 2, 0 );
	
	AbstandV = new QGroupBox( this, "AbstandV" );
	AbstandV->setTitle( tr( "Vertical Spaces" ) );
	AbstandV->setColumnLayout(0, Qt::Vertical );
	AbstandV->layout()->setSpacing( 0 );
	AbstandV->layout()->setMargin( 0 );
	AbstandVLayout = new QGridLayout( AbstandV->layout() );
	AbstandVLayout->setAlignment( Qt::AlignTop );
	AbstandVLayout->setSpacing( 5 );
	AbstandVLayout->setMargin( 10 );

	BaseGrid = new QCheckBox( AbstandV, "BaseGrid" );
	BaseGrid->setText( tr( "Adjust to Baseline Grid" ) );
	BaseGrid->setChecked(vor->BaseAdj);
	AbstandVLayout->addMultiCellWidget( BaseGrid, 0, 0, 0, 1 );
	AboveV = new MSpinBox( AbstandV, 1 );
	AboveV->setMinimumSize( QSize( 70, 22 ) );
	AboveV->setMaxValue( 300 );
	AboveV->setMinValue( 0 );
	AbstandVLayout->addWidget( AboveV, 1, 1 );

	BelowV = new MSpinBox( AbstandV, 1 );
	BelowV->setMinimumSize( QSize( 70, 22 ) );
	BelowV->setMaxValue( 300 );
	BelowV->setMinValue( 0 );
	AbstandVLayout->addWidget( BelowV, 2, 1 );

	LineSpVal = new MSpinBox( AbstandV, 1 );
	LineSpVal->setMinimumSize( QSize( 70, 22 ) );
	LineSpVal->setSuffix( tr( " pt" ) );
	LineSpVal->setMaxValue( 300 );
	LineSpVal->setMinValue( 1 );
	LineSpVal->setValue(vor->LineSpa);
	AbstandVLayout->addWidget( LineSpVal, 3, 1 );

	TextLabel3 = new QLabel( AbstandV, "TextLabel3" );
	TextLabel3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                                        TextLabel3->sizePolicy().hasHeightForWidth() ) );
	TextLabel3->setMinimumSize( QSize( 90, 22 ) );
	TextLabel3->setText( tr( "Line Spacing:" ) );
	AbstandVLayout->addWidget( TextLabel3, 3, 0 );
	TextLabel1_2_2 = new QLabel( AbstandV, "TextLabel1_2_2" );
	TextLabel1_2_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                               TextLabel1_2_2->sizePolicy().hasHeightForWidth() ) );
	TextLabel1_2_2->setMinimumSize( QSize( 90, 22 ) );
	TextLabel1_2_2->setText( tr( "Above:" ) );
	AbstandVLayout->addWidget( TextLabel1_2_2, 1, 0 );
	TextLabel1_2_3 = new QLabel( AbstandV, "TextLabel1_2_3" );
	TextLabel1_2_3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                               TextLabel1_2_3->sizePolicy().hasHeightForWidth() ) );
	TextLabel1_2_3->setMinimumSize( QSize( 90, 22 ) );
	TextLabel1_2_3->setText( tr( "Below:" ) );
	AbstandVLayout->addWidget( TextLabel1_2_3, 2, 0 );
	EditStyleLayout->addWidget( AbstandV, 2, 1 );

	GroupBox10 = new QGroupBox( this, "GroupBox10" );
	GroupBox10->setTitle( tr( "Tabulators and Indentation" ) );
	GroupBox10->setColumnLayout(0, Qt::Vertical );
	GroupBox10->layout()->setSpacing( 0 );
	GroupBox10->layout()->setMargin( 0 );
	GroupBox10Layout = new QVBoxLayout(GroupBox10->layout());
	GroupBox10Layout->setAlignment( Qt::AlignTop );
	GroupBox10Layout->setSpacing( 5 );
	GroupBox10Layout->setMargin( 10 );
	TabList = new Tabruler(GroupBox10, true, DocsEin, vor->TabValues, -1);
	TabList->setIndentSpin(vor->Indent);
	TabList->setFirstSpin(vor->First);
	TabList->setIndent();
	TabList->setFirst();
	GroupBox10Layout->addWidget( TabList );

	EditStyleLayout->addMultiCellWidget( GroupBox10, 3, 3, 0, 1 );

	Layout17 = new QHBoxLayout;
	Layout17->setSpacing( 6 );
	Layout17->setMargin( 0 );
	QSpacerItem* spacer2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout17->addItem( spacer2 );
	OkButton = new QPushButton( this, "OkButton" );
	OkButton->setText( tr( "OK" ) );
	Layout17->addWidget( OkButton );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout17->addItem( spacer );
	Cancel = new QPushButton( this, "Cancel" );
	Cancel->setText( tr( "Cancel" ) );
	OkButton->setDefault( true );
	Layout17->addWidget( Cancel );
	QSpacerItem* spacer_3 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout17->addItem( spacer_3 );
	EditStyleLayout->addMultiCellLayout( Layout17, 4, 4, 0, 1 );
	werte = vor;
	allV = v;
	// tooltips
	QToolTip::add( Name, tr( "Name of your paragraph style" ) );
	QToolTip::add( FontC, tr( "Font of selected text or object" ) );
	QToolTip::add( SizeC, tr( "Font Size" ) );
	QToolTip::add( TxFill, tr( "Color of text fill" ) );
	QToolTip::add( TxStroke, tr( "Color of text stroke" ) );
	QToolTip::add( DropCaps, tr( "Provides an oversized first letter for a paragraph. Used for stylistic effect" ) );
	QToolTip::add( DropLines, tr( "Determines the overall height, in line numbers, of the Drop Caps" ) );
	QToolTip::add( BaseGrid, tr( "Align text to baseline grid" ) );
	QToolTip::add( AboveV, tr( "Spacing above the paragraph" ) );
	QToolTip::add( BelowV, tr( "Spacing below the paragraph" ) );
	QToolTip::add( LineSpVal, tr( "Line Spacing" ) );

	// signals and slots connections
	connect( Cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( OkButton, SIGNAL( clicked() ), this, SLOT( Verlassen() ) );
	connect( DropCaps, SIGNAL( clicked() ), this, SLOT( ManageDrops() ) );
	connect(SizeC, SIGNAL(valueChanged(int)), this, SLOT(FontChange()));
	connect(EffeS, SIGNAL(State(int)), this, SLOT(ColorChange()));
	AboveV->setDecimals(10);
	BelowV->setDecimals(10);
	/* PFJ - 29.02.04 - Altered switch so only case 2 is tested */
	QString ein, measure[] = { tr(" pt"), tr(" mm"), tr(" in"), tr(" p")};
	ein = measure[dEin];
	if (dEin == 2)
	{
		AboveV->setDecimals(10000);
		BelowV->setDecimals(10000);
	}
	AboveV->setSuffix(ein);
	BelowV->setSuffix(ein);
	BelowV->setValue(vor->Anach * UmReFaktor);
	AboveV->setValue(vor->Avor * UmReFaktor);
	ColorChange();
}

void EditStyle::ColorChange()
{
	int s = EffeS->getStyle();
	bool enabled = (s & 4) ? true : false;
	StrokeIcon->setEnabled(enabled);
	TxStroke->setEnabled(enabled);
}

void EditStyle::ManageDrops()
{
	bool enabled = DropCaps->isChecked() ? true : false;
	DropLines->setEnabled(enabled);
	CapLabel->setEnabled(enabled);
}

void EditStyle::FontChange()
{
	double val = SizeC->value();
	LineSpVal->setValue((val  * AutoVal / 100) + val);
}

void EditStyle::Verlassen()
{
	if (IsNew)
	{
		for (uint x=0; x<allV.count()-1; ++x)
		{
			if (Name->text() == allV[x].Vname)
			{
				QMessageBox::information(this, tr("Warning"), tr("Name of the Style is not unique"), tr("OK"),
				                         0, 0, 0, QMessageBox::Ok);
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
					QMessageBox::information(this, tr("Warning"), tr("Name of the Style is not unique"),
					                         tr("OK"),0, 0, 0, QMessageBox::Ok);
					Name->selectAll();
					Name->setFocus();
					return;
				}
			}
		}
	}
	werte->FontEffect = EffeS->getStyle();
	werte->Ausri = AligS->getStyle();
	werte->LineSpa = LineSpVal->value();
	werte->Indent = TabList->getIndent();
	werte->First = TabList->getFirst();
	werte->Avor = AboveV->value() / UmReFaktor;
	werte->Anach = BelowV->value() / UmReFaktor;
	werte->Vname = Name->text();
	werte->Font = FontC->currentText();
	werte->FontSize = qRound(SizeC->value() * 10.0);
	werte->Drop = DropCaps->isChecked();
	werte->DropLin = DropLines->value();
	werte->FColor = TxFill->currentText();
	werte->FShade = PM2->getValue();
	werte->SColor = TxStroke->currentText();
	werte->SShade = PM1->getValue();
	werte->BaseAdj = BaseGrid->isChecked();
	werte->TabValues = TabList->getTabVals();
	accept();
}
