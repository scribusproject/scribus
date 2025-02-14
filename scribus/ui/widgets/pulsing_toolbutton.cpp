#include "pulsing_toolbutton.h"

#include <QBrush>
#include <QColor>
#include <QPaintEvent>
#include <QPalette>
#include <QStyle>
#include <QStyleOptionButton>
#include <QStylePainter>
#include <QString>
#include <QToolButton>
#include <QVariant>
#include <QVariantAnimation>
#include <QWidget>

PulsingToolButton::PulsingToolButton(QWidget *parent):
	QToolButton(parent),
	m_transition(nullptr)
{
}

void PulsingToolButton::paintEvent(QPaintEvent* event)
{
	if (m_transition == nullptr)
	{
		QToolButton::paintEvent(event);
	}
	else
	{
		QStylePainter p(this);
		QStyleOptionToolButton opt;
		initStyleOption(&opt);
		p.drawComplexControl(QStyle::CC_ToolButton, opt);

		m_currentColor.setAlpha(100);
		p.setBrush(m_currentColor);
		p.setPen(Qt::NoPen);
		p.drawRect(0,0,width(),height());
	}
}

void PulsingToolButton::animatePulsing()
{
	if (!isChecked())
		return;

	// too fast, don't show a second animation
	if (m_transition != nullptr)
		return;

	m_transition = new QVariantAnimation(this);
	m_transition->setDuration(1000); // TODO: this should be a class const

	QColor startColor = palette().button().color();
	QColor endColor = palette().shadow().color();

	m_transition->setStartValue(startColor);
	m_transition->setEndValue(endColor);

	connect(m_transition, &QVariantAnimation::valueChanged, this, [this](const QVariant &value){m_currentColor = value.value<QColor>(); repaint();});
	connect(m_transition, &QVariantAnimation::destroyed, this, [this](){m_transition = nullptr; repaint();});

	m_transition->start(QAbstractAnimation::DeleteWhenStopped);
}
