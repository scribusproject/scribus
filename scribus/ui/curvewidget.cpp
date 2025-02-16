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
#include "iconmanager.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scclocale.h"
#include "ui/customfdialog.h"
#include "ui/scmessagebox.h"
#include "util.h"
#include "util_math.h"

/* ********************************************************************************* *
 *
 * Constructor + Setup
 *
 * ********************************************************************************* */

const qreal POINT_RADIUS = 4.0;
const qreal TOLERANCE = 2.0; // pixel tolerance to hit a point

KCurve::KCurve(QWidget *parent) : QWidget(parent)
{
	setMouseTracking(true);
	setMinimumSize(150, 150);
	m_points.resize(0);
	m_points.addPoint(0.0, 0.0);
	m_points.addPoint(1.0, 1.0);
	m_points_back = m_points.copy();

	setFocusPolicy(Qt::StrongFocus);
}

void KCurve::paintEvent(QPaintEvent *)
{
	int wWidth = width();
	int wHeight = height();
	QPointF offset(-.5, -.5);
	QColor colorHandle = palette().color(QPalette::Highlight);
	QColor colorBackground = palette().color(QPalette::Base);
	QColor colorGraph = palette().color(QPalette::WindowText);
	QColor colorGrid = palette().color(QPalette::Midlight); // Qt::gray
	QPainterPath path = curvePath();

	// Drawing selection or all histogram values
	QPainter painter;
	painter.begin(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

	// Draw background
	painter.fillRect(this->rect(), colorBackground);

	// Draw grid separators
	painter.setPen(QPen(colorGrid, 1, Qt::SolidLine));
	painter.drawLine( QPoint(wWidth * .25, 0) - offset, QPoint(wWidth * .25, wHeight) - offset );
	painter.drawLine( QPoint(wWidth * .5, 0) - offset, QPoint(wWidth * .5, wHeight) - offset );
	painter.drawLine( QPoint(wWidth * .75, 0) - offset, QPoint(wWidth * .75, wHeight) - offset );
	painter.drawLine( QPoint(0, wHeight * .25) - offset, QPoint(wWidth, wHeight * .25) - offset );
	painter.drawLine( QPoint(0, wHeight * .5) - offset, QPoint(wWidth, wHeight * .5) - offset );
	painter.drawLine( QPoint(0, wHeight * .75) - offset, QPoint(wWidth, wHeight * .75) - offset );

	// Draw curve
	painter.setPen(QPen(colorGraph, 2, Qt::SolidLine));
	painter.drawPath(path);
	painter.setBrush(Qt::NoBrush);

	// Draw handles
	for (int i = 0; i < m_points.size(); i++)
	{
		FPoint p = m_points.point(i);
		QPointF dot(p.x() * width(), (1 - p.y()) * height());

		if (i == selectedPoint())
		{
			painter.setPen(QPen(colorHandle, 2, Qt::SolidLine));
			painter.setBrush(colorHandle);
			painter.drawEllipse( dot, POINT_RADIUS, POINT_RADIUS );
		}
		else
		{
			painter.setPen(QPen(colorGraph, 2, Qt::SolidLine));
			painter.setBrush(colorBackground);
			painter.drawEllipse( dot, POINT_RADIUS, POINT_RADIUS );
		}
	}

	painter.end();
}

void KCurve::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Delete || e->key() == Qt::Key_Backspace)
	{
		if (m_points.size() > 2)
		{
			m_points.removeAt(selectedPoint());

			m_selectedPoint = qBound(0, m_selectedPoint - 1, m_points.size() - 1);

			repaint();
			emit modified();
			QWidget::keyPressEvent(e);
		}
	}
}

void KCurve::mousePressEvent ( QMouseEvent * e )
{
	if (e->button() != Qt::LeftButton)
		return;

	// invert mouseY
	m_mousePos = QPointF( e->pos().x(), (height() - e->pos().y()));

	m_leftmost = - 1;
	m_selectedPoint = 0;
	m_rightmost = m_points.size();

	m_dragging = false;

	for (int i = 0; i < m_points.size(); i++)
	{
		QPointF pt(m_points[i].x() * width(), m_points[i].y() * height());

		// calculate left point of mouse position
		if (pt.x() >= 0 && pt.x() <= m_mousePos.x())
			m_leftmost = i;

		if (manhattanDistance(pt, m_mousePos) <= POINT_RADIUS + TOLERANCE)
		{
			m_selectedPoint = i;
			m_leftmost = i - 1;
			m_rightmost = i + 1;

			m_dragging = true;
			repaint();
			return;
		}
	}

	// add new point
	if (!m_dragging)
	{
		FPoint onCurve(m_mousePos.x() / width(), m_mousePos.y() / height());
		m_selectedPoint = m_leftmost + 1;
		m_rightmost = m_leftmost + 2;
		m_points.insert(m_selectedPoint, onCurve );

		m_dragging = true;

		repaint();
		emit modified();
	}
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
	if (!m_dragging)
		return;

	setCursor(QCursor(Qt::CrossCursor));

	// invert mouseY
	QPointF mouse(e->pos().x(), height() - e->pos().y());

	// left bound
	qreal leftBound = (m_leftmost < 0) ? 0. : m_points.at(m_leftmost).x();
	qreal rightBound = (m_rightmost >= m_points.size()) ? 1. : m_points.at(m_rightmost).x();

	// limit to bounds
	qreal yPos = qBound(0., mouse.y() / height(), 1.);
	qreal xPos = qBound(leftBound + 1E-4, mouse.x() / width(), rightBound - 1E-4);

	QPointF pointSelectedDot(xPos, yPos );

	// set new position
	m_points.setPoint( m_selectedPoint, pointSelectedDot);

	repaint();
	emit modified();
}

FPointArray KCurve::getCurve() const
{
	return m_points.copy();
}

QPainterPath KCurve::curvePath() const
{
	QPainterPath path;

	if (m_points.size() <= 0)
	{
		qWarning() << Q_FUNC_INFO << "Can't create path because there are no path nodes!";
		return path;
	}

	QPointF pointStart( 0, (1 - m_points.at(0).y()) * height() );
	QPointF pointEnd( width(), (1 - m_points.at(m_points.size() - 1).y()) * height() );

	QList<QPointF> points;
	points.append( pointStart );

	// Calculate absolute position of each point
	for (int i = 0; i < m_points.size(); i++)
	{
		FPoint p = m_points.at(i);
		points.append(QPointF(p.x() * width(), (1 - p.y()) * height()));
	}

	points.append( pointEnd );

	// Add start line
	path.moveTo( pointStart );
	path.lineTo( points.at(1) );

	for (int id = 0; id < points.size(); id++)
	{
		if (m_linear)
		{
			path.lineTo(points.at(id).toPoint());
			continue;
		}

		int i0 = id - 1;
		int i1 = id;
		int i2 = id + 1;
		int i3 = id + 2;

		if (i0 < 0 || i2 > points.size() - 1 || i3 > points.size() - 1)
			continue;

		FPoint p0 = (i0 >= 0) ? points.at(i0) : points.at(i1);
		FPoint p1 = points.at(i1);
		FPoint p2 = (i2 < points.size()) ? points.at(i2) : points.at(points.size() - 1);
		FPoint p3 = (i3 < points.size()) ? points.at(i3) : points.at(points.size() - 1);

		QList<QPointF> inputPoints;
		inputPoints.append(p0.toQPointF()); // Previous point
		inputPoints.append(p1.toQPointF()); // Point
		inputPoints.append(p2.toQPointF()); // Next point
		inputPoints.append(p3.toQPointF()); // Next but one point

		double t = qMax(width() / (p2.x() - p1.x()) / 6, 1.);

		QList<QPointF> bezPoints = catmullToBezier(inputPoints, t);

		path.cubicTo(
			bezPoints.at(1), // Control 1
			bezPoints.at(2), // Control 2
			bezPoints.at(3) // Next Point
		);
	}

	// Add end line
	path.lineTo(pointEnd);

	return path;
}

void KCurve::setCurve(const FPointArray& inlist)
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

bool KCurve::isLinear() const
{
	return m_linear;
}

int KCurve::selectedPoint() const
{
	return m_points.size() > 0 ? qBound(0, m_selectedPoint, m_points.size() -1) : -1;
}


/* ********************************************************************************* *
 *
 * Constructor + Setup
 *
 * ********************************************************************************* */

CurveWidget::CurveWidget( QWidget* parent ) : QWidget( parent )
{
	CurveWidgetLayout = new QHBoxLayout(this);
	CurveWidgetLayout->setContentsMargins(0, 0, 0, 0);
	CurveWidgetLayout->setSpacing(6);

	layout1 = new QVBoxLayout;
	layout1->setContentsMargins(0, 0, 0, 0);
	layout1->setSpacing(6);

	invertButton = new QPushButton( this );
	invertButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	invertButton->setIcon( IconManager::instance().loadIcon("invert") );
	invertButton->setIconSize(QSize(22, 22));
	layout1->addWidget( invertButton );

	resetButton = new QPushButton( this );
	resetButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	resetButton->setIcon( IconManager::instance().loadIcon("reload") );
	resetButton->setIconSize(QSize(22, 22));
	layout1->addWidget( resetButton );
	linearButton = new QPushButton( this );
	QIcon ic;
	ic.addPixmap(IconManager::instance().loadPixmap("curve-bezier"), QIcon::Normal, QIcon::Off);
	ic.addPixmap(IconManager::instance().loadPixmap("curve-linear"), QIcon::Normal, QIcon::On);
	linearButton->setIcon(ic);
	linearButton->setCheckable( true );
	linearButton->setChecked(false);
	linearButton->setIconSize(QSize(22, 22));
	linearButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	layout1->addWidget( linearButton );
	spacer1 = new QSpacerItem( 2, 3, QSizePolicy::Minimum, QSizePolicy::Expanding );
	layout1->addItem( spacer1 );

	loadButton = new QPushButton( this );
	loadButton->setIcon( IconManager::instance().loadIcon("document-open", 22) );
	loadButton->setIconSize(QSize(22, 22));
	loadButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	layout1->addWidget( loadButton );

	saveButton = new QPushButton( this );
	saveButton->setIcon( IconManager::instance().loadIcon("document-save-as") );
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
	for (int a = 0; a < curve.size(); a++)
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
	PrefsContext* dirs = PrefsManager::instance().prefsFile->getContext("dirs");
	QString wdir = dirs->get("curves", ".");
	CustomFDialog dia(this, wdir, tr("Open"), tr("Curve Files (*.scu *.SCU);;All Files (*)"), fdHidePreviewCheckBox | fdExistingFiles);
	if (dia.exec() != QDialog::Accepted)
		return;
		
	QString fileName = dia.selectedFile();
	if (fileName.isEmpty())
		return;
	dirs->set("curves", fileName.left(fileName.lastIndexOf("/")));

	QFile f(fileName);
	if (!f.open(QIODevice::ReadOnly))
		return;

	QTextStream fp(&f);
	int numVals;
	double xval, yval;
	FPointArray curve;
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

void CurveWidget::doSave()
{
	QString wdir = PrefsManager::instance().prefsFile->getContext("dirs")->get("curves", ".");
	CustomFDialog dia(this, wdir, tr("Save as"), tr("Curve Files (*.scu *.SCU);;All Files (*)"), fdHidePreviewCheckBox | fdNone);
	if (dia.exec() != QDialog::Accepted)
		return;

	QString fileName = dia.selectedFile();
	if (fileName.isEmpty())
		return;
	if (!fileName.endsWith(".scu"))
		fileName += ".scu";

	PrefsManager::instance().prefsFile->getContext("dirs")->set("curves", fileName.left(fileName.lastIndexOf("/")));
	if (!overwrite(this, fileName))
		return;

	QString efval;
	FPointArray Vals = cDisplay->getCurve();
	QString tmp;
	tmp.setNum(Vals.size());
	efval += tmp;
	for (int p = 0; p < Vals.size(); p++)
	{
		const FPoint& pv = Vals.point(p);
		efval += QString(" %1 %2").arg(pv.x()).arg(pv.y());
	}

	if (cDisplay->isLinear())
		efval += " 1";
	else
		efval += " 0";

	QFile fx(fileName);
	if (!fx.open(QIODevice::WriteOnly))
	{
		ScMessageBox::warning(this, CommonStrings::trWarning, tr("Cannot write the file: \n%1").arg(fileName));
		return;
	}
	QTextStream tsx(&fx);
	tsx << efval;
	fx.close();
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
	invertButton->setText( QString() );
	resetButton->setText( QString() );
	loadButton->setText( QString() );
	saveButton->setText( QString() );
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


