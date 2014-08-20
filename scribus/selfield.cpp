/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "selfield.h"

#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QLabel>

#include "commonstrings.h"
#include "scfonts.h"
#include "util_icon.h"

SelectFields::SelectFields(QWidget* parent, QString Felder, QString Own, ScribusDoc *Doc, int Art) : QDialog(parent)
{
	setModal(true);
	setWindowTitle( tr( "Select Fields" ) );
	setWindowIcon(QIcon(loadIcon ( "AppIcon.png" )));
	FTyp = Art;
	SelectFieldsLayout = new QVBoxLayout( this );
	SelectFieldsLayout->setMargin(10);
	SelectFieldsLayout->setSpacing(5);
	Layout5 = new QHBoxLayout;
	Layout5->setMargin(0);
	Layout5->setSpacing(5);
	Layout1 = new QVBoxLayout;
	Layout1->setMargin(0);
	Layout1->setSpacing(5);

	Text1 = new QLabel( tr( "Available Fields" ), this );
	Layout1->addWidget( Text1 );
	AvailFields = new QListWidget( this );
	AvailFields->setMinimumSize( QSize( 130, 180 ) );
	for (int se = 0; se < Doc->Items->count(); ++se)
	{
		PageItem* item = Doc->Items->at(se);
		if (Art < 2)
		{
			if ((item->isAnnotation()) && (item->annotation().Type() > 1))
				AvailFields->addItem(item->itemName());
		}
		else
		{
			if ((item->isAnnotation()) && (item->annotation().Type() == Art) && (item->itemName() != Own))
				AvailFields->addItem(item->itemName());
		}
	}
	Layout1->addWidget( AvailFields );
	Layout5->addLayout( Layout1 );

	if (Art > 1)
	{
		Layout2 = new QVBoxLayout;
		Layout2->setMargin(0);
		Layout2->setSpacing(5);
		QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
		Layout2->addItem( spacer );
		ToSel = new QPushButton( tr( "&>>" ), this );
		Layout2->addWidget( ToSel );
		FromSel = new QPushButton( tr( "&<<" ), this );
		Layout2->addWidget( FromSel );
		QSpacerItem* spacer_2 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
		Layout2->addItem( spacer_2 );
		Layout5->addLayout( Layout2 );
		Layout3 = new QVBoxLayout;
		Layout3->setMargin(0);
		Layout3->setSpacing(5);
		Text2 = new QLabel( tr( "Selected Fields" ), this );
		Layout3->addWidget( Text2 );
		SelFields = new QListWidget( this );
		SelFields->setMinimumSize( QSize( 130, 180 ) );
		QStringList pfol;
		pfol = Felder.split(",", QString::SkipEmptyParts);
		if (pfol.count() > 0)
		{
			for (int cfx = 0; cfx < pfol.count(); ++cfx)
				SelFields->addItem(pfol[cfx].simplified());
		}
		FromSel->setEnabled(false);
		ToSel->setEnabled(false);
		Layout3->addWidget( SelFields );
		Layout5->addLayout( Layout3 );
		connect(SelFields, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(SelEField(QListWidgetItem*)));
		connect(ToSel, SIGNAL(clicked()), this, SLOT(PutToSel()));
		connect(FromSel, SIGNAL(clicked()), this, SLOT(RemoveSel()));
	}
	SelectFieldsLayout->addLayout( Layout5 );
	S_Fields = "";
	Layout4 = new QHBoxLayout;
	Layout4->setMargin(0);
	Layout4->setSpacing(5);
	QSpacerItem* spacer_3 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout4->addItem( spacer_3 );
	OK = new QPushButton( CommonStrings::tr_OK, this );
	OK->setDefault( true );
	Layout4->addWidget( OK );
	Cancel = new QPushButton( CommonStrings::tr_Cancel, this );
	Layout4->addWidget( Cancel );
	QSpacerItem* spacer_4 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout4->addItem( spacer_4 );
	SelectFieldsLayout->addLayout( Layout4 );
	connect(OK, SIGNAL(clicked()), this, SLOT(SetRetVal()));
	connect(Cancel, SIGNAL(clicked()), this, SLOT(reject()));
	connect(AvailFields, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(SelAField(QListWidgetItem*)));
}

void SelectFields::SetRetVal()
{
	S_Fields = QString();
	if (FTyp > 1)
	{
		if (SelFields->count() > 0)
			S_Fields = SelFields->item(0)->text();
		for (int r = 1; r < SelFields->count(); ++r)
			S_Fields += ", "+SelFields->item(r)->text();
	}
	else if (AvailFields->currentItem())
	{
		S_Fields = AvailFields->currentItem()->text();
	}

	if (S_Fields.isEmpty())
	{
		reject();
		return;
	}
	accept();
}

void SelectFields::RemoveSel()
{
	QListWidgetItem *it = SelFields->takeItem(SelFields->currentRow());
	delete it;
	SelFields->clearSelection();
	if (SelFields->count() == 0)
		FromSel->setEnabled(false);
}

void SelectFields::PutToSel()
{
	if (SelFields->count() != 0)
	{
		if (SelFields->findItems(AvailFields->currentItem()->text(), Qt::MatchExactly).count() == 0)
			SelFields->addItem(AvailFields->currentItem()->text());
	}
	else
		SelFields->addItem(AvailFields->currentItem()->text());
}

void SelectFields::SelAField(QListWidgetItem *c)
{
	if ((c != NULL) && (FTyp > 1))
	{
		FromSel->setEnabled(false);
		ToSel->setEnabled(true);
		SelFields->clearSelection();
	}
}

void SelectFields::SelEField(QListWidgetItem *c)
{
	if (c != NULL)
	{
		FromSel->setEnabled(true);
		ToSel->setEnabled(false);
		AvailFields->clearSelection();
	}
}
