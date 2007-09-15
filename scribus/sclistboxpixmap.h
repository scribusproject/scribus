/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCLISTBOXPIXMAP_H
#define SCLISTBOXPIXMAP_H

#include <memory>

#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QPixmap>
#include <QVariant>

#include "scguardedptr.h"
#include "scribusapi.h"

class QVariant;

class ScListBoxDelegate
{
public:
	virtual QString text(const QVariant&) const = 0;
	virtual ~ScListBoxDelegate() {};
};

template<unsigned int pixWidth, unsigned int pixHeight>
class ScListBoxPixmap : public QAbstractItemDelegate, public ScListBoxDelegate
{
public:
	ScListBoxPixmap(void);
	virtual QSize sizeHint (const QStyleOptionViewItem & option, const QModelIndex & index ) const;
	virtual void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
protected:
	static std::auto_ptr<QPixmap> pmap;
	// The drawPixmap function must not modify pixmap size
	virtual void redraw(const QVariant&) const = 0;
};

template<unsigned int pixWidth, unsigned int pixHeight> 
std::auto_ptr<QPixmap> ScListBoxPixmap<pixWidth, pixHeight>::pmap;


template<unsigned int pixWidth, unsigned int pixHeight>
ScListBoxPixmap<pixWidth, pixHeight>::ScListBoxPixmap(void) : QAbstractItemDelegate()
{
	if (!pmap.get())
	{
		pmap.reset( new QPixmap(pixWidth, pixHeight) );
	}
};


template<unsigned int pixWidth, unsigned int pixHeight>
QSize ScListBoxPixmap<pixWidth, pixHeight>::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	int h,w;
	QFontMetrics metrics(option.font);
	const QVariant data(index.data(Qt::DisplayRole));
	if ( text(data).isEmpty() )
	{
		h = pmap->height();
		w = pmap->width() + 6;
	}
	else
	{
		h = qMax( pmap->height(), metrics.lineSpacing() + 2 );
		w = pmap->width() + metrics.width(text(data)) + 6;
	}
    return QSize(qMax(w, QApplication::globalStrut().width()), qMax( h, QApplication::globalStrut().height()));
};


template<unsigned int pixWidth, unsigned int pixHeight>
void ScListBoxPixmap<pixWidth, pixHeight>::paint(QPainter * qpainter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	int yPos;
	
	const QVariant data(index.data(Qt::UserRole));
	redraw(data);
	
	int itemHeight = sizeHint( option, index ).height();

	if (option.state & QStyle::State_Selected)
		qpainter->fillRect(option.rect, option.palette.highlight());
		
    qpainter->save();
	qpainter->setRenderHint(QPainter::Antialiasing, true);
	qpainter->setPen(Qt::NoPen);

	if ( !pmap->isNull() ) {
		yPos = ( itemHeight - pmap->height() ) / 2;
		qpainter->drawPixmap( option.rect.x() + 3, option.rect.y() + yPos, *pmap);
    }
	if (option.state & QStyle::State_Selected)
		qpainter->setBrush(option.palette.highlightedText());
	else
		qpainter->setBrush(QBrush(Qt::black));
	qpainter->setPen(Qt::black);
	
	QString txt = index.data(Qt::DisplayRole).toString();

    if ( !txt.isEmpty() ) {
		QFontMetrics fm = qpainter->fontMetrics();
		yPos = ( ( itemHeight - fm.height() ) / 2 ) + fm.ascent();
		qpainter->drawText( option.rect.x() + pmap->width() + 5, option.rect.y() + yPos, txt );
    }
    qpainter->restore();
};

#endif
