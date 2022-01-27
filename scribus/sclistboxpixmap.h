/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCLISTBOXPIXMAP_H
#define SCLISTBOXPIXMAP_H

#include <memory>

#include <QAbstractItemDelegate>
#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QPixmap>
#include <QScopedPointer>
#include <QStyleOptionViewItem>
#include <QVariant>

#include "scguardedptr.h"
#include "scribusapi.h"

class QVariant;

template<unsigned int pixWidth, unsigned int pixHeight>
class ScListBoxPixmap : public QAbstractItemDelegate
{
public:
	ScListBoxPixmap(void);

	virtual QString text(const QVariant&) const = 0;

	// Functions reimplemented from QAbstractItemDelegate
	QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const override;
	void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const override;

protected:
	static QScopedPointer<QPixmap> pmap;
	// The drawPixmap function must not modify pixmap size
	virtual void redraw(const QVariant&) const = 0;
};

template<unsigned int pixWidth, unsigned int pixHeight> 
QScopedPointer<QPixmap> ScListBoxPixmap<pixWidth, pixHeight>::pmap;

template<unsigned int pixWidth, unsigned int pixHeight>
ScListBoxPixmap<pixWidth, pixHeight>::ScListBoxPixmap(void) : QAbstractItemDelegate()
{
	if (pmap.isNull())
		pmap.reset(new QPixmap(pixWidth, pixHeight));
}

template<unsigned int pixWidth, unsigned int pixHeight>
QSize ScListBoxPixmap<pixWidth, pixHeight>::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	int h,w;
	QFontMetrics metrics(option.font);
	const QVariant data(index.data(Qt::DisplayRole));
	
	QString textData = text(data);
	if (textData.isEmpty())
	{
		h = pmap->height();
		w = pmap->width() + 6;
	}
	else
	{
		h = qMax(pmap->height(), metrics.lineSpacing() + 2);
		w = pmap->width() + metrics.horizontalAdvance(textData) + 6;
	}

	return QSize(qMax(w, 0), qMax(h, 0));
}

template<unsigned int pixWidth, unsigned int pixHeight>
void ScListBoxPixmap<pixWidth, pixHeight>::paint(QPainter * qpainter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	const QVariant data(index.data(Qt::UserRole));
	redraw(data);
	
	int itemHeight = sizeHint(option, index).height();

	if (option.state & QStyle::State_Selected)
		qpainter->fillRect(option.rect, option.palette.highlight());
		
	qpainter->save();
	qpainter->setRenderHint(QPainter::Antialiasing, true);
	qpainter->setPen(Qt::NoPen);

	if (!pmap->isNull())
	{
		int yPos = (itemHeight - pmap->height()) / 2;
		qpainter->drawPixmap(option.rect.x() + 3, option.rect.y() + yPos, *pmap);
	}

	if (option.state & QStyle::State_Selected)
		qpainter->setPen(QPen(option.palette.highlightedText(), 0));
	else
		qpainter->setPen(QPen(option.palette.text(), 0));
	
	QString txt(index.data(Qt::DisplayRole).toString());
	if (!txt.isEmpty())
	{
		QFontMetrics fm = qpainter->fontMetrics();
		int yPos = (itemHeight - fm.height()) / 2 + fm.ascent();
		qpainter->drawText(option.rect.x() + pmap->width() + 5, option.rect.y() + yPos, txt);
	}
	qpainter->restore();
}

#endif
