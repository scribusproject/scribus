#include "prefsdialogbase.h"
#include "prefsdialogbase.moc"
#include <qvariant.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
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
	prefsSelection->setArrangement(QIconView::TopToBottom);
	prefsSelection->setItemsMovable(false);
	prefsSelection->setAutoArrange( true );
	prefsSelection->setSorting( false );
	prefsSelection->setFocusPolicy(QWidget::NoFocus);
	prefsSelection->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)7, 0, 0, prefsSelection->sizePolicy().hasHeightForWidth() ) );
	layout3->addWidget( prefsSelection );
	prefsWidgets = new QWidgetStack( this, "prefsWidgets" );
	layout3->addWidget( prefsWidgets );
	prefsLayout->addLayout( layout3 );
	layout4 = new QHBoxLayout( 0, 0, 6, "layout4");
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout4->addItem( spacer );
	buttonOk = new QPushButton( this, "buttonOk" );
	buttonOk->setAutoDefault( TRUE );
	buttonOk->setDefault( TRUE );
	layout4->addWidget( buttonOk );
	buttonCancel = new QPushButton( this, "buttonCancel" );
	buttonCancel->setAutoDefault( TRUE );
	layout4->addWidget( buttonCancel );
	prefsLayout->addLayout( layout4 );
	languageChange();
	connect(prefsSelection, SIGNAL(clicked(QIconViewItem *)), this, SLOT(itemSelected(QIconViewItem* )));
}

void PrefsDialogBase::addItem(QString name, QPixmap icon, QWidget *tab)
{
	QIconViewItem* ic = new QIconViewItem(prefsSelection, name, icon);
	prefsWidgets->addWidget(tab, counter);
	itemMap.insert(ic, counter);
	counter++;
}

void PrefsDialogBase::itemSelected(QIconViewItem* ic)
{
	if (ic == 0)
		return;
	if (itemMap.contains(ic))
		prefsWidgets->raiseWidget(itemMap[ic]);
}
/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void PrefsDialogBase::languageChange()
{
    buttonOk->setText( tr( "&OK" ) );
    buttonCancel->setText( tr( "&Cancel" ) );
}

