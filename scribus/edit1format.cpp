#include "edit1format.h"
#include "edit1format.moc"
extern QPixmap loadIcon(QString nam);
#include <qmessagebox.h>

EditStyle::EditStyle( QWidget* parent, struct StVorL *vor, QValueList<StVorL> v, bool neu, preV *Prefs, double au)
    : QDialog( parent, "EditST", true, 0)
{
    setCaption( tr( "Edit Style" ) );
 		setIcon(loadIcon("AppIcon.xpm"));
		AutoVal = au;
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

    GroupFontLayout->addWidget( FontC, 0, 1 );
    SizeC = new QSpinBox( GroupFont, "SizeC" );
    SizeC->setMinimumSize( QSize( 70, 22 ) );
    SizeC->setSuffix( tr( " pt" ) );
		SizeC->setMinValue(1);
		SizeC->setMaxValue(512);
		SizeC->setValue(vor->FontSize);

    GroupFontLayout->addWidget( SizeC, 1, 1 );
    TextF2 = new QLabel( GroupFont, "TextF2" );
    TextF2->setMinimumSize( QSize( 50, 22 ) );
    TextF2->setText( tr( "Size:" ) );
    GroupFontLayout->addWidget( TextF2, 1, 0 );
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
    LeftInd->setSuffix( tr( " pt" ) );
    LeftInd->setMaxValue( 3000 );
    LeftInd->setMinValue( -3000 );
    LeftInd->setLineStep(10);
    LeftInd->setValue(qRound(vor->Indent * 10));
    GroupBox10Layout->addWidget( LeftInd, 1, 1 );

    FirstLin = new MSpinBox( GroupBox10, 1);
    FirstLin->setSuffix( tr( " pt" ) );
    FirstLin->setMaxValue( 3000 );
    FirstLin->setMinValue( -3000 );
    FirstLin->setLineStep(10);
    FirstLin->setValue(qRound(vor->First * 10));
    GroupBox10Layout->addWidget( FirstLin, 0, 1 );
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
    AboveV->setSuffix( tr( " pt" ) );
    AboveV->setMaxValue( 3000 );
    AboveV->setMinValue( 0 );
    AboveV->setValue(qRound(vor->Avor * 10));
    AboveV->setLineStep(10);
    AbstandVLayout->addWidget( AboveV, 0, 1 );

    BelowV = new MSpinBox( AbstandV, 1 );
    BelowV->setMinimumSize( QSize( 70, 22 ) );
    BelowV->setSuffix( tr( " pt" ) );
    BelowV->setMaxValue( 3000 );
    BelowV->setMinValue( 0 );
    BelowV->setValue(qRound(vor->Anach * 10));
    BelowV->setLineStep(10);
	  AbstandVLayout->addWidget( BelowV, 1, 1 );

    LineSpVal = new MSpinBox( AbstandV, 1 );
    LineSpVal->setMinimumSize( QSize( 70, 22 ) );
    LineSpVal->setSuffix( tr( " pt" ) );
    LineSpVal->setMaxValue( 3000 );
    LineSpVal->setMinValue( 1 );
    LineSpVal->setLineStep(10);
    LineSpVal->setValue(qRound(vor->LineSpa * 10));
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
//    EditStyleLayout->addWidget( AbstandV, 3, 1 );
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
    connect(SizeC, SIGNAL(valueChanged(int)), this, SLOT(FontChange(int)));
}

void EditStyle::FontChange(int val)
{
	LineSpVal->setValue(static_cast<int>(((val * AutoVal / 100) + val) * 10));
}

void EditStyle::Verlassen()
{
	if (IsNew)
		{
		for (uint x=0; x<allV.count()-1; ++x)
			{
			if (Name->text() == allV[x].Vname)
				{
  			QMessageBox::information(this,tr("Warning"),tr("Name of the Style is not unique"),tr("OK"),0, 0, 0, 1);
      	Name->selectAll();
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
  				QMessageBox::information(this,tr("Warning"),tr("Name of the Style is not unique"),tr("OK"),0, 0, 0, 1);
      		Name->selectAll();
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
	werte->LineSpa = static_cast<double>(LineSpVal->value()) / 10;
	werte->Indent = static_cast<double>(LeftInd->value() / 10);
	werte->First = static_cast<double>(FirstLin->value() / 10);
	werte->Avor = static_cast<double>(AboveV->value() / 10);
	werte->Anach = static_cast<double>(BelowV->value() / 10);
	werte->Vname = Name->text();
	werte->Font = FontC->currentText();
	werte->FontSize = SizeC->value();
	accept();
}
