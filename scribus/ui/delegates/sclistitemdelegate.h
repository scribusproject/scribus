#ifndef SCLISTITEMDELEGATE_H
#define SCLISTITEMDELEGATE_H

#include <QItemDelegate>
#include <QListWidget>
#include "scribusapi.h"

class SCRIBUS_API ScListItemDelegate : public QItemDelegate
{
public:

	enum TextPosition {
		Bottom = 0,
		Right = 1
	};

	enum Style {
		Simple = 0,
		Card = 1
	};

	ScListItemDelegate(QListView::ViewMode mode, QSize tileSize, TextPosition textPosition = TextPosition::Bottom, Style style = Style::Card, QObject *parent = nullptr);
	virtual ~ScListItemDelegate(){};

	void paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

	QSize sizeHint( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
	QSize iconSize();
	void setIconSize(QSize size);
	TextPosition textPosition();
	void setTextPosition(TextPosition position);
	Style style();
	void setStyle(Style style);
	QListView::ViewMode viewMode();
	void setViewMode(QListView::ViewMode mode);

private:
	QSize m_iconSize = { QSize(64, 64) };
	TextPosition m_textPosition = { TextPosition::Bottom };
	Style m_style = { Style::Card };
	QListView::ViewMode m_mode = { QListWidget::IconMode };
};

#endif // SCLISTITEMDELEGATE_H
