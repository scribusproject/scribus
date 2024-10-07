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
	m_font = this->font();

	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
	calculateFrame();

}

/* ********************************************************************************* *
 *
 * Members
 *
 * ********************************************************************************* */

QSize FormWidget::minimumSizeHint() const
{
	int l = 0;
	int t = 0;
	int r = 0;
	int b = 0;
	int w = 0;
	int h = 0;
	labelSize(l, t, r, b, w, h);

	return QSize(w, h).expandedTo(QWidget::minimumSizeHint());
}

/* ********************************************************************************* *
 *
 * Helper
 *
 * ********************************************************************************* */

void FormWidget::calculateFrame()
{
	int l = 0;
	int t = 0;
	int r = 0;
	int b = 0;
	int w = 0;
	int h = 0;
	labelSize(l, t, r, b, w, h);
	setContentsMargins(l, t, r, b);

	update();
}

void FormWidget::updateShortcut()
{	
	m_shortcutId = 0;
	m_hasShortcut = false;

	if (m_label.isEmpty() || m_labelVisibility == false)
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

	if (!labelVisibility() && !iconVisibility()) return;

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

	QRect rLabel = rect();
	QRect rIcon = rect();
	QPixmap pix = m_pixmap;
	pix.setDevicePixelRatio(devicePixelRatio());
	QRect rWidgets = contentsRect();

	// painter.setBrush(Qt::NoBrush);
	// painter.setPen(Qt::magenta);
	// painter.drawRect(rect());
	// painter.setPen(Qt::blue);
	// painter.drawRect(rWidgets);

	if (canUsePixmap() && pix.size() != m_pixmapSize)
		pix = pix.scaled(m_pixmapSize * devicePixelRatio(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

	if (!isEnabled())
		pix = style->generatedIconPixmap(QIcon::Disabled, pix, &opt);

	if (canUsePixmap())
	{
		rIcon = QRect(0, 0, m_pixmap.deviceIndependentSize().width(), m_pixmap.deviceIndependentSize().height());
		rIcon.moveCenter(rWidgets.center());

		switch (m_iconPosition){
		default:
		case Left:
			rIcon.moveLeft(0);
			rLabel = rLabel.adjusted(rIcon.width() + m_space, 0, 0, 0);
			break;
		case Right:
			rIcon.moveRight(width());
			rLabel = rLabel.adjusted(0, 0, -rIcon.width() - m_space, 0);
			break;
		case Top:
			rIcon.moveTop(0);
			rLabel = rLabel.adjusted(0, rIcon.height() + m_space, 0, 0);
			break;
		case Bottom:
			rIcon.moveBottom(height());
			rLabel = rLabel.adjusted(0, 0, 0, -rIcon.height() - m_space);
			break;
		}

		style->drawItemPixmap(&painter, rIcon, align, pix);

		// painter.setBrush(Qt::NoBrush);
		// painter.setPen(Qt::red);
		// painter.drawRect(rIcon);
	}

	if (canUseLabel())
	{
		switch (m_position){
		case Left:
			style->drawItemText(&painter, rLabel, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextShowMnemonic, opt.palette, isEnabled(), m_label, foregroundRole());
			break;
		case Right:
			style->drawItemText(&painter, rLabel, Qt::AlignRight | Qt::AlignVCenter | Qt::TextShowMnemonic, opt.palette, isEnabled(), m_label, foregroundRole());
			break;
		case Top:
			style->drawItemText(&painter, rLabel, Qt::AlignHCenter | Qt::AlignTop | Qt::TextShowMnemonic, opt.palette, isEnabled(), m_label, foregroundRole());
			break;
		case Bottom:
			style->drawItemText(&painter, rLabel, Qt::AlignHCenter | Qt::AlignBottom | Qt::TextShowMnemonic, opt.palette, isEnabled(), m_label, foregroundRole());
			break;
		}

		// painter.setBrush(Qt::NoBrush);
		// painter.setPen(Qt::green);
		// painter.drawRect(rLabel);
	}

}

bool FormWidget::event(QEvent *e)
{	

#ifndef EXCLUDE_FOR_DESIGNER_PLUGIN
	QEvent::Type type = e->type();
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


void FormWidget::labelSize(int &l, int &t, int &r, int &b, int &w, int &h) const
{   
	l = 0;
	t = 0;
	r = 0;
	b = 0;
	w = 0;
	h = 0;

	if (canUsePixmap())
	{
		int iconWidth = m_pixmapSize.width() + m_space;
		int iconHeight = m_pixmapSize.height() + m_space;

		w += iconWidth;
		h += iconHeight;

		switch (m_iconPosition)
		{
		case Left:
			l += iconWidth;
			break;
		case Right:
			r += iconWidth;
			break;
		case Top:
			t += iconHeight;
			break;
		case Bottom:
			b += iconHeight;
			break;
		}

	}

	if (canUseLabel())
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

		int labelWidth = metrics.horizontalAdvance(label) + metrics.horizontalAdvance(QLatin1Char(' ')) + m_space;
		int labelHeight = metrics.height() + m_space;

		w += labelWidth;
		h += labelHeight;

		switch (m_position)
		{
			case Left:
				l += labelWidth;
				break;
			case Right:
				r += labelWidth;
				break;
			case Top:				
				t += labelHeight;
				break;
			case Bottom:				
				b += labelHeight;
				break;
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

void FormWidget::setIconDirection(LabelPosition direction)
{
	m_iconPosition = direction;
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
	calculateFrame();

}

void FormWidget::setPixmapSize(QSize size)
{
	m_pixmapSize = size;
	calculateFrame();
}

void FormWidget::setIconVisibility(bool visible)
{
	m_iconVisibility = visible;
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
