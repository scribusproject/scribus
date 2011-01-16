/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "colorlistbox.h"
#include <QPainter>
#include <QPixmap>
#include <QBitmap>
#include <cstdlib>
#include <QToolTip>
#include <QEvent>
#include <QHelpEvent>
#include <QMenu>
#include <QCursor>

#include "commonstrings.h"
#include "sccolorengine.h"
#include "scconfig.h"
#include "scribusdoc.h"
#include "util_color.h"
#include "util_icon.h"



ColorPixmapValue::ColorPixmapValue() : m_color(), m_doc(NULL), m_name("invalid")
{}

ColorPixmapValue::ColorPixmapValue(const ColorPixmapValue& other) : m_color(other.m_color), m_doc(other.m_doc), m_name(other.m_name)
{}

ColorPixmapValue& ColorPixmapValue::operator= (const ColorPixmapValue& other)
{
	m_color = other.m_color;
	m_doc = other.m_doc;
	m_name = other.m_name;
	return *this;
}

ColorPixmapValue::ColorPixmapValue( const ScColor& col, ScribusDoc* doc, const QString colName ) 
{
	m_doc = (doc) ? doc->guardedPtr() : NULL;
	m_color = col;
	m_name = colName;
}



class SCRIBUS_API ColorSmallItemDelegate : public ScListBoxPixmap<15,15>
{
public:
	ColorSmallItemDelegate(): ScListBoxPixmap<15,15>() {};
	~ColorSmallItemDelegate() {};
	
	virtual void redraw(const QVariant&) const;
	virtual QString text(const QVariant&) const;
	virtual int rtti() const { return 654873547; };
};

class SCRIBUS_API ColorWideItemDelegate : public ScListBoxPixmap<30,15>
{
public:
	ColorWideItemDelegate(): ScListBoxPixmap<30,15>() {};
	~ColorWideItemDelegate() {};
	
	virtual void redraw(const QVariant&) const;
	virtual QString text(const QVariant&) const;
	virtual int rtti() const { return 654873548; };
};

class SCRIBUS_API ColorFancyItemDelegate : public ScListBoxPixmap<60,15>
{
public:
	ColorFancyItemDelegate(): ScListBoxPixmap<60,15>() {};
	~ColorFancyItemDelegate() {};
	
	virtual void redraw(const QVariant&) const;
	virtual QString text(const QVariant&) const;
	virtual int rtti() const { return 654873549; };
};


void ColorSmallItemDelegate::redraw(const QVariant& data) const
{
	QPixmap* pPixmap = ScListBoxPixmap<15,15>::pmap.get();
	pPixmap->fill(Qt::transparent);
	if (data.canConvert<ColorPixmapValue>())
	{
		ColorPixmapValue item(data.value<ColorPixmapValue>());
		QColor rgb = ScColorEngine::getDisplayColor(item.m_color, item.m_doc);
		pPixmap->fill(rgb);
		QPainter painter(pPixmap);
		painter.setBrush(Qt::NoBrush);
		QPen b(Qt::black, 1);
		painter.setPen(b);
		painter.drawRect(0, 0, 15, 15);
		painter.end();
	}
}

void ColorWideItemDelegate::redraw(const QVariant& data) const
{
	QPixmap* pPixmap = ScListBoxPixmap<30,15>::pmap.get();
	pPixmap->fill(Qt::transparent);
	if (data.canConvert<ColorPixmapValue>())
	{
		ColorPixmapValue item(data.value<ColorPixmapValue>());
		QColor rgb = ScColorEngine::getDisplayColor(item.m_color, item.m_doc);
		pPixmap->fill(rgb);
	}
}


void ColorFancyItemDelegate::redraw(const QVariant& data) const
{
	static QPixmap smallPix(15, 15);
	static QPixmap alertIcon;
	static QPixmap cmykIcon;
	static QPixmap rgbIcon;
	static QPixmap spotIcon;
	static QPixmap regIcon;
	static bool iconsInitialized = false;

	if ( !iconsInitialized ) {
		alertIcon = loadIcon("alert.png", true);
		cmykIcon = loadIcon("cmyk.png", true);
		rgbIcon = loadIcon("rgb.png", true);
		spotIcon = loadIcon("spot.png", true);
		regIcon = loadIcon("register.png", true);
		iconsInitialized = true;
	}

	QPixmap* pPixmap = ScListBoxPixmap<60,15>::pmap.get();
	pPixmap->fill(Qt::transparent);

	if (data.canConvert<ColorPixmapValue>())
	{
		ColorPixmapValue item(data.value<ColorPixmapValue>());
		
		QColor rgb = ScColorEngine::getDisplayColor(item.m_color, item.m_doc);
		smallPix.fill(rgb);
		QPainter painter(&smallPix);
		painter.setBrush(Qt::NoBrush);
		QPen b(Qt::black, 1);
		painter.setPen(b);
		painter.drawRect(0, 0, 15, 15);
		painter.end();
		
		paintAlert(smallPix, *pPixmap, 0, 0);
		bool isOutOfGamut = ScColorEngine::isOutOfGamut(item.m_color, item.m_doc);
		if (isOutOfGamut)
			paintAlert(alertIcon, *pPixmap, 15, 0);
		if (item.m_color.getColorModel() == colorModelCMYK)
			paintAlert(cmykIcon, *pPixmap, 30, 0);
		else
			paintAlert(rgbIcon, *pPixmap, 30, 0);
		if (item.m_color.isSpotColor())
			paintAlert(spotIcon, *pPixmap, 45, 0);
		if (item.m_color.isRegistrationColor())
			paintAlert(regIcon, *pPixmap, 46, 0);
	}
}


QString ColorSmallItemDelegate::text(const QVariant& data) const
{
	if (data.canConvert<ColorPixmapValue>())
		return data.value<ColorPixmapValue>().m_name;
	else
		return data.toString();
}

QString ColorWideItemDelegate::text(const QVariant& data) const
{
	if (data.canConvert<ColorPixmapValue>())
		return data.value<ColorPixmapValue>().m_name;
	else
		return data.toString();
}

QString ColorFancyItemDelegate::text(const QVariant& data) const
{
	if (data.canConvert<ColorPixmapValue>())
		return data.value<ColorPixmapValue>().m_name;
	else
		return data.toString();
}


int ColorListBox::initialized;
int ColorListBox::sortRule;

ColorListBox::ColorListBox(QWidget * parent)
	: QListWidget(parent)
{
	cList = NULL;
	if (initialized != 12345)
		sortRule = 0;
	initialized = 12345;
	setItemDelegate(new ColorWideItemDelegate());
	connect(this, SIGNAL(showContextMenue()), this, SLOT(slotRightClick()));
}


QString ColorListBox::currentColor() const
{
	if (currentRow() >= 0)
		return item(currentRow())->data(Qt::DisplayRole).toString();
	else
		return CommonStrings::tr_NoneColor;
}

void ColorListBox::slotRightClick()
{
	blockSignals(true);
	QString currentSel = QString();
	QListWidgetItem* itc = currentItem();
	if (itc)
		currentSel = itc->text();
	QListWidgetItem* it = item(0);
	if (it)
	{
		QString first = QString();
		QMenu *pmen = new QMenu();
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		pmen->addAction( tr("Sort by Name"));
		pmen->addAction( tr("Sort by Color"));
		pmen->addAction( tr("Sort by Type"));
		sortRule = pmen->actions().indexOf(pmen->exec(QCursor::pos()));
		delete pmen;
		if (it->text() == CommonStrings::None || it->text() == CommonStrings::tr_NoneColor)
			first = it->text();
		clear();
		reset();
		if (!first.isEmpty())
			addItem(first);
		if (m_type == ColorListBox::fancyPixmap)
			insertFancyPixmapItems( *cList );
		else if (m_type == ColorListBox::widePixmap)
			insertWidePixmapItems( *cList );
		else if (m_type == ColorListBox::smallPixmap)
			insertSmallPixmapItems( *cList );
		if (!currentSel.isEmpty())
		{
			QList<QListWidgetItem *> items = findItems(currentSel, Qt::MatchExactly);
			if (items.count() > 0)
				setCurrentItem(items[0]);
		}
	}
	blockSignals(false);
}

void ColorListBox::updateBox(ColorList& list, ColorListBox::PixmapType type)
{
	clear();
	reset();
	m_type = type;
	insertItems(list, type);
}

void ColorListBox::insertItems(ColorList& list, ColorListBox::PixmapType type)
{
	cList = &list;
	m_type = type;
	if (type == ColorListBox::fancyPixmap)
		insertFancyPixmapItems( list );
	else if (type == ColorListBox::widePixmap)
		insertWidePixmapItems( list );
	else if (type == ColorListBox::smallPixmap)
		insertSmallPixmapItems( list );
}

void ColorListBox::addItem(ColorPixmapItem* item, ColorListBox::PixmapType type)
{
	QListWidget::addItem(item);
	if (itemDelegate())
		delete itemDelegate();
	if (type == ColorListBox::fancyPixmap)
		setItemDelegate(new ColorFancyItemDelegate());
	else if (type == ColorListBox::widePixmap)
		setItemDelegate(new ColorWideItemDelegate());
	else if (type == ColorListBox::smallPixmap)
		setItemDelegate(new ColorSmallItemDelegate());
}

void ColorListBox::addItem(ColorPixmapItem* item)
{
	QListWidget::addItem(item);
}

void ColorListBox::addItem(QString text)
{
	QListWidget::addItem(text);
}

void ColorListBox::insertSmallPixmapItems(ColorList& list)
{
	ColorList::Iterator it;
	ScribusDoc* doc = list.document();
	if (sortRule > 0)
	{
		QMap<QString, QString> sortMap;
		for (it = list.begin(); it != list.end(); ++it)
		{
			if (it.key() == CommonStrings::None || it.key() == CommonStrings::tr_NoneColor)
				continue;
			if (sortRule == 1)
			{
				QColor c = it.value().getRawRGBColor();
				QString sortString = QString("%1-%2-%3-%4").arg(c.hue(), 3, 10, QChar('0')).arg(c.saturation(), 3, 10, QChar('0')).arg(c.value(), 3, 10, QChar('0')).arg(it.key());
				sortMap.insert(sortString, it.key());
			}
			else if (sortRule == 2)
			{
				QString sortString = QString("%1-%2");
				if (it.value().isRegistrationColor())
					sortMap.insert(sortString.arg("A").arg(it.key()), it.key());
				else if (it.value().isSpotColor())
					sortMap.insert(sortString.arg("B").arg(it.key()), it.key());
				else if (it.value().getColorModel() == colorModelCMYK)
					sortMap.insert(sortString.arg("C").arg(it.key()), it.key());
				else
					sortMap.insert(sortString.arg("D").arg(it.key()), it.key());
			}
		}
		QMap<QString, QString>::Iterator itc;
		for (itc = sortMap.begin(); itc != sortMap.end(); ++itc)
		{
			addItem( new ColorPixmapItem(list[itc.value()], doc, itc.value()) );
		}
	}
	else
	{
		for (it = list.begin(); it != list.end(); ++it)
		{
			if (it.key() == CommonStrings::None || it.key() == CommonStrings::tr_NoneColor)
				continue;
			addItem( new ColorPixmapItem(it.value(), doc, it.key()) );
		}
	}
	if (itemDelegate())
		delete itemDelegate();
	setItemDelegate(new ColorSmallItemDelegate());
}

void ColorListBox::insertWidePixmapItems(ColorList& list)
{
	ColorList::Iterator it;
	ScribusDoc* doc = list.document();
	if (sortRule > 0)
	{
		QMap<QString, QString> sortMap;
		for (it = list.begin(); it != list.end(); ++it)
		{
			if (it.key() == CommonStrings::None || it.key() == CommonStrings::tr_NoneColor)
				continue;
			if (sortRule == 1)
			{
				QColor c = it.value().getRawRGBColor();
				QString sortString = QString("%1-%2-%3-%4").arg(c.hue(), 3, 10, QChar('0')).arg(c.saturation(), 3, 10, QChar('0')).arg(c.value(), 3, 10, QChar('0')).arg(it.key());
				sortMap.insert(sortString, it.key());
			}
			else if (sortRule == 2)
			{
				QString sortString = QString("%1-%2");
				if (it.value().isRegistrationColor())
					sortMap.insert(sortString.arg("A").arg(it.key()), it.key());
				else if (it.value().isSpotColor())
					sortMap.insert(sortString.arg("B").arg(it.key()), it.key());
				else if (it.value().getColorModel() == colorModelCMYK)
					sortMap.insert(sortString.arg("C").arg(it.key()), it.key());
				else
					sortMap.insert(sortString.arg("D").arg(it.key()), it.key());
			}
		}
		QMap<QString, QString>::Iterator itc;
		for (itc = sortMap.begin(); itc != sortMap.end(); ++itc)
		{
			addItem( new ColorPixmapItem(list[itc.value()], doc, itc.value()) );
		}
	}
	else
	{
		for (it = list.begin(); it != list.end(); ++it)
		{
			if (it.key() == CommonStrings::None || it.key() == CommonStrings::tr_NoneColor)
				continue;
			addItem( new ColorPixmapItem(it.value(), doc, it.key()) );
		}
	}
	if (itemDelegate())
		delete itemDelegate();
	setItemDelegate(new ColorWideItemDelegate());
}

void ColorListBox::insertFancyPixmapItems(ColorList& list)
{
	ColorList::Iterator it;
	ScribusDoc* doc = list.document();
	if (sortRule > 0)
	{
		QMap<QString, QString> sortMap;
		for (it = list.begin(); it != list.end(); ++it)
		{
			if (it.key() == CommonStrings::None || it.key() == CommonStrings::tr_NoneColor)
				continue;
			if (sortRule == 1)
			{
				QColor c = it.value().getRawRGBColor();
				QString sortString = QString("%1-%2-%3-%4").arg(c.hue(), 3, 10, QChar('0')).arg(c.saturation(), 3, 10, QChar('0')).arg(c.value(), 3, 10, QChar('0')).arg(it.key());
				sortMap.insert(sortString, it.key());
			}
			else if (sortRule == 2)
			{
				QString sortString = QString("%1-%2");
				if (it.value().isRegistrationColor())
					sortMap.insert(sortString.arg("A").arg(it.key()), it.key());
				else if (it.value().isSpotColor())
					sortMap.insert(sortString.arg("B").arg(it.key()), it.key());
				else if (it.value().getColorModel() == colorModelCMYK)
					sortMap.insert(sortString.arg("C").arg(it.key()), it.key());
				else
					sortMap.insert(sortString.arg("D").arg(it.key()), it.key());
			}
		}
		QMap<QString, QString>::Iterator itc;
		for (itc = sortMap.begin(); itc != sortMap.end(); ++itc)
		{
			addItem( new ColorPixmapItem(list[itc.value()], doc, itc.value()) );
		}
	}
	else
	{
		for (it = list.begin(); it != list.end(); ++it)
		{
			if (it.key() == CommonStrings::None || it.key() == CommonStrings::tr_NoneColor)
				continue;
			addItem( new ColorPixmapItem(it.value(), doc, it.key()) );
		}
	}
	if (itemDelegate())
		delete itemDelegate();
	setItemDelegate(new ColorFancyItemDelegate());
}

bool ColorListBox::viewportEvent(QEvent *event)
{
	/* commented out because of random crashes in the colorcombobox of the gradient editor
	if (event->type() == QEvent::ToolTip)
	{
		if (cList != NULL)
		{
			QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
			QListWidgetItem* it = itemAt(helpEvent->pos());
			if (it != NULL)
			{
				event->accept();
				QString tipText = "";
				if (cList->contains(it->text()))
				{
					ScColor col = (*cList)[it->text()];
					if (col.getColorModel() == colorModelCMYK)
					{
						int c, m, y, k;
						col.getCMYK(&c, &m, &y, &k);
						tipText = QString("C:%1% M:%2% Y:%3% K:%4%").arg(qRound(c / 2.55)).arg(qRound(m / 2.55)).arg(qRound(y / 2.55)).arg(qRound(k / 2.55));
					}
					else
					{
						int r, g, b;
						col.getRawRGBColor(&r, &g, &b);
						tipText = QString("R:%1 G:%2 B:%3").arg(r).arg(g).arg(b);
					}
				}
				QToolTip::showText(helpEvent->globalPos(), tipText, this);
				return true;
			}
		}
	}
	else */
	if (event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
		if (mouseEvent->button() == Qt::RightButton)
			return true;
	}
	else if (event->type() == QEvent::MouseButtonRelease)
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
		if (mouseEvent->button() == Qt::RightButton)
		{
			emit showContextMenue();
			return true;
		}
	}
	return QListWidget::viewportEvent(event);
}
