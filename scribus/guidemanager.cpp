#include "guidemanager.h"
#include "guidemanager.moc"
extern QPixmap loadIcon(QString nam);
extern double UmReFaktor;
 
GuideManager::GuideManager( QWidget* parent, Page* page, int Einh)
    : QDialog( parent, "GuideManager", true, 0 )
{
	QString tmp;
	int decimals;
    setCaption( tr( "Manage Guides" ) );
  	setIcon(loadIcon("AppIcon.png"));
	Einheit = Einh;
	switch (Einheit)
		{
			case 0:
				decimals = 100;
				break;
			case 1:
				decimals = 1000;
				break;
			case 2:
				decimals = 10000;
				break;
			case 3:
				decimals = 100;
				break;
		}

	XLocal = page->XGuides;
	YLocal = page->YGuides;
	pag = page;
	curX = 0;
	curY = 0;
    GuideManagerLayout = new QVBoxLayout( this, 11, 6, "GuideManagerLayout"); 
    Layout6 = new QHBoxLayout( 0, 0, 6, "Layout6"); 

    VerGroup = new QGroupBox( this, "VerGroup" );
    VerGroup->setTitle( tr( "Horizontal Guides" ) );
    VerGroup->setColumnLayout(0, Qt::Vertical );
    VerGroup->layout()->setSpacing( 6 );
    VerGroup->layout()->setMargin( 11 );
    VerGroupLayout = new QVBoxLayout( VerGroup->layout() );
    VerGroupLayout->setAlignment( Qt::AlignTop );
    VerticalList = new QListBox( VerGroup, "VerticalList" );
    VerticalList->setMinimumSize( QSize( 0, 200 ) );
    VerGroupLayout->addWidget( VerticalList );
    Layout2 = new QHBoxLayout( 0, 0, 6, "Layout2"); 
    TextLabel1 = new QLabel( VerGroup, "TextLabel1" );
    TextLabel1->setText( tr( "Y-Pos:" ) );
    Layout2->addWidget( TextLabel1 );
    VertSpin = new MSpinBox( VerGroup, 4 );
	VertSpin->setDecimals( decimals );
	VertSpin->setMaxValue(page->doku->PageH);
	VertSpin->setMinValue(0);
    Layout2->addWidget( VertSpin );
    VerGroupLayout->addLayout( Layout2 );
    Layout1 = new QHBoxLayout( 0, 0, 6, "Layout1"); 
    VertSet = new QPushButton( VerGroup, "VertSet" );
    VertSet->setText( tr( "Add" ) );
    VertSet->setAutoDefault( false );
    Layout1->addWidget( VertSet );
    VertDel = new QPushButton( VerGroup, "VertDel" );
    VertDel->setText( tr( "Delete" ) );
    VertDel->setAutoDefault( false );
    Layout1->addWidget( VertDel );
    VerGroupLayout->addLayout( Layout1 );
    Layout6->addWidget( VerGroup );

    HorGroup = new QGroupBox( this, "HorGroup" );
    HorGroup->setTitle( tr( "Vertical Guides" ) );
    HorGroup->setColumnLayout(0, Qt::Vertical );
    HorGroup->layout()->setSpacing( 6 );
    HorGroup->layout()->setMargin( 11 );
    HorGroupLayout = new QVBoxLayout( HorGroup->layout() );
    HorGroupLayout->setAlignment( Qt::AlignTop );
    HorList = new QListBox( HorGroup, "HorList" );
    HorList->setMinimumSize( QSize( 0, 200 ) );
    HorGroupLayout->addWidget( HorList );
    Layout4 = new QHBoxLayout( 0, 0, 6, "Layout4"); 
    TextLabel2 = new QLabel( HorGroup, "TextLabel2" );
    TextLabel2->setText( tr( "X-Pos:" ) );
    Layout4->addWidget( TextLabel2 );
    HoriSpin = new MSpinBox( HorGroup, 4 );
	HoriSpin->setDecimals( decimals );
	HoriSpin->setMaxValue(page->doku->PageB);
	HoriSpin->setMinValue(0);
    Layout4->addWidget( HoriSpin );
    HorGroupLayout->addLayout( Layout4 );
    Layout3 = new QHBoxLayout( 0, 0, 6, "Layout3"); 
    HoriSet = new QPushButton( HorGroup, "HoriSet" );
    HoriSet->setText( tr( "Add" ) );
    HoriSet->setAutoDefault( false );
    Layout3->addWidget( HoriSet );
    HoriDel = new QPushButton( HorGroup, "HoriDel" );
    HoriDel->setText( tr( "Delete" ) );
    HoriDel->setAutoDefault( false );
    Layout3->addWidget( HoriDel );
    HorGroupLayout->addLayout( Layout3 );
    Layout6->addWidget( HorGroup );
    GuideManagerLayout->addLayout( Layout6 );
    Lock = new QCheckBox(this, "Lock");
    Lock->setText( tr("Lock Guides"));
    Lock->setChecked(page->doku->GuideLock);
    GuideManagerLayout->addWidget(Lock);
    Layout5 = new QHBoxLayout( 0, 0, 6, "Layout5"); 
    QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout5->addItem( spacer );
    OK = new QPushButton( this, "OK" );
    OK->setText( tr( "OK" ) );
    OK->setDefault( true );
    Layout5->addWidget( OK );
    QSpacerItem* spacer_2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout5->addItem( spacer_2 );
    Cancel = new QPushButton( this, "Cancel" );
    Cancel->setText( tr( "Cancel" ) );
    Layout5->addWidget( Cancel );
    QSpacerItem* spacer_3 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout5->addItem( spacer_3 );
    GuideManagerLayout->addLayout( Layout5 );
	if (XLocal.count() != 0)
		GetHVal(0);
	else
		HoriSpin->setValue(0);
	if (YLocal.count() != 0)
		GetVVal(0);
	else
		VertSpin->setValue(0);
	UnitChange();
	UpdateHList();
	UpdateVList();
    VerticalList->setFocusPolicy(QWidget::ClickFocus);
	HorList->setFocusPolicy(QWidget::ClickFocus);
	OK->setFocus();
    // signals and slots connections
    connect( OK, SIGNAL( clicked() ), this, SLOT( ExitOK() ) );
    connect( Cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect(HorList, SIGNAL(highlighted(int)), this, SLOT(GetHVal(int)));
    connect(VerticalList, SIGNAL(highlighted(int)), this, SLOT(GetVVal(int)));
	connect(VertSet, SIGNAL(clicked()), this, SLOT(SetVVal()));
	connect(VertDel, SIGNAL(clicked()), this, SLOT(DelVVal()));
	connect(HoriSet, SIGNAL(clicked()), this, SLOT(SetHVal()));
	connect(HoriDel, SIGNAL(clicked()), this, SLOT(DelHVal()));
	connect(HoriSpin, SIGNAL(valueChanged(int)), this, SLOT(valChange()));
	connect(VertSpin, SIGNAL(valueChanged(int)), this, SLOT(valChange()));
	connect(Lock, SIGNAL(clicked()), this, SLOT(HandleLock()));
	valChange();
}

void GuideManager::HandleLock()
{
	pag->doku->GuideLock = Lock->isChecked();
}

void GuideManager::UnitChange()
{
	QString tmp = GetUnit();
	HoriSpin->setSuffix(tmp);
	VertSpin->setSuffix(tmp);
}

QString GuideManager::GetUnit()
{
	QString tmp = (Einheit == 0) ? tr(" pt") : 
					(Einheit == 1) ? tr(" mm") :
					(Einheit == 2) ? tr(" in") : tr(" p"); 
	return tmp;
}

void GuideManager::UpdateHList()
{
	QString tmp;
	HorList->clear();
	if (XLocal.count() != 0)
	{
		for (uint xg = 0; xg < XLocal.count(); ++xg)
	   		HorList->insertItem(tmp.setNum(qRound(XLocal[xg]*UmReFaktor*10000.0)/10000.0, 'f', 4)+GetUnit());
	}
}

void GuideManager::UpdateVList()
{
	QString tmp;
	VerticalList->clear();
	if (YLocal.count() != 0)
	{
		for (uint yg = 0; yg < YLocal.count(); ++yg)
   			VerticalList->insertItem(tmp.setNum(qRound(YLocal[yg]*UmReFaktor*10000.0)/10000.0, 'f', 4)+GetUnit());
	}
}

void GuideManager::GetHVal(int n)
{
	HoriSpin->setValue(XLocal[n]*UmReFaktor);
	curX = n;
	HoriDel->setEnabled(true);
}

void GuideManager::GetVVal(int n)
{
	VertSpin->setValue(YLocal[n]*UmReFaktor);
	curY = n;
	VertDel->setEnabled(true);
}

void GuideManager::SetHVal()
{
	XLocal.append(HoriSpin->value() / UmReFaktor);
	qHeapSort(XLocal);
	UpdateHList();
	setFocus();
}

void GuideManager::SetVVal()
{
	YLocal.append(VertSpin->value() / UmReFaktor);
	qHeapSort(YLocal);
	UpdateVList();
	setFocus();
}

void GuideManager::DelHVal()
{
	XLocal.remove(XLocal[curX]);
	UpdateHList();
	HoriDel->setEnabled(false);
}

void GuideManager::DelVVal()
{
	YLocal.remove(YLocal[curY]);
	UpdateVList();
	VertDel->setEnabled(false);
}

void GuideManager::valChange()
{
	HoriDel->setEnabled(false);
	VertDel->setEnabled(false);
}

void GuideManager::ExitOK()
{
	pag->XGuides = XLocal;
	pag->YGuides = YLocal;
	pag->update();
	accept();
}
