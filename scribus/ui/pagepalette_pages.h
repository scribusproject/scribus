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
#include "scdockpalette.h"

class PageLayouts;
class ScribusView;
class ScribusMainWindow;
class SeItem;
class SeList;
class SeView;

class SCRIBUS_API PagePalette_Pages : public QWidget, Ui::PagePalette_PagesBase
{
	Q_OBJECT

public:
	PagePalette_Pages(QWidget* parent);
	~PagePalette_Pages() {};

protected:
	void changeEvent(QEvent *e) override;

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

private slots:
	void iconSetChange();
	void pageView_applyMasterPage(const QString& masterpageName, int pageIndex);
	void pageView_movePage(int r, int c);
	void pageView_gotoPage(int r, int c, int b);
	void pageView_deletePage(int pageIndex);

signals:
	void gotoMasterPage(QString);
	void gotoPage(int);

protected:
	ScribusView       *currView { nullptr};
	ScribusMainWindow *m_scMW { nullptr};

	QList<SeItem*>    pageList;
	QPixmap pix;

	QPixmap createIcon(int number, QString masterPage, const QPixmap& pixin);
};

#endif // SEITENPAL_H
