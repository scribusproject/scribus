#include "prefsdialogbase.h"
#include <qvariant.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
#include <qlabel.h>
#include <qfont.h>
//Added by qt3to4:
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "commonstrings.h"

extern QPixmap loadIcon(QString nam);

PrefsDialogBase::PrefsDialogBase( QWidget* parent ) : QDialog( parent, "PrefsDialogBase", true, 0 )
{
	counter = 0;
	setName( "PrefsDialogBase" );
	setIcon(loadIcon("AppIcon.png"));
	setSizeGripEnabled( true );
	prefsLayout = new QVBoxLayout( this, 11, 6, "prefsLayout"); 
	layout3 = new QHBoxLayout( 0, 0, 6, "layout3"); 
	prefsSelection = new Q3IconView( this, "prefsSelection" );
	prefsSelection->setHScrollBarMode( Q3IconView::AlwaysOff );
	prefsSelection->setVScrollBarMode( Q3IconView::Auto );
	prefsSelection->setArrangement(Q3IconView::LeftToRight);
	prefsSelection->setItemsMovable(false);
	prefsSelection->setAutoArrange( false );
	prefsSelection->setSorting( false );
	prefsSelection->setWordWrapIconText(false);
	prefsSelection->setFocusPolicy(Qt::NoFocus);
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
	prefsWidgets = new Q3WidgetStack( this, "prefsWidgets" );
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
	connect(prefsSelection, SIGNAL(clicked(Q3IconViewItem *)), this, SLOT(itemSelected(Q3IconViewItem* )));
	connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

int PrefsDialogBase::addItem(QString name, QPixmap icon, QWidget *tab)
{
	Q3IconViewItem* icx = new Q3IconViewItem(prefsSelection, name, icon);
	prefsWidgets->addWidget(tab, counter);
	icx->setDragEnabled(false);
	itemMap.insert(icx, counter);
	counter++;
	return counter-1;
}

void PrefsDialogBase::arrangeIcons()
{
	int maxWidth = 0;
	Q3IconViewItem* ic = prefsSelection->firstItem();
	int startY = 5;
	for (uint cc = 0; cc < prefsSelection->count(); ++cc)
	{
		int w = ic->width();
		maxWidth = QMAX(w, maxWidth);
		ic = ic->nextItem();
	}
	ic = prefsSelection->firstItem();
	prefsSelection->setAutoArrange( false );
	prefsSelection->setResizeMode(Q3IconView::Fixed);
	for (uint cc = 0; cc < prefsSelection->count(); ++cc)
	{
		int w = ic->width();
		int moveW = (maxWidth - w) / 2;
		ic->move(moveW, startY);
		startY += ic->height()+5;
		ic = ic->nextItem();
	}
}

void PrefsDialogBase::itemSelected(Q3IconViewItem* ic)
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
    buttonOk->setText( CommonStrings::tr_OK );
    buttonCancel->setText( CommonStrings::tr_Cancel );
    backToDefaults->setText( tr( "&Defaults" ) );
}

