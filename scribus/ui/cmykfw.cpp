
/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmykfw.h"

#include <QAction>
#include <QByteArray>
#include <QCheckBox>
#include <QCursor>
#include <QDomDocument>
#include <QDir>
#include <QFileInfo>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QStackedWidget>
#include <QSignalBlocker>
#include <QSlider>
#include <QSpacerItem>
#include <QToolTip>
#include <QTextStream>
#include <QTreeWidget>
#include <QVBoxLayout>
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
#include "swatchcombo.h"
#include "ui/scmessagebox.h"
#include "util.h"
#include "util_color.h"
#include "util_formats.h"
#include "iconmanager.h"


CMYKChoose::CMYKChoose( QWidget* parent, ScribusDoc* doc, ScColor orig, QString name, ColorList *Colors, bool newCol  )
		: QDialog( parent ), CurrSwatch(doc)
{
	setModal(true);
	m_doc = doc;
	isNew = newCol;
	dynamic = true;
	Wsave = false;
	isHLC = false;
	EColors = Colors;
	CurrSwatch.clear();
	alertIcon = IconManager::instance()->loadPixmap("alert.png");
	imageA = QPixmap(50,50);
	imageA.fill( ScColorEngine::getDisplayColor(orig, m_doc) );
	if ( ScColorEngine::isOutOfGamut(orig, m_doc) )
		paintAlert(alertIcon,imageA, 2, 2, false);
	imageN = QPixmap(50,50);
	imageN.fill( ScColorEngine::getDisplayColor(orig, m_doc) );
	if ( ScColorEngine::isOutOfGamut(orig, m_doc) )
		paintAlert(alertIcon, imageN, 2, 2, false);
	Farbe = orig;

	resize( 498, 306 );
	setWindowTitle( tr( "Edit Color" ) );
	setWindowIcon(IconManager::instance()->loadIcon("AppIcon.png"));

	setupUi(this);
	ColorMap->setDoc(doc);
	ColorName->setText(name);
	ColorSwatch->setPixmapType(ColorListBox::fancyPixmap);
	
	ComboBox1->addItem( tr( "CMYK" ) );
	ComboBox1->addItem( tr( "RGB" ) );
	ComboBox1->addItem( tr( "Web Safe RGB" ) );
	ComboBox1->addItem( tr( "Lab" ) );
	ComboBox1->addItem( tr( "HLC" ) );

	Separations->setChecked(orig.isSpotColor());

	OldC->setPixmap( imageA );
	NewC->setPixmap( imageN );

	buttonOK->setText(CommonStrings::tr_OK);
	buttonOK->setDefault( true );
	buttonCancel->setText(CommonStrings::tr_Cancel);

	hsvSelector = Swatches->addTopLevelItem( tr( "Color Map" ) );
	hsvSelector->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
	csm.findPaletteLocations();
	systemSwatches = Swatches->addTopLevelItem( tr("Scribus Swatches"));
	csm.findPalettes(systemSwatches);
	Swatches->addSubItem("Scribus Small", systemSwatches);
	systemSwatches->setExpanded(true);
	userSwatches = Swatches->addTopLevelItem( tr("User Swatches"));
	csm.findUserPalettes(userSwatches);
	customColSet = csm.userPaletteNames();
	userSwatches->setExpanded(true);
	// Swatches combo uses elided text strings, so we cannot
	// set default combo item in constructor: at that point
	// the swatch combo widget does not know its width yet.
	// We set it in the dialog showEvent().
	// Swatches->setCurrentComboItem( tr( "Color Map" ));

	slidersLayout->setSpacing(5);
	slidersLayout->setMargin(0);

	CyanSp->setNewUnit(0);
	CyanSp->setMinimum(0);
	CyanSp->setMaximum(100);
	CyanSp->setSuffix( tr(" %"));

	CyanSL->setAutoFillBackground(true);
	CyanSL->setMinimumSize(QSize(255, 16));
	CyanSL->setMinimum(0);
	CyanSL->setMaximum(100);
	CyanSL->setPalette(sliderPix(180));

	MagentaSp->setNewUnit(0);
	MagentaSp->setMinimum(0);
	MagentaSp->setMaximum(100);
	MagentaSp->setSuffix( tr(" %"));

	MagentaSL->setAutoFillBackground(true);
	MagentaSL->setMinimumSize(QSize(255, 16));
	MagentaSL->setMinimum(0);
	MagentaSL->setMaximum(100);
	MagentaSL->setPalette(sliderPix(300));

	YellowSp->setNewUnit(0);
	YellowSp->setMinimum(0);
	YellowSp->setMaximum(100);
	YellowSp->setSuffix( tr(" %"));

	YellowSL->setAutoFillBackground(true);
	YellowSL->setMinimumSize(QSize(255, 16));
	YellowSL->setMinimum(0);
	YellowSL->setMaximum(100);
	YellowSL->setPalette(sliderPix(60));

	BlackSp->setNewUnit(0);
	BlackSp->setMinimum(0);
	BlackSp->setMaximum(100);
	BlackSp->setSuffix( tr(" %"));

	BlackSL->setAutoFillBackground(true);
	BlackSL->setMinimumSize(QSize(255, 16));
	BlackSL->setMinimum(0);
	BlackSL->setMaximum(100);
	BlackSL->setPalette(sliderBlack());

	if (orig.getColorModel () == colorModelCMYK)
	{
		double ccd, cmd, cyd, ckd;
		CMYKColorF cmyk;
		ScColorEngine::getCMYKValues(orig, m_doc, cmyk);
		ccd = cmyk.c * 100.0;
		cmd = cmyk.m * 100.0;
		cyd = cmyk.y * 100.0;
		ckd = cmyk.k * 100.0;
		CyanSp->setValue(ccd);
		CyanSL->setValue(qRound(ccd * 1000));
		MagentaSp->setValue(cmd);
		MagentaSL->setValue(qRound(cmd * 1000));
		YellowSp->setValue(cyd);
		YellowSL->setValue(qRound(cyd * 1000));
		BlackSp->setValue(ckd);
		BlackSL->setValue(qRound(ckd * 1000));
		BlackComp = qRound(cmyk.k * 255.0);
	}
	int h, s, v;
	ScColorEngine::getRGBColor(orig, m_doc).getHsv(&h, &s, &v);
	ColorMap->setFixedWidth(180);
	ColorMap->drawPalette(v);
	ColorMap->setMark(h, s);
	Fnam = name;
	ColorName->selectAll();
	ColorName->setFocus();
	TabStack->setCurrentIndex(0);
	setFixedSize(minimumSizeHint());
	setContextMenuPolicy(Qt::CustomContextMenu);
	if (orig.getColorModel () == colorModelRGB)
	{
		ComboBox1->setCurrentIndex(1);
		selModel ( tr( "RGB" ));
	}
	else if (orig.getColorModel() == colorModelCMYK)
	{
		ComboBox1->setCurrentIndex(0);
		selModel ( tr( "CMYK" ));
	}
	else if (orig.getColorModel() == colorModelLab)
	{
		ComboBox1->setCurrentIndex(3);
		selModel ( tr( "Lab" ));
	}
	isRegistration = Farbe.isRegistrationColor();
	if (Farbe.isRegistrationColor())
	{
		ComboBox1->setEnabled(false);
		Separations->setEnabled(false);
	}
	// signals and slots connections
//	Regist->setToolTip( "<qt>" + tr( "Choosing this will enable printing this on all plates. Registration colors are used for printer marks such as crop marks, registration marks and the like. These are not typically used in the layout itself." ) + "</qt>");
	Separations->setToolTip( "<qt>" + tr( "Choosing this will make this color a spot color, thus creating another spot when creating plates or separations. This is used most often when a logo or other color needs exact representation or cannot be replicated with CMYK inks. Metallic and fluorescent inks are good examples which cannot be easily replicated with CMYK inks." ) + "</qt>");
	connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( buttonOK, SIGNAL( clicked() ), this, SLOT( leave() ) );
	connect( CyanSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	connect( MagentaSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	connect( YellowSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	connect( BlackSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	connect( CyanSL, SIGNAL( valueChanged(int) ), this, SLOT( setValueS(int) ) );
	connect( MagentaSL, SIGNAL( valueChanged(int) ), this, SLOT( setValueS(int) ) );
	connect( YellowSL, SIGNAL( valueChanged(int) ), this, SLOT( setValueS(int) ) );
	connect( BlackSL, SIGNAL( valueChanged(int) ), this, SLOT( setValueS(int) ) );
	connect( CyanSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
	connect( MagentaSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
	connect( YellowSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
	connect( BlackSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
	connect( ColorMap, SIGNAL( ColorVal(int, int, bool)), this, SLOT( setColor2(int, int, bool)));
	connect( ComboBox1, SIGNAL(activated(const QString&)), this, SLOT(selModel(const QString&)));
//	connect( Swatches, SIGNAL(activated(int)), this, SLOT(selSwatch(int)));
	connect(Swatches, SIGNAL(activated(const QString &)), this, SLOT(selSwatch()));
	connect(ColorSwatch, SIGNAL(itemClicked(int) ), this, SLOT( selFromSwatch(int) ) );
	connect(Separations, SIGNAL(clicked()), this, SLOT(setSpot()));
//	connect(Regist, SIGNAL(clicked()), this, SLOT(setRegist()));
	connect(this, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(slotRightClick()));
	layout()->activate();
}

QString CMYKChoose::colorName()
{
	return ColorName->text();
}

bool CMYKChoose::isSpotColor()
{
	return Separations->isChecked();
}

void CMYKChoose::setValSLiders(double value)
{
	if (CyanSp == sender())
		CyanSL->setValue(value * 1000);
	if (MagentaSp == sender())
		MagentaSL->setValue(value * 1000);
	if (YellowSp == sender())
		YellowSL->setValue(value * 1000);
	if (BlackSp == sender())
		BlackSL->setValue(value * 1000);
}

void CMYKChoose::slotRightClick()
{
	QMenu *pmen = new QMenu();
	QAction* dynAct;
	if (dynamic)
		dynAct = pmen->addAction( tr("Static Color Bars"));
	else
		dynAct = pmen->addAction( tr("Dynamic Color Bars"));
	connect(dynAct, SIGNAL(triggered()), this, SLOT(toggleSL()));
	pmen->exec(QCursor::pos());
	delete pmen;
}

void CMYKChoose::setValueS(int val)
{
	disconnect( CyanSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	disconnect( MagentaSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	disconnect( YellowSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	disconnect( BlackSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	if (CyanSL == sender())
		CyanSp->setValue(val / 1000.0);
	if (MagentaSL == sender())
		MagentaSp->setValue(val / 1000.0);
	if (YellowSL == sender())
		YellowSp->setValue(val / 1000.0);
	if (BlackSL == sender())
		BlackSp->setValue(val / 1000.0);
	setColor();
	connect( CyanSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	connect( MagentaSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	connect( YellowSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	connect( BlackSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
}

void CMYKChoose::toggleSL()
{
	dynamic = !dynamic;
	CyanSL->setPalette(sliderPix(Farbe.getColorModel() == colorModelCMYK ? 180 : 0));
	MagentaSL->setPalette(sliderPix(Farbe.getColorModel() == colorModelCMYK? 300 : 120));
	YellowSL->setPalette(sliderPix(Farbe.getColorModel() == colorModelCMYK? 60 : 240));
	if (Farbe.getColorModel() == colorModelCMYK)
		BlackSL->setPalette(sliderBlack());
}

QPalette CMYKChoose::sliderPix(int farbe)
{
	RGBColor rgb;
	CMYKColor cmyk;
	QImage image0 = QImage(255, 10, QImage::Format_ARGB32);
	QPainter p;
	p.begin(&image0);
	p.setPen(Qt::NoPen);
	int r, g, b, c, m, y, k;
	QColor tmp;
	for (int x = 0; x < 255; x += 5)
	{
		if (Farbe.getColorModel() == colorModelCMYK)
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
		else if (Farbe.getColorModel() == colorModelRGB)
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
		else if (Farbe.getColorModel() == colorModelLab)
		{
			double L, a, b;
			double val = static_cast<double>(x) / 255.0;
			Farbe.getLab(&L, &a, &b);
			if (isHLC)
			{
				QLineF lin;
				lin.setP1(QPointF(0.0, 0.0));
				lin.setP2(QPointF(a, b));
				double H = lin.angle();
				double C = lin.length();
				double tmpA, tmpB;
				if (dynamic)
				{
					switch (farbe)
					{
					case 0:
						lin = QLineF::fromPolar(C, -360 * val);
						tmpA = lin.p2().x();
						tmpB = lin.p2().y();
						tmp = ScColorEngine::getDisplayColorGC(ScColor(L, tmpA, tmpB), m_doc);
						break;
					case 120:
						tmp = ScColorEngine::getDisplayColorGC(ScColor(100 * val, a, b), m_doc);
						break;
					case 240:
						lin = QLineF::fromPolar(128 * val, H);
						tmpA = lin.p2().x();
						tmpB = lin.p2().y();
						tmp = ScColorEngine::getDisplayColorGC(ScColor(L, tmpA, tmpB), m_doc);
						break;
					}
					p.setBrush(tmp);
				}
				else
				{
					switch (farbe)
					{
					case 0:
						lin = QLineF::fromPolar(128, -360 * val);
						tmpA = lin.p2().x();
						tmpB = lin.p2().y();
						tmp = ScColorEngine::getDisplayColorGC(ScColor(100.0, tmpA, tmpB), m_doc);
						break;
					case 120:
						tmp = ScColorEngine::getDisplayColorGC(ScColor(100 * val, 0.0, 0.0), m_doc);
						break;
					case 240:
						lin = QLineF::fromPolar(128 * val, 0);
						tmpA = lin.p2().x();
						tmpB = lin.p2().y();
						tmp = ScColorEngine::getDisplayColorGC(ScColor(100.0, tmpA, tmpB), m_doc);
						break;
					}
					p.setBrush(tmp);
				}
			}
			else
			{
				if (dynamic)
				{
					switch (farbe)
					{
					case 0:
						tmp = ScColorEngine::getDisplayColorGC(ScColor(100 * val, a, b), m_doc);
						break;
					case 120:
						tmp = ScColorEngine::getDisplayColorGC(ScColor(L, 256 * val - 128.0, b), m_doc);
						break;
					case 240:
						tmp = ScColorEngine::getDisplayColorGC(ScColor(L, a, 256 * val - 128.0), m_doc);
						break;
					}
					p.setBrush(tmp);
				}
				else
				{
					switch (farbe)
					{
					case 0:
						tmp = ScColorEngine::getDisplayColorGC(ScColor(100 * val, 0.0, 0.0), m_doc);
						break;
					case 120:
						tmp = ScColorEngine::getDisplayColorGC(ScColor(100.0, 256 * val - 128.0, 0.0), m_doc);
						break;
					case 240:
						tmp = ScColorEngine::getDisplayColorGC(ScColor(100.0, 0.0, 256 * val - 128.0), m_doc);
						break;
					}
				}
				p.setBrush(tmp);
			}
		}
		p.drawRect(x, 0, 5, 10);
	}
	p.end();
	QPalette pal;
	pal.setBrush(QPalette::Window, QBrush(image0));
	return pal;
}

QPalette CMYKChoose::sliderBlack()
{
	QImage image0 = QImage(255, 10, QImage::Format_ARGB32);
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
	QPalette pal;
	pal.setBrush(QPalette::Window, QBrush(image0));
	return pal;
}

void CMYKChoose::selSwatch()
{
	QTreeWidgetItem *c = Swatches->currentItem();
	if (c == hsvSelector)
		TabStack->setCurrentIndex(0);
	else
	{
		CurrSwatch.clear();
		QString pfadC2 = "";
		QString txt = c->data(0, Qt::UserRole).toString() + "/" + c->text(0);
		if (!customColSet.contains(txt))
			pfadC2 = csm.paletteFileFromName(txt);
		else
			pfadC2 = csm.userPaletteFileFromName(txt);
		if (importColorsFromFile(pfadC2, CurrSwatch))
		{
			CurrSwatch.insert("White", ScColor(0, 0, 0, 0));
			CurrSwatch.insert("Black", ScColor(0, 0, 0, 255));
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
		ColorSwatch->setColors(CurrSwatch, false);
		ColorSwatch->setCurrentRow(0);
		TabStack->setCurrentIndex(1);
	}
}

void CMYKChoose::setSpot()
{
	QSignalBlocker comboBox1Blocker(ComboBox1);
	if (Separations->isChecked())
	{
//		Commented out to allow RGB Spot-Colors
//		ComboBox1->setCurrentIndex( 0 );
//		selModel( tr("CMYK"));
		if (ComboBox1->currentIndex() == 2) // Web Safe RGB
		{
			ComboBox1->setCurrentIndex(1);
			selModel( tr("RGB"));
		}
	}
}

void CMYKChoose::selModel(const QString& mod)
{
	int h, s, v;

	disconnect( CyanSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	disconnect( MagentaSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	disconnect( YellowSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	disconnect( BlackSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	disconnect( CyanSL, SIGNAL( valueChanged(int) ), this, SLOT( setValueS(int) ) );
	disconnect( MagentaSL, SIGNAL( valueChanged(int) ), this, SLOT( setValueS(int) ) );
	disconnect( YellowSL, SIGNAL( valueChanged(int) ), this, SLOT( setValueS(int) ) );
	disconnect( BlackSL, SIGNAL( valueChanged(int) ), this, SLOT( setValueS(int) ) );
	disconnect( CyanSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
	disconnect( MagentaSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
	disconnect( YellowSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
	disconnect( BlackSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
	isHLC = false;
	if (mod == tr("CMYK"))
	{
		Wsave = false;
		CyanSL->setMaximum( 100 * 1000.0);
		CyanSL->setMinimum( 0 * 1000.0 );
		CyanSL->setSingleStep(1 * 1000.0);
		CyanSL->setPageStep(10 * 1000.0);

		MagentaSL->setMaximum( 100 * 1000.0 );
		MagentaSL->setMinimum( 0 * 1000.0 );
		MagentaSL->setSingleStep(1 * 1000.0);
		MagentaSL->setPageStep(10 * 1000.0);

		YellowSL->setMaximum( 100 * 1000.0 );
		YellowSL->setMinimum( 0 * 1000.0 );
		YellowSL->setSingleStep(1 * 1000.0);
		YellowSL->setPageStep(10 * 1000.0);

		BlackSL->setMaximum( 100 * 1000.0);
		BlackSL->setMinimum( 0 * 1000.0);
		BlackSL->setSingleStep(1 * 1000.0);
		BlackSL->setPageStep(10 * 1000.0);

		CyanSp->setMaximum( 100 );
		CyanSp->setMinimum( 0 );
		CyanSp->setDecimals(1);
		CyanSp->setSingleStep(1);
		CyanSp->setSuffix( tr(" %"));

		MagentaSp->setMaximum( 100);
		MagentaSp->setMinimum( 0 );
		MagentaSp->setDecimals(1);
		MagentaSp->setSingleStep(1);
		MagentaSp->setSuffix( tr(" %"));

		YellowSp->setMaximum( 100 );
		YellowSp->setMinimum( 0 );
		YellowSp->setDecimals(1);
		YellowSp->setSingleStep(1);
		YellowSp->setSuffix( tr(" %"));

		BlackSp->setDecimals(1);
		
		CyanT->setText( tr("C:"));
		MagentaT->setText( tr("M:"));
		YellowT->setText( tr("Y:"));
		BlackSL->show();
		BlackSp->show();
		BlackT->show();
		if (Farbe.getColorModel() != colorModelCMYK)
			Farbe = ScColorEngine::convertToModel(Farbe, m_doc, colorModelCMYK);
		CyanSL->setPalette(sliderPix(180));
		MagentaSL->setPalette(sliderPix(300));
		YellowSL->setPalette(sliderPix(60));
		BlackSL->setPalette(sliderBlack());
		ScColorEngine::getRGBColor(Farbe, m_doc).getHsv(&h, &s, &v);
		setValues();
		ColorMap->drawMode = 0;
		ColorMap->drawPalette(v);
		ColorMap->setMark(h, s);
	}
	else if ((mod == tr("Web Safe RGB")) || (mod == tr("RGB")))
	{
		Wsave = false;
		CyanSL->setMaximum( 255 * 1000.0 );
		CyanSL->setMinimum( 0 * 1000.0 );
		CyanSL->setSingleStep(1 * 1000.0);
		CyanSL->setPageStep(1 * 1000.0);

		MagentaSL->setMaximum( 255 * 1000.0 );
		MagentaSL->setMinimum( 0 * 1000.0 );
		MagentaSL->setSingleStep(1 * 1000.0);
		MagentaSL->setPageStep(1 * 1000.0);

		YellowSL->setMaximum( 255 * 1000.0 );
		YellowSL->setMinimum( 0 * 1000.0 );
		YellowSL->setSingleStep(1 * 1000.0);
		YellowSL->setPageStep(1 * 1000.0);

		CyanSp->setSingleStep(1);
		CyanSp->setMaximum( 255 );
		CyanSp->setMinimum( 0 );
		CyanSp->setDecimals(0);
		CyanSp->setSuffix("");

		MagentaSp->setSingleStep(1);
		MagentaSp->setMaximum( 255 );
		MagentaSp->setMinimum( 0 );
		MagentaSp->setDecimals(0);
		MagentaSp->setSuffix("");

		YellowSp->setSingleStep(1);
		YellowSp->setMaximum( 255 );
		YellowSp->setMinimum( 0 );
		YellowSp->setDecimals(0);
		YellowSp->setSuffix("");
		
		CyanT->setText( tr("R:"));
		MagentaT->setText( tr("G:"));
		YellowT->setText( tr("B:"));
		BlackSL->hide();
		BlackSp->hide();
		BlackT->hide();
		if (mod == tr("Web Safe RGB"))
		{
			Wsave = true;
			CyanSL->setSingleStep(51 * 1000.0);
			MagentaSL->setSingleStep(51 * 1000.0);
			YellowSL->setSingleStep(51 * 1000.0);
			CyanSL->setPageStep(51 * 1000.0);
			MagentaSL->setPageStep(51 * 1000.0);
			YellowSL->setPageStep(51 * 1000.0);
			CyanSp->setSingleStep(51);
			MagentaSp->setSingleStep(51);
			YellowSp->setSingleStep(51);
		}
		if (Farbe.getColorModel() != colorModelRGB)
			Farbe = ScColorEngine::convertToModel(Farbe, m_doc, colorModelRGB);
		CyanSL->setPalette(sliderPix(0));
		MagentaSL->setPalette(sliderPix(120));
		YellowSL->setPalette(sliderPix(240));
		ScColorEngine::getRGBColor(Farbe, m_doc).getHsv(&h, &s, &v);
		setValues();
		ColorMap->drawMode = 0;
		ColorMap->drawPalette(v);
		ColorMap->setMark(h, s);
	}
	else if (mod == tr("Lab"))
	{
		Wsave = false;
		CyanSL->setSingleStep(1 * 1000.0);
		CyanSL->setPageStep(10 * 1000.0);
		CyanSL->setMaximum( 100 * 1000.0 );
		CyanSL->setMinimum( 0 * 1000.0 );
		MagentaSL->setSingleStep(1 * 1000.0);
		MagentaSL->setPageStep(10 * 1000.0);
		MagentaSL->setMaximum( 128 * 1000.0 );
		MagentaSL->setMinimum( -128 * 1000.0 );
		YellowSL->setSingleStep(1 * 1000.0);
		YellowSL->setPageStep(10 * 1000.0);
		YellowSL->setMaximum( 128 * 1000.0 );
		YellowSL->setMinimum( -128 * 1000.0 );

		CyanSp->setDecimals(2);
		CyanSp->setSingleStep(1);
		CyanSp->setMaximum( 100 );
		CyanSp->setSuffix( tr(""));

		MagentaSp->setDecimals(2);
		MagentaSp->setSingleStep(1);
		MagentaSp->setMaximum( 128);
		MagentaSp->setMinimum( -128 );
		MagentaSp->setSuffix("");

		YellowSp->setDecimals(2);
		YellowSp->setMaximum( 128 );
		YellowSp->setMinimum( -128 );
		YellowSp->setSingleStep(1);
		YellowSp->setSuffix("");

		CyanT->setText( tr("L:"));
		MagentaT->setText( tr("a:"));
		YellowT->setText( tr("b:"));

		BlackSL->hide();
		BlackSp->hide();
		BlackT->hide();
		if (Farbe.getColorModel() != colorModelLab)
			Farbe = ScColorEngine::convertToModel(Farbe, m_doc, colorModelLab);
		CyanSL->setPalette(sliderPix(0));
		MagentaSL->setPalette(sliderPix(120));
		YellowSL->setPalette(sliderPix(240));
		setValues();
		ColorMap->drawMode = 1;
		ColorMap->drawPalette(CyanSp->value() * 2.55);
		ColorMap->setMark(MagentaSp->value(), YellowSp->value());
	}
	else if (mod == tr("HLC"))
	{
		Wsave = false;
		CyanSL->setSingleStep(1 * 1000.0);
		CyanSL->setPageStep(10 * 1000.0);
		CyanSL->setMaximum( 360 * 1000.0 );
		CyanSL->setMinimum( 0 * 1000.0 );
		MagentaSL->setSingleStep(1 * 1000.0);
		MagentaSL->setPageStep(10 * 1000.0);
		MagentaSL->setMaximum( 100 * 1000.0 );
		MagentaSL->setMinimum( 0 * 1000.0 );
		YellowSL->setSingleStep(1 * 1000.0);
		YellowSL->setPageStep(10 * 1000.0);
		YellowSL->setMaximum( 128 * 1000.0 );
		YellowSL->setMinimum( 0 * 1000.0 );

		CyanSp->setDecimals(2);
		CyanSp->setSingleStep(1);
		CyanSp->setMaximum( 360 );
		CyanSp->setSuffix( tr(""));

		MagentaSp->setDecimals(2);
		MagentaSp->setSingleStep(1);
		MagentaSp->setMaximum( 100);
		MagentaSp->setMinimum( 0 );
		MagentaSp->setSuffix("");

		YellowSp->setDecimals(2);
		YellowSp->setMaximum( 128 );
		YellowSp->setMinimum( 0 );
		YellowSp->setSingleStep(1);
		YellowSp->setSuffix("");

		CyanT->setText( tr("H:"));
		MagentaT->setText( tr("L:"));
		YellowT->setText( tr("C:"));

		BlackSL->hide();
		BlackSp->hide();
		BlackT->hide();
		if (Farbe.getColorModel() != colorModelLab)
			Farbe = ScColorEngine::convertToModel(Farbe, m_doc, colorModelLab);
		isHLC = true;
		CyanSL->setPalette(sliderPix(0));
		MagentaSL->setPalette(sliderPix(120));
		YellowSL->setPalette(sliderPix(240));
		setValues();
		ColorMap->drawMode = 2;
		double L, a, b;
		Farbe.getLab(&L, &a, &b);
		ColorMap->drawPalette(L * 2.55);
		ColorMap->setMark(a, b);
	}
	imageN.fill( ScColorEngine::getDisplayColor(Farbe, m_doc) );
	if (ScColorEngine::isOutOfGamut(Farbe, m_doc))
		paintAlert(alertIcon, imageN, 2, 2, false);
	NewC->setPixmap( imageN );
	NewC->setToolTip( "<qt>" + tr( "If color management is enabled, an exclamation mark indicates that the color may be outside of the color gamut of the current printer profile selected. What this means is the color may not print exactly as indicated on screen. More hints about gamut warnings are in the online help under Color Management." ) + "</qt>");
	OldC->setToolTip( "<qt>" + tr( "If color management is enabled, an exclamation mark indicates that the color may be outside of the color gamut of the current printer profile selected. What this means is the color may not print exactly as indicated on screen. More hints about gamut warnings are in the online help under Color Management." ) + "</qt>");

	connect( CyanSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	connect( MagentaSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	connect( YellowSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	connect( BlackSp, SIGNAL( valueChanged(double) ), this, SLOT( setValSLiders(double) ) );
	connect( CyanSL, SIGNAL( valueChanged(int) ), this, SLOT( setValueS(int) ) );
	connect( MagentaSL, SIGNAL( valueChanged(int) ), this, SLOT( setValueS(int) ) );
	connect( YellowSL, SIGNAL( valueChanged(int) ), this, SLOT( setValueS(int) ) );
	connect( BlackSL, SIGNAL( valueChanged(int) ), this, SLOT( setValueS(int) ) );
	connect( CyanSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
	connect( MagentaSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
	connect( YellowSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
	connect( BlackSL, SIGNAL( valueChanged(int) ), this, SLOT( setColor() ) );
}

void CMYKChoose::setColor()
{
	int    h, s, v;
	double c, m, y;
	double k = 0;
	double L, a, b;
	ScColor tmp;
	if (Farbe.getColorModel() == colorModelCMYK)
	{
		c = CyanSp->value() / 100.0;
		m = MagentaSp->value() / 100.0;
		y = YellowSp->value() / 100.0;
		k = BlackSp->value() / 100.0;
		tmp.setColorF(c, m, y, k);
		Farbe = tmp;
		if (dynamic)
		{
			CyanSL->setPalette(sliderPix(180));
			MagentaSL->setPalette(sliderPix(300));
			YellowSL->setPalette(sliderPix(60));
			BlackSL->setPalette(sliderBlack());
		}
		BlackComp = qRound(k * 255.0);
		ScColorEngine::getRGBColor(tmp, m_doc).getHsv(&h, &s, &v);
		ColorMap->drawPalette(v);
		ColorMap->setMark(h, s);
	}
	else if (Farbe.getColorModel() == colorModelRGB)
	{
		int ic = qRound(CyanSp->value());
		int im = qRound(MagentaSp->value());
		int iy = qRound(YellowSp->value());
		int ik = qRound(BlackSp->value());
		if (Wsave)
		{
			ic = ic / 51 * 51;
			im = im / 51 * 51;
			iy = iy / 51 * 51;
			blockSignals(true);
			CyanSp->setValue(ic);
			MagentaSp->setValue(im);
			YellowSp->setValue(iy);
			CyanSL->setValue(ic * 1000.0);
			MagentaSL->setValue(im * 1000.0);
			YellowSL->setValue(iy * 1000.0);
			blockSignals(false);
		}
		c = ic / 255.0;
		m = im / 255.0;
		y = iy / 255.0;
		k = ik / 255.0;
		tmp.setRgbColorF(c, m, y);
		QColor tmp2 = ScColorEngine::getRGBColor(tmp, m_doc);
		tmp2.getHsv(&h, &s, &v);
		BlackComp = 255 - v;
		Farbe = tmp;
		if (dynamic)
		{
			CyanSL->setPalette(sliderPix(0));
			MagentaSL->setPalette(sliderPix(120));
			YellowSL->setPalette(sliderPix(240));
		}
		BlackComp = qRound(k * 255.0);
		ScColorEngine::getRGBColor(tmp, m_doc).getHsv(&h, &s, &v);
		ColorMap->drawPalette(v);
		ColorMap->setMark(h, s);
	}
	else if (Farbe.getColorModel() == colorModelLab)
	{
		double Lalt;
		Farbe.getLab(&Lalt, &a, &b);
		if (isHLC)
		{
			L = MagentaSp->value();
			double cv = 360 - CyanSp->value();
			double yv = YellowSp->value();
			QLineF lin = QLineF::fromPolar(yv, cv);
			a = lin.p2().x();
			b = lin.p2().y();
		}
		else
		{
			L = CyanSp->value();
			a = MagentaSp->value();
			b = YellowSp->value();
		}
		tmp.setLabColor(L, a, b);
		Farbe = tmp;
		if (dynamic)
		{
			CyanSL->setPalette(sliderPix(0));
			MagentaSL->setPalette(sliderPix(120));
			YellowSL->setPalette(sliderPix(240));
		}
		BlackComp = qRound(L * 2.55);
		if (L != Lalt)
			ColorMap->drawPalette(L * 2.55);
		ColorMap->setMark(a, b);
	}
	imageN.fill(ScColorEngine::getDisplayColor(tmp, m_doc) );
	if ( ScColorEngine::isOutOfGamut(tmp, m_doc) )
		paintAlert(alertIcon, imageN, 2, 2, false);
	NewC->setPixmap( imageN );
}

void CMYKChoose::setColor2(int h, int s, bool ende)
{
	ScColor tmp;
	if (Farbe.getColorModel() == colorModelLab)
	{
		if (isHLC)
			tmp = ScColor(MagentaSp->value(), static_cast<double>(h), static_cast<double>(s));
		else
			tmp = ScColor(CyanSp->value(), static_cast<double>(h), static_cast<double>(s));
	}
	else
	{
		int r, g, b;
		int ih = qMax(0, qMin(h, 359));
		int is = qMax(0, qMin(255 - s, 255));
		int iv = 255 - BlackComp;
		QColor tm = QColor::fromHsv(ih, is, iv);
		tm.getRgb(&r, &g, &b);
		tmp.fromQColor(tm);
		if (Farbe.getColorModel() == colorModelCMYK)
		{
			CMYKColorF cmyk;
			ScColorEngine::getCMYKValues(tmp, m_doc, cmyk);
			tmp.setColorF(cmyk.c, cmyk.m, cmyk.y, cmyk.k);
		}
	}
	imageN.fill( ScColorEngine::getDisplayColor(tmp, m_doc) );
	if (ScColorEngine::isOutOfGamut(tmp, m_doc))
		paintAlert(alertIcon, imageN, 2, 2, false);
	NewC->setPixmap( imageN );
	Farbe = tmp;
	if (ende)
		setValues();
}

void CMYKChoose::selFromSwatch(int itemIndex)
{
	QSignalBlocker comboBox1Blocker(ComboBox1);
	QString colorName = ColorSwatch->text(itemIndex);
	ScColor tmp = CurrSwatch[colorName];
	if (isRegistration)
	{
		if (tmp.getColorModel() != colorModelCMYK)
			tmp = ScColorEngine::convertToModel(tmp, m_doc, colorModelCMYK);
		selModel( tr("CMYK"));
	}
	else
	{
		if (tmp.getColorModel() == colorModelCMYK)
		{
			ComboBox1->setCurrentIndex( 0 );
			selModel( tr("CMYK"));
		}
		else if (tmp.getColorModel() == colorModelRGB)
		{
			ComboBox1->setCurrentIndex( 1 );
			selModel( tr("RGB"));
		}
		else
		{
			ComboBox1->setCurrentIndex( 3 );
			selModel( tr("Lab"));
		}
	}
	imageN.fill( ScColorEngine::getDisplayColor(tmp, m_doc) );
	if ( ScColorEngine::isOutOfGamut(tmp, m_doc) )
		paintAlert(alertIcon, imageN, 2, 2, false);
	NewC->setPixmap( imageN );
	Farbe = tmp;
	setValues();
	Separations->setChecked(tmp.isSpotColor());
	if ((isNew) && (!ColorName->isModified()))
		ColorName->setText(colorName);
}

void CMYKChoose::setValues()
{
	QSignalBlocker cyanSpBlocker(CyanSp);
	QSignalBlocker cyanSLBlocker(CyanSL);
	QSignalBlocker magentaSpBlocker(MagentaSp);
	QSignalBlocker magentaSLBlocker(MagentaSL);
	QSignalBlocker yellowSpBlocker(YellowSp);
	QSignalBlocker yellowSLBlocker(YellowSL);
	QSignalBlocker blackSpBlocker(BlackSp);
	QSignalBlocker blackSLBlocker(BlackSL);

	if (Farbe.getColorModel() == colorModelCMYK)
	{
		CMYKColorF cmyk;
		double cc, cm, cy, ck;
		ScColorEngine::getCMYKValues(Farbe, m_doc, cmyk);
		cmyk.getValues(cc, cm, cy, ck);
		CyanSp->setValue(cc * 100.0);
		CyanSL->setValue(qRound(cc * 100.0 * 1000.0));
		MagentaSp->setValue(cm * 100.0);
		MagentaSL->setValue(qRound(cm * 100.0 * 1000.0));
		YellowSp->setValue(cy * 100.0);
		YellowSL->setValue(qRound(cy * 100.0 * 1000.0));
		BlackSp->setValue(ck * 100.0);
		BlackSL->setValue(qRound(ck * 100.0 * 1000.0));
		if (dynamic)
		{
			CyanSL->setPalette(sliderPix(180));
			MagentaSL->setPalette(sliderPix(300));
			YellowSL->setPalette(sliderPix(60));
			BlackSL->setPalette(sliderBlack());
		}
	}
	else if (Farbe.getColorModel() == colorModelRGB)
	{
		RGBColor rgb;
		int r, g, b;
		ScColorEngine::getRGBValues(Farbe, m_doc, rgb);
		rgb.getValues(r, g, b);
		CyanSp->setValue(static_cast<double>(r));
		CyanSL->setValue(r * 1000.0);
		MagentaSp->setValue(static_cast<double>(g));
		MagentaSL->setValue(g * 1000.0);
		YellowSp->setValue(static_cast<double>(b));
		YellowSL->setValue(b * 1000.0);
		int h, s, v;
		ScColorEngine::getRGBColor(Farbe, m_doc).getHsv(&h, &s, &v);
		BlackComp = 255 - v;
		if (dynamic)
		{
			CyanSL->setPalette(sliderPix(0));
			MagentaSL->setPalette(sliderPix(120));
			YellowSL->setPalette(sliderPix(240));
		}
	}
	else if (Farbe.getColorModel() == colorModelLab)
	{
		double L, a, b;
		Farbe.getLab(&L, &a, &b);
		if (isHLC)
		{
			MagentaSp->setValue(L);
			MagentaSL->setValue(L * 1000.0);
			QLineF lin;
			lin.setP1(QPointF(0.0, 0.0));
			lin.setP2(QPointF(a, b));
			CyanSp->setValue(360 - lin.angle());
			CyanSL->setValue(360 - lin.angle() * 1000.0);
			YellowSp->setValue(lin.length());
			YellowSL->setValue(lin.length() * 1000.0);
		}
		else
		{
			CyanSp->setValue(L);
			CyanSL->setValue(L * 1000.0);
			MagentaSp->setValue(a);
			MagentaSL->setValue(a * 1000.0);
			YellowSp->setValue(b);
			YellowSL->setValue(b * 1000.0);
		}
		if (dynamic)
		{
			CyanSL->setPalette(sliderPix(0));
			MagentaSL->setPalette(sliderPix(120));
			YellowSL->setPalette(sliderPix(240));
		}
	}
}

void CMYKChoose::showEvent(QShowEvent * event)
{
	if (!event->spontaneous())
		Swatches->setCurrentComboItem( tr( "Color Map" ));
	QDialog::showEvent(event);
}

void CMYKChoose::leave()
{
	// if condition 10/21/2004 pv #1191 - just be sure that user cannot create "None" color
	if (ColorName->text().isEmpty())
	{
		ScMessageBox::information(this, CommonStrings::trWarning, tr("You cannot create a color without a name.\nPlease give it a name"));
		ColorName->setFocus();
		ColorName->selectAll();
		return;
	}
	if (ColorName->text() == CommonStrings::None || ColorName->text() == CommonStrings::tr_NoneColor)
	{
		ScMessageBox::information(this, CommonStrings::trWarning, tr("You cannot create a color named \"%1\".\nIt is a reserved name for transparent color").arg(ColorName->text()));
		ColorName->setFocus();
		ColorName->selectAll();
		return;
	}
	if ((Fnam != ColorName->text()) || (isNew))
	{
		if (EColors->contains(ColorName->text()))
		{
			ScMessageBox::information(this, CommonStrings::trWarning, tr("The name of the color already exists.\nPlease choose another one."));
			ColorName->selectAll();
			ColorName->setFocus();
			return;
		}
		else
			accept();
	}
	else
		accept();
}
