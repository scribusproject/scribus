/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "newtemp.h"

#include <QPixmap>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>

#include "commonstrings.h"
#include "iconmanager.h"
#include "pagestructs.h"
#include "scpage.h"
#include "scribusdoc.h"

NewTm::NewTm( QWidget* parent, const QString& text, const QString& title, ScribusDoc *doc, const QString& answerText)
		: QDialog( parent )
{
	setModal(true);
	setWindowTitle(title);
	setWindowIcon(IconManager::instance().loadIcon("AppIcon.png"));
	QueryLayout = new QVBoxLayout( this );
	QueryLayout->setSpacing(6);
	QueryLayout->setContentsMargins(9, 9, 9, 9);
	Layout2 = new QHBoxLayout;
	Layout2->setSpacing(6);
	Layout2->setContentsMargins(0, 0, 0, 0);
	Answer = new QLineEdit( this );
	Frage = new QLabel( text, this );
	Frage->setBuddy(Answer);
	Frage->adjustSize();
	Layout2->addWidget( Frage );
	Layout2->addWidget( Answer );
	QueryLayout->addLayout( Layout2 );

	Layout3 = nullptr;
	Links = nullptr;
	if (doc->pagePositioning() != singlePage)
	{
		Layout3 = new QHBoxLayout;
		Layout3->setSpacing(6);
		Layout3->setContentsMargins(0, 0, 0, 0);
		Links = new QComboBox( this );
		QStringList::Iterator pNames;
		QList<PageSet> pageSet(doc->pageSets());
		for (pNames = pageSet[doc->pagePositioning()].pageNames.begin(); pNames != pageSet[doc->pagePositioning()].pageNames.end(); ++pNames )
		{
			//Links->insertItem((*pNames));
			Links->addItem(CommonStrings::translatePageSetLocString((*pNames)));
		}
		if (doc->currentPage()->LeftPg == 1)
			Links->setCurrentIndex(0);
		else if (doc->currentPage()->LeftPg == 0)
			Links->setCurrentIndex(Links->count()-1);
		else
			Links->setCurrentIndex(doc->currentPage()->LeftPg-1);
		Links->setEditable(false);
		Layout3->addWidget( Links );
		QueryLayout->addLayout( Layout3 );
	}
	Layout1 = new QHBoxLayout;
	Layout1->setSpacing(6);
	Layout1->setContentsMargins(0, 0, 0, 0);
	QSpacerItem* spacer = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1->addItem( spacer );
	PushButton1 = new QPushButton( CommonStrings::tr_OK, this );
	PushButton1->setDefault( true );
	Layout1->addWidget( PushButton1 );
	PushButton2 = new QPushButton( CommonStrings::tr_Cancel, this );
	Layout1->addWidget( PushButton2 );
	QueryLayout->addLayout( Layout1 );
	if (!answerText.isEmpty())
	{
		Answer->setText(answerText);
		Answer->setMinimumWidth(sizeHint().width());
		Answer->selectAll();
	}
	setMinimumSize(sizeHint());
	Answer->setFocus();

	// signals and slots connections
	connect( PushButton1, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( PushButton2, SIGNAL( clicked() ), this, SLOT( reject() ) );
}
