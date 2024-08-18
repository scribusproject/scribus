#include <QPainter>
#include <QStyleOption>
#include <QHBoxLayout>
#include <QEvent>
#include <QShortcutEvent>
#include <QFontDatabase>
#include <QApplication>


#include "form_widget.h"

FormWidget::FormWidget(QWidget *parent)
	: QWidget(parent)
{
	QSizePolicy policy(QSizePolicy::Preferred, QSizePolicy::Minimum);
	m_font = this->font();
	m_devicePixelRatio = qApp->devicePixelRatio();

	setSizePolicy(policy);
	calculateFrame();

}

/* ********************************************************************************* *
 *
 * Members
 *
 * ********************************************************************************* */

QSize FormWidget::minimumSizeHint() const
{
	int w = 0, h = 0;
	labelSize(w,h);

	return QSize(w, h).expandedTo(QWidget::minimumSizeHint());
}

/* ********************************************************************************* *
 *
 * Helper
 *
 * ********************************************************************************* */

void FormWidget::calculateFrame()
{

	int w, h;
	labelSize(w,h);

	switch (m_position)
	{
		case Left:
			setContentsMargins(w, 0, 0, 0);
			break;
		case Right:
			setContentsMargins(0, 0, w, 0);
			break;
		case Top:
			setContentsMargins(0, h, 0, 0);
			break;
		case Bottom:
			setContentsMargins(0, 0, 0, h);
			break;
	}

	update();
}

void FormWidget::updateShortcut()
{	
	m_shortcutId = 0;
	m_hasShortcut = false;

	if (hasPixmap() || m_label.isEmpty() || m_labelVisibility == false)
		return;

	if (!m_label.contains(QLatin1Char('&')))
		return;

#ifndef EXCLUDE_FOR_DESIGNER_PLUGIN
	m_shortcutId = this->grabShortcut(QKeySequence::mnemonic(m_label));
#endif
	m_hasShortcut = true;
}

/* ********************************************************************************* *
 *
 * Events
 *
 * ********************************************************************************* */

void FormWidget::paintEvent(QPaintEvent *e)
{
	QWidget::paintEvent(e);

	if (!labelVisibility()) return;

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

	QStyle *style = QWidget::style();
	QStyleOption opt;
	opt.initFrom(this);
	int align = QStyle::visualAlignment(this->layoutDirection(), Qt::AlignCenter);

	QFont tmpFont(m_font);
	if (m_useSmallFont)
		tmpFont.setPointSize(smallFontSize());

	QPen pen(QPalette::Text);
	painter.setPen(pen);
	painter.setFont(tmpFont);

	QRect r = rect();
	QPixmap pix = m_pixmap;

	if (hasPixmap() && pix.size() != m_pixmapSize)
		pix = pix.scaled(m_pixmapSize * m_devicePixelRatio, Qt::KeepAspectRatio, Qt::SmoothTransformation);

	if (!isEnabled())
		pix = style->generatedIconPixmap(QIcon::Disabled, pix, &opt);


	switch (m_position){
	case Left:
		if (hasPixmap())
		{
			r= QRect (0, (height() - m_pixmap.height()) / 2, m_pixmap.width(), m_pixmap.height());
			style->drawItemPixmap(&painter, r, align, pix);

		}
		else
			style->drawItemText(&painter, r, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextShowMnemonic, opt.palette, isEnabled(), m_label, foregroundRole());

		break;
	case Right:
		if (hasPixmap())
		{
			r= QRect (width() - m_pixmap.width(), (height() - m_pixmap.height()) / 2, m_pixmap.width(), m_pixmap.height());
			style->drawItemPixmap(&painter, r, align, pix);
		}
		else
			style->drawItemText(&painter, r, Qt::AlignRight | Qt::AlignVCenter | Qt::TextShowMnemonic, opt.palette, isEnabled(), m_label, foregroundRole());

		break;
	case Top:
		if (hasPixmap())
		{
			r= QRect ((width() - m_pixmap.width()) / 2, 0, m_pixmap.width(), m_pixmap.height());
			style->drawItemPixmap(&painter, r, align, pix);
		}
		else
			style->drawItemText(&painter, r, Qt::AlignHCenter | Qt::AlignTop | Qt::TextShowMnemonic, opt.palette, isEnabled(), m_label, foregroundRole());

		break;
	case Bottom:
		if (hasPixmap())
		{
			r= QRect ((width() - m_pixmap.width()) / 2, height() - m_pixmap.height(), m_pixmap.width(), m_pixmap.height());
			style->drawItemPixmap(&painter, r, align, pix);
		}
		else
			style->drawItemText(&painter, r, Qt::AlignHCenter | Qt::AlignBottom | Qt::TextShowMnemonic, opt.palette, isEnabled(), m_label, foregroundRole());

		break;
	}

}

bool FormWidget::event(QEvent *e)
{
	QEvent::Type type = e->type();

#ifndef EXCLUDE_FOR_DESIGNER_PLUGIN
	if (type == QEvent::Shortcut && this->layout()->count() > 0) {
		QShortcutEvent *se = static_cast<QShortcutEvent *>(e);
		if (se->shortcutId() == m_shortcutId) {

			int focus = -1;
			int newFocus = -1;
			QList<int> widgets;

			// Check if a child is already highlighted
			for (int i = 0; i < this->layout()->count(); i++)
			{
				QLayoutItem * li = this->layout()->itemAt(i);
				QWidget *child = li->widget();

				if (!child)
					continue;

				if (child->hasFocus())
				{
					focus = i;
					continue;
				}

				if (child->focusPolicy() != Qt::NoFocus)
					widgets.append(i);
			}

			// Get new focus ID
			foreach ( int i, widgets )
			{
				if ( focus == -1 || i > focus)
				{
					newFocus = i;
					break;
				}

				if (i == widgets.last())
				{
					newFocus = widgets.first();
					break;
				}
			}

			if ( newFocus < 0 || newFocus > this->layout()->count())
				return QWidget::event(e);

			// Set focus on new item
			QWidget * wdgFocus = this->layout()->itemAt(newFocus)->widget();

			if (wdgFocus)
			{
				wdgFocus->setFocus(Qt::ShortcutFocusReason);
				window()->setAttribute(Qt::WA_KeyboardFocusChange);
			}

		}
	}
#endif

	return QWidget::event(e);
}



/* ********************************************************************************* *
 *
 * Properties
 *
 * ********************************************************************************* */


void FormWidget::labelSize(int &w, int &h) const
{   
	w = 0;
	h = 0;

	if (hasPixmap())
	{
        	w = m_pixmapSize.width() * m_devicePixelRatio + m_space;
        	h = m_pixmapSize.height() * m_devicePixelRatio + m_space;
	}
	else
	{

		if ((m_label.isEmpty() && m_preserveLabelSpace == false) || m_labelVisibility == false)
		{
			w = 0;
			h = 0;
		}
		else
		{
			QFont tmpFont(m_font);
			if (m_useSmallFont)
				tmpFont.setPointSize(smallFontSize());

			QFontMetrics metrics(tmpFont);
			QString label = m_label;

			// calculate width without hidden "&"
			if (m_hasShortcut)
			{
				int pos = 0;
				while ( pos < m_label.size())
				{
					int index = label.indexOf(QLatin1String("&"), pos);

					if (pos == index)
						label = label.replace(index, 1, "");

					pos++;
				}
			}

			w = metrics.horizontalAdvance(label) + metrics.horizontalAdvance(QLatin1Char(' ')) + m_space;
			h = metrics.height() + m_space;

		}
	}
}

int FormWidget::smallFontSize() const
{
	// uncommend to compare the different font size calculations
	// qDebug() << Q_FUNC_INFO << QFont().pointSize() << qRound(QFont().pointSize() * .75) << QFontDatabase::systemFont(QFontDatabase::SmallestReadableFont).pointSize();

#ifdef Q_OS_MACOS
	return QFontDatabase::systemFont(QFontDatabase::SmallestReadableFont).pointSize();
#else
	return qRound(QFont().pointSize() * .75);
#endif

}


void FormWidget::setText(const QString &text)
{
	if (m_label != text)
	{
		m_label = text;
		updateShortcut();
		calculateFrame();
	}
}

void FormWidget::setLabelVisibility(bool visible)
{
	if (m_labelVisibility != visible)
	{
		m_labelVisibility = visible;
		updateShortcut();
		calculateFrame();
	}
}

void FormWidget::setPreserveLabelSpace(bool preserveSpace)
{
	m_preserveLabelSpace = preserveSpace;
	calculateFrame();
}

void FormWidget::setDirection(LabelPosition direction)
{
	m_position = direction;
	calculateFrame();
}

void FormWidget::setFont(QFont font)
{
	m_font = font;
	calculateFrame();
}

void FormWidget::setUseSmallFont(bool smallFont)
{
	m_useSmallFont = smallFont;
	calculateFrame();
}

void FormWidget::setSpace(int space)
{
	m_space = space;
	calculateFrame();
}

void FormWidget::setPixmap(QPixmap icon)
{
	m_pixmap = icon;
	updateShortcut();
	calculateFrame();

}

void FormWidget::setPixmapSize(QSize size)
{
	m_pixmapSize = size;
	calculateFrame();
}

void FormWidget::addWidget(QWidget* widget)
{
	if (layout() != nullptr)
		layout()->addWidget(widget);
	else
	{
		QHBoxLayout* lay = new QHBoxLayout();
		lay->setContentsMargins(0, 0, 0, 0);
		lay->setSpacing(4);
		lay->addWidget(widget);
		setLayout(lay);
	}

	calculateFrame();
}
