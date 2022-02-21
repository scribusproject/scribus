/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QApplication>
#include <QBitmap>
#include <QCursor>
#include <QDrag>
#include <QEvent>
#include <QHeaderView>
#include <QLabel>
#include <QList>
#include <QMenu>
#include <QMimeData>
#include <QMessageBox>
#include <QPainter>

#include "commonstrings.h"
#include "iconmanager.h"
#include "pagelayout.h"
#include "pagepalette_widgets.h"
#include "scpage.h"
#include "scribusapp.h"
#include "ui/scmessagebox.h"


/* IconItems Code */
SeItem::SeItem(const QString& text, uint nr, const QPixmap& pix)
	: QTableWidgetItem(QIcon(pix), "", 1002),
	  pageNumber(nr),
	  pageName(text)
{
	setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

const QString& SeItem::getPageName()
{
	return pageName;
}

/* ListBox Subclass */
SeList::SeList(QWidget* parent) : QListWidget(parent)
{
	setAcceptDrops(true);
}

void SeList::mouseReleaseEvent(QMouseEvent *m)
{
	m_mousePressed = false;
	if (m->button() == Qt::RightButton)
	{
		QMenu *pmen = new QMenu();
//		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		QAction *px = pmen->addAction( tr("Show Page Previews"), this, SLOT(toggleThumbnail()));
		px->setCheckable(true);
		if (m_thumb)
			px->setChecked(true);
		pmen->exec(QCursor::pos());
		delete pmen;
	}
	QListWidget::mouseReleaseEvent(m);
}

void SeList::toggleThumbnail()
{
	m_thumb = !m_thumb;
	emit thumbnailChanged();
}

void SeList::mousePressEvent(QMouseEvent* e)
{
	e->accept();
	m_currItem = nullptr;
	QListWidgetItem *i = itemAt(e->pos());
	if (i)
	{
		m_currItem = i;
		m_mousePos = e->pos();
		m_mousePressed = true;
	}
	QListWidget::mousePressEvent(e);
}

void SeList::mouseMoveEvent(QMouseEvent* e)
{
	if ((m_mousePressed) && ((m_mousePos - e->pos()).manhattanLength() > 4))
	{
		m_mousePressed = false;
		QListWidgetItem *item = itemAt(m_mousePos);
		if (!item)
			return;
		QMimeData *mimeData = new QMimeData;
		QString pageName = item->data(Qt::UserRole).toString();
		mimeData->setData("page/magic", "1" + pageName.toLocal8Bit());
		mimeData->setText("1" + pageName);
		QDrag *dr = new QDrag(this);
		dr->setMimeData(mimeData);
		const QPixmap& pm = IconManager::instance().loadPixmap("doc.png");
		dr->setPixmap(pm);
	//	dr->setDragCursor(pm, Qt::CopyAction);
	//	dr->setDragCursor(pm, Qt::MoveAction);
		dr->exec(Qt::CopyAction | Qt::MoveAction);
		QApplication::setOverrideCursor(Qt::ArrowCursor);
	}
}

void SeList::keyPressEvent(QKeyEvent * e)
{
	int k = e->key();
	if (k == Qt::Key_Delete)
	{
		if (currentItem())
		{
			e->accept();
			emit delMasterRequest(currentItem()->text());
			return;
		}
	}
	
	QListWidget::keyPressEvent(e);
}


/* QTable Subclass */
SeView::SeView(QWidget* parent) : QTableWidget(parent)
{
	setDragEnabled(true);
	setAcceptDrops(true);
	setDropIndicatorShown(true);
	setShowGrid(false);
	setWordWrap(true);
}

void SeView::mousePressEvent(QMouseEvent* e)
{
	e->accept();
	m_mousePos = e->pos();
	m_mousePressed = true;
	QTableWidget::mousePressEvent(e);
}

void SeView::mouseReleaseEvent(QMouseEvent* e)
{
	e->accept();
	m_mousePressed = false;

	emit Click(rowAt(e->pos().y()), columnAt(e->pos().x()), e->button());
	QTableWidget::mouseReleaseEvent(e);
}

void SeView::mouseMoveEvent(QMouseEvent* e)
{
	if ((m_mousePressed) && ((m_mousePos - e->pos()).manhattanLength() > 4))
	{
		m_mousePressed = false;
		int a = rowAt(e->pos().y());
		int b = columnAt(e->pos().x());
		if ((a != -1) && (b != -1))
		{
			QTableWidgetItem* ite = item(a, b);
			if (ite != nullptr)
			{
				if (ite->type() == 1002)
				{
					SeItem* it = (SeItem*)ite;
					QString str(it->pageName);
					bool dummy;
					int p = getPage(a, b, &dummy);
					QString tmp;
					QMimeData *mimeData = new QMimeData;
					mimeData->setData("page/magic", "2 " + tmp.setNum(p).toLocal8Bit() + " " + str.toLocal8Bit());
					mimeData->setText("2 " + tmp.setNum(p) + " " + str);
					QDrag *dr = new QDrag(this);
					dr->setMimeData(mimeData);
					const QPixmap& pm = IconManager::instance().loadPixmap("doc.png");
					dr->setPixmap(pm);
				//	dr->setDragCursor(pm, Qt::CopyAction);
				//	dr->setDragCursor(pm, Qt::MoveAction);
					dr->exec(Qt::CopyAction | Qt::MoveAction);
					QApplication::setOverrideCursor(Qt::ArrowCursor);
				}
			}
		}
	}
	QTableWidget::mouseMoveEvent(e);
}

void SeView::dropEvent(QDropEvent * e)
{
	QString str, tmp;
	bool lastPage = false;
	if (e->mimeData()->hasFormat("page/magic"))
	{
		e->setDropAction(Qt::MoveAction);
		e->accept();
		// HACK to prevent strange Qt4 cursor behaviour after dropping. It's examined by Trolltech now - PV.
		// It's the one and only reason why to include QApplication here.
		// But sadly this destroys our normal Cursors
		// Fixed at least in Qt-4.4.2
//		QApplication::restoreOverrideCursor();
		str = e->mimeData()->text();
		clearPix();
		if (str.startsWith("1"))
		{
			int a = rowAt(e->pos().y());
			int b = columnAt(e->pos().x());
			int p;
			tmp = str.remove(0,1);
			if ((a == -1) || (b == -1))
				return;
			if (a == rowCount() - 1)
			{
				emit NewPage(m_pageCount, tmp);
				return;
			}
			p = getPage(a, b, &lastPage);
			if (columnCount() == 1)
			{
				if ((a % 2) == 0)
					emit NewPage(p, tmp);
				else
				{
					emit UseTemp(tmp, p);
					QTableWidgetItem* ite = item(a, b);
					if (ite == nullptr)
						return;
					if (ite->type() == 1002)
					{
						SeItem* it = (SeItem*)ite;
						it->pageName = tmp;
					}
				}
				return;
			}
			if ((b % 2) == 0)
			{
				if (lastPage)
					emit NewPage(p + 1, tmp);
				else
					emit NewPage(p, tmp);
			}
			else
			{
				emit UseTemp(tmp, p);
				QTableWidgetItem* ite = item(a, b);
				if (ite == nullptr)
					return;
				if (ite->type() == 1002)
				{
					SeItem* it = (SeItem*)ite;
					it->pageName = tmp;
				}
			}
			return;
		}
		if (str.startsWith("2"))
		{
			int st = str.indexOf(" ");
			int en = str.indexOf(" ", st + 1);
			tmp = str.mid(en+1);
			int dr = str.midRef(st, en-st).toInt();
			int a = rowAt(e->pos().y());
			int b = columnAt(e->pos().x());
			if ((a == -1) || (b == -1))
				return;
			QTableWidgetItem* ite = item(a, b);
			int p = getPage(a, b, &lastPage);
			if (a == rowCount() - 1)
			{
				emit movePage(dr, p+1);
				return;
			}
			if (columnCount() == 1)
			{
				if ((a % 2) == 0)
					emit movePage(dr, p);
				else
				{
					emit UseTemp(tmp, p);
					if (ite == nullptr)
						return;
					SeItem* it = (SeItem*)ite;
					it->pageName = tmp;
				}
				return;
			}
			if ((b % 2) == 0)
				emit movePage(dr, lastPage ? p+1 : p);
			else
			{
				emit UseTemp(tmp, p);
				if (ite == nullptr)
					return;
				if (ite->type() == 1002)
				{
					SeItem* it = (SeItem*)ite;
					it->pageName = tmp;
				}
			}
			return;
		}
	}
}

void SeView::dragEnterEvent(QDragEnterEvent *e)
{
	if (e->mimeData()->hasFormat("page/magic"))
		e->acceptProposedAction();
}

void SeView::dragLeaveEvent(QDragLeaveEvent *)
{
	clearPix();
}

void SeView::dragMoveEvent(QDragMoveEvent *e)
{
	if (!e->mimeData()->hasFormat("page/magic"))
		return;
	e->acceptProposedAction();
	int a = rowAt(e->pos().y());
	int b = columnAt(e->pos().x());
	clearPix();
	if ((a == -1) || (b == -1))
		return;
	if (columnCount() == 1)
	{
		if ((a % 2) == 0)
		{
			item(a, 0)->setBackground(Qt::darkBlue);
		}
	}
	else
	{
		if (((b % 2) == 0) || (a == rowCount()-1))
		{
			item(a, b)->setBackground(Qt::darkBlue);
		}
	}
}

void SeView::keyPressEvent(QKeyEvent * e)
{
	int k = e->key();
	if (k == Qt::Key_Delete)
	{
		e->accept();
		bool dummy;
		int pageToDelete = getPage(currentRow(), currentColumn(), &dummy);
		emit delPageRequest(pageToDelete);
		return;
	}

	QTableWidget::keyPressEvent(e);
}

void SeView::clearPix()
{
	int rowcounter = 0;
	for (int i = 0; i < rowCount(); ++i)
	{
		int counter = 0;
		if (columnCount() == 1)
		{
			if ((i % 2) == 0)
			{
				item(rowcounter, 0)->setBackground(Qt::white);
				rowcounter += 2;
			}
		}
		else
		{
			for (int j = 0; j < columnCount(); ++j)
			{
				if ((j % 2) == 0)
				{
					item(rowcounter, counter)->setBackground(Qt::white);
					counter += 2;
				}
			}
			rowcounter++;
		}
	}
	for (int i = 0; i < columnCount(); ++i)
	{
		item(rowCount()-1, i)->setBackground(Qt::white);
	}
}

int SeView::getPage(int r, int c, bool *last)
{
	int counter = m_firstPage;
	int rowcounter = 0;
	int ret = m_pageCount - 1;
	*last = false;
	if (r == rowCount() - 1)
	{
		*last = true;
		return ret;
	}
	if ((r == 0) && (c < m_firstPage * m_colmult + m_coladd))
		return 0;
	for (int a = 0; a < m_pageCount; ++a)
	{
		if ((rowcounter * m_rowmult + m_rowadd == r) && (counter * m_colmult + m_coladd == c))
		{
			ret = a;
			return ret;
		}
		if (columnCount() == 1)
		{
			if ((rowcounter * m_rowmult) == r)
			{
				ret = a;
				return ret;
			}
		}
		else
		{
			if ((counter * m_colmult == c) && (rowcounter * m_rowmult + m_rowadd == r))
			{
				ret = a;
				return ret;
			}
		}
		counter++;
		if (counter > m_cols - 1)
		{
			counter = 0;
			rowcounter++;
		}
	}
	*last = true;
	return ret;
}

SeItem* SeView::getPageItem(int pageIndex)
{
	int rows = this->rowCount();
	int columns = this->columnCount();
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			QTableWidgetItem* tbItem = item(i, j);
			SeItem* pageItem = dynamic_cast<SeItem*>(tbItem);
			if (pageItem && pageItem->pageNumber == static_cast<uint>(pageIndex))
				return pageItem;
		}
	}
	return nullptr;
}

/* Der Muelleimer */
TrashBin::TrashBin(QWidget * parent) : QLabel(parent)
{
	Normal = IconManager::instance().loadPixmap("trashcan.png");
	Offen = IconManager::instance().loadPixmap("trashcan2.png");
	setPixmap(Normal);
	setScaledContents(false);
	setAcceptDrops(true);

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
}

void TrashBin::dragEnterEvent(QDragEnterEvent *e)
{
	if (e->mimeData()->hasFormat("page/magic"))
	{
		e->accept();
		setPixmap(Offen);
	}
}

void TrashBin::dragLeaveEvent(QDragLeaveEvent *)
{
	setPixmap(Normal);
}

void TrashBin::dropEvent(QDropEvent * e)
{
	setPixmap(Normal);
	QString str, tmp;
	if (e->mimeData()->hasFormat("page/magic"))
	{
		e->accept();
		str = e->mimeData()->text();
		if (str.startsWith("2"))
		{
			int st = str.indexOf(" ");
			int en = str.indexOf(" ", st+1);
			emit delPageRequest(str.midRef(st, en - st).toInt());
		}
		if (str.startsWith("1"))
		{
			tmp = str.remove(0,1);
			emit delMasterRequest(tmp);
		}
	}
}

void TrashBin::iconSetChange()
{
	Normal = IconManager::instance().loadPixmap("trashcan.png");
	Offen = IconManager::instance().loadPixmap("trashcan2.png");
	setPixmap(Normal);
}
