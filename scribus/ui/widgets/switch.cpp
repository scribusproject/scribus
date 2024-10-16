#include "switch.h"
#include <QPainter>
#include <QShortcutEvent>
#include <QStyleOption>
#include "util_gui.h"

Switch::Switch(QWidget *parent) : QAbstractButton(parent),
	m_anim(new QPropertyAnimation(this, "position", this))
{
	QAbstractButton::setText("");
	setCheckable(true);
	connect(this, &QAbstractButton::toggled, this, &Switch::animate);

	setSizeMode(Size::Small);
}

Switch::Size Switch::sizeMode() const
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

void Switch::setText(QString const &text)
{
	QAbstractButton::setText(text);
	updateShortcut();
	adjustSize();
	update();
}


QSize Switch::sizeHint() const
{	
	int label = (text().isEmpty()) ? 0 : m_space + textSize().width();
	return QSize(switchSize().width() + label, qMax(switchSize().height(), textSize().height()));
}

QSize Switch::textSize() const
{
	QFontMetrics metrics(font());
	QString label = text();

	// calculate width without hidden "&"
	if (m_hasShortcut)
	{
		int pos = 0;
		while ( pos < text().size())
		{
			int index = label.indexOf(QLatin1String("&"), pos);

			if (pos == index)
				label = label.replace(index, 1, "");

			pos++;
		}
	}

	return QSize(metrics.horizontalAdvance(label) + metrics.horizontalAdvance(QLatin1Char(' ')), metrics.height());
}

QSize Switch::switchSize() const
{
	return QSize(m_radius * 3 + m_margin * 2, m_radius * 2 + m_margin * 2);
}

void Switch::paintEvent(QPaintEvent *event)
{
	QColor cBackground;
	QColor cHandle = colorByRole(QPalette::Button, 1, isEnabled());

	if(isChecked())
		cBackground = colorByRole(QPalette::Highlight, 1, isEnabled());
	else
		cBackground = colorByRole(QPalette::WindowText, 0.6, isEnabled());

	QStyle *style = QWidget::style();
	QStyleOption opt;
	opt.initFrom(this);

	QRect rSwitch(0, 0, switchSize().width(), switchSize().height());
	rSwitch.moveCenter(opt.rect.center());
	rSwitch.moveLeft(0);

	QPainter p(this);

	// Draw switch
	p.setRenderHint(QPainter::Antialiasing, true);
	p.setPen(Qt::NoPen);
	p.setBrush(cBackground);
	p.drawRoundedRect(rSwitch, rSwitch.height() / 2, rSwitch.height() / 2);
	p.setBrush(cHandle);	
	p.drawEllipse(QRectF(position() - m_radius, height() / 2 - m_radius, m_radius * 2, m_radius * 2));

	// Draw Focus State
	if (opt.state & QStyle::State_HasFocus)
	{
		p.setBrush(colorByRole(QPalette::Highlight, .2, isEnabled()));
		p.drawEllipse(QRectF(position() - m_radius, height() / 2 - m_radius, m_radius * 2, m_radius * 2));
	}

	// Draw label
	QRect rLabel(opt.rect.adjusted(rSwitch.width() + m_space, 0, 0, 0));
	style->drawItemText(&p, rLabel, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextShowMnemonic, opt.palette, isEnabled(), text(), foregroundRole());
}

bool Switch::event(QEvent *e)
{

#ifndef EXCLUDE_FOR_DESIGNER_PLUGIN
	QEvent::Type type = e->type();
	if (type == QEvent::Shortcut)
	{
		QShortcutEvent *se = static_cast<QShortcutEvent *>(e);
		if (se->shortcutId() == m_shortcutId)
		{
			this->setFocus(Qt::ShortcutFocusReason);
			window()->setAttribute(Qt::WA_KeyboardFocusChange);
		}
	}
#endif

	return QWidget::event(e);
}

void Switch::updateShortcut()
{
	m_shortcutId = 0;
	m_hasShortcut = false;

	if (text().isEmpty())
		return;

	if (!text().contains(QLatin1Char('&')))
		return;

#ifndef EXCLUDE_FOR_DESIGNER_PLUGIN
	m_shortcutId = this->grabShortcut(QKeySequence::mnemonic(text()));
#endif
	m_hasShortcut = true;
}

void Switch::animate(bool toggled)
{
	m_anim->stop();
	m_anim->setDuration(120);
	m_anim->setStartValue(position());

	if (toggled)
		m_anim->setEndValue(switchSize().width() - m_radius - m_margin);
	else
		m_anim->setEndValue(m_margin + m_radius);

	m_anim->start();
}
