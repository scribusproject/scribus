/****************************************************************************
** Form implementation generated from reading ui file 'Druck.ui'
**
** Created: Mon Sep 17 21:42:19 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "druck.h"
#include "druck.moc"
#include "config.h"
#include "customfdialog.h"
#include "cupsoptions.h"
#include <qtextstream.h>
#include <qstringlist.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <qmap.h>
#ifdef HAVE_CUPS
#include <cups/cups.h>
#endif
#ifdef HAVE_CMS
extern bool CMSuse;
#endif

extern bool loadText(QString nam, QString *Buffer);
extern QPixmap loadIcon(QString nam);

AdvOptions::AdvOptions(QWidget* parent, bool Hm, bool Vm, bool Ic) : QDialog( parent, "prin", true, 0 )
{
	setCaption( tr( "Advanced Options" ) );
 	setIcon(loadIcon("AppIcon.png"));
	AdvOptionsLayout = new QVBoxLayout( this );
	AdvOptionsLayout->setSpacing( 5 );
	AdvOptionsLayout->setMargin( 10 );
	MirrorH = new QCheckBox(this, "MirrorH");
	MirrorH->setText( tr("Mirror Page(s) horizontal"));
	MirrorH->setChecked(Hm);
	AdvOptionsLayout->addWidget( MirrorH );
	MirrorV = new QCheckBox(this, "MirrorV");
	MirrorV->setText( tr("Mirror Page(s) vertical"));
	MirrorV->setChecked(Vm);
	AdvOptionsLayout->addWidget( MirrorV );
#ifdef HAVE_CMS
	if (CMSuse)
		{
		UseICC = new QCheckBox(this, "ICC");
		UseICC->setText( tr("Apply ICC-Profiles"));
		UseICC->setChecked(Ic);
		AdvOptionsLayout->addWidget( UseICC );
		}
#endif

	Layout2 = new QHBoxLayout;
	Layout2->setSpacing( 30 );
	Layout2->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout2->addItem( spacer );
	PushButton1 = new QPushButton( this, "PushButton1" );
	PushButton1->setText( tr( "OK" ) );
	Layout2->addWidget( PushButton1 );
	QSpacerItem* spacer2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout2->addItem( spacer2 );
	PushButton2 = new QPushButton( this, "PushButton1_2" );
	PushButton2->setText( tr( "Cancel" ) );
	PushButton2->setDefault( TRUE );
	PushButton2->setFocus();
	Layout2->addWidget( PushButton2 );
	QSpacerItem* spacer3 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout2->addItem( spacer3 );
	AdvOptionsLayout->addLayout( Layout2 );
	setMinimumSize( sizeHint() );
	connect( PushButton2, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( PushButton1, SIGNAL( clicked() ), this, SLOT( accept() ) );
}

Druck::Druck( QWidget* parent, QString PDatei, QString PDev, QString PCom)
    : QDialog( parent, "Dr", true, 0)
{
		PrinterOpts = "";
    setCaption( tr( "Setup Printer" ) );
 		setIcon(loadIcon("AppIcon.png"));
    DruckLayout = new QVBoxLayout( this ); 
    DruckLayout->setSpacing( 6 );
    DruckLayout->setMargin( 10 );

    Drucker = new QGroupBox( this, "Drucker" );
    Drucker->setTitle( tr( "Print destination" ) );
    Drucker->setColumnLayout(0, Qt::Vertical );
    Drucker->layout()->setSpacing( 0 );
    Drucker->layout()->setMargin( 0 );
    DruckerLayout = new QGridLayout( Drucker->layout() );
    DruckerLayout->setAlignment( Qt::AlignTop );
    DruckerLayout->setSpacing( 6 );
    DruckerLayout->setMargin( 10 );

    Layout1x = new QHBoxLayout;
    Layout1x->setSpacing( 6 );
    Layout1x->setMargin( 0 );
    PrintDest = new QComboBox( true, Drucker, "PrintDest" );
    PrintDest->setMinimumSize( QSize( 250, 22 ) );
    PrintDest->setMaximumSize( QSize( 260, 22 ) );
    PrintDest->setEditable(false);
    QString Pcap;
    QString tmp;
    QStringList wt;
#ifdef HAVE_CUPS
		cups_dest_t *dests;
		int num_dests;
		num_dests = cupsGetDests(&dests);
		for (int pr = 0; pr < num_dests; pr++)
			{
			tmp = QString(dests[pr].name);
    	PrintDest->insertItem(tmp);
    	if (tmp == PDev)
    		{
    		PrintDest->setCurrentItem(PrintDest->count()-1);
    		ToFile = false;
    		}
			}
		cupsFreeDests(num_dests, dests);
#else
    if (loadText("/etc/printcap", &Pcap))
    	{
    	QTextStream ts(&Pcap, IO_ReadOnly);
    	while(!ts.atEnd())
    		{
    		tmp = ts.readLine();
    		if (tmp.isEmpty())
    			continue;
        if ((tmp[0] != '#') && (tmp[0] != ' ') && (tmp[0] != '\n') && (tmp[0] != '\t'))
    			{
    			tmp = tmp.stripWhiteSpace();
					if (tmp.right(2) == ":\\")
    				tmp = tmp.left(tmp.length()-2);
					else
    				tmp = tmp.left(tmp.length()-1);
					wt = QStringList::split("|", tmp);
    			PrintDest->insertItem(wt[0]);
    			if (wt[0] == PDev)
    				{
    				PrintDest->setCurrentItem(PrintDest->count()-1);
    				ToFile = false;
    				}
    			}
    		}
    	}
#endif
    PrintDest->insertItem( tr("File"));
    if (PDev == "")
    	{
    	Geraet = PrintDest->text(0);
    	ToFile = false;
    	}
    else
    	Geraet = PDev;
    Layout1x->addWidget( PrintDest );

#ifdef HAVE_CUPS
    OptButton = new QPushButton(Drucker, "Optionen" );
    OptButton->setText( tr( "Options..." ) );
    Layout1x->addWidget( OptButton );
#endif
    QSpacerItem* spacerDR = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1x->addItem( spacerDR );
    DruckerLayout->addLayout( Layout1x, 0, 0);
    Layout1 = new QHBoxLayout; 
    Layout1->setSpacing( 6 );
    Layout1->setMargin( 0 );
    DateiT = new QLabel( Drucker, "DateiT" );
    DateiT->setEnabled( false );
    DateiT->setText( tr( "File:" ) );
    Layout1->addWidget( DateiT );
    LineEdit1 = new QLineEdit( Drucker, "LineEdit1" );
    LineEdit1->setMinimumSize( QSize( 240, 22 ) );
    LineEdit1->setEnabled(false);
    LineEdit1->setText(PDatei);
    Layout1->addWidget( LineEdit1 );
    ToolButton1 = new QToolButton( Drucker, "ToolButton1" );
    ToolButton1->setMinimumSize( QSize( 80, 22 ) );
    ToolButton1->setText( tr( "Change..." ) );
    ToolButton1->setEnabled(false);
    Layout1->addWidget( ToolButton1 );
    DruckerLayout->addLayout( Layout1, 1, 0 );

    OtherCom = new QCheckBox(Drucker, "Dc");
    OtherCom->setChecked(false);
    OtherCom->setText( tr("Alternative Printer Command"));
    DruckerLayout->addWidget( OtherCom, 2, 0, Qt::AlignLeft);
    LayoutCC = new QHBoxLayout;
    LayoutCC->setSpacing( 6 );
    LayoutCC->setMargin( 0 );
    OthText = new QLabel( Drucker, "DateiTc" );
    OthText->setEnabled( false );
    OthText->setText( tr( "Command:" ) );
    LayoutCC->addWidget( OthText );
    Command = new QLineEdit( Drucker, "LineEdit12" );
    Command->setMinimumSize( QSize( 240, 22 ) );
    Command->setEnabled(false);
    Command->setText(PCom);
    LayoutCC->addWidget( Command );
    DruckerLayout->addLayout( LayoutCC, 3, 0 );
    DruckLayout->addWidget( Drucker );


    Umfang = new QButtonGroup( this, "Umfang" );
    Umfang->setTitle( tr( "Range:" ) );
    Umfang->setColumnLayout(0, Qt::Vertical );
    Umfang->layout()->setSpacing( 0 );
    Umfang->layout()->setMargin( 0 );
    UmfangLayout = new QHBoxLayout( Umfang->layout() );
    UmfangLayout->setAlignment( Qt::AlignTop );
    UmfangLayout->setSpacing( 6 );
    UmfangLayout->setMargin( 5 );

    ButtonGroup5 = new QButtonGroup( Umfang, "ButtonGroup5" );
    ButtonGroup5->setFrameShape( QButtonGroup::NoFrame );
    ButtonGroup5->setTitle( tr( "" ) );
    ButtonGroup5->setColumnLayout(0, Qt::Vertical );
    ButtonGroup5->layout()->setSpacing( 0 );
    ButtonGroup5->layout()->setMargin( 0 );
    ButtonGroup5Layout = new QGridLayout( ButtonGroup5->layout() );
    ButtonGroup5Layout->setAlignment( Qt::AlignTop );
    ButtonGroup5Layout->setSpacing( 6 );
    ButtonGroup5Layout->setMargin( 5 );

    TextLabel2_2 = new QLabel( ButtonGroup5, "TextLabel2_2" );
    TextLabel2_2->setEnabled( false );
    TextLabel2_2->setText( tr( "To page:" ) );

    ButtonGroup5Layout->addWidget( TextLabel2_2, 3, 0 );

    To = new QSpinBox( ButtonGroup5, "To" );
    To->setEnabled( false );
    To->setMinimumSize( QSize( 70, 22 ) );
    To->setMinValue(1);
    To->setMaxValue(1000);
    To->setValue(1);
    ButtonGroup5Layout->addWidget( To, 3, 1 );

    From = new QSpinBox( ButtonGroup5, "From" );
    From->setEnabled( false );
    From->setMinimumSize( QSize( 70, 22 ) );
    From->setMinValue(1);
    From->setMaxValue(1000);
    From->setValue(1);

    ButtonGroup5Layout->addWidget( From, 2, 1 );

    TextLabel2 = new QLabel( ButtonGroup5, "TextLabel2" );
    TextLabel2->setEnabled( false );
    TextLabel2->setText( tr( "From page:" ) );

    ButtonGroup5Layout->addWidget( TextLabel2, 2, 0 );

    RadioButton2 = new QRadioButton( ButtonGroup5, "RadioButton2" );
    RadioButton2->setText( tr( "Print range" ) );

    ButtonGroup5Layout->addMultiCellWidget( RadioButton2, 1, 1, 0, 1 );

    RadioButton1 = new QRadioButton( ButtonGroup5, "RadioButton1" );
    RadioButton1->setText( tr( "Print all" ) );
    RadioButton1->setChecked( true );

    ButtonGroup5Layout->addWidget( RadioButton1, 0, 0 );
    UmfangLayout->addWidget( ButtonGroup5 );

    ButtonGroup4 = new QButtonGroup( Umfang, "ButtonGroup4" );
    ButtonGroup4->setFrameShape( QButtonGroup::NoFrame );
    ButtonGroup4->setTitle( tr( "" ) );
    ButtonGroup4->setColumnLayout(0, Qt::Vertical );
    ButtonGroup4->layout()->setSpacing( 0 );
    ButtonGroup4->layout()->setMargin( 0 );
    ButtonGroup4Layout = new QGridLayout( ButtonGroup4->layout() );
    ButtonGroup4Layout->setAlignment( Qt::AlignTop );
    ButtonGroup4Layout->setSpacing( 6 );
    ButtonGroup4Layout->setMargin( 5 );

    FirstPlast = new QRadioButton( ButtonGroup4, "FirstPlast" );
    FirstPlast->setText( tr( "Print last page first" ) );

    ButtonGroup4Layout->addMultiCellWidget( FirstPlast, 1, 1, 0, 1 );

    FirstPfirst = new QRadioButton( ButtonGroup4, "FirstPfirst" );
    FirstPfirst->setText( tr( "Print first page first" ) );
    FirstPfirst->setChecked( true );

    ButtonGroup4Layout->addMultiCellWidget( FirstPfirst, 0, 0, 0, 1 );

    TextLabel3 = new QLabel( ButtonGroup4, "TextLabel3" );
    TextLabel3->setText( tr( "Number of copies:" ) );

    ButtonGroup4Layout->addWidget( TextLabel3, 2, 0 );

    Copies = new QSpinBox( ButtonGroup4, "Copies" );
    Copies->setEnabled( true );
    Copies->setMinimumSize( QSize( 70, 22 ) );
    Copies->setMinValue( 1 );
    Copies->setMaxValue(1000);
    Copies->setValue(1);

    ButtonGroup4Layout->addWidget( Copies, 2, 1 );
    UmfangLayout->addWidget( ButtonGroup4 );
    DruckLayout->addWidget( Umfang );

    Optionen = new QButtonGroup( this, "Optionen" );
    Optionen->setTitle( tr( "Options" ) );
    Optionen->setColumnLayout(0, Qt::Vertical );
    Optionen->layout()->setSpacing( 0 );
    Optionen->layout()->setMargin( 0 );
    OptionenLayout = new QGridLayout( Optionen->layout() );
    OptionenLayout->setAlignment( Qt::AlignTop );
    OptionenLayout->setSpacing( 5 );
    OptionenLayout->setMargin( 5 );

    ButtonGroup3 = new QButtonGroup( Optionen, "ButtonGroup3" );
    ButtonGroup3->setGeometry( QRect( 11, 19, 138, 95 ) );
    ButtonGroup3->setFrameShape( QButtonGroup::NoFrame );
    ButtonGroup3->setTitle( tr( "" ) );
    ButtonGroup3->setColumnLayout(0, Qt::Vertical );
    ButtonGroup3->layout()->setSpacing( 0 );
    ButtonGroup3->layout()->setMargin( 0 );
    ButtonGroup3Layout = new QVBoxLayout( ButtonGroup3->layout() );
    ButtonGroup3Layout->setAlignment( Qt::AlignTop );
    ButtonGroup3Layout->setSpacing( 5 );
    ButtonGroup3Layout->setMargin( 5 );

    NormalP = new QRadioButton( ButtonGroup3, "NormalP" );
    NormalP->setText( tr( "Print Normal" ) );
    NormalP->setChecked( true );
    ButtonGroup3Layout->addWidget( NormalP );

    PrintSep = new QRadioButton( ButtonGroup3, "PrintSep" );
    PrintSep->setText( tr( "Print Separations" ) );
    ButtonGroup3Layout->addWidget( PrintSep );
    ToSeparation = false;

    SepArt = new QComboBox( true, ButtonGroup3, "SepArt" );
    SepArt->insertItem( tr( "All" ) );
    SepArt->insertItem("Cyan");
    SepArt->insertItem("Magenta");
    SepArt->insertItem("Yellow");
    SepArt->insertItem("Black");
    SepArt->setEnabled( false );
    SepArt->setEditable( false );
    ButtonGroup3Layout->addWidget( SepArt );
    OptionenLayout->addWidget( ButtonGroup3, 0, 0, Qt::AlignLeft );

    ButtonGroup3_2 = new QButtonGroup( Optionen, "ButtonGroup3_2" );
    ButtonGroup3_2->setFrameShape( QButtonGroup::NoFrame );
    ButtonGroup3_2->setTitle( "" );
    ButtonGroup3_2->setColumnLayout(0, Qt::Vertical );
    ButtonGroup3_2->layout()->setSpacing( 0 );
    ButtonGroup3_2->layout()->setMargin( 0 );
    ButtonGroup3_2Layout = new QVBoxLayout( ButtonGroup3_2->layout() );
    ButtonGroup3_2Layout->setAlignment( Qt::AlignTop );
    ButtonGroup3_2Layout->setSpacing( 5 );
    ButtonGroup3_2Layout->setMargin( 5 );

    PrintGray = new QRadioButton( ButtonGroup3_2, "PrintGray" );
    PrintGray->setText( tr( "Print in color if available" ) );
    PrintGray->setChecked( true );
    ButtonGroup3_2Layout->addWidget( PrintGray );

    PrintGray2 = new QRadioButton( ButtonGroup3_2, "PrintGray2" );
    PrintGray2->setText( tr( "Print in grayscale" ) );
    ButtonGroup3_2Layout->addWidget( PrintGray2 );
    MirrorH = false;
    MirrorV = false,
    ICCinUse = false;
    AdvOptButton = new QPushButton(ButtonGroup3_2, "Adv");
    AdvOptButton->setText( tr("Advanced Options..."));
    ButtonGroup3_2Layout->addWidget( AdvOptButton );
    
    OptionenLayout->addWidget( ButtonGroup3_2, 0, 1 );
    DruckLayout->addWidget( Optionen );

    Layout2 = new QHBoxLayout; 
    Layout2->setSpacing( 24 );
    Layout2->setMargin( 0 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout2->addItem( spacer );

    OKButton = new QPushButton( this, "OKButton" );
    OKButton->setText( tr( "OK" ) );
    OKButton->setDefault( true );
    Layout2->addWidget( OKButton );
    OKButton_2 = new QPushButton( this, "OKButton_2" );
    OKButton_2->setText( tr( "Cancel" ) );
    OKButton_2->setDefault( false );
    Layout2->addWidget( OKButton_2 );

    DruckLayout->addLayout( Layout2 );
    if ((PDev== tr("File")) || (PrintDest->count() == 1))
    	{
    	PrintDest->setCurrentItem(PrintDest->count()-1);
    	DateiT->setEnabled(true);
    	LineEdit1->setEnabled(true);
    	ToolButton1->setEnabled(true);
    	ToFile = true;
    	}
    setMaximumSize(sizeHint());

    // signals and slots connections
    connect( OKButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( OKButton_2, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( PrintDest, SIGNAL(activated(const QString&)), this, SLOT(SelPrinter(const QString&)));
    connect( RadioButton1, SIGNAL(toggled(bool)), this, SLOT(SelRange(bool)));
    connect( NormalP, SIGNAL(toggled(bool)), this, SLOT(SelMode(bool)));
    connect( ToolButton1, SIGNAL(clicked()), this, SLOT(SelFile()));
    connect( OtherCom, SIGNAL(clicked()), this, SLOT(SelComm()));
    connect( AdvOptButton, SIGNAL( clicked() ), this, SLOT( SetAdvOptions() ) );
#ifdef HAVE_CUPS
    connect( OptButton, SIGNAL( clicked() ), this, SLOT( SetOptions() ) );
#endif
}

void Druck::SetAdvOptions()
{
	AdvOptions* dia = new AdvOptions(this, MirrorH, MirrorV, ICCinUse);
	if (dia->exec())
		{
    MirrorH = dia->MirrorH->isChecked();
    MirrorV = dia->MirrorV->isChecked();
#ifdef HAVE_CMS
		if (CMSuse)
			ICCinUse = dia->UseICC->isChecked();
#endif
		}
	delete dia;
}

void Druck::SetOptions()
{
	PrinterOpts = "";
	CupsOptions* dia = new CupsOptions(this, Geraet);
	if (dia->exec())
		{
		struct CupsOptions::OpData daten;
		QMap<QString,CupsOptions::OpData>::Iterator it;
		for (it = dia->KeyToText.begin(); it != dia->KeyToText.end(); ++it)
			{
			if (dia->KeyToDefault[it.key()] != dia->FlagsOpt.at(it.data().Cnum)->currentText())
				{
				if (it.data().KeyW == "mirror")
					PrinterOpts += " -o mirror";
				else
					{
					if (it.data().KeyW == "page-set")
						{
						PrinterOpts += " -o "+it.data().KeyW+"=";
						if (dia->FlagsOpt.at(it.data().Cnum)->currentItem() == 1)
							PrinterOpts += "even";
						else
							PrinterOpts += "odd";
						}
					else
						{
						if (it.data().KeyW == "number-up")
							{
							PrinterOpts += " -o "+it.data().KeyW+"=";
							switch (dia->FlagsOpt.at(it.data().Cnum)->currentItem())
								{
								case 1:
									PrinterOpts += "2";
									break;
								case 2:
									PrinterOpts += "4";
									break;
								case 3:
									PrinterOpts += "6";
									break;
								case 4:
									PrinterOpts += "9";
									break;
								case 5:
									PrinterOpts += "16";
									break;
								}
							}
						else
							{
							if (it.data().KeyW == "orientation")
								PrinterOpts += " -o landscape";
							else
								PrinterOpts += " -o "+it.data().KeyW+"="+dia->FlagsOpt.at(it.data().Cnum)->currentText();
							}
						}
					}
				}
			}		
		}
	delete dia;
}

void Druck::SelComm()
{
	if (OtherCom->isChecked())
		{
		OthText->setEnabled(true);
		Command->setEnabled(true);
    DateiT->setEnabled(false);
    LineEdit1->setEnabled(false);
    ToolButton1->setEnabled(false);
		PrintDest->setEnabled(false);
    ToFile = false;
#ifdef HAVE_CUPS
		OptButton->setEnabled(false);
#endif
  	}
	else
		{
		OthText->setEnabled(false);
		Command->setEnabled(false);
		PrintDest->setEnabled(true);
		SelPrinter(PrintDest->currentText());
#ifdef HAVE_CUPS
		if (Geraet != tr("File"))
			OptButton->setEnabled(true);
#endif
		}
}

void Druck::SelPrinter(const QString& prn)
{
	bool setter = prn == tr("File") ? true : false;
  DateiT->setEnabled(setter);
  LineEdit1->setEnabled(setter);
  ToolButton1->setEnabled(setter);
  ToFile = setter;
#ifdef HAVE_CUPS
	OptButton->setEnabled(!setter);
#endif
	Geraet = prn;
}

void Druck::SelRange(bool e)
{
  TextLabel2_2->setEnabled( !e );
  TextLabel2->setEnabled( !e );
  To->setEnabled( !e );
  From->setEnabled( !e );
}

void Druck::SelMode(bool e)
{
	SepArt->setEnabled( !e );
  ToSeparation = !e;
}

void Druck::SelFile()
{
	CustomFDialog dia(this, tr("Save as"), tr("Postscript-Files (*.ps);;All Files (*)"), false, false);
	if (LineEdit1->text() != "")
		dia.setSelection(LineEdit1->text());
	if (dia.exec() == QDialog::Accepted)
  	LineEdit1->setText(dia.selectedFile());
}

void Druck::setMinMax(int min, int max)
{
	To->setMinValue(min);
	To->setMaxValue(max);
	From->setMinValue(min);
	From->setMaxValue(max);
}

void Druck::setFromTo(int min, int max)
{
	To->setValue(max);
	From->setValue(min);
}

QString Druck::printerName()
{
	return Geraet;
}

QString Druck::outputFileName()
{
	return LineEdit1->text();
}

bool Druck::outputToFile()
{
	return ToFile;
}

int Druck::fromPage()
{
	return From->value();
}

int Druck::toPage()
{
	return To->value();
}

int Druck::numCopies()
{
	return Copies->value();
}

int Druck::pageOrder()
{
	return FirstPfirst->isChecked() ? 0 : 1;
}

bool Druck::outputSeparations()
{
	return ToSeparation;
}

QString Druck::separationName()
{
	return SepArt->currentText();
}

bool Druck::color()
{
	return PrintGray->isChecked();
}
