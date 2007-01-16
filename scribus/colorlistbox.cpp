/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "colorlistbox.h"
#include "colorlistbox.moc"
#include <qpainter.h>
#include <qpixmap.h>
#include <qbitmap.h>
#include <cstdlib>

#include "scconfig.h"
#include "commonstrings.h"
#include "scribusdoc.h"
#include "colorutil.h"
#include "util.h"
#include "sccolorengine.h"

ColorSmallPixmapItem::ColorSmallPixmapItem( const ScColor& col, ScribusDoc* doc, const QString colName ) 
					: ScListBoxPixmap<15,15>()
{
	m_color = col;
	m_doc = (doc) ? doc->guardedPtr() : NULL;
	setText(colName);
}

void ColorSmallPixmapItem::redraw(void)
{
	QPixmap* pPixmap = ScListBoxPixmap<15,15>::pmap.get();
	QColor rgb = ScColorEngine::getDisplayColor(m_color, m_doc);
	pPixmap->fill(rgb);
	QPainter painter(pPixmap);
	painter.setBrush(Qt::NoBrush);
	QPen b(Qt::black, 1);
	painter.setPen(b);
	painter.drawRect(0, 0, 15, 15);
	painter.end();
}

ColorWidePixmapItem::ColorWidePixmapItem( const ScColor& col, ScribusDoc* doc, const QString colName ) 
					: ScListBoxPixmap<30,15>()
{
	m_color = col;
	m_doc = (doc) ? doc->guardedPtr() : NULL;
	setText(colName);
}

void ColorWidePixmapItem::redraw(void)
{
	QColor rgb = ScColorEngine::getDisplayColor(m_color, m_doc);
	ScListBoxPixmap<30,15>::pmap->fill(rgb);
}

ColorFancyPixmapItem::ColorFancyPixmapItem( const ScColor& col, ScribusDoc* doc, const QString colName ) 
					: ScListBoxPixmap<60,15>()
{
	m_color = col;
	m_doc = (doc) ? doc->guardedPtr() : NULL;
	setText(colName);
}

void ColorFancyPixmapItem::redraw(void)
{
	static QPixmap smallPix(15, 15);
	static QPixmap alertIcon;
	static QPixmap cmykIcon;
	static QPixmap rgbIcon;
	static QPixmap spotIcon;
	static QPixmap regIcon;
	static bool iconsInitialized = false;

	if ( !iconsInitialized ) {
		alertIcon = loadIcon("alert.png");
		cmykIcon = loadIcon("cmyk.png");
		rgbIcon = loadIcon("rgb.png");
		spotIcon = loadIcon("spot.png");
		regIcon = loadIcon("register.png");
		iconsInitialized = true;
	}

	QColor rgb = ScColorEngine::getDisplayColor(m_color, m_doc);
	smallPix.fill(rgb);
	QPainter painter(&smallPix);
	painter.setBrush(Qt::NoBrush);
	QPen b(Qt::black, 1);
	painter.setPen(b);
	painter.drawRect(0, 0, 15, 15);
	painter.end();

	QPixmap* pPixmap = ScListBoxPixmap<60,15>::pmap.get();
	pPixmap->fill(Qt::color0);
	paintAlert(smallPix, *pPixmap, 0, 0);
	bool isOutOfGamut = ScColorEngine::isOutOfGamut(m_color, m_doc);
	if (isOutOfGamut)
		paintAlert(alertIcon, *pPixmap, 15, 0);
	if ((m_color.getColorModel() == colorModelCMYK) || (m_color.isSpotColor()))
		paintAlert(cmykIcon, *pPixmap, 30, 0);
	else
		paintAlert(rgbIcon, *pPixmap, 30, 0);
	if (m_color.isSpotColor())
		paintAlert(spotIcon, *pPixmap, 45, 0);
	if (m_color.isRegistrationColor())
		paintAlert(regIcon, *pPixmap, 46, 0);
	if (pPixmap->mask() && ((!m_color.isSpotColor() && !m_color.isRegistrationColor()) || !isOutOfGamut))
	{
		QPainter alpha; // transparency handling
		alpha.begin(pPixmap->mask());
		alpha.setBrush(Qt::color0);
		alpha.setPen(Qt::color0);
		if (!m_color.isSpotColor() && !m_color.isRegistrationColor())
			alpha.drawRect(45, 0, 15, 15);
		if (!isOutOfGamut)
			alpha.drawRect(15, 0, 15, 15);
		alpha.end();
	}
}

ColorListBox::ColorListBox(QWidget * parent, const char * name, WFlags f)
	: QListBox(parent, name, f)
{
	if (name == "")
		setName("ColorListBox");
}

void ColorListBox::updateBox(ColorList& list, ColorListBox::PixmapType type)
{
	clear();
	insertItems(list, type);
}

void ColorListBox::insertItems(ColorList& list, ColorListBox::PixmapType type)
{
	if (type == ColorListBox::fancyPixmap)
		insertFancyPixmapItems( list );
	else if (type == ColorListBox::widePixmap)
		insertWidePixmapItems( list );
	else if (type == ColorListBox::smallPixmap)
		insertSmallPixmapItems( list );
}

void ColorListBox::insertSmallPixmapItems(ColorList& list)
{
	ColorList::Iterator it;
	ScribusDoc* doc = list.document();
	for (it = list.begin(); it != list.end(); ++it)
	{
		if (it.key() == CommonStrings::None || it.key() == CommonStrings::NoneColor)
			continue;
		insertItem( new ColorSmallPixmapItem(it.data(), doc, it.key()) );
	}
}

void ColorListBox::insertWidePixmapItems(ColorList& list)
{
	ColorList::Iterator it;
	ScribusDoc* doc = list.document();
	for (it = list.begin(); it != list.end(); ++it)
	{
		if (it.key() == CommonStrings::None || it.key() == CommonStrings::NoneColor)
			continue;
		insertItem( new ColorWidePixmapItem(it.data(), doc, it.key()) );
	}
}

void ColorListBox::insertFancyPixmapItems(ColorList& list)
{
	ColorList::Iterator it;
	ScribusDoc* doc = list.document();
	for (it = list.begin(); it != list.end(); ++it)
	{
		if (it.key() == CommonStrings::None || it.key() == CommonStrings::NoneColor)
			continue;
		insertItem( new ColorFancyPixmapItem(it.data(), doc, it.key()) );
	}
}
