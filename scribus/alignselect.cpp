#include "alignselect.h"
#include "alignselect.moc"

extern QPixmap loadIcon(QString nam);

AlignSelect::AlignSelect(QWidget* parent) : QButtonGroup(parent, "AlignSelect")
{
    setFrameShape( QButtonGroup::NoFrame );
    setTitle("");
    setExclusive( true );
    setColumnLayout(0, Qt::Vertical );
    layout()->setSpacing( 0 );
    layout()->setMargin( 0 );
    GroupAlignLayout = new QGridLayout( layout() );
    GroupAlignLayout->setAlignment( Qt::AlignTop );

    TextL = new QToolButton( this, "TextL" );
    TextL->setMaximumSize( QSize( 22, 22 ) );
    TextL->setPixmap(loadIcon("text_left.png"));
    TextL->setToggleButton( true );
    TextL->setOn( true );
    GroupAlignLayout->addWidget( TextL, 0, 0 );

    TextR = new QToolButton( this, "TextR" );
    TextR->setMaximumSize( QSize( 22, 22 ) );
    TextR->setPixmap(loadIcon("text_right.png"));
    TextR->setToggleButton( true );
    GroupAlignLayout->addWidget( TextR, 0, 1 );

    TextC = new QToolButton( this, "TextC" );
    TextC->setMaximumSize( QSize( 22, 22 ) );
    TextC->setPixmap(loadIcon("text_center.png"));
    TextC->setToggleButton( true );
    GroupAlignLayout->addWidget( TextC, 0, 2 );

    TextB = new QToolButton( this, "TextB" );
    TextB->setMaximumSize( QSize( 22, 22 ) );
    TextB->setPixmap(loadIcon("text_block.png"));
    TextB->setToggleButton( true );
    GroupAlignLayout->addWidget( TextB, 0, 3 );

    TextF = new QToolButton( this, "TextF" );
    TextF->setMaximumSize( QSize( 22, 22 ) );
    TextF->setPixmap(loadIcon("text_force.png"));
    TextF->setToggleButton( true );
    GroupAlignLayout->addWidget( TextF, 0, 4 );
    QToolTip::add( TextL, tr( "Align Text Left" ) );
    QToolTip::add( TextR, tr( "Align Text Right" ) );
    QToolTip::add( TextB, tr( "Align Text Block" ) );
    QToolTip::add( TextC, tr( "Align Text Center" ) );
    resize(minimumSizeHint());
    connect(this, SIGNAL(clicked(int)), this, SLOT(setTypeStyle(int)));
}

void AlignSelect::setStyle(int s)
{
	switch (s)
		{
		case 0:
			setButton(0);
			break;
		case 2:
			setButton(1);
			break;
		case 1:
			setButton(2);
			break;
		case 3:
			setButton(3);
			break;
		case 4:
			setButton(4);
			break;
		}
}

int AlignSelect::getStyle()
{
	int ret = 0;
	if (TextL->isOn())
		ret = 0;
	if (TextR->isOn())
		ret = 2;
	if (TextC->isOn())
		ret = 1;
	if (TextB->isOn())
		ret = 3;
	if (TextF->isOn())
		ret = 4;
	return ret;
}

void AlignSelect::setTypeStyle(int a)
{
	switch (a)
	{
		case 0:
			emit State(0);
			break;
		case 1:
			emit State(2);
			break;
		case 2:
			emit State(1);
			break;
		case 3:
			emit State(3);
			break;
		case 4:
			emit State(4);
			break;
	}
}
