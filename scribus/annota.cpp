/****************************************************************************
** Form implementation generated from reading ui file 'Annot.ui'
**
** Created: Tue Feb 19 22:31:55 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "annota.h"
#include "annota.moc"
#include "customfdialog.h"
#include <qstringlist.h>

extern QPixmap loadIcon(QString nam);

Annota::Annota(QWidget* parent, PageItem *it, int Seite, int b, int h, CListe Farben, ScribusView* vie)
            : QDialog( parent, "AN", true, 0 )
{
    setCaption( tr( "Annotation Properties" ) );
  	setIcon(loadIcon("AppIcon.png"));
  	item = it;
  	Breite = b;
  	Hoehe = h;
  	OriBreite = b;
  	OriHoehe = h;
  	view = vie;
	MaxSeite = Seite;
    QStringList tl;
	if ((item->AnActType == 2) || (item->AnActType == 7))
	{
    	QString tm = item->AnAction;
    	tl = tl.split(" ", tm);
	}
	else
	{
		tl.append("0");
		tl.append("0");
	}
		
    AnnotLayout = new QVBoxLayout( this ); 
    AnnotLayout->setSpacing( 6 );
    AnnotLayout->setMargin( 11 );

    Layout1 = new QHBoxLayout; 
    Layout1->setSpacing( 6 );
    Layout1->setMargin( 0 );

    TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setText( tr( "Type:" ) );
    Layout1->addWidget( TextLabel1 );

    ComboBox1 = new QComboBox( true, this, "ComboBox1" );
    ComboBox1->insertItem( tr( "Text" ) );
    ComboBox1->insertItem( tr( "Link" ) );
    ComboBox1->insertItem( tr( "External Link" ) );
    ComboBox1->insertItem( tr( "External Web-Link" ) );
    ComboBox1->setEditable(false);
    Layout1->addWidget( ComboBox1 );
    AnnotLayout->addLayout( Layout1 );
		item->AnType < 2 ? ComboBox1->setCurrentItem(item->AnType) : ComboBox1->setCurrentItem(item->AnType-10);
    if (item->AnActType == 7)
			ComboBox1->setCurrentItem(2);
    if (item->AnActType == 8)
			ComboBox1->setCurrentItem(3);
    Fram = new QWidgetStack(this);
    AnnotLayout->addWidget( Fram );

    GroupBox1 = new QGroupBox( this, "GroupBox1" );
    GroupBox1->setTitle( tr( "Destination" ) );
    GroupBox1->setColumnLayout(0, Qt::Vertical );
    GroupBox1->layout()->setSpacing( 0 );
    GroupBox1->layout()->setMargin( 0 );
    GroupBox1Layout = new QGridLayout( GroupBox1->layout() );
    GroupBox1Layout->setAlignment( Qt::AlignTop );
    GroupBox1Layout->setSpacing( 6 );
    GroupBox1Layout->setMargin( 11 );

    Destfile = new QLineEdit(GroupBox1, "File");
	Destfile->setText(item->An_Extern);
	Destfile->setReadOnly(true);
    GroupBox1Layout->addMultiCellWidget( Destfile, 0, 0, 0, 1 );
	ChFile = new QPushButton(GroupBox1, "Change");
	ChFile->setText( tr("Change..."));
    GroupBox1Layout->addWidget( ChFile, 0, 2 );
	if ((item->AnActType != 7) && (item->AnActType != 8))
	{
		Destfile->hide();
		ChFile->hide();
	}

    TextLabel3 = new QLabel( GroupBox1, "TextLabel3" );
    TextLabel3->setText( tr( "Page:" ) );
    GroupBox1Layout->addWidget( TextLabel3, 1, 0 );
    SpinBox1 = new QSpinBox( GroupBox1, "SpinBox1" );
    SpinBox1->setMinValue(1);
    SpinBox1->setMaxValue(item->AnActType == 7 ? 1000 : Seite);
    SpinBox1->setValue(item->AnZiel+1);
    GroupBox1Layout->addWidget( SpinBox1, 1, 1 );
	if ((Destfile->text() != "") && (item->AnActType == 7))
    	Pg = new Navigator( GroupBox1, 100, item->AnZiel+1, view, item->An_Extern);
	else
    	Pg = new Navigator( GroupBox1, 100, item->AnZiel, view);
    Pg->setMinimumSize(QSize(Pg->pmx.width(), Pg->pmx.height()));
    GroupBox1Layout->addMultiCellWidget(Pg, 1, 3, 2, 2);

    TextLabel4 = new QLabel( GroupBox1, "TextLabel4" );
    TextLabel4->setText( tr( "X-Pos:" ) );
    GroupBox1Layout->addWidget( TextLabel4, 2, 0 );
    SpinBox2 = new QSpinBox( GroupBox1, "SpinBox2" );
    SpinBox2->setSuffix( tr( " pt" ) );
    SpinBox2->setMaxValue(Breite);
    SpinBox2->setValue(tl[0].toInt());
    GroupBox1Layout->addWidget( SpinBox2, 2, 1 );
    TextLabel5 = new QLabel( GroupBox1, "TextLabel5" );
    TextLabel5->setText( tr( "Y-Pos:" ) );
    GroupBox1Layout->addWidget( TextLabel5, 3, 0 );
    SpinBox3 = new QSpinBox( GroupBox1, "SpinBox3" );
    SpinBox3->setMaxValue(Hoehe);
    SpinBox3->setSuffix( tr( " pt" ) );
    SpinBox3->setValue(Hoehe-tl[1].toInt());
    GroupBox1Layout->addWidget( SpinBox3, 3, 1 );
    Fram->addWidget(GroupBox1, 1);

    Frame9 = new QFrame( this, "Frame7" );
    Frame9->setFrameShape( QFrame::NoFrame );
    Frame9->setFrameShadow( QFrame::Plain );
    Fram->addWidget(Frame9, 2);

    Layout1_2 = new QHBoxLayout; 
    Layout1_2->setSpacing( 6 );
    Layout1_2->setMargin( 0 );

    QSpacerItem* spacerr = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1_2->addItem( spacerr );
    PushButton1 = new QPushButton( this, "PushButton1" );
    PushButton1->setText( tr( "OK" ) );
    PushButton1->setDefault( true );
    Layout1_2->addWidget( PushButton1 );
    PushButton2 = new QPushButton( this, "PushButton2" );
    PushButton2->setText( tr( "Cancel" ) );
    Layout1_2->addWidget( PushButton2 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1_2->addItem( spacer );
    AnnotLayout->addLayout( Layout1_2 );
    connect(PushButton1, SIGNAL(clicked()), this, SLOT(SetVals()));
    connect(PushButton2, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ComboBox1, SIGNAL(activated(int)), this, SLOT(SetZiel(int)));
    connect(SpinBox1, SIGNAL(valueChanged(int)), this, SLOT(SetPg(int)));
    connect(Pg, SIGNAL(Coords(double, double)), this, SLOT(SetCo(double, double)));
    connect(SpinBox2, SIGNAL(valueChanged(int)), this, SLOT(SetCross()));
    connect(SpinBox3, SIGNAL(valueChanged(int)), this, SLOT(SetCross()));
	connect(ChFile, SIGNAL(clicked()), this, SLOT(GetFile()));
    SetZiel(item->AnType);
    SetCross();
}

void Annota::SetCo(double x, double y)
{
	SpinBox2->setValue(static_cast<int>(x*Breite));
	SpinBox3->setValue(static_cast<int>(y*Hoehe));
}

void Annota::SetPg(int v)
{
	disconnect(SpinBox1, SIGNAL(valueChanged(int)), this, SLOT(SetPg(int)));
	if (ComboBox1->currentItem() == 2)
	{
		if (!Pg->SetSeite(v, 100, Destfile->text()))
		{
			SpinBox1->setValue(1);
			Pg->SetSeite(1, 100, Destfile->text());
		}
		Breite = Pg->Breite;
		Hoehe = Pg->Hoehe;
	}
	else
	{
		Pg->SetSeite(v-1, 100);
		SpinBox1->setValue(v);
		Breite = OriBreite;
		Hoehe = OriHoehe;
	}
	SpinBox2->setMaxValue(Breite);
	SpinBox3->setMaxValue(Hoehe);
	connect(SpinBox1, SIGNAL(valueChanged(int)), this, SLOT(SetPg(int)));
}

void Annota::SetCross()
{
	int x,y;
	disconnect(Pg, SIGNAL(Coords(double, double)), this, SLOT(SetCo(double, double)));
	x = static_cast<int>(static_cast<double>(SpinBox2->value())/static_cast<double>(Breite)*Pg->pmx.width());
	y = static_cast<int>(static_cast<double>(SpinBox3->value())/static_cast<double>(Hoehe)*Pg->pmx.height());
	Pg->drawMark(x, y);
	connect(Pg, SIGNAL(Coords(double, double)), this, SLOT(SetCo(double, double)));
}

void Annota::SetVals()
{
	QString tmp, tmp2;
	item->AnZiel = SpinBox1->value()-1;
	item->AnType = ComboBox1->currentItem()+10;
	if (item->AnType == 10)
		item->AnActType = 0;
	if (item->AnType == 11)
	{
		item->AnAction = tmp.setNum(SpinBox2->value())+" "+tmp2.setNum(Hoehe-SpinBox3->value())+" 0";
		item->An_Extern = "";
		item->AnActType = 2;
	}
	if (item->AnType == 12)
	{
		item->AnAction = tmp.setNum(SpinBox2->value())+" "+tmp2.setNum(Hoehe-SpinBox3->value())+" 0";
		if (Destfile->text() != "")
		{
			item->An_Extern = Destfile->text();
			item->AnActType = 7;
		}
		item->AnType = 11;
	}
	if (item->AnType == 13)
	{
		item->AnAction = "";
		if (Destfile->text() != "")
		{
			item->An_Extern = Destfile->text();
			item->AnActType = 8;
		}
		item->AnType = 11;
	}
	accept();
}

void Annota::SetZiel(int it)
{
	disconnect(ComboBox1, SIGNAL(activated(int)), this, SLOT(SetZiel(int)));
	Pg->show();
	TextLabel3->show();
	TextLabel4->show();
	TextLabel5->show();
	SpinBox1->show();
	SpinBox2->show();
	SpinBox3->show();
	switch (it)
	{
		case 1:
    		Fram->raiseWidget(1);
			Destfile->setText("");
			Destfile->hide();
			ChFile->hide();
			item->AnActType = 2;
			SetPg(QMIN(SpinBox1->value(), MaxSeite));
			break;
		case 2:
    		Fram->raiseWidget(1);
			Destfile->show();
			ChFile->show();
			Destfile->setReadOnly(true);
			if ((Destfile->text() == "")  || (item->AnActType == 8))
			{
				Destfile->setText("");
				GetFile();
			}
			if (Destfile->text() == "")
			{
				item->AnActType = 2;
				Destfile->setText("");
				Destfile->hide();
				ChFile->hide();
				ComboBox1->setCurrentItem(1);
			}
			else
				item->AnActType = 7;
			SetPg(QMIN(SpinBox1->value(), MaxSeite));
			break;
		case 3:
    		Fram->raiseWidget(1);
			Destfile->show();
			Destfile->setReadOnly(false);
			ChFile->hide();
			Pg->hide();
			TextLabel3->hide();
			TextLabel4->hide();
			TextLabel5->hide();
			SpinBox1->hide();
			SpinBox2->hide();
			SpinBox3->hide();
			item->AnActType = 8;
			break;
		case 11:
    		Fram->raiseWidget(1);
			if (item->AnActType == 7)
			{
				Destfile->show();
				ChFile->show();
				Destfile->setReadOnly(true);
			}
			if (item->AnActType == 8)
			{
				Destfile->show();
				Destfile->setReadOnly(false);
				ChFile->hide();
				Pg->hide();
				TextLabel3->hide();
				TextLabel4->hide();
				TextLabel5->hide();
				SpinBox1->hide();
				SpinBox2->hide();
				SpinBox3->hide();
			}
			if (Pg->isVisible())
				SetPg(QMIN(SpinBox1->value(), MaxSeite));
			break;
		default:
    		Fram->raiseWidget(2);
			break;
	}
	connect(ComboBox1, SIGNAL(activated(int)), this, SLOT(SetZiel(int)));
}

void Annota::GetFile()
{
	QString fn;
	CustomFDialog dia(this, tr("Open"), tr("PDF-Documents (*.pdf);;All Files (*)"));
	if (Destfile->text() != "")
		dia.setSelection(Destfile->text());
	if (dia.exec() == QDialog::Accepted)
	{
		fn = dia.selectedFile();
		if (!fn.isEmpty())
		{
			Destfile->setText(fn);
			SpinBox1->setValue(1);
    		SpinBox1->setMaxValue(1000);
			SetPg(1);
		}
	}
}
