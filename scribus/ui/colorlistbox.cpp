/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "colorlistbox.h"

#include <cstdlib>
#include <QBitmap>
#include <QCursor>
#include <QEvent>
#include <QHelpEvent>
#include <QMenu>
#include <QPainter>
#include <QPersistentModelIndex>
#include <QPixmap>
#include <QSignalBlocker>
#include <QToolTip>

#include "colorlistmodel.h"
#include "commonstrings.h"
#include "iconmanager.h"
#include "sccolorengine.h"
#include "scconfig.h"
#include "scribusapp.h"
#include "scribusdoc.h"
#include "util_color.h"

class SCRIBUS_API ColorSmallItemDelegate : public ScListBoxPixmap<15, 15>
{
public:
	ColorSmallItemDelegate(): ScListBoxPixmap<15, 15>() {};
	~ColorSmallItemDelegate() = default;
	
	void redraw(const QVariant&) const override;
	QString text(const QVariant&) const override;
};

class SCRIBUS_API ColorWideItemDelegate : public ScListBoxPixmap<30, 15>
{
public:
	ColorWideItemDelegate(): ScListBoxPixmap<30, 15>() {};
	~ColorWideItemDelegate() = default;
	
	void redraw(const QVariant&) const override;
	QString text(const QVariant&) const override;
};

class SCRIBUS_API ColorFancyItemDelegate : public ScListBoxPixmap<60, 15>
{
public:
	ColorFancyItemDelegate();
	~ColorFancyItemDelegate() = default;
	
	void iconSetChange();
	void redraw(const QVariant&) const override;
	QString text(const QVariant&) const override;

private:
	QPixmap alertIcon;
	QPixmap cmykIcon;
	QPixmap rgbIcon;
	QPixmap labIcon;
	QPixmap spotIcon;
	QPixmap regIcon;
};

ColorFancyItemDelegate::ColorFancyItemDelegate() : ScListBoxPixmap<60, 15>()
{
	iconSetChange();
}

void ColorSmallItemDelegate::redraw(const QVariant& data) const
{
	QPixmap* pPixmap = ScListBoxPixmap<15, 15>::pmap.data();
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
	QPixmap* pPixmap = ScListBoxPixmap<30, 15>::pmap.data();
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

	QPixmap* pPixmap = ScListBoxPixmap<60, 15>::pmap.data();
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
		else if (item.m_color.getColorModel() == colorModelRGB)
			paintAlert(rgbIcon, *pPixmap, 30, 0);
		else if (item.m_color.getColorModel() == colorModelLab)
			paintAlert(labIcon, *pPixmap, 30, 0);
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
	return data.toString();
}

QString ColorWideItemDelegate::text(const QVariant& data) const
{
	if (data.canConvert<ColorPixmapValue>())
		return data.value<ColorPixmapValue>().m_name;
	return data.toString();
}

QString ColorFancyItemDelegate::text(const QVariant& data) const
{
	if (data.canConvert<ColorPixmapValue>())
		return data.value<ColorPixmapValue>().m_name;
	return data.toString();
}

void ColorFancyItemDelegate::iconSetChange()
{
	IconManager& iconManager = IconManager::instance();

	alertIcon = iconManager.loadPixmap("alert.png", true);
	cmykIcon = iconManager.loadPixmap("cmyk.png", true);
	rgbIcon = iconManager.loadPixmap("rgb.png", true);
	labIcon = iconManager.loadPixmap("lab.png", true);
	spotIcon = iconManager.loadPixmap("spot.png", true);
	regIcon = iconManager.loadPixmap("register.png", true);
}

int ColorListBox::initialized;
int ColorListBox::sortRule;

ColorListBox::ColorListBox(QWidget * parent)
	: QListView(parent)
{
	if (initialized != 12345)
		sortRule = 0;
	initialized = 12345;
	QListView::setModel(new ColorListModel(this));
	setPixmapType(ColorListBox::widePixmap);

	connect(this, SIGNAL(clicked(QModelIndex)),       this, SLOT(emitItemClicked(QModelIndex)));
	connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(emitItemDoubleClicked(QModelIndex)));
	connect(this->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)),
            this, SLOT(emitCurrentChanged(QModelIndex, QModelIndex)));
	connect(this->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SIGNAL(itemSelectionChanged()));
	connect(this, SIGNAL(contextMenuRequested()), this, SLOT(slotRightClick()));
}

ColorListBox::ColorListBox(ColorListBox::PixmapType type, QWidget * parent)
	: QListView(parent)
{
	cList = nullptr;
	if (initialized != 12345)
		sortRule = 0;
	initialized = 12345;
	QListView::setModel(new ColorListModel(this));
	setPixmapType(type);

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));

	connect(this, SIGNAL(clicked(QModelIndex)),       this, SLOT(emitItemClicked(QModelIndex)));
	connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(emitItemDoubleClicked(QModelIndex)));
	connect(this->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)),
            this, SLOT(emitCurrentChanged(QModelIndex, QModelIndex)));
	connect(this->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SIGNAL(itemSelectionChanged()));
	connect(this, SIGNAL(contextMenuRequested()), this, SLOT(slotRightClick()));
}

ColorListBox::~ColorListBox()
{
	if (itemDelegate())
		delete itemDelegate();
	clear();
}

void ColorListBox::clear()
{
	QAbstractItemModel* itemModel = model();
	itemModel->removeRows(0, itemModel->rowCount());
}

int ColorListBox::count() const
{
	return this->model()->rowCount();
}

void ColorListBox::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QListView::changeEvent(e);
}

void ColorListBox::emitCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
	QPersistentModelIndex persistentCurrent = current;

	QString text = model()->data(current, Qt::DisplayRole).toString();
	emit currentTextChanged(text);
	emit currentRowChanged(persistentCurrent.row());
}

void ColorListBox::emitItemClicked(const QModelIndex &current)
{
	QPersistentModelIndex persistentCurrent = current;
	emit itemClicked(persistentCurrent.row());
}

void ColorListBox::emitItemDoubleClicked(const QModelIndex &current)
{
	QPersistentModelIndex persistentCurrent = current;
	emit itemDoubleClicked(persistentCurrent.row());
}

void ColorListBox::iconSetChange()
{
	if (m_type == ColorListBox::fancyPixmap)
	{
		QAbstractItemDelegate* curDelegate = itemDelegate();
		ColorFancyItemDelegate* colorDelegate = dynamic_cast<ColorFancyItemDelegate*>(curDelegate);
		if (colorDelegate)
		{
			colorDelegate->iconSetChange();
			this->update();
		}
	}
}

void ColorListBox::languageChange()
{
	// Not needed anymore normally: on language change a paintEvent is sent to widget
	// and model will return the new translated string for None color
	/*if (this->count() > 0)
	{
		QModelIndexList result;
		QModelIndex start = model()->index(0, 0, this->rootIndex());
		result =  model()->match(start, Qt::UserRole, CommonStrings::None, 1, Qt::MatchExactly | Qt::MatchCaseSensitive);
		if (result.isEmpty())
			return;
		int index = result.first().row();
		QListWidgetItem* item = this->item(index);
		item->setText(CommonStrings::tr_NoneColor);
	}*/
}

QString ColorListBox::currentColor() const
{
	if (currentRow() >= 0)
	{
		QAbstractItemModel* itemModel = model();
		return itemModel->data(currentIndex(), Qt::DisplayRole).toString();
	}
	return CommonStrings::tr_NoneColor;
}

int ColorListBox::currentRow() const
{
	return currentIndex().row();
}

QVariant ColorListBox::data(int row, int role) const
{
	QModelIndex index = model()->index(row, 0);
	return model()->data(index, role);
}

QStringList ColorListBox::findColors(const QString &name, Qt::MatchFlags flags) const
{
	QStringList foundColors;
	QAbstractItemModel* currentModel = model();

	QModelIndex firstIndex = currentModel->index(0, 0, QModelIndex());
	QModelIndexList indexes = currentModel->match(firstIndex, Qt::DisplayRole, name, -1, flags);
	for (int i = 0; i < indexes.count(); ++i)
	{
		QModelIndex modelIndex = indexes.at(i);
		QVariant modelData = currentModel->data(modelIndex, Qt::DisplayRole);
		foundColors.append(modelData.toString());
	}

	return foundColors;
}

bool ColorListBox::hasSelection() const
{
	return this->selectionModel()->hasSelection();
}

void ColorListBox::insertItem(int row, const ScColor& color, const QString& colorName)
{
	ColorListModel* colorListModel = qobject_cast<ColorListModel*>(model());
	if (!colorListModel)
		return;

	ScribusDoc* doc = nullptr;
	if (cList)
		doc = cList->document();

	ColorPixmapValue value(color, doc, colorName);
	colorListModel->insert(row, value);
}

bool ColorListBox::isNoneColorShown() const
{
	ColorListModel* colorListModel = qobject_cast<ColorListModel*>(model());
	if (colorListModel)
		return colorListModel->isNoneColorShown();
	return false;
}

void ColorListBox::removeItem(int i)
{
	// None color item cannot be removed
	if (isNoneColorShown() && (i == 0))
		return;

	model()->removeRow(i);
}

int ColorListBox::row(const QString& colorName)
{
	QAbstractItemModel* currentModel = model();

	QModelIndex firstIndex = currentModel->index(0, 0, QModelIndex());
	QModelIndexList indexes = currentModel->match(firstIndex, Qt::DisplayRole, colorName, -1, Qt::MatchExactly);
	if (indexes.count() > 0)
	{
		const QModelIndex& first = indexes.at(0);
		return first.row();
	}
	return -1;
}

void ColorListBox::setCurrentColor(QString colorName)
{
	if (colorName == CommonStrings::None)
		colorName = CommonStrings::tr_NoneColor;

	QModelIndex firstIndex = model()->index(0, 0, QModelIndex());
	QModelIndexList indexes = this->model()->match(firstIndex, Qt::DisplayRole, colorName, -1, Qt::MatchExactly);
	if (indexes.count() > 0)
		this->selectionModel()->setCurrentIndex(indexes[0], QItemSelectionModel::ClearAndSelect);
}

void ColorListBox::setColors(ColorList& list, bool insertNone)
{
	ColorList::Iterator it;

	ColorListModel* colorModel = qobject_cast<ColorListModel*>(this->model());
	if (!colorModel)
		return;

	cList = &list;

	colorModel->setColorList(list, insertNone);
}

void ColorListBox::setCurrentRow(int row)
{
	QModelIndex index = this->model()->index(row, 0);
	selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
}

void ColorListBox::setPixmapType(ColorListBox::PixmapType type)
{
	if (type == ColorListBox::fancyPixmap)
	{
		QAbstractItemDelegate* oldDelegate = itemDelegate();
		ColorFancyItemDelegate* colorDelegate = dynamic_cast<ColorFancyItemDelegate*>(oldDelegate);
		if (!colorDelegate)
		{
			setItemDelegate(new ColorFancyItemDelegate());
			delete oldDelegate;
			m_type = type;
		}
	}
	else if (type == ColorListBox::widePixmap)
	{
		QAbstractItemDelegate* oldDelegate = itemDelegate();
		ColorWideItemDelegate* colorDelegate = dynamic_cast<ColorWideItemDelegate*>(oldDelegate);
		if (!colorDelegate)
		{
			setItemDelegate(new ColorWideItemDelegate());
			delete oldDelegate;
			m_type = type;
		}
	}
	else if (type == ColorListBox::smallPixmap)
	{
		QAbstractItemDelegate* oldDelegate = itemDelegate();
		ColorSmallItemDelegate* colorDelegate = dynamic_cast<ColorSmallItemDelegate*>(oldDelegate);
		if (!colorDelegate)
		{
			setItemDelegate(new ColorSmallItemDelegate());
			delete oldDelegate;
			m_type = type;
		}
	}
}

void ColorListBox::setShowNoneColor(bool showNone)
{
	ColorListModel* colorListModel = qobject_cast<ColorListModel*>(model());
	if (colorListModel)
		colorListModel->setShowNoneColor(showNone);
}

void ColorListBox::slotRightClick()
{
	QSignalBlocker sigBlocker(this);
	QString currentSel = currentColor();
	if (currentSel.isEmpty())
		return;

	QMenu *pmen = new QMenu();
	pmen->addAction( tr("Sort by Name"));
	pmen->addAction( tr("Sort by Color"));
	pmen->addAction( tr("Sort by Type"));
	sortRule = pmen->actions().indexOf(pmen->exec(QCursor::pos()));
	delete pmen;

	ColorListModel* colorListModel = qobject_cast<ColorListModel*>(model());
	if (!colorListModel)
		return;

	if (sortRule == 0)
		colorListModel->setSortRule(ColorListModel::SortByName);
	else if (sortRule == 1)
		colorListModel->setSortRule(ColorListModel::SortByValues);
	else if (sortRule == 2)
		colorListModel->setSortRule(ColorListModel::SortByType);

	if (!currentSel.isEmpty())
		setCurrentColor(currentSel);
}

QString ColorListBox::text(int row) const
{
	QVariant varText = data(row, Qt::DisplayRole);
	return varText.toString();
}

void ColorListBox::updateBox(ColorList& list)
{
	bool showNoneColor = false;

	clear();
	reset();

	ColorListModel* colorModel = qobject_cast<ColorListModel*>(this->model());
	if (colorModel)
		showNoneColor = colorModel->isNoneColorShown();
	setColors(list, showNoneColor);
}

bool ColorListBox::viewportEvent(QEvent *event)
{
	if (event != nullptr)
	{
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
				emit contextMenuRequested();
				return true;
			}
		}
	}
	return QListView::viewportEvent(event);
}
