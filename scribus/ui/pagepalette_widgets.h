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
	QPoint m_mousePos;
	bool m_mousePressed {false};
	bool m_thumb {false};

};

/* ********************************************************************************* *
 *
 * Page Cell
 *
 * ********************************************************************************* */

class SCRIBUS_API PageCell
{
	friend class PanelPages;
	friend class PageGrid;

public:
	PageCell(const QString& text, uint pgnr, const QPixmap& pix, double pageRatio, const QColor color = Qt::black);
	// ~PageCell() {};

	const QString& pageName() {return m_pageName;}
	QPixmap pagePreview() {return m_pagePreview;}
	uint pageNumber() {return m_pageNumber;}
	QColor masterPageColor() {return m_masterPageColor;}
	int pageWidthByHeight(int height) {return qCeil(height * m_pageRatio);}
	double pageRatio() {return m_pageRatio;}

	void setPageName(const QString& name ) {m_pageName = name;}
	void setPagePreview(QPixmap preview ) {m_pagePreview = preview;}
	void setPageNumber(uint number ) {m_pageNumber = number;}
	void setMasterPageColor(QColor color ) {m_masterPageColor = color;}
	void setPageRatio(double pageRatio){m_pageRatio = pageRatio;}

private:
	uint m_pageNumber;
	QPixmap m_pagePreview;
	QString m_pageName;
	QColor m_masterPageColor;
	double m_pageRatio;
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
	explicit PageGrid(QWidget *parent = nullptr);

	void setPageInGroup(int amount);

	void setDocumentPageSize(QSizeF pageSize);

	void setRowHeight(int size);
	int rowHeight();

	void setFontSize(int size);
	int fontSize();

	void setSelectionColor(QColor color);
	QColor selectionColor();

	void setPageLayout(PageLayout layout);
	PageLayout pageLayout();

	void setPageOffset(int pageCount);
	int pageOffset();

	QList<PageCell*> pageList;

	int pageId(int r, int c, bool clampId = true);
	int pageId(QPoint pos, bool clampId = true);

	PageCell* getPageItem(int pageIndex);

	int pageCount();
	int pageHeight();

	void setSelectedPage(int pageID);
	int selectedPage();

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

	QSize m_pageSize;
	QSizeF m_documentPageSize;
	int m_rowHeight;
	int m_cellGap;
	int m_groupSpace;
	int m_rowSpace;
	int m_fontSize;
	int m_labelGap;
	QRect m_rectInsert;
	QRect m_rectSelection;
	QRect m_rectAdd;
	QColor m_colorSelection;
	int m_selectedPage;
	int m_hoveredPage;
	bool m_enableSelection;
	QPoint m_mousePos;
	State m_state {State::None};
	PageLayout m_pageLayout;
	int m_cellsInGroup;
	int m_pageOffset;
	QMenu *m_contextMenu;

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
	void drawTile(QPainter &painter, QPoint cellPosition, PageCell * tile, bool selected, bool hovered);
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
	PageGrid *m_pageGrid;
	QScrollArea* m_scroll;

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
	QPixmap normal;
	QPixmap open;

protected slots:
	void iconSetChange();

signals:
	void delPageRequest(int);
	void delMasterRequest(QString);
};

#endif

