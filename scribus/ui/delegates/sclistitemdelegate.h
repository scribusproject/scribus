#ifndef SCLISTITEMDELEGATE_H
#define SCLISTITEMDELEGATE_H

#include <QAbstractItemDelegate>
#include "scribusapi.h"

class SCRIBUS_API ScListItemDelegate : public QAbstractItemDelegate
{
public:

	enum TextPosition {
		Bottom = 0,
		Right = 1
	};

	ScListItemDelegate(QSize tileSize, TextPosition textPosition = TextPosition::Bottom, QObject *parent = nullptr);
	virtual ~ScListItemDelegate(){};

	void paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

	QSize sizeHint( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
	QSize iconSize();
	void setIconSize(QSize size);
	TextPosition textPosition();
	void setTextPosition(TextPosition position);

private:
	QSize m_iconSize = {QSize(64,64)};
	TextPosition m_textPosition = {TextPosition::Bottom};
};

#endif // SCLISTITEMDELEGATE_H
