/****************************************************************************
** Form implementation generated from reading ui file 'Help.ui'
**
** Created: Tue Jul 3 21:51:52 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "helpbrowser.h"
#include "helpbrowser.moc"
#include <qpushbutton.h>
#include <qtextbrowser.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qtextcodec.h>
#include <qfileinfo.h>
#include <config.h>
extern QPixmap loadIcon(QString nam);

static const char* const image0_data[] = { 
"16 16 85 2",
"Qt c None",
".c c #000000",
".a c #181418",
".K c #201c20",
".z c #202020",
".p c #292429",
".b c #292829",
".k c #312829",
".2 c #313031",
"#q c #313431",
".T c #393031",
"#r c #393431",
"#s c #393439",
".# c #393839",
"#p c #393c39",
".U c #410808",
".3 c #413c41",
".l c #414441",
".G c #4a484a",
"#n c #521c08",
".F c #524c52",
".y c #525052",
".g c #525552",
".o c #5a595a",
".n c #625d62",
".d c #626162",
".E c #626562",
".7 c #6a2400",
".0 c #6a2808",
"#k c #6a2c00",
".u c #6a656a",
".r c #6a696a",
".H c #6a6d6a",
"#h c #732400",
".P c #732808",
".q c #737173",
"#e c #7b4410",
"#a c #834818",
".j c #838183",
".8 c #8b5929",
".m c #8b898b",
".X c #946941",
".O c #946d41",
".S c #948d94",
".4 c #9c6d4a",
".i c #9c959c",
".f c #9c999c",
".L c #9c9d9c",
"#m c #a44000",
".t c #a4a1a4",
".V c #a4a5a4",
"#g c #ac4800",
".s c #acaaac",
"#l c #b44400",
"#d c #b4aeb4",
".h c #b4b2b4",
".J c #b4b6b4",
"#. c #bd5500",
".x c #bdb6bd",
".I c #bdbabd",
".v c #bdbebd",
"#c c #c55900",
".e c #c5c2c5",
".Q c #d5beb4",
".N c #d5bebd",
".6 c #de6500",
"#f c #de7931",
"#b c #de955a",
"#o c #ded6d5",
"#i c #ded6de",
"#j c #dedade",
".R c #dedede",
".9 c #e6a57b",
".Z c #e6baa4",
".5 c #e6c6b4",
".W c #e6dee6",
".M c #e6e2e6",
".1 c #e6e6e6",
"## c #eee2e6",
".D c #eee6ee",
".w c #eeeaee",
".Y c #f6e2d5",
".A c #f6f2f6",
".B c #fffaff",
".C c #ffffff",
"QtQtQtQtQtQtQt.#.aQtQtQtQtQtQtQt",
"QtQtQt.b.cQt.d.e.f.#QtQtQtQtQtQt",
"QtQtQt.b.c.g.e.h.i.j.bQtQtQtQtQt",
"QtQtQt.k.l.e.m.n.o.d.j.pQtQtQtQt",
"QtQtQt.g.e.q.r.s.t.u.g.j.pQtQtQt",
"QtQt.g.e.q.r.v.w.w.x.r.y.j.zQtQt",
"Qt.d.e.q.u.v.A.B.C.D.x.E.F.j.pQt",
".G.e.H.u.I.A.B.B.B.B.A.J.d.G.j.K",
"Qt.b.b.L.M.B.N.O.P.Q.C.R.S.T.UQt",
"QtQt.b.V.W.B.X.Y.Z.0.B.1.i.2QtQt",
"QtQt.3.s.1.B.4.5.6.7.C.M.s.bQtQt",
"QtQt.#.s.M.B.8.9#..7.B##.s.bQtQt",
"QtQt.3.s.D.B#a#b#c.7.C.M#d.bQtQt",
"QtQt.#.s.M.B#e#f#g#h.B.M.s.bQtQt",
"QtQt.3.t#i#j#k#l#m#n#j#o.t.bQtQt",
"QtQtQt#p#q#r.b.b.b.b.2#s.b.bQtQt"};

static const char* const image1_data[] = { 
"16 16 95 2",
"Qt c None",
"#w c None",
".n c None",
"#q c None",
".k c None",
".l c None",
".p c None",
".r c None",
".j c None",
".g c None",
".w c None",
"#b c None",
"#e c None",
".s c None",
"#v c None",
".A c None",
".d c None",
"#B c None",
".h c None",
".# c None",
".B c None",
".U c None",
"#x c None",
"#r c None",
"#A c None",
"#C c None",
".q c None",
"#c c None",
".b c None",
".i c None",
".F c None",
".a c None",
"#d c None",
".m c None",
".c c None",
".f c None",
".e c None",
".v c None",
"#u c None",
".G c None",
".z c #000000",
"#z c #081020",
".T c #081829",
"#y c #081831",
"#a c #081c29",
".2 c #081c31",
"#f c #082031",
"#n c #082439",
"#p c #082839",
".Y c #0895bd",
".3 c #101420",
"#o c #102841",
".K c #104462",
".t c #104c73",
".o c #105073",
"#s c #105573",
".L c #10557b",
"#l c #10597b",
"#m c #106183",
"#k c #1081a4",
".0 c #1095bd",
".Z c #1099bd",
".X c #1099c5",
".7 c #10a1cd",
".H c #18486a",
".C c #184c6a",
".x c #184c73",
".M c #185573",
".4 c #18597b",
"#j c #1891b4",
".5 c #1891bd",
".W c #18b2cd",
".O c #18b6d5",
".u c #203852",
"#h c #2095b4",
"#t c #20a5cd",
"#i c #2999b4",
".8 c #29bede",
"## c #29cad5",
".6 c #39dae6",
".9 c #41d6e6",
"#. c #4adae6",
"#g c #4adeee",
".1 c #5aaec5",
".J c #5ac6de",
".N c #7bd6e6",
".y c #83a1bd",
".D c #83a1c5",
".S c #83c6e6",
".P c #a4e2f6",
".Q c #cdeeff",
".R c #d5eeff",
".V c #deeeff",
".E c #e6f2ff",
".I c #ffffff",
"Qt.#Qt.#.a.b.c.d.e.f.g.d.e.f.g.d",
".h.g.i.j.k.l.m.n.oQt.p.n.p.gQt.j",
"Qt.#Qt.#.q.r.s.t.u.rQt.rQt.rQt.r",
".v.j.w.gQt.j.x.y.z.jQt.gQt.j.A.g",
".g.r.B.rQt.C.D.E.zQt.g.r.g.r.F.r",
"QtQt.G.B.H.y.I.J.K.L.o.L.o.LQtQt",
"Qt.rQt.M.D.I.N.O.P.Q.I.R.S.T.U.#",
".GQt.t.y.V.J.W.X.Y.Z.0.Z.1.2Qt.g",
".U.#.3.4.5.6.7.X.8.9#.##.8#a#b#c",
"#dQt#e#f.L.5#g.7.Z#h#i#j#k.z.p.j",
".g.r.g.r.T#l.5.6#m#n#o#p.z.z#qQt",
".p.j.p.g#r.T#s#t.zQt.pQt.p#u.pQt",
"#v.rQt.r#w#x#y#s.z.r.g.r.g.c.g.r",
".p.g.A#c#dQt#e#z.z.g.p.j#A.g.p.j",
".g.r#B.r.g.r.g.r.z.r.g.r.g.r.g.r",
".p.j.p#C.p.j.p.g#e.j.p.g.p.jQtQt"};

static const char* const image2_data[] = { 
"16 16 87 2",
".b c None",
".z c None",
"#u c None",
".i c None",
".j c None",
".o c None",
".g c None",
".e c None",
".B c None",
".w c None",
".E c None",
".u c None",
".d c None",
".c c None",
".h c None",
".D c None",
".q c None",
".f c None",
".p c None",
".v c None",
".x c None",
".m c None",
".a c None",
".# c None",
".y c None",
".t c None",
".C c None",
".n c None",
".G c None",
".F c None",
".k c None",
"Qt c None",
"#t c #000000",
"## c #000408",
"#p c #000810",
"#i c #000c18",
"#n c #080c10",
"#r c #081018",
"#k c #081420",
"#l c #081820",
"#s c #082841",
".Y c #088db4",
".W c #0895c5",
"#j c #103452",
".l c #103852",
".s c #10385a",
"#q c #10405a",
"#o c #104462",
".K c #104c6a",
".r c #104c73",
"#m c #10597b",
"#h c #10658b",
".1 c #10698b",
"#. c #106994",
".X c #1095bd",
"#f c #1099bd",
".Z c #1099c5",
".U c #109dc5",
".7 c #10a1c5",
".0 c #10a5cd",
".V c #10aecd",
"#g c #10c2de",
"#a c #18405a",
".O c #185073",
"#e c #1895b4",
".9 c #18a1c5",
".6 c #18bad5",
".H c #204862",
".L c #294c6a",
"#c c #2991ac",
"#d c #2995b4",
".A c #317d9c",
".J c #3181a4",
".4 c #41d2de",
".5 c #4adeee",
".8 c #4ae2ee",
".T c #5ac2de",
"#b c #6aa5c5",
".3 c #94ceee",
".P c #a4cee6",
".M c #acdeee",
".S c #c5e2f6",
".2 c #d5e6f6",
".I c #e6f2ff",
".Q c #e6f6ff",
".R c #f6f6ff",
".N c #ffffff",
"Qt.#Qt.a.b.c.d.e.f.c.d.e.b.c.g.e",
".b.h.i.h.j.h.k.l.m.h.n.h.o.h.b.h",
".f.e.p.c.#.e.q.r.s.e.t.c.u.e.v.c",
".w.h.x.h.y.h.z.r.A.l.B.h.C.h.e.h",
".D.c.E.e.F.#.G.H.I.J.s.e.D.c.D.e",
".e.h.K.r.r.r.H.L.M.N.A.l.e.h.e.h",
".D.e.O.P.Q.R.Q.S.T.U.N.J.s.e.D.c",
".e.h.r.R.T.V.W.X.Y.Z.0.I.1.l.e.h",
".D.c.L.2.3.4.5.6.Z.7.8.9#.##.D.e",
".e.h#a#b#c#d#e#f#g.8.9#h#i.h.e.h",
".D.e#j#k#k#k#l#m.8.9#.#n.D.e.D.c",
".e.h.e.h.e.h.e#o.9#h#p.h.e.h.e.h",
".D.c.D.e.D.c.D#q#.#r.D.e.D.c.D.e",
".e.h.e.h.e.h.e#s##.h.e.h.e.h.e.h",
".D.e.D.c.D.e.D#t.D.e.D.c.D.e.D.c",
".e.h#u.h.B.#.B.#.e.h.e.h.e.h.e.h"};


/* 
 *  Constructs a HelpBrowser which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
HelpBrowser::HelpBrowser( QWidget* parent, QString Capt, QString Datei )
    : QWidget( parent, "Help", WType_TopLevel | WDestructiveClose )
{
    QPixmap image0( ( const char** ) image0_data );
    QPixmap image1( ( const char** ) image1_data );
    QPixmap image2( ( const char** ) image2_data );
    resize( 547, 450 ); 
    setCaption( Capt );
  	setIcon(loadIcon("AppIcon.xpm"));
    HelpBrowserLayout = new QVBoxLayout( this ); 
    HelpBrowserLayout->setSpacing( 2 );
    HelpBrowserLayout->setMargin( 2 );

    Layout13 = new QHBoxLayout; 
    Layout13->setSpacing( 6 );
    Layout13->setMargin( 0 );

    HomeB = new QToolButton( this, "HomeB" );
    HomeB->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, HomeB->sizePolicy().hasHeightForWidth() ) );
    HomeB->setMinimumSize( QSize( 0, 0 ) );
    HomeB->setMaximumSize( QSize( 32767, 32767 ) );
    HomeB->setText( tr( "" ) );
    HomeB->setPixmap( image0 );
    Layout13->addWidget( HomeB );

    BackB = new QToolButton( this, "BackB" );
    BackB->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, BackB->sizePolicy().hasHeightForWidth() ) );
    BackB->setMinimumSize( QSize( 0, 0 ) );
    BackB->setMaximumSize( QSize( 32767, 32767 ) );
    BackB->setPixmap( image1 );
    Layout13->addWidget( BackB );

    ForwB = new QToolButton( this, "ForwB" );
    ForwB->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, ForwB->sizePolicy().hasHeightForWidth() ) );
    ForwB->setMinimumSize( QSize( 0, 0 ) );
    ForwB->setMaximumSize( QSize( 32767, 32767 ) );
    ForwB->setPixmap( image2 );
    Layout13->addWidget( ForwB );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout13->addItem( spacer );
    HelpBrowserLayout->addLayout( Layout13 );

    Anzeige = new QTextBrowser( this, "Anzeige" );
    Anzeige->setFrameShape( QTextBrowser::StyledPanel );
  	QString pfad = PREL;
  	QString pfad2;
  	bool Avail = true;
		if (Datei == "")
  		pfad2 = pfad + "/lib/scribus/doc/"+QString(QTextCodec::locale()).left(2)+"/index.html";
		else
			pfad2 = Datei;
  	QFileInfo fi;
  	fi = QFileInfo(pfad2);
  	if (!fi.exists())
  		{
  		pfad2 = pfad + "/lib/scribus/doc/en/index.html";
  		fi = QFileInfo(pfad2);
  		if (!fi.exists())
  			{
  			Anzeige->setText("<h2>"+tr("Sorry, no Manual available!")+"</h2>");
  			Avail = false;
  			}
  		}
  	if (Avail)
    	Anzeige->setSource(pfad2);
    HelpBrowserLayout->addWidget( Anzeige );

    // signals and slots connections
    connect( HomeB, SIGNAL( clicked() ), Anzeige, SLOT( home() ) );
    connect( ForwB, SIGNAL( clicked() ), Anzeige, SLOT( forward() ) );
    connect( BackB, SIGNAL( clicked() ), Anzeige, SLOT( backward() ) );
}

