#include "switch.h"
#include <QPainter>

Switch::Switch(QWidget *parent) : QAbstractButton(parent),
	m_anim(new QPropertyAnimation(this, "position", this))
{
	setCheckable(true);
	connect(this, &QAbstractButton::toggled, this, &Switch::animate);

	setSizeMode(Size::Small);
}

Switch::Size Switch::sizeMode()
{
	return m_size;
}

void Switch::setSizeMode(Size mode)
{
	m_size = mode;
	m_pos -= m_radius; // remove old radius

	switch(m_size)
	{
	case Size::Normal:
		m_radius = 10;
		break;
	case Size::Small:
		m_radius = 6;
		break;
	}

	m_pos += m_radius; // add new radius

	adjustSize();
	update();
}


QSize Switch::sizeHint() const
{
	return QSize(m_radius * 3 + m_margin * 2, m_radius * 2 + m_margin * 2);
}

void Switch::paintEvent(QPaintEvent *event)
{
	QColor cBackground;
	QColor cHandle = palette().color(QPalette::Button);

	if(isEnabled())
	{
		if(isChecked())
			cBackground = palette().color(QPalette::Highlight);
		else
			cBackground = palette().color(QPalette::Dark);
	}
	else
	{
		cBackground = palette().color(QPalette::Disabled, QPalette::Highlight);
		cHandle = palette().color(QPalette::Disabled, QPalette::Button);
	}

	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing, true);
	p.setPen(Qt::NoPen);
	p.setBrush(cBackground);
	p.drawRoundedRect(QRect(0, 0, width(), height()), height() / 2, height() / 2);
	p.setBrush(cHandle);
	p.drawEllipse(QRectF(position() - m_radius, height() / 2 - m_radius, m_radius * 2, m_radius * 2));
}

void Switch::animate(bool toggled)
{
	if (toggled) {
		m_anim->setStartValue(m_margin + m_radius);
		m_anim->setEndValue(width() - m_radius - m_margin);
		m_anim->setDuration(120);
		m_anim->start();
	} else {
		m_anim->setStartValue(position());
		m_anim->setEndValue(m_margin + m_radius);
		m_anim->setDuration(120);
		m_anim->start();
	}
}
