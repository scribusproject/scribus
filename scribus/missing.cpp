#include "missing.h"
#include "missing.moc"
extern QPixmap loadIcon(QString nam);

#include <qimage.h>
#include <qpixmap.h>

static const char* const image100_data[] = {
"48 48 4 1",
". c None",
"b c #000000",
"# c #a4a1a4",
"a c #ffffff",
"................................................",
"................................................",
"................................................",
"................................................",
"................................................",
"................................................",
"................................................",
"................................................",
"................................................",
"................................................",
"...................########.....................",
"................###aaaaaaaa###..................",
"..............##aaaaaaaaaaaaaa##................",
".............#aaaaaaaaaaaaaaaaaa#...............",
"............#aaaaaaaabbbbaaaaaaaab..............",
"...........#aaaaaaaabbbbbbaaaaaaaab.............",
"..........#aaaaaaaaabbbbbbaaaaaaaaab............",
".........#aaaaaaaaaaabbbbaaaaaaaaaaab...........",
".........#aaaaaaaaaaaaaaaaaaaaaaaaaab#..........",
"........#aaaaaaaaaaaaaaaaaaaaaaaaaaaab#.........",
"........#aaaaaaaaaabbbbbbbaaaaaaaaaaab#.........",
"........#aaaaaaaaaaaabbbbbaaaaaaaaaaab##........",
"........#aaaaaaaaaaaabbbbbaaaaaaaaaaab##........",
"........#aaaaaaaaaaaabbbbbaaaaaaaaaaab##........",
"........#aaaaaaaaaaaabbbbbaaaaaaaaaaab##........",
"........#aaaaaaaaaaaabbbbbaaaaaaaaaaab##........",
".........#aaaaaaaaaaabbbbbaaaaaaaaaab###........",
".........#aaaaaaaaaaabbbbbaaaaaaaaaab###........",
"..........#aaaaaaaaaabbbbbaaaaaaaaab###.........",
"...........baaaaaaabbbbbbbbbaaaaaab####.........",
"............baaaaaaaaaaaaaaaaaaaab####..........",
".............baaaaaaaaaaaaaaaaaab####...........",
"..............bbaaaaaaaaaaaaaabb####............",
"...............#bbbaaaaaaaabbb#####.............",
"................###bbbaaaab#######..............",
"..................####baaab#####................",
".....................#baaab##...................",
".......................baab##...................",
"........................bab##...................",
".........................bb##...................",
"..........................###...................",
"...........................##...................",
"................................................",
"................................................",
"................................................",
"................................................",
"................................................",
"................................................"};


DmF::DmF( QWidget* parent, QString fon, preV *Prefs, bool miss )
    : QDialog( parent, "mfont", true, 0 )
{
    QPixmap image0( ( const char** ) image100_data );
		if (miss)
    	setCaption( tr( "Missing Font" ) );
		else
    	setCaption( tr( "Select Font" ) );
 		setIcon(loadIcon("AppIcon.xpm"));
    MissingFontLayout = new QHBoxLayout( this );
    MissingFontLayout->setSpacing( 0 );
    MissingFontLayout->setMargin( 10 );
    Layout1 = new QGridLayout;
    Layout1->setSpacing( 12 );
    Layout1->setMargin( 0 );
    TextLabel1 = new QLabel( this, "TextLabel1" );
		if (miss)
    	TextLabel1->setText(tr("The Font %1 is not installed.").arg(fon));
		else
    	TextLabel1->setText(tr("Select new Font:"));
    Layout1->addMultiCellWidget( TextLabel1, 0, 0, 1, 2 );
		if (miss)
			{
    	PixmapLabel1 = new QLabel( this, "PixmapLabel1" );
    	PixmapLabel1->setMinimumSize( QSize( 50, 50 ) );
    	PixmapLabel1->setMaximumSize( QSize( 50, 50 ) );
    	PixmapLabel1->setPixmap( image0 );
    	PixmapLabel1->setScaledContents( true );
    	Layout1->addWidget( PixmapLabel1, 1, 0 );
    	TextLabel4 = new QLabel( this, "TextLabel4" );
    	TextLabel4->setText( tr( "Use" ) );
    	Layout1->addWidget( TextLabel4, 1, 1 );
			}
    Replace = new FontCombo(this, Prefs);
		Ersatz = Replace->text(0);
		if (miss)
			{
    	Layout1->addWidget( Replace, 1, 2 );
    	TextLabel6 = new QLabel( this, "TextLabel6" );
    	TextLabel6->setText( tr( "instead" ) );
    	Layout1->addWidget( TextLabel6, 1, 3 );
			}
		else
			{
			Replace->setCurrentText(fon);
    	Layout1->addMultiCellWidget( Replace, 1, 1, 1, 2 );
    	PushButton2 = new QPushButton( this, "PushButton1" );
    	PushButton2->setText( tr( "Cancel" ) );
    	Layout1->addWidget( PushButton2, 2, 1 );
			}
    PushButton1 = new QPushButton( this, "PushButton1" );
    PushButton1->setText( tr( "OK" ) );
    Layout1->addWidget( PushButton1, 2, 2 );
    MissingFontLayout->addLayout( Layout1 );

    // signals and slots connections
    connect( PushButton1, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( Replace, SIGNAL( activated(const QString&) ), this, SLOT( NeuerFont(const QString&) ) );
		if (!miss)
    	connect( PushButton2, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

void DmF::NeuerFont(const QString& e)
{
    Ersatz = e;
}

