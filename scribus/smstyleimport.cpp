/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smstyleimport.h"
//#include "smstyleimport.moc"
#include "commonstrings.h"

#include <qpixmap.h>
#include <qlayout.h>
#include <q3listview.h>
#include <qpushbutton.h>
#include <q3header.h>
#include <q3buttongroup.h>
#include <qradiobutton.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include "util_icon.h"

ImportDialog::ImportDialog( QWidget* parent, StyleSet<ParagraphStyle> *pstyleList,
	                                         StyleSet<CharStyle> *cstyleList,
	                                         QMap<QString, multiLine> *lstyleList)
: QDialog( parent, "ImportDialog", true, 0 )
{
	setCaption( tr( "Choose Styles" ) );
	setIcon(loadIcon("AppIcon.png"));
	ChooseStylesLayout = new Q3VBoxLayout( this, 10, 5, "ChooseStylesLayout");
	StyleView = new Q3ListView( this, "StyleView" );
	StyleView->clear();
	StyleView->addColumn( tr( "Available Styles" ) );
	StyleView->header()->setClickEnabled( false, StyleView->header()->count() - 1 );
	StyleView->header()->setResizeEnabled( false, StyleView->header()->count() - 1 );
	StyleView->setSorting(-1);

	Q3ListViewItem *croot = new Q3ListViewItem(StyleView, tr("Character Styles"));
	for (uint x = 0; x < cstyleList->count(); ++x)
	{
		CharStyle& vg ((*cstyleList)[x]);
		Q3CheckListItem *item = new Q3CheckListItem (croot, vg.name(), Q3CheckListItem::CheckBox);
		item->setOn(true);
		storedCharStyles.insert(item, vg.name());
	}

	Q3ListViewItem *proot = new Q3ListViewItem(StyleView, tr("Paragraph Styles"));
	for (uint x = 0; x < pstyleList->count(); ++x)
	{
		ParagraphStyle& vg ((*pstyleList)[x]);
		Q3CheckListItem *item = new Q3CheckListItem (proot, vg.name(), Q3CheckListItem::CheckBox);
		item->setOn(true);
		storedStyles.insert(item, vg.name());
	}

	Q3ListViewItem *lroot = new Q3ListViewItem(StyleView, tr("Line Styles"));
	QList<QString> lkeys = lstyleList->keys();
	for (int x = 0; x < lkeys.count(); ++x)
	{
		Q3CheckListItem *item = new Q3CheckListItem (lroot, lkeys[x], Q3CheckListItem::CheckBox);
		item->setOn(true);
		storedLineStyles.insert(item, lkeys[x]);
	}

	StyleView->setSorting(0);
	StyleView->setTreeStepSize(10);
	StyleView->setRootIsDecorated(true);
	StyleView->setOpen(lroot, true);
	StyleView->setOpen(croot, true);
	StyleView->setOpen(proot, true);

	ChooseStylesLayout->addWidget( StyleView );

	clashBox = new Q3ButtonGroup(2, Qt::Vertical, tr("In case of a name clash"), this, "clashBox");
	renameButton = new QRadioButton(tr("Rename imported style"), clashBox, "renameButton");
	renameButton->setChecked(true);
	replaceButton = new QRadioButton(tr("Replace existing style"), clashBox, "replaceButton");
	ChooseStylesLayout->addWidget(clashBox);

	layout2 = new Q3HBoxLayout( 0, 0, 5, "layout2");
	QSpacerItem* spacer1 = new QSpacerItem( 71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout2->addItem( spacer1 );
	OkButton = new QPushButton( CommonStrings::tr_OK, this, "OkButton" );
	layout2->addWidget( OkButton );
	CancelButton = new QPushButton( CommonStrings::tr_Cancel, this, "CancelButton" );
	layout2->addWidget( CancelButton );
	ChooseStylesLayout->addLayout( layout2 );

	resize(230, 400);

	connect(CancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(OkButton, SIGNAL(clicked()), this, SLOT(accept()));
}

bool ImportDialog::clashRename()
{
	return renameButton->isChecked();
}




