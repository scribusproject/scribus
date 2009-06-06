/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "pagelayout.h"

#include <QVBoxLayout>
#include <QListWidgetItem>
#include <QLabel>
#include <QPixmap>
#include <QList>

#include "sccombobox.h"
#include "commonstrings.h"
#include "util_icon.h"

PageListWidget::PageListWidget(QWidget* parent) : QListWidget(parent)
{
	setDragEnabled(false);
	setViewMode(QListView::IconMode);
	setFlow(QListView::TopToBottom);
	setSortingEnabled(false);
	setWrapping(false);
	setWordWrap(true);
	setAcceptDrops(false);
	setDropIndicatorShown(false);
	setDragDropMode(QAbstractItemView::NoDragDrop);
	setResizeMode(QListView::Adjust);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setFocusPolicy(Qt::NoFocus);
	setIconSize(QSize(32, 32));
	setMaximumWidth(100);
	clear();
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
}

void PageListWidget::arrangeIcons()
{
	int maxWidth = 0;
	QListWidgetItem* ic;
	int startY = 5;
	for (int cc = 0; cc < count(); ++cc)
	{
		ic = item(cc);
		QRect ir = visualItemRect(ic);
		maxWidth = qMax(ir.width(), maxWidth);
	}
	setResizeMode(QListView::Fixed);
	int startX = (viewport()->width() - maxWidth) / 2;
	for (int cc = 0; cc < count(); ++cc)
	{
		ic = item(cc);
		QRect ir = visualItemRect(ic);
		int moveW = (maxWidth - ir.width()) / 2;
		setPositionForIndex(QPoint(moveW + startX, startY), indexFromItem(ic));
		startY += ir.height()+5;
	}
}

PageLayouts::PageLayouts( QWidget* parent, QList<PageSet> pSets, bool mode )  : QGroupBox( parent )
{
	pageSets = pSets;
	modus = mode;
	layoutGroupLayout = new QVBoxLayout(this);
	layoutGroupLayout->setSpacing( 5 );
	layoutGroupLayout->setMargin( 10 );
	if (modus)
	{
		layoutsView = new PageListWidget( this );
		layoutGroupLayout->addWidget( layoutsView );
	}
	else
	{
		layoutsCombo = new ScComboBox( this );
		layoutGroupLayout->addWidget( layoutsCombo );
	}
	layoutLabel1 = new QLabel( this );
	layoutGroupLayout->addWidget( layoutLabel1 );
	firstPage = new ScComboBox( this );
	layoutGroupLayout->addWidget( firstPage );
	languageChange();
// 	We need change combos width setMaximumWidth(minimumSizeHint().width());

	if (modus)
		connect(layoutsView, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(itemSelected(QListWidgetItem* )));
	else
		connect(layoutsCombo, SIGNAL(activated(int)), this, SLOT(itemSelected(int)));
	connect(firstPage, SIGNAL(activated(int)), this, SIGNAL(selectedFirstPage(int)));
}

void PageLayouts::updateLayoutSelector(QList<PageSet> pSets)
{
	disconnect(layoutsCombo, SIGNAL(activated(int)), this, SLOT(itemSelected(int)));
	pageSets = pSets;
	layoutsCombo->clear();
	for (int pg = 0; pg < pageSets.count(); ++pg)
	{
		QString psname=CommonStrings::translatePageSetString(pageSets[pg].Name);
		if (pg == 0)
			layoutsCombo->addItem(loadIcon("16/page-simple.png"), psname);
		else if (pg == 1)
			layoutsCombo->addItem(loadIcon("16/page-doublesided.png"), psname);
		else if (pg == 2)
			layoutsCombo->addItem(loadIcon("16/page-3fold.png"), psname);
		else if (pg == 3)
			layoutsCombo->addItem(loadIcon("16/page-4fold.png"), psname);
		else
			layoutsCombo->addItem(loadIcon("16/page-simple.png"), psname);
	}
	connect(layoutsCombo, SIGNAL(activated(int)), this, SLOT(itemSelected(int)));
}

void PageLayouts::selectFirstP(int nr)
{
	disconnect(firstPage, SIGNAL(activated(int)), this, SIGNAL(selectedFirstPage(int)));
	firstPage->setCurrentIndex(nr);
	connect(firstPage, SIGNAL(activated(int)), this, SIGNAL(selectedFirstPage(int)));
}

void PageLayouts::selectItem(uint nr)
{
	if (modus)
		disconnect(layoutsView, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(itemSelected(QListWidgetItem* )));
	else
		disconnect(layoutsCombo, SIGNAL(activated(int)), this, SLOT(itemSelected(int)));
	disconnect(firstPage, SIGNAL(activated(int)), this, SIGNAL(selectedFirstPage(int)));
	if (nr > 0)
	{
		firstPage->setEnabled(true);
		firstPage->clear();
		QStringList::Iterator pNames;
		for(pNames = pageSets[nr].pageNames.begin(); pNames != pageSets[nr].pageNames.end(); ++pNames )
		{
			firstPage->addItem(CommonStrings::translatePageSetLocString((*pNames)));
		}
	}
	else
	{
		firstPage->clear();
		firstPage->addItem(" ");
		firstPage->setEnabled(false);
	}
	if (modus)
	{
		layoutsView->setCurrentRow(nr);
		layoutsView->item(nr)->setSelected(true);
		connect(layoutsView, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(itemSelected(QListWidgetItem* )));
	}
	else
	{
		layoutsCombo->setCurrentIndex(nr);
		connect(layoutsCombo, SIGNAL(activated(int)), this, SLOT(itemSelected(int)));
	}
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
			firstPage->addItem(CommonStrings::translatePageSetLocString((*pNames)));
		}
	}
	else
	{
		firstPage->clear();
		firstPage->addItem(" ");
		firstPage->setEnabled(false);
	}
	connect(firstPage, SIGNAL(activated(int)), this, SIGNAL(selectedFirstPage(int)));
}

void PageLayouts::itemSelected(int ic)
{
	itemSelectedPost(ic);
	emit selectedLayout(ic);
}

void PageLayouts::itemSelected(QListWidgetItem* ic)
{
	if (ic == 0)
		return;
	itemSelectedPost(layoutsView->row(ic));
	emit selectedLayout(layoutsView->row(ic));
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
			QListWidgetItem *ic;
			if (pg == 0)
			{
				ic = new QListWidgetItem( QIcon(loadIcon("32/page-simple.png")), psname, layoutsView );
				ic->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			}
			else if (pg == 1)
			{
				ic = new QListWidgetItem( QIcon(loadIcon("32/page-doublesided.png")), psname, layoutsView );
				ic->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			}
			else if (pg == 2)
			{
				ic = new QListWidgetItem( QIcon(loadIcon("32/page-3fold.png")), psname, layoutsView );
				ic->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			}
			else if (pg == 3)
			{
				ic = new QListWidgetItem( QIcon(loadIcon("32/page-4fold.png")), psname, layoutsView );
				ic->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			}
			else
			{
				ic = new QListWidgetItem( QIcon(loadIcon("32/page-simple.png")), psname, layoutsView );
				ic->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			}
		}
		layoutsView->arrangeIcons();
	}
	else
	{
		disconnect(layoutsCombo, SIGNAL(activated(int)), this, SLOT(itemSelected(int)));
		int currIndex=layoutsCombo->currentIndex();
		layoutsCombo->clear();
		for (int pg = 0; pg < pageSets.count(); ++pg)
		{
			QString psname=CommonStrings::translatePageSetString(pageSets[pg].Name);
			if (pg == 0)
				layoutsCombo->addItem(loadIcon("16/page-simple.png"), psname);
			else if (pg == 1)
				layoutsCombo->addItem(loadIcon("16/page-doublesided.png"), psname);
			else if (pg == 2)
				layoutsCombo->addItem(loadIcon("16/page-3fold.png"), psname);
			else if (pg == 3)
				layoutsCombo->addItem(loadIcon("16/page-4fold.png"), psname);
			else
				layoutsCombo->addItem(loadIcon("16/page-simple.png"), psname);
		}
		layoutsCombo->setCurrentIndex(currIndex);
		connect(layoutsCombo, SIGNAL(activated(int)), this, SLOT(itemSelected(int)));

		disconnect(firstPage, SIGNAL(activated(int)), this, SIGNAL(selectedFirstPage(int)));
		int currFirstPageIndex=firstPage->currentIndex();
		firstPage->clear();
		if (currIndex>=0 && currIndex<pageSets.count())
			for(QStringList::Iterator pNames = pageSets[currIndex].pageNames.begin(); pNames != pageSets[currIndex].pageNames.end(); ++pNames )
			{
				firstPage->addItem(CommonStrings::translatePageSetLocString((*pNames)));
			}
		firstPage->setCurrentIndex(currFirstPageIndex);
		connect(firstPage, SIGNAL(activated(int)), this, SIGNAL(selectedFirstPage(int)));
	}
	layoutLabel1->setText( tr( "First Page is:" ) );

	QString layoutText( tr( "Number of pages to show side-by-side on the canvas\nOften used for allowing items to be placed across page spreads" ) );
	if (modus)
		layoutsView->setToolTip(layoutText);
	else
		layoutsCombo->setToolTip(layoutText);
	firstPage->setToolTip( tr( "Location on the canvas where the first page of the document is placed" ) );
}
