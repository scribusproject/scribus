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
#include <qcombobox.h>

extern QPixmap loadIcon(QString nam);

PageLayouts::PageLayouts( QWidget* parent, QValueList<PageSet> pSets )  : QGroupBox( parent )
{
	pageSets = pSets;
	setColumnLayout(0, Qt::Vertical );
	layout()->setSpacing( 5 );
	layout()->setMargin( 10 );
	layoutGroupLayout = new QVBoxLayout( layout() );
	layoutGroupLayout->setAlignment( Qt::AlignTop );

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

	layoutLabel1 = new QLabel( this, "layoutLabel1" );
	layoutGroupLayout->addWidget( layoutLabel1 );
	firstPage = new QComboBox( true, this, "LMode" );
	firstPage->setEditable(false);
	layoutGroupLayout->addWidget( firstPage );
	languageChange();
	clearWState( WState_Polished );
	connect(layoutsView, SIGNAL(clicked(QIconViewItem *)), this, SLOT(itemSelected(QIconViewItem* )));
}

void PageLayouts::selectItem(uint nr)
{
	QIconViewItem* ic = layoutsView->firstItem();
	for (uint cc = 0; cc < layoutsView->count(); ++cc)
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
			layoutsView->setSelected(ic, true);
			layoutsView->ensureVisible(0, ic->y(), 5, 5);
			break;
		}
		ic = ic->nextItem();
	}
}

void PageLayouts::itemSelected(QIconViewItem* ic)
{
	if (ic == 0)
		return;
	int choosen = layoutsView->index(ic);
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
	layoutsView->setSelected(ic, true);
	emit selectedLayout(layoutsView->index(ic));
}

void PageLayouts::languageChange()
{
	setTitle( tr( "Page Layout" ) );
	layoutsView->clear();
	for (uint pg = 0; pg < pageSets.count(); ++pg)
	{
		if (pg == 0)
			(void) new QIconViewItem( layoutsView, pageSets[pg].Name, loadIcon("pagesingle.png") );
		else if (pg == 1)
			(void) new QIconViewItem( layoutsView, pageSets[pg].Name, loadIcon("pagedouble.png") );
		else if (pg == 2)
			(void) new QIconViewItem( layoutsView, pageSets[pg].Name, loadIcon("pagetriple.png") );
		else if (pg == 3)
			(void) new QIconViewItem( layoutsView, pageSets[pg].Name, loadIcon("pagequadro.png") );
		else
			(void) new QIconViewItem( layoutsView, pageSets[pg].Name, loadIcon("page.png") );
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
	layoutLabel1->setText( tr( "First Page is:" ) );
}
