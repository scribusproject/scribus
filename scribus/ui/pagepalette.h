/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SEITENPAL_H
#define SEITENPAL_H

#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QPixmap>
#include <QSplitter>
#include <QToolTip>
#include <QVBoxLayout>

class QEvent;
class QStackedWidget;
class SeItem;
class SeList;
class SeView;
class TrashBin;

#include "scribusapi.h"
#include "scdockpalette.h"

class PagePalette_MasterPages;
class PagePalette_Pages;
class ScribusView;
class ScribusMainWindow;

class SCRIBUS_API PagePalette : public ScDockPalette
{
	Q_OBJECT

public:
	PagePalette(QWidget* parent);
	~PagePalette() {};
	
	QWidget* currentWidget();
	QStackedWidget* stackedWidget() const;

	PagePalette_MasterPages* masterpageWidget() const;
	PagePalette_Pages* pageWidget() const;

protected:
	ScribusView       *m_view;
	ScribusMainWindow *m_scMW;

	virtual void changeEvent(QEvent *e);

public slots:
	void setView(ScribusView *view);
	void enablePalette(const bool);
	//! Recreate master pages thumbnails (if it's set on)
	void rebuildMasters();
	//! Recreate common pages arrangement
	void rebuildPages();
	//! Rebuild all lists
	void Rebuild();
	void markPage(uint nr);
	void updateMasterPageList();

	void languageChange();

	bool masterPageMode();
	void startMasterPageMode(QString);
	void endMasterPageMode();

signals:
	void gotoPage(int);
};

#endif // SEITENPAL_H
