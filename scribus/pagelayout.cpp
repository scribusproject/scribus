/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "pagelayout.h"
#include "pagelayout.moc"

#include <qvariant.h>
#include <qgroupbox.h>
#include <qiconview.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qpixmap.h>

#include "sccombobox.h"
#include "commonstrings.h"

extern QPixmap loadIcon(QString nam);

PageLayouts::PageLayouts( QWidget* parent, QValueList<PageSet> pSets, bool mode )  : QGroupBox( parent )
{
	pageSets = pSets;
	modus = mode;
	setColumnLayout(0, Qt::Vertical );
	layout()->setSpacing( 5 );
	layout()->setMargin( 10 );
	layoutGroupLayout = new QVBoxLayout( layout() );
	layoutGroupLayout->setAlignment( Qt::AlignTop );
	if (modus)
	{
		layoutsView = new QIconView( this, "layoutsView" );
		layoutsView->setHScrollBarMode( QIconView::AlwaysOff );
		layoutsView->setVScrollBarMode( QIconView::Auto );
		layoutsView->setArrangement(QIconView::LeftToRight);
		layoutsView->setItemsMovable(false);
		layoutsView->setAutoArrange( false );
		layoutsView->setSorting( false );
		layoutsView->setFocusPolicy(QWidget::NoFocus);
		layoutsView->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)7, 0, 0, layoutsView->sizePolicy().hasHeightForWidth() ) );
		layoutsView->setSelectionMode(QIconView::Single);
		layoutGroupLayout->addWidget( layoutsView );
	}
	else
	{
		layoutsCombo = new ScComboBox( false, this, "layoutsCombo" );
		layoutGroupLayout->addWidget( layoutsCombo );
	}
	layoutLabel1 = new QLabel( this, "layoutLabel1" );
	layoutGroupLayout->addWidget( layoutLabel1 );
	firstPage = new ScComboBox( false, this, "firstPage" );
	layoutGroupLayout->addWidget( firstPage );
	languageChange();
	clearWState( WState_Polished );
	if (modus)
		connect(layoutsView, SIGNAL(clicked(QIconViewItem *)), this, SLOT(itemSelected(QIconViewItem* )));
	else
		connect(layoutsCombo, SIGNAL(activated(int)), this, SLOT(itemSelected(int)));
	connect(firstPage, SIGNAL(activated(int)), this, SIGNAL(selectedFirstPage(int)));
}

void PageLayouts::updateLayoutSelector(QValueList<PageSet> pSets)
{
	disconnect(layoutsCombo, SIGNAL(activated(int)), this, SLOT(itemSelected(int)));
	pageSets = pSets;
	layoutsCombo->clear();
	for (uint pg = 0; pg < pageSets.count(); ++pg)
	{
		QString psname=CommonStrings::translatePageSetString(pageSets[pg].Name);
		if (pg == 0)
			layoutsCombo->insertItem(loadIcon("pagesingle16.png"), psname);
		else if (pg == 1)
			layoutsCombo->insertItem(loadIcon("pagedouble16.png"), psname);
		else if (pg == 2)
			layoutsCombo->insertItem(loadIcon("pagetriple16.png"), psname);
		else if (pg == 3)
			layoutsCombo->insertItem(loadIcon("pagequadro16.png"), psname);
		else
			layoutsCombo->insertItem(loadIcon("page16.png"), psname);
	}
	connect(layoutsCombo, SIGNAL(activated(int)), this, SLOT(itemSelected(int)));
}

void PageLayouts::selectFirstP(int nr)
{
	disconnect(firstPage, SIGNAL(activated(int)), this, SIGNAL(selectedFirstPage(int)));
	firstPage->setCurrentItem(nr);
	connect(firstPage, SIGNAL(activated(int)), this, SIGNAL(selectedFirstPage(int)));
}

void PageLayouts::selectItem(uint nr)
{
	QIconViewItem* ic=0;
	uint cce;
	if (modus)
	{
		disconnect(layoutsView, SIGNAL(clicked(QIconViewItem *)), this, SLOT(itemSelected(QIconViewItem* )));
		ic = layoutsView->firstItem();
		cce = layoutsView->count();
	}
	else
	{
		disconnect(layoutsCombo, SIGNAL(activated(int)), this, SLOT(itemSelected(int)));
		cce = layoutsCombo->count();
	}
	disconnect(firstPage, SIGNAL(activated(int)), this, SIGNAL(selectedFirstPage(int)));
	for (uint cc = 0; cc < cce; ++cc)
	{
		if (cc == nr)
		{
			if (cc > 0)
			{
				firstPage->setEnabled(true);
				firstPage->clear();
				QStringList::Iterator pNames;
				for(pNames = pageSets[cc].pageNames.begin(); pNames != pageSets[cc].pageNames.end(); ++pNames )
				{
					firstPage->insertItem(CommonStrings::translatePageSetLocString((*pNames)));
				}
			}
			else
			{
				firstPage->clear();
				firstPage->insertItem(" ");
				firstPage->setEnabled(false);
			}
			if (modus)
			{
				layoutsView->setSelected(ic, true);
				layoutsView->ensureVisible(0, ic->y(), 5, 5);
			}
			else
				layoutsCombo->setCurrentItem(cc);
			break;
		}
		if (modus)
			ic = ic->nextItem();
	}
	if (modus)
		connect(layoutsView, SIGNAL(clicked(QIconViewItem *)), this, SLOT(itemSelected(QIconViewItem* )));
	else
		connect(layoutsCombo, SIGNAL(activated(int)), this, SLOT(itemSelected(int)));
	connect(firstPage, SIGNAL(activated(int)), this, SIGNAL(selectedFirstPage(int)));
}

void PageLayouts::itemSelectedPost(int choosen)
{
	disconnect(firstPage, SIGNAL(activated(int)), this, SIGNAL(selectedFirstPage(int)));
	if (choosen > 0)
	{
		firstPage->setEnabled(true);
		firstPage->clear();
		QStringList::Iterator pNames;
		for(pNames = pageSets[choosen].pageNames.begin(); pNames != pageSets[choosen].pageNames.end(); ++pNames )
		{
			firstPage->insertItem(CommonStrings::translatePageSetLocString((*pNames)));
		}
	}
	else
	{
		firstPage->clear();
		firstPage->insertItem(" ");
		firstPage->setEnabled(false);
	}
	connect(firstPage, SIGNAL(activated(int)), this, SIGNAL(selectedFirstPage(int)));
}

void PageLayouts::itemSelected(int ic)
{
	itemSelectedPost(ic);
	emit selectedLayout(ic);
}

void PageLayouts::itemSelected(QIconViewItem* ic)
{
	if (ic == 0)
		return;
	itemSelectedPost(layoutsView->index(ic));
	emit selectedLayout(layoutsView->index(ic));
}

void PageLayouts::languageChange()
{
	setTitle( tr( "Page Layout" ) );
	if (modus)
	{
		layoutsView->clear();
		for (uint pg = 0; pg < pageSets.count(); ++pg)
		{
			QString psname=CommonStrings::translatePageSetString(pageSets[pg].Name);
			if (pg == 0)
				(void) new QIconViewItem( layoutsView, psname, loadIcon("pagesingle.png") );
			else if (pg == 1)
				(void) new QIconViewItem( layoutsView, psname, loadIcon("pagedouble.png") );
			else if (pg == 2)
				(void) new QIconViewItem( layoutsView, psname, loadIcon("pagetriple.png") );
			else if (pg == 3)
				(void) new QIconViewItem( layoutsView, psname, loadIcon("pagequadro.png") );
			else
				(void) new QIconViewItem( layoutsView, psname, loadIcon("page.png") );
		}
		int maxWidth = 0;
		QIconViewItem* ic = layoutsView->firstItem();
		int startY = 5;
		for (uint cc = 0; cc < layoutsView->count(); ++cc)
		{
			int w = ic->width();
			maxWidth = QMAX(w, maxWidth);
			ic = ic->nextItem();
		}
		ic = layoutsView->firstItem();
		layoutsView->setAutoArrange( false );
		layoutsView->setResizeMode(QIconView::Fixed);
		for (uint cc = 0; cc < layoutsView->count(); ++cc)
		{
			int w = ic->width();
			int moveW = (maxWidth - w) / 2;
			ic->move(moveW, startY);
			startY += ic->height()+5;
			ic = ic->nextItem();
		}
	}
	else
	{
		layoutsCombo->clear();
		for (uint pg = 0; pg < pageSets.count(); ++pg)
		{
			QString psname=CommonStrings::translatePageSetString(pageSets[pg].Name);
			if (pg == 0)
				layoutsCombo->insertItem(loadIcon("pagesingle16.png"), psname);
			else if (pg == 1)
				layoutsCombo->insertItem(loadIcon("pagedouble16.png"), psname);
			else if (pg == 2)
				layoutsCombo->insertItem(loadIcon("pagetriple16.png"), psname);
			else if (pg == 3)
				layoutsCombo->insertItem(loadIcon("pagequadro16.png"), psname);
			else
				layoutsCombo->insertItem(loadIcon("page16.png"), psname);
		}
	}
	layoutLabel1->setText( tr( "First Page is:" ) );
}
