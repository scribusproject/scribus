/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "curvewidget.h"
#include "curvewidget.moc"

#include <cmath>

#include <qpushbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qpainter.h>
#include <qapplication.h>
#include <qcursor.h>
#include <qmessagebox.h>
#include "colorutil.h"
#include "customfdialog.h"
#include "util.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "commonstrings.h"

extern QPixmap loadIcon(QString nam);

KCurve::KCurve(QWidget *parent) : QWidget(parent)
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
	int    wWidth = width() - 1;
	int    wHeight = height() - 1;
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
	p1.drawLine(wWidth/4, 0, wWidth/4, wHeight);
	p1.drawLine(wWidth/2, 0, wWidth/2, wHeight);
	p1.drawLine(3*wWidth/4, 0, 3*wWidth/4, wHeight);
	p1.drawLine(0, wHeight/4, wWidth, wHeight/4);
	p1.drawLine(0, wHeight/2, wWidth, wHeight/2);
	p1.drawLine(0, 3*wHeight/4, wWidth, 3*wHeight/4);

	// Draw curve.
	double curvePrevVal = getCurveValue(0.0);
	p1.setPen(QPen::QPen(Qt::black, 1, Qt::SolidLine));
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
		repaint(false);
		emit modified();
	}
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
	m_points_back = m_points.copy();
	m_points.resize(0);
	m_points = inlist.copy();
	repaint(false);
	emit modified();
}

void KCurve::resetCurve()
{
	m_points.resize(0);
	m_points = m_points_back.copy();
	repaint(false);
	emit modified();
}

void KCurve::leaveEvent( QEvent * )
{
	qApp->setOverrideCursor(QCursor(ArrowCursor), true);
}

CurveWidget::CurveWidget( QWidget* parent ) : QWidget( parent )
{
	CurveWidgetLayout = new QHBoxLayout( this, 5, 5, "CurveWidgetLayout");

	layout1 = new QVBoxLayout( 0, 0, 5, "layout1");

	invertButton = new QPushButton( this, "invertButton" );
	invertButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, invertButton->sizePolicy().hasHeightForWidth() ) );
	invertButton->setPixmap( loadIcon("invert.png") );
	layout1->addWidget( invertButton );

	resetButton = new QPushButton( this, "resetButton" );
	resetButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, resetButton->sizePolicy().hasHeightForWidth() ) );
	resetButton->setPixmap( loadIcon("reload.png") );
	layout1->addWidget( resetButton );
	spacer1 = new QSpacerItem( 21, 31, QSizePolicy::Minimum, QSizePolicy::Expanding );
	layout1->addItem( spacer1 );

	loadButton = new QPushButton( this, "loadButton" );
	loadButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, loadButton->sizePolicy().hasHeightForWidth() ) );
	loadButton->setPixmap( loadIcon("22/document-open.png") );
	layout1->addWidget( loadButton );

	saveButton = new QPushButton( this, "saveButton" );
	saveButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, saveButton->sizePolicy().hasHeightForWidth() ) );
	saveButton->setPixmap( loadIcon("22/document-save-as.png") );
	layout1->addWidget( saveButton );
	CurveWidgetLayout->addLayout( layout1 );

	cDisplay = new KCurve(this);
	cDisplay->setMinimumSize( QSize( 150, 150 ) );
	CurveWidgetLayout->addWidget( cDisplay );
	languageChange();
	clearWState( WState_Polished );
	connect(invertButton, SIGNAL(clicked()), this, SLOT(doInvert()));
	connect(resetButton, SIGNAL(clicked()), this, SLOT(doReset()));
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

void CurveWidget::doLoad()
{
	QString fileName;
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString wdir = dirs->get("curves", ".");
	CustomFDialog dia(this, wdir, tr("Open"), tr("Curve Files (*.scu);;All Files (*)"));
	if (dia.exec() == QDialog::Accepted)
		fileName = dia.selectedFile();
	else
		return;
	if (!fileName.isEmpty())
	{
		if (!fileName.endsWith(".scu"))
			fileName += ".scu";
		dirs->set("curves", fileName.left(fileName.findRev("/")));
		QFile f(fileName);
		if (f.open(IO_ReadOnly))
		{
			QTextStream fp(&f);
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
			cDisplay->setCurve(curve);
		}
	}
}

void CurveWidget::doSave()
{
	QString fileName;
	QString wdir = PrefsManager::instance()->prefsFile->getContext("dirs")->get("curves", ".");
	CustomFDialog dia(this, wdir, tr("Save as"), tr("Curve Files (*.scu);;All Files (*)"), fdNone);
	if (dia.exec() == QDialog::Accepted)
		fileName = dia.selectedFile();
	else
		return;
	if (!fileName.isEmpty())
	{
		PrefsManager::instance()->prefsFile->getContext("dirs")->set("curves", fileName.left(fileName.findRev("/")));
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
			QFile fx(fileName);
			if (fx.open(IO_WriteOnly))
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

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CurveWidget::languageChange()
{
	invertButton->setText( QString::null );
	resetButton->setText( QString::null );
	loadButton->setText( QString::null );
	saveButton->setText( QString::null );
	QToolTip::remove( invertButton );
	QToolTip::remove( resetButton );
	QToolTip::remove( loadButton );
	QToolTip::remove( saveButton );
	QToolTip::add( invertButton, tr( "Inverts the curve" ) );
	QToolTip::add( resetButton, tr( "Resets the curve" ) );
	QToolTip::add( loadButton, tr( "Loads a curve" ) );
	QToolTip::add( saveButton, tr( "Saves this curve" ) );
}

