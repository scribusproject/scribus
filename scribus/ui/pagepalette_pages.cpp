/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/


#include <QApplication>
#include <QBitmap>
#include <QComboBox>
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
#include "pagepalette_pages.h"
#include "pagepalette_widgets.h"
#include "scpage.h"
#include "scribusapp.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"

PagePalette_Pages::PagePalette_Pages(QWidget* parent) : QWidget(parent)
{
	currView = nullptr;
	m_scMW = ScCore->primaryMainWindow();
	
	setupUi(this);
	
	setSizePolicy( QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding));

	masterPageList->setMinimumSize(QSize(200, 40));
	masterPageList->m_thumb = false;
	masterPageList->setIconSize(QSize(60, 60));

	QHeaderView *Header = pageView->verticalHeader();
	Header->setSectionsMovable(false);
	Header->setSectionResizeMode(QHeaderView::Fixed);
	Header->hide();
	Header = pageView->horizontalHeader();
	Header->setSectionsMovable(false);
	Header->setSectionResizeMode(QHeaderView::Fixed);
	Header->hide();
	pageView->setSortingEnabled(false);
	pageView->setSelectionMode( QAbstractItemView::NoSelection );
	pageView->setRowCount(1);
	pageView->setColumnCount(1);
	pageView->setIconSize(QSize(60, 60));

	trash->setMinimumSize(QSize(32,32));
	trash->setMaximumSize(QSize(32,32));

	pix = IconManager::instance().loadPixmap("32/page-simple.png");

	rebuild();
	languageChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));

	connect(masterPageList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(selMasterPage()));
	connect(masterPageList, SIGNAL(thumbnailChanged()), this, SLOT(rebuildMasters()));
	connect(masterPageList, SIGNAL(delMasterRequest(QString)), this, SLOT(deleteMasterPage(QString)));

	connect(pageLayout, SIGNAL(selectedLayout(int))   , this, SLOT(handlePageLayout(int)));
	connect(pageLayout, SIGNAL(selectedFirstPage(int)), this, SLOT(handleFirstPage(int)));
	connect(pageView  , SIGNAL(Click(int,int,int))    , this, SLOT(pageView_gotoPage(int,int,int)));
	connect(pageView  , SIGNAL(movePage(int,int))     , this, SLOT(pageView_movePage(int,int)));
	connect(pageView  , SIGNAL(delPageRequest(int))   , this, SLOT(pageView_deletePage(int)));
	connect(pageView  , SIGNAL(UseTemp(QString,int))  , this, SLOT(pageView_applyMasterPage(QString,int)));
	connect(pageView  , SIGNAL(NewPage(int,QString))  , m_scMW, SLOT(slotNewPageP(int,QString)));
	connect(trash     , SIGNAL(delPageRequest(int))   , m_scMW, SLOT(deletePage2(int)));
	connect(trash     , SIGNAL(delMasterRequest(QString))    , this, SLOT(deleteMasterPage(QString)));
	
	connect(this, SIGNAL(gotoPage(int))          , m_scMW, SLOT(selectPagesFromOutlines(int)));
}

void PagePalette_Pages::deleteMasterPage(const QString& tmp)
{
	if (tmp == CommonStrings::trMasterPageNormal)
		return;
	QString extraWarn = "";
	for (int i=0; i < currView->m_doc->DocPages.count(); ++i )
	{
		if (currView->m_doc->DocPages[i]->masterPageName() == tmp)
			extraWarn = tr("This master page is used at least once in the document.");
	}
	int exit = ScMessageBox::warning(this,
									  CommonStrings::trWarning,
									  tr("Do you really want to delete this master page?")+"\n"+extraWarn,
									  QMessageBox::Yes | QMessageBox::No,
									  QMessageBox::NoButton,	// GUI default
									  QMessageBox::Yes);	// batch default
	if (exit == QMessageBox::Yes)
	{
		bool oldMPMode = currView->m_doc->masterPageMode();
		int  storedPageNum = currView->m_doc->currentPageNumber();
		int  storedViewXCoor = currView->horizontalScrollBar()->value();
		int  storedViewYCoor = currView->verticalScrollBar()->value();

		currView->m_doc->setMasterPageMode(true);
		currView->m_doc->scMW()->deletePage2(currView->m_doc->MasterNames[tmp]);
		//<<CB TODO Move back into ScribusDoc::deleteMasterPage();
		//This must happen after the pages have been reformed (view/doc)
		currView->m_doc->rebuildMasterNames();
		// Fix up any pages that refer to the deleted master page
		currView->m_doc->replaceMasterPage(tmp);
		currView->m_doc->setMasterPageMode(oldMPMode);
		currView->m_doc->setModified(true);

		currView->m_doc->setCurrentPage(currView->m_doc->DocPages.at(storedPageNum));
		currView->reformPages(false);
		currView->setContentsPos(storedViewXCoor, storedViewYCoor);
		currView->DrawNew();
	}
}

void PagePalette_Pages::pageView_applyMasterPage(const QString& masterpageName, int pageIndex)
{
	m_scMW->Apply_MasterPage(masterpageName, pageIndex, false);
	currView->reformPages();
	currView->DrawNew();
	SeItem* pageItem = pageView->getPageItem(pageIndex);
	if (pageItem)
		pageItem->setIcon(createIcon(pageIndex, masterpageName, pix));
}

void PagePalette_Pages::pageView_movePage(int r, int c)
{
	if ((r == c) || (r >= pageView->m_pageCount))
		return;
	if (c >= pageView->m_pageCount)
		currView->m_doc->movePage(r, r + 1, c, 2);
	else
		currView->m_doc->movePage(r, r + 1, c, 0);
	currView->reformPages();
	rebuildPages();
	currView->DrawNew();
}

void PagePalette_Pages::pageView_gotoPage(int r, int c, int b)
{
	if ((b == Qt::LeftButton) && (r != -1) && (c != -1))
	{
		int p;
		bool dummy;
		p = pageView->getPage(r, c, &dummy);
		emit gotoPage(p);
	}
}

void PagePalette_Pages::pageView_deletePage(int pageIndex)
{
	int exit = ScMessageBox::question(this, tr("Delete Page?"),
			                          "<qt>" + tr("Are you sure you want to delete this page?") + "</qt>",
			                          QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	if (exit != QMessageBox::Yes)
		return;
	m_scMW->deletePage2(pageIndex);
}

void PagePalette_Pages::enablePalette(const bool enabled)
{
	pageView->setEnabled(enabled);
	masterPageList->setEnabled(enabled);
	pageLayout->setEnabled(enabled);
}

void PagePalette_Pages::handlePageLayout(int layout)
{
	pageLayout->selectFirstP(currView->m_doc->pageSets()[layout].FirstPage);
	currView->m_doc->setPagePositioning(layout);
	currView->reformPages();
	currView->DrawNew();
	currView->GotoPage(currView->m_doc->currentPageNumber());
	rebuildPages();
}

void PagePalette_Pages::handleFirstPage(int fp)
{
	currView->m_doc->setPageSetFirstPage(currView->m_doc->pagePositioning(), fp);
	currView->reformPages();
	currView->DrawNew();
	currView->GotoPage(currView->m_doc->currentPageNumber());
	rebuildPages();
}

void PagePalette_Pages::rebuildMasters()
{
	if (m_scMW->scriptIsRunning())
		return;

	masterPageList->clear();
	if (currView == nullptr)
		return;

	QPixmap pm;
	QListWidgetItem* item;
	for (auto it = currView->m_doc->MasterNames.cbegin(); it != currView->m_doc->MasterNames.cend(); ++it)
	{
		const QString& pageName = it.key();
		QString pageLabel = (pageName == CommonStrings::masterPageNormal) ? CommonStrings::trMasterPageNormal : pageName;
		if (masterPageList->m_thumb)
		{
			pm = QPixmap::fromImage(currView->MPageToPixmap(pageName, 60));
			item = new QListWidgetItem(QIcon(pm), pageLabel, masterPageList);
		}
		else
			item = new QListWidgetItem(pageLabel, masterPageList);
		item->setData(Qt::UserRole, pageName);
	}
}

void PagePalette_Pages::rebuildPages()
{
	if (m_scMW->scriptIsRunning())
		return;

	disconnect(pageLayout, SIGNAL(selectedLayout(int)), this, SLOT(handlePageLayout(int)));
	disconnect(pageLayout, SIGNAL(selectedFirstPage(int)), this, SLOT(handleFirstPage(int)));

	pageView->clearContents();
	pageView->setRowCount(1);
	pageView->setColumnCount(1);
	if (currView == nullptr)
	{
		connect(pageLayout, SIGNAL(selectedLayout(int)), this, SLOT(handlePageLayout(int)));
		connect(pageLayout, SIGNAL(selectedFirstPage(int)), this, SLOT(handleFirstPage(int)));
		return;
	}

	const PageSet& currentPageSet = currView->m_doc->pageSets()[currView->m_doc->pagePositioning()];
	pageLayout->updateLayoutSelector(currView->m_doc);
	pageLayout->selectItem(currView->m_doc->pagePositioning());
	pageLayout->firstPage->setCurrentIndex(currentPageSet.FirstPage);
	pageView->m_pageCount = currView->m_doc->DocPages.count();
	
	int counter = currentPageSet.FirstPage;
	int cols = currentPageSet.Columns;
	int rows = (currView->m_doc->DocPages.count() + counter) / currentPageSet.Columns;
	if (((currView->m_doc->DocPages.count() + counter) % currentPageSet.Columns) != 0)
		rows++;
	int rowcounter = 0;
	int colmult, rowmult, coladd, rowadd;
	if (cols == 1)
	{
		pageView->setColumnCount(cols);
		pageView->setRowCount(rows * 2 + 1);
		colmult = 1;
		coladd = 0;
		rowmult = 2;
		rowadd = 1;
	}
	else
	{
		pageView->setColumnCount(cols * 2);
		pageView->setRowCount(rows + 1);
		colmult = 2;
		coladd = 1;
		rowmult = 1;
		rowadd = 0;
	}

	for (int row = 0; row < pageView->rowCount(); ++row)
	{
		for (int column = 0; column < pageView->columnCount(); ++column)
		{
			QTableWidgetItem *tW = new QTableWidgetItem(1001);
			tW->setFlags(Qt::ItemIsEnabled);
			pageView->setItem(row, column, tW);
		}
	}

	pageView->m_coladd = coladd;
	pageView->m_colmult = colmult;
	pageView->m_rowadd = rowadd;
	pageView->m_rowmult = rowmult;
	pageView->m_firstPage = counter;
	pageView->m_cols = currentPageSet.Columns;

	int columnWidth = pix.width() * pageView->devicePixelRatioF();
	int rowHeight = pix.height() * pageView->devicePixelRatioF();
	int rowHeight5 = 5 * pageView->devicePixelRatioF();
	int rowHeight10 = 10 * pageView->devicePixelRatioF();

	pageList.clear();
	for (int i = 0; i < currView->m_doc->DocPages.count(); ++i)
	{
		QString str = currView->m_doc->DocPages.at(i)->masterPageName();
		SeItem *it = new SeItem(str, i, createIcon(i, str, pix));
		pageList.append(it);
		pageView->setItem(rowcounter * rowmult + rowadd, counter * colmult + coladd, it);
		pageView->setColumnWidth(counter * colmult + coladd, columnWidth);
		if (cols == 1)
		{
			pageView->setRowHeight(rowcounter * rowmult, 10);
			pageView->setRowHeight(rowcounter * rowmult + rowadd, rowHeight);
		}
		else
			pageView->setRowHeight(rowcounter * rowmult + rowadd, rowHeight + rowHeight5);
		counter++;
		if (counter > currentPageSet.Columns - 1)
		{
			counter = 0;
			rowcounter++;
		}
	}
	pageView->setRowHeight(pageView->rowCount() - 1, rowHeight10);

	if (cols != 1)
	{
		for (int i = 0; i < pageView->columnCount(); ++i)
		{
			if ((i % 2) == 0)
				pageView->setColumnWidth(i, rowHeight10);
			else
				pageView->setColumnWidth(i, columnWidth);
		}
	}

	pageView->update();
	if (currView != nullptr)
		markPage(currView->m_doc->currentPageNumber());
	connect(pageLayout, SIGNAL(selectedLayout(int)), this, SLOT(handlePageLayout(int)));
	connect(pageLayout, SIGNAL(selectedFirstPage(int)), this, SLOT(handleFirstPage(int)));
}

void PagePalette_Pages::rebuild()
{
	rebuildMasters();
	rebuildPages();
	enablePalette(currView != nullptr);
}

void PagePalette_Pages::markPage(uint nr)
{
	if (currView == nullptr)
		return;

	SeItem *it;
	for (int i = 0; i < pageList.count(); i++)
	{
		it = pageList.at(i);
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
	if (masterPageList->m_currItem == nullptr)
		return;
	QVariant pageVar = masterPageList->m_currItem->data(Qt::UserRole);
	emit gotoMasterPage(pageVar.toString());
}

QPixmap PagePalette_Pages::createIcon(int number, QString masterPage, const QPixmap& pixin)
{
	// Necessary on windows to ensure the pixmap is drawable
	QPixmap ret(pixin.width() * devicePixelRatioF(), pixin.height() * devicePixelRatioF());
	ret.setDevicePixelRatio(devicePixelRatioF());

	QPainter p;
	if (!p.begin(&ret))
		return ret;

	p.drawPixmap(0, 0, pixin);
	p.setBrush(Qt::white);
	p.setBackground(Qt::white);
	p.setBackgroundMode(Qt::OpaqueMode);
	p.setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	QString tmp(currView->m_doc->getSectionPageNumberForPageIndex(number));
	if (tmp.isEmpty())
		tmp = tmp.setNum(number + 1);
	QRegExp regExp ("([A-Z]*[0-9]*)( *[\\.|\\-|_] *)(.*)");
	if (regExp.indexIn(masterPage) != -1)
		masterPage = regExp.cap(1);
	QRect rect(0, 0, pixin.width(), pixin.height());
	p.setRenderHint(QPainter::TextAntialiasing, true);
	p.setFont(QFont("Helvetica", 7, QFont::Normal));
	p.drawText(rect, Qt::AlignCenter, tmp + "\n" + masterPage);
	p.end();

	if (!pixin.mask().isNull())
		ret.setMask( pixin.mask() );
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

void PagePalette_Pages::iconSetChange()
{
	pix = IconManager::instance().loadPixmap("32/page-simple.png");
	if (currView != nullptr)
		rebuild();
}

void PagePalette_Pages::languageChange()
{
	retranslateUi(this);
}
