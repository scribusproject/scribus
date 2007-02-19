/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smstyleimport.h"
#include "smstyleimport.moc"
#include "commonstrings.h"

#include <qpixmap.h>
#include <qlayout.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qheader.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>

extern QPixmap loadIcon(QString nam);

ImportDialog::ImportDialog( QWidget* parent, StyleSet<ParagraphStyle> *pstyleList, StyleSet<ParagraphStyle> *pstyleOld,
	                                         StyleSet<CharStyle> *cstyleList, StyleSet<CharStyle> *cstyleOld,
	                                         QMap<QString, multiLine> *lstyleList, QMap<QString, multiLine> *lstyleOld)
: QDialog( parent, "ImportDialog", true, 0 )
{
	setCaption( tr( "Choose Styles" ) );
	setIcon(loadIcon("AppIcon.png"));
	ChooseStylesLayout = new QVBoxLayout( this, 10, 5, "ChooseStylesLayout");
	StyleView = new QListView( this, "StyleView" );
	StyleView->clear();
	StyleView->addColumn( tr( "Available Styles" ) );
	StyleView->header()->setClickEnabled( false, StyleView->header()->count() - 1 );
	StyleView->header()->setResizeEnabled( false, StyleView->header()->count() - 1 );
	StyleView->setSorting(-1);

	QListViewItem *croot = new QListViewItem(StyleView, tr("Character Styles"));
	for (uint x = 0; x < cstyleList->count(); ++x)
	{
		CharStyle& vg ((*cstyleList)[x]);
		const CharStyle* vg2 = static_cast<const CharStyle*>(cstyleOld->resolve(vg.name()));
		bool found = vg2 && vg.equiv(*vg2);
		if (!found)
		{
			QCheckListItem *item = new QCheckListItem (croot, vg.name(), QCheckListItem::CheckBox);
			item->setOn(true);
			storedCharStyles.insert(item, vg.name());
		}
	}

	QListViewItem *proot = new QListViewItem(StyleView, tr("Paragraph Styles"));
	for (uint x = 0; x < pstyleList->count(); ++x)
	{
		ParagraphStyle& vg ((*pstyleList)[x]);
		const ParagraphStyle* vg2 = static_cast<const ParagraphStyle*>(pstyleOld->resolve(vg.name()));
		bool found = vg2 && vg.equiv(*vg2);
		if (!found)
		{
			QCheckListItem *item = new QCheckListItem (proot, vg.name(), QCheckListItem::CheckBox);
			item->setOn(true);
			storedStyles.insert(item, vg.name());
		}
	}

	QListViewItem *lroot = new QListViewItem(StyleView, tr("Line Styles"));
	QValueList<QString> lkeys = lstyleList->keys();
	for (uint x = 0; x < lkeys.count(); ++x)
	{
		QCheckListItem *item = new QCheckListItem (lroot, lkeys[x], QCheckListItem::CheckBox);
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

	clashBox = new QButtonGroup(2, QGroupBox::Vertical, tr("In case of a name clash"), this, "clashBox");
	renameButton = new QRadioButton(tr("Rename imported style"), clashBox, "renameButton");
	renameButton->setChecked(true);
	replaceButton = new QRadioButton(tr("Replace existing style"), clashBox, "replaceButton");
	ChooseStylesLayout->addWidget(clashBox);

	layout2 = new QHBoxLayout( 0, 0, 5, "layout2");
	QSpacerItem* spacer1 = new QSpacerItem( 71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout2->addItem( spacer1 );
	OkButton = new QPushButton( CommonStrings::tr_OK, this, "OkButton" );
	layout2->addWidget( OkButton );
	CancelButton = new QPushButton( CommonStrings::tr_Cancel, this, "CancelButton" );
	layout2->addWidget( CancelButton );
	ChooseStylesLayout->addLayout( layout2 );

	resize(230, 400);
	clearWState( WState_Polished );
	connect(CancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(OkButton, SIGNAL(clicked()), this, SLOT(accept()));
}

bool ImportDialog::clashRename()
{
	return renameButton->isChecked();
}




