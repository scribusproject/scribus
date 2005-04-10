#include "prefsdialogbase.h"
#include "prefsdialogbase.moc"
#include <qvariant.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qlabel.h>
#include <qfont.h>
extern QPixmap loadIcon(QString nam);

PrefsDialogBase::PrefsDialogBase( QWidget* parent ) : QDialog( parent, "PrefsDialogBase", true, 0 )
{
	counter = 0;
	setName( "PrefsDialogBase" );
	setIcon(loadIcon("AppIcon.png"));
	setSizeGripEnabled( TRUE );
	prefsLayout = new QVBoxLayout( this, 11, 6, "prefsLayout"); 
	layout3 = new QHBoxLayout( 0, 0, 6, "layout3"); 
	prefsSelection = new QIconView( this, "prefsSelection" );
	prefsSelection->setHScrollBarMode( QIconView::AlwaysOff );
	prefsSelection->setVScrollBarMode( QIconView::Auto );
	prefsSelection->setArrangement(QIconView::LeftToRight);
	prefsSelection->setItemsMovable(false);
	prefsSelection->setAutoArrange( false );
	prefsSelection->setSorting( false );
	prefsSelection->setFocusPolicy(QWidget::NoFocus);
	prefsSelection->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)7, 0, 0, prefsSelection->sizePolicy().hasHeightForWidth() ) );
	layout3->addWidget( prefsSelection );
	layout5 = new QVBoxLayout( 0, 0, 6, "layout5");
	tabNameLabel = new QLabel( this, "tabNameLabel" );
	QFont f(tabNameLabel->font());
	f.setPointSize(f.pointSize()+4);
	f.setBold(true);
	tabNameLabel->setFont(f);
	tabNameLabel->setText("");
	layout5->addWidget( tabNameLabel );
	prefsWidgets = new QWidgetStack( this, "prefsWidgets" );
	layout5->addWidget( prefsWidgets );
	layout3->addLayout(layout5);
	prefsLayout->addLayout( layout3 );
	layout4 = new QHBoxLayout( 0, 0, 6, "layout4");
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout4->addItem( spacer );
	backToDefaults = new QPushButton( this, "backToDefaults" );
	backToDefaults->setAutoDefault( false );
	backToDefaults->setDefault( false );
	layout4->addWidget( backToDefaults );
	buttonOk = new QPushButton( this, "buttonOk" );
	buttonOk->setAutoDefault( false );
	buttonOk->setDefault( false );
	layout4->addWidget( buttonOk );
	buttonCancel = new QPushButton( this, "buttonCancel" );
	buttonCancel->setAutoDefault( false );
	buttonCancel->setDefault( false );
	layout4->addWidget( buttonCancel );
	prefsLayout->addLayout( layout4 );
	languageChange();
	connect(prefsSelection, SIGNAL(clicked(QIconViewItem *)), this, SLOT(itemSelected(QIconViewItem* )));
	connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

int PrefsDialogBase::addItem(QString name, QPixmap icon, QWidget *tab)
{
	QIconViewItem* icx = new QIconViewItem(prefsSelection, name, icon);
	prefsWidgets->addWidget(tab, counter);
	icx->setDragEnabled(false);
	itemMap.insert(icx, counter);
	counter++;
	return counter-1;
}

void PrefsDialogBase::arrangeIcons()
{
	int maxWidth = 0;
	QIconViewItem* ic = prefsSelection->firstItem();
	int startY = 5;
	for (uint cc = 0; cc < prefsSelection->count(); ++cc)
	{
		int w = ic->width();
		maxWidth = QMAX(w, maxWidth);
		ic = ic->nextItem();
	}
	ic = prefsSelection->firstItem();
	prefsSelection->setAutoArrange( false );
	prefsSelection->setResizeMode(QIconView::Fixed);
	for (uint cc = 0; cc < prefsSelection->count(); ++cc)
	{
		int w = ic->width();
		int moveW = (maxWidth - w) / 2;
		ic->move(moveW, startY);
		startY += ic->height()+5;
		ic = ic->nextItem();
	}
}

void PrefsDialogBase::itemSelected(QIconViewItem* ic)
{
	if (ic == 0)
		return;
	if (itemMap.contains(ic))
	{
		prefsWidgets->raiseWidget(itemMap[ic]);
		tabNameLabel->setText(ic->text());
	}
}
/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void PrefsDialogBase::languageChange()
{
    buttonOk->setText( tr( "&OK" ) );
    buttonCancel->setText( tr( "&Cancel" ) );
    backToDefaults->setText( tr( "&Defaults" ) );
}

