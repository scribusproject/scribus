/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PAGEPALETTE_MASTERPAGES_H
#define PAGEPALETTE_MASTERPAGES_H

class QCloseEvent;
class QHBoxLayout;
class QListWidget;
class QListWidgetItem;
class QToolButton;
class QVBoxLayout;

#include "scribusapi.h"
#include "ui_pagepalette_masterpagesbase.h"
class ScribusDoc;
class ScribusView;


/*! \brief A dialog to manage/edit Master Pages
*/
class SCRIBUS_API PagePalette_MasterPages : public QWidget, Ui::PagePalette_MasterPagesBase
{
	Q_OBJECT

	friend class PagePalette;

public:
	PagePalette_MasterPages( QWidget* parent, ScribusView *pCurrentView, QString masterPageName);
	~PagePalette_MasterPages() {};

	void setView(ScribusView* view, QString masterPageName = "");
	
	void updateMasterPageList(void);
	void updateMasterPageList(QString MasterPageName);

public slots:
	void languageChange();
	void selectMasterPage(QString name);

private:
	QString      currentPage;
	ScribusDoc  *currentDoc;
	ScribusView *currentView;

protected:

	void connectSignals();
	void disconnectSignals();

	virtual void changeEvent(QEvent *e);

private slots:
	void duplicateMasterPage();
	void deleteMasterPage();
	void newMasterPage();
	void renameMasterPage(QListWidgetItem * item);
	void importPage();
	void selectMasterPage(QListWidgetItem *item);

signals:
	void removePage(int);
	void finished();
};

#endif // MUSTERPAGES_H
