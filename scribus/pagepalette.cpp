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


#include "commonstrings.h"
#include "page.h"
#include "pagelayout.h"
#include "pagepalette.h"
#include "sccombobox.h"
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
			dr->setPixmap(loadIcon("doc.png"));
			dr->exec(Qt::CopyAction | Qt::MoveAction);
			QApplication::setOverrideCursor(Qt::ArrowCursor);
		}
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
			if (!QMessageBox::question(this, tr("Delete Master Page?"),
				"<qt>" + tr("Are you sure you want to delete this master page?") + "</qt>",
				CommonStrings::trYesKey, CommonStrings::trNoKey, QString::null, 0, 1 ))
			{
				emit DelMaster(currentItem()->text());
			}
		}
	}
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
					dr->setPixmap(loadIcon("doc.png"));
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
		QApplication::restoreOverrideCursor();
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
	}
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


PagePalette::PagePalette(QWidget* parent) : ScrPaletteBase( parent, "SP", false, 0)
{
	m_scMW=(ScribusMainWindow*)parent;
	PagePaletteLayout = new QVBoxLayout( this );
	PagePaletteLayout->setSpacing( 5 );
	PagePaletteLayout->setMargin( 5 );
	Splitter1 = new QSplitter( this );
	Splitter1->setOrientation( Qt::Vertical );
	QWidget* privateLayoutWidget = new QWidget( Splitter1 );
	Layout2 = new QVBoxLayout( privateLayoutWidget );
	Layout2->setMargin(0);
	Layout2->setSpacing(5);
	TextLabel1 = new QLabel( privateLayoutWidget );
	Layout2->addWidget( TextLabel1 );
	masterPageList = new SeList(privateLayoutWidget);
	masterPageList->setMinimumSize(QSize(130,70));
	masterPageList->Thumb = false;
	masterPageList->setIconSize(QSize(60, 60));
	Layout2->addWidget( masterPageList );
	QWidget* privateLayoutWidget_2 = new QWidget( Splitter1 );
	Layout3 = new QVBoxLayout( privateLayoutWidget_2);
	Layout3->setMargin(0);
	Layout3->setSpacing(5);
	TextLabel2 = new QLabel( privateLayoutWidget_2 );
	Layout3->addWidget( TextLabel2 );
	pageView = new SeView(privateLayoutWidget_2);
	QHeaderView *Header = pageView->verticalHeader();
	Header->setMovable(false);
	Header->setResizeMode(QHeaderView::Fixed);
	Header->hide();
	Header = pageView->horizontalHeader();
	Header->setMovable(false);
	Header->setResizeMode(QHeaderView::Fixed);
	Header->hide();
	pageView->setSortingEnabled(false);
	pageView->setSelectionMode( QAbstractItemView::NoSelection );
	pageView->setRowCount(1);
	pageView->setColumnCount(1);
	pageView->setIconSize(QSize(60, 60));
	pageView->setMinimumSize(QSize(130,120));
	Layout3->addWidget( pageView );
	PagePaletteLayout->addWidget( Splitter1 );

	Layout1 = new QHBoxLayout;
	Layout1->setSpacing( 5 );
	Layout1->setMargin( 0 );
	QList<PageSet> dummy;
	dummy.clear();
	struct PageSet pageS;
	pageS.Name = CommonStrings::trPageSet2;
	pageS.FirstPage = 0;
	pageS.Rows = 1;
	pageS.Columns = 1;
//	pageS.GapHorizontal = 0.0;
//	pageS.GapVertical = 0.0;
//	pageS.GapBelow = 40.0;
	pageS.pageNames.clear();
	pageS.pageNames.append(CommonStrings::trPageLocMiddleRight);
	dummy.append(pageS);
	pageLayout = new PageLayouts(this, dummy, false);
	Layout1->addWidget( pageLayout );

// 	QSpacerItem* spacer = new QSpacerItem( 15, 15, QSizePolicy::Expanding, QSizePolicy::Minimum );
// 	Layout1->addItem( spacer );

	Trash = new TrashBin( this );
	Trash->setMinimumSize(QSize(32,32));
	Trash->setMaximumSize(QSize(32,32));
	Layout1->addWidget( Trash );
	PagePaletteLayout->addLayout( Layout1 );
	pix = loadIcon("32/page-simple.png");
	currView = 0;
	Rebuild();
	languageChange();
	connect(masterPageList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(selMasterPage()));
	connect(masterPageList, SIGNAL(thumbnailChanged()), this, SLOT(rebuildMasters()));
	connect(masterPageList, SIGNAL(DelMaster(QString)), this, SLOT(deleteMasterPage(QString)));
	connect(pageView, SIGNAL(Click(int, int, int)), this, SLOT(pageView_gotoPage(int, int, int)));
	connect(pageView, SIGNAL(movePage(int, int)), this, SLOT(pageView_movePage(int, int)));
	connect(Trash, SIGNAL(DelMaster(QString)), this, SLOT(deleteMasterPage(QString)));
	connect(pageLayout, SIGNAL(selectedLayout(int )), this, SLOT(handlePageLayout(int )));
	connect(pageLayout, SIGNAL(selectedFirstPage(int )), this, SLOT(handleFirstPage(int )));
	connect(this, SIGNAL(EditTemp(QString)), m_scMW, SLOT(manageMasterPages(QString)));
	connect(pageView, SIGNAL(UseTemp(QString, int)), m_scMW, SLOT(Apply_MasterPage(QString, int)));
	connect(pageView, SIGNAL(NewPage(int, QString)), m_scMW, SLOT(slotNewPageP(int, QString)));
	connect(Trash, SIGNAL(DelPage(int)), m_scMW, SLOT(DeletePage2(int)));
	connect(pageView, SIGNAL(DelPage(int)), m_scMW, SLOT(DeletePage2(int)));
	connect(this, SIGNAL(gotoPage(int)), m_scMW, SLOT(selectPagesFromOutlines(int)));
}

void PagePalette::deleteMasterPage(QString tmp)
{
	if (tmp == CommonStrings::trMasterPageNormal)
		return;
	int Nr = currView->Doc->MasterNames[tmp];
	Page* Seite = currView->Doc->MasterPages.takeAt(Nr);
	delete Seite;
	currView->Doc->MasterNames.clear();

	for (int aa=0; aa < currView->Doc->MasterPages.count(); ++aa)
	{
		Seite = currView->Doc->MasterPages.at(aa);
		Seite->setPageNr(aa);
		if (currView->Doc->currentPageLayout == doublePage)
		{
			Seite->Margins.Left = Seite->LeftPg ? currView->Doc->pageMargins.Right : currView->Doc->pageMargins.Left;
			Seite->Margins.Right= Seite->LeftPg? currView->Doc->pageMargins.Left : currView->Doc->pageMargins.Right;
		}
		else
		{
			Seite->Margins.Right = currView->Doc->pageMargins.Right;
			Seite->Margins.Left = currView->Doc->pageMargins.Left;
		}
		Seite->Margins.Top = currView->Doc->pageMargins.Top;
		Seite->Margins.Bottom = currView->Doc->pageMargins.Bottom;
		currView->Doc->MasterNames[Seite->pageName()] = aa;
	}
	for (int b=0; b<currView->Doc->DocPages.count(); ++b)
	{
		if (currView->Doc->DocPages.at(b)->MPageNam == tmp)
			currView->Doc->DocPages.at(b)->MPageNam = CommonStrings::masterPageNormal;
	}
	currView->DrawNew();
	rebuildMasters();
	rebuildPages();
	currView->Doc->setModified(true);
}

void PagePalette::pageView_movePage(int r, int c)
{
	if (r == c)
		return;
	if (c > pageView->MaxC)
		currView->Doc->movePage(r, r + 1, c, 2);
	else
		currView->Doc->movePage(r, r + 1, c, 0);
	currView->reformPages();
	rebuildPages();
	currView->DrawNew();
//CB done by doc::reformpages
// 	currView->Doc->setModified(true);
}

void PagePalette::pageView_gotoPage(int r, int c, int b)
{
	int p;
	bool dummy;
	if ((b == Qt::LeftButton) && (r != -1) && (c != -1))
	{
		p = pageView->GetPage(r, c, &dummy);
		emit gotoPage(p);
	}
}

void PagePalette::enablePalette(const bool enabled)
{
	pageView->setEnabled(enabled);
	masterPageList->setEnabled(enabled);
	pageLayout->setEnabled(enabled);
}

void PagePalette::handlePageLayout(int layout)
{
	pageLayout->selectFirstP(currView->Doc->pageSets[layout].FirstPage);
	currView->Doc->resetPage(currView->Doc->pageMargins, layout);
	currView->reformPages();
	currView->DrawNew();
	currView->GotoPage(currView->Doc->currentPageNumber());
	rebuildPages();
//CB done by doc::reformpages
//	currView->Doc->setModified(true);
}

void PagePalette::handleFirstPage(int fp)
{
	currView->Doc->pageSets[currView->Doc->currentPageLayout].FirstPage = fp;
	currView->reformPages();
	currView->DrawNew();
	currView->GotoPage(currView->Doc->currentPageNumber());
	rebuildPages();
//CB done by doc::reformpages
// 	currView->Doc->setModified(true);
}

void PagePalette::rebuildMasters()
{
	if (m_scMW->ScriptRunning)
		return;
	masterPageList->clear();
	if (currView == 0)
		return;
	QPixmap pm;
	QMap<QString,int>::Iterator it;
	for (it = currView->Doc->MasterNames.begin(); it != currView->Doc->MasterNames.end(); ++it)
	{
		if (masterPageList->Thumb)
		{
			pm = QPixmap::fromImage(currView->MPageToPixmap(it.key(),60));
			new QListWidgetItem(QIcon(pm), it.key() == CommonStrings::masterPageNormal ? CommonStrings::trMasterPageNormal : it.key(), masterPageList);
		}
		else
			masterPageList->addItem(it.key() == CommonStrings::masterPageNormal ? CommonStrings::trMasterPageNormal : it.key());
	}
}

void PagePalette::rebuildPages()
{
	if (m_scMW->ScriptRunning)
		return;
	QString str;
	disconnect(pageLayout, SIGNAL(selectedLayout(int )), this, SLOT(handlePageLayout(int )));
	disconnect(pageLayout, SIGNAL(selectedFirstPage(int )), this, SLOT(handleFirstPage(int )));
	pageView->clearContents();
	pageView->setRowCount(1);
	pageView->setColumnCount(1);
	if (currView == 0)
	{
		connect(pageLayout, SIGNAL(selectedLayout(int )), this, SLOT(handlePageLayout(int )));
		connect(pageLayout, SIGNAL(selectedFirstPage(int )), this, SLOT(handleFirstPage(int )));
		return;
	}
	pageLayout->updateLayoutSelector(currView->Doc->pageSets);
	pageLayout->selectItem(currView->Doc->currentPageLayout);
	pageLayout->firstPage->setCurrentIndex(currView->Doc->pageSets[currView->Doc->currentPageLayout].FirstPage);
	pageView->MaxC = currView->Doc->Pages->count()-1;
	int counter, rowcounter, colmult, rowmult, coladd,rowadd;
	counter = currView->Doc->pageSets[currView->Doc->currentPageLayout].FirstPage;
	int cols = currView->Doc->pageSets[currView->Doc->currentPageLayout].Columns;
	int rows = (currView->Doc->Pages->count()+counter) / currView->Doc->pageSets[currView->Doc->currentPageLayout].Columns;
	if (((currView->Doc->Pages->count()+counter) % currView->Doc->pageSets[currView->Doc->currentPageLayout].Columns) != 0)
		rows++;
	rowcounter = 0;
	if (cols == 1)
	{
		pageView->setColumnCount(cols);
		pageView->setRowCount(rows*2+1);
		colmult = 1;
		coladd = 0;
		rowmult = 2;
		rowadd = 1;
	}
	else
	{
		pageView->setColumnCount(cols*2);
		pageView->setRowCount(rows+1);
		colmult = 2;
		coladd = 1;
		rowmult = 1;
		rowadd = 0;
	}
	for (int rr = 0; rr < pageView->rowCount(); rr++)
	{
		for (int cc = 0; cc < pageView->columnCount(); cc++)
		{
			QTableWidgetItem *tW = new QTableWidgetItem(1001);
			tW->setFlags(Qt::ItemIsEnabled);
			pageView->setItem(rr, cc, tW);
		}
	}
	pageView->coladd = coladd;
	pageView->colmult = colmult;
	pageView->rowadd = rowadd;
	pageView->rowmult = rowmult;
	pageView->firstP = counter;
	pageView->cols = currView->Doc->pageSets[currView->Doc->currentPageLayout].Columns;
	pageList.clear();
	for (int a = 0; a < currView->Doc->Pages->count(); ++a)
	{
		str = currView->Doc->Pages->at(a)->MPageNam;
		SeItem *it = new SeItem(str, a, CreateIcon(a, pix));
		pageList.append(it);
		pageView->setItem(rowcounter*rowmult+rowadd, counter*colmult+coladd, (QTableWidgetItem *)it);
		pageView->setColumnWidth(counter*colmult+coladd, pix.width());
		if (cols == 1)
		{
			pageView->setRowHeight(rowcounter*rowmult, 10);
			pageView->setRowHeight(rowcounter*rowmult+rowadd, pix.height());
		}
		else
			pageView->setRowHeight(rowcounter*rowmult+rowadd, pix.height()+5);
		counter++;
		if (counter > currView->Doc->pageSets[currView->Doc->currentPageLayout].Columns-1)
		{
			counter = 0;
			rowcounter++;
		}
	}
	pageView->setRowHeight(pageView->rowCount()-1, 10);
	counter = 0;
	if (cols != 1)
	{
		for (int c = 0; c < pageView->columnCount(); ++c)
		{
			if ((counter % 2) == 0)
				pageView->setColumnWidth(counter, 10);
			else
				pageView->setColumnWidth(counter, pix.width());
			counter++;
		}
	}
	pageView->repaint();
	if (currView != 0)
		markPage(currView->Doc->currentPageNumber());
	connect(pageLayout, SIGNAL(selectedLayout(int )), this, SLOT(handlePageLayout(int )));
	connect(pageLayout, SIGNAL(selectedFirstPage(int )), this, SLOT(handleFirstPage(int )));
}

void PagePalette::Rebuild()
{
	rebuildMasters();
	rebuildPages();
	enablePalette(currView != 0);
}

void PagePalette::markPage(uint nr)
{
	if (currView != 0)
	{
		SeItem *it;
		for (int a = 0; a < pageList.count(); a++)
		{
			it = pageList.at(a);
			if (it->pageNumber == nr)
			{
				pageView->clearSelection();
				pageView->item(it->row(), it->column())->setSelected(true);
				pageView->scrollToItem(pageView->item(it->row(), it->column()), QAbstractItemView::EnsureVisible);
				break;
			}
		}
	}
}

void PagePalette::setView(ScribusView *view)
{
	if (m_scMW->ScriptRunning)
		return;
	currView = view;
}

void PagePalette::selMasterPage()
{
	if (masterPageList->CurItem != 0)
		emit EditTemp(masterPageList->CurItem->text());
}

QPixmap PagePalette::CreateIcon(int nr, QPixmap pixin)
{
	QPainter p;
	// Necessary on windows to ensure the pixmap is drawable
	QPixmap ret(pixin.width(), pixin.height()); // Qt4, pixin.depth());
	if (p.begin(&ret))
	{
		p.drawPixmap( 0, 0, pixin );
//		if( !pixin.mask().isNull() )
//			ret.setMask( pixin.mask() );
		p.setBrush(Qt::white);
		p.setBackground(Qt::white);
		p.setBackgroundMode(Qt::OpaqueMode);
		p.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		p.setFont(QFont("Helvetica", 12, QFont::Bold));
		//QString tmp = tmp.setNum(nr+1);
		QString tmp(currView->Doc->getSectionPageNumberForPageIndex(nr));
		if (tmp.isEmpty())
			tmp = tmp.setNum(nr+1);
		QRect b = p.fontMetrics().boundingRect(tmp);
		QRect c = QRect((ret.width() / 2 - b.width() / 2)-2, (ret.height() / 2 - b.height() / 2)-2, b.width()+4, b.height()+4);
		p.drawRect(c);
		QRect d = QRect((ret.width() / 2 - b.width() / 2), (ret.height() / 2 - b.height() / 2), b.width(), b.height());
		p.setFont(QFont("Helvetica", 10, QFont::Normal));
		p.drawText(d, Qt::AlignCenter, tmp);
		p.end();
		if( !pixin.mask().isNull() )
			ret.setMask( pixin.mask() );
	}
	return ret;
}

void PagePalette::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void PagePalette::languageChange()
{
	setWindowTitle( tr( "Arrange Pages" ) );
	TextLabel1->setText( tr( "Available Master Pages:" ) );
	TextLabel2->setText( tr( "Document Pages:" ) );
	
	masterPageList->setToolTip( "<qt>" + tr( "List of master pages in the document. Master page names may be dragged onto the page view below to apply master pages, or ont empty space to create new pages." ) + "</qt>");
	pageView->setToolTip( "<qt>" + tr( "List of normal pages in the document, shown with the document layout. Pages may be dragged to rearrange or delete them.")  + "</qt>");
	Trash->setToolTip( "<qt>" + tr("Drag pages or master pages onto the trash to delete them") + "</qt>");
}

//CB Whats this variable returned for.. its always true... ?
//CB Clean up the interface and stick it behind this member for now
const bool PagePalette::getNamen()
{
	return pageView->Namen;
}

//CB Whats this variable returned for.. its always true... ?
//CB Clean up the interface and stick it behind this member for now
const bool PagePalette::getThumb()
{
	return masterPageList->Thumb;
}

