/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/


#include <QApplication>
#include <QEvent>
#include <QList>
#include <QPainter>
#include <QStackedWidget>

#include "commonstrings.h"
#include "pagelayout.h"
#include "pagepalette.h"
#include "pagepalette_masterpages.h"
#include "pagepalette_pages.h"
#include "pagepalette_widgets.h"
#include "sccombobox.h"
#include "scpage.h"
#include "scribus.h"
#include "scribusview.h"
#include "util_icon.h"

PagePalette::PagePalette(QWidget* parent) : ScDockPalette(parent, "PagePalette", 0)
{
	m_scMW = (ScribusMainWindow*) parent;
	m_view = 0;

	setObjectName(QString::fromLocal8Bit("PagePalette"));

	QStackedWidget* stackedWidget = new QStackedWidget(this);
	stackedWidget->setObjectName(QString::fromLocal8Bit("stackedWidget"));

	PagePalette_Pages* pageWidget = new PagePalette_Pages(stackedWidget);
	pageWidget->setObjectName(QString::fromLocal8Bit("PagePalette_Pages"));
	stackedWidget->addWidget(pageWidget);

	setWidget(stackedWidget);

	connect(pageWidget, SIGNAL(gotoMasterPage(QString)), m_scMW, SLOT(manageMasterPages(QString)));
	
	Rebuild();
	languageChange();
}

QWidget* PagePalette::currentWidget()
{
	QStackedWidget* sw = dynamic_cast<QStackedWidget*>(this->widget());
	if (sw)
		return sw->currentWidget();
	return 0;
}

PagePalette_MasterPages* PagePalette::masterpageWidget() const
{
	if (this->stackedWidget()->count() >= 2)
		return dynamic_cast<PagePalette_MasterPages*>(this->stackedWidget()->widget(1));
	return 0;
}

PagePalette_Pages* PagePalette::pageWidget() const
{
	if (this->stackedWidget()->count() >= 1)
		return dynamic_cast<PagePalette_Pages*>(this->stackedWidget()->widget(0));
	return 0;
}

QStackedWidget* PagePalette::stackedWidget() const
{
	QStackedWidget* sw = dynamic_cast<QStackedWidget*>(this->widget());
	return sw;
}

void PagePalette::enablePalette(const bool enabled)
{
	PagePalette_Pages* pageWidget = dynamic_cast<PagePalette_Pages*>(this->currentWidget());
	if (pageWidget)
		pageWidget->enablePalette(enabled);
}

void PagePalette::rebuildMasters()
{
	if (m_scMW->scriptIsRunning())
		return;

	PagePalette_Pages* pageWidget = dynamic_cast<PagePalette_Pages*>(this->currentWidget());
	if (pageWidget)
		pageWidget->rebuildMasters();
}

void PagePalette::rebuildPages()
{
	if (m_scMW->scriptIsRunning())
		return;
	
	PagePalette_Pages* pageWidget = dynamic_cast<PagePalette_Pages*>(this->currentWidget());
	if (pageWidget)
		pageWidget->rebuildPages();
}

void PagePalette::Rebuild()
{
	rebuildMasters();
	rebuildPages();
	enablePalette(m_view != 0);
}

void PagePalette::markPage(uint nr)
{
	PagePalette_Pages* pageWidget = dynamic_cast<PagePalette_Pages*>(this->currentWidget());
	if (pageWidget)
		pageWidget->markPage(nr);
}

void PagePalette::setView(ScribusView *view)
{
	PagePalette_Pages* pageWidget = dynamic_cast<PagePalette_Pages*>(this->currentWidget());
	if (pageWidget)
		pageWidget->setView(view);
	PagePalette_MasterPages* mpWidget = dynamic_cast<PagePalette_MasterPages*>(this->currentWidget());
	if (mpWidget)
		mpWidget->setView(view);
	m_view = view;
}

void PagePalette::updateMasterPageList()
{
	PagePalette_Pages* pageWidget = dynamic_cast<PagePalette_Pages*>(this->currentWidget());
	if (pageWidget)
		pageWidget->rebuildMasters();
	else
	{
		PagePalette_MasterPages* mpWidget = dynamic_cast<PagePalette_MasterPages*>(this->currentWidget());
		if (mpWidget)
			mpWidget->updateMasterPageList();
	}
}

bool PagePalette::masterPageMode()
{
	PagePalette_MasterPages* mpWidget = dynamic_cast<PagePalette_MasterPages*>(this->currentWidget());
	return (mpWidget != NULL);
}

void PagePalette::startMasterPageMode(QString masterPage)
{
	m_view->Deselect(true);

	QStackedWidget* stackedWidget = this->stackedWidget();
	if (stackedWidget->count() < 2)
	{
		PagePalette_MasterPages* mpWidget = new PagePalette_MasterPages(stackedWidget, m_view, masterPage);
		mpWidget->setObjectName(QString::fromLocal8Bit("PagePalette_MasterPages"));
		stackedWidget->addWidget(mpWidget);

		connect(mpWidget, SIGNAL(removePage(int )), m_scMW, SLOT(deletePage2(int )));
		connect(mpWidget, SIGNAL(finished())      , m_scMW, SLOT(manageMasterPagesEnd()));
	}
	else
	{
		ScribusDoc* doc = m_view->Doc;
		PagePalette_MasterPages* mpWidget = this->masterpageWidget();
		if (mpWidget->currentView != m_view)
			mpWidget->setView(m_view, masterPage);
		mpWidget->updateMasterPageList(masterPage);
		if (doc->currentPage()->pageName() != masterPage)
			mpWidget->selectMasterPage(masterPage);
	}

	// Set focus to page palette or focus may be set to wrong document window
	this->setFocus();
	stackedWidget->setCurrentIndex(1);
}

void PagePalette::endMasterPageMode()
{
	if (this->stackedWidget()->currentIndex() > 0)
	{
		// Set focus to page palette or focus may be set to wrong document window
		this->setFocus();
		this->stackedWidget()->setCurrentIndex(0);
	}

	PagePalette_Pages* pageWidget = dynamic_cast<PagePalette_Pages*>(this->currentWidget());
	pageWidget->setView(m_view);
	pageWidget->Rebuild();
}

void PagePalette::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}

	ScDockPalette::changeEvent(e);
}

void PagePalette::languageChange()
{
	setWindowTitle( tr( "Arrange Pages" ) );
}
