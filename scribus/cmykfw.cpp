#include "cmykfw.h"
#include "cmykfw.moc"
#include <qpainter.h>
#include <qpopupmenu.h>
#include <qcursor.h>
#include <qmessagebox.h>
#include <qfileinfo.h>
#include <qdir.h>
#include <cstdlib>

#include "commonstrings.h"
#include "scconfig.h"
#include "scpaths.h"
#ifdef HAVE_CMS
extern bool SoftProofing;
extern bool Gamut;
extern bool CMSuse;
#endif
#include "util.h"

CMYKChoose::CMYKChoose( QWidget* parent, ScColor orig, QString name, ColorList *Colors, QStringList Cust  )
		: QDialog( parent, "fw", true, 0 )
{
	if (orig.getColorModel () == colorModelCMYK)
		CMYKmode = true;
	else
		CMYKmode = false;
	dynamic = true;
	Wsave = false;
	EColors = Colors;
	CurrSwatch.clear();
	orig.checkGamut();
	alertIcon = loadIcon("alert.png");
	imageA = QPixmap(50,50);
	imageA.fill(orig.getRawRGBColor());
	if (orig.isOutOfGamut())
		paintAlert(alertIcon,imageA, 2, 2);
	imageN = QPixmap(50,50);
	imageN.fill(orig.getRawRGBColor());
	if (orig.isOutOfGamut())
		paintAlert(alertIcon, imageN, 2, 2);
	Farbe = orig;
	QPixmap image0 = SliderPix(180);
	QPixmap image1 = SliderPix(300);
	QPixmap image2 = SliderPix(60);
	QPixmap image3 = SliderBlack();
	int cc, cm, cy, ck;
	double ccd, cmd, cyd, ckd;
	orig.getCMYK(&cc, &cm, &cy, &ck);
	ccd = cc / 2.55;
	cmd = cm / 2.55;
	cyd = cy / 2.55;
	ckd = ck / 2.55;
	int cr, cg, cb;
	double crd, cgd, cbd;
	orig.getRGB(&cr, &cg, &cb);
	crd = cr / 2.55;
	cgd = cg / 2.55;
	cbd = cb / 2.55;
	resize( 498, 306 );
	setCaption( tr( "Edit Color" ) );
	setIcon(loadIcon("AppIcon.png"));
	CMYKFarbenLayout = new QHBoxLayout( this );
	CMYKFarbenLayout->setSpacing( 6 );
	CMYKFarbenLayout->setMargin( 11 );
	Layout23 = new QVBoxLayout;
	Layout23->setSpacing( 7 );
	Layout23->setMargin( 0 );

	TextLabel1 = new QLabel( tr( "&Name:" ), this, "TextLabel1" );
	TextLabel1->setMinimumSize( QSize( 200, 22 ) );
	Layout23->addWidget( TextLabel1 );

	Farbname = new QLineEdit( this, "Farbname" );
	Farbname->setMinimumSize( QSize( 200, 22 ) );
	Farbname->setText( name );
	TextLabel1->setBuddy( Farbname );
	Layout23->addWidget( Farbname );

	TextLabel3 = new QLabel( tr( "Color &Model" ), this, "TextLabel3" );
	TextLabel3->setMinimumSize( QSize( 100, 22 ) );
	Layout23->addWidget( TextLabel3 );

	ComboBox1 = new QComboBox( true, this, "ComboBox1" );
	ComboBox1->setEditable(false);
	ComboBox1->insertItem( tr( "CMYK" ) );
	ComboBox1->insertItem( tr( "RGB" ) );
	ComboBox1->insertItem( tr( "Web Safe RGB" ) );
	ComboBox1->setMinimumSize( QSize( 200, 22 ) );
	if (!CMYKmode)
		ComboBox1->setCurrentItem( 1 );
	TextLabel3->setBuddy( ComboBox1 );
	Layout23->addWidget( ComboBox1 );

	Separations = new QCheckBox( this, "Separations" );
	Separations->setText( tr( "Is Spot-Color" ) );
	Separations->setChecked(orig.isSpotColor());
	Layout23->addWidget( Separations );

	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
	Layout23->addItem( spacer );

	Layout2 = new QGridLayout;
	Layout2->setSpacing( 6 );
	Layout2->setMargin( 0 );

	TextLabel5_2 = new QLabel( tr( "New" ), this, "TextLabel5_2" );
	TextLabel5_2->setMinimumSize( QSize( 50, 22 ) );
	TextLabel5_2->setMaximumSize( QSize( 100, 22 ) );

	Layout2->addWidget( TextLabel5_2, 0, 1 );

	OldC = new QLabel( "", this, "OldC" );
	OldC->setMinimumSize( QSize( 50, 50 ) );
	OldC->setMaximumSize( QSize( 50, 50 ) );
	OldC->setFrameShape( QLabel::WinPanel );
	OldC->setFrameShadow( QLabel::Sunken );
	OldC->setScaledContents( true );
	OldC->setAlignment( int( QLabel::AlignCenter ) );
	OldC->setPixmap( imageA );

	Layout2->addWidget( OldC, 1, 0 );

	TextLabel5 = new QLabel( tr( "Old" ), this, "TextLabel5" );
	TextLabel5->setMinimumSize( QSize( 50, 22 ) );
	TextLabel5->setMaximumSize( QSize( 100, 22 ) );

	Layout2->addWidget( TextLabel5, 0, 0 );

	NewC = new QLabel( "", this, "NewC" );
	NewC->setMinimumSize( QSize( 50, 50 ) );
	NewC->setMaximumSize( QSize( 50, 50 ) );
	NewC->setFrameShape( QLabel::WinPanel );
	NewC->setFrameShadow( QLabel::Sunken );
	NewC->setScaledContents( true );
	NewC->setAlignment( int( QLabel::AlignCenter ) );
	NewC->setPixmap( imageN );

	Layout2->addWidget( NewC, 1, 1 );
	Layout23->addLayout( Layout2 );

	Layout21 = new QHBoxLayout;
	Layout21->setSpacing( 20 );
	Layout21->setMargin( 10 );

	Cancel_2 = new QPushButton( CommonStrings::tr_OK, this, "Cancel_2" );
	Cancel_2->setDefault( true );
	Layout21->addWidget( Cancel_2 );
	Cancel = new QPushButton( CommonStrings::tr_Cancel, this, "Cancel" );
	Layout21->addWidget( Cancel );
	Layout23->addLayout( Layout21 );
	CMYKFarbenLayout->addLayout( Layout23 );

	Frame4 = new QFrame( this, "Frame4" );
	Frame4->setFrameShape( QFrame::NoFrame );
	Frame4->setFrameShadow( QFrame::Raised );
	Frame4Layout = new QVBoxLayout( Frame4 );
	Frame4Layout->setSpacing( 6 );
	Frame4Layout->setMargin( 0 );
	
	Swatches = new QComboBox( true, Frame4, "ComboBox1" );
	Swatches->setEditable(false);
	Swatches->insertItem( tr( "HSV-Colormap" ) );
	Swatches->insertItem("X11 RGB-Set");
	Swatches->insertItem("X11 Grey-Set");
	Swatches->insertItem("Gnome-Set");
	Swatches->insertItem("SVG-Set");
	Swatches->insertItem("OpenOffice.org-Set");

	if (Cust.count() != 0)
	{
		QStringList realEx;
		realEx.clear();
		for (uint m = 0; m < Cust.count(); ++m)
		{
			QString Cpfad = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus/"+Cust[m]);
			QFileInfo cfi(Cpfad);
			if (cfi.exists())
			{
				Swatches->insertItem(Cust[m]);
				realEx.append(Cust[m]);
			}
		}
		CColSet = realEx;
	}
	Frame4Layout->addWidget( Swatches );
	
	TabStack = new QWidgetStack( Frame4, "TabStack" );
	TabStack->setFrameShape( QWidgetStack::NoFrame );

	Frame5a = new QFrame( TabStack, "Frame4" );
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
	ColorMap = new ColorChart( Frame5);
	ColorMap->setMinimumSize( QSize( 180, 128 ) );
	ColorMap->setMaximumSize( QSize( 180, 128 ) );
	Frame5Layout->addWidget( ColorMap );
	Frame5aLayout->addWidget( Frame5, 0, AlignCenter);
	TabStack->addWidget( Frame5a, 0 );
	
	ColorSwatch = new QListBox(TabStack, "StyledL");
	TabStack->addWidget( ColorSwatch, 1 );

	Frame4Layout->addWidget( TabStack );
	
	Layout2x = new QGridLayout;
	Layout2x->setSpacing( 6 );
	Layout2x->setMargin( 0 );

	CyanT = new QLabel( tr( "C:" ), Frame4, "Cyant" );
	Layout2x->addWidget(CyanT, 0, 0);

	Layout1_2 = new QVBoxLayout;
	Layout1_2->setSpacing( 0 );
	Layout1_2->setMargin( 0 );

	CyanP = new QLabel( Frame4, "CyanP" );
	CyanP->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5,
	                                   CyanP->sizePolicy().hasHeightForWidth() ) );
	CyanP->setMinimumSize( QSize( 200, 10 ) );
	CyanP->setPixmap(image0);
	CyanP->setScaledContents( true );
	Layout1_2->addWidget( CyanP );

	CyanSL = new QSlider( Frame4, "CyanSL" );
	CyanSL->setMinimumSize( QSize( 200, 16 ) );
	CyanSL->setMaxValue( 100 );
	CyanSL->setOrientation( QSlider::Horizontal );
	CyanSL->setTickmarks( QSlider::NoMarks );
	Layout1_2->addWidget( CyanSL );
	Layout2x->addLayout(Layout1_2, 0, 1);

	CyanSp = new MSpinBox( Frame4, 0 );
	CyanSp->setMaxValue( 100 );
	CyanSp->setSuffix( tr(" %"));
	Layout2x->addWidget(CyanSp, 0, 2);
	CyanSp->setValue(ccd);
	CyanSL->setValue(qRound(ccd));

	MagentaT = new QLabel( tr( "M:" ), Frame4, "Cyant" );
	Layout2x->addWidget(MagentaT, 1, 0);

	Layout1_2_2 = new QVBoxLayout;
	Layout1_2_2->setSpacing( 0 );
	Layout1_2_2->setMargin( 0 );

	MagentaP = new QLabel( Frame4, "MagentaP" );
	MagentaP->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5,
	                                      MagentaP->sizePolicy().hasHeightForWidth() ) );
	MagentaP->setMinimumSize( QSize( 200, 10 ) );
	MagentaP->setPixmap(image1);
	MagentaP->setScaledContents( true );
	Layout1_2_2->addWidget( MagentaP );

	MagentaSL = new QSlider( Frame4, "MagentaSL" );
	MagentaSL->setMinimumSize( QSize( 200, 16 ) );
	MagentaSL->setMaxValue( 100 );
	MagentaSL->setOrientation( QSlider::Horizontal );
	MagentaSL->setTickmarks( QSlider::NoMarks );
	Layout1_2_2->addWidget( MagentaSL );
	Layout2x->addLayout(Layout1_2_2, 1, 1);

	MagentaSp = new MSpinBox( Frame4, 0 );
	MagentaSp->setMaxValue( 100 );
	MagentaSp->setSuffix( tr(" %"));
	Layout2x->addWidget(MagentaSp, 1, 2);
	MagentaSp->setValue(cmd);
	MagentaSL->setValue(qRound(cmd));

	YellowT = new QLabel( tr( "Y:" ), Frame4, "Cyant" );
	Layout2x->addWidget(YellowT, 2, 0);

	Layout1_2_3 = new QVBoxLayout;
	Layout1_2_3->setSpacing( 0 );
	Layout1_2_3->setMargin( 0 );

	YellowP = new QLabel( Frame4, "YellowP" );
	YellowP->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5,
	                                     YellowP->sizePolicy().hasHeightForWidth() ) );
	YellowP->setMinimumSize( QSize( 200, 10 ) );
	YellowP->setPixmap(image2);
	YellowP->setScaledContents( true );
	Layout1_2_3->addWidget( YellowP );

	YellowSL = new QSlider( Frame4, "YellowSL" );
	YellowSL->setMinimumSize( QSize( 200, 16 ) );
	YellowSL->setMaxValue( 100 );
	YellowSL->setOrientation( QSlider::Horizontal );
	YellowSL->setTickmarks( QSlider::NoMarks );
	Layout1_2_3->addWidget( YellowSL );
	Layout2x->addLayout(Layout1_2_3, 2, 1);

	YellowSp = new MSpinBox( Frame4, 0 );
	YellowSp->setMaxValue( 100 );
	YellowSp->setSuffix( tr(" %"));
	Layout2x->addWidget(YellowSp, 2, 2);
	YellowSp->setValue(cyd);
	YellowSL->setValue(qRound(cyd));

	BlackT = new QLabel( tr( "K:" ), Frame4, "Cyant" );
	Layout2x->addWidget(BlackT, 3, 0);

	Layout1_2_4 = new QVBoxLayout;
	Layout1_2_4->setSpacing( 0 );
	Layout1_2_4->setMargin( 0 );

	BlackP = new QLabel( Frame4, "BlackP" );
	BlackP->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5,
	                                    BlackP->sizePolicy().hasHeightForWidth() ) );
	BlackP->setMinimumSize( QSize( 200, 10 ) );
	BlackP->setPixmap(image3);
	BlackP->setScaledContents( true );
	Layout1_2_4->addWidget( BlackP );

	BlackSL = new QSlider( Frame4, "BlackSL" );
	BlackSL->setMinimumSize( QSize( 200, 16 ) );
	BlackSL->setMaxValue( 100 );
	BlackSL->setOrientation( QSlider::Horizontal );
	BlackSL->setTickmarks( QSlider::NoMarks );
	Layout1_2_4->addWidget( BlackSL );
	Layout2x->addLayout(Layout1_2_4, 3, 1);

	BlackSp = new MSpinBox( Frame4, 0 );
	BlackSp->setMaxValue( 100 );
	BlackSp->setSuffix( tr(" %"));
	Layout2x->addWidget(BlackSp, 3, 2);
	BlackSp->setValue(ckd);
	BlackSL->setValue(qRound(ckd));
	BlackComp = ck;
	Frame4Layout->addLayout( Layout2x );
	CMYKFarbenLayout->addWidget( Frame4 );
	int h, s, v;
	orig.getRGBColor().hsv(&h, &s, &v);
	ColorMap->drawPalette(v);
	ColorMap->setMark(h, s);
	Fnam = name;
	Farbname->selectAll();
	Farbname->setFocus();
	TabStack->raiseWidget(0);
	// signals and slots connections
	connect( Cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( Cancel_2, SIGNAL( clicked() ), this, SLOT( Verlassen() ) );
	connect( CyanSp, SIGNAL( valueChanged(int) ), CyanSL, SLOT( setValue(int) ) );
	connect( CyanSL, SIGNAL( valueChanged(int) ), this, SLOT( SetValueS(int) ) );
	connect( MagentaSp, SIGNAL( valueChanged(int) ), MagentaSL, SLOT( setValue(int) ) );
	connect( MagentaSL, SIGNAL( valueChanged(int) ), this, SLOT( SetValueS(int) ) );
	connect( YellowSp, SIGNAL( valueChanged(int) ), YellowSL, SLOT( setValue(int) ) );
	connect( YellowSL, SIGNAL( valueChanged(int) ), this, SLOT( SetValueS(int) ) );
	connect( BlackSp, SIGNAL( valueChanged(int) ), BlackSL, SLOT( setValue(int) ) );
	connect( BlackSL, SIGNAL( valueChanged(int) ), this, SLOT( SetValueS(int) ) );
	connect( CyanSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
	connect( MagentaSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
	connect( YellowSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
	connect( BlackSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
	connect( ColorMap, SIGNAL( ColorVal(int, int, bool)), this, SLOT( setColor2(int, int, bool)));
	connect( ComboBox1, SIGNAL(activated(const QString&)), this, SLOT(SelModel(const QString&)));
	connect( Swatches, SIGNAL(activated(int)), this, SLOT(SelSwatch(int)));
	connect(ColorSwatch, SIGNAL(highlighted(int)), this, SLOT(SelFromSwatch(int)));
	connect(Separations, SIGNAL(clicked()), this, SLOT(setSpot()));
	if (!CMYKmode)
		SelModel ( tr( "RGB" ));
}

void CMYKChoose::mouseReleaseEvent(QMouseEvent *m)
{
	if (m->button() == RightButton)
	{
		QPopupMenu *pmen = new QPopupMenu();
		qApp->setOverrideCursor(QCursor(ArrowCursor), true);
		int px = pmen->insertItem( tr("Dynamic Color Bars"), this, SLOT(ToggleSL()));
		int py = pmen->insertItem( tr("Static Color Bars"), this, SLOT(ToggleSL()));
		pmen->setItemChecked((dynamic ? px : py) , true);
		pmen->exec(QCursor::pos());
		delete pmen;
	}
}

void CMYKChoose::SetValueS(int val)
{
	disconnect( CyanSp, SIGNAL( valueChanged(int) ), CyanSL, SLOT( setValue(int) ) );
	disconnect( MagentaSp, SIGNAL( valueChanged(int) ), MagentaSL, SLOT( setValue(int) ) );
	disconnect( YellowSp, SIGNAL( valueChanged(int) ), YellowSL, SLOT( setValue(int) ) );
	disconnect( BlackSp, SIGNAL( valueChanged(int) ), BlackSL, SLOT( setValue(int) ) );
	if (CyanSL == sender())
		CyanSp->setValue(val);
	if (MagentaSL == sender())
		MagentaSp->setValue(val);
	if (YellowSL == sender())
		YellowSp->setValue(val);
	if (BlackSL == sender())
		BlackSp->setValue(val);
	setColor();
	connect( CyanSp, SIGNAL( valueChanged(int) ), CyanSL, SLOT( setValue(int) ) );
	connect( MagentaSp, SIGNAL( valueChanged(int) ), MagentaSL, SLOT( setValue(int) ) );
	connect( YellowSp, SIGNAL( valueChanged(int) ), YellowSL, SLOT( setValue(int) ) );
	connect( BlackSp, SIGNAL( valueChanged(int) ), BlackSL, SLOT( setValue(int) ) );
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
	QPixmap image0 = QPixmap(255,10);
	QPainter p;
	p.begin(&image0);
	p.setPen(NoPen);
	int r, g, b, c, m, y, k;
	QColor tmp;
	for (int x = 0; x < 255; x += 5)
	{
		if (CMYKmode)
		{
			Farbe.getCMYK(&c, &m, &y, &k);
			if (dynamic)
			{
				switch (farbe)
				{
				case 180:
					tmp = ScColor(x, m, y, k).getRGBColor();
					break;
				case 300:
					tmp = ScColor(c, x, y, k).getRGBColor();
					break;
				case 60:
					tmp = ScColor(c, m, x, k).getRGBColor();
					break;
				}
				p.setBrush(tmp);
			}
			else
			{
				switch (farbe)
				{
				case 180:
					tmp = ScColor(x, 0, 0, 0).getRGBColor();
					break;
				case 300:
				        tmp = ScColor(0, x, 0, 0).getRGBColor();
					break;
				case 60:
					tmp = ScColor(0, 0, x, 0).getRGBColor();
					break;
				}
				p.setBrush(tmp);
			}
		}
		else
		{
			Farbe.getRGB(&r, &g, &b);
			if (dynamic)
			{
				switch (farbe)
				{
				case 0:
					tmp = ScColor(x, g, b).getRGBColor();
					break;
				case 120:
					tmp = ScColor(r, x, b).getRGBColor();
					break;
				case 240:
					tmp = ScColor(r, g, x).getRGBColor();
					break;
				}
				p.setBrush(tmp);
			}
			else
			{
				switch (farbe)
				{
				case 0:
					tmp = ScColor(x, 0, 0).getRGBColor();
					break;
				case 120:
				        tmp = ScColor(0, x, 0).getRGBColor();
					break;
				case 240:
					tmp = ScColor(0, 0, x).getRGBColor();
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
	p.setPen(NoPen);
	int c, m, y, k;
	Farbe.getCMYK(&c, &m, &y, &k);
	for (int x = 0; x < 255; x += 5)
	{
		if (dynamic)
			p.setBrush(ScColor(c, m, y, x).getRGBColor());
		else
			p.setBrush(ScColor(0, 0, 0, x).getRGBColor());
		p.drawRect(x, 0, 5, 10);
		val -= 5;
	}
	p.end();
	return image0;
}

void CMYKChoose::SelSwatch(int n)
{
	if (n == 0)
		TabStack->raiseWidget(0);
	else
	{
		bool cus = false;
		CurrSwatch.clear();
		QString Cpfad = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus/"+Swatches->currentText());
		QString pfadC = ScPaths::instance().libDir();
		QString pfadC2 = pfadC + "rgbscribus.txt";
		switch (n)
		{
		case 1:
			pfadC2 = pfadC + "rgbscribus.txt";
			break;
		case 2:
			pfadC2 = pfadC + "rgbscribusgreys.txt";
			break;
		case 3:
			pfadC2 = pfadC + "rgbscribusgnome.txt";
			break;
		case 4:
			pfadC2 = pfadC + "rgbsvg.txt";
			break;
		case 5:
			pfadC2 = pfadC + "rgbscribusopenoffice.txt";
			cus = true;
			break;
		default:
			pfadC2 = Cpfad;
			cus = true;
			break;
		}
		if (n != 0)
		{
			QFile fiC(pfadC2);
			if (fiC.open(IO_ReadOnly))
			{
				QString ColorEn, Cname;
				int Rval, Gval, Bval, Kval;
				QTextStream tsC(&fiC);
				ColorEn = tsC.readLine();
				while (!tsC.atEnd())
				{
					ScColor tmp;
					ColorEn = tsC.readLine();
					QTextStream CoE(&ColorEn, IO_ReadOnly);
					CoE >> Rval;
					CoE >> Gval;
					CoE >> Bval;
					if (cus)
					{
						CoE >> Kval;
						Cname = CoE.read().stripWhiteSpace();
						tmp.setColor(Rval, Gval, Bval, Kval);
					}
					else
					{
						Cname = CoE.read().stripWhiteSpace();
						tmp.setColorRGB(Rval, Gval, Bval);
					}
					CurrSwatch.insert(Cname, tmp);
				}
				fiC.close();
			}
			else
			{
				CurrSwatch.insert("White", ScColor(0, 0, 0, 0));
				CurrSwatch.insert("Black", ScColor(0, 0, 0, 255));
				CurrSwatch.insert("Blue", ScColor(255, 255, 0, 0));
				CurrSwatch.insert("Cyan", ScColor(255, 0, 0, 0));
				CurrSwatch.insert("Green", ScColor(255, 0, 255, 0));
				CurrSwatch.insert("Red", ScColor(0, 255, 255, 0));
				CurrSwatch.insert("Yellow", ScColor(0, 0, 255, 0));
				CurrSwatch.insert("Magenta", ScColor(0, 255, 0, 0));
			}
		}
		ColorSwatch->clear();
		ColorList::Iterator it;
		QPixmap pm = QPixmap(30, 15);
		for (it = CurrSwatch.begin(); it != CurrSwatch.end(); ++it)
		{
			pm.fill(CurrSwatch[it.key()].getRGBColor());
			ColorSwatch->insertItem(pm, it.key());
		}
		ColorSwatch->setSelected(ColorSwatch->currentItem(), false);
		TabStack->raiseWidget(1);
	}
}

void CMYKChoose::setSpot()
{
	disconnect( ComboBox1, SIGNAL(activated(const QString&)), this, SLOT(SelModel(const QString&)));
	if (Separations->isChecked())
	{
		ComboBox1->setCurrentItem( 0 );
		SelModel( tr("CMYK"));
	}
	connect( ComboBox1, SIGNAL(activated(const QString&)), this, SLOT(SelModel(const QString&)));
}

void CMYKChoose::SelModel(const QString& mod)
{
	disconnect( CyanSp, SIGNAL( valueChanged(int) ), CyanSL, SLOT( setValue(int) ) );
	disconnect( MagentaSp, SIGNAL( valueChanged(int) ), MagentaSL, SLOT( setValue(int) ) );
	disconnect( YellowSp, SIGNAL( valueChanged(int) ), YellowSL, SLOT( setValue(int) ) );
	disconnect( BlackSp, SIGNAL( valueChanged(int) ), BlackSL, SLOT( setValue(int) ) );
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
		CyanSL->setMaxValue( 100 );
		MagentaSL->setMaxValue( 100 );
		YellowSL->setMaxValue( 100 );
		CyanSp->setMaxValue( 100 );
		MagentaSp->setMaxValue( 100);
		YellowSp->setMaxValue( 100 );
		CyanSL->setLineStep(1);
		MagentaSL->setLineStep(1);
		YellowSL->setLineStep(1);
		CyanSL->setPageStep(10);
		MagentaSL->setPageStep(10);
		YellowSL->setPageStep(10);
		CyanSp->setDecimals(1);
		MagentaSp->setDecimals(1);
		YellowSp->setDecimals(1);
		CyanSp->setLineStep(1);
		MagentaSp->setLineStep(1);
		YellowSp->setLineStep(1);
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
		BlackP->show();
		BlackSL->show();
		BlackSp->show();
		BlackT->show();
		setValues();
	}
	else
		//	if (mod == tr("RGB"))
	{
		CMYKmode = false;
		Wsave = false;
		CyanSL->setMaxValue( 255 );
		MagentaSL->setMaxValue( 255 );
		YellowSL->setMaxValue( 255 );
		CyanSL->setLineStep(1);
		MagentaSL->setLineStep(1);
		YellowSL->setLineStep(1);
		CyanSL->setPageStep(1);
		MagentaSL->setPageStep(1);
		YellowSL->setPageStep(1);
		CyanSp->setDecimals(1);
		MagentaSp->setDecimals(1);
		YellowSp->setDecimals(1);
		CyanSp->setLineStep(1);
		MagentaSp->setLineStep(1);
		YellowSp->setLineStep(1);
		CyanSp->setMaxValue( 255 );
		MagentaSp->setMaxValue( 255 );
		YellowSp->setMaxValue( 255 );
		CyanSp->setSuffix("");
		MagentaSp->setSuffix("");
		YellowSp->setSuffix("");
		CyanT->setText( tr("R:"));
		MagentaT->setText( tr("G:"));
		YellowT->setText( tr("B:"));
		CyanP->setPixmap(SliderPix(0));
		MagentaP->setPixmap(SliderPix(120));
		YellowP->setPixmap(SliderPix(240));
		BlackP->hide();
		BlackSL->hide();
		BlackSp->hide();
		BlackT->hide();
		if (mod == tr("Web Safe RGB"))
		{
			Wsave = true;
			CyanSL->setLineStep(51);
			MagentaSL->setLineStep(51);
			YellowSL->setLineStep(51);
			CyanSL->setPageStep(51);
			MagentaSL->setPageStep(51);
			YellowSL->setPageStep(51);
			CyanSp->setLineStep(51);
			MagentaSp->setLineStep(51);
			YellowSp->setLineStep(51);
		}
		setValues();
	}
	connect( CyanSp, SIGNAL( valueChanged(int) ), CyanSL, SLOT( setValue(int) ) );
	connect( MagentaSp, SIGNAL( valueChanged(int) ), MagentaSL, SLOT( setValue(int) ) );
	connect( YellowSp, SIGNAL( valueChanged(int) ), YellowSL, SLOT( setValue(int) ) );
	connect( BlackSp, SIGNAL( valueChanged(int) ), BlackSL, SLOT( setValue(int) ) );
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
		QColor tmp2 = QColor(c, m, y, QColor::Rgb);
		tmp2.hsv(&h, &s, &v);
		BlackComp = 255 - v;
		if (dynamic)
		{
			CyanP->setPixmap(SliderPix(0));
			MagentaP->setPixmap(SliderPix(120));
			YellowP->setPixmap(SliderPix(240));
		}
	}
	imageN.fill(tmp.getRawRGBColor());
	tmp.checkGamut();
	if (tmp.isOutOfGamut())
		paintAlert(alertIcon, imageN, 2, 2);
	tmp.getRGBColor().hsv(&h, &s, &v);
	NewC->setPixmap( imageN );
	Farbe = tmp;
	ColorMap->drawPalette(v);
	ColorMap->setMark(h, s);
}

void CMYKChoose::setColor2(int h, int s, bool ende)
{
	QColor tm = QColor(QMAX(QMIN(359,h),0), QMAX(QMIN(255,255-s),0), 255-BlackComp, QColor::Hsv);
	int r, g, b;
	tm.rgb(&r, &g, &b);
	ScColor tmp;
	tmp.fromQColor(tm);
	if (CMYKmode)
		tmp.setColorModel(colorModelCMYK);
	tmp.checkGamut();
	imageN.fill(tmp.getRawRGBColor());
	if (tmp.isOutOfGamut())
		paintAlert(alertIcon, imageN, 2, 2);
	NewC->setPixmap( imageN );
	Farbe = tmp;
	if (ende)
		setValues();
}

void CMYKChoose::SelFromSwatch(int c)
{
	ScColor tmp = CurrSwatch[ColorSwatch->text(c)];
	if (CMYKmode)
		tmp.setColorModel(colorModelCMYK);
	tmp.checkGamut();
	imageN.fill(tmp.getRawRGBColor());
	if (tmp.isOutOfGamut())
		paintAlert(alertIcon, imageN, 2, 2);
	NewC->setPixmap( imageN );
	Farbe = tmp;
	setValues();
}

void CMYKChoose::setValues()
{
	if (CMYKmode)
	{
		int cc, cm, cy, ck;
		Farbe.getCMYK(&cc, &cm, &cy, &ck);
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
		int r, g, b;
		Farbe.getRGB(&r, &g, &b);
		CyanSp->setValue(static_cast<double>(r));
		CyanSL->setValue(r);
		MagentaSp->setValue(static_cast<double>(g));
		MagentaSL->setValue(g);
		YellowSp->setValue(static_cast<double>(b));
		YellowSL->setValue(b);
		int h, s, v;
		Farbe.getRGBColor().hsv(&h, &s, &v);
		BlackComp = 255 - v;
		if (dynamic)
		{
			CyanP->setPixmap(SliderPix(0));
			MagentaP->setPixmap(SliderPix(120));
			YellowP->setPixmap(SliderPix(240));
		}
	}
}

QColor CMYKChoose::CMYK2RGB(int c, int m, int y, int k)
{
	/* this code should no longer be used */
	return QColor(255-QMIN(255, c+k), 255-QMIN(255,m+k), 255-QMIN(255,y+k));
}

void CMYKChoose::Verlassen()
{
	// if condition 10/21/2004 pv #1191 - just be sure that user cannot create "None" color
	if (Farbname->text() == "None" || Farbname->text() == tr("None"))
	{
		QMessageBox::information(this, tr("Warning"), tr("You cannot create a color named \"%1\".\nIt's a reserved name for transparent color").arg(Farbname->text()), 0);
		Farbname->setFocus();
		Farbname->selectAll();
		return;
	}
	if (Fnam != Farbname->text())
	{
		if (EColors->contains(Farbname->text()))
		{
			QMessageBox::information(this, tr("Warning"), tr("Name of the Color is not unique"), CommonStrings::tr_OK, 0, 0, 0, QMessageBox::Ok);
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
