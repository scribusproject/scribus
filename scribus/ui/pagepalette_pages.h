/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PAGEPALETTE_PAGES_H
#define PAGEPALETTE_PAGES_H

#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QPixmap>
#include <QSplitter>
#include <QVBoxLayout>

#include "ui_pagepalette_pagesbase.h"
#include "scribusapi.h"

class ScribusView;
class ScribusMainWindow;

class SCRIBUS_API PagePalette_Pages : public QWidget, Ui::PagePalette_PagesBase
{
	Q_OBJECT

public:
	PagePalette_Pages(QWidget* parent);
	~PagePalette_Pages() {};

public slots:
	void setView(ScribusView *view);
	void deleteMasterPage(const QString& tmp);
	void enablePalette(const bool);
	void handlePageLayout(int layout);
	void handleFirstPage(int fp);
	//! Recreate master pages thumbnails (if it's set on)
	void rebuildMasters();
	//! Recreate common pages arrangement
	void rebuildPages();
	void rebuild();
	void markPage(uint nr);
	void selMasterPage();
	void languageChange();
	void updatePagePreview();

private slots:
	void iconSetChange();
	void pageView_applyMasterPage(const QString& masterpageName, int pageIndex);
	void pageView_movePage(int r, int c);
	void pageView_gotoPage(int pageID, int b);
	void pageView_deletePage(int pageIndex);
	void pageView_updatePagePreview();

	void newPage();
	void duplicatePage();
	void importPage();
	void movePage();
	void deletePage();

signals:
	void gotoMasterPage(QString);
	void gotoPage(int);

protected:
	ScribusView       *currView { nullptr};
	ScribusMainWindow *m_scMW { nullptr};
	bool m_pagePreviewUpdatePending {true};

//	QPixmap createPagePreview(const QPixmap& pixin, QSize size);

	void changeEvent(QEvent *e) override;
};

#endif // SEITENPAL_H
