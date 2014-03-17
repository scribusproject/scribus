/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "curvewidget.h"

#include <cmath>

#include <QApplication>
#include <QCursor>
#include <QEvent>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QSpacerItem>
#include <QTextStream>
#include <QToolTip>
#include <QVBoxLayout>


#include "commonstrings.h"
#include "customfdialog.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scclocale.h"
#include "util.h"
#include "util_color.h"
#include "util_icon.h"


KCurve::KCurve(QWidget *parent) : QWidget(parent)
{
	m_grab_point     = FPoint();
	m_dragging = false;
	m_linear = false;
	m_pos = 0;
	setMouseTracking(true);
	setMinimumSize(150, 150);
	m_points.resize(0);
	m_points.addPoint(0.0, 0.0);
	m_points.addPoint(1.0, 1.0);
	setFocusPolicy(Qt::StrongFocus);
}

KCurve::~KCurve()
{
}

void KCurve::paintEvent(QPaintEvent *)
{
	int    x = 0;
	int    wWidth = width() - 1;
	int    wHeight = height() - 1;
	// Drawing selection or all histogram values.
	QPainter p1;
	p1.begin(this);
	//  draw background
	p1.fillRect(QRect(0, 0, wWidth, wHeight), QColor(255, 255, 255));
	// Draw grid separators.
	p1.setPen(QPen(Qt::gray, 1, Qt::SolidLine));
	p1.drawLine(wWidth/4, 0, wWidth/4, wHeight);
	p1.drawLine(wWidth/2, 0, wWidth/2, wHeight);
	p1.drawLine(3*wWidth/4, 0, 3*wWidth/4, wHeight);
	p1.drawLine(0, wHeight/4, wWidth, wHeight/4);
	p1.drawLine(0, wHeight/2, wWidth, wHeight/2);
	p1.drawLine(0, 3*wHeight/4, wWidth, 3*wHeight/4);

	// Draw curve.
	double curvePrevVal = getCurveValue(0.0);
	p1.setPen(QPen(Qt::black, 1, Qt::SolidLine));
	for (x = 0 ; x < wWidth ; x++)
	{
		double curveX;
		double curveVal;
		//		curveX = (x + 0.5) / wWidth;
		curveX = x / static_cast<double>(wWidth);
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
			p1.setPen(QPen(Qt::red, 3, Qt::SolidLine));
			p1.drawEllipse( int(p.x() * wWidth) - 2, wHeight - 2 - int(p.y() * wHeight), 4, 4 );
		}
		else
		{
			p1.setPen(QPen(Qt::red, 1, Qt::SolidLine));
			p1.drawEllipse( int(p.x() * wWidth) - 3, wHeight - 3 - int(p.y() * wHeight), 6, 6 );
		}
	}
	p1.end();
}

void KCurve::keyPressEvent(QKeyEvent *e)
{
	if(e->key() == Qt::Key_Delete || e->key() == Qt::Key_Backspace)
	{
		if (m_points.size() > 2)
		{
			FPoint closest_point = m_points.point(0);
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
			repaint();
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
			setCursor(QCursor(Qt::CrossCursor));
		}
	}
	else
	{
		if(fabs(y - closest_point.y()) * width() > 5)
			return;
		m_dragging = true;
		setCursor(QCursor(Qt::CrossCursor));
	}
	// Determine the leftmost and rightmost points.
	m_leftmost = 0;
	m_rightmost = 1;
	p = m_points.point(0);
	cc = 0;
	while(cc < m_points.size())
	{
		if (p != m_grab_point)
		{
			if(p.x() > m_leftmost && p.x() < x)
				m_leftmost = p.x();
			if(p.x() < m_rightmost && p.x() > x)
				m_rightmost = p.x();
		}
		cc++;
		p = m_points.point(cc);
    }
	repaint();
	emit modified();
}

void KCurve::mouseReleaseEvent ( QMouseEvent * e )
{
	if (e->button() != Qt::LeftButton)
		return;
	setCursor(QCursor(Qt::ArrowCursor));
	m_dragging = false;
	repaint();
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
			setCursor(QCursor(Qt::ArrowCursor));
		else
			setCursor(QCursor(Qt::CrossCursor));
	}
	else  // Else, drag the selected point
	{
		setCursor(QCursor(Qt::CrossCursor));
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
		repaint();
		emit modified();
	}
}

double KCurve::getCurveValue(double x)
{
	return getCurveYValue(m_points, x, m_linear);
}

FPointArray KCurve::getCurve()
{
	return m_points.copy();
}

void KCurve::setCurve(FPointArray inlist)
{
	m_points_back = m_points.copy();
	m_points.resize(0);
	m_points = inlist.copy();
	repaint();
	emit modified();
}

void KCurve::resetCurve()
{
	m_points.resize(0);
	m_points = m_points_back.copy();
	repaint();
	emit modified();
}

void KCurve::setLinear(bool setter)
{
	m_linear = setter;
	repaint();
	emit modified();
}

bool KCurve::isLinear()
{
	return m_linear;
}

CurveWidget::CurveWidget( QWidget* parent ) : QWidget( parent )
{
	CurveWidgetLayout = new QHBoxLayout(this);
	CurveWidgetLayout->setMargin(5);
	CurveWidgetLayout->setSpacing(5);

	layout1 = new QVBoxLayout;
	layout1->setMargin(0);
	layout1->setSpacing(5);

	invertButton = new QPushButton( this );
	invertButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	invertButton->setIcon( loadIcon("invert.png") );
	invertButton->setIconSize(QSize(22, 22));
	layout1->addWidget( invertButton );

	resetButton = new QPushButton( this );
	resetButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	resetButton->setIcon( loadIcon("reload.png") );
	resetButton->setIconSize(QSize(22, 22));
	layout1->addWidget( resetButton );
	linearButton = new QPushButton( this );
	QIcon ic;
	ic.addPixmap(loadIcon("curvebezier.png"), QIcon::Normal, QIcon::Off);
	ic.addPixmap(loadIcon("curvelinear.png"), QIcon::Normal, QIcon::On);
	linearButton->setIcon(ic);
	linearButton->setCheckable( true );
	linearButton->setChecked(false);
	linearButton->setIconSize(QSize(22, 22));
	linearButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	layout1->addWidget( linearButton );
	spacer1 = new QSpacerItem( 2, 3, QSizePolicy::Minimum, QSizePolicy::Expanding );
	layout1->addItem( spacer1 );

	loadButton = new QPushButton( this );
	loadButton->setIcon( loadIcon("22/document-open.png") );
	loadButton->setIconSize(QSize(22, 22));
	loadButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	layout1->addWidget( loadButton );

	saveButton = new QPushButton( this );
	saveButton->setIcon( loadIcon("22/document-save-as.png") );
	saveButton->setIconSize(QSize(22, 22));
	saveButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	layout1->addWidget( saveButton );
	CurveWidgetLayout->addLayout( layout1 );

	cDisplay = new KCurve(this);
	cDisplay->setMinimumSize( QSize( 150, 150 ) );
	CurveWidgetLayout->addWidget( cDisplay );
	languageChange();

	connect(invertButton, SIGNAL(clicked()), this, SLOT(doInvert()));
	connect(resetButton, SIGNAL(clicked()), this, SLOT(doReset()));
	connect(linearButton, SIGNAL(clicked()), this, SLOT(doLinear()));
	connect(loadButton, SIGNAL(clicked()), this, SLOT(doLoad()));
	connect(saveButton, SIGNAL(clicked()), this, SLOT(doSave()));
}

void CurveWidget::doInvert()
{
	FPointArray curve = cDisplay->getCurve();
	for (uint a = 0; a < curve.size(); a++)
	{
		FPoint p = curve.point(a);
		curve.setPoint(a, p.x(), 1.0 - p.y());
	}
	cDisplay->setCurve(curve);
}

void CurveWidget::doReset()
{
	cDisplay->resetCurve();
}

void CurveWidget::doLinear()
{
	cDisplay->setLinear(linearButton->isChecked());
}

void CurveWidget::setLinear(bool setter)
{
	cDisplay->setLinear(setter);
	linearButton->setChecked(setter);
}

void CurveWidget::doLoad()
{
	QString fileName;
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString wdir = dirs->get("curves", ".");
	CustomFDialog dia(this, wdir, tr("Open"), tr("Curve Files (*.scu *.SCU);;All Files (*)"), fdHidePreviewCheckBox | fdExistingFiles);
	if (dia.exec() == QDialog::Accepted)
		fileName = dia.selectedFile();
	else
		return;
	if (!fileName.isEmpty())
	{
		dirs->set("curves", fileName.left(fileName.lastIndexOf("/")));
		QFile f(fileName);
		if (f.open(QIODevice::ReadOnly))
		{
			QTextStream fp(&f);
			int numVals;
			double xval, yval;
			FPointArray curve;
			curve.resize(0);
			fp >> numVals;
			for (int nv = 0; nv < numVals; nv++)
			{
				QString s;
				fp >> s;
				xval = ScCLocale::toDoubleC(s);
				fp >> s;
				yval = ScCLocale::toDoubleC(s);
				curve.addPoint(xval, yval);
			}
			cDisplay->setCurve(curve);
			int lin;
			fp >> lin;
			cDisplay->setLinear(lin);
		}
	}
}

void CurveWidget::doSave()
{
	QString fileName;
	QString wdir = PrefsManager::instance()->prefsFile->getContext("dirs")->get("curves", ".");
	CustomFDialog dia(this, wdir, tr("Save as"), tr("Curve Files (*.scu *.SCU);;All Files (*)"), fdHidePreviewCheckBox | fdNone);
	if (dia.exec() == QDialog::Accepted)
		fileName = dia.selectedFile();
	else
		return;
	if (!fileName.isEmpty())
	{
		if (!fileName.endsWith(".scu"))
			fileName += ".scu";
		PrefsManager::instance()->prefsFile->getContext("dirs")->set("curves", fileName.left(fileName.lastIndexOf("/")));
		if (overwrite(this, fileName))
		{
			QString efval = "";
			FPointArray Vals = cDisplay->getCurve();
			QString tmp;
			tmp.setNum(Vals.size());
			efval += tmp;
			for (uint p = 0; p < Vals.size(); p++)
			{
				FPoint pv = Vals.point(p);
				efval += QString(" %1 %2").arg(pv.x()).arg(pv.y());
			}
			if (cDisplay->isLinear())
				efval += " 1";
			else
				efval += " 0";
			QFile fx(fileName);
			if (fx.open(QIODevice::WriteOnly))
			{
				QTextStream tsx(&fx);
				tsx << efval;
				fx.close();
			}
			else
				QMessageBox::warning(this, CommonStrings::trWarning, tr("Cannot write the file: \n%1").arg(fileName), CommonStrings::tr_OK);
		}
	}
}

void CurveWidget::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void CurveWidget::languageChange()
{
	invertButton->setText( QString::null );
	resetButton->setText( QString::null );
	loadButton->setText( QString::null );
	saveButton->setText( QString::null );
	invertButton->setToolTip("");
	resetButton->setToolTip("");
	linearButton->setToolTip("");
	loadButton->setToolTip("");
	saveButton->setToolTip("");
	invertButton->setToolTip( tr( "Inverts the curve" ) );
	resetButton->setToolTip( tr( "Resets the curve" ) );
	linearButton->setToolTip( tr( "Switches between linear and cubic interpolation of the curve" ) );
	loadButton->setToolTip( tr( "Loads a curve" ) );
	saveButton->setToolTip( tr( "Saves this curve" ) );
}


