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
#include "pagepalette_pages.h"
#include "pagepalette_widgets.h"
#include "sccombobox.h"
#include "scpage.h"
#include "scribus.h"
#include "scribuscore.h"
#include "scribusview.h"
#include "util_icon.h"

PagePalette_Pages::PagePalette_Pages(QWidget* parent) : QWidget(parent)
{
	m_scMW = ScCore->primaryMainWindow();
	
	setupUi(this);
	
	setSizePolicy( QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

	masterPageList->setMinimumSize(QSize(130,70));
	masterPageList->Thumb = false;
	masterPageList->setIconSize(QSize(60, 60));

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

	trash->setMinimumSize(QSize(32,32));
	trash->setMaximumSize(QSize(32,32));

	pix = loadIcon("32/page-simple.png");

	currView = 0;
	Rebuild();
	languageChange();

	connect(masterPageList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(selMasterPage()));
	connect(masterPageList, SIGNAL(thumbnailChanged()), this, SLOT(rebuildMasters()));
	connect(masterPageList, SIGNAL(DelMaster(QString)), this, SLOT(deleteMasterPage(QString)));

	connect(pageLayout, SIGNAL(selectedLayout(int ))   , this, SLOT(handlePageLayout(int )));
	connect(pageLayout, SIGNAL(selectedFirstPage(int )), this, SLOT(handleFirstPage(int )));
	connect(pageView  , SIGNAL(Click(int, int, int))   , this, SLOT(pageView_gotoPage(int, int, int)));
	connect(pageView  , SIGNAL(movePage(int, int))     , this, SLOT(pageView_movePage(int, int)));
	connect(pageView  , SIGNAL(DelPage(int))           , m_scMW, SLOT(deletePage2(int)));
	connect(pageView  , SIGNAL(UseTemp(QString, int))  , m_scMW, SLOT(Apply_MasterPage(QString, int)));
	connect(pageView  , SIGNAL(NewPage(int, QString))  , m_scMW, SLOT(slotNewPageP(int, QString)));
	connect(trash     , SIGNAL(DelPage(int))           , m_scMW, SLOT(deletePage2(int)));
	connect(trash     , SIGNAL(DelMaster(QString))     , this, SLOT(deleteMasterPage(QString)));
	
	connect(this, SIGNAL(gotoPage(int))          , m_scMW, SLOT(selectPagesFromOutlines(int)));
}

void PagePalette_Pages::deleteMasterPage(QString tmp)
{
	if (tmp == CommonStrings::trMasterPageNormal)
		return;
	QString extraWarn = "";
	for (int i=0; i < currView->Doc->DocPages.count(); ++i )
	{
		if (currView->Doc->DocPages[i]->MPageNam == tmp)
			extraWarn = tr("This master page is used at least once in the document.");
	}
	int exit = QMessageBox::warning(this,
	                              CommonStrings::trWarning,
	                              tr("Do you really want to delete this master page?")+"\n"+extraWarn,
	                              QMessageBox::Yes | QMessageBox::No);
	if (exit == QMessageBox::Yes)
	{
		bool oldMPMode = currView->Doc->masterPageMode();
		int  storedPageNum = currView->Doc->currentPageNumber();
		int  storedViewXCoor = currView->horizontalScrollBar()->value();
		int  storedViewYCoor = currView->verticalScrollBar()->value();

		currView->Doc->setMasterPageMode(true);
		currView->Doc->scMW()->deletePage2(currView->Doc->MasterNames[tmp]);
		//<<CB TODO Move back into ScribusDoc::deleteMasterPage();
		//This must happen after the pages have been reformed (view/doc)
		currView->Doc->rebuildMasterNames();
		// Fix up any pages that refer to the deleted master page
		currView->Doc->replaceMasterPage(tmp);
		currView->Doc->setMasterPageMode(oldMPMode);
		currView->Doc->setModified(true);

		currView->Doc->setCurrentPage(currView->Doc->DocPages.at(storedPageNum));
		currView->reformPages(false);
		currView->setContentsPos(storedViewXCoor, storedViewYCoor);
		currView->DrawNew();
	}
}

void PagePalette_Pages::pageView_movePage(int r, int c)
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

void PagePalette_Pages::pageView_gotoPage(int r, int c, int b)
{
	int p;
	bool dummy;
	if ((b == Qt::LeftButton) && (r != -1) && (c != -1))
	{
		p = pageView->GetPage(r, c, &dummy);
		emit gotoPage(p);
	}
}

void PagePalette_Pages::enablePalette(const bool enabled)
{
	pageView->setEnabled(enabled);
	masterPageList->setEnabled(enabled);
	pageLayout->setEnabled(enabled);
}

void PagePalette_Pages::handlePageLayout(int layout)
{
	pageLayout->selectFirstP(currView->Doc->pageSets()[layout].FirstPage);
	currView->Doc->resetPage(layout);
	currView->reformPages();
	currView->DrawNew();
	currView->GotoPage(currView->Doc->currentPageNumber());
	rebuildPages();
//CB done by doc::reformpages
//	currView->Doc->setModified(true);
}

void PagePalette_Pages::handleFirstPage(int fp)
{
//	currView->Doc->pageSets()[currView->Doc->currentPageLayout].FirstPage = fp;
	currView->Doc->setPageSetFirstPage(currView->Doc->pagePositioning(), fp);
	currView->reformPages();
	currView->DrawNew();
	currView->GotoPage(currView->Doc->currentPageNumber());
	rebuildPages();
//CB done by doc::reformpages
// 	currView->Doc->setModified(true);
}

void PagePalette_Pages::rebuildMasters()
{
	if (m_scMW->scriptIsRunning())
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

void PagePalette_Pages::rebuildPages()
{
	if (m_scMW->scriptIsRunning())
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
	pageLayout->updateLayoutSelector(currView->Doc->pageSets());
	pageLayout->selectItem(currView->Doc->pagePositioning());
	pageLayout->firstPage->setCurrentIndex(currView->Doc->pageSets()[currView->Doc->pagePositioning()].FirstPage);
	pageView->MaxC = currView->Doc->DocPages.count()-1;
	int counter = currView->Doc->pageSets()[currView->Doc->pagePositioning()].FirstPage;
	int cols = currView->Doc->pageSets()[currView->Doc->pagePositioning()].Columns;
	int rows = (currView->Doc->DocPages.count()+counter) / currView->Doc->pageSets()[currView->Doc->pagePositioning()].Columns;
	if (((currView->Doc->DocPages.count()+counter) % currView->Doc->pageSets()[currView->Doc->pagePositioning()].Columns) != 0)
		rows++;
	int rowcounter = 0;
	int colmult, rowmult, coladd, rowadd;
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
	pageView->cols = currView->Doc->pageSets()[currView->Doc->pagePositioning()].Columns;
	pageList.clear();
	for (int a = 0; a < currView->Doc->DocPages.count(); ++a)
	{
		str = currView->Doc->DocPages.at(a)->MPageNam;
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
		if (counter > currView->Doc->pageSets()[currView->Doc->pagePositioning()].Columns-1)
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

void PagePalette_Pages::Rebuild()
{
	rebuildMasters();
	rebuildPages();
	enablePalette(currView != 0);
}

void PagePalette_Pages::markPage(uint nr)
{
	if (currView == 0)
		return;

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

void PagePalette_Pages::setView(ScribusView *view)
{
	if (m_scMW->scriptIsRunning())
		return;
	currView = view;
}

void PagePalette_Pages::selMasterPage()
{
	if (masterPageList->CurItem != 0)
		emit gotoMasterPage(masterPageList->CurItem->text());
}

QPixmap PagePalette_Pages::CreateIcon(int nr, QPixmap pixin)
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

void PagePalette_Pages::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	
	QWidget::changeEvent(e);
}

void PagePalette_Pages::languageChange()
{
	setWindowTitle( tr( "Arrange Pages" ) );
	textLabel1->setText( tr( "Available Master Pages:" ) );
	textLabel2->setText( tr( "Document Pages:" ) );
	
	masterPageList->setToolTip( "<qt>" + tr( "List of master pages in the document. Master page names may be dragged onto the page view below to apply master pages, or onto the empty space between pages to create new pages." ) + "</qt>");
	pageView->setToolTip( "<qt>" + tr( "List of normal pages in the document, shown with the document layout. Pages may be dragged to rearrange or delete them.")  + "</qt>");
	trash->setToolTip( "<qt>" + tr("Drag pages or master pages onto the trash to delete them") + "</qt>");
}
