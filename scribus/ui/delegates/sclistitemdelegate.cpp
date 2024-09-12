#include "sclistitemdelegate.h"

#include <QPainter>
#include <QRect>
#include <QApplication>
#include <QFlags>
#include <QFontMetrics>
#include <QTextLayout>


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

	QFont subFont = QFont();
#ifdef Q_OS_MACOS
	subFont.setPointSize(QFontDatabase::systemFont(QFontDatabase::SmallestReadableFont).pointSize());
#else
	subFont.setPointSize(qRound(QFont().pointSize() * .75));
#endif

	QFontMetrics fm = painter->fontMetrics();
	QFontMetrics sfm(subFont);

	QTextOption tOptions;
	QTextOption sOptions;

	if(m_style == Style::Simple)
	{
		radius = 0;
		margin = 0;
		linePen = Qt::NoPen;
		lineMarkedPen = Qt::NoPen;
	}

	QRect rSaveArea = option.rect.adjusted(padding, padding, -padding, -padding);
	QRect rIcon = rSaveArea;
	QRect rText = rSaveArea;
	QRect rSubText = rSaveArea;

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
	QString subTitle = index.data(Qt::UserRole).toString();

	switch (m_textPosition)
	{
	default:
	case TextPosition::Bottom:
		{
			tOptions.setFlags(QFlag(Qt::AlignBottom|Qt::AlignHCenter|Qt::TextWordWrap));
			sOptions.setFlags(QFlag(Qt::AlignBottom|Qt::AlignHCenter|Qt::TextWordWrap));

			rIcon = QRect(0, 0, m_iconSize.width(), m_iconSize.height());
			rIcon.moveCenter(rSaveArea.center());
			rIcon.moveTop(rSaveArea.top());

			rSubText = sfm.boundingRect(rSaveArea, sOptions.flags(), subTitle);
			rSubText.moveBottom(rSaveArea.bottom());

			rText = rSaveArea.adjusted(0, rIcon.height(), 0, -rSubText.height() -fm.leading() -fm.descent());

		}
		break;
	case TextPosition::Right:
		{
			tOptions.setFlags(QFlag(Qt::AlignCenter|Qt::AlignLeft|Qt::TextWordWrap));
			sOptions.setFlags(QFlag(Qt::AlignBottom|Qt::AlignLeft|Qt::TextWordWrap));

			rIcon = QRect(0, 0, m_iconSize.width(), m_iconSize.height());
			rIcon.moveCenter(rSaveArea.center());
			rIcon.moveLeft(rSaveArea.left());

			rSubText = rSaveArea.adjusted(padding + m_iconSize.width(), 0, 0, 0);
			rSubText = sfm.boundingRect(rSaveArea, sOptions.flags(), subTitle);
			rSubText.moveBottom(rSaveArea.bottom());

			rText = rSaveArea.adjusted(padding + m_iconSize.width(), 0, 0, -rSubText.height() -fm.leading() -fm.descent());

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
		QRect rectNorm = rText.normalized();

		painter->setPen(fontPen);
		painter->setBrush(Qt::NoBrush);
		painter->setFont(QFont());

		int textWidth = rText.width();
		int lineCount = 0;
		QTextLayout textLayout(title, painter->font());
		textLayout.beginLayout();

		while (++lineCount < rText.height() / fm.lineSpacing()) {
			QTextLine line = textLayout.createLine();
			if (!line.isValid())
				break;

			line.setLineWidth(rText.width());
			textWidth += line.naturalTextWidth();
		}
		textLayout.endLayout();
		title = fm.elidedText(title, Qt::ElideRight, textWidth);

		painter->drawText(rText, tOptions.flags(), title, &rectNorm);

		painter->setFont(subFont);
		painter->drawText(rSubText, sOptions.flags(), subTitle);

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
