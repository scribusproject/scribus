/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmykfw.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QSlider>
#include <QMenu>
#include <QPainter>
#include <QCursor>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QDomDocument>
#include <QToolTip>
#include <QTextStream>
#include <QSpacerItem>
#include <QByteArray>
#include <QPixmap>
#include <QAction>
#include <cstdlib>

#include "colorchart.h"
#include "colorlistbox.h"
#include "commonstrings.h"
#include "sccolorengine.h"
#include "sccombobox.h"
#include "scconfig.h"
#include "scpaths.h"
#include "scribusdoc.h"
#include "scrspinbox.h"
#include "util.h"
#include "util_color.h"
#include "util_formats.h"
#include "util_icon.h"


CMYKChoose::CMYKChoose( QWidget* parent, ScribusDoc* doc, ScColor orig, QString name, ColorList *Colors, QStringList Cust, bool newCol  )
		: QDialog( parent ), CurrSwatch(doc)
{
	setModal(true);
	m_doc = doc;
	isNew = newCol;
	if (orig.getColorModel () == colorModelCMYK)
		CMYKmode = true;
	else
		CMYKmode = false;
	dynamic = true;
	Wsave = false;
	EColors = Colors;
	CurrSwatch.clear();
	alertIcon = loadIcon("alert.png");
	imageA = QPixmap(50,50);
	imageA.fill( ScColorEngine::getDisplayColor(orig, m_doc) );
	if ( ScColorEngine::isOutOfGamut(orig, m_doc) )
		paintAlert(alertIcon,imageA, 2, 2, false);
	imageN = QPixmap(50,50);
	imageN.fill( ScColorEngine::getDisplayColor(orig, m_doc) );
	if ( ScColorEngine::isOutOfGamut(orig, m_doc) )
		paintAlert(alertIcon, imageN, 2, 2, false);
	Farbe = orig;
	QPixmap image0 = SliderPix(180);
	QPixmap image1 = SliderPix(300);
	QPixmap image2 = SliderPix(60);
	QPixmap image3 = SliderBlack();
	CMYKColor cmyk;
	double ccd, cmd, cyd, ckd;
	ScColorEngine::getCMYKValues(orig, m_doc, cmyk);
	ccd = cmyk.c / 2.55;
	cmd = cmyk.m / 2.55;
	cyd = cmyk.y / 2.55;
	ckd = cmyk.k / 2.55;
	RGBColor rgb;
	double crd, cgd, cbd;
	ScColorEngine::getRGBValues(orig, m_doc, rgb);
	crd = rgb.r / 2.55;
	cgd = rgb.g / 2.55;
	cbd = rgb.b / 2.55;
	resize( 498, 306 );
	setWindowTitle( tr( "Edit Color" ) );
	setWindowIcon(QIcon(loadIcon("AppIcon.png")));
	CMYKFarbenLayout = new QHBoxLayout( this );
	CMYKFarbenLayout->setSpacing( 5 );
	CMYKFarbenLayout->setMargin( 10 );
	Layout23 = new QVBoxLayout;
	Layout23->setSpacing( 5 );
	Layout23->setMargin( 0 );

	TextLabel1 = new QLabel( tr( "&Name:" ), this );
	TextLabel1->setMinimumSize( QSize( 200, 22 ) );
	Layout23->addWidget( TextLabel1 );

	Farbname = new QLineEdit( this );
	Farbname->setMinimumSize( QSize( 200, 22 ) );
	Farbname->setText( name );
	TextLabel1->setBuddy( Farbname );
	Layout23->addWidget( Farbname );

	TextLabel3 = new QLabel( tr( "Color &Model" ), this );
	TextLabel3->setMinimumSize( QSize( 100, 22 ) );
	Layout23->addWidget( TextLabel3 );

	ComboBox1 = new ScComboBox( this );
	ComboBox1->addItem( tr( "CMYK" ) );
	ComboBox1->addItem( tr( "RGB" ) );
	ComboBox1->addItem( tr( "Web Safe RGB" ) );
	if (!CMYKmode)
		ComboBox1->setCurrentIndex( 1 );
	TextLabel3->setBuddy( ComboBox1 );
	Layout23->addWidget( ComboBox1 );

	Separations = new QCheckBox( this );
	Separations->setText( tr( "Is Spot Color" ) );
	Separations->setChecked(orig.isSpotColor());
	Layout23->addWidget( Separations );
/*
	Regist = new QCheckBox( this );
	Regist->setText( tr( "Is Registration Color" ) );
	Regist->setChecked(orig.isRegistrationColor());
	Layout23->addWidget( Regist );
*/
	QSpacerItem* spacer = new QSpacerItem( 1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding );
	Layout23->addItem( spacer );

	Layout2 = new QGridLayout();
	Layout2->setSpacing( 6 );
	Layout2->setMargin( 0 );

	TextLabel5_2 = new QLabel( tr( "New" ), this );
	TextLabel5_2->setMinimumSize( QSize( 50, 22 ) );
	TextLabel5_2->setMaximumSize( QSize( 100, 22 ) );

	Layout2->addWidget( TextLabel5_2, 0, 1 );

	OldC = new QLabel( "", this );
	OldC->setMinimumSize( QSize( 50, 50 ) );
	OldC->setMaximumSize( QSize( 50, 50 ) );
	OldC->setFrameShape( QLabel::WinPanel );
	OldC->setFrameShadow( QLabel::Sunken );
	OldC->setScaledContents( true );
	OldC->setAlignment(Qt::AlignCenter);
	OldC->setPixmap( imageA );

	Layout2->addWidget( OldC, 1, 0 );

	TextLabel5 = new QLabel( tr( "Old" ), this );
	TextLabel5->setMinimumSize( QSize( 50, 22 ) );
	TextLabel5->setMaximumSize( QSize( 100, 22 ) );

	Layout2->addWidget( TextLabel5, 0, 0 );

	NewC = new QLabel( "", this );
	NewC->setMinimumSize( QSize( 50, 50 ) );
	NewC->setMaximumSize( QSize( 50, 50 ) );
	NewC->setFrameShape( QLabel::WinPanel );
	NewC->setFrameShadow( QLabel::Sunken );
	NewC->setScaledContents( true );
	NewC->setAlignment(Qt::AlignCenter);
	NewC->setPixmap( imageN );

	Layout2->addWidget( NewC, 1, 1 );
	Layout23->addLayout( Layout2 );

	Layout21 = new QHBoxLayout;
	Layout21->setSpacing( 20 );
	Layout21->setMargin( 10 );

	Cancel_2 = new QPushButton( CommonStrings::tr_OK, this );
	Cancel_2->setDefault( true );
	Layout21->addWidget( Cancel_2 );
	Cancel = new QPushButton( CommonStrings::tr_Cancel, this );
	Layout21->addWidget( Cancel );
	Layout23->addLayout( Layout21 );
	CMYKFarbenLayout->addLayout( Layout23 );

	Frame4 = new QFrame( this );
	Frame4->setFrameShape( QFrame::NoFrame );
	Frame4->setFrameShadow( QFrame::Raised );
	Frame4Layout = new QVBoxLayout( Frame4 );
	Frame4Layout->setSpacing( 5 );
	Frame4Layout->setMargin( 0 );

	Swatches = new ScComboBox( Frame4 );
	Swatches->addItem( tr( "HSV-Colormap" ) );
/*
	Swatches->insertItem("X11 RGB-Set");
	Swatches->insertItem("X11 Grey-Set");
	Swatches->insertItem("Gnome-Set");
	Swatches->insertItem("SVG-Set");
	Swatches->insertItem("OpenOffice.org-Set");
*/
	csm.findPaletteLocations();
	csm.findPalettes();
	QStringList allSets(csm.paletteNames());
	for ( QStringList::Iterator it = allSets.begin(); it != allSets.end(); ++it )
	{
		Swatches->addItem((*it));
	}
	customSetStartIndex=Swatches->count();

	if (Cust.count() != 0)
	{
		QStringList realEx;
		realEx.clear();
		for (int m = 0; m < Cust.count(); ++m)
		{
			QString Cpfad = QDir::convertSeparators( ScPaths::getApplicationDataDir() +Cust[m]);
			QFileInfo cfi(Cpfad);
			if (cfi.exists())
			{
				Swatches->addItem(Cust[m]);
				realEx.append(Cust[m]);
			}
		}
		CColSet = realEx;
	}
	Frame4Layout->addWidget( Swatches );

	TabStack = new QStackedWidget( Frame4 );
	TabStack->setFrameShape( QFrame::NoFrame );

	Frame5a = new QFrame( TabStack );
	Frame5a->setFrameShape( QFrame::NoFrame );
	Frame5a->setFrameShadow( QFrame::Raised );
	Frame5aLayout = new QHBoxLayout( Frame5a );
	Frame5aLayout->setSpacing( 0 );
	Frame5aLayout->setMargin( 0 );
	Frame5 = new QFrame(Frame5a);
	Frame5->setFrameShape( QLabel::WinPanel );
	Frame5->setFrameShadow( QLabel::Sunken );
	Frame5->setMinimumSize( QSize( 182, 130 ) );
	Frame5->setMaximumSize( QSize( 182, 130 ) );
	Frame5Layout = new QHBoxLayout( Frame5 );
	Frame5Layout->setSpacing( 0 );
	Frame5Layout->setMargin( 0 );
	ColorMap = new ColorChart( Frame5, doc);
	ColorMap->setMinimumSize( QSize( 180, 128 ) );
	ColorMap->setMaximumSize( QSize( 180, 128 ) );
	Frame5Layout->addWidget( ColorMap );
	Frame5aLayout->addWidget( Frame5, 0, Qt::AlignCenter);
	TabStack->addWidget( Frame5a );

	ColorSwatch = new ColorListBox(TabStack);
	TabStack->addWidget( ColorSwatch );

	Frame4Layout->addWidget( TabStack );

	Layout2x = new QGridLayout();
	Layout2x->setSpacing( 5 );
	Layout2x->setMargin( 0 );

	CyanT = new QLabel( tr( "C:" ), Frame4 );
	Layout2x->addWidget(CyanT, 0, 0);

	Layout1_2 = new QVBoxLayout;
	Layout1_2->setSpacing( 0 );
	Layout1_2->setMargin( 0 );

	CyanP = new QLabel( Frame4 );
//	CyanP->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, CyanP->sizePolicy().hasHeightForWidth() ) );
	CyanP->setMinimumSize( QSize( 200, 10 ) );
	CyanP->setPixmap(image0);
	CyanP->setScaledContents( true );
	Layout1_2->addWidget( CyanP );

	CyanSL = new QSlider( Frame4 );
	CyanSL->setMinimumSize( QSize( 200, 16 ) );
	CyanSL->setMaximum( 100 );
	CyanSL->setOrientation( Qt::Horizontal );
	Layout1_2->addWidget( CyanSL );
	Layout2x->addLayout(Layout1_2, 0, 1);

	CyanSp = new ScrSpinBox( 0, 100, Frame4, 0 );
	CyanSp->setSuffix( tr(" %"));
	Layout2x->addWidget(CyanSp, 0, 2);
	CyanSp->setValue(ccd);
	CyanSL->setValue(qRound(ccd));

	MagentaT = new QLabel( tr( "M:" ), Frame4 );
	Layout2x->addWidget(MagentaT, 1, 0);

	Layout1_2_2 = new QVBoxLayout;
	Layout1_2_2->setSpacing( 0 );
	Layout1_2_2->setMargin( 0 );

	MagentaP = new QLabel( Frame4 );
//	MagentaP->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, MagentaP->sizePolicy().hasHeightForWidth() ) );
	MagentaP->setMinimumSize( QSize( 200, 10 ) );
	MagentaP->setPixmap(image1);
	MagentaP->setScaledContents( true );
	Layout1_2_2->addWidget( MagentaP );

	MagentaSL = new QSlider( Frame4 );
	MagentaSL->setMinimumSize( QSize( 200, 16 ) );
	MagentaSL->setMaximum( 100 );
	MagentaSL->setOrientation( Qt::Horizontal );
	Layout1_2_2->addWidget( MagentaSL );
	Layout2x->addLayout(Layout1_2_2, 1, 1);

	MagentaSp = new ScrSpinBox( 0, 100, Frame4, 0 );
	MagentaSp->setSuffix( tr(" %"));
	Layout2x->addWidget(MagentaSp, 1, 2);
	MagentaSp->setValue(cmd);
	MagentaSL->setValue(qRound(cmd));

	YellowT = new QLabel( tr( "Y:" ), Frame4 );
	Layout2x->addWidget(YellowT, 2, 0);

	Layout1_2_3 = new QVBoxLayout;
	Layout1_2_3->setSpacing( 0 );
	Layout1_2_3->setMargin( 0 );

	YellowP = new QLabel( Frame4);
//	YellowP->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, YellowP->sizePolicy().hasHeightForWidth() ) );
	YellowP->setMinimumSize( QSize( 200, 10 ) );
	YellowP->setPixmap(image2);
	YellowP->setScaledContents( true );
	Layout1_2_3->addWidget( YellowP );

	YellowSL = new QSlider( Frame4 );
	YellowSL->setMinimumSize( QSize( 200, 16 ) );
	YellowSL->setMaximum( 100 );
	YellowSL->setOrientation( Qt::Horizontal );
	Layout1_2_3->addWidget( YellowSL );
	Layout2x->addLayout(Layout1_2_3, 2, 1);

	YellowSp = new ScrSpinBox( 0, 100, Frame4, 0 );
	YellowSp->setSuffix( tr(" %"));
	Layout2x->addWidget(YellowSp, 2, 2);
	YellowSp->setValue(cyd);
	YellowSL->setValue(qRound(cyd));

	BlackT = new QLabel( tr( "K:" ), Frame4 );
	Layout2x->addWidget(BlackT, 3, 0);

	Layout1_2_4 = new QVBoxLayout;
	Layout1_2_4->setSpacing( 0 );
	Layout1_2_4->setMargin( 0 );

	BlackP = new QLabel( Frame4 );
//	BlackP->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, BlackP->sizePolicy().hasHeightForWidth() ) );
	BlackP->setMinimumSize( QSize( 200, 10 ) );
	BlackP->setPixmap(image3);
	BlackP->setScaledContents( true );
	Layout1_2_4->addWidget( BlackP );

	BlackSL = new QSlider( Frame4 );
	BlackSL->setMinimumSize( QSize( 200, 16 ) );
	BlackSL->setMaximum( 100 );
	BlackSL->setOrientation( Qt::Horizontal );
	Layout1_2_4->addWidget( BlackSL );
	Layout2x->addLayout(Layout1_2_4, 3, 1);

	BlackSp = new ScrSpinBox( 0, 100, Frame4, 0 );
	BlackSp->setSuffix( tr(" %"));
	Layout2x->addWidget(BlackSp, 3, 2);
	BlackSp->setValue(ckd);
	BlackSL->setValue(qRound(ckd));
	BlackComp = cmyk.k;
	Frame4Layout->addLayout( Layout2x );
	QSpacerItem* spacer2 = new QSpacerItem( 2, 2, QSizePolicy::Minimum, QSizePolicy::Expanding );
	Frame4Layout->addItem( spacer2 );
	CMYKFarbenLayout->addWidget( Frame4 );
	int h, s, v;
	ScColorEngine::getRGBColor(orig, m_doc).getHsv(&h, &s, &v);
	ColorMap->drawPalette(v);
	ColorMap->setMark(h, s);
	Fnam = name;
	Farbname->selectAll();
	Farbname->setFocus();
	TabStack->setCurrentIndex(0);
	setFixedSize(minimumSizeHint());
	setContextMenuPolicy(Qt::CustomContextMenu);
	// signals and slots connections
//	Regist->setToolTip( "<qt>" + tr( "Choosing this will enable printing this on all plates. Registration colors are used for printer marks such as crop marks, registration marks and the like. These are not typically used in the layout itself." ) + "</qt>");
	Separations->setToolTip( "<qt>" + tr( "Choosing this will make this color a spot color, thus creating another spot when creating plates or separations. This is used most often when a logo or other color needs exact representation or cannot be replicated with CMYK inks. Metallic and fluorescent inks are good examples which cannot be easily replicated with CMYK inks." ) + "</qt>");
	connect( Cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( Cancel_2, SIGNAL( clicked() ), this, SLOT( Verlassen() ) );
	connect( CyanSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	connect( MagentaSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	connect( YellowSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	connect( BlackSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	connect( CyanSL, SIGNAL( valueChanged(int) ), this, SLOT( SetValueS(int) ) );
	connect( MagentaSL, SIGNAL( valueChanged(int) ), this, SLOT( SetValueS(int) ) );
	connect( YellowSL, SIGNAL( valueChanged(int) ), this, SLOT( SetValueS(int) ) );
	connect( BlackSL, SIGNAL( valueChanged(int) ), this, SLOT( SetValueS(int) ) );
	connect( CyanSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
	connect( MagentaSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
	connect( YellowSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
	connect( BlackSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
	connect( ColorMap, SIGNAL( ColorVal(int, int, bool)), this, SLOT( setColor2(int, int, bool)));
	connect( ComboBox1, SIGNAL(activated(const QString&)), this, SLOT(SelModel(const QString&)));
	connect( Swatches, SIGNAL(activated(int)), this, SLOT(SelSwatch(int)));
	connect(ColorSwatch, SIGNAL( itemClicked(QListWidgetItem*) ), this, SLOT( SelFromSwatch(QListWidgetItem*) ) );
	connect(Separations, SIGNAL(clicked()), this, SLOT(setSpot()));
//	connect(Regist, SIGNAL(clicked()), this, SLOT(setRegist()));
	connect(this, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(slotRightClick()));
	layout()->activate();
	if (!CMYKmode)
		SelModel ( tr( "RGB" ));
	isRegistration = Farbe.isRegistrationColor();
	if (Farbe.isRegistrationColor())
	{
		ComboBox1->setEnabled(false);
		Separations->setEnabled(false);
	}
}

void CMYKChoose::setValSLiders(double value)
{
	int val = qRound(value);
	if (CyanSp == sender())
		CyanSL->setValue(val);
	if (MagentaSp == sender())
		MagentaSL->setValue(val);
	if (YellowSp == sender())
		YellowSL->setValue(val);
	if (BlackSp == sender())
		BlackSL->setValue(val);
}

void CMYKChoose::slotRightClick()
{
	QMenu *pmen = new QMenu();
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	QAction* dynAct;
	if (dynamic)
		dynAct = pmen->addAction( tr("Static Color Bars"));
	else
		dynAct = pmen->addAction( tr("Dynamic Color Bars"));
	connect(dynAct, SIGNAL(triggered()), this, SLOT(ToggleSL()));
	pmen->exec(QCursor::pos());
	delete pmen;
}

void CMYKChoose::SetValueS(int val)
{
	disconnect( CyanSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	disconnect( MagentaSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	disconnect( YellowSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	disconnect( BlackSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	if (CyanSL == sender())
		CyanSp->setValue(val);
	if (MagentaSL == sender())
		MagentaSp->setValue(val);
	if (YellowSL == sender())
		YellowSp->setValue(val);
	if (BlackSL == sender())
		BlackSp->setValue(val);
	setColor();
	connect( CyanSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	connect( MagentaSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	connect( YellowSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	connect( BlackSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
}

void CMYKChoose::ToggleSL()
{
	dynamic = !dynamic;
	CyanP->setPixmap(SliderPix(CMYKmode ? 180 : 0));
	MagentaP->setPixmap(SliderPix(CMYKmode? 300 : 120));
	YellowP->setPixmap(SliderPix(CMYKmode? 60 : 240));
	if (CMYKmode)
		BlackP->setPixmap(SliderBlack());
}

QPixmap CMYKChoose::SliderPix(int farbe)
{
	RGBColor rgb;
	CMYKColor cmyk;
	QPixmap image0 = QPixmap(255,10);
	QPainter p;
	p.begin(&image0);
	p.setPen(Qt::NoPen);
	int r, g, b, c, m, y, k;
	QColor tmp;
	for (int x = 0; x < 255; x += 5)
	{
		if (CMYKmode)
		{
			ScColorEngine::getCMYKValues(Farbe, m_doc, cmyk);
			cmyk.getValues(c, m, y, k);
			if (dynamic)
			{
				switch (farbe)
				{
				case 180:
					tmp = ScColorEngine::getDisplayColorGC(ScColor(x, m, y, k), m_doc);
					break;
				case 300:
					tmp = ScColorEngine::getDisplayColorGC(ScColor(c, x, y, k), m_doc);
					break;
				case 60:
					tmp = ScColorEngine::getDisplayColorGC(ScColor(c, m, x, k), m_doc);
					break;
				}
				p.setBrush(tmp);
			}
			else
			{
				switch (farbe)
				{
				case 180:
					tmp = ScColorEngine::getDisplayColorGC(ScColor(x, 0, 0, 0), m_doc);
					break;
				case 300:
					tmp = ScColorEngine::getDisplayColorGC(ScColor(0, x, 0, 0), m_doc);
					break;
				case 60:
					tmp = ScColorEngine::getDisplayColorGC(ScColor(0, 0, x, 0), m_doc);
					break;
				}
				p.setBrush(tmp);
			}
		}
		else
		{
			ScColorEngine::getRGBValues(Farbe, m_doc, rgb);
			rgb.getValues(r, g, b);
			if (dynamic)
			{
				switch (farbe)
				{
				case 0:
					tmp = ScColorEngine::getDisplayColorGC(ScColor(x, g, b), m_doc);
					break;
				case 120:
					tmp = ScColorEngine::getDisplayColorGC(ScColor(r, x, b), m_doc);
					break;
				case 240:
					tmp = ScColorEngine::getDisplayColorGC(ScColor(r, g, x), m_doc);
					break;
				}
				p.setBrush(tmp);
			}
			else
			{
				switch (farbe)
				{
				case 0:
					tmp = ScColorEngine::getDisplayColorGC(ScColor(x, 0, 0), m_doc);
					break;
				case 120:
					tmp = ScColorEngine::getDisplayColorGC(ScColor(0, x, 0), m_doc);
					break;
				case 240:
					tmp = ScColorEngine::getDisplayColorGC(ScColor(0, 0, x), m_doc);
					break;
				}
				p.setBrush(tmp);
			}
		}
		p.drawRect(x, 0, 5, 10);
	}
	p.end();
	return image0;
}

QPixmap CMYKChoose::SliderBlack()
{
	QPixmap image0 = QPixmap(255,10);
	QPainter p;
	int val = 255;
	p.begin(&image0);
	p.setPen(Qt::NoPen);
	int c, m, y, k;
	CMYKColor cmyk;
	ScColorEngine::getCMYKValues(Farbe, m_doc, cmyk);
	cmyk.getValues(c, m, y, k);
	for (int x = 0; x < 255; x += 5)
	{
		if (dynamic)
			p.setBrush( ScColorEngine::getDisplayColorGC(ScColor(c, m, y, x), m_doc) );
		else
			p.setBrush( ScColorEngine::getDisplayColorGC(ScColor(0, 0, 0, x), m_doc) );
		p.drawRect(x, 0, 5, 10);
		val -= 5;
	}
	p.end();
	return image0;
}

void CMYKChoose::SelSwatch(int n)
{
	if (n == 0)
		TabStack->setCurrentIndex(0);
	else
	{
		bool cus = false;
		CurrSwatch.clear();
		QString Cpfad = QDir::convertSeparators(ScPaths::getApplicationDataDir() + Swatches->currentText());
		QString pfadC = ScPaths::instance().libDir()+"swatches/";
		QString pfadC2 = pfadC + "Scribus_Basic.txt";
		if (n<customSetStartIndex)
		{
			QString listText=Swatches->itemText(n);
			if (listText=="Scribus OpenOffice")
				cus=true;
			pfadC2 = csm.paletteFileFromName(listText);
		}
		else
		{
			pfadC2 = Cpfad;
			cus = true;
		}
		if (n != 0)
		{
			QFileInfo fi = QFileInfo(pfadC2);
			QString ext = fi.suffix().toLower();
			if (extensionIndicatesEPSorPS(ext) || (ext == "ai"))
			{
				QString tmp, tmp2, FarNam;
				double c, m, y, k;
				ScColor cc;
				QFile f(pfadC2);
				if (f.open(QIODevice::ReadOnly))
				{
					QDataStream ts(&f);
					while (!ts.atEnd())
					{
						tmp = readLinefromDataStream(ts);
						if (((tmp.startsWith("%%CMYKCustomColor")) || (tmp.startsWith("%%CMYKProcessColor"))) && extensionIndicatesEPSorPS(ext))
						{
							if (tmp.startsWith("%%CMYKCustomColor"))
								tmp = tmp.remove(0,18);
							else if (tmp.startsWith("%%CMYKProcessColor"))
								tmp = tmp.remove(0,19);
							QTextStream ts2(&tmp, QIODevice::ReadOnly);
							ts2 >> c >> m >> y >> k;
							FarNam = ts2.readAll();
							FarNam = FarNam.trimmed();
							FarNam = FarNam.remove(0,1);
							FarNam = FarNam.remove(FarNam.length()-1,1);
							FarNam = FarNam.simplified();
							cc = ScColor(static_cast<int>(255 * c), static_cast<int>(255 * m), static_cast<int>(255 * y), static_cast<int>(255 * k));
							cc.setSpotColor(true);
							if (!CurrSwatch.contains(FarNam))
								CurrSwatch.insert(FarNam, cc);
							while (!ts.atEnd())
							{
								quint64 oldPos = ts.device()->pos();
								tmp = readLinefromDataStream(ts);
								if (!tmp.startsWith("%%+"))
								{
									ts.device()->seek(oldPos);
									break;
								}
								tmp = tmp.remove(0,3);
								QTextStream ts2(&tmp, QIODevice::ReadOnly);
								ts2 >> c >> m >> y >> k;
								FarNam = ts2.readAll();
								FarNam = FarNam.trimmed();
								FarNam = FarNam.remove(0,1);
								FarNam = FarNam.remove(FarNam.length()-1,1);
								FarNam = FarNam.simplified();
								cc = ScColor(static_cast<int>(255 * c), static_cast<int>(255 * m), static_cast<int>(255 * y), static_cast<int>(255 * k));
								cc.setSpotColor(true);
								if (!CurrSwatch.contains(FarNam))
									CurrSwatch.insert(FarNam, cc);
							}
						}
						if (tmp.startsWith("%%EndComments"))
						{
							if (ext == "ai")
							{
								while (!ts.atEnd())
								{
									bool isX = false;
									tmp = readLinefromDataStream(ts);
									if ((tmp.endsWith("Xa") || tmp.endsWith(" k") || tmp.endsWith(" x")) && (tmp.length() > 4))
									{
										QTextStream ts2(&tmp, QIODevice::ReadOnly);
										ts2 >> c >> m >> y >> k;
										if (tmp.endsWith(" x"))
										{
											isX = true;
											int an = tmp.indexOf("(");
											int en = tmp.lastIndexOf(")");
											FarNam = tmp.mid(an+1, en-an-1);
											FarNam = FarNam.simplified();
										}
										tmp = readLinefromDataStream(ts);
										if (tmp.endsWith("Pc"))
										{
											if (!isX)
											{
												tmp = tmp.trimmed();
												tmp = tmp.remove(0,1);
												int en = tmp.indexOf(")");
												FarNam = tmp.mid(0, en);
												FarNam = FarNam.simplified();
											}
											cc = ScColor(static_cast<int>(255 * c), static_cast<int>(255 * m), static_cast<int>(255 * y), static_cast<int>(255 * k));
											cc.setSpotColor(true);
											if (!CurrSwatch.contains(FarNam))
												CurrSwatch.insert(FarNam, cc);
										}
									}
								}
							}
							break;
						}
					}
					f.close();
				}
				CurrSwatch.insert("White", ScColor(0, 0, 0, 0));
				CurrSwatch.insert("Black", ScColor(0, 0, 0, 255));
			}
			else
			{
				QFile fiC(pfadC2);
				if (fiC.open(QIODevice::ReadOnly))
				{
					QString ColorEn, Cname;
					int Rval, Gval, Bval, Kval;
					QTextStream tsC(&fiC);
					ColorEn = tsC.readLine();
					if (ColorEn.startsWith("<?xml version="))
					{
						QByteArray docBytes("");
						loadRawText(pfadC2, docBytes);
						QString docText("");
						docText = QString::fromUtf8(docBytes);
						QDomDocument docu("scridoc");
						docu.setContent(docText);
						ScColor lf = ScColor();
						QDomElement elem = docu.documentElement();
						QDomNode PAGE = elem.firstChild();
						while(!PAGE.isNull())
						{
							QDomElement pg = PAGE.toElement();
							if(pg.tagName()=="COLOR" && pg.attribute("NAME")!=CommonStrings::None)
							{
								if (pg.hasAttribute("CMYK"))
									lf.setNamedColor(pg.attribute("CMYK"));
								else
									lf.fromQColor(QColor(pg.attribute("RGB")));
								if (pg.hasAttribute("Spot"))
									lf.setSpotColor(static_cast<bool>(pg.attribute("Spot").toInt()));
								else
									lf.setSpotColor(false);
								if (pg.hasAttribute("Register"))
									lf.setRegistrationColor(static_cast<bool>(pg.attribute("Register").toInt()));
								else
									lf.setRegistrationColor(false);
								CurrSwatch.insert(pg.attribute("NAME"), lf);
							}
							PAGE=PAGE.nextSibling();
						}
					}
					else
					{
						while (!tsC.atEnd())
						{
							ScColor tmp;
							ColorEn = tsC.readLine();
							if (ColorEn.length()>0 && ColorEn[0]==QChar('#'))
								continue;
							QTextStream CoE(&ColorEn, QIODevice::ReadOnly);
							CoE >> Rval;
							CoE >> Gval;
							CoE >> Bval;
							if (cus)
							{
								CoE >> Kval;
								Cname = CoE.readAll().trimmed();
								tmp.setColor(Rval, Gval, Bval, Kval);
							}
							else
							{
								Cname = CoE.readAll().trimmed();
								tmp.setColorRGB(Rval, Gval, Bval);
							}
		
							if ((n<customSetStartIndex) && (Cname.length()==0))
							{
								if (!cus)
									Cname=QString("#%1%2%3").arg(Rval,2,16).arg(Gval,2,16).arg(Bval,2,16).toUpper();
								else
									Cname=QString("#%1%2%3%4").arg(Rval,2,16).arg(Gval,2,16).arg(Bval,2,16).arg(Kval,2,16).toUpper();
								Cname.replace(" ","0");
							}
							if (CurrSwatch.contains(Cname))
							{
								if (tmp==CurrSwatch[Cname])
									continue;
								Cname=QString("%1%2").arg(Cname).arg(CurrSwatch.count());
							}
		
							CurrSwatch.insert(Cname, tmp);
						}
					}
					fiC.close();
				}
				else
				{
					CurrSwatch.insert("White", ScColor(0, 0, 0, 0));
					CurrSwatch.insert("Black", ScColor(0, 0, 0, 255));
					ScColor cc = ScColor(255, 255, 255, 255);
					cc.setRegistrationColor(true);
					CurrSwatch.insert("Registration", cc);
					CurrSwatch.insert("Blue", ScColor(255, 255, 0, 0));
					CurrSwatch.insert("Cyan", ScColor(255, 0, 0, 0));
					CurrSwatch.insert("Green", ScColor(255, 0, 255, 0));
					CurrSwatch.insert("Red", ScColor(0, 255, 255, 0));
					CurrSwatch.insert("Yellow", ScColor(0, 0, 255, 0));
					CurrSwatch.insert("Magenta", ScColor(0, 255, 0, 0));
				}
			}
		}
		ColorSwatch->clear();
		ColorSwatch->insertFancyPixmapItems(CurrSwatch);
		ColorSwatch->setCurrentRow( 0 );
		TabStack->setCurrentIndex(1);
	}
}
/*
void CMYKChoose::setRegist()
{
	disconnect( ComboBox1, SIGNAL(activated(const QString&)), this, SLOT(SelModel(const QString&)));
	if (Regist->isChecked())
	{
		ComboBox1->setCurrentIndex( 0 );
		Separations->setChecked(false);
		SelModel( tr("CMYK"));
	}
	connect( ComboBox1, SIGNAL(activated(const QString&)), this, SLOT(SelModel(const QString&)));
}
*/
void CMYKChoose::setSpot()
{
	disconnect( ComboBox1, SIGNAL(activated(const QString&)), this, SLOT(SelModel(const QString&)));
	if (Separations->isChecked())
	{
		ComboBox1->setCurrentIndex( 0 );
		SelModel( tr("CMYK"));
	}
	connect( ComboBox1, SIGNAL(activated(const QString&)), this, SLOT(SelModel(const QString&)));
}

void CMYKChoose::SelModel(const QString& mod)
{
	disconnect( CyanSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	disconnect( MagentaSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	disconnect( YellowSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	disconnect( BlackSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	disconnect( CyanSL, SIGNAL( valueChanged(int) ), this, SLOT( SetValueS(int) ) );
	disconnect( MagentaSL, SIGNAL( valueChanged(int) ), this, SLOT( SetValueS(int) ) );
	disconnect( YellowSL, SIGNAL( valueChanged(int) ), this, SLOT( SetValueS(int) ) );
	disconnect( BlackSL, SIGNAL( valueChanged(int) ), this, SLOT( SetValueS(int) ) );
	disconnect( CyanSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
	disconnect( MagentaSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
	disconnect( YellowSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
	disconnect( BlackSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
	if (mod == tr("CMYK"))
	{
		CMYKmode = true;
		Wsave = false;
		CyanSL->setMaximum( 100 );
		MagentaSL->setMaximum( 100 );
		YellowSL->setMaximum( 100 );
		CyanSp->setMaximum( 100 );
		MagentaSp->setMaximum( 100);
		YellowSp->setMaximum( 100 );
		CyanSL->setSingleStep(1);
		MagentaSL->setSingleStep(1);
		YellowSL->setSingleStep(1);
		CyanSL->setPageStep(10);
		MagentaSL->setPageStep(10);
		YellowSL->setPageStep(10);
		CyanSp->setDecimals(1);
		MagentaSp->setDecimals(1);
		YellowSp->setDecimals(1);
		CyanSp->setSingleStep(1);
		MagentaSp->setSingleStep(1);
		YellowSp->setSingleStep(1);
		CyanSp->setSuffix( tr(" %"));
		MagentaSp->setSuffix( tr(" %"));
		YellowSp->setSuffix( tr(" %"));
		CyanT->setText( tr("C:"));
		MagentaT->setText( tr("M:"));
		YellowT->setText( tr("Y:"));
		CyanP->setPixmap(SliderPix(180));
		MagentaP->setPixmap(SliderPix(300));
		YellowP->setPixmap(SliderPix(60));
		BlackP->setPixmap(SliderBlack());
/*		BlackP->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
		BlackSL->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
		BlackSp->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
		BlackT->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum)); */
		BlackP->show();
		BlackSL->show();
		BlackSp->show();
		BlackT->show();
		Farbe = ScColorEngine::convertToModel(Farbe, m_doc, colorModelCMYK);
		setValues();
	}
	else
		//	if (mod == tr("RGB"))
	{
		CMYKmode = false;
		Wsave = false;
		CyanSL->setMaximum( 255 );
		MagentaSL->setMaximum( 255 );
		YellowSL->setMaximum( 255 );
		CyanSL->setSingleStep(1);
		MagentaSL->setSingleStep(1);
		YellowSL->setSingleStep(1);
		CyanSL->setPageStep(1);
		MagentaSL->setPageStep(1);
		YellowSL->setPageStep(1);
		CyanSp->setSingleStep(1);
		MagentaSp->setSingleStep(1);
		YellowSp->setSingleStep(1);
		CyanSp->setMaximum( 255 );
		MagentaSp->setMaximum( 255 );
		YellowSp->setMaximum( 255 );
		CyanSp->setDecimals(0);
		MagentaSp->setDecimals(0);
		YellowSp->setDecimals(0);
		CyanSp->setSuffix("");
		MagentaSp->setSuffix("");
		YellowSp->setSuffix("");
		CyanT->setText( tr("R:"));
		MagentaT->setText( tr("G:"));
		YellowT->setText( tr("B:"));
		CyanP->setPixmap(SliderPix(0));
		MagentaP->setPixmap(SliderPix(120));
		YellowP->setPixmap(SliderPix(240));
		Layout2x->setSizeConstraint(QLayout::SetFixedSize);
/*		BlackP->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));
		BlackSL->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));
		BlackSp->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));
		BlackT->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored)); */
		BlackP->hide();
		BlackSL->hide();
		BlackSp->hide();
		BlackT->hide();
		if (mod == tr("Web Safe RGB"))
		{
			Wsave = true;
			CyanSL->setSingleStep(51);
			MagentaSL->setSingleStep(51);
			YellowSL->setSingleStep(51);
			CyanSL->setPageStep(51);
			MagentaSL->setPageStep(51);
			YellowSL->setPageStep(51);
			CyanSp->setSingleStep(51);
			MagentaSp->setSingleStep(51);
			YellowSp->setSingleStep(51);
		}
		Farbe = ScColorEngine::convertToModel(Farbe, m_doc, colorModelRGB);
		setValues();
	}
	imageN.fill( ScColorEngine::getDisplayColor(Farbe, m_doc) );
	if (ScColorEngine::isOutOfGamut(Farbe, m_doc))
		paintAlert(alertIcon, imageN, 2, 2, false);
	NewC->setPixmap( imageN );
	NewC->setToolTip( "<qt>" + tr( "If color management is enabled, a triangle warning indicator is a warning that the color maybe outside of the color gamut of the current printer profile selected. What this means is the color may not print exactly as indicated on screen. More hints about gamut warnings are in the online help under Color Management." ) + "</qt>");
	OldC->setToolTip( "<qt>" + tr( "If color management is enabled, a triangle warning indicator is a warning that the color maybe outside of the color gamut of the current printer profile selected. What this means is the color may not print exactly as indicated on screen. More hints about gamut warnings are in the online help under Color Management." ) + "</qt>");

	connect( CyanSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	connect( MagentaSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	connect( YellowSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	connect( BlackSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	connect( CyanSL, SIGNAL( valueChanged(int) ), this, SLOT( SetValueS(int) ) );
	connect( MagentaSL, SIGNAL( valueChanged(int) ), this, SLOT( SetValueS(int) ) );
	connect( YellowSL, SIGNAL( valueChanged(int) ), this, SLOT( SetValueS(int) ) );
	connect( BlackSL, SIGNAL( valueChanged(int) ), this, SLOT( SetValueS(int) ) );
	connect( CyanSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
	connect( MagentaSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
	connect( YellowSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
	connect( BlackSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
}

void CMYKChoose::setColor()
{
	int c, m, y, k;
	if (CMYKmode)
	{
		c = qRound(CyanSp->value() * 2.55);
		m = qRound(MagentaSp->value() * 2.55);
		y = qRound(YellowSp->value() * 2.55);
		k = qRound(BlackSp->value() * 2.55);
	}
	else
	{
		c = qRound(CyanSp->value());
		m = qRound(MagentaSp->value());
		y = qRound(YellowSp->value());
		k = qRound(BlackSp->value());
	}
	int h, s, v;
	BlackComp = k;
	if (Wsave)
	{
		blockSignals(true);
		c = c / 51 * 51;
		m = m / 51 * 51;
		y = y / 51 * 51;
		CyanSp->setValue(c);
		MagentaSp->setValue(m);
		YellowSp->setValue(y);
		CyanSL->setValue(c);
		MagentaSL->setValue(m);
		YellowSL->setValue(y);
		blockSignals(false);
	}
	ScColor tmp;
	if (CMYKmode)
	{
		tmp.setColor(c, m, y, k);
		if (dynamic)
		{
			CyanP->setPixmap(SliderPix(180));
			MagentaP->setPixmap(SliderPix(300));
			YellowP->setPixmap(SliderPix(60));
			BlackP->setPixmap(SliderBlack());
		}
	}
	else
	{
		tmp.setColorRGB(c, m, y);
		QColor tmp2 = QColor(c, m, y);
		tmp2.getHsv(&h, &s, &v);
		BlackComp = 255 - v;
		if (dynamic)
		{
			CyanP->setPixmap(SliderPix(0));
			MagentaP->setPixmap(SliderPix(120));
			YellowP->setPixmap(SliderPix(240));
		}
	}
	imageN.fill(ScColorEngine::getDisplayColor(tmp, m_doc) );
	if ( ScColorEngine::isOutOfGamut(tmp, m_doc) )
		paintAlert(alertIcon, imageN, 2, 2, false);
	ScColorEngine::getRGBColor(tmp, m_doc).getHsv(&h, &s, &v);
	NewC->setPixmap( imageN );
	Farbe = tmp;
	ColorMap->drawPalette(v);
	ColorMap->setMark(h, s);
}

void CMYKChoose::setColor2(int h, int s, bool ende)
{
	QColor tm = QColor::fromHsv(qMax(qMin(359,h),0), qMax(qMin(255,255-s),0), 255-BlackComp, QColor::Hsv);
	int r, g, b;
	tm.getRgb(&r, &g, &b);
	ScColor tmp;
	tmp.fromQColor(tm);
	if (CMYKmode)
	{
		CMYKColor cmyk;
		ScColorEngine::getCMYKValues(tmp, m_doc, cmyk);
		tmp.setColor(cmyk.c, cmyk.m, cmyk.y, cmyk.k);
	}
	imageN.fill( ScColorEngine::getDisplayColor(tmp, m_doc) );
	if ( ScColorEngine::isOutOfGamut(tmp, m_doc) )
		paintAlert(alertIcon, imageN, 2, 2, false);
	NewC->setPixmap( imageN );
	Farbe = tmp;
	if (ende)
		setValues();
}

void CMYKChoose::SelFromSwatch(QListWidgetItem* c)
{
	disconnect( ComboBox1, SIGNAL(activated(const QString&)), this, SLOT(SelModel(const QString&)));
	ScColor tmp = CurrSwatch[c->text()];
	if (isRegistration)
	{
		if (tmp.getColorModel() != colorModelCMYK)
			tmp = ScColorEngine::convertToModel(tmp, m_doc, colorModelCMYK);
		SelModel( tr("CMYK"));
	}
	else
	{
		if (tmp.getColorModel() == colorModelCMYK)
		{
			ComboBox1->setCurrentIndex( 0 );
			SelModel( tr("CMYK"));
		}
		else
		{
			ComboBox1->setCurrentIndex( 1 );
			SelModel( tr("RGB"));
		}
	}
	imageN.fill( ScColorEngine::getDisplayColor(tmp, m_doc) );
	if ( ScColorEngine::isOutOfGamut(tmp, m_doc) )
		paintAlert(alertIcon, imageN, 2, 2, false);
	NewC->setPixmap( imageN );
	Farbe = tmp;
	setValues();
	Separations->setChecked(tmp.isSpotColor());
	if ((isNew) && (!Farbname->isModified()))
		Farbname->setText(c->text());
	connect( ComboBox1, SIGNAL(activated(const QString&)), this, SLOT(SelModel(const QString&)));
}

void CMYKChoose::setValues()
{
	if (CMYKmode)
	{
		CMYKColor cmyk;
		int cc, cm, cy, ck;
		ScColorEngine::getCMYKValues(Farbe, m_doc, cmyk);
		cmyk.getValues(cc, cm, cy, ck);
		CyanSp->setValue(cc / 2.55);
		CyanSL->setValue(qRound(cc / 2.55));
		MagentaSp->setValue(cm / 2.55);
		MagentaSL->setValue(qRound(cm / 2.55));
		YellowSp->setValue(cy / 2.55);
		YellowSL->setValue(qRound(cy / 2.55));
		BlackSp->setValue(ck / 2.55);
		BlackSL->setValue(qRound(ck / 2.55));
		if (dynamic)
		{
			CyanP->setPixmap(SliderPix(180));
			MagentaP->setPixmap(SliderPix(300));
			YellowP->setPixmap(SliderPix(60));
			BlackP->setPixmap(SliderBlack());
		}
	}
	else
	{
		RGBColor rgb;
		int r, g, b;
		ScColorEngine::getRGBValues(Farbe, m_doc, rgb);
		rgb.getValues(r, g, b);
		CyanSp->setValue(static_cast<double>(r));
		CyanSL->setValue(r);
		MagentaSp->setValue(static_cast<double>(g));
		MagentaSL->setValue(g);
		YellowSp->setValue(static_cast<double>(b));
		YellowSL->setValue(b);
		int h, s, v;
		ScColorEngine::getRGBColor(Farbe, m_doc).getHsv(&h, &s, &v);
		BlackComp = 255 - v;
		if (dynamic)
		{
			CyanP->setPixmap(SliderPix(0));
			MagentaP->setPixmap(SliderPix(120));
			YellowP->setPixmap(SliderPix(240));
		}
	}
}

void CMYKChoose::Verlassen()
{
	// if condition 10/21/2004 pv #1191 - just be sure that user cannot create "None" color
	if (Farbname->text().isEmpty())
	{
		QMessageBox::information(this, CommonStrings::trWarning, tr("You cannot create a color without a name\nPlease give it a name"), 0);
		Farbname->setFocus();
		Farbname->selectAll();
		return;
	}
	if (Farbname->text() == CommonStrings::None || Farbname->text() == CommonStrings::tr_NoneColor)
	{
		QMessageBox::information(this, CommonStrings::trWarning, tr("You cannot create a color named \"%1\".\nIt is a reserved name for transparent color").arg(Farbname->text()), 0);
		Farbname->setFocus();
		Farbname->selectAll();
		return;
	}
	if ((Fnam != Farbname->text()) || (isNew))
	{
		if (EColors->contains(Farbname->text()))
		{
			QMessageBox::information(this, CommonStrings::trWarning, tr("The name of the color already exists,\nplease choose another one."), CommonStrings::tr_OK, 0, 0, 0, QMessageBox::Ok);
			Farbname->selectAll();
			Farbname->setFocus();
			return;
		}
		else
			accept();
	}
	else
		accept();
}
