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
#include <QMessageBox>
#include <QMimeData>
#include <QPainter>
#include <QRegularExpression>

#include "commonstrings.h"
#include "iconmanager.h"
#include "pagelayout.h"
#include "pagepalette_pages.h"
#include "pagepalette_widgets.h"
#include "qobjectdefs.h"
#include "scpage.h"
#include "scribusapp.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"

PagePalette_Pages::PagePalette_Pages(QWidget *parent)
	: QWidget(parent)
{
	m_scMW = ScCore->primaryMainWindow();

	setupUi(this);

	setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding));

	masterPageList->setMinimumSize(QSize(200, 40));
	masterPageList->m_thumb = true;
	masterPageList->setIconSize(QSize(60, 60));

	trash->setMinimumSize(QSize(32, 32));
	trash->setMaximumSize(QSize(32, 32));

	PageGrid *pageGrid = pageViewWidget->pageGrid();

	rebuild();
	iconSetChange();
	languageChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));

	connect(masterPageList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(selMasterPage()));
	connect(masterPageList, SIGNAL(thumbnailChanged()), this, SLOT(rebuildMasters()));
	connect(masterPageList, SIGNAL(delMasterRequest(QString)), this, SLOT(deleteMasterPage(QString)));

	connect(pageLayout, SIGNAL(selectedLayout(int)), this, SLOT(handlePageLayout(int)));
	connect(pageLayout, SIGNAL(selectedFirstPage(int)), this, SLOT(handleFirstPage(int)));

	connect(pageGrid, SIGNAL(click(int, int)), this, SLOT(pageView_gotoPage(int, int)));
	connect(pageGrid, SIGNAL(movePage(int, int)), this, SLOT(pageView_movePage(int, int)));
	connect(pageGrid, SIGNAL(delPageRequest(int)), this, SLOT(pageView_deletePage(int)));
	connect(pageGrid, SIGNAL(useTemplate(QString, int)), this, SLOT(pageView_applyMasterPage(QString, int)));
	connect(pageGrid, SIGNAL(newPage(int, QString)), m_scMW, SLOT(slotNewPageP(int, QString)));
	connect(pageGrid, SIGNAL(previewSizeChanged()), this, SLOT(updatePagePreview()));

	connect(trash, SIGNAL(delPageRequest(int)), m_scMW, SLOT(deletePage2(int)));
	connect(trash, SIGNAL(delMasterRequest(QString)), this, SLOT(deleteMasterPage(QString)));

	connect( buttonPageInsert, SIGNAL(clicked()), this, SLOT(newPage()) );
	connect( buttonPageImport, SIGNAL(clicked()), this, SLOT(importPage()) );
	connect( buttonPageDuplicate, SIGNAL(clicked()), this, SLOT(duplicatePage()) );
	connect( buttonPageMove, SIGNAL(clicked()), this, SLOT(movePage()) );

	connect(this, SIGNAL(gotoPage(int)), m_scMW, SLOT(selectPagesFromOutlines(int)));
}

void PagePalette_Pages::deleteMasterPage(const QString &tmp)
{
	if (tmp == CommonStrings::trMasterPageNormal)
		return;
	QString extraWarn;
	for (int i = 0; i < currView->m_doc->DocPages.count(); ++i)
	{
		if (currView->m_doc->DocPages[i]->masterPageName() == tmp)
			extraWarn = tr("This master page is used at least once in the document.");
	}
	int exit = ScMessageBox::warning(this,
									 CommonStrings::trWarning,
									 tr("Do you really want to delete this master page?") + "\n"
									 + extraWarn,
									 QMessageBox::Yes | QMessageBox::No,
									 QMessageBox::NoButton, // GUI default
									 QMessageBox::Yes);     // batch default
	if (exit == QMessageBox::Yes)
	{
		bool oldMPMode = currView->m_doc->masterPageMode();
		int storedPageNum = currView->m_doc->currentPageNumber();
		int storedViewXCoor = currView->horizontalScrollBar()->value();
		int storedViewYCoor = currView->verticalScrollBar()->value();

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

void PagePalette_Pages::pageView_applyMasterPage(const QString &masterpageName, int pageIndex)
{
	m_scMW->Apply_MasterPage(masterpageName, pageIndex, false);
	currView->reformPages();
	currView->DrawNew();

	if (pageIndex < 0 && pageIndex >= currView->m_doc->DocPages.count())
		return;

	ScPage page = *currView->m_doc->DocPages.at(pageIndex);
	double pageRatio = page.width() / page.height();

	PageCell *pc = pageViewWidget->pageGrid()->getPageItem(pageIndex);
	pc->setPageName(masterpageName);
	pc->setPageRatio(pageRatio);

}

void PagePalette_Pages::pageView_movePage(int r, int c)
{
	if ((r == c) || (r >= pageViewWidget->pageGrid()->pageCount()))
		return;
	if (c >= pageViewWidget->pageGrid()->pageCount())
		currView->m_doc->movePage(r, r + 1, c, 2);
	else
		currView->m_doc->movePage(r, r + 1, c, 0);

	currView->reformPages();
	rebuildPages();
	currView->DrawNew();
}

void PagePalette_Pages::pageView_gotoPage(int pageID, int b)
{
	if (b == Qt::LeftButton)
	{
		emit gotoPage(pageID);
	}
}

void PagePalette_Pages::pageView_deletePage(int pageIndex)
{
	int exit = ScMessageBox::question(this,
									  tr("Delete Page?"),
									  "<qt>" + tr("Are you sure you want to delete this page?")
									  + "</qt>",
									  QMessageBox::Yes | QMessageBox::No,
									  QMessageBox::Yes);
	if (exit != QMessageBox::Yes)
		return;
	m_scMW->deletePage2(pageIndex);
}

void PagePalette_Pages::pageView_updatePagePreview()
{
	if (currView == nullptr)
	{
		m_pagePreviewUpdatePending = true;
		return;
	}

	if (currView->m_doc == nullptr)
	{
		m_pagePreviewUpdatePending = true;
		return;
	}

//	qDebug() << Q_FUNC_INFO << "- start page preview update";

	PageToPixmapFlags flags = Pixmap_DrawFrame | Pixmap_DrawBackground | Pixmap_DontReloadImages | Pixmap_NoCanvasModeChange;
	QMap<int, QImage> previews = currView->PagesToPixmap(pageViewWidget->pageGrid()->pageHeight(), -1, flags);

	for (int i = 0; i < currView->m_doc->DocPages.count(); ++i)
	{
		if (previews.contains(i) && i < pageViewWidget->pageGrid()->pageList.count())
		{
			ScPage page = *currView->m_doc->DocPages.at(i);
			double pageRatio = page.width() / page.height();
			QPixmap pix = ( previews.contains(i) ) ? QPixmap::fromImage( previews.value(i) ) : QPixmap();

			PageCell *pc = pageViewWidget->pageGrid()->pageList.at(i);
			pc->setPagePreview( pix );
			pc->setPageRatio( pageRatio );
		}
	}

	pageViewWidget->pageGrid()->update();

	m_pagePreviewUpdatePending = true;

}

void PagePalette_Pages::updatePagePreview()
{
	if (currView == nullptr || pageViewWidget->pageGrid()->pageList.empty())
	{
		m_pagePreviewUpdatePending = true;
		return;
	}

	if (currView->m_doc == nullptr)
	{
		m_pagePreviewUpdatePending = true;
		return;
	}

	if (m_pagePreviewUpdatePending &&
		(currView->updatesEnabled()) &&
		(currView->m_doc->DocPages.count() == pageViewWidget->pageGrid()->pageList.count()))
	{
		m_pagePreviewUpdatePending = false;

		// limit preview update to 1 refresh event each 500ms
		QTimer::singleShot(500, this, [this]()
		{
			pageView_updatePagePreview();
		});
	}
}


void PagePalette_Pages::newPage()
{
	m_scMW->slotNewPageM();
}

void PagePalette_Pages::duplicatePage()
{
	m_scMW->copyPage();
}

void PagePalette_Pages::importPage()
{
	m_scMW->slotPageImport();
}

void PagePalette_Pages::movePage()
{
	m_scMW->movePage();
}

void PagePalette_Pages::deletePage()
{
	int selectedPage = currView->m_doc->currentPageNumber();
	pageView_deletePage(selectedPage);
}

void PagePalette_Pages::enablePalette(const bool enabled)
{
	pageViewWidget->setEnabled(enabled);
	masterPageList->setEnabled(enabled);
	pageLayout->setEnabled(enabled);
	buttonPageDuplicate->setEnabled(enabled);
	buttonPageImport->setEnabled(enabled);
	buttonPageInsert->setEnabled(enabled);
	buttonPageMove->setEnabled(enabled);
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
	QListWidgetItem *item;
	for (auto it = currView->m_doc->MasterNames.cbegin(); it != currView->m_doc->MasterNames.cend(); ++it)
	{
		const QString &pageName = it.key();
		QString pageLabel = (pageName == CommonStrings::masterPageNormal)
				? CommonStrings::trMasterPageNormal
				: pageName;
		if (masterPageList->m_thumb)
		{
			pm = QPixmap::fromImage(currView->MPageToPixmap(pageName, 60));
			item = new QListWidgetItem(QIcon(pm), pageLabel, masterPageList);
		} else
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

	pageViewWidget->pageGrid()->clear();
	if (currView == nullptr)
	{
		connect(pageLayout, SIGNAL(selectedLayout(int)), this, SLOT(handlePageLayout(int)));
		connect(pageLayout, SIGNAL(selectedFirstPage(int)), this, SLOT(handleFirstPage(int)));
		return;
	}

	const PageSet &currentPageSet = currView->m_doc->pageSets()[currView->m_doc->pagePositioning()];
	pageLayout->updateLayoutSelector(currView->m_doc);
	pageLayout->selectItem(currView->m_doc->pagePositioning());
	pageLayout->firstPage->setCurrentIndex(currentPageSet.FirstPage);

	int counter = currentPageSet.FirstPage;
	int cols = currentPageSet.Columns;

	pageViewWidget->pageGrid()->setDocumentPageSize(QSize(currView->m_doc->pageWidth(), currView->m_doc->pageHeight()));
	pageViewWidget->pageGrid()->setPageInGroup(cols);
	pageViewWidget->pageGrid()->setPageOffset(counter);

	m_pagePreviewUpdatePending = false;

//	QElapsedTimer timer;
//	timer.start();

	// Create all page previews
	PageToPixmapFlags flags = Pixmap_DrawFrame | Pixmap_DrawBackground | Pixmap_DontReloadImages | Pixmap_NoCanvasModeChange;
	QMap<int, QImage> previews = currView->PagesToPixmap(pageViewWidget->pageGrid()->pageHeight(), -1, flags);

	for (int i = 0; i < currView->m_doc->DocPages.count(); ++i)
	{
		ScPage page = *currView->m_doc->DocPages.at(i);

		QString str = page.masterPageName();
		double pageRatio = page.width() / page.height();
		QPixmap pix = (previews.contains(i)) ? QPixmap::fromImage(previews.value(i)) : QPixmap();

		// empty page
		PageCell *pc = new PageCell(str, i,
									pix,// QPixmap can replace with real page preview
									pageRatio);
		pageViewWidget->pageGrid()->pageList.append(pc);
	}

//	qDebug() << Q_FUNC_INFO << "- Pages rebuilt in" << timer.elapsed() << "milliseconds";

	pageViewWidget->pageGrid()->calculateSize();
	pageViewWidget->pageGrid()->update();

	m_pagePreviewUpdatePending = true;

	if (currView != nullptr)
	{
		markPage(currView->m_doc->currentPageNumber());
	}
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

	if (pageViewWidget->pageGrid()->pageCount() > 0)
	{
		pageViewWidget->pageGrid()->setSelectedPage(nr);
		pageViewWidget->scrollToPage(nr);
	}
}

void PagePalette_Pages::setView(ScribusView *view)
{
	if (m_scMW->scriptIsRunning())
		return;

	currView = view;

//	if (currView)
//		connect(currView->m_doc, SIGNAL(pagePreviewChanged()), this, SLOT(updatePagePreview()));

}

void PagePalette_Pages::selMasterPage()
{
	if (masterPageList->m_currItem == nullptr)
		return;
	QVariant pageVar = masterPageList->m_currItem->data(Qt::UserRole);
	emit gotoMasterPage(pageVar.toString());
}

//QPixmap PagePalette_Pages::createPagePreview(const QPixmap &pixin, QSize size)
//{
//	QRect pageRect(0, 0, size.width(), size.height());
//	QPixmap tile(size);

//	QPainter painter;
//	painter.begin(&tile);
//	painter.fillRect(pageRect, Qt::white);
//	painter.drawPixmap(pageRect, pixin);
//	painter.end();

//	return tile;
//}

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
	IconManager& iconManager = IconManager::instance();

	// Pages
	buttonPageImport->setIcon(iconManager.loadIcon("page-import"));
	buttonPageInsert->setIcon(iconManager.loadIcon("page-insert"));
	buttonPageDuplicate->setIcon(iconManager.loadIcon("page-duplicate"));
	buttonPageMove->setIcon(iconManager.loadIcon("page-move"));
}

void PagePalette_Pages::languageChange()
{
	// Pages
	buttonPageDuplicate->setToolTip( tr( "Duplicate the selected page" ) );
	buttonPageMove->setToolTip( tr( "Move the selected page" ) );
	buttonPageInsert->setToolTip( tr( "Add a new page" ) );
	buttonPageImport->setToolTip( tr( "Import pages from another document" ) );

	retranslateUi(this);
}
