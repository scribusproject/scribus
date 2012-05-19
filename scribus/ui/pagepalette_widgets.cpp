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
#include "pagelayout.h"
#include "pagepalette_widgets.h"
#include "sccombobox.h"
#include "scpage.h"
#include "scribus.h"
#include "scribusview.h"
#include "util_icon.h"

/* IconItems Code */
SeItem::SeItem(QString text, uint nr, const QPixmap& Pix) : QTableWidgetItem(QIcon(Pix), "", 1002)
{
	pageNumber = nr;
	pageName = text;
	setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

const QString& SeItem::getPageName()
{
	return pageName;
}

/* ListBox Subclass */
SeList::SeList(QWidget* parent) : QListWidget(parent)
{
	Mpressed = false;
	setAcceptDrops(true);
}

void SeList::mouseReleaseEvent(QMouseEvent *m)
{
	Mpressed = false;
	if (m->button() == Qt::RightButton)
	{
		QMenu *pmen = new QMenu();
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
        QAction *px = pmen->addAction( tr("Show Page Previews"), this, SLOT(toggleThumbnail()));
		px->setCheckable(true);
		if (Thumb)
			px->setChecked(true);
		pmen->exec(QCursor::pos());
		delete pmen;
	}
	QListWidget::mouseReleaseEvent(m);
}

void SeList::toggleThumbnail()
{
	Thumb = !Thumb;
	emit thumbnailChanged();
}

void SeList::mousePressEvent(QMouseEvent* e)
{
	e->accept();
	CurItem = 0;
	QListWidgetItem *i = itemAt(e->pos());
	if (i)
	{
		CurItem = i;
		Mpos = e->pos();
		Mpressed = true;
	}
	QListWidget::mousePressEvent(e);
}

void SeList::mouseMoveEvent(QMouseEvent* e)
{
	if ((Mpressed) && ((Mpos - e->pos()).manhattanLength() > 4))
	{
		Mpressed = false;
		QListWidgetItem *i = itemAt(Mpos);
		if (i)
		{
			QMimeData *mimeData = new QMimeData;
			mimeData->setData("page/magic", "1"+i->text().toLocal8Bit());
			mimeData->setText("1"+i->text());
			QDrag *dr = new QDrag(this);
			dr->setMimeData(mimeData);
			const QPixmap& pm = loadIcon("doc.png");
			dr->setPixmap(pm);
		//	dr->setDragCursor(pm, Qt::CopyAction);
		//	dr->setDragCursor(pm, Qt::MoveAction);
			dr->exec(Qt::CopyAction | Qt::MoveAction);
			QApplication::setOverrideCursor(Qt::ArrowCursor);
		}
	}
}

void SeList::keyPressEvent(QKeyEvent * e)
{
	bool accepted = false;
	int k = e->key();
	if (k == Qt::Key_Delete)
	{
		if (currentItem())
		{
			e->accept();
			if (!QMessageBox::question(this, tr("Delete Master Page?"),
				"<qt>" + tr("Are you sure you want to delete this master page?") + "</qt>",
				CommonStrings::trYesKey, CommonStrings::trNoKey, QString::null, 0, 1 ))
			{
				emit DelMaster(currentItem()->text());
			}
			accepted = true;
		}
	}
	if (!accepted)
		QListWidget::keyPressEvent(e);
}


/* QTable Subclass */
SeView::SeView(QWidget* parent) : QTableWidget(parent)
{
	setDragEnabled(true);
	setAcceptDrops(true);
    setDropIndicatorShown(true);
//	viewport()->setAcceptDrops(true);
	setShowGrid(false);
	setWordWrap(true);
	Mpressed = false;
	Namen = true;
// 	setFocusPolicy(Qt::NoFocus);
}

void SeView::mousePressEvent(QMouseEvent* e)
{
	e->accept();
	Mpos = e->pos();
	Mpressed = true;
	QTableWidget::mousePressEvent(e);
}

void SeView::mouseReleaseEvent(QMouseEvent* e)
{
	e->accept();
	Mpressed = false;
/*	if (e->button() == RightButton)
	{
		QPopupMenu *pmen = new QPopupMenu();
		qApp->setOverrideCursor(QCursor(Qt::ArrowCursor), true);
		int px = pmen->insertItem( tr("Show Master Page Names"), this, SLOT(ToggleNam()));
		if (Namen)
			pmen->setItemChecked(px, true);
		pmen->exec(QCursor::pos());
		delete pmen;
	} */
	emit Click(rowAt(e->pos().y()), columnAt(e->pos().x()), e->button());
	QTableWidget::mouseReleaseEvent(e);
}

void SeView::ToggleNam()
{
/*	Namen = !Namen;
	int val = 35;
	if (Namen)
		val = 100;
	setColumnWidth(1, val);
	setColumnWidth(3, val);
	hide();
	show(); */
}

void SeView::mouseMoveEvent(QMouseEvent* e)
{
	if ((Mpressed) && ((Mpos - e->pos()).manhattanLength() > 4))
	{
		Mpressed = false;
		int a = rowAt(e->pos().y());
		int b = columnAt(e->pos().x());
		if ((a != -1) && (b != -1))
		{
			QTableWidgetItem* ite = item(a, b);
			if (ite != 0)
			{
				if (ite->type() == 1002)
				{
					SeItem* it = (SeItem*)ite;
					QString str(it->pageName);
					bool dummy;
					int p = GetPage(a, b, &dummy);
					QString tmp;
					QMimeData *mimeData = new QMimeData;
					mimeData->setData("page/magic", "2 "+tmp.setNum(p).toLocal8Bit()+" "+str.toLocal8Bit());
					mimeData->setText("2 "+tmp.setNum(p)+" "+str);
					QDrag *dr = new QDrag(this);
					dr->setMimeData(mimeData);
					const QPixmap& pm = loadIcon("doc.png");
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
		ClearPix();
		if (str.startsWith("1"))
		{
			int a = rowAt(e->pos().y());
			int b = columnAt(e->pos().x());
			int p;
			tmp = str.remove(0,1);
			if ((a == -1) || (b == -1))
				return;
			if (a == rowCount()-1)
			{
				emit NewPage(MaxC+1, tmp);
				return;
			}
			p = GetPage(a, b, &lastPage);
			if (columnCount() == 1)
			{
				if ((a % 2) == 0)
					emit NewPage(p, tmp);
				else
				{
					emit UseTemp(tmp, p);
					QTableWidgetItem* ite = item(a, b);
					if (ite == 0)
						return;
					if (ite->type() == 1002)
					{
						SeItem* it = (SeItem*)ite;
						it->pageName = tmp;
					}
				}
				return;
			}
			else
			{
				if ((b % 2) == 0)
				{
					if (lastPage)
						emit NewPage(p+1, tmp);
					else
						emit NewPage(p, tmp);
				}
				else
				{
					emit UseTemp(tmp, p);
					QTableWidgetItem* ite = item(a, b);
					if (ite == 0)
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
		if (str.startsWith("2"))
		{
			int st = str.indexOf(" ");
			int en = str.indexOf(" ", st+1);
			tmp = str.mid(en+1);
			int dr = str.mid(st, en-st).toInt();
			int a = rowAt(e->pos().y());
			int b = columnAt(e->pos().x());
			int p;
			if ((a == -1) || (b == -1))
				return;
			QTableWidgetItem* ite = item(a, b);
			p = GetPage(a, b, &lastPage);
			if (a == rowCount()-1)
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
					if (ite == 0)
						return;
					SeItem* it = (SeItem*)ite;
					it->pageName = tmp;
				}
				return;
			}
			else
			{
				if ((b % 2) == 0)
					emit movePage(dr, lastPage ? p+1 : p);
				else
				{
					emit UseTemp(tmp, p);
					if (ite == 0)
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
}

void SeView::dragEnterEvent(QDragEnterEvent *e)
{
	if (e->mimeData()->hasFormat("page/magic"))
		e->acceptProposedAction();
}

void SeView::dragLeaveEvent(QDragLeaveEvent *)
{
	ClearPix();
}

void SeView::dragMoveEvent(QDragMoveEvent *e)
{
	QString str, tmp;
	if (e->mimeData()->hasFormat("page/magic"))
	{
		e->acceptProposedAction();
		str = e->mimeData()->text();
		int a = rowAt(e->pos().y());
		int b = columnAt(e->pos().x());
		ClearPix();
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
}

void SeView::keyPressEvent(QKeyEvent * e)
{
	bool accepted = false;
	int k = e->key();
	if (k == Qt::Key_Delete)
	{
		e->accept();
		if (!QMessageBox::question(this, tr("Delete Page?"),
			 "<qt>" + tr("Are you sure you want to delete this page?") + "</qt>",
						 CommonStrings::trYesKey, CommonStrings::trNoKey, QString::null, 0, 1 ))
		{
			bool dummy;
			int pageToDelete=GetPage(currentRow(), currentColumn(), &dummy);
			emit DelPage(pageToDelete);
		}
		accepted = true;
	}
	if (!accepted)
		QTableWidget::keyPressEvent(e);
}

void SeView::ClearPix()
{
	int counter = 0;
	int rowcounter = 0;
	for (int a = 0; a < rowCount(); ++a)
	{
		counter = 0;
		if (columnCount() == 1)
		{
			if ((a % 2) == 0)
			{
				item(rowcounter, 0)->setBackground(Qt::white);
				rowcounter += 2;
			}
		}
		else
		{
			for (int b = 0; b < columnCount(); ++b)
			{
				if ((b % 2) == 0)
				{
					item(rowcounter, counter)->setBackground(Qt::white);
					counter += 2;
				}
			}
			rowcounter++;
		}
	}
	for (int c = 0; c < columnCount(); ++c)
	{
		item(rowCount()-1, c)->setBackground(Qt::white);
	}
}

int SeView::GetPage(int r, int c, bool *last)
{
	int counter = firstP;
	int rowcounter = 0;
	int ret = MaxC;
	*last = false;
	if (r == rowCount()-1)
	{
		*last = true;
		return ret;
	}
	if ((r == 0) && (c < firstP*colmult+coladd))
		return 0;
	for (int a = 0; a < MaxC+1; ++a)
	{
		if ((rowcounter*rowmult+rowadd == r) && (counter*colmult+coladd == c))
		{
			ret = a;
			return ret;
		}
		else
		{
			if (columnCount() == 1)
			{
				if ((rowcounter*rowmult) == r)
				{
					ret = a;
					return ret;
				}
			}
			else
			{
				if ((counter*colmult == c) && (rowcounter*rowmult+rowadd == r))
				{
					ret = a;
					return ret;
				}
			}
		}
		counter++;
		if (counter > cols-1)
		{
			counter = 0;
			rowcounter++;
		}
	}
	*last = true;
	return ret;
}

/* Der Muelleimer */
TrashBin::TrashBin(QWidget * parent) : QLabel(parent)
{
	Normal = loadIcon("trashcan.png");
	Offen = loadIcon("trashcan2.png");
	setPixmap(Normal);
	setScaledContents(false);
	setAcceptDrops(true);
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
			emit DelPage(str.mid(st, en-st).toInt());
		}
		if (str.startsWith("1"))
		{
			tmp = str.remove(0,1);
			emit DelMaster(tmp);
		}
	}
}
