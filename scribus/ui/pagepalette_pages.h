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
	
	virtual void changeEvent(QEvent *e);

public slots:
	void setView(ScribusView *view);
	void deleteMasterPage(QString tmp);
	void pageView_movePage(int r, int c);
	void pageView_gotoPage(int r, int c, int b);
	void enablePalette(const bool);
	void handlePageLayout(int layout);
	void handleFirstPage(int fp);
	//! Recreate master pages thumbnails (if it's set on)
	void rebuildMasters();
	//! Recreate common pages arrangement
	void rebuildPages();
	void Rebuild();
	void markPage(uint nr);
	void selMasterPage();
	QPixmap CreateIcon(int nr, QPixmap pixin);
	void languageChange();

signals:
	void gotoMasterPage(QString);
	void gotoPage(int);

protected:
	ScribusView       *currView;
	ScribusMainWindow *m_scMW;

	QList<SeItem*>    pageList;
	QPixmap pix;
};

#endif // SEITENPAL_H
