#include "gradientlistbox.h"

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

#include "models/gradientlistmodel.h"
#include "commonstrings.h"
#include "iconmanager.h"
#include "scribusapp.h"
#include "scribusdoc.h"
#include "scpainter.h"


class SCRIBUS_API GradientWideItemDelegate : public ScListBoxPixmap<48, 16>
{
public:
	GradientWideItemDelegate(): ScListBoxPixmap<48, 16>() {};
	~GradientWideItemDelegate() = default;

	void redraw(const QVariant&) const override;
	QString text(const QVariant&) const override;
};


void GradientWideItemDelegate::redraw(const QVariant& data) const
{
	int w = 48;
	int h = 16;

	QPixmap* pPixmap = ScListBoxPixmap<48, 16>::pmap.data();
	pPixmap->fill(Qt::transparent);

	if (data.canConvert<GradientPixmapValue>())
	{
		GradientPixmapValue item(data.value<GradientPixmapValue>());
		QImage pixm(w, h, QImage::Format_ARGB32_Premultiplied);

		ScPainter *p = new ScPainter(&pixm, w, h);
		p->setPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->setFillMode(ScPainter::Gradient);
		p->fill_gradient = item.m_gradient;
		p->setGradient(VGradient::linear, FPoint(0,h/2), FPoint(w, h/2), FPoint(0,0), 1, 0);
		p->drawRect(0, 0, w, h);
		p->end();
		delete p;

		QPainter pb(pPixmap);
		QBrush b(QColor(205,205,205), IconManager::instance().loadPixmap("testfill.png"));
		pb.fillRect(0, 0, w, h, b);
		pb.drawImage(QRect(0, 0, w, h), pixm);
		pb.end();
	}
}

QString GradientWideItemDelegate::text(const QVariant& data) const
{
	if (data.canConvert<GradientPixmapValue>())
		return data.value<GradientPixmapValue>().m_name;
	return data.toString();
}


int GradientListBox::initialized;
int GradientListBox::sortRule;

GradientListBox::GradientListBox(QWidget * parent)
	: QListView(parent)
{
	if (initialized != 12345)
		sortRule = 0;
	initialized = 12345;
	QListView::setModel(new GradientListModel(this));
	setPixmapType(GradientListBox::widePixmap);

	connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(emitItemClicked(QModelIndex)));
	connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(emitItemDoubleClicked(QModelIndex)));
	connect(this->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(emitCurrentChanged(QModelIndex,QModelIndex)));
	connect(this->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SIGNAL(itemSelectionChanged()));
	connect(this, SIGNAL(contextMenuRequested()), this, SLOT(slotRightClick()));
}

GradientListBox::GradientListBox(GradientListBox::PixmapType type, QWidget * parent)
	: QListView(parent)
{
	cList = nullptr;
	if (initialized != 12345)
		sortRule = 0;
	initialized = 12345;
	QListView::setModel(new GradientListModel(this));
	setPixmapType(type);

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));

	connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(emitItemClicked(QModelIndex)));
	connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(emitItemDoubleClicked(QModelIndex)));
	connect(this->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(emitCurrentChanged(QModelIndex,QModelIndex)));
	connect(this->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SIGNAL(itemSelectionChanged()));
	connect(this, SIGNAL(contextMenuRequested()), this, SLOT(slotRightClick()));
}

GradientListBox::~GradientListBox()
{
	if (itemDelegate())
		delete itemDelegate();
	clear();
}

void GradientListBox::clear()
{
	QAbstractItemModel* itemModel = model();
	itemModel->removeRows(0, itemModel->rowCount());
}

int GradientListBox::count() const
{
	return this->model()->rowCount();
}

void GradientListBox::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QListView::changeEvent(e);
}

void GradientListBox::emitCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
	QPersistentModelIndex persistentCurrent = current;

	QString text = model()->data(current, Qt::DisplayRole).toString();
	emit currentTextChanged(text);
	emit currentRowChanged(persistentCurrent.row());
}

void GradientListBox::emitItemClicked(const QModelIndex &current)
{
	QPersistentModelIndex persistentCurrent = current;
	emit itemClicked(persistentCurrent.row());
}

void GradientListBox::emitItemDoubleClicked(const QModelIndex &current)
{
	QPersistentModelIndex persistentCurrent = current;
	emit itemDoubleClicked(persistentCurrent.row());
}

void GradientListBox::iconSetChange()
{

}

void GradientListBox::languageChange()
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

QString GradientListBox::currentGradientName() const
{
	if (currentRow() >= 0)
	{
		QAbstractItemModel* itemModel = model();
		return itemModel->data(currentIndex(), Qt::DisplayRole).toString();
	}
	return CommonStrings::tr_NoneColor;
}

VGradient GradientListBox::currentGradient() const
{
	if (currentRow() >= 0)
	{
		QAbstractItemModel* itemModel = model();
		GradientPixmapValue gpv = qvariant_cast<GradientPixmapValue>(itemModel->data(currentIndex(), Qt::UserRole));
		return gpv.m_gradient;
	}
	return VGradient();

}

int GradientListBox::currentRow() const
{
	return currentIndex().row();
}

QVariant GradientListBox::data(int row, int role) const
{
	QModelIndex index = model()->index(row, 0);
	return model()->data(index, role);
}

QStringList GradientListBox::findGradients(const QString &name, Qt::MatchFlags flags) const
{
	QStringList foundGradients;
	QAbstractItemModel* currentModel = model();

	QModelIndex firstIndex = currentModel->index(0, 0, QModelIndex());
	QModelIndexList indexes = currentModel->match(firstIndex, Qt::DisplayRole, name, -1, flags);
	for (int i = 0; i < indexes.count(); ++i)
	{
		QModelIndex modelIndex = indexes.at(i);
		QVariant modelData = currentModel->data(modelIndex, Qt::DisplayRole);
		foundGradients.append(modelData.toString());
	}

	return foundGradients;
}

bool GradientListBox::hasSelection() const
{
	return this->selectionModel()->hasSelection();
}

void GradientListBox::insertItem(int row, const VGradient& gradient, const QString& gradientName)
{
	GradientListModel* gradientListModel = qobject_cast<GradientListModel*>(model());
	if (!gradientListModel)
		return;

	ScribusDoc* doc = nullptr;
	if (cList)
		doc = cList->document();

	GradientPixmapValue value(gradient, doc, gradientName);
	gradientListModel->insert(row, value);
}

bool GradientListBox::isNoneColorShown() const
{
	GradientListModel* gradientListModel = qobject_cast<GradientListModel*>(model());
	if (gradientListModel)
		return gradientListModel->isNoneColorShown();
	return false;
}

void GradientListBox::removeItem(int i)
{
	// None color item cannot be removed
	if (isNoneColorShown() && (i == 0))
		return;

	model()->removeRow(i);
}

int GradientListBox::row(const QString& gradientName)
{
	QAbstractItemModel* currentModel = model();

	QModelIndex firstIndex = currentModel->index(0, 0, QModelIndex());
	QModelIndexList indexes = currentModel->match(firstIndex, Qt::DisplayRole, gradientName, -1, Qt::MatchExactly);
	if (indexes.count() > 0)
	{
		const QModelIndex& first = indexes.at(0);
		return first.row();
	}
	return -1;
}

void GradientListBox::setCurrentGradient(QString gradientName)
{
	QModelIndex firstIndex = model()->index(0, 0, QModelIndex());
	QModelIndexList indexes = this->model()->match(firstIndex, Qt::DisplayRole, gradientName, -1, Qt::MatchExactly);
	if (indexes.count() > 0)
		this->selectionModel()->setCurrentIndex(indexes[0], QItemSelectionModel::ClearAndSelect);
}

void GradientListBox::setGradients(GradientList& list, bool insertNone)
{
	ColorList::Iterator it;

	GradientListModel* gradientModel = qobject_cast<GradientListModel*>(this->model());
	if (!gradientModel)
		return;

	cList = &list;

	gradientModel->setGradientList(list, insertNone);
}

void GradientListBox::setCurrentRow(int row)
{
	QModelIndex index = this->model()->index(row, 0);
	selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
}

void GradientListBox::setPixmapType(GradientListBox::PixmapType type)
{
	if (type == GradientListBox::widePixmap)
	{
		QAbstractItemDelegate* oldDelegate = itemDelegate();
		GradientWideItemDelegate* gradientDelegate = dynamic_cast<GradientWideItemDelegate*>(oldDelegate);
		if (!gradientDelegate)
		{
			setItemDelegate(new GradientWideItemDelegate());
			delete oldDelegate;
			m_type = type;
		}
	}

}

void GradientListBox::setShowNoneColor(bool showNone)
{
	GradientListModel* gradientListModel = qobject_cast<GradientListModel*>(model());
	if (gradientListModel)
		gradientListModel->setShowNoneColor(showNone);
}

void GradientListBox::slotRightClick()
{
	QSignalBlocker sigBlocker(this);
	QString currentSel = currentGradientName();
	if (currentSel.isEmpty())
		return;

	QMenu *pmen = new QMenu();
	pmen->addAction( tr("Sort by Name"));
	pmen->addAction( tr("Sort by Type"));
	sortRule = pmen->actions().indexOf(pmen->exec(QCursor::pos()));
	delete pmen;

	GradientListModel* gradientListModel = qobject_cast<GradientListModel*>(model());
	if (!gradientListModel)
		return;

	if (sortRule == 0)
		gradientListModel->setSortRule(GradientListModel::SortByName);
	else if (sortRule == 1)
		gradientListModel->setSortRule(GradientListModel::SortByType);

	if (!currentSel.isEmpty())
		setCurrentGradient(currentSel);
}

QString GradientListBox::text(int row) const
{
	QVariant varText = data(row, Qt::DisplayRole);
	return varText.toString();
}

void GradientListBox::updateBox(GradientList &list)
{
	bool showNoneColor = false;

	clear();
	reset();

	GradientListModel* gradientModel = qobject_cast<GradientListModel*>(this->model());
	if (gradientModel)
		showNoneColor = gradientModel->isNoneColorShown();
	setGradients(list, showNoneColor);
}

bool GradientListBox::viewportEvent(QEvent *event)
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

