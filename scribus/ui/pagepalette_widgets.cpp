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
#include <QElapsedTimer>
#include <QEvent>
#include <QHeaderView>
#include <QLabel>
#include <QList>
#include <QMenu>
#include <QMimeData>
#include <QMessageBox>
#include <QPainter>
#include <QScrollArea>
#include <QStringView>

#include "iconmanager.h"
#include "pagepalette_widgets.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scribusapp.h"
#include "util_gui.h"

const QString MIMETYPE = "page/magic";

/* ********************************************************************************* *
 *
 *
 * Master Page List
 *
 *
 * ********************************************************************************* */
SeList::SeList(QWidget* parent) : QListWidget(parent)
{
	setAcceptDrops(true);

	// Read prefs
	PrefsContext *prefCollapse = PrefsManager::instance().prefsFile->getContext(m_prefsContext);
	m_thumb = prefCollapse->getBool(m_prefsName, m_thumb);
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

	// write prefs
	PrefsContext *prefCollapse = PrefsManager::instance().prefsFile->getContext(m_prefsContext);
	prefCollapse->set(m_prefsName, m_thumb);
	PrefsManager::instance().prefsFile->write();

	emit thumbnailChanged();
}

void SeList::mousePressEvent(QMouseEvent* e)
{
	e->accept();
	m_currItem = nullptr;
	QListWidgetItem *i = itemAt(e->position().toPoint());
	if (i)
	{
		m_currItem = i;
		m_mousePos = e->position().toPoint();
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
		mimeData->setData(MIMETYPE, "1" + pageName.toLocal8Bit());
		mimeData->setText("1" + pageName);
		QDrag *dr = new QDrag(this);
		dr->setMimeData(mimeData);
		const QPixmap& pm = item->icon().pixmap(60);
		dr->setPixmap(pm);
		dr->exec(Qt::CopyAction | Qt::MoveAction);
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


/* ********************************************************************************* *
 *
 *
 * Page Viewer
 *
 *
 * ********************************************************************************* */

PageViewer::PageViewer(QWidget *parent) : QWidget(parent)
{
	m_pageGrid = new PageGrid(this);

	m_scroll = new QScrollArea;
	m_scroll->setWidget(m_pageGrid);

	m_scroll->setWidgetResizable(true);
	m_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	//m_scroll->setFrameShape(QFrame::Box);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(m_scroll);
	layout->setContentsMargins(0,0,0,0);

	setLayout(layout);

}

PageGrid *PageViewer::pageGrid()
{
	return m_pageGrid;
}

void PageViewer::scrollToPage(int pageId)
{
	QPoint posOfSelection = pageGrid()->pagePosition(pageId);

	m_scroll->ensureVisible(
				posOfSelection.x(),
				posOfSelection.y(),
				pageGrid()->rowHeight(),
				pageGrid()->rowHeight()
				);

}

void PageViewer::keyPressEvent(QKeyEvent *event)
{
	int k = event->key();
	if (k == Qt::Key_Delete)
	{
		event->accept();

		m_pageGrid->deleteSelectedPage();
		return;
	}

	QWidget::keyPressEvent(event);
}



/* ********************************************************************************* *
 *
 *
 * Page Grid
 *
 *
 * *********************************************************************************
 *
 * Grid Scheme for Double Page
 *
 *        Column 1       Column 2              Column 3       Column 4
 *     ------------------------------       ------------------------------
 *  R  | Group                      |       | Group                      |
 *  o  | ----------      ---------- | Group | ----------      ---------- |
 *  w  | | Cell   | Cell | Cell   | | Space | | Cell   | Cell | Cell   | |
 *     | |        | Gap  |        | |       | |        | Gap  |        | |
 *  1  | ----------      ---------- |       | ----------      ---------- |
 *     ------------------------------       ------------------------------
 *                Row Space                             Row Space
 *     ------------------------------       ------------------------------
 *  R  | Group                      |       | Group                      |
 *  o  | ----------      ---------- | Group | ----------      ---------- |
 *  w  | | Cell   | Cell | Cell   | | Space | | Cell   | Cell | Cell   | |
 *     | |        | Gap  |        | |       | |        | Gap  |        | |
 *  2  | ----------      ---------- |       | ----------      ---------- |
 *     ------------------------------       ------------------------------
 *
 * A group can have minimum 1 cell or maximum 4 cells
 *
 * ********************************************************************************* */



PageGrid::PageGrid(QWidget *parent)
	: QWidget{parent}
{
	pageList = QList<PageCell*>();
	m_colorSelection = PrefsManager::instance().appPrefs.displayPrefs.pageBorderColor;
	m_documentPageSize = QSize(m_rowHeight, m_rowHeight);
	m_contextMenu = new QMenu(this);


	setContextMenuPolicy(Qt::CustomContextMenu);
	setAcceptDrops(true);
	calculateSize();
	initContextMenu();

	PrefsContext *prefTileSize = PrefsManager::instance().prefsFile->getContext("PagePalette");
	int size = prefTileSize->getInt("pagePreviewSize", m_rowHeight);
	setRowHeight(size);

//	connect(this, &PageGrid::customContextMenuRequested, this, &PageGrid::showContextMenu);

}

/* ********************************************************************************* *
 *
 * Public Members
 *
 * ********************************************************************************* */

void PageGrid::setPageInGroup(int amount)
{
	m_cellsInGroup = amount;
}

void PageGrid::setDocumentPageSize(QSizeF pageSize)
{
	m_documentPageSize = pageSize;
}

void PageGrid::setRowHeight(int height)
{
	// clamp sizes
	m_rowHeight = qBound(64, height, 128);
	m_rowSpace = qBound(8, height / 8, 16);

	//	updateTileSize();
	calculateSize();
	update();

	PrefsContext *prefTileSize = PrefsManager::instance().prefsFile->getContext("PagePalette");
	prefTileSize->set("pagePreviewSize", height);
	PrefsManager::instance().prefsFile->write();
}

void PageGrid::setFontSize(int size)
{
	m_fontSize = size;
	update();
}

void PageGrid::setSelectionColor(QColor color)
{
	m_colorSelection = color;
	update();
}

void PageGrid::setPageLayout(PageLayout layout)
{
	m_pageLayout = layout;

	switch(m_pageLayout)
	{
	default:
	case PageLayout::singlePage:
		m_cellsInGroup = 1;
		break;
	case PageLayout::doublePage:
		m_cellsInGroup = 2;
		break;
	case PageLayout::triplePage:
		m_cellsInGroup = 3;
		break;
	case PageLayout::quadroPage:
		m_cellsInGroup = 4;
		break;
	}

	update();
}

void PageGrid::setPageOffset(int pageCount)
{
	m_pageOffset = pageCount;
	calculateSize();
	update();
}

int PageGrid::pageId(int r, int c, bool clampId)
{

	if ( r == -1 || c == -1) return -1;

	int id = (r == 0) ? c : r * columns() + c;
	id -= m_pageOffset;

	// Important: The page id can be equal to pageCount to have an extra cell after the last one.
	return (clampId) ? clampPageId(id, true) : id;

}

int PageGrid::pageId(QPoint pos, bool clampId)
{
	int row = rowAt(pos);
	int col = columnAt(pos);

	return pageId(row, col, clampId);
}

PageCell *PageGrid::getPageItem(int pageIndex)
{
	if(pageCount() == 0 || pageIndex >= pageCount() || pageIndex < 0)
		return nullptr;

	return pageList.at( clampPageId(pageIndex) );

}

int PageGrid::pageCount()
{
	return pageList.count();
}

int PageGrid::pageHeight()
{
	return m_rowHeight - m_fontSize - m_labelGap;
}

void PageGrid::setSelectedPage(int pageID)
{
	m_selectedPage = clampPageId(pageID);
	update();
}

void PageGrid::deleteSelectedPage()
{
	emit delPageRequest( clampPageId(m_selectedPage) );
}

void PageGrid::clear()
{
	pageList.clear();
	clearUi();
	calculateSize();
}

/* ********************************************************************************* *
 *
 * Private Members
 *
 * ********************************************************************************* */

int PageGrid::columns()
{
	return m_cellsInGroup ;
}

int PageGrid::rows()
{
	// total page count + offset pages
	return qCeil( (double)(pageCount() + m_pageOffset + 1) / (double)columns() );
}

int PageGrid::columnAt(QPoint pos)
{
	if(pageCount() == 0) return -1;

	int row = rowAt(pos);
	if ( row == -1) return row;

	int m_col = -1;

	int cellId = row * m_cellsInGroup;
	int pageId = cellId - m_pageOffset;
	QRect cellRect(m_groupSpace, m_rowSpace + row * (rowHeight() + m_rowSpace), dummyPageSize().width(), rowHeight());
	int width = 0;

	for (int i = 0; i < m_cellsInGroup; ++i)
	{

		int id = cellId + i;

		// skip empty cells
		if (id < m_pageOffset || id >= pageCount() + m_pageOffset)
		{
			//qDebug() << "Cell" << id << "is Empty";
			width = dummyPageSize().width();
		}
		else
		{
			//qDebug() << "Cell" << id << "is Page" << "PageId" << pageId + i;
			PageCell *pc = pageList.at(pageId + i);
			width = pc->pageWidthByHeight(pageHeight());
		}

		cellRect.setWidth( width );

		if(cellRect.contains(pos))
		{
			m_col = i;
			//qDebug() << "Page" << pageId + i << "at" << pos << "in column"<< i;
			break;
		}

		cellRect.setX(cellRect.x() + width + m_cellGap);
	};

	int m_columns = columns();

	return (m_col < m_columns) ? m_col : m_columns -1;
}

int PageGrid::rowAt(QPoint pos)
{
	if(pageCount() == 0) return -1;

	int gridHeight = m_rowSpace + rowHeight();
	int m_rows = rows();
	int m_row = pos.y() / gridHeight;

	return (m_row < m_rows) ? m_row : m_rows -1;
}

int PageGrid::rowWidth(int rowId)
{
	if(pageCount() == 0 || rowId < 0) return -1;

	int cellId = rowId * m_cellsInGroup;
	int pageId = cellId - m_pageOffset;
	int width = 0;

	for (int i = 0; i < m_cellsInGroup; ++i)
	{
		int id = cellId + i;

		if (id < m_pageOffset || id >= pageCount() + m_pageOffset)
		{
			width += dummyPageSize().width();
		}
		else
		{
			PageCell *pc = pageList.at(pageId + i);
			width += pc->pageWidthByHeight(pageHeight());
		}

		width += m_cellGap;

	};

	width += m_groupSpace * 2;

	return width;

}

QRect PageGrid::rectAt(int row, int col)
{
	if ( row == -1 || col == -1) return QRect();

	int cellId = row * m_cellsInGroup;
	int pageId = cellId - m_pageOffset;
	QRect cellRect(m_groupSpace, m_rowSpace + row * (rowHeight() + m_rowSpace), dummyPageSize().width(), rowHeight());
	int width = 0;

	for (int i = 0; i < m_cellsInGroup; ++i)
	{

		int id = cellId + i;

		// skip empty cells
		if (id < m_pageOffset || id >= pageCount() + m_pageOffset)
		{
			width = dummyPageSize().width();
		}
		else
		{
			PageCell *pc = pageList.at(pageId + i);
			width = pc->pageWidthByHeight(pageHeight());
		}

		cellRect.setWidth( width );

		if(i == col)
		{
			return cellRect;
		}

		cellRect.setX(cellRect.x() + width + m_cellGap);
	};

	return QRect();

}

QSize PageGrid::dummyPageSize()
{
	double ratio = m_documentPageSize.width() / m_documentPageSize.height();
	QSize dummySize(pageHeight() * ratio, pageHeight());

	//qDebug() << dummySize << ratio << m_documentPageSize.width() << m_documentPageSize.height();

	return dummySize;
}

QPoint PageGrid::mapPosToCell(QPoint pos, Mode &mode)
{
	int col = columnAt(pos);
	int row = rowAt(pos);

	// return if there is no row or column
	if (col == -1 || row == -1)
	{
		mode = Mode::Invalid;
		return QPoint();
	}

	int id = pageId(row, col, false);
	QRect cellRect = rectAt(row, col);
	int x = cellRect.x();
	int y = cellRect.y();

	// cell doesn't have a page
	if (id < 0 || id >= pageCount()){
		mode = Mode::Add;
	}
	else
	{
		// check if mouse is on "insert area"
		QRect insertArea(x, y, 8, rowHeight());
		if (insertArea.contains(pos))
			mode = Mode::Insert;
		else
			mode = Mode::Hover;
	}

	return QPoint(x,y);
}

QPoint PageGrid::pagePosition(int pageId)
{
	if (pageId < 0 || pageId >= pageCount()) return QPoint();

	int row = qCeil( (pageId + m_pageOffset) / m_cellsInGroup );
	int col = 0;

	return rectAt(row, col).topLeft();
}

int PageGrid::clampPageId(int pageID, bool allowPlusOne)
{
	// Always returns 0 if there is no page in the page list
	if (pageCount() == 0) return 0;

	int max = (allowPlusOne) ? pageCount() : pageCount() -1;
	return qBound( 0, pageID, max);
}


/* ********************************************************************************* *
 *
 * Methods
 *
 * ********************************************************************************* */

void PageGrid::updateSelectedPage(QPoint pos)
{
	int col = columnAt(pos);
	int row = rowAt(pos);
	int id = pageId(row, col);

	// check if page id is in range of an existing cell
	if (id > -1 && id < pageCount() )
	{
		int newSelectedPage = clampPageId( id );

		// repaint only if there is a change
		if (newSelectedPage != m_selectedPage)
		{
			m_selectedPage = newSelectedPage;
			update();
		}
	}

}

void PageGrid::updateModeMarker(QPoint pos)
{	
	Mode mode;
	QPoint mapPos = mapPosToCell(pos, mode);

	// check if mouse is on "insert area"
	switch (mode)
	{
	case Mode::Add:
		m_rectInsert = QRect();
		m_rectAdd = QRect(mapPos.x(), mapPos.y(), dummyPageSize().width(), dummyPageSize().height() );
		m_hoveredPage = -1;
		break;
	case Mode::Insert:
	{
		int selectorWidth = 4;
		int offsetX = mapPos.x() - selectorWidth / 2 - qRound((double)m_cellGap / 2);
		m_rectInsert = QRect(offsetX, mapPos.y() - 4, selectorWidth, pageHeight() + 8 );
		m_rectAdd = QRect();
		m_hoveredPage = -1;
	}
		break;
	case Mode::Hover:
	{
		int col = columnAt(pos);
		int row = rowAt(pos);
		int id = pageId(row, col);
		m_rectInsert = QRect();
		m_rectAdd = QRect();
		m_hoveredPage = id;
	}
		break;
	case Mode::Invalid:
	default:
		m_rectInsert = QRect();
		m_rectAdd = QRect();
		m_hoveredPage = -1;
		break;
	}

	update();

}

void PageGrid::calculateSize()
{
	int width = dummyPageSize().width() + m_groupSpace * 2;

	for (int i = 0; i < rows(); ++i)
	{
		width = qMax(width, rowWidth(i));
	}

	setMinimumHeight(rows() * (m_rowHeight + m_rowSpace) );
	setMinimumWidth(width);

}

void PageGrid::clearUi()
{
	m_rectInsert = QRect();
	m_rectSelection = QRect();
	m_rectAdd = QRect();
	m_hoveredPage = -1;
	update();
}

void PageGrid::drawTile(QPainter &painter, QPoint cellPosition, PageCell *tile, bool selected, bool hovered, QColor labelColor, bool isRight)
{

	if (tile == nullptr)
		return;

	QFont font(this->font().family(), m_fontSize, QFont::Normal);
	QFont fontName(this->font().family(), m_fontSize * 0.8, QFont::Normal);
	QRect rectPage(cellPosition.x(), cellPosition.y(), tile->pageWidthByHeight(pageHeight()), pageHeight());
	QRect rectCell(cellPosition.x(), cellPosition.y(), tile->pageWidthByHeight(pageHeight()), m_rowHeight);

	QString pageName = tile->pageName;
	int pageNameHyphen = tile->pageName.indexOf("-");
	// Fail fast if no hyphen or nothing to the left of it
	if (pageNameHyphen > 0)
	{
		// Trim whitespace and ensure at least 1-4 chars or ignore it
		QString tmp = tile->pageName.left(pageNameHyphen).trimmed();
		if (tmp.length() > 0 && tmp.length() < 5)
			pageName = tmp;
	}

	painter.save();

	// Draw Page
	if (m_rowHeight == TileSize::Small)
		painter.fillRect(rectPage, QBrush(Qt::white));
	else
		painter.drawPixmap(rectPage, tile->pagePreview, tile->pagePreview.rect());

	// Setup painter for text drawing
	painter.setBrush(Qt::NoBrush);
	painter.setRenderHint(QPainter::TextAntialiasing, true);
	painter.setFont(font);

	// Draw Page Number
	painter.setPen(QPen( labelColor ));
	painter.drawText(rectCell, Qt::AlignHCenter|Qt::AlignBottom | Qt::TextWordWrap, tile->pageNumberDisplay);

	// Draw Page Name	
	painter.setFont((pageNameHyphen > 0) ? font : fontName);
	if (m_rowHeight == TileSize::Small)
	{
		painter.setPen(Qt::black);
		painter.drawText(rectPage.adjusted(1, 1, 0, 0), Qt::AlignVCenter | Qt::AlignCenter |Qt::TextWordWrap, pageName);
	}
	else
	{
		painter.setBackgroundMode(Qt::OpaqueMode);
		painter.setBackground(QColor(102, 102, 102));
		painter.setPen(Qt::white);
		painter.drawText(rectPage.adjusted(1, 1, 0, 0), Qt::AlignTop | ((isRight) ? Qt::AlignRight : Qt::AlignLeft) |Qt::TextWordWrap, pageName);
	}


	// Draw selection
	//	if(selected)
	//	{
	//		painter.setBrush(Qt::NoBrush);
	//		painter.setPen( QPen(m_colorSelection, 2) );
	//		painter.drawRect(rectPage);
	//	}

	// Draw hover
	if (hovered)
	{
		QColor colHover = palette().highlight().color();
		colHover.setAlphaF(0.5);
		painter.setBackgroundMode(Qt::TransparentMode);
		painter.setBrush(colHover);
		painter.setPen( Qt::NoPen );
		painter.drawRect(rectPage);
	}

	painter.restore();

}

void PageGrid::initContextMenu()
{
	m_contextMenu->clear();

	m_contextMenu->addAction(tr("&Small Preview"), [this]() {
		setRowHeight(QVariant(TileSize::Small).toInt());
		emit previewSizeChanged();
	});
	m_contextMenu->addAction(tr("&Medium Preview"), [this]() {
		setRowHeight(QVariant(TileSize::Medium).toInt());
		emit previewSizeChanged();
	});
	m_contextMenu->addAction(tr("&Large Preview"), [this]() {
		setRowHeight(QVariant(TileSize::Large).toInt());
		emit previewSizeChanged();
	});
}

void PageGrid::showContextMenu(QPoint pos)
{
	//m_contextMenu->popup( mapToGlobal(pos) );
	m_contextMenu->exec( mapToGlobal(pos) );
}

/* ********************************************************************************* *
 *
 * Events
 *
 * ********************************************************************************* */

void PageGrid::paintEvent(QPaintEvent *event)
{

	int count = 0;
	int x = m_groupSpace;
	int y = m_rowSpace;
	int offset = 0;
	QRect selectedPageRect;
	QColor foregroundColor( PrefsManager::instance().appPrefs.displayPrefs.scratchColor.lightness() <= 128 ? Qt::white : Qt::black);

	QPainter painter(this);

	// Draw background
	painter.fillRect(rect(), PrefsManager::instance().appPrefs.displayPrefs.scratchColor);

	if (pageCount() == 0) return;

//	QElapsedTimer timer;
//	timer.start();

	// Draw pages
	for (int r = 0; r < rows(); r++)
	{
		int groupStart = m_groupSpace;
		int groupWidth = 0;
		bool drawGroupRect = false;

		for (int c = 0; c < columns(); c++)
		{

			// cell is after last page cell
			if (count >= pageCount() + m_pageOffset)
				break;

			// cell is a page cell
			if (count >= m_pageOffset && count < pageCount() + m_pageOffset)
			{
				int id = count - m_pageOffset;
				PageCell * cell = getPageItem(id);
				if (id == m_selectedPage)
					selectedPageRect = QRect(x, y, cell->pageWidthByHeight(pageHeight()), pageHeight() );
				QPoint pos(x,y);
				bool isRightPage = (m_cellsInGroup == c + 1 && m_cellsInGroup > 1);
				drawTile(painter, pos, cell, (id == m_selectedPage) ? true : false, (id == m_hoveredPage) ? true : false, foregroundColor, isRightPage);

				// add space only between pages
				if ((c + 1) % m_cellsInGroup == 0 || count == pageCount() + m_pageOffset -1)
					offset = 0;
				else
					offset = m_cellGap;


				x += cell->pageWidthByHeight(pageHeight()) + offset;
				groupWidth += cell->pageWidthByHeight(pageHeight()) + offset;
				drawGroupRect = true;


			}
			// cell is before first page cell
			else
			{
				offset = ((c + 1) % m_cellsInGroup) == 0 ? 0 : m_cellGap;
				x += dummyPageSize().width() + offset;

				// adjust start on first row if first page has an offset
				if(r == 0)
					groupStart = x;
			}

			count++;
		}

		// Draw group border
		if (drawGroupRect)
			painter.drawRect( QRect(groupStart, y, groupWidth, pageHeight()) );

		y += m_rowHeight + m_rowSpace;
		x = m_groupSpace;
	}

	// Draw selected page
	if (!selectedPageRect.isEmpty())
	{
		QColor colorSelection = (this->isEnabled()) ? m_colorSelection : disabledColor(m_colorSelection);
		painter.setPen( QPen(colorSelection, 2) );
		painter.setBrush(Qt::NoBrush);
		painter.drawRect(selectedPageRect);
	}

	// Draw insert marker
	if (!m_rectInsert.isEmpty())
		painter.fillRect(m_rectInsert, palette().highlight());

	// Draw add page marker
	if (!m_rectAdd.isEmpty())
	{
		QColor colAdd = foregroundColor;
		colAdd.setAlphaF(0.2f);
		painter.setBackgroundMode(Qt::TransparentMode);
		painter.fillRect(m_rectAdd, colAdd);
		painter.setBrush(Qt::NoBrush);
		colAdd.setAlphaF(1.0f);
		painter.setPen( QPen(colAdd, 1, Qt::DashLine) );
		painter.drawRect(m_rectAdd.adjusted(0, 0, -1, -1));
	}

	// Draw selection frame
	if (!m_rectSelection.isEmpty())
	{
		QColor colorSelection = palette().highlight().color();
		painter.setPen( QPen(colorSelection) );
		colorSelection.setAlphaF(0.5);
		painter.setBrush(colorSelection);
		painter.drawRect(m_rectSelection);
	}


	painter.end();

//	qDebug() << "PageGrid::paintEvent() - Render Time Page Grid:" << timer.elapsed() << "milliseconds";

}


void PageGrid::resizeEvent(QResizeEvent *event)
{
	if (event->oldSize() != event->size())
		calculateSize();

	QWidget::resizeEvent(event);
}

void PageGrid::dropEvent(QDropEvent *event)
{

	if (!event->mimeData()->hasFormat(MIMETYPE))
		return;

	QPoint dropEventPos = event->position().toPoint();
	event->setDropAction(Qt::MoveAction);
	event->accept();

	clearUi();

	QString tmp;
	QString str = event->mimeData()->text();

	// external drop
	if (str.startsWith("1"))
	{
		int r = rowAt(dropEventPos);
		int c = columnAt(dropEventPos);
		tmp = str.remove(0,1);

		if ( r == -1 || c == -1 )
			return;

		int p = pageId(r, c);
		Mode mode;
		mapPosToCell(dropEventPos, mode);

		switch (mode)
		{
		case Mode::Add:
		case Mode::Insert:
			emit newPage(p, tmp);
			break;
		case Mode::Hover:
			emit useTemplate(tmp, clampPageId(p));
			break;
		case Mode::Invalid:
			break;
		}

		return;

	}

	// internal drop
	if (str.startsWith("2"))
	{

		int st = str.indexOf(" ");
		int en = str.indexOf(" ", st + 1);
		tmp = str.mid(en + 1);
		int dr = QStringView(str).sliced(st, en - st).toInt();
		int r = rowAt(dropEventPos);
		int c = columnAt(dropEventPos);
		int id = pageId(r, c);

		if ( r == -1 || c == -1 || id == -1 )
			return;

		Mode mode;
		mapPosToCell(dropEventPos, mode);

		switch (mode)
		{
		case Mode::Add:
		case Mode::Insert:
			emit movePage(dr, id);
			break;
		case Mode::Hover:
			// apply master page only if drop is on existing page
			emit useTemplate(tmp, clampPageId(id));
			break;
		case Mode::Invalid:
			break;
		}

		update();
		return;

	}

}

void PageGrid::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat(MIMETYPE))
		event->acceptProposedAction();
}

void PageGrid::dragLeaveEvent(QDragLeaveEvent *event)
{
	clearUi();
}

void PageGrid::dragMoveEvent(QDragMoveEvent *event)
{
	if (!event->mimeData()->hasFormat(MIMETYPE))
		return;

	event->acceptProposedAction();

	QPoint dragEventPos = event->position().toPoint();
	updateModeMarker(dragEventPos);

}

void PageGrid::mouseReleaseEvent(QMouseEvent *event)
{
	event->accept();

	QPoint mouseEventPos = event->position().toPoint();

	switch (m_state)
	{
	case State::StartDrag:
		if(pageCount() > 0)
		{
			updateSelectedPage(mouseEventPos);

			int row = rowAt(mouseEventPos);
			int col = columnAt(mouseEventPos);
			int pageID = pageId(row, col);

			if (pageID > -1 && pageID < pageCount() )
				emit click(m_selectedPage, event->button());
		}
		break;

	case State::StartSelection:
		break;

	default:
		break;

	}

	m_state = State::None;
	clearUi();

	// Context Menu
	if (event->button() == Qt::RightButton)
		m_contextMenu->exec( QCursor::pos() );

	QWidget::mouseReleaseEvent(event);
}

void PageGrid::mousePressEvent(QMouseEvent *event)
{
	event->accept();
	m_mousePos = event->position().toPoint();

	m_state = State::None;

	if (event->button() == Qt::LeftButton)
	{
		int id = pageId(m_mousePos, false);

		if (id == -1 && m_enableSelection == true)
			m_state = State::StartSelection;
		else if (id > -1)
			m_state = State::StartDrag;
	}

	QWidget::mousePressEvent(event);
}

void PageGrid::mouseMoveEvent(QMouseEvent *event)
{

	QPoint mouseEventPos = event->position().toPoint();

	if ((m_mousePos - mouseEventPos).manhattanLength() > 4)
	{
		switch (m_state)
		{
		// Drag
		case State::StartDrag:
		{
			m_state = State::None;

			int a = rowAt(mouseEventPos);
			int b = columnAt(mouseEventPos);
			if ( a != -1 && b != -1 )
			{
				PageCell* ite = getPageItem(pageId(a, b));
				if (ite != nullptr)
				{

					QString str(ite->pageName);

					int p = ite->pageNumber;
					QString tmp;
					QMimeData *mimeData = new QMimeData;
					mimeData->setData(MIMETYPE, "2 " + tmp.setNum(p).toLocal8Bit() + " " + str.toLocal8Bit());
					mimeData->setText("2 " + tmp.setNum(p) + " " + str);
					QDrag *dr = new QDrag(this);
					dr->setMimeData(mimeData);
					//const QPixmap& pm = IconManager::instance().loadPixmap("doc.png");
					const QPixmap& pm = ite->pagePreview;
					dr->setPixmap(pm);
					dr->exec(Qt::CopyAction | Qt::MoveAction);

				}
			}
			break;
		}
		// Selection
		case State::StartSelection:
		{
			int x = qMin(m_mousePos.x(), mouseEventPos.x());
			int y = qMin(m_mousePos.y(), mouseEventPos.y());
			int width = (m_mousePos.x() < mouseEventPos.x()) ? mouseEventPos.x() - m_mousePos.x() : m_mousePos.x() - mouseEventPos.x();
			int height = (m_mousePos.y() < mouseEventPos.y()) ? mouseEventPos.y() - m_mousePos.y() : m_mousePos.y() - mouseEventPos.y();

			m_rectSelection = QRect(x, y, width, height);

			update();
			break;
		}
		default:
			// do nothing
			break;
		}
	}

	QWidget::mouseMoveEvent(event);
}


/* ********************************************************************************* *
 *
 *
 * Trash Bin
 *
 *
 * ********************************************************************************* */
TrashBin::TrashBin(QWidget * parent) : QLabel(parent)
{
	iconSetChange();
	setScaledContents(false);
	setAcceptDrops(true);

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
}

void TrashBin::dragEnterEvent(QDragEnterEvent *e)
{
	if (e->mimeData()->hasFormat(MIMETYPE))
	{
		e->accept();
		setPixmap(open);
	}
}

void TrashBin::dragLeaveEvent(QDragLeaveEvent *)
{
	setPixmap(normal);
}

void TrashBin::dropEvent(QDropEvent * e)
{
	setPixmap(normal);
	QString str, tmp;
	if (e->mimeData()->hasFormat(MIMETYPE))
	{
		e->accept();
		str = e->mimeData()->text();
		if (str.startsWith("2"))
		{
			int st = str.indexOf(" ");
			int en = str.indexOf(" ", st + 1);
			emit delPageRequest(QStringView(str).sliced(st, en - st).toInt());
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
	normal = IconManager::instance().loadPixmap("trashcan.png", 24);
	open = IconManager::instance().loadPixmap("trashcan2.png", 24);
	setPixmap(normal);
}
