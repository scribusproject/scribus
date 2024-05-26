/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "basepointwidget.h"
#include "util_gui.h"

/* ********************************************************************************* *
 *
 * Constructor + Setup
 *
 * ********************************************************************************* */

BasePointWidget::BasePointWidget(Mode mode, QWidget *parent) : QWidget(parent)
{
	setFixedSize(QSize(51, 51));
	setMouseTracking(true);

	m_mode = mode;

	setup();
}

BasePointWidget::BasePointWidget(QWidget *parent) : BasePointWidget(BasePointWidget::Full, parent){}

void BasePointWidget::setup()
{
	int p = 8; // save area padding for rotation
	int s = 4; // space
	int a = 9; // anchor

	// Widget / Save Area ratio = 1:1.4
	// Sizes for rows and columns for square
	// |   p   |    a   |   s   |   a   |   s   |   a   |   p   |
	// |   8   |    8   |   5   |   8   |   5   |   8   |   8   | = 50
	// |   8   |    9   |   4   |   9   |   4   |   9   |   8   | = 51

	// Calculated TopLeft points of anchors
	// |       |  8,8   |       | 21,15 |       | 34,8  |       |
	// |       |  8,21  |       | 21,21 |       | 34,28 |       |
	// |       |  8,34  |       | 21,34 |       | 34,34 |       |

	// Add available anchor points

	switch(m_mode){
	case Mode::Full:
		m_baseRect = QRectF(p + a/2,
							p + a/2,
							(a + s) *2,
							(a + s) *2
							).adjusted(0.5,0.5,0.5,0.5);

		anchors.insert(AnchorPoint::TopLeft,     QRect(p             ,p              ,a  ,a) );
		anchors.insert(AnchorPoint::Top,         QRect(p + a + s     ,p              ,a  ,a) );
		anchors.insert(AnchorPoint::TopRight,    QRect(p +(a + s) *2 ,p              ,a  ,a) );
		anchors.insert(AnchorPoint::Left,        QRect(p             ,p + a + s      ,a  ,a) );
		anchors.insert(AnchorPoint::Center,      QRect(p + a + s     ,p + a + s      ,a  ,a) );
		anchors.insert(AnchorPoint::Right,       QRect(p +(a + s) *2 ,p + a + s      ,a  ,a) );
		anchors.insert(AnchorPoint::BottomLeft,  QRect(p             ,p +(a + s) *2  ,a  ,a) );
		anchors.insert(AnchorPoint::Bottom,      QRect(p + a + s     ,p +(a + s) *2  ,a  ,a) );
		anchors.insert(AnchorPoint::BottomRight, QRect(p +(a + s) *2 ,p +(a + s) *2  ,a  ,a) );

		break;
	case Mode::Frame:
		m_baseRect = QRectF(p + a/2,
							p + a/2,
							(a + s) *2,
							(a + s) *2
							).adjusted(0.5,0.5,0.5,0.5);

		anchors.insert(AnchorPoint::TopLeft,     QRect(p             ,p              ,a  ,a) );
		anchors.insert(AnchorPoint::TopRight,    QRect(p +(a + s) *2 ,p              ,a  ,a) );
		anchors.insert(AnchorPoint::Center,      QRect(p + a + s     ,p + a + s      ,a  ,a) );
		anchors.insert(AnchorPoint::BottomLeft,  QRect(p             ,p +(a + s) *2  ,a  ,a) );
		anchors.insert(AnchorPoint::BottomRight, QRect(p +(a + s) *2 ,p +(a + s) *2  ,a  ,a) );

		break;
	case Mode::Line:
		m_baseRect = QRectF(p + a/2,
							p + a + s + a/2,
							(a + s) *2,
							1
							).adjusted(0.5,0.5,-0.5,-0.5);

		anchors.insert(AnchorPoint::Left,		QRect(p             ,p + a + s      ,a  ,a) );
		anchors.insert(AnchorPoint::Center,		QRect(p + a + s     ,p + a + s      ,a  ,a) );
		anchors.insert(AnchorPoint::Right,		QRect(p +(a + s) *2 ,p + a + s      ,a  ,a) );
		break;

	}
}


/* ********************************************************************************* *
 *
 * Events
 *
 * ********************************************************************************* */

void BasePointWidget::paintEvent(QPaintEvent *e)
{
	Q_UNUSED(e)

	QPainter p;
	p.begin(this);

	p.setRenderHint(QPainter::Antialiasing, true);
	p.setTransform(m_transform, true);

	QColor frame = colorByRole(QPalette::WindowText, 0.6, isEnabled());
	QColor bg = colorByRole(QPalette::WindowText, 0.15, isEnabled());

	// Base
	p.setBrush(QBrush( bg ));
	p.setPen(QPen( frame, 1) );
	p.drawRect(m_baseRect);

	p.setPen(Qt::NoPen);

	// Anchor Normal
	p.setBrush(QBrush( frame ));

	QMapIterator<AnchorPoint, QRect> i(anchors);
	while (i.hasNext()) {
		i.next();
		p.drawRect( i.value().adjusted(2,2,-2,-2));
	}


	// Anchor Hovered
	if (m_hoveredAnchor != AnchorPoint::None)
	{
		p.setBrush(QBrush(palette().color(QPalette::Highlight)));
		p.drawRect(anchors.value(m_hoveredAnchor));
	}


	// Anchor Selected
	p.setBrush(palette().color(QPalette::WindowText));
	p.drawRect(anchors.value(m_selectedAnchor));

	p.end();

}

void BasePointWidget::mousePressEvent(QMouseEvent *m)
{
	m->accept();
	m_pressed = true;
}

void BasePointWidget::mouseReleaseEvent(QMouseEvent *m)
{
	m->accept();
	if (m_pressed)
	{
		QPoint p = mapToTransformation(m->pos());

		QMapIterator<AnchorPoint, QRect> i(anchors);
		while (i.hasNext()) {
			i.next();
			if(i.value().contains(p)) m_selectedAnchor = i.key();
		}

		emit buttonClicked(m_selectedAnchor);
	}
	update();
}

void BasePointWidget::mouseMoveEvent(QMouseEvent *m)
{
	m->accept();

	QPoint p = mapToTransformation(m->pos());

	m_hoveredAnchor = AnchorPoint::None;

	QMapIterator<AnchorPoint, QRect> i(anchors);
	while (i.hasNext()) {
		i.next();
		if(i.value().contains(p)) m_hoveredAnchor = i.key();
	}

	update();
}

void BasePointWidget::leaveEvent(QEvent*)
{
	m_hoveredAnchor = AnchorPoint::None;
	update();
}

/* ********************************************************************************* *
 *
 * Helper
 *
 * ********************************************************************************* */

QPoint BasePointWidget::mapToTransformation(QPoint position)
{
	return position * m_transform.inverted();
}


/* ********************************************************************************* *
 *
 * Properties
 *
 * ********************************************************************************* */

AnchorPoint BasePointWidget::selectedAnchor()
{
	return m_selectedAnchor;
}

void BasePointWidget::setSelectedAnchor(AnchorPoint id)
{
	// check if current mode supports Anchor
	switch(m_mode){
	case Mode::Full:
		m_selectedAnchor = id;
		break;
	case Mode::Frame:

		switch(id){
		case AnchorPoint::TopLeft:
		case AnchorPoint::Top:
			m_selectedAnchor = AnchorPoint::TopLeft;
			break;
		case AnchorPoint::TopRight:
		case AnchorPoint::Right:
			m_selectedAnchor = AnchorPoint::TopRight;
			break;
		case AnchorPoint::BottomRight:
		case AnchorPoint::Bottom:
			m_selectedAnchor = AnchorPoint::BottomRight;
			break;
		case AnchorPoint::BottomLeft:
		case AnchorPoint::Left:
			m_selectedAnchor = AnchorPoint::BottomLeft;
			break;
		case AnchorPoint::Center:
			m_selectedAnchor = AnchorPoint::Center;
			break;
		case AnchorPoint::None:
			m_selectedAnchor = AnchorPoint::None;
			break;
		}

		break;
	case Mode::Line:

		switch(id){
		case AnchorPoint::TopLeft:
		case AnchorPoint::Left:
		case AnchorPoint::BottomLeft:
			m_selectedAnchor = AnchorPoint::Left;
			break;
		case AnchorPoint::Top:
		case AnchorPoint::Center:
		case AnchorPoint::Bottom:
			m_selectedAnchor = AnchorPoint::Center;
			break;
		case AnchorPoint::TopRight:
		case AnchorPoint::Right:
		case AnchorPoint::BottomRight:
			m_selectedAnchor = AnchorPoint::Right;
			break;
		case AnchorPoint::None:
			m_selectedAnchor = AnchorPoint::None;
			break;
		}
	}

	emit buttonClicked(m_selectedAnchor);

	update();
}

void BasePointWidget::setMode(Mode mode)
{
	if(m_mode == mode) return; // avoid basepoint reset

	AnchorPoint selected = selectedAnchor();
	m_mode = mode;
	anchors.clear();
	setup();
	setSelectedAnchor(selected);
	update();
}

void BasePointWidget::setAngle(int angle)
{
	m_angle = -angle;
	QPoint center = rect().center();
	m_transform = QTransform()
			.translate( center.x() + 1, center.y() + 1 )
			.rotate( m_angle )
			.translate( -center.x() - 1, -center.y() - 1 );

	update();
}

int BasePointWidget::angle()
{
	return m_angle;
}

BasePointWidget::Mode BasePointWidget::mode()
{
	return m_mode;
}
