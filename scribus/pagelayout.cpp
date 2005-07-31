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

extern QPixmap loadIcon(QString nam);

PageLayouts::PageLayouts( QWidget* parent )  : QGroupBox( parent )
{
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

	layout1 = new QHBoxLayout( 0, 0, 5, "layout1");

	layoutLabel1 = new QLabel( this, "layoutLabel1" );
	layout1->addWidget( layoutLabel1 );

	firstPage = new QSpinBox( this, "firstPage" );
	firstPage->setMaxValue( 4 );
	firstPage->setMinValue( 1 );
	layout1->addWidget( firstPage );
	layoutGroupLayout->addLayout( layout1 );

	layoutLabel2 = new QLabel( this, "layoutLabel2" );
	layoutGroupLayout->addWidget( layoutLabel2 );
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
			firstPage->setMaxValue( layoutsView->index(ic)+1 );
			firstPage->setValue( 1 );
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
	firstPage->setMaxValue( layoutsView->index(ic)+1 );
	firstPage->setValue( 1 );
	layoutsView->setSelected(ic, true);
	emit selectedLayout(layoutsView->index(ic));
}

void PageLayouts::languageChange()
{
	setTitle( tr( "Page Layout" ) );
	layoutsView->clear();
	(void) new QIconViewItem( layoutsView, tr( "Single Page" ), loadIcon("pagesingle.png") );
	(void) new QIconViewItem( layoutsView, tr( "Double sided" ), loadIcon("pagedouble.png") );
	(void) new QIconViewItem( layoutsView, tr( "3-Fold" ), loadIcon("pagetriple.png") );
	(void) new QIconViewItem( layoutsView, tr( "4-Fold" ), loadIcon("pagequadro.png") );
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
	layoutLabel1->setText( tr( "Page #" ) );
	layoutLabel2->setText( tr( "is the first Page." ) );
}
