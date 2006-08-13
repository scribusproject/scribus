/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "effectsdialog.h"
#include "effectsdialog.moc"
#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qtextstream.h>
#include <qwidgetstack.h>
#include <qwidget.h>
#include <qslider.h>
#include <qpainter.h>
#include "colorcombo.h"
#include "cmsettings.h"
#include "sccombobox.h"
#include "scribusdoc.h"
#include "shadebutton.h"
#include "mspinbox.h"
#include "page.h"
#include "util.h"

extern QPixmap loadIcon(QString nam);

KCurve::KCurve(QWidget *parent, const char *name, WFlags f) : QWidget(parent, name, f)
{
	m_grab_point     = FPoint();
	m_dragging = false;
	m_pix = NULL;
	m_pos = 0;
	setMouseTracking(true);
	setPaletteBackgroundColor(Qt::NoBackground);
	setMinimumSize(150, 150);
	m_points.resize(0);
	m_points.addPoint(0.0, 0.0);
	m_points.addPoint(1.0, 1.0);
	setFocusPolicy(QWidget::StrongFocus);
}

KCurve::~KCurve()
{
	if (m_pix) delete m_pix;
}

void KCurve::setPixmap(QPixmap pix)
{
	if (m_pix) delete m_pix;
	m_pix = new QPixmap(pix);
	repaint(false);
}

void KCurve::paintEvent(QPaintEvent *)
{
	int    x, y;
	int    wWidth = width();
	int    wHeight = height();
	x  = 0;
	y  = 0;
	// Drawing selection or all histogram values.
	// A QPixmap is used for enable the double buffering.
	QPixmap pm(size());
	QPainter p1;
	p1.begin(&pm, this);
	//  draw background
	if(m_pix)
	{
		p1.scale(1.0*wWidth/m_pix->width(), 1.0*wHeight/m_pix->height());
		p1.drawPixmap(0, 0, *m_pix);
		p1.resetXForm();
	}
	else
		pm.fill();
	// Draw grid separators.
	p1.setPen(QPen::QPen(Qt::gray, 1, Qt::SolidLine));
	p1.drawLine(wWidth/3, 0, wWidth/3, wHeight);
	p1.drawLine(2*wWidth/3, 0, 2*wWidth/3, wHeight);
	p1.drawLine(0, wHeight/3, wWidth, wHeight/3);
	p1.drawLine(0, 2*wHeight/3, wWidth, 2*wHeight/3);

	// Draw curve.
	double curvePrevVal = getCurveValue(0.0);
	p1.setPen(QPen::QPen(Qt::black, 1, Qt::SolidLine));
	for (x = 0 ; x < wWidth ; x++)
	{
		double curveX;
		double curveVal;
		curveX = (x + 0.5) / wWidth;
//		curveX = x / static_cast<double>(wWidth);
		curveVal = getCurveValue(curveX);
		p1.drawLine(x - 1, wHeight - int(curvePrevVal * wHeight), x,     wHeight - int(curveVal * wHeight));
		curvePrevVal = curveVal;
	}
	p1.drawLine(x - 1, wHeight - int(curvePrevVal * wHeight), x,     wHeight - int(getCurveValue(1.0) * wHeight));
	for (uint dh = 0; dh < m_points.size(); dh++)
	{
		FPoint p = m_points.point(dh);
		if(p == m_grab_point)
		{
			p1.setPen(QPen::QPen(Qt::red, 3, Qt::SolidLine));
			p1.drawEllipse( int(p.x() * wWidth) - 2, wHeight - 2 - int(p.y() * wHeight), 4, 4 );
		}
		else
		{
			p1.setPen(QPen::QPen(Qt::red, 1, Qt::SolidLine));
			p1.drawEllipse( int(p.x() * wWidth) - 3, wHeight - 3 - int(p.y() * wHeight), 6, 6 );
		}
	}
	p1.end();
	bitBlt(this, 0, 0, &pm);
}

void KCurve::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Delete || e->key() == Qt::Key_Backspace)
    {
		if (m_points.size() > 2)
		{
			FPoint closest_point;
			FPoint p = m_points.point(0);
			int pos = 0;
			uint cc =0;
			double distance = 1000; // just a big number
			while(cc < m_points.size())
			{
				if (fabs (m_grab_point.x() - p.x()) < distance)
				{
					distance = fabs(m_grab_point.x() - p.x());
					closest_point = p;
					m_pos = pos;
				}
				cc++;
				p = m_points.point(cc);
				pos++;
			}
			FPointArray cli;
			cli.putPoints(0, m_pos, m_points);
			cli.putPoints(cli.size(), m_points.size()-m_pos-1, m_points, m_pos+1);
			m_points.resize(0);
			m_points = cli.copy();
			m_grab_point = closest_point;
			repaint(false);
			emit modified();
			QWidget::keyPressEvent(e);
		}
	}
}

void KCurve::mousePressEvent ( QMouseEvent * e )
{
	FPoint closest_point = FPoint();
	double distance;
	if (e->button() != Qt::LeftButton)
		return;
	double x = e->pos().x() / (float)width();
	double y = 1.0 - e->pos().y() / (float)height();
	distance = 1000; // just a big number
	FPoint p = m_points.point(0);
	int insert_pos =0;
	int pos = 0;
	uint cc =0;
	while(cc < m_points.size())
	{
		if (fabs (x - p.x()) < distance)
		{
			distance = fabs(x - p.x());
			closest_point = p;
			insert_pos = pos;
		}
		cc++;
		p = m_points.point(cc);
		pos++;
	}
	m_pos = insert_pos;
	m_grab_point = closest_point;
	m_grabOffsetX = m_grab_point.x() - x;
	m_grabOffsetY = m_grab_point.y() - y;
	m_grab_point = FPoint(x + m_grabOffsetX, y + m_grabOffsetY);
	double curveVal = getCurveValue(x);
	if(distance * width() > 5)
	{
		m_dragging = false;
		if(fabs(y - curveVal) * width() > 5)
			return;
		if (m_points.size() < 14)
		{
			if (x > closest_point.x())
				m_pos++;
			FPointArray cli;
			cli.putPoints(0, m_pos, m_points);
			cli.resize(cli.size()+1);
			cli.putPoints(cli.size()-1, 1, x, curveVal);
			cli.putPoints(cli.size(), m_points.size()-m_pos, m_points, m_pos);
			m_points.resize(0);
			m_points = cli.copy();
			m_dragging = true;
			m_grab_point = m_points.point(m_pos);
			m_grabOffsetX = m_grab_point.x() - x;
			m_grabOffsetY = m_grab_point.y() - curveVal;
			m_grab_point = FPoint(x + m_grabOffsetX, curveVal + m_grabOffsetY);
			qApp->setOverrideCursor(QCursor(crossCursor), true);
		}
	}
	else
	{
		if(fabs(y - closest_point.y()) * width() > 5)
			return;
		m_dragging = true;
		qApp->setOverrideCursor(QCursor(crossCursor), true);
	}
	// Determine the leftmost and rightmost points.
	m_leftmost = 0;
	m_rightmost = 1;
	repaint(false);
	emit modified();
}

void KCurve::mouseReleaseEvent ( QMouseEvent * e )
{
	if (e->button() != Qt::LeftButton)
		return;
	qApp->setOverrideCursor(QCursor(ArrowCursor), true);
	m_dragging = false;
	repaint(false);
	emit modified();
}

void KCurve::mouseMoveEvent ( QMouseEvent * e )
{
	double x = e->pos().x() / (float)width();
	double y = 1.0 - e->pos().y() / (float)height();

	if (m_dragging == false)   // If no point is selected set the the cursor shape if on top
	{
		double distance = 1000;
		double ydistance = 1000;
		FPoint p = m_points.point(0);
		uint cc =0;
		while(cc < m_points.size())
		{
			if (fabs (x - p.x()) < distance)
			{
				distance = fabs(x - p.x());
				ydistance = fabs(y - p.y());
			}
			cc++;
			p = m_points.point(cc);
		}
		if (distance * width() > 5 || ydistance * height() > 5)
			qApp->setOverrideCursor(QCursor(ArrowCursor), true);
		else
			qApp->setOverrideCursor(QCursor(crossCursor), true);
	}
	else  // Else, drag the selected point
	{
		qApp->setOverrideCursor(QCursor(crossCursor), true);
		x += m_grabOffsetX;
		y += m_grabOffsetY;
		if (x <= m_leftmost)
			x = m_leftmost + 1E-4; // the addition so we can grab the dot later.
		if(x >= m_rightmost)
			x = m_rightmost - 1E-4;
		if(y > 1.0)
			y = 1.0;
		if(y < 0.0)
			y = 0.0;
		m_grab_point = FPoint(x, y);
		m_points.setPoint( m_pos, m_grab_point);
	}
	repaint(false);
	emit modified();
}

double KCurve::getCurveValue(double x)
{
	return getCurveYValue(m_points, x);
}

FPointArray KCurve::getCurve()
{
	return m_points.copy();
}

void KCurve::setCurve(FPointArray inlist)
{
	m_points.resize(0);
	m_points = inlist.copy();
	repaint(false);
	emit modified();
}

void KCurve::leaveEvent( QEvent * )
{
	qApp->setOverrideCursor(QCursor(ArrowCursor), true);
}

EffectsDialog::EffectsDialog( QWidget* parent, PageItem* item, ScribusDoc* docc ) : QDialog( parent, "EffectsDialog", true, 0 )
{
	EffectsDialogLayout = new QHBoxLayout( this, 10, 5, "EffectsDialogLayout");
	setCaption( tr( "Image Effects" ) );
	setIcon(loadIcon("AppIcon.png"));
	currItem = item;
	effectsList = currItem->effectsInUse;
	doc = docc;
	currentOptions = 0;
	bool mode = false;
	CMSettings cms(docc, "", 0);
	image.LoadPicture(currItem->Pfile, cms, false, false, ScImage::RGBData, 72, &mode);
	int ix = image.width();
	int iy = image.height();
	imageScale = 1.0;
	if ((ix > 220) || (iy > 220))
	{
		double sx = ix / 220.0;
		double sy = iy / 220.0;
		if (sy < sx)
			image.createLowRes(sx);
		else
			image.createLowRes(sy);
		imageScale = QMIN(sx, sy);
	}
	layout16 = new QVBoxLayout( 0, 0, 5, "layout16");
	pixmapLabel1 = new QLabel( this, "pixmapLabel1" );
	pixmapLabel1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, pixmapLabel1->sizePolicy().hasHeightForWidth() ) );
	pixmapLabel1->setMinimumSize( QSize( 220, 220 ) );
	pixmapLabel1->setMaximumSize( QSize( 220, 220 ) );
	pixmapLabel1->setFrameShape( QLabel::StyledPanel );
	pixmapLabel1->setFrameShadow( QLabel::Sunken );
	pixmapLabel1->setScaledContents( false );
	layout16->addWidget( pixmapLabel1 );
	textLabel5 = new QLabel( this, "textLabel5" );
	textLabel5->setText( tr( "Options:" ) );
	layout16->addWidget( textLabel5 );
	optionStack = new QWidgetStack( this, "optionStack" );
	optionStack->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, optionStack->sizePolicy().hasHeightForWidth() ) );
	optionStack->setMinimumSize( QSize( 220, 160 ) );
	optionStack->setFrameShape( QWidgetStack::GroupBoxPanel );
	WStackPage = new QWidget( optionStack, "WStackPage" );
	optionStack->addWidget( WStackPage, 0 );

	WStackPage_2 = new QWidget( optionStack, "WStackPage_2" );
	WStackPageLayout = new QVBoxLayout( WStackPage_2, 5, 5, "WStackPageLayout");
	WStackPageLayout->setAlignment( Qt::AlignTop );
	layout17 = new QHBoxLayout( 0, 0, 5, "layout7");
	textLabel3 = new QLabel( tr( "Color:" ), WStackPage_2, "textLabel3" );
	layout17->addWidget( textLabel3 );

	colData = new ColorCombo(false, WStackPage_2, "colData");
	ColorList::Iterator it;
	for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		colData->insertSmallItem(doc->PageColors[it.key()], it.key());
	layout17->addWidget( colData );
	WStackPageLayout->addLayout( layout17 );

	layout19 = new QHBoxLayout( 0, 0, 5, "layout7");
	textLabel4 = new QLabel( tr( "Shade:" ), WStackPage_2, "textLabel4" );
	layout19->addWidget( textLabel4 );
	shade = new ShadeButton(WStackPage_2);
	shade->setValue(100);
	layout19->addWidget( shade );
	WStackPageLayout->addLayout( layout19 );
	optionStack->addWidget( WStackPage_2, 1 );

	WStackPage_3 = new QWidget( optionStack, "WStackPage_3" );
	WStackPage3Layout = new QVBoxLayout( WStackPage_3, 5, 5, "WStackPageLayout");
	WStackPage3Layout->setAlignment( Qt::AlignTop );
	layout20 = new QHBoxLayout( 0, 0, 5, "layout7");
	textLabel6 = new QLabel( tr( "Brightness:" ), WStackPage_3, "textLabel6" );
	layout20->addWidget( textLabel6, Qt::AlignLeft );
	textLabel7 = new QLabel( "0", WStackPage_3, "textLabel7" );
	layout20->addWidget( textLabel7, Qt::AlignRight );
	WStackPage3Layout->addLayout( layout20 );
	brightnessSlider = new QSlider( WStackPage_3, "Slider1_2" );
	brightnessSlider->setMinValue(-255);
	brightnessSlider->setMaxValue(255);
	brightnessSlider->setValue(0);
	brightnessSlider->setOrientation( QSlider::Horizontal );
	brightnessSlider->setTickmarks( QSlider::Below );
	WStackPage3Layout->addWidget( brightnessSlider );
	optionStack->addWidget( WStackPage_3, 2 );

	WStackPage_4 = new QWidget( optionStack, "WStackPage_4" );
	WStackPage4Layout = new QVBoxLayout( WStackPage_4, 5, 5, "WStackPageLayout");
	WStackPage4Layout->setAlignment( Qt::AlignTop );
	layout21 = new QHBoxLayout( 0, 0, 5, "layout7");
	textLabel8 = new QLabel( tr( "Contrast:" ), WStackPage_4, "textLabel8" );
	layout21->addWidget( textLabel8, Qt::AlignLeft );
	textLabel9 = new QLabel( "0", WStackPage_4, "textLabel9" );
	layout21->addWidget( textLabel9, Qt::AlignRight );
	WStackPage4Layout->addLayout( layout21 );
	contrastSlider = new QSlider( WStackPage_4, "Slider2" );
	contrastSlider->setMinValue(-127);
	contrastSlider->setMaxValue(127);
	contrastSlider->setValue(0);
	contrastSlider->setOrientation( QSlider::Horizontal );
	contrastSlider->setTickmarks( QSlider::Below );
	WStackPage4Layout->addWidget( contrastSlider );
	optionStack->addWidget( WStackPage_4, 3 );

	WStackPage_5 = new QWidget( optionStack, "WStackPage_5" );
	WStackPage5Layout = new QVBoxLayout( WStackPage_5, 5, 5, "WStackPageLayout");
	WStackPage5Layout->setAlignment( Qt::AlignTop );
	layout22 = new QHBoxLayout( 0, 0, 5, "layout7");
	textLabel10 = new QLabel( tr( "Radius:" ), WStackPage_5, "textLabel10" );
	layout22->addWidget( textLabel10 );
	shRadius = new MSpinBox( 0.0, 10.0, WStackPage_5, 1 );
	shRadius->setValue(0);
	layout22->addWidget( shRadius );
	WStackPage5Layout->addLayout( layout22 );
	layout23 = new QHBoxLayout( 0, 0, 5, "layout7");
	textLabel11 = new QLabel( tr("Value:"), WStackPage_5, "textLabel11" );
	layout23->addWidget( textLabel11 );
	shValue = new MSpinBox( 0.0, 5.0, WStackPage_5, 1 );
	shValue->setValue(1.0);
	layout23->addWidget( shValue );
	WStackPage5Layout->addLayout( layout23 );
	optionStack->addWidget( WStackPage_5, 4 );

	WStackPage_6 = new QWidget( optionStack, "WStackPage_6" );
	WStackPage6Layout = new QVBoxLayout( WStackPage_6, 5, 5, "WStackPageLayout");
	WStackPage6Layout->setAlignment( Qt::AlignTop );
	layout24 = new QHBoxLayout( 0, 0, 5, "layout7");
	textLabel12 = new QLabel( tr( "Radius:" ), WStackPage_6, "textLabel10" );
	layout24->addWidget( textLabel12 );
	blRadius = new MSpinBox( 0.0, 30.0, WStackPage_6, 1 );
	blRadius->setValue(0);
	layout24->addWidget( blRadius );
	WStackPage6Layout->addLayout( layout24 );
	optionStack->addWidget( WStackPage_6, 5 );

	WStackPage_7 = new QWidget( optionStack, "WStackPage_4" );
	WStackPage7Layout = new QVBoxLayout( WStackPage_7, 5, 5, "WStackPageLayout");
	WStackPage7Layout->setAlignment( Qt::AlignTop );
	layout26 = new QHBoxLayout( 0, 0, 5, "layout7");
	textLabel14 = new QLabel( tr( "Posterize:" ), WStackPage_7, "textLabel8" );
	layout26->addWidget( textLabel14, Qt::AlignLeft );
	textLabel15 = new QLabel( "0", WStackPage_7, "textLabel9" );
	layout26->addWidget( textLabel15, Qt::AlignRight );
	WStackPage7Layout->addLayout( layout26 );
	solarizeSlider = new QSlider( WStackPage_7, "Slider2" );
	solarizeSlider->setMinValue(1);
	solarizeSlider->setMaxValue(255);
	solarizeSlider->setValue(255);
	solarizeSlider->setOrientation( QSlider::Horizontal );
	solarizeSlider->setTickmarks( QSlider::Below );
	WStackPage7Layout->addWidget( solarizeSlider );
	optionStack->addWidget( WStackPage_7, 6 );

	WStackPage_8 = new QWidget( optionStack, "WStackPage_8" );
	WStackPage8Layout = new QGridLayout( WStackPage_8, 1, 1, 0, 5, "WStackPage8Layout");
	textLabel1d = new QLabel( tr( "Color 1:" ), WStackPage_8, "textLabel1d" );
	WStackPage8Layout->addWidget( textLabel1d, 0, 0 );
	colData1 = new ColorCombo(false, WStackPage_8, "colData1");
	for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		colData1->insertSmallItem(doc->PageColors[it.key()], it.key());
	WStackPage8Layout->addWidget( colData1, 0, 1 );
	shade1 = new ShadeButton(WStackPage_8);
	shade1->setValue(100);
	WStackPage8Layout->addWidget( shade1, 0, 2 );
	textLabel2d = new QLabel( tr( "Color 2:" ), WStackPage_8, "textLabel2d" );
	WStackPage8Layout->addWidget( textLabel2d, 1, 0 );
	colData2 = new ColorCombo(false, WStackPage_8, "colData2");
	for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		colData2->insertSmallItem(doc->PageColors[it.key()], it.key());
	WStackPage8Layout->addWidget( colData2, 1, 1 );
	shade2 = new ShadeButton(WStackPage_8);
	shade2->setValue(100);
	WStackPage8Layout->addWidget( shade2, 1, 2 );
	optionStack->addWidget( WStackPage_8, 7 );

	WStackPage_9 = new QWidget( optionStack, "WStackPage_9" );
	WStackPage9Layout = new QGridLayout( WStackPage_9, 1, 1, 0, 5, "WStackPage9Layout");
	textLabel1t = new QLabel( tr( "Color 1:" ), WStackPage_9, "textLabel1t" );
	WStackPage9Layout->addWidget( textLabel1t, 0, 0 );
	colDatat1 = new ColorCombo(false, WStackPage_9, "colDatat1");
	for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		colDatat1->insertSmallItem(doc->PageColors[it.key()], it.key());
	WStackPage9Layout->addWidget( colDatat1, 0, 1 );
	shadet1 = new ShadeButton(WStackPage_9);
	shadet1->setValue(100);
	WStackPage9Layout->addWidget( shadet1, 0, 2 );
	textLabel2t = new QLabel( tr( "Color 2:" ), WStackPage_9, "textLabel2t" );
	WStackPage9Layout->addWidget( textLabel2t, 1, 0 );
	colDatat2 = new ColorCombo(false, WStackPage_9, "colDatat2");
	for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		colDatat2->insertSmallItem(doc->PageColors[it.key()], it.key());
	WStackPage9Layout->addWidget( colDatat2, 1, 1 );
	shadet2 = new ShadeButton(WStackPage_9);
	shadet2->setValue(100);
	WStackPage9Layout->addWidget( shadet2, 1, 2 );
	textLabel3t = new QLabel( tr( "Color 3:" ), WStackPage_9, "textLabel3t" );
	WStackPage9Layout->addWidget( textLabel3t, 2, 0 );
	colDatat3 = new ColorCombo(false, WStackPage_9, "colDatat3");
	for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		colDatat3->insertSmallItem(doc->PageColors[it.key()], it.key());
	WStackPage9Layout->addWidget( colDatat3, 2, 1 );
	shadet3 = new ShadeButton(WStackPage_9);
	shadet3->setValue(100);
	WStackPage9Layout->addWidget( shadet3, 2, 2 );
	optionStack->addWidget( WStackPage_9, 8 );

	WStackPage_10 = new QWidget( optionStack, "WStackPage_10" );
	WStackPage10Layout = new QGridLayout( WStackPage_10, 1, 1, 0, 5, "WStackPage10Layout");
	textLabel1q = new QLabel( tr( "Color 1:" ), WStackPage_10, "textLabel1q" );
	WStackPage10Layout->addWidget( textLabel1q, 0, 0 );
	colDataq1 = new ColorCombo(false, WStackPage_10, "colDataq1");
	for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		colDataq1->insertSmallItem(doc->PageColors[it.key()], it.key());
	WStackPage10Layout->addWidget( colDataq1, 0, 1 );
	shadeq1 = new ShadeButton(WStackPage_10);
	shadeq1->setValue(100);
	WStackPage10Layout->addWidget( shadeq1, 0, 2 );
	textLabel2q = new QLabel( tr( "Color 2:" ), WStackPage_10, "textLabel2q" );
	WStackPage10Layout->addWidget( textLabel2q, 1, 0 );
	colDataq2 = new ColorCombo(false, WStackPage_10, "colDataq2");
	for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		colDataq2->insertSmallItem(doc->PageColors[it.key()], it.key());
	WStackPage10Layout->addWidget( colDataq2, 1, 1 );
	shadeq2 = new ShadeButton(WStackPage_10);
	shadeq2->setValue(100);
	WStackPage10Layout->addWidget( shadeq2, 1, 2 );
	textLabel3q = new QLabel( tr( "Color 3:" ), WStackPage_10, "textLabel3q" );
	WStackPage10Layout->addWidget( textLabel3q, 2, 0 );
	colDataq3 = new ColorCombo(false, WStackPage_10, "colDataq3");
	for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		colDataq3->insertSmallItem(doc->PageColors[it.key()], it.key());
	WStackPage10Layout->addWidget( colDataq3, 2, 1 );
	shadeq3 = new ShadeButton(WStackPage_10);
	shadeq3->setValue(100);
	WStackPage10Layout->addWidget( shadeq3, 2, 2 );
	textLabel4q = new QLabel( tr( "Color 4:" ), WStackPage_10, "textLabel4q" );
	WStackPage10Layout->addWidget( textLabel4q, 3, 0 );
	colDataq4 = new ColorCombo(false, WStackPage_10, "colDataq4");
	for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		colDataq4->insertSmallItem(doc->PageColors[it.key()], it.key());
	WStackPage10Layout->addWidget( colDataq4, 3, 1 );
	shadeq4 = new ShadeButton(WStackPage_10);
	shadeq4->setValue(100);
	WStackPage10Layout->addWidget( shadeq4, 3, 2 );
	optionStack->addWidget( WStackPage_10, 9 );

	WStackPage_11 = new QWidget( optionStack, "WStackPage_11" );
	WStackPage11Layout = new QVBoxLayout( WStackPage_11, 5, 5, "WStackPageLayout");
	WStackPage11Layout->setAlignment( Qt::AlignTop );
	Kdisplay = new KCurve(WStackPage_11);
	WStackPage11Layout->addWidget( Kdisplay );
	optionStack->addWidget( WStackPage_11, 10 );

	layout16->addWidget( optionStack );
	EffectsDialogLayout->addLayout( layout16 );

	layout18 = new QVBoxLayout( 0, 0, 0, "layout18");
	layout10 = new QGridLayout( 0, 1, 1, 0, 5, "layout10");
	layout2 = new QVBoxLayout( 0, 0, 5, "layout2");
	textLabel1 = new QLabel( this, "textLabel1" );
	textLabel1->setText( tr( "Available Effects" ) );
	layout2->addWidget( textLabel1 );
	availableEffects = new QListBox( this, "availableEffects" );
	availableEffects->clear();
	availableEffects->insertItem( tr("Blur"));
	availableEffects->insertItem( tr("Brightness"));
	availableEffects->insertItem( tr("Colorize"));
	availableEffects->insertItem( tr("Duotone"));
	availableEffects->insertItem( tr("Tritone"));
	availableEffects->insertItem( tr("Quadtone"));
	availableEffects->insertItem( tr("Contrast"));
	availableEffects->insertItem( tr("Grayscale"));
	availableEffects->insertItem( tr("Curves"));
	availableEffects->insertItem( tr("Invert"));
	availableEffects->insertItem( tr("Posterize"));
	availableEffects->insertItem( tr("Sharpen"));
	availableEffects->setMinimumSize(fontMetrics().width( tr( "Available Effects" ))+40, 180);
	layout2->addWidget( availableEffects );
	layout10->addLayout( layout2, 0, 0 );

	layout1 = new QVBoxLayout( 0, 0, 5, "layout1");
	QSpacerItem* spacer1 = new QSpacerItem( 21, 61, QSizePolicy::Minimum, QSizePolicy::Expanding );
	layout1->addItem( spacer1 );
	toEffects = new QPushButton( this, "toEffects" );
	toEffects->setText( tr( ">>" ) );
	toEffects->setEnabled(false);
	layout1->addWidget( toEffects );
	fromEffects = new QPushButton( this, "fromEffects" );
	fromEffects->setText( tr( "<<" ) );
	fromEffects->setEnabled(false);
	layout1->addWidget( fromEffects );
	QSpacerItem* spacer2 = new QSpacerItem( 31, 81, QSizePolicy::Minimum, QSizePolicy::Expanding );
	layout1->addItem( spacer2 );
	layout10->addLayout( layout1, 0, 1 );

	layout8 = new QVBoxLayout( 0, 0, 5, "layout8");
	textLabel2 = new QLabel( this, "textLabel2" );
	textLabel2->setText( tr( "Effects in use" ) );
	layout8->addWidget( textLabel2 );
	usedEffects = new QListBox( this, "usedEffects" );
	usedEffects->setMinimumSize(fontMetrics().width( tr( "Available Effects" ))+40, 180);
	usedEffects->clear();
	effectValMap.clear();
	for (uint a = 0; a < effectsList.count(); ++a)
	{
		if ((*effectsList.at(a)).effectCode == ScImage::EF_INVERT)
		{
			usedEffects->insertItem( tr("Invert"));
			effectValMap.insert(usedEffects->item(usedEffects->count()-1), "");
		}
		if ((*effectsList.at(a)).effectCode == ScImage::EF_GRAYSCALE)
		{
			usedEffects->insertItem( tr("Grayscale"));
			effectValMap.insert(usedEffects->item(usedEffects->count()-1), "");
		}
		if ((*effectsList.at(a)).effectCode == ScImage::EF_COLORIZE)
		{
			usedEffects->insertItem( tr("Colorize"));
			effectValMap.insert(usedEffects->item(usedEffects->count()-1), (*effectsList.at(a)).effectParameters);
		}
		if ((*effectsList.at(a)).effectCode == ScImage::EF_BRIGHTNESS)
		{
			usedEffects->insertItem( tr("Brightness"));
			effectValMap.insert(usedEffects->item(usedEffects->count()-1), (*effectsList.at(a)).effectParameters);
		}
		if ((*effectsList.at(a)).effectCode == ScImage::EF_CONTRAST)
		{
			usedEffects->insertItem( tr("Contrast"));
			effectValMap.insert(usedEffects->item(usedEffects->count()-1), (*effectsList.at(a)).effectParameters);
		}
		if ((*effectsList.at(a)).effectCode == ScImage::EF_SHARPEN)
		{
			usedEffects->insertItem( tr("Sharpen"));
			effectValMap.insert(usedEffects->item(usedEffects->count()-1), (*effectsList.at(a)).effectParameters);
		}
		if ((*effectsList.at(a)).effectCode == ScImage::EF_BLUR)
		{
			usedEffects->insertItem( tr("Blur"));
			effectValMap.insert(usedEffects->item(usedEffects->count()-1), (*effectsList.at(a)).effectParameters);
		}
		if ((*effectsList.at(a)).effectCode == ScImage::EF_SOLARIZE)
		{
			usedEffects->insertItem( tr("Posterize"));
			effectValMap.insert(usedEffects->item(usedEffects->count()-1), (*effectsList.at(a)).effectParameters);
		}
		if ((*effectsList.at(a)).effectCode == ScImage::EF_DUOTONE)
		{
			usedEffects->insertItem( tr("Duotone"));
			effectValMap.insert(usedEffects->item(usedEffects->count()-1), (*effectsList.at(a)).effectParameters);
		}
		if ((*effectsList.at(a)).effectCode == ScImage::EF_TRITONE)
		{
			usedEffects->insertItem( tr("Tritone"));
			effectValMap.insert(usedEffects->item(usedEffects->count()-1), (*effectsList.at(a)).effectParameters);
		}
		if ((*effectsList.at(a)).effectCode == ScImage::EF_QUADTONE)
		{
			usedEffects->insertItem( tr("Quadtone"));
			effectValMap.insert(usedEffects->item(usedEffects->count()-1), (*effectsList.at(a)).effectParameters);
		}
		if ((*effectsList.at(a)).effectCode == ScImage::EF_GRADUATE)
		{
			usedEffects->insertItem( tr("Curves"));
			effectValMap.insert(usedEffects->item(usedEffects->count()-1), (*effectsList.at(a)).effectParameters);
		}
	}
	layout8->addWidget( usedEffects );
	layout7 = new QHBoxLayout( 0, 0, 5, "layout7");
	QSpacerItem* spacer4 = new QSpacerItem( 10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout7->addItem( spacer4 );
	effectUp = new QPushButton( this, "effectUp" );
	effectUp->setText( "" );
	effectUp->setPixmap(loadIcon("Raiselayer.png"));
	effectUp->setEnabled(false);
	layout7->addWidget( effectUp );
	effectDown = new QPushButton( this, "effectDown" );
	effectDown->setText( "" );
	effectDown->setPixmap(loadIcon("Lowerlayer.png"));
	effectDown->setEnabled(false);
	layout7->addWidget( effectDown );
	QSpacerItem* spacer5 = new QSpacerItem( 10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout7->addItem( spacer5 );
	layout8->addLayout( layout7 );
	layout10->addLayout( layout8, 0, 2 );
	layout18->addLayout( layout10 );

	layout9 = new QHBoxLayout( 0, 0, 5, "layout9");
	QSpacerItem* spacer3 = new QSpacerItem( 111, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout9->addItem( spacer3 );

	okButton = new QPushButton( this, "okButton" );
	okButton->setText( tr( "OK" ) );
	layout9->addWidget( okButton );

	cancelButton = new QPushButton( this, "cancelButton" );
	cancelButton->setText( tr( "Cancel" ) );
	layout9->addWidget( cancelButton );
	layout18->addLayout( layout9 );
	EffectsDialogLayout->addLayout( layout18 );
	optionStack->raiseWidget(0);
	createPreview();
	resize( minimumSizeHint() );
	clearWState( WState_Polished );

	// signals and slots connections
	connect( okButton, SIGNAL( clicked() ), this, SLOT( leaveOK() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( usedEffects, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( selectEffect(QListBoxItem*) ) );
	connect( usedEffects, SIGNAL( doubleClicked(QListBoxItem*) ), this, SLOT( moveFromEffects() ) );
	connect( availableEffects, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( selectAvailEffect(QListBoxItem*) ) );
	connect( availableEffects, SIGNAL( doubleClicked(QListBoxItem*) ), this, SLOT( moveToEffects() ) );
	connect( toEffects, SIGNAL( clicked() ), this, SLOT( moveToEffects() ) );
	connect( fromEffects, SIGNAL( clicked() ), this, SLOT( moveFromEffects() ) );
	connect( effectUp, SIGNAL( clicked() ), this, SLOT( moveEffectUp() ) );
	connect( effectDown, SIGNAL( clicked() ), this, SLOT( moveEffectDown() ) );
	connect( colData, SIGNAL(activated(int)), this, SLOT( createPreview()));
	connect( shade, SIGNAL(clicked()), this, SLOT(createPreview()));
	connect( colData1, SIGNAL(activated(int)), this, SLOT( createPreview()));
	connect( shade1, SIGNAL(clicked()), this, SLOT(createPreview()));
	connect( colData2, SIGNAL(activated(int)), this, SLOT( createPreview()));
	connect( shade2, SIGNAL(clicked()), this, SLOT(createPreview()));
	connect( colDatat1, SIGNAL(activated(int)), this, SLOT( createPreview()));
	connect( shadet1, SIGNAL(clicked()), this, SLOT(createPreview()));
	connect( colDatat2, SIGNAL(activated(int)), this, SLOT( createPreview()));
	connect( shadet2, SIGNAL(clicked()), this, SLOT(createPreview()));
	connect( colDatat3, SIGNAL(activated(int)), this, SLOT( createPreview()));
	connect( shadet3, SIGNAL(clicked()), this, SLOT(createPreview()));
	connect( colDataq1, SIGNAL(activated(int)), this, SLOT( createPreview()));
	connect( shadeq1, SIGNAL(clicked()), this, SLOT(createPreview()));
	connect( colDataq2, SIGNAL(activated(int)), this, SLOT( createPreview()));
	connect( shadeq2, SIGNAL(clicked()), this, SLOT(createPreview()));
	connect( colDataq3, SIGNAL(activated(int)), this, SLOT( createPreview()));
	connect( shadeq3, SIGNAL(clicked()), this, SLOT(createPreview()));
	connect( colDataq4, SIGNAL(activated(int)), this, SLOT( createPreview()));
	connect( shadeq4, SIGNAL(clicked()), this, SLOT(createPreview()));
	connect( brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(updateBright(int)));
	connect( brightnessSlider, SIGNAL(sliderReleased()), this, SLOT(createPreview()));
	connect( contrastSlider, SIGNAL(valueChanged(int)), this, SLOT(updateContrast(int)));
	connect( contrastSlider, SIGNAL(sliderReleased()), this, SLOT(createPreview()));
	connect( shRadius, SIGNAL(valueChanged(int)), this, SLOT(createPreview()));
	connect( shValue, SIGNAL(valueChanged(int)), this, SLOT(createPreview()));
	connect( blRadius, SIGNAL(valueChanged(int)), this, SLOT(createPreview()));
	connect( solarizeSlider, SIGNAL(valueChanged(int)), this, SLOT(updateSolarize(int)));
	connect( solarizeSlider, SIGNAL(sliderReleased()), this, SLOT(createPreview()));
	connect( Kdisplay, SIGNAL(modified()), this, SLOT(createPreview()));
	tim.start();
}

void EffectsDialog::leaveOK()
{
	saveValues(true);
	accept();
}

void EffectsDialog::updateSolarize(int val)
{
	QString tmp;
	tmp.setNum(val);
	textLabel15->setText(tmp);
	createPreview();
}

void EffectsDialog::updateContrast(int val)
{
	QString tmp;
	tmp.setNum(val);
	textLabel9->setText(tmp);
	createPreview();
}

void EffectsDialog::updateBright(int val)
{
	QString tmp;
	tmp.setNum(val);
	textLabel7->setText(tmp);
	createPreview();
}

void EffectsDialog::createPreview()
{
	if (tim.elapsed() < 50)
		return;
	ScImage im(image);
	saveValues(false);
	im.applyEffect(effectsList, doc->PageColors, false);
	QPixmap Bild = QPixmap(pixmapLabel1->width(), pixmapLabel1->height());
	int x = (pixmapLabel1->width() - im.qImage().width()) / 2;
	int y = (pixmapLabel1->height() - im.qImage().height()) / 2;
	QPainter p;
	QBrush b(QColor(205,205,205), loadIcon("testfill.png"));
	p.begin(&Bild);
	p.fillRect(0, 0, pixmapLabel1->width(), pixmapLabel1->height(), b);
	p.drawImage(x, y, im.qImage());
	p.end();
	pixmapLabel1->setPixmap( Bild );
	tim.start();
}

void EffectsDialog::saveValues(bool final)
{
	if (currentOptions != 0)
	{
		if (currentOptions->text() == tr("Colorize"))
		{
			QString efval = "";
			efval = colData->currentText();
			QString tmp;
			tmp.setNum(shade->getValue());
			efval += "\n"+tmp;
			effectValMap[currentOptions] = efval;
		}
		if (currentOptions->text() == tr("Duotone"))
		{
			QString efval = "";
			efval = colData1->currentText()+"\n";
			efval += colData2->currentText()+"\n";
			QString tmp;
			tmp.setNum(shade1->getValue());
			efval += tmp;
			tmp.setNum(shade2->getValue());
			efval += " "+tmp;
			effectValMap[currentOptions] = efval;
		}
		if (currentOptions->text() == tr("Tritone"))
		{
			QString efval = "";
			efval = colDatat1->currentText()+"\n";
			efval += colDatat2->currentText()+"\n";
			efval += colDatat3->currentText()+"\n";
			QString tmp;
			tmp.setNum(shadet1->getValue());
			efval += tmp;
			tmp.setNum(shadet2->getValue());
			efval += " "+tmp;
			tmp.setNum(shadet3->getValue());
			efval += " "+tmp;
			effectValMap[currentOptions] = efval;
		}
		if (currentOptions->text() == tr("Quadtone"))
		{
			QString efval = "";
			efval = colDataq1->currentText()+"\n";
			efval += colDataq2->currentText()+"\n";
			efval += colDataq3->currentText()+"\n";
			efval += colDataq4->currentText()+"\n";
			QString tmp;
			tmp.setNum(shadeq1->getValue());
			efval += tmp;
			tmp.setNum(shadeq2->getValue());
			efval += " "+tmp;
			tmp.setNum(shadeq3->getValue());
			efval += " "+tmp;
			tmp.setNum(shadeq4->getValue());
			efval += " "+tmp;
			effectValMap[currentOptions] = efval;
		}
		if (currentOptions->text() == tr("Brightness"))
		{
			QString tmp;
			tmp.setNum(brightnessSlider->value());
			effectValMap[currentOptions] = tmp;
		}
		if (currentOptions->text() == tr("Contrast"))
		{
			QString tmp;
			tmp.setNum(contrastSlider->value());
			effectValMap[currentOptions] = tmp;
		}
		if (currentOptions->text() == tr("Sharpen"))
		{
			QString efval = "";
			QString tmp;
			tmp.setNum(shRadius->value());
			efval += tmp;
			tmp.setNum(shValue->value());
			efval += " "+tmp;
			effectValMap[currentOptions] = efval;
		}
		if (currentOptions->text() == tr("Blur"))
		{
			QString efval = "";
			QString tmp;
			if (final)
				tmp.setNum(blRadius->value());
			else
				tmp.setNum(blRadius->value()/imageScale);
			efval += tmp;
			tmp.setNum(1.0);
			efval += " "+tmp;
			effectValMap[currentOptions] = efval;
		}
		if (currentOptions->text() == tr("Posterize"))
		{
			QString tmp;
			tmp.setNum(solarizeSlider->value());
			effectValMap[currentOptions] = tmp;
		}
		if (currentOptions->text() == tr("Curves"))
		{
			QString efval = "";
			FPointArray Vals = Kdisplay->getCurve();
			QString tmp;
			tmp.setNum(Vals.size());
			efval += tmp;
			for (uint p = 0; p < Vals.size(); p++)
			{
				FPoint pv = Vals.point(p);
				efval += QString(" %1 %2").arg(pv.x()).arg(pv.y());
			}
			effectValMap[currentOptions] = efval;
		}
	}
	effectsList.clear();
	struct ScImage::imageEffect ef;
	for (uint e = 0; e < usedEffects->count(); ++e)
	{
		if (usedEffects->item(e)->text() == tr("Invert"))
		{
			ef.effectCode = ScImage::EF_INVERT;
			ef.effectParameters = "";
		}
		if (usedEffects->item(e)->text() == tr("Grayscale"))
		{
			ef.effectCode = ScImage::EF_GRAYSCALE;
			ef.effectParameters = "";
		}
		if (usedEffects->item(e)->text() == tr("Colorize"))
		{
			ef.effectCode = ScImage::EF_COLORIZE;
			ef.effectParameters = effectValMap[usedEffects->item(e)];
		}
		if (usedEffects->item(e)->text() == tr("Brightness"))
		{
			ef.effectCode = ScImage::EF_BRIGHTNESS;
			ef.effectParameters = effectValMap[usedEffects->item(e)];
		}
		if (usedEffects->item(e)->text() == tr("Contrast"))
		{
			ef.effectCode = ScImage::EF_CONTRAST;
			ef.effectParameters = effectValMap[usedEffects->item(e)];
		}
		if (usedEffects->item(e)->text() == tr("Sharpen"))
		{
			ef.effectCode = ScImage::EF_SHARPEN;
			ef.effectParameters = effectValMap[usedEffects->item(e)];
		}
		if (usedEffects->item(e)->text() == tr("Blur"))
		{
			ef.effectCode = ScImage::EF_BLUR;
			ef.effectParameters = effectValMap[usedEffects->item(e)];
		}
		if (usedEffects->item(e)->text() == tr("Posterize"))
		{
			ef.effectCode = ScImage::EF_SOLARIZE;
			ef.effectParameters = effectValMap[usedEffects->item(e)];
		}
		if (usedEffects->item(e)->text() == tr("Duotone"))
		{
			ef.effectCode = ScImage::EF_DUOTONE;
			ef.effectParameters = effectValMap[usedEffects->item(e)];
		}
		if (usedEffects->item(e)->text() == tr("Tritone"))
		{
			ef.effectCode = ScImage::EF_TRITONE;
			ef.effectParameters = effectValMap[usedEffects->item(e)];
		}
		if (usedEffects->item(e)->text() == tr("Quadtone"))
		{
			ef.effectCode = ScImage::EF_QUADTONE;
			ef.effectParameters = effectValMap[usedEffects->item(e)];
		}
		if (usedEffects->item(e)->text() == tr("Curves"))
		{
			ef.effectCode = ScImage::EF_GRADUATE;
			ef.effectParameters = effectValMap[usedEffects->item(e)];
		}
		effectsList.append(ef);
	}
}

void EffectsDialog::moveToEffects()
{
	usedEffects->insertItem(availableEffects->currentText());
	if (availableEffects->currentText() == tr("Invert"))
		effectValMap.insert(usedEffects->item(usedEffects->count()-1), "");
	if (availableEffects->currentText() == tr("Grayscale"))
		effectValMap.insert(usedEffects->item(usedEffects->count()-1), "");
	if (availableEffects->currentText() == tr("Brightness"))
		effectValMap.insert(usedEffects->item(usedEffects->count()-1), "0");
	if (availableEffects->currentText() == tr("Contrast"))
		effectValMap.insert(usedEffects->item(usedEffects->count()-1), "0");
	if (availableEffects->currentText() == tr("Sharpen"))
		effectValMap.insert(usedEffects->item(usedEffects->count()-1), "0 1");
	if (availableEffects->currentText() == tr("Blur"))
		effectValMap.insert(usedEffects->item(usedEffects->count()-1), "0 1");
	if (availableEffects->currentText() == tr("Posterize"))
		effectValMap.insert(usedEffects->item(usedEffects->count()-1), "255");
	if (availableEffects->currentText() == tr("Colorize"))
	{
		ColorList::Iterator it;
		it = doc->PageColors.begin();
		QString efval = it.key()+"\n100";
		effectValMap.insert(usedEffects->item(usedEffects->count()-1), efval);
	}
	if (availableEffects->currentText() == tr("Duotone"))
	{
		ColorList::Iterator it;
		it = doc->PageColors.begin();
		QString efval = it.key()+"\n"+it.key()+"\n100 100";
		effectValMap.insert(usedEffects->item(usedEffects->count()-1), efval);
	}
	if (availableEffects->currentText() == tr("Tritone"))
	{
		ColorList::Iterator it;
		it = doc->PageColors.begin();
		QString efval = it.key()+"\n"+it.key()+"\n"+it.key()+"\n100 100 100";
		effectValMap.insert(usedEffects->item(usedEffects->count()-1), efval);
	}
	if (availableEffects->currentText() == tr("Quadtone"))
	{
		ColorList::Iterator it;
		it = doc->PageColors.begin();
		QString efval = it.key()+"\n"+it.key()+"\n"+it.key()+"\n"+it.key()+"\n100 100 100 100";
		effectValMap.insert(usedEffects->item(usedEffects->count()-1), efval);
	}
	if (availableEffects->currentText() == tr("Curves"))
		effectValMap.insert(usedEffects->item(usedEffects->count()-1), "2 0.0 0.0 1.0 1.0");
	disconnect( usedEffects, SIGNAL( selected(QListBoxItem*) ), this, SLOT( selectEffect(QListBoxItem*) ) );
	usedEffects->setCurrentItem(usedEffects->item(usedEffects->count()-1));
	selectEffect(usedEffects->item(usedEffects->count()-1));
	connect( usedEffects, SIGNAL( selected(QListBoxItem*) ), this, SLOT( selectEffect(QListBoxItem*) ) );
	createPreview();
}

void EffectsDialog::moveFromEffects()
{
	disconnect( usedEffects, SIGNAL( selected(QListBoxItem*) ), this, SLOT( selectEffect(QListBoxItem*) ) );
	effectValMap.remove(usedEffects->item(usedEffects->currentItem()));
	usedEffects->removeItem(usedEffects->currentItem());
	currentOptions = 0;
	if (usedEffects->count() == 0)
		fromEffects->setEnabled(false);
	if (usedEffects->count() < 2)
	{
		effectUp->setEnabled(false);
		effectDown->setEnabled(false);
	}
	usedEffects->setSelected(usedEffects->item(usedEffects->currentItem()), true);
	selectEffect(usedEffects->item(usedEffects->currentItem()));
	createPreview();
	connect( usedEffects, SIGNAL( selected(QListBoxItem*) ), this, SLOT( selectEffect(QListBoxItem*) ) );
}

void EffectsDialog::moveEffectUp()
{
	int curr = usedEffects->currentItem();
	if (curr == 0)
		return;
	disconnect( usedEffects, SIGNAL( selected(QListBoxItem*) ), this, SLOT( selectEffect(QListBoxItem*) ) );
	usedEffects->clearSelection();
	QListBoxItem *it = usedEffects->item(curr);
	usedEffects->takeItem(it);
	usedEffects->insertItem(it, curr-1);
	usedEffects->setCurrentItem(it);
	selectEffect(usedEffects->item(usedEffects->currentItem()));
	createPreview();
	connect( usedEffects, SIGNAL( selected(QListBoxItem*) ), this, SLOT( selectEffect(QListBoxItem*) ) );
}

void EffectsDialog::moveEffectDown()
{
	int curr = usedEffects->currentItem();
	if (curr == static_cast<int>(usedEffects->count())-1)
		return;
	disconnect( usedEffects, SIGNAL( selected(QListBoxItem*) ), this, SLOT( selectEffect(QListBoxItem*) ) );
	usedEffects->clearSelection();
	QListBoxItem *it = usedEffects->item(curr);
	usedEffects->takeItem(it);
	usedEffects->insertItem(it, curr+1);
	usedEffects->setCurrentItem(it);
	selectEffect(usedEffects->item(usedEffects->currentItem()));
	createPreview();
	connect( usedEffects, SIGNAL( selected(QListBoxItem*) ), this, SLOT( selectEffect(QListBoxItem*) ) );
}

void EffectsDialog::selectEffect(QListBoxItem* c)
{
	toEffects->setEnabled(false);
	if (currentOptions != 0)
	{
		if (currentOptions->text() == tr("Colorize"))
		{
			QString efval = "";
			efval = colData->currentText();
			QString tmp;
			tmp.setNum(shade->getValue());
			efval += "\n"+tmp;
			effectValMap[currentOptions] = efval;
		}
		if (currentOptions->text() == tr("Brightness"))
		{
			QString tmp;
			tmp.setNum(brightnessSlider->value());
			effectValMap[currentOptions] = tmp;
		}
		if (currentOptions->text() == tr("Contrast"))
		{
			QString tmp;
			tmp.setNum(contrastSlider->value());
			effectValMap[currentOptions] = tmp;
		}
		if (currentOptions->text() == tr("Sharpen"))
		{
			QString efval = "";
			QString tmp;
			tmp.setNum(shRadius->value());
			efval += tmp;
			tmp.setNum(shValue->value());
			efval += " "+tmp;
			effectValMap[currentOptions] = efval;
		}
		if (currentOptions->text() == tr("Blur"))
		{
			QString efval = "";
			QString tmp;
			tmp.setNum(blRadius->value());
			efval += tmp;
			tmp.setNum(1.0);
			efval += " "+tmp;
			effectValMap[currentOptions] = efval;
		}
		if (currentOptions->text() == tr("Posterize"))
		{
			QString tmp;
			tmp.setNum(solarizeSlider->value());
			effectValMap[currentOptions] = tmp;
		}
		if (currentOptions->text() == tr("Duotone"))
		{
			QString efval = "";
			efval = colData1->currentText()+"\n";
			efval += colData2->currentText()+"\n";
			QString tmp;
			tmp.setNum(shade1->getValue());
			efval += tmp;
			tmp.setNum(shade2->getValue());
			efval += " "+tmp;
			effectValMap[currentOptions] = efval;
		}
		if (currentOptions->text() == tr("Tritone"))
		{
			QString efval = "";
			efval = colDatat1->currentText()+"\n";
			efval += colDatat2->currentText()+"\n";
			efval += colDatat3->currentText()+"\n";
			QString tmp;
			tmp.setNum(shadet1->getValue());
			efval += tmp;
			tmp.setNum(shadet2->getValue());
			efval += " "+tmp;
			tmp.setNum(shadet3->getValue());
			efval += " "+tmp;
			effectValMap[currentOptions] = efval;
		}
		if (currentOptions->text() == tr("Quadtone"))
		{
			QString efval = "";
			efval = colDataq1->currentText()+"\n";
			efval += colDataq2->currentText()+"\n";
			efval += colDataq3->currentText()+"\n";
			efval += colDataq4->currentText()+"\n";
			QString tmp;
			tmp.setNum(shadeq1->getValue());
			efval += tmp;
			tmp.setNum(shadeq2->getValue());
			efval += " "+tmp;
			tmp.setNum(shadeq3->getValue());
			efval += " "+tmp;
			tmp.setNum(shadeq4->getValue());
			efval += " "+tmp;
			effectValMap[currentOptions] = efval;
		}
		if (currentOptions->text() == tr("Curves"))
		{
			QString efval = "";
			FPointArray Vals = Kdisplay->getCurve();
			QString tmp;
			tmp.setNum(Vals.size());
			efval += tmp;
			for (uint p = 0; p < Vals.size(); p++)
			{
				FPoint pv = Vals.point(p);
				efval += QString(" %1 %2").arg(pv.x()).arg(pv.y());
			}
			effectValMap[currentOptions] = efval;
		}
	}
	if (c)
	{
		fromEffects->setEnabled(true);
		if (usedEffects->count() > 1)
		{
			effectUp->setEnabled(true);
			effectDown->setEnabled(true);
			if (usedEffects->currentItem() == 0)
				effectUp->setEnabled(false);
			if (usedEffects->currentItem() == static_cast<int>(usedEffects->count())-1)
				effectDown->setEnabled(false);
		}
		if (c->text() == tr("Grayscale"))
			optionStack->raiseWidget(0);
		else if (c->text() == tr("Invert"))
			optionStack->raiseWidget(0);
		else if (c->text() == tr("Colorize"))
		{
			disconnect( colData, SIGNAL(activated(int)), this, SLOT( createPreview()));
			disconnect( shade, SIGNAL(clicked()), this, SLOT(createPreview()));
			QString tmpstr = effectValMap[c];
			QString col;
			int shading;
			QTextStream fp(&tmpstr, IO_ReadOnly);
			col = fp.readLine();
			fp >> shading;
			colData->setCurrentText(col);
			shade->setValue(shading);
			optionStack->raiseWidget(1);
			connect( colData, SIGNAL(activated(int)), this, SLOT( createPreview()));
			connect( shade, SIGNAL(clicked()), this, SLOT(createPreview()));
		}
		else if (c->text() == tr("Duotone"))
		{
			disconnect( colData1, SIGNAL(activated(int)), this, SLOT( createPreview()));
			disconnect( shade1, SIGNAL(clicked()), this, SLOT(createPreview()));
			disconnect( colData2, SIGNAL(activated(int)), this, SLOT( createPreview()));
			disconnect( shade2, SIGNAL(clicked()), this, SLOT(createPreview()));
			QString tmpstr = effectValMap[c];
			QString col1, col2;
			int shading1, shading2;
			QTextStream fp(&tmpstr, IO_ReadOnly);
			col1 = fp.readLine();
			col2 = fp.readLine();
			fp >> shading1;
			fp >> shading2;
			colData1->setCurrentText(col1);
			shade1->setValue(shading1);
			colData2->setCurrentText(col2);
			shade2->setValue(shading2);
			optionStack->raiseWidget(7);
			connect( colData1, SIGNAL(activated(int)), this, SLOT( createPreview()));
			connect( shade1, SIGNAL(clicked()), this, SLOT(createPreview()));
			connect( colData2, SIGNAL(activated(int)), this, SLOT( createPreview()));
			connect( shade2, SIGNAL(clicked()), this, SLOT(createPreview()));
		}
		else if (c->text() == tr("Tritone"))
		{
			disconnect( colDatat1, SIGNAL(activated(int)), this, SLOT( createPreview()));
			disconnect( shadet1, SIGNAL(clicked()), this, SLOT(createPreview()));
			disconnect( colDatat2, SIGNAL(activated(int)), this, SLOT( createPreview()));
			disconnect( shadet2, SIGNAL(clicked()), this, SLOT(createPreview()));
			disconnect( colDatat3, SIGNAL(activated(int)), this, SLOT( createPreview()));
			disconnect( shadet3, SIGNAL(clicked()), this, SLOT(createPreview()));
			QString tmpstr = effectValMap[c];
			QString col1, col2, col3;
			int shading1, shading2, shading3;
			QTextStream fp(&tmpstr, IO_ReadOnly);
			col1 = fp.readLine();
			col2 = fp.readLine();
			col3 = fp.readLine();
			fp >> shading1;
			fp >> shading2;
			fp >> shading3;
			colDatat1->setCurrentText(col1);
			shadet1->setValue(shading1);
			colDatat2->setCurrentText(col2);
			shadet2->setValue(shading2);
			colDatat3->setCurrentText(col3);
			shadet3->setValue(shading3);
			optionStack->raiseWidget(8);
			connect( colDatat1, SIGNAL(activated(int)), this, SLOT( createPreview()));
			connect( shadet1, SIGNAL(clicked()), this, SLOT(createPreview()));
			connect( colDatat2, SIGNAL(activated(int)), this, SLOT( createPreview()));
			connect( shadet2, SIGNAL(clicked()), this, SLOT(createPreview()));
			connect( colDatat3, SIGNAL(activated(int)), this, SLOT( createPreview()));
			connect( shadet3, SIGNAL(clicked()), this, SLOT(createPreview()));
		}
		else if (c->text() == tr("Quadtone"))
		{
			disconnect( colDataq1, SIGNAL(activated(int)), this, SLOT( createPreview()));
			disconnect( shadeq1, SIGNAL(clicked()), this, SLOT(createPreview()));
			disconnect( colDataq2, SIGNAL(activated(int)), this, SLOT( createPreview()));
			disconnect( shadeq2, SIGNAL(clicked()), this, SLOT(createPreview()));
			disconnect( colDataq3, SIGNAL(activated(int)), this, SLOT( createPreview()));
			disconnect( shadeq3, SIGNAL(clicked()), this, SLOT(createPreview()));
			disconnect( colDataq4, SIGNAL(activated(int)), this, SLOT( createPreview()));
			disconnect( shadeq4, SIGNAL(clicked()), this, SLOT(createPreview()));
			QString tmpstr = effectValMap[c];
			QString col1, col2, col3, col4;
			int shading1, shading2, shading3, shading4;
			QTextStream fp(&tmpstr, IO_ReadOnly);
			col1 = fp.readLine();
			col2 = fp.readLine();
			col3 = fp.readLine();
			col4 = fp.readLine();
			fp >> shading1;
			fp >> shading2;
			fp >> shading3;
			fp >> shading4;
			colDataq1->setCurrentText(col1);
			shadeq1->setValue(shading1);
			colDataq2->setCurrentText(col2);
			shadeq2->setValue(shading2);
			colDataq3->setCurrentText(col3);
			shadeq3->setValue(shading3);
			colDataq4->setCurrentText(col4);
			shadeq4->setValue(shading4);
			optionStack->raiseWidget(9);
			connect( colDataq1, SIGNAL(activated(int)), this, SLOT( createPreview()));
			connect( shadeq1, SIGNAL(clicked()), this, SLOT(createPreview()));
			connect( colDataq2, SIGNAL(activated(int)), this, SLOT( createPreview()));
			connect( shadeq2, SIGNAL(clicked()), this, SLOT(createPreview()));
			connect( colDataq3, SIGNAL(activated(int)), this, SLOT( createPreview()));
			connect( shadeq3, SIGNAL(clicked()), this, SLOT(createPreview()));
			connect( colDataq4, SIGNAL(activated(int)), this, SLOT( createPreview()));
			connect( shadeq4, SIGNAL(clicked()), this, SLOT(createPreview()));
		}
		else if (c->text() == tr("Brightness"))
		{
			disconnect( brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(updateBright(int)));
			disconnect( brightnessSlider, SIGNAL(sliderReleased()), this, SLOT(createPreview()));
			QString tmpstr = effectValMap[c];
			int brightness;
			QTextStream fp(&tmpstr, IO_ReadOnly);
			fp >> brightness;
			brightnessSlider->setValue(brightness);
			QString tmp;
			tmp.setNum(brightness);
			textLabel7->setText(tmp);
			optionStack->raiseWidget(2);
			connect( brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(updateBright(int)));
			connect( brightnessSlider, SIGNAL(sliderReleased()), this, SLOT(createPreview()));
		}
		else if (c->text() == tr("Contrast"))
		{
			disconnect( contrastSlider, SIGNAL(valueChanged(int)), this, SLOT(updateContrast(int)));
			disconnect( contrastSlider, SIGNAL(sliderReleased()), this, SLOT(createPreview()));
			QString tmpstr = effectValMap[c];
			int contrast;
			QTextStream fp(&tmpstr, IO_ReadOnly);
			fp >> contrast;
			contrastSlider->setValue(contrast);
			QString tmp;
			tmp.setNum(contrast);
			textLabel9->setText(tmp);
			optionStack->raiseWidget(3);
			connect( contrastSlider, SIGNAL(valueChanged(int)), this, SLOT(updateContrast(int)));
			connect( contrastSlider, SIGNAL(sliderReleased()), this, SLOT(createPreview()));
		}
		else if (c->text() == tr("Sharpen"))
		{
			disconnect( shRadius, SIGNAL(valueChanged(int)), this, SLOT(createPreview()));
			disconnect( shValue, SIGNAL(valueChanged(int)), this, SLOT(createPreview()));
			QString tmpstr = effectValMap[c];
			double radius, sigma;
			QTextStream fp(&tmpstr, IO_ReadOnly);
			fp >> radius;
			fp >> sigma;
			shRadius->setValue(radius);
			shValue->setValue(sigma);
			optionStack->raiseWidget(4);
			connect( shRadius, SIGNAL(valueChanged(int)), this, SLOT(createPreview()));
			connect( shValue, SIGNAL(valueChanged(int)), this, SLOT(createPreview()));
		}
		else if (c->text() == tr("Blur"))
		{
			disconnect( blRadius, SIGNAL(valueChanged(int)), this, SLOT(createPreview()));
			QString tmpstr = effectValMap[c];
			double radius, sigma;
			QTextStream fp(&tmpstr, IO_ReadOnly);
			fp >> radius;
			fp >> sigma;
			blRadius->setValue(radius);
			//			blValue->setValue(sigma);
			optionStack->raiseWidget(5);
			connect( blRadius, SIGNAL(valueChanged(int)), this, SLOT(createPreview()));
		}
		else if (c->text() == tr("Posterize"))
		{
			disconnect( solarizeSlider, SIGNAL(valueChanged(int)), this, SLOT(updateSolarize(int)));
			disconnect( solarizeSlider, SIGNAL(sliderReleased()), this, SLOT(createPreview()));
			QString tmpstr = effectValMap[c];
			int solarize;
			QTextStream fp(&tmpstr, IO_ReadOnly);
			fp >> solarize;
			solarizeSlider->setValue(solarize);
			QString tmp;
			tmp.setNum(solarize);
			textLabel15->setText(tmp);
			optionStack->raiseWidget(6);
			connect( solarizeSlider, SIGNAL(valueChanged(int)), this, SLOT(updateSolarize(int)));
			connect( solarizeSlider, SIGNAL(sliderReleased()), this, SLOT(createPreview()));
		}
		else if (c->text() == tr("Curves"))
		{
			disconnect( Kdisplay, SIGNAL(modified()), this, SLOT(createPreview()));
			QString tmpstr = effectValMap[c];
			QTextStream fp(&tmpstr, IO_ReadOnly);
			int numVals;
			double xval, yval;
			FPointArray curve;
			curve.resize(0);
			fp >> numVals;
			for (int nv = 0; nv < numVals; nv++)
			{
				fp >> xval;
				fp >> yval;
				curve.addPoint(xval, yval);
			}
			Kdisplay->setCurve(curve);
			optionStack->raiseWidget(10);
			connect( Kdisplay, SIGNAL(modified()), this, SLOT(createPreview()));
		}
		else
			optionStack->raiseWidget(0);
		currentOptions = c;
	}
	else
		optionStack->raiseWidget(0);
	disconnect( availableEffects, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( selectAvailEffect(QListBoxItem*) ) );
	availableEffects->clearSelection();
	connect( availableEffects, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( selectAvailEffect(QListBoxItem*) ) );
}

void EffectsDialog::selectAvailEffect(QListBoxItem* c)
{
	if (c)
		toEffects->setEnabled(true);
	fromEffects->setEnabled(false);
	effectUp->setEnabled(false);
	effectDown->setEnabled(false);
	disconnect( usedEffects, SIGNAL( selected(QListBoxItem*) ), this, SLOT( selectEffect(QListBoxItem*) ) );
	if (currentOptions != 0)
	{
		if (currentOptions->text() == tr("Colorize"))
		{
			QString efval = "";
			efval = colData->currentText();
			QString tmp;
			tmp.setNum(shade->getValue());
			efval += "\n"+tmp;
			effectValMap[currentOptions] = efval;
		}
		if (currentOptions->text() == tr("Brightness"))
		{
			QString tmp;
			tmp.setNum(brightnessSlider->value());
			effectValMap[currentOptions] = tmp;
		}
		if (currentOptions->text() == tr("Contrast"))
		{
			QString tmp;
			tmp.setNum(contrastSlider->value());
			effectValMap[currentOptions] = tmp;
		}
		if (currentOptions->text() == tr("Sharpen"))
		{
			QString efval = "";
			QString tmp;
			tmp.setNum(shRadius->value());
			efval += tmp;
			tmp.setNum(shValue->value());
			efval += " "+tmp;
			effectValMap[currentOptions] = efval;
		}
		if (currentOptions->text() == tr("Blur"))
		{
			QString efval = "";
			QString tmp;
			tmp.setNum(blRadius->value());
			efval += tmp;
			tmp.setNum(1.0);
			efval += " "+tmp;
			effectValMap[currentOptions] = efval;
		}
		if (currentOptions->text() == tr("Posterize"))
		{
			QString tmp;
			tmp.setNum(solarizeSlider->value());
			effectValMap[currentOptions] = tmp;
		}
		if (currentOptions->text() == tr("Duotone"))
		{
			QString efval = "";
			efval = colData1->currentText()+"\n";
			efval += colData2->currentText()+"\n";
			QString tmp;
			tmp.setNum(shade1->getValue());
			efval += tmp;
			tmp.setNum(shade2->getValue());
			efval += " "+tmp;
			effectValMap[currentOptions] = efval;
			effectValMap[currentOptions] = efval;
		}
		if (currentOptions->text() == tr("Tritone"))
		{
			QString efval = "";
			efval = colDatat1->currentText()+"\n";
			efval += colDatat2->currentText()+"\n";
			efval += colDatat3->currentText()+"\n";
			QString tmp;
			tmp.setNum(shadet1->getValue());
			efval += tmp;
			tmp.setNum(shadet2->getValue());
			efval += " "+tmp;
			tmp.setNum(shadet3->getValue());
			efval += " "+tmp;
			effectValMap[currentOptions] = efval;
		}
		if (currentOptions->text() == tr("Quadtone"))
		{
			QString efval = "";
			efval = colDataq1->currentText()+"\n";
			efval += colDataq2->currentText()+"\n";
			efval += colDataq3->currentText()+"\n";
			efval += colDataq4->currentText()+"\n";
			QString tmp;
			tmp.setNum(shadeq1->getValue());
			efval += tmp;
			tmp.setNum(shadeq2->getValue());
			efval += " "+tmp;
			tmp.setNum(shadeq3->getValue());
			efval += " "+tmp;
			tmp.setNum(shadeq4->getValue());
			efval += " "+tmp;
			effectValMap[currentOptions] = efval;
		}
		if (currentOptions->text() == tr("Curves"))
		{
			QString efval = "";
			FPointArray Vals = Kdisplay->getCurve();
			QString tmp;
			tmp.setNum(Vals.size());
			efval += tmp;
			for (uint p = 0; p < Vals.size(); p++)
			{
				FPoint pv = Vals.point(p);
				efval += QString(" %1 %2").arg(pv.x()).arg(pv.y());
			}
			effectValMap[currentOptions] = efval;
		}
	}
	currentOptions = 0;
	usedEffects->clearSelection();
	optionStack->raiseWidget(0);
	connect( usedEffects, SIGNAL( selected(QListBoxItem*) ), this, SLOT( selectEffect(QListBoxItem*) ) );
}
