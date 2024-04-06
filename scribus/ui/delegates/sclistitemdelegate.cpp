#include "sclistitemdelegate.h"

#include <QPainter>
#include <QRect>
#include <QApplication>
#include <QFlags>


ScListItemDelegate::ScListItemDelegate(QSize iconSize, TextPosition textPosition, QObject *parent) : QAbstractItemDelegate (parent)
{
	setIconSize(iconSize);
	setTextPosition(textPosition);
}

void ScListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const{

	int padding = 8;
	qreal margin = 4.5;
	int radius = 2;
	QColor cHighlight = QApplication::palette().highlight().color();
	cHighlight.setAlphaF(0.2f);

	QTextOption tOptions;

	QRect rIcon = option.rect;
	QRect rText = option.rect;
	QRectF rItem = option.rect;
	rItem = rItem.adjusted(margin, margin, -margin, -margin);

	QPen linePen(QApplication::palette().mid().color(), 1, Qt::SolidLine);
	QPen lineMarkedPen(QApplication::palette().highlight().color(), 1, Qt::SolidLine);
	QPen fontPen(QApplication::palette().text().color(), 1, Qt::SolidLine);

	QBrush fillNormal(QApplication::palette().alternateBase().color());
	QBrush fillSelected(cHighlight);

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
}

QSize ScListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const{
	return option.rect.size();
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
