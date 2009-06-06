/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "missing.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QImage>
#include <QPixmap>

#include "fontcombo.h"
#include "scribusdoc.h"
#include "page.h"
#include "prefsmanager.h"
#include "commonstrings.h"
#include "util_icon.h"
#include "util.h"

static const char* const image100_data[] =
    {
        "32 32 243 2",
        ".# c None",
        "Qt c None",
        ".d c #000000",
        ".c c #000000",
        ".8 c #000000",
        ".t c #000000",
        "#p c #000000",
        ".J c #000000",
        "#K c #000000",
        "aR c #000000",
        ".i c #000000",
        "#7 c #000000",
        "#g c #000000",
        "bH c #000000",
        ".A c #000000",
        "#y c #000000",
        ".Q c #000000",
        "#z c #d54c00",
        "aH c #de5000",
        "#V c #000000",
        "a1 c #000000",
        ".7 c #000000",
        ".s c #000000",
        "bW c #000000",
        "bn c #000000",
        "#o c #000000",
        ".I c #000000",
        "bx c #000000",
        "bV c #000000",
        ".a c #cd4c00",
        ".b c #cd4800",
        "bG c #000000",
        "bO c #000000",
        "#J c #521c00",
        "aQ c #4a1c00",
        "bU c #000000",
        ".m c #7b2c00",
        ".W c #7b2c00",
        "#6 c #6a2800",
        "a2 c #d54c00",
        ".9 c #d55000",
        "bT c #5a2000",
        ".u c #d54c00",
        "bb c #9c3800",
        "#f c #943800",
        "#q c #d54c00",
        "bo c #d55000",
        "bP c #943400",
        "bQ c #833000",
        "bS c #833000",
        "bR c #8b3000",
        ".z c #9c3800",
        "ao c #a43c00",
        "bu c #ac3c00",
        "#x c #ac4000",
        ".e c #d54c00",
        ".h c #cd4c00",
        "#L c #d55000",
        "aG c #b44000",
        ".P c #bd4400",
        "bI c #cd4800",
        ".X c #d54c00",
        "#U c #bd4400",
        "bN c #b44400",
        ".6 c #bd4400",
        "a0 c #c54800",
        "#8 c #d54c00",
        "bc c #d55000",
        ".n c #de5000",
        ".r c #cd4800",
        "ag c #cd4c00",
        "bm c #c54800",
        "#h c #d54c00",
        "#n c #cd4c00",
        ".B c #d54c00",
        "ap c #d55000",
        "ay c #cd4c00",
        ".H c #d54c00",
        "#A c #d55000",
        "aI c #d54c00",
        ".R c #d55000",
        ".V c #d54c00",
        "aP c #d55000",
        "br c #181818",
        "bh c #181c18",
        "bg c #202020",
        "a7 c #292c29",
        "a6 c #313031",
        "bf c #313429",
        "aV c #393839",
        "a5 c #393c31",
        "aU c #414439",
        "bs c #737118",
        "ak c #737173",
        "ab c #7b797b",
        "aa c #837d83",
        "#Z c #838183",
        "#0 c #8b858b",
        "at c #8b896a",
        "aj c #8b897b",
        "#Q c #8b898b",
        "#F c #8b8d8b",
        "#P c #948d94",
        "#E c #949194",
        ".2 c #949594",
        "#b c #9c959c",
        "aW c #9c9941",
        ".3 c #9c999c",
        "#R c #a4a19c",
        "bq c #acaa18",
        "as c #acaa7b",
        "aT c #bdb652",
        "#1 c #bdba9c",
        "#O c #bdbaa4",
        ".4 c #c5c6bd",
        ".1 c #cdcabd",
        ".g c #d54c00",
        ".j c #d55000",
        "bM c #d55008",
        ".Y c #d55500",
        "bK c #d55508",
        "af c #d55900",
        "#Y c #d5d2ac",
        ".f c #de5000",
        "bJ c #de5008",
        ".o c #de5500",
        "bL c #de5508",
        ".q c #de5900",
        "#m c #de5d00",
        "#i c #de6100",
        ".G c #de6500",
        "aq c #de6d00",
        "#B c #de7100",
        "bw c #de7900",
        "#c c #dedacd",
        "#u c #dedebd",
        ".C c #e66900",
        "ax c #e66d00",
        "#I c #e67100",
        ".k c #e67900",
        ".S c #e67d00",
        "by c #e68500",
        "aO c #e68900",
        "#5 c #e68d00",
        "ac c #e6dea4",
        "bi c #e6e629",
        "a8 c #e6e639",
        "#k c #e6e6cd",
        ".l c #ee7d00",
        "#W c #ee8d00",
        "#e c #ee9100",
        ".y c #ee9900",
        "a3 c #eea100",
        "ah c #eeae00",
        "aB c #eeea7b",
        "#. c #f69108",
        ".v c #f69d00",
        "ba c #f6a500",
        ".K c #f6aa00",
        "#r c #f6ae00",
        "aF c #f6c600",
        "bp c #f6ca00",
        "bF c #f6d600",
        "bE c #f6d608",
        "bC c #f6d610",
        "a# c #f6eeac",
        "#t c #f6eecd",
        "#a c #f6f2de",
        "aA c #f6f67b",
        ".O c #ffae00",
        "#w c #ffae08",
        ".5 c #ffbe08",
        ".p c #ffc200",
        ".Z c #ffc210",
        ".w c #ffc600",
        ".x c #ffca00",
        "#T c #ffce08",
        "#M c #ffce10",
        "bz c #ffd600",
        "bA c #ffd608",
        "bD c #ffd610",
        ".D c #ffd631",
        "bB c #ffda10",
        "#j c #ffda20",
        "#l c #ffda29",
        "aS c #ffde00",
        "#9 c #ffde10",
        ".U c #ffde5a",
        "#H c #ffe231",
        ".T c #ffe25a",
        "#C c #ffe641",
        "aw c #ffea10",
        "## c #ffea7b",
        "bl c #ffee00",
        "bd c #ffee08",
        "ar c #ffee18",
        "#4 c #ffee41",
        "#X c #ffee52",
        "#d c #ffee83",
        "aJ c #fff220",
        "an c #fff241",
        "#v c #fff28b",
        ".N c #fff2ac",
        "aN c #fff618",
        "ai c #fff65a",
        "#s c #fff69c",
        ".L c #fff6ac",
        "b# c #fffa08",
        "a4 c #fffa18",
        "aZ c #fffa29",
        "aE c #fffa39",
        "az c #fffa52",
        "#N c #fffa9c",
        ".0 c #fffac5",
        "#G c #fffacd",
        "bv c #ffff00",
        "bt c #ffff08",
        "bk c #ffff10",
        "bj c #ffff18",
        "be c #ffff20",
        "b. c #ffff29",
        "a9 c #ffff31",
        "aY c #ffff39",
        "aX c #ffff41",
        "aM c #ffff4a",
        "aL c #ffff52",
        "aK c #ffff5a",
        "aD c #ffff62",
        "aC c #ffff6a",
        "av c #ffff73",
        "au c #ffff7b",
        "ae c #ffff83",
        "am c #ffff8b",
        "al c #ffff94",
        "a. c #ffff9c",
        "ad c #ffffa4",
        "#3 c #ffffac",
        "#2 c #ffffb4",
        "#S c #ffffbd",
        "#D c #ffffc5",
        ".F c #ffffcd",
        ".E c #ffffd5",
        ".M c #ffffe6",
        "QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt",
        ".#.#.#.#.#.#.#.#.#.#.#.#.#.#.a.b.c.d.#.#.#.#.#.#.#.#.#.#.#.#.#.#",
        ".#.#.#.#.#.#.#.#.#.#.#.#.#.e.f.g.h.i.c.#.#.#.#.#.#.#.#.#.#.#.#.#",
        ".#.#.#.#.#.#.#.#.#.#.#.#.a.j.k.l.g.m.i.d.#.#.#.#.#.#.#.#.#.#.#.#",
        ".#.#.#.#.#.#.#.#.#.#.#.#.n.o.p.p.q.r.s.t.#.#.#.#.#.#.#.#.#.#.#.#",
        ".#.#.#.#.#.#.#.#.#.#.#.u.g.v.w.x.y.j.z.A.d.#.#.#.#.#.#.#.#.#.#.#",
        ".#.#.#.#.#.#.#.#.#.#.#.B.C.D.E.F.D.G.H.I.J.#.#.#.#.#.#.#.#.#.#.#",
        ".#.#.#.#.#.#.#.#.#.#.e.j.K.L.M.M.N.O.g.P.Q.c.#.#.#.#.#.#.#.#.#.#",
        ".#.#.#.#.#.#.#.#.#.a.R.S.T.M.M.M.M.U.l.V.W.i.d.#.#.#.#.#.#.#.#.#",
        ".#.#.#.#.#.#.#.#.#.X.Y.Z.0.1.2.3.4.0.5.q.6.7.8.#.#.#.#.#.#.#.#.#",
        ".#.#.#.#.#.#.#.#.9.g#.###a#b.3.3#b#c#d#e.f#f#g.d.#.#.#.#.#.#.#.#",
        ".#.#.#.#.#.#.#.##h#i#j.E#k.3.2.3.2.1.F#l#m#n#o#p.#.#.#.#.#.#.#.#",
        ".#.#.#.#.#.#.##q.f#r#s.E#t#b.3.2#b#u.E#v#w.g#x#y.c.#.#.#.#.#.#.#",
        ".#.#.#.#.#.##z#A#B#C#D.F.F.3#E#E#F#G#D#D#H#I.V#J#K.#.#.#.#.#.#.#",
        ".#.#.#.#.#.##L.g#M#N.F#D.F#O#P#Q#R#D#D#S#N#T.f#U#V.8.#.#.#.#.#.#",
        ".#.#.#.#.#.a.g#W#X#S#S#S#S#Y#Z#0#1#S#2#2#3#4#5.j#6#7.d.#.#.#.#.#",
        ".#.#.#.#.##8.q#9a.#2#2#2#2a#aaabac#3#3adadae#9afag.s.t.#.#.#.#.#",
        ".#.#.#.#.u.jahaiadadadadadadajaka.ada.alamalan#r.gao.A.d.#.#.#.#",
        ".#.#.#.#apaqarama.ala.ala.alasatalamamaeaeauavawaxay.I.J.#.#.#.#",
        ".#.#.#.e.g.wazaeaeaeaeaeaeaeaAaBauauavavaCaCaDaEaF.jaG.Q.c.#.#.#",
        ".#.#aHaI#WaJaCaCavavavavavaCavaCaCaDaDaKaKaLaLaMaNaOaPaQaR.#.#.#",
        ".#.#.X.qaSaMaLaKaKaDaKaKaKaTaUaVaWaLaMaMaXaXaYaYaZaSafa0a1.8.#.#",
        ".#a2.fa3a4aXaMaMaMaMaMaMaMa5a6a7a7a8aXaYaYa9a9b.b.b#ba.gbb#g.d.#",
        ".#bc#ibdbea9a9a9a9a9a9a9a9bfbgbgbhbibebebebebjbjbkbkbl.Gbmbn#p.#",
        "bo.gbpbkbebjbebebebebebebebqbhbrbsbjbjbkbjbkbkbtbkbtbtaF.fbu#y.c",
        ".B.Sbvbkbkbjbkbjbjbjbkbjbkbjbkbkbkbkbtbkbtbtbtbtbvbtbvbvbw.Hbx.J",
        ".fbybzbAbBbCbBbDbBbCbBbDbBbCbBbDbBbEbBbEbAbEbAbFbAbFbAbFaO.gbGbH",
        "bI.j.gbJ.jbKbKbLbKbLbKbLbKbKbKbL.jbM.jbJ.jbM.j.j.g.j.g.j.gbNbO#y",
        "aR.mbPbQbRbSbRbSbRbSbRbSbRbSbRbSbRbSbRbSbRbSbRbSbRbSbRbSbTbUbVaR",
        ".c.i#VbWbnbnbnbnbnbnbnbnbnbnbnbnbnbnbnbnbnbnbnbnbnbnbnbnbW#V.i.c",
        ".#.d.c.t.t.t.t.t.t.t.t.t.t.t.t.t.t.t.t.t.t.t.t.t.t.t.t.t.t.c.d.#",
        "QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt"
    };


MissingFont::MissingFont( QWidget* parent, QString fon, ScribusDoc* doc ) : QDialog( parent )
{
	QPixmap image0( ( const char** ) image100_data );
	setModal(true);
	setWindowTitle( tr("Missing Font"));
	setWindowIcon(QIcon(loadIcon("AppIcon.png")));
	missingFontLayout = new QHBoxLayout( this );
	missingFontLayout->setMargin(10);
	missingFontLayout->setSpacing(0);
	missingFontGridLayout = new QGridLayout;
	missingFontGridLayout->setSpacing( 10 );
	missingFontGridLayout->setMargin( 0 );
	notInstalledLabel = new QLabel( tr("The Font %1 is not installed.").arg(fon), this );
	missingFontGridLayout->addWidget( notInstalledLabel, 0, 0, 1, 4 );
	pixmapLabel = new QLabel( this );
	pixmapLabel->setPixmap( image0 );
	missingFontGridLayout->addWidget( pixmapLabel, 1, 0 );
	useLabel = new QLabel( tr( "Use" ), this );
	missingFontGridLayout->addWidget( useLabel, 1, 1 );
	replaceFontCombo = new FontCombo(this);
	if (doc != 0)
	{
		setCurrentComboItem(replaceFontCombo, doc->toolSettings.defFont);
		replacementFont = doc->toolSettings.defFont;
	}
	else
	{
		setCurrentComboItem(replaceFontCombo, PrefsManager::instance()->appPrefs.toolSettings.defFont);
		replacementFont = PrefsManager::instance()->appPrefs.toolSettings.defFont;
	}
	missingFontGridLayout->addWidget( replaceFontCombo, 1, 2 );
	insteadLabel = new QLabel( tr( "instead" ), this );
	missingFontGridLayout->addWidget( insteadLabel, 1, 3 );
	okButton = new QPushButton( CommonStrings::tr_OK, this );
	missingFontGridLayout->addWidget( okButton, 2, 2 );
	missingFontLayout->addLayout( missingFontGridLayout );

	// signals and slots connections
	connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( replaceFontCombo, SIGNAL( activated(const QString&) ), this, SLOT( newFont(const QString&) ) );
}

void MissingFont::newFont(const QString& replacement)
{
	replacementFont = replacement;
}

const QString MissingFont::getReplacementFont()
{
	return replacementFont;
}

