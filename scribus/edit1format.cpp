#include "edit1format.h"
#include "edit1format.moc"
#include "tabmanager.h"
extern QPixmap loadIcon(QString nam);
extern double UmReFaktor;
#include <qmessagebox.h>

EditStyle::EditStyle( QWidget* parent, struct StVorL *vor, QValueList<StVorL> v, bool neu, preV *Prefs, double au, int dEin)
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
    GroupFontLayout->addMultiCellWidget( FontC, 0, 0, 1, 2 );
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

    DropCaps = new QCheckBox( GroupFont, "DropCaps" );
    DropCaps->setText( tr( "Drop Caps" ) );
		DropCaps->setChecked(vor->Drop);
    GroupFontLayout->addMultiCellWidget( DropCaps, 2, 2, 0, 2 );
    CapLabel = new QLabel( GroupFont, "CapLabel" );
		CapLabel->setText( tr("Lines:"));
    GroupFontLayout->addWidget( CapLabel, 3, 0 );
    DropLines = new QSpinBox( GroupFont, "DropLines" );
    DropLines->setMinValue( 2 );
    DropLines->setMaxValue( 20 );
    DropLines->setValue(vor->DropLin);
    GroupFontLayout->addWidget( DropLines, 3, 1 );
		if (vor->Drop)
			{
			DropLines->setEnabled(true);
			CapLabel->setEnabled(true);
			}
		else
			{
			DropLines->setEnabled(false);
			CapLabel->setEnabled(false);
			}

    EditStyleLayout->addWidget( GroupFont, 2, 0 );

    GroupBox10 = new QGroupBox( this, "GroupBox10" );
    GroupBox10->setTitle( tr( "Indentation" ) );
    GroupBox10->setColumnLayout(0, Qt::Vertical );
    GroupBox10->layout()->setSpacing( 0 );
    GroupBox10->layout()->setMargin( 0 );
    GroupBox10Layout = new QGridLayout( GroupBox10->layout() );
    GroupBox10Layout->setAlignment( Qt::AlignTop );
    GroupBox10Layout->setSpacing( 5 );
    GroupBox10Layout->setMargin( 10 );
    TextLabel1_2 = new QLabel( GroupBox10, "TextLabel1_2" );
    TextLabel1_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabel1_2->sizePolicy().hasHeightForWidth() ) );
    TextLabel1_2->setMinimumSize( QSize( 90, 22 ) );
    TextLabel1_2->setText( tr( "Left Indent:" ) );
    GroupBox10Layout->addWidget( TextLabel1_2, 1, 0 );
    TextLabel2 = new QLabel( GroupBox10, "TextLabel2" );
    TextLabel2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabel2->sizePolicy().hasHeightForWidth() ) );
    TextLabel2->setMinimumSize( QSize( 90, 22 ) );
    TextLabel2->setText( tr( "First Line:" ) );
    GroupBox10Layout->addWidget( TextLabel2, 0, 0 );

    LeftInd = new MSpinBox( GroupBox10, 1 );
    LeftInd->setMaxValue( 300 );
    LeftInd->setMinValue( -300 );
    GroupBox10Layout->addWidget( LeftInd, 1, 1 );

    FirstLin = new MSpinBox( GroupBox10, 1);
    FirstLin->setMaxValue( 300 );
    FirstLin->setMinValue( -300 );
    GroupBox10Layout->addWidget( FirstLin, 0, 1 );
		TabsButton = new QPushButton( GroupBox10, "Tabul" );
		TabsButton->setText( tr( "Tabulators..." ) );
		GroupBox10Layout->addMultiCellWidget( TabsButton, 2, 2, 0, 1 );
    EditStyleLayout->addWidget( GroupBox10, 3, 1 );

    ButtonGroup1 = new QButtonGroup( this, "ButtonGroup1" );
    ButtonGroup1->setTitle( tr( "Alignment" ) );
    ButtonGroup1->setColumnLayout(0, Qt::Vertical );
    ButtonGroup1->layout()->setSpacing( 0 );
    ButtonGroup1->layout()->setMargin( 0 );
    ButtonGroup1Layout = new QGridLayout( ButtonGroup1->layout() );
    ButtonGroup1Layout->setAlignment( Qt::AlignTop );
    ButtonGroup1Layout->setSpacing( 5 );
    ButtonGroup1Layout->setMargin( 10 );

    Left = new QRadioButton( ButtonGroup1, "Left" );
    Left->setText( tr( "Left" ) );
    Left->setChecked( true );
    ButtonGroup1Layout->addWidget( Left, 0, 0 );

    Right = new QRadioButton( ButtonGroup1, "Right" );
    Right->setText( tr( "Right" ) );
    ButtonGroup1Layout->addWidget( Right, 1, 0 );

    Center = new QRadioButton( ButtonGroup1, "Center" );
    Center->setText( tr( "Center" ) );
    ButtonGroup1Layout->addWidget( Center, 0, 1 );

    Full = new QRadioButton( ButtonGroup1, "Full" );
    Full->setText( tr( "Block" ) );
    ButtonGroup1Layout->addWidget( Full, 1, 1 );

    Forced = new QRadioButton( ButtonGroup1, "Full" );
    Forced->setText( tr( "Forced" ) );
    ButtonGroup1Layout->addWidget( Forced, 2, 1 );
		switch (vor->Ausri)
			{
			case 0 :
				Left->setChecked( true );
				break;
			case 1 :
				Center->setChecked( true );
				break;
			case 2 :
				Right->setChecked( true );
				break;
			case 3 :
				Full->setChecked( true );
				break;
			case 4 :
				Forced->setChecked( true );
				break;
			}
    EditStyleLayout->addWidget( ButtonGroup1, 3, 0 );

    AbstandV = new QGroupBox( this, "AbstandV" );
    AbstandV->setTitle( tr( "Vertical Spaces" ) );
    AbstandV->setColumnLayout(0, Qt::Vertical );
    AbstandV->layout()->setSpacing( 0 );
    AbstandV->layout()->setMargin( 0 );
    AbstandVLayout = new QGridLayout( AbstandV->layout() );
    AbstandVLayout->setAlignment( Qt::AlignTop );
    AbstandVLayout->setSpacing( 5 );
    AbstandVLayout->setMargin( 10 );

    AboveV = new MSpinBox( AbstandV, 1 );
    AboveV->setMinimumSize( QSize( 70, 22 ) );
    AboveV->setMaxValue( 300 );
    AboveV->setMinValue( 0 );
    AbstandVLayout->addWidget( AboveV, 0, 1 );

    BelowV = new MSpinBox( AbstandV, 1 );
    BelowV->setMinimumSize( QSize( 70, 22 ) );
    BelowV->setMaxValue( 300 );
    BelowV->setMinValue( 0 );
	  AbstandVLayout->addWidget( BelowV, 1, 1 );

    LineSpVal = new MSpinBox( AbstandV, 1 );
    LineSpVal->setMinimumSize( QSize( 70, 22 ) );
    LineSpVal->setSuffix( tr( " pt" ) );
    LineSpVal->setMaxValue( 300 );
    LineSpVal->setMinValue( 1 );
    LineSpVal->setValue(vor->LineSpa);
    AbstandVLayout->addWidget( LineSpVal, 2, 1 );

    TextLabel3 = new QLabel( AbstandV, "TextLabel3" );
    TextLabel3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabel3->sizePolicy().hasHeightForWidth() ) );
    TextLabel3->setMinimumSize( QSize( 90, 22 ) );
    TextLabel3->setText( tr( "Line Spacing" ) );
    AbstandVLayout->addWidget( TextLabel3, 2, 0 );
    TextLabel1_2_2 = new QLabel( AbstandV, "TextLabel1_2_2" );
    TextLabel1_2_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabel1_2_2->sizePolicy().hasHeightForWidth() ) );
    TextLabel1_2_2->setMinimumSize( QSize( 90, 22 ) );
    TextLabel1_2_2->setText( tr( "Above:" ) );
    AbstandVLayout->addWidget( TextLabel1_2_2, 0, 0 );
    TextLabel1_2_3 = new QLabel( AbstandV, "TextLabel1_2_3" );
    TextLabel1_2_3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, TextLabel1_2_3->sizePolicy().hasHeightForWidth() ) );
    TextLabel1_2_3->setMinimumSize( QSize( 90, 22 ) );
    TextLabel1_2_3->setText( tr( "Below:" ) );
    AbstandVLayout->addWidget( TextLabel1_2_3, 1, 0 );
    EditStyleLayout->addWidget( AbstandV, 2, 1 );

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
    // signals and slots connections
    connect( Cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( OkButton, SIGNAL( clicked() ), this, SLOT( Verlassen() ) );
    connect( TabsButton, SIGNAL( clicked() ), this, SLOT( ManageTabs() ) );
    connect( DropCaps, SIGNAL( clicked() ), this, SLOT( ManageDrops() ) );
    connect(SizeC, SIGNAL(valueChanged(int)), this, SLOT(FontChange()));
		LeftInd->setDecimals(10);
		FirstLin->setDecimals(10);
		AboveV->setDecimals(10);
		BelowV->setDecimals(10);
		QString ein;
		switch (dEin)
			{
			case 0:
				ein = " pt";
				break;
			case 1:
	    	ein = " mm";
				break;
			case 2:
	    	ein = " in";
				LeftInd->setDecimals(10000);
				FirstLin->setDecimals(10000);
	  		AboveV->setDecimals(10000);
	  		BelowV->setDecimals(10000);
				break;
			case 3:
	    	ein = " p";
				break;
			}
    LeftInd->setSuffix(ein);
    FirstLin->setSuffix(ein);
    AboveV->setSuffix(ein);
    BelowV->setSuffix(ein);
    BelowV->setValue(vor->Anach * UmReFaktor);
    AboveV->setValue(vor->Avor * UmReFaktor);
    FirstLin->setValue(vor->First * UmReFaktor);
    LeftInd->setValue(vor->Indent * UmReFaktor);
}

void EditStyle::ManageDrops()
{
	if (DropCaps->isChecked())
		{
		DropLines->setEnabled(true);
		CapLabel->setEnabled(true);
		}
	else
		{
		DropLines->setEnabled(false);
		CapLabel->setEnabled(false);
		}
}

void EditStyle::ManageTabs()
{
	TabManager *dia = new TabManager(this, DocsEin, werte->TabValues);
	if (dia->exec())
		werte->TabValues = dia->tmpTab;
	delete dia;
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
  			QMessageBox::information(this, tr("Warning"), tr("Name of the Style is not unique"), tr("OK"),0, 0, 0, 1);
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
  				QMessageBox::information(this, tr("Warning"), tr("Name of the Style is not unique"), tr("OK"),0, 0, 0, 1);
      		Name->selectAll();
					Name->setFocus();
					return;
					}
				}
			}
		}
	if (Left->isChecked())
		werte->Ausri = 0;
	if (Center->isChecked())
		werte->Ausri = 1;
	if (Right->isChecked())
		werte->Ausri = 2;
	if (Full->isChecked())
		werte->Ausri = 3;
	if (Forced->isChecked())
		werte->Ausri = 4;
	werte->LineSpa = LineSpVal->value();
	werte->Indent = LeftInd->value() / UmReFaktor;
	werte->First = FirstLin->value() / UmReFaktor;
	werte->Avor = AboveV->value() / UmReFaktor;
	werte->Anach = BelowV->value() / UmReFaktor;
	werte->Vname = Name->text();
	werte->Font = FontC->currentText();
	werte->FontSize = qRound(SizeC->value() * 10.0);
	werte->Drop = DropCaps->isChecked();
	werte->DropLin = DropLines->value();
	accept();
}
