#include "pagelayout.h"
#include "sccombobox.h"

#include <qvariant.h>
#include <q3groupbox.h>
#include <q3iconview.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
#include <qpixmap.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <Q3ValueList>

extern QPixmap loadIcon(QString nam);

PageLayouts::PageLayouts( QWidget* parent, Q3ValueList<PageSet> pSets, bool mode )  : Q3GroupBox( parent )
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
		layoutsView = new Q3IconView( this, "layoutsView" );
		layoutsView->setHScrollBarMode( Q3IconView::AlwaysOff );
		layoutsView->setVScrollBarMode( Q3IconView::Auto );
		layoutsView->setArrangement(Q3IconView::LeftToRight);
		layoutsView->setItemsMovable(false);
		layoutsView->setAutoArrange( false );
		layoutsView->setSorting( false );
		layoutsView->setFocusPolicy(Qt::NoFocus);
		layoutsView->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)7, 0, 0, layoutsView->sizePolicy().hasHeightForWidth() ) );
		layoutsView->setSelectionMode(Q3IconView::Single);
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
	setAttribute( Qt::WA_WState_Polished, false );
	if (modus)
		connect(layoutsView, SIGNAL(clicked(Q3IconViewItem *)), this, SLOT(itemSelected(Q3IconViewItem* )));
	else
		connect(layoutsCombo, SIGNAL(activated(int)), this, SLOT(itemSelected(int)));
	connect(firstPage, SIGNAL(activated(int)), this, SIGNAL(selectedFirstPage(int)));
}

void PageLayouts::updateLayoutSelector(Q3ValueList<PageSet> pSets)
{
	disconnect(layoutsCombo, SIGNAL(activated(int)), this, SLOT(itemSelected(int)));
	pageSets = pSets;
	layoutsCombo->clear();
	for (int pg = 0; pg < pageSets.count(); ++pg)
	{
		if (pg == 0)
			layoutsCombo->insertItem(loadIcon("pagesingle16.png"), pageSets[pg].Name);
		else if (pg == 1)
			layoutsCombo->insertItem(loadIcon("pagedouble16.png"), pageSets[pg].Name);
		else if (pg == 2)
			layoutsCombo->insertItem(loadIcon("pagetriple16.png"), pageSets[pg].Name);
		else if (pg == 3)
			layoutsCombo->insertItem(loadIcon("pagequadro16.png"), pageSets[pg].Name);
		else
			layoutsCombo->insertItem(loadIcon("page16.png"), pageSets[pg].Name);
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
	Q3IconViewItem* ic;
	uint cce;
	if (modus)
	{
		disconnect(layoutsView, SIGNAL(clicked(Q3IconViewItem *)), this, SLOT(itemSelected(Q3IconViewItem* )));
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
					firstPage->insertItem((*pNames));
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
		connect(layoutsView, SIGNAL(clicked(Q3IconViewItem *)), this, SLOT(itemSelected(Q3IconViewItem* )));
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
			firstPage->insertItem((*pNames));
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

void PageLayouts::itemSelected(Q3IconViewItem* ic)
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
		for (int pg = 0; pg < pageSets.count(); ++pg)
		{
			if (pg == 0)
				(void) new Q3IconViewItem( layoutsView, pageSets[pg].Name, loadIcon("pagesingle.png") );
			else if (pg == 1)
				(void) new Q3IconViewItem( layoutsView, pageSets[pg].Name, loadIcon("pagedouble.png") );
			else if (pg == 2)
				(void) new Q3IconViewItem( layoutsView, pageSets[pg].Name, loadIcon("pagetriple.png") );
			else if (pg == 3)
				(void) new Q3IconViewItem( layoutsView, pageSets[pg].Name, loadIcon("pagequadro.png") );
			else
				(void) new Q3IconViewItem( layoutsView, pageSets[pg].Name, loadIcon("page.png") );
		}
		int maxWidth = 0;
		Q3IconViewItem* ic = layoutsView->firstItem();
		int startY = 5;
		for (uint cc = 0; cc < layoutsView->count(); ++cc)
		{
			int w = ic->width();
			maxWidth = QMAX(w, maxWidth);
			ic = ic->nextItem();
		}
		ic = layoutsView->firstItem();
		layoutsView->setAutoArrange( false );
		layoutsView->setResizeMode(Q3IconView::Fixed);
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
		for (int pg = 0; pg < pageSets.count(); ++pg)
		{
			if (pg == 0)
				layoutsCombo->insertItem(loadIcon("pagesingle16.png"), pageSets[pg].Name);
			else if (pg == 1)
				layoutsCombo->insertItem(loadIcon("pagedouble16.png"), pageSets[pg].Name);
			else if (pg == 2)
				layoutsCombo->insertItem(loadIcon("pagetriple16.png"), pageSets[pg].Name);
			else if (pg == 3)
				layoutsCombo->insertItem(loadIcon("pagequadro16.png"), pageSets[pg].Name);
			else
				layoutsCombo->insertItem(loadIcon("page16.png"), pageSets[pg].Name);
		}
	}
	layoutLabel1->setText( tr( "First Page is:" ) );
}
