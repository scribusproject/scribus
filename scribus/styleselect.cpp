#include "styleselect.h"
#include "styleselect.moc"

extern QPixmap loadIcon(QString nam);

StyleSelect::StyleSelect(QWidget* parent) : QWidget(parent, "StyleSelect")
{
	Layout1 = new QHBoxLayout( this, 0, 0, "Layout1");

    Underline = new QToolButton( this, "Underline" );
    Underline->setMaximumSize( QSize( 22, 22 ) );
    Underline->setText("");
    Underline->setPixmap(loadIcon("Unter.xpm"));
    Underline->setToggleButton( true );
    Layout1->addWidget( Underline );

    ButtonGroup1 = new QGroupBox( this, "ButtonGroup1" );
    ButtonGroup1->setFrameShape( QGroupBox::NoFrame );
    ButtonGroup1->setTitle("");
    ButtonGroup1->setColumnLayout(0, Qt::Vertical );
    ButtonGroup1->layout()->setSpacing( 0 );
    ButtonGroup1->layout()->setMargin( 0 );
    ButtonGroup1Layout = new QHBoxLayout( ButtonGroup1->layout() );
    ButtonGroup1Layout->setAlignment( Qt::AlignTop );

    Subs = new QToolButton( ButtonGroup1, "Subs" );
    Subs->setMaximumSize( QSize( 22, 22 ) );
    Subs->setText("");
    Subs->setPixmap(loadIcon("Tief.xpm"));
    Subs->setToggleButton( true );
    ButtonGroup1Layout->addWidget( Subs );

    Supers = new QToolButton( ButtonGroup1, "Supers" );
    Supers->setMaximumSize( QSize( 22, 22 ) );
    Supers->setText("");
    Supers->setPixmap(loadIcon("Hoch.xpm"));
    Supers->setToggleButton( true );
    ButtonGroup1Layout->addWidget( Supers );
    Layout1->addWidget( ButtonGroup1 );

    Kapital = new QToolButton( this, "Kapital" );
    Kapital->setMaximumSize( QSize( 22, 22 ) );
    Kapital->setText("");
    Kapital->setPixmap(loadIcon("Kapital.xpm"));
    Kapital->setToggleButton( true );
    Layout1->addWidget( Kapital );

    Strike = new QToolButton( this, "Strike" );
    Strike->setMaximumSize( QSize( 22, 22 ) );
    Strike->setText("");
    Strike->setPixmap(loadIcon("Strike.xpm"));
    Strike->setToggleButton( true );
    Layout1->addWidget( Strike );

    Outlined = new QToolButton( this, "Outlined" );
    Outlined->setMaximumSize( QSize( 22, 22 ) );
    Outlined->setText("");
    Outlined->setPixmap(loadIcon("outlined.png"));
    Outlined->setToggleButton( true );
    Layout1->addWidget( Outlined );
    QToolTip::add( Underline, tr( "Underline" ) );
    QToolTip::add( Kapital, tr( "Small Caps" ) );
    QToolTip::add( Subs, tr( "Subscript" ) );
    QToolTip::add( Supers, tr( "Superscript" ) );
    QToolTip::add( Strike, tr( "Strike Out" ) );
    QToolTip::add( Outlined, tr( "Outline Text" ) );
    connect(Kapital, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
    connect(Underline, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
    connect(Subs, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
    connect(Strike, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
    connect(Supers, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
    connect(Outlined, SIGNAL(clicked()), this, SLOT(setTypeStyle()));
    resize(minimumSizeHint());
}

void StyleSelect::setStyle(int s)
{
	Supers->setOn(false);
	Subs->setOn(false);
	Strike->setOn(false);
	Underline->setOn(false);
	Kapital->setOn(false);
	Outlined->setOn(false);
	if (s & 1)
		Supers->setOn(true);
	if (s & 2)
		Subs->setOn(true);
	if (s & 4)
		Outlined->setOn(true);
	if (s & 8)
		Underline->setOn(true);
	if (s & 16)
		Strike->setOn(true);
	if (s & 64)
		Kapital->setOn(true);
}

int StyleSelect::getStyle()
{
	int ret = 0;
	if (Supers->isOn())
		ret |= 1;
	if (Subs->isOn())
		ret |= 2;
	if (Outlined->isOn())
		ret |= 4;
	if (Underline->isOn())
		ret |= 8;
	if (Strike->isOn())
		ret |= 16;
	if (Kapital->isOn())
		ret |= 64;
	return ret;
}

void StyleSelect::setTypeStyle()
{
	if (Supers == sender())
		Subs->setOn(false);
	if (Subs == sender())
		Supers->setOn(false);
	emit State(getStyle());
}

