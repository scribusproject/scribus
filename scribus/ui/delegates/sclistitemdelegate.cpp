#include "sclistitemdelegate.h"

#include <QPainter>
#include <QRect>
#include <QApplication>
#include <QFlags>


ScListItemDelegate::ScListItemDelegate(QListView::ViewMode mode, QSize iconSize, TextPosition textPosition, Style style, QObject *parent) : QItemDelegate (parent)
{
	setIconSize(iconSize);
	setTextPosition(textPosition);
	setStyle(style);
	setViewMode(mode);
}

void ScListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const{

	int padding = 8;
	qreal margin = 2.5;
	int radius = 2;
	QPalette::ColorGroup cg = option.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
	QColor cBaseFill = option.palette.brush(cg, QPalette::AlternateBase).color();
	QColor cLinePen = option.palette.brush(cg, QPalette::Mid).color();
	QColor cText = option.palette.brush(cg, QPalette::Text).color();
	QColor cHighlight = option.palette.brush(cg, QPalette::Highlight).color();

	QPen linePen(cLinePen, 1, Qt::SolidLine);
	QPen lineMarkedPen(cHighlight, 1, Qt::SolidLine);
	QPen fontPen(cText, 1, Qt::SolidLine);

	QBrush fillNormal(cBaseFill);
	cHighlight.setAlphaF(0.2f);
	QBrush fillSelected(cHighlight);

	if(m_style == Style::Simple)
	{
		radius = 0;
		margin = 0;
		linePen = Qt::NoPen;
		lineMarkedPen = Qt::NoPen;
	}

	QTextOption tOptions;

	QRect rIcon = option.rect;
	QRect rText = option.rect;
	QRectF rItem = option.rect;
	rItem = rItem.adjusted(margin, margin, -margin, -margin);

	painter->save();
	painter->setRenderHint(QPainter::Antialiasing, true);

	if (option.state & QStyle::State_Selected)
	{
		painter->setBrush(fillSelected);
		painter->setPen(lineMarkedPen);
		painter->drawRoundedRect(rItem, radius, radius);

	}
	else
	{
		painter->setBrush(fillNormal);
		painter->setPen(linePen);
		painter->drawRoundedRect(rItem, radius, radius);
	}

	// Item Data
	QIcon ic = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
	QString title = index.data(Qt::DisplayRole).toString();

	switch (m_textPosition)
	{
	default:
	case TextPosition::Bottom:
		{
			int hSpace = (option.rect.width() - m_iconSize.width()) / 2;
			int vSpace = padding;
			QPoint p = option.rect.topLeft();
			rIcon = QRect(p.x() + hSpace, p.y() + vSpace, m_iconSize.width(), m_iconSize.height());

			rText = option.rect.adjusted(padding, padding + m_iconSize.height(), -padding, -padding);
			tOptions.setFlags(QFlag(Qt::AlignBottom|Qt::AlignHCenter|Qt::TextWordWrap));
		}
		break;
	case TextPosition::Right:
		{
			QPoint p = option.rect.topLeft();
			rIcon = QRect(p.x() + padding, p.y() + padding, m_iconSize.width(), m_iconSize.height());

			rText = option.rect.adjusted(padding * 2 + m_iconSize.width(), padding, -padding, -padding);
			tOptions.setFlags(QFlag(Qt::AlignVCenter|Qt::AlignLeft|Qt::TextWordWrap));
		}
		break;
	}

	// Icon
	if (!ic.isNull())
	{
		ic.paint(painter, rIcon);
	}

	// Label
	if (!title.isEmpty())
	{
		painter->setPen(fontPen);
		painter->setFont(QFont());
		painter->drawText(rText, tOptions.flags(), title, &rText);
	}

	painter->restore();
}

QSize ScListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const{

	QSize sh = option.rect.size();

	switch(m_mode){
	default:
	case QListView::ViewMode::IconMode:
		{
		}
		break;
	case QListView::ViewMode::ListMode:
		{
			sh.setWidth(0);
			sh.setHeight(m_iconSize.height() + 16);
		}
		break;
	}

	return sh;//QSize(sh.width(), sh.height());
}

QSize ScListItemDelegate::iconSize()
{
	return m_iconSize;
}

void ScListItemDelegate::setIconSize(QSize size)
{
	m_iconSize = size;
}

ScListItemDelegate::TextPosition ScListItemDelegate::textPosition()
{
	return m_textPosition;
}

void ScListItemDelegate::setTextPosition(TextPosition position)
{
	m_textPosition = position;
}

ScListItemDelegate::Style ScListItemDelegate::style()
{
	return m_style;
}

void ScListItemDelegate::setStyle(Style style)
{
	m_style = style;
}

QListView::ViewMode ScListItemDelegate::viewMode()
{
	return m_mode;
}

void ScListItemDelegate::setViewMode(QListView::ViewMode mode)
{
	m_mode = mode;
}
