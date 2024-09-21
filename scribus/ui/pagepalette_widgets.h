/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PAGEPALETTE_WIDGETS_H
#define PAGEPALETTE_WIDGETS_H

#include "qmenu.h"
#include <QCheckBox>
#include <QDialog>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QHBoxLayout>
#include <QImage>
#include <QKeyEvent>
#include <QLabel>
#include <QLayout>
#include <QListWidget>
#include <QMouseEvent>
#include <QPixmap>
#include <QPushButton>
#include <QScrollArea>
#include <QSplitter>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QToolTip>
#include <QVBoxLayout>

class QEvent;

#include "scribusapi.h"
#include "pagestructs.h"

class ScribusView;
class ScribusMainWindow;
class PageLayouts;


/* ********************************************************************************* *
 *
 * Master Page List
 *
 * ********************************************************************************* */

class SCRIBUS_API SeList : public QListWidget
{
	friend class PagePalette;
	friend class PagePalette_Pages;

	Q_OBJECT

public:
	SeList(QWidget* parent);
	~SeList() {};

private slots:
	void toggleThumbnail();

signals:
	//! Emitted when user requests/disables the thumbnais for master pages.
	void thumbnailChanged();
	void delMasterRequest(QString);

protected:
	void mouseReleaseEvent(QMouseEvent *m);
	void mousePressEvent(QMouseEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
	virtual void keyPressEvent(QKeyEvent* e);

	QListWidgetItem *m_currItem {nullptr};
	QPoint m_mousePos {QPoint()};
	bool m_mousePressed {false};
	bool m_thumb {false};

};

struct SCRIBUS_API PageCell
{
	QString pageName {QString()};
	uint pageNumber {1};
	QPixmap pagePreview {QPixmap()};
	QColor masterPageColor {QColor()};
	double pageRatio {1.4142}; // IsoA

	PageCell(const QString& text, uint nr, const QPixmap& pix, double pageRatio, const QColor color = Qt::black) :
		pageName(text), pageNumber(nr), pagePreview(pix), masterPageColor(color), pageRatio(pageRatio) {}

	int pageWidthByHeight(int height) { return qCeil(height * pageRatio); }

};

/* ********************************************************************************* *
 *
 * Page Grid
 *
 * ********************************************************************************* */

class SCRIBUS_API PageGrid : public QWidget
{
	Q_OBJECT

	friend class PageViewer;

public:

	enum TileSize {
		Small = 64,
		Medium = 96,
		Large = 128
	};

	explicit PageGrid(QWidget *parent = nullptr);

	void setPageInGroup(int amount);

	void setDocumentPageSize(QSizeF pageSize);

	void setRowHeight(int height);
	int rowHeight() { return m_rowHeight; };

	void setFontSize(int size);
	int fontSize() { return m_fontSize; };

	void setSelectionColor(QColor color);
	QColor selectionColor() { return m_colorSelection; };

	void setPageLayout(PageLayout layout);
	PageLayout pageLayout() { return m_pageLayout; };

	void setPageOffset(int pageCount);
	int pageOffset() { return m_pageOffset; };

	QList<PageCell*> pageList {QList<PageCell*>()};

	int pageId(int r, int c, bool clampId = true);
	int pageId(QPoint pos, bool clampId = true);

	PageCell* getPageItem(int pageIndex);

	int pageCount();
	int pageHeight();

	void setSelectedPage(int pageID);
	int selectedPage() { return m_selectedPage; };

	void deleteSelectedPage();
	void clear();
	void calculateSize();

private:

	enum State {
		None = 0,
		StartDrag = 1,
		StartSelection = 2
	};

	enum Mode {
		Invalid = 0,
		Insert = 1,
		Add = 2,
		Hover = 3
	};

	QSize m_pageSize {QSize()};
	QSizeF m_documentPageSize {QSizeF()};
	int m_rowHeight {QVariant(TileSize::Medium).toInt()};
	int m_cellGap {1};
	int m_groupSpace {16};
	int m_rowSpace {12};
	int m_fontSize {QFont().pointSize()}; // font size of number label and masterpage label
	int m_labelGap {8}; // gap between page and number label
	QRect m_rectInsert {QRect()};
	QRect m_rectSelection {QRect()};
	QRect m_rectAdd {QRect()};
	QColor m_colorSelection;
	int m_selectedPage {-1};
	int m_hoveredPage {-1};
	bool m_enableSelection {false};
	QPoint m_mousePos {QPoint()};
	State m_state {State::None};
	PageLayout m_pageLayout {PageLayout::singlePage};
	int m_cellsInGroup {1}; // 1 for single page
	int m_pageOffset {0};
	QMenu *m_contextMenu {nullptr};

	int columns();
	int rows();

	int columnAt(QPoint pos);
	int rowAt(QPoint pos);
	int rowWidth(int rowId);
	QRect rectAt(int row, int col);

	QSize dummyPageSize();

	QPoint mapPosToCell(QPoint pos, Mode &mode);
	QPoint pagePosition(int pageId);

	int clampPageId(int pageID, bool allowPlusOne = false);

	void updateSelectedPage(QPoint pos);
	void updateModeMarker(QPoint pos);
	void clearUi();
	void drawTile(QPainter &painter, QPoint cellPosition, PageCell * tile, bool selected, bool hovered, QColor labelColor, bool isRight);
	void initContextMenu();

private slots:
	void showContextMenu(QPoint pos);

protected:

	void paintEvent(QPaintEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;

	void dropEvent(QDropEvent * event) override;
	void dragEnterEvent(QDragEnterEvent *event) override;
	void dragLeaveEvent(QDragLeaveEvent *event) override;
	void dragMoveEvent(QDragMoveEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

signals:
	void useTemplate(QString, int);
	void newPage(int, QString);
	void movePage(int, int);
	void click(int, int);
	void delPageRequest(int);
	void previewSizeChanged();
};

/* ********************************************************************************* *
 *
 * Page Viewer
 *
 * ********************************************************************************* */

class SCRIBUS_API PageViewer : public QWidget
{

public:
	PageViewer(QWidget *parent = nullptr);
	~PageViewer() {};

	PageGrid *pageGrid();
	void scrollToPage(int pageId);


protected:
	PageGrid *m_pageGrid {nullptr};
	QScrollArea* m_scroll {nullptr};

	virtual void keyPressEvent(QKeyEvent* event);

};

/* ********************************************************************************* *
 *
 * Trash bin
 *
 * ********************************************************************************* */

class SCRIBUS_API TrashBin : public QLabel
{
	Q_OBJECT

public:
	TrashBin( QWidget * parent );
	~TrashBin() {};

	void dragEnterEvent( QDragEnterEvent *e );
	void dragLeaveEvent( QDragLeaveEvent * );
	void dropEvent( QDropEvent * e );

protected:
	QPixmap normal {QPixmap()};
	QPixmap open {QPixmap()};

protected slots:
	void iconSetChange();

signals:
	void delPageRequest(int);
	void delMasterRequest(QString);
};

#endif

