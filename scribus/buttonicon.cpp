#include "buttonicon.h"
#include "buttonicon.moc"
extern QPixmap loadIcon(QString nam);

ButtonIcon::ButtonIcon(QWidget* parent, PageItem* ite)
		: QDialog( parent, "bu", true, 0 )
{
	Item = ite;
	setCaption( tr( "Icon Placement" ) );
	setIcon(loadIcon("AppIcon.png"));
	ButtonIconLayout = new QVBoxLayout( this, 11, 6, "ButtonIconLayout");
	Layout1 = new QGridLayout( 0, 0, 6, "Layout6");
	TextLabel1_2 = new QLabel( this, "TextLabel1_2" );
	TextLabel1_2->setText( tr( "Layout:" ) );
	Layout1->addWidget( TextLabel1_2, 0, 0 );
	Place = new QComboBox( true, this, "Place" );
	QString tmp_pla[] = { tr("Caption only"), tr("Icon only"), tr("Caption below Icon"), tr("Caption above Icon"),
	                      tr("Caption right to Icon"), tr("Caption left to Icon"), tr("Caption overlays Icon")};
	size_t array_pla = sizeof(tmp_pla) / sizeof(*tmp_pla);
	/* PFJ - 29/02/04 - Changed from uint to int and var name */
	for (uint prop = 0; prop < array_pla; ++prop)
		Place->insertItem(tmp_pla[prop]);
	Place->setEditable(false);
	Layout1->addWidget( Place, 0, 1 );
	TextLabel1 = new QLabel( this, "TextLabel1" );
	TextLabel1->setText( tr( "Scale:" ) );
	Layout1->addWidget( TextLabel1, 1, 0 );
	ScaleW = new QComboBox( true, this, "ScaleW" );
	/* PFJ - 29/02/04 - Changed to QString/size_t/for style. It saves a small
						of time during the setup */
	QString scalew[] = {tr("Always"), tr("When Icon it too small"),
	                    tr("When Icon is too big"), tr("Never")};
	size_t scalewArray = sizeof(scalew) / sizeof(*scalew);
	for (uint prop = 0; prop < scalewArray; ++prop)
		ScaleW->insertItem(scalew[prop]);
	ScaleW->setEditable(false);
	Layout1->addWidget( ScaleW, 1, 1 );
	TextLabel2 = new QLabel( this, "TextLabel2" );
	TextLabel2->setText( tr( "Scale How:" ) );
	Layout1->addWidget( TextLabel2, 2, 0 );
	ScaleH = new QComboBox( true, this, "ScaleH" );
	ScaleH->insertItem( tr( "Proportional" ) );
	ScaleH->insertItem( tr( "Non Proportional" ) );
	ScaleH->setEditable(false);
	Layout1->addWidget( ScaleH, 2, 1 );
	ButtonIconLayout->addLayout( Layout1 );
	QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	ButtonIconLayout->addItem( spacer );
	Layout5 = new QHBoxLayout( 0, 0, 6, "Layout5");
	QSpacerItem* spacer_2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout5->addItem( spacer_2 );
	Layout3 = new QGridLayout( 0, 1, 1, 0, 6, "Layout3");
	GroupButton = new QGroupBox( this, "GroupButton" );
	GroupButton->setMinimumSize( QSize( 150, 150 ) );
	GroupButton->setMaximumSize( QSize( 150, 150 ) );
	GroupButton->setTitle( tr( "" ) );
	TextLabel3 = new QLabel( GroupButton, "TextLabel3" );
	TextLabel3->setGeometry( QRect( 50, 50, 48, 48 ) );
	TextLabel3->setFrameShape( QLabel::Box );
	TextLabel3->setFrameShadow( QLabel::Raised );
	TextLabel3->setLineWidth( 1 );
	TextLabel3->setText( tr( "Icon" ) );
	Layout3->addWidget( GroupButton, 0, 0 );
	SliderX = new QSlider( this, "SliderX" );
	SliderX->setMaxValue( 1000 );
	SliderX->setValue( 500 );
	SliderX->setOrientation( QSlider::Horizontal );
	SliderX->setTickmarks( QSlider::Left );
	SliderX->setTickInterval( 100 );
	Layout3->addWidget( SliderX, 1, 0 );
	SliderY = new QSlider( this, "SliderY" );
	SliderY->setMaxValue( 1000 );
	SliderY->setValue( 500 );
	SliderY->setOrientation( QSlider::Vertical );
	SliderY->setTickmarks( QSlider::Left );
	SliderY->setTickInterval( 100 );
	Layout3->addWidget( SliderY, 0, 1 );
	Layout5->addLayout( Layout3 );
	QSpacerItem* spacer_3 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout5->addItem( spacer_3 );
	ButtonIconLayout->addLayout( Layout5 );
	QSpacerItem* spacer_4 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	ButtonIconLayout->addItem( spacer_4 );
	Layout4 = new QHBoxLayout( 0, 0, 6, "Layout4");
	OK = new QPushButton( this, "OK" );
	OK->setText( tr( "OK" ) );
	OK->setDefault( TRUE );
	Layout4->addWidget( OK );
	Cancel = new QPushButton( this, "Cancel" );
	Cancel->setText( tr( "Cancel" ) );
	Layout4->addWidget( Cancel );
	Reset = new QPushButton( this, "Reset" );
	Reset->setText( tr( "Reset" ) );
	Layout4->addWidget( Reset );
	ButtonIconLayout->addLayout( Layout4 );

	SetAllVals();

	connect(ScaleW, SIGNAL(activated(int)), this, SLOT(SetScaleHow(int)));
	connect(SliderX, SIGNAL(valueChanged(int)), this, SLOT(MoveIconX(int)));
	connect(SliderY, SIGNAL(valueChanged(int)), this, SLOT(MoveIconY(int)));
	connect(Cancel, SIGNAL(clicked()), this, SLOT(reject()));
	connect(OK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(Reset, SIGNAL(clicked()), this, SLOT(SetAllVals()));
}

void ButtonIcon::MoveIconY(int y)
{
	TextLabel3->setGeometry(QRect(static_cast<int>(SliderX->value() / 1000.0 * 100),
	                              static_cast<int>(y / 1000.0 * 100), 48, 48));
	IcPlaceY = y / 1000.0;
}

void ButtonIcon::MoveIconX(int x)
{
	TextLabel3->setGeometry(QRect(static_cast<int>(x / 1000.0 * 100),
	                              static_cast<int>(SliderY->value() / 1000.0 * 100), 48, 48));
	IcPlaceX = x / 1000.0;
}

void ButtonIcon::SetAllVals()
{
	Place->setCurrentItem(Item->AnIPlace);
	ScaleW->setCurrentItem(Item->AnScaleW);
	ScaleH->setCurrentItem(Item->LocalScX != Item->LocalScY ? 1 : 0);
	SetScaleHow(ScaleW->currentItem());
	SliderX->setValue(static_cast<int>(Item->LocalX / (Item->Width - Item->pixm.width()) * 1000));
	SliderY->setValue(static_cast<int>(Item->LocalY / (Item->Height - Item->pixm.height()) * 1000));
	TextLabel3->setGeometry(QRect(static_cast<int>(SliderX->value() / 1000.0 * 100),
	                              static_cast<int>(SliderY->value() / 1000.0 * 100), 48, 48));
}

void ButtonIcon::SetScaleHow(int s)
{
	bool setter = (s == 3) ? true : false;
	GroupButton->setEnabled(setter);
	TextLabel3->setEnabled(setter);
	SliderX->setEnabled(setter);
	SliderY->setEnabled(setter);
	IcScaleH = s;
}
