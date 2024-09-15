/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QApplication>
#include <QPainter>

#include "pagesize.h"
#include "pagesizelist.h"
#include "prefsmanager.h"
#include "ui/delegates/sclistitemdelegate.h"


PageSizeList::PageSizeList(QWidget* parent) :
	QListView(parent)
{
	m_model = new QStandardItemModel();
	setModel(m_model);
	setDragEnabled(false);
	setViewMode(QListView::IconMode);
	setFlow(QListView::LeftToRight);
	setWordWrap(true);
	setAcceptDrops(false);
	setDropIndicatorShown(false);
	setDragDropMode(QAbstractItemView::NoDragDrop);
	setResizeMode(QListView::Adjust);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setFocusPolicy(Qt::NoFocus);
	setIconSize(QSize(80, 80));
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
#ifdef Q_OS_MACOS
	setGridSize(QSize(150, 150));
#else
	setGridSize(QSize(160, 160));
#endif
	setItemDelegate(new ScListItemDelegate(QListWidget::IconMode, iconSize()));
	setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
}

void PageSizeList::setFormat(QString format)
{
	loadPageSizes(format, m_orientation, m_category);
	m_name = format;
	setSortMode(m_sortMode);
}

void PageSizeList::setOrientation(int orientation)
{
	loadPageSizes(m_name, orientation, m_category);
	m_orientation = orientation;
	setSortMode(m_sortMode);
}

void PageSizeList::setCategory(PageSizeInfo::Category category)
{
	loadPageSizes(m_name, m_orientation, category);
	m_category = category;
	setSortMode(m_sortMode);
}

void PageSizeList::setSortMode(SortMode sortMode)
{
	QSignalBlocker sig(this);

	m_sortMode = sortMode;

	switch (sortMode)
	{
	case NameAsc:
		m_model->setSortRole(ItemData::Name);
		m_model->sort(0, Qt::AscendingOrder);
		break;
	case NameDesc:
		m_model->setSortRole(ItemData::Name);
		m_model->sort(0, Qt::DescendingOrder);
		break;
	case DimensionAsc:
		m_model->setSortRole(ItemData::Dimension);
		m_model->sort(0, Qt::AscendingOrder);
		break;
	case DimensionDesc:
		m_model->setSortRole(ItemData::Dimension);
		m_model->sort(0, Qt::DescendingOrder);
		break;
	}
}

void PageSizeList::setValues(QString format, int orientation, PageSizeInfo::Category category, SortMode sortMode)
{
	loadPageSizes(format, orientation, category);
	m_name = format;
	m_orientation = orientation;
	m_category = category;
	setSortMode(sortMode);
}

void PageSizeList::loadPageSizes(QString name, int orientation, PageSizeInfo::Category category)
{
	QSignalBlocker sig(this);

	PageSize ps(name);
	PageSize pref(PrefsManager::instance().appPrefs.docSetupPrefs.pageSize);

	int sel = -1;

	// reset sorting to default
	m_model->setSortRole(ItemData::Name);
	m_model->sort(0, Qt::AscendingOrder);

	if (m_category == category && this->selectionModel()->currentIndex().isValid())
		sel = this->selectionModel()->currentIndex().row();

	m_model->clear();

	foreach (auto item, ps.pageSizes())
	{
		QSize size;
		size.setWidth(orientation == 0 ? item.width : item.height);
		size.setHeight(orientation == 0 ? item.height : item.width);

		// Add items of selected category or all preferred and defaults
		if (item.category == category ||
				(category == PageSizeInfo::Preferred && ps.activePageSizes().contains(item.sizeName)) ||
				(category == PageSizeInfo::Preferred && item.sizeName == pref.name()))
		{
			QStandardItem* itemA = new QStandardItem();
			itemA->setText(item.trSizeName);
			itemA->setIcon(sizePreview(this->iconSize(), size));
			itemA->setData(QVariant(item.sizeLabel), ItemData::SizeLabel);
			itemA->setData(QVariant(item.pageUnitIndex), ItemData::Unit);
			itemA->setData(QVariant(item.category), ItemData::Category);
			itemA->setData(QVariant(item.sizeName), ItemData::Name);
			itemA->setData(QVariant(item.width * item.height), ItemData::Dimension);
			m_model->appendRow(itemA);

			if (sel == -1 && item.sizeName == ps.name())
				sel = itemA->row();
		}
	}

	if (sel > -1)
		this->selectionModel()->setCurrentIndex(m_model->item(sel)->index(), QItemSelectionModel::Select );
}

void PageSizeList::updateGeometries()
{
	QListView::updateGeometries();
	verticalScrollBar()->setSingleStep(10);
}

QIcon PageSizeList::sizePreview(QSize iconSize, QSize pageSize) const
{
	double devicePixelRatio = qApp->devicePixelRatio();
	double max = mm2pts(500 * devicePixelRatio); // reference for scale: large side of B3
	double iW = iconSize.width() * devicePixelRatio;
	double iH = iconSize.height() * devicePixelRatio;
	double pW = pageSize.width() * devicePixelRatio;
	double pH = pageSize.height() * devicePixelRatio;
	bool isVertical = pW <= pH;

	QPixmap pix(QSize(iW, iH));
	pix.fill(Qt::transparent);

	// vertical
	double mod = qMin(pH / max, 1.0);
	double ratio = pW / pH;
	int height = iH * mod;
	int width = height * ratio;
	int x = (iW - width) / 2;
	int y = iH - height;

	// horizontal
	if (!isVertical)
	{
		mod = qMin(pW / max, 1.0);
		ratio = pH / pW;
		width = iW * mod;
		height = width * ratio;
		x = (iW - width) / 2;
		y = iH - height;
	}

	QRect page(x, y, width, height);

	QLinearGradient m_gradient(page.topLeft(), page.bottomRight());
	m_gradient.setColorAt(0.0, Qt::white);
	m_gradient.setColorAt(1.0, QColor(240, 240, 240));

	QPainter painter(&pix);
	painter.setPen(QPen(palette().text().color()));
	painter.setBrush(m_gradient);
	painter.drawRect(page.adjusted(0, 0, -1, -1));
	painter.end();

	return QIcon(pix);
}
