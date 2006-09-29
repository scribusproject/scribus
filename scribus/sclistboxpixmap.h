/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCLISTBOXPIXMAP_H
#define SCLISTBOXPIXMAP_H

#include "scribusapi.h"
#include <memory>
#include <qapplication.h>
#include <qlistbox.h>
#include <qpainter.h>
#include <qpixmap.h>
using namespace std;

template<unsigned int pixWidth, unsigned int pixHeight>
class SCRIBUS_API ScListBoxPixmap : public QListBoxItem
{
public:
	ScListBoxPixmap(void);
	virtual int	width(const QListBox *)  const;
	virtual int	height(const QListBox *) const;
	virtual void paint(QPainter *);
protected:
	static  auto_ptr<QPixmap> pmap;
	// The drawPixmap function must not modify pixmap size
	virtual void redraw(void) = 0;
};

template<unsigned int pixWidth, unsigned int pixHeight> 
auto_ptr<QPixmap> ScListBoxPixmap<pixWidth, pixHeight>::pmap;

template<unsigned int pixWidth, unsigned int pixHeight>
ScListBoxPixmap<pixWidth, pixHeight>::ScListBoxPixmap(void) : QListBoxItem()
{
	if (!pmap.get())
	{
		pmap.reset( new QPixmap(pixWidth, pixHeight) );
	}
};

template<unsigned int pixWidth, unsigned int pixHeight>
int	ScListBoxPixmap<pixWidth, pixHeight>::width(const QListBox* lb)  const
{
	if ( text().isEmpty() )
		return QMAX( pmap->width() + 6, QApplication::globalStrut().width() );
    return QMAX( pmap->width() + lb->fontMetrics().width(text()) + 6,QApplication::globalStrut().width() );
};

template<unsigned int pixWidth, unsigned int pixHeight>
int	ScListBoxPixmap<pixWidth, pixHeight>::height(const QListBox* lb) const
{
	int h;
	if ( text().isEmpty() )
		h = pmap->height();
    else
		h = QMAX( pmap->height(), lb->fontMetrics().lineSpacing() + 2 );
    return QMAX( h, QApplication::globalStrut().height() );
};

template<unsigned int pixWidth, unsigned int pixHeight>
void ScListBoxPixmap<pixWidth, pixHeight>::paint(QPainter * qpainter)
{
	int yPos;
	redraw();
	int itemHeight = height( listBox() );
	if ( !pmap->isNull() ) {
		yPos = ( itemHeight - pmap->height() ) / 2;
		qpainter->drawPixmap( 3, yPos, *pmap);
    }
    if ( !text().isEmpty() ) {
		QFontMetrics fm = qpainter->fontMetrics();
		yPos = ( ( itemHeight - fm.height() ) / 2 ) + fm.ascent();
		qpainter->drawText( pmap->width() + 5, yPos, text() );
    }
};

#endif
