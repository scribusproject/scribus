/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "basepointwidget.h"

/*BasePointWidget::BasePointWidget(QWidget *pa, int mode)
	: QWidget(pa),
	  m_mode(mode)
{
	if (m_mode == 0)
	{
		setMinimumSize(QSize(48, 48));
		setMaximumSize(QSize(48, 48));
	}
	else
	{
		setMinimumSize(QSize(48, 16));
		setMaximumSize(QSize(48, 16));
	}
	setMouseTracking(true);
} 

void BasePointWidget::paintEvent(QPaintEvent *e)
{
	QPainter p;
	p.begin(this);
	p.setRenderHint(QPainter::Antialiasing, true);
	p.setBrush(palette().color(QPalette::Window));
	p.setPen(Qt::NoPen);
	p.drawRect(0, 0, width(), height());
	p.setBrush(Qt::black);
	p.setPen(QPen(palette().color(QPalette::WindowText), 1));
	if (m_mode == 0)
	{
		p.drawEllipse( 4,  4, 10, 10);
		p.drawEllipse(34,  4, 10, 10);
		p.drawEllipse(19, 19, 10, 10);
		p.drawEllipse( 4, 34, 10, 10);
		p.drawEllipse(34, 34, 10, 10);
	}
	else
	{
		p.drawEllipse( 4,  4, 10, 10);
		p.drawEllipse(19,  4, 10, 10);
		p.drawEllipse(34,  4, 10, 10);
	}
	p.setPen(QPen(palette().color(QPalette::WindowText), 2));
	if (m_mode == 0)
	{
		p.drawLine(8, 8, 33, 8);
		p.drawLine(8, 38, 33, 38);
		p.drawLine(8, 8, 8, 33);
		p.drawLine(38, 8, 38, 33);
	}
	else
		p.drawLine(8, 8, 33, 8);
	p.setBrush(Qt::white);
	p.setPen(QPen(palette().color(QPalette::WindowText), 2));
	if (m_mode == 0)
	{
		p.drawEllipse( 3,  3, 10, 10);
		p.drawEllipse(33,  3, 10, 10);
		p.drawEllipse(18, 18, 10, 10);
		p.drawEllipse( 3, 33, 10, 10);
		p.drawEllipse(33, 33, 10, 10);
	}
	else
	{
		p.drawEllipse( 3,  3, 10, 10);
		p.drawEllipse(18,  3, 10, 10);
		p.drawEllipse(33,  3, 10, 10);
	}
	if (m_hooveredBase > -1)
	{
		p.setBrush(Qt::NoBrush);
		p.setPen(QPen(palette().color(QPalette::Highlight), 1));
		if (m_hooveredBase == 0)
			p.drawEllipse( 4,  4, 8, 8);
		else if (m_hooveredBase == 1)
			p.drawEllipse(34,  4, 8, 8);
		else if (m_hooveredBase == 2)
		{
			if (m_mode == 0)
				p.drawEllipse(19, 19, 8, 8);
			else
				p.drawEllipse(19,  4, 8, 8);
		}
		else if (m_hooveredBase == 3)
			p.drawEllipse( 4, 34, 8, 8);
		else if (m_hooveredBase == 4)
			p.drawEllipse(34, 34, 8, 8);
	}
	p.setBrush(palette().color(QPalette::WindowText));
	p.setPen(Qt::NoPen);
	if (m_selectedBase == 0)
		p.drawEllipse( 5, 5, 6, 6);
	else if (m_selectedBase == 1)
		p.drawEllipse(35, 5, 6, 6);
	else if (m_selectedBase == 2)
	{
		if (m_mode == 0)
			p.drawEllipse(20, 20, 6, 6);
		else
			p.drawEllipse(20,  5, 6, 6);
	}
	else if (m_selectedBase == 3)
		p.drawEllipse( 5, 35, 6, 6);
	else if (m_selectedBase == 4)
		p.drawEllipse(35, 35, 6, 6);
	p.end();
}

void BasePointWidget::mousePressEvent(QMouseEvent *m)
{
	m->accept();
	m_mousePressed = true;
}

void BasePointWidget::mouseReleaseEvent(QMouseEvent *m)
{
	m->accept();
	if (m_mousePressed)
	{
		QPoint p = m->pos();
		if (m_mode == 0)
		{
			if (QRect( 3,  3, 10, 10).contains(p))
				m_selectedBase = 0;
			else if (QRect(33,  3, 10, 10).contains(p))
				m_selectedBase = 1;
			else if (QRect(18, 18, 10, 10).contains(p))
				m_selectedBase = 2;
			else if (QRect( 3, 33, 10, 10).contains(p))
				m_selectedBase = 3;
			else if (QRect(33, 33, 10, 10).contains(p))
				m_selectedBase = 4;
		}
		else
		{
			if (QRect( 3,  3, 9, 9).contains(p))
				m_selectedBase = 0;
			else if (QRect(33,  3, 10, 10).contains(p))
				m_selectedBase = 1;
			else if (QRect(18,  3, 10, 10).contains(p))
				m_selectedBase = 2;
		}
		m_mousePressed = false;
		emit buttonClicked(m_selectedBase);
	}
	update();
}

void BasePointWidget::mouseMoveEvent(QMouseEvent *m)
{
	m->accept();
	QPoint p = m->pos();
	m_hooveredBase = -1;
	if (m_mode == 0)
	{
		if (QRect( 3,  3, 10, 10).contains(p))
			m_hooveredBase = 0;
		else if (QRect(33,  3, 10, 10).contains(p))
			m_hooveredBase = 1;
		else if (QRect(18, 18, 10, 10).contains(p))
			m_hooveredBase = 2;
		else if (QRect( 3, 33, 10, 10).contains(p))
			m_hooveredBase = 3;
		else if (QRect(33, 33, 10, 10).contains(p))
			m_hooveredBase = 4;
	}
	else
	{
		if (QRect( 3,  3, 10, 10).contains(p))
			m_hooveredBase = 0;
		else if (QRect(33,  3, 10, 10).contains(p))
			m_hooveredBase = 1;
		else if (QRect(18,  3, 10, 10).contains(p))
			m_hooveredBase = 2;
	}
	update();
}

void BasePointWidget::leaveEvent(QEvent*)
{
	m_hooveredBase = -1;
	update();
}

int BasePointWidget::checkedId() const
{
	return m_selectedBase;
}

void BasePointWidget::setCheckedId(int id)
{
	m_selectedBase = id;
	update();
}
*/

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
	m_hoveredAnchor = AnchorPoint::None;
	m_pressed = false;
	anchors = QMap<AnchorPoint, QRect>();
	m_angle = 0;

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
		m_selectedAnchor = AnchorPoint::TopLeft;

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
		m_selectedAnchor = AnchorPoint::TopLeft;

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
		m_selectedAnchor = AnchorPoint::Left;

		anchors.insert(AnchorPoint::Left,        QRect(p             ,p + a + s      ,a  ,a) );
		anchors.insert(AnchorPoint::Center,      QRect(p + a + s     ,p + a + s      ,a  ,a) );
		anchors.insert(AnchorPoint::Right,       QRect(p +(a + s) *2 ,p + a + s      ,a  ,a) );
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

	//    QColor normal( ColorUtils::colorBlend( palette().color(QPalette::WindowText), QColor(128,128,128), 0.8 ));
	//    QColor dark( ColorUtils::colorAlpha( palette().color(QPalette::WindowText), 0.15 ));

	QColor normal( palette().color(QPalette::WindowText) );
	normal = QColor::fromHslF(normal.hslHueF(), normal.hslSaturationF(), (normal.lightnessF() < 0.5) ? 0.4 : 0.6);

	QColor dark( palette().color(QPalette::WindowText) );
	dark.setAlphaF(0.15f);

	// Base
	p.setBrush(QBrush( dark ));
	p.setPen(QPen( normal, 1) );
	p.drawRect(m_baseRect);

	p.setPen(Qt::NoPen);

	// Anchor Normal
	p.setBrush(QBrush( normal ));

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

//int BasePointWidget::checkedId()
//{
//    return selectedAnchor();
//}

void BasePointWidget::setSelectedAnchor(AnchorPoint id)
{
	m_selectedAnchor = id;
	update();
}

//void BasePointWidget::setCheckedId(int id)
//{
//    setSelectedAnchor(static_cast<AnchorPoint>(id));
//}

void BasePointWidget::setMode(Mode mode)
{
	m_mode = mode;
	anchors.clear();
	setup();
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
