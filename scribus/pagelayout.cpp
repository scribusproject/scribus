/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "pagelayout.h"
//#include "pagelayout.moc"

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
#include <Q3VBoxLayout>
#include <Q3ValueList>

#include "sccombobox.h"
#include "commonstrings.h"

extern QPixmap loadIcon(QString nam);

PageLayouts::PageLayouts( QWidget* parent, Q3ValueList<PageSet> pSets, bool mode )  : Q3GroupBox( parent )
{
	pageSets = pSets;
	modus = mode;
	setColumnLayout(0, Qt::Vertical );
	layout()->setSpacing( 5 );
	layout()->setMargin( 10 );
	layoutGroupLayout = new Q3VBoxLayout( layout() );
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
		layoutsView->setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Ignored, 0, 0, layoutsView->sizePolicy().hasHeightForWidth() ) );
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
	setMaximumWidth(minimumSizeHint().width());

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
		QString psname=CommonStrings::translatePageSetString(pageSets[pg].Name);
		if (pg == 0)
			layoutsCombo->insertItem(loadIcon("16/page-simple.png"), psname);
		else if (pg == 1)
			layoutsCombo->insertItem(loadIcon("16/page-doublesided.png"), psname);
		else if (pg == 2)
			layoutsCombo->insertItem(loadIcon("16/page-3fold.png"), psname);
		else if (pg == 3)
			layoutsCombo->insertItem(loadIcon("16/page-4fold.png"), psname);
		else
			layoutsCombo->insertItem(loadIcon("16/page-simple.png"), psname);
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
	Q3IconViewItem* ic=0;
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

void PageLayouts::itemSelected(Q3IconViewItem* ic)
{
	if (ic == 0)
		return;
	itemSelectedPost(layoutsView->index(ic));
	emit selectedLayout(layoutsView->index(ic));
}

void PageLayouts::languageChange()
{
	setTitle( tr( "Document Layout" ) );
	if (modus)
	{
		layoutsView->clear();
		for (int pg = 0; pg < pageSets.count(); ++pg)
		{
			QString psname=CommonStrings::translatePageSetString(pageSets[pg].Name);
			if (pg == 0)
				(void) new Q3IconViewItem( layoutsView, psname, loadIcon("32/page-simple.png") );
			else if (pg == 1)
				(void) new Q3IconViewItem( layoutsView, psname, loadIcon("32/page-doublesided.png") );
			else if (pg == 2)
				(void) new Q3IconViewItem( layoutsView, psname, loadIcon("32/page-3fold.png") );
			else if (pg == 3)
				(void) new Q3IconViewItem( layoutsView, psname, loadIcon("32/page-4fold.png") );
			else
				(void) new Q3IconViewItem( layoutsView, psname, loadIcon("32/page-simple.png") );
		}
		int maxWidth = 0;
		Q3IconViewItem* ic = layoutsView->firstItem();
		int startY = 5;
		for (uint cc = 0; cc < layoutsView->count(); ++cc)
		{
			int w = ic->width();
			maxWidth = qMax(w, maxWidth);
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
		disconnect(layoutsCombo, SIGNAL(activated(int)), this, SLOT(itemSelected(int)));
		int currIndex=layoutsCombo->currentItem();
		layoutsCombo->clear();
		for (int pg = 0; pg < pageSets.count(); ++pg)
		{
			QString psname=CommonStrings::translatePageSetString(pageSets[pg].Name);
			if (pg == 0)
				layoutsCombo->insertItem(loadIcon("16/page-simple.png"), psname);
			else if (pg == 1)
				layoutsCombo->insertItem(loadIcon("16/page-doublesided.png"), psname);
			else if (pg == 2)
				layoutsCombo->insertItem(loadIcon("16/page-3fold.png"), psname);
			else if (pg == 3)
				layoutsCombo->insertItem(loadIcon("16/page-4fold.png"), psname);
			else
				layoutsCombo->insertItem(loadIcon("16/page-simple.png"), psname);
		}
		layoutsCombo->setCurrentItem(currIndex);
		connect(layoutsCombo, SIGNAL(activated(int)), this, SLOT(itemSelected(int)));
		
		disconnect(firstPage, SIGNAL(activated(int)), this, SIGNAL(selectedFirstPage(int)));
		int currFirstPageIndex=firstPage->currentItem();
		firstPage->clear();
		if (currIndex>=0 && currIndex<pageSets.count())
			for(QStringList::Iterator pNames = pageSets[currIndex].pageNames.begin(); pNames != pageSets[currIndex].pageNames.end(); ++pNames )
			{
				firstPage->insertItem(CommonStrings::translatePageSetLocString((*pNames)));
			}
		firstPage->setCurrentItem(currFirstPageIndex);
		connect(firstPage, SIGNAL(activated(int)), this, SIGNAL(selectedFirstPage(int)));
	}
	layoutLabel1->setText( tr( "First Page is:" ) );
	

}
