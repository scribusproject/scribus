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

#include "page.h"
#include "pagestructs.h"
#include "scribusdoc.h"
#include "commonstrings.h"
#include "util_icon.h"

NewTm::NewTm( QWidget* parent, QString text, QString titel, ScribusDoc *doc, const QString& answerText)
		: QDialog( parent )
{
	setModal(true);
	setWindowTitle(titel);
	setWindowIcon(QIcon(loadIcon ( "AppIcon.png" )));
	QueryLayout = new QVBoxLayout( this );
	QueryLayout->setSpacing( 5 );
	QueryLayout->setMargin( 5 );
	Layout2 = new QHBoxLayout;
	Layout2->setSpacing( 5 );
	Layout2->setMargin( 0 );
	Answer = new QLineEdit( this );
	Frage = new QLabel( text, this );
	Frage->setBuddy(Answer);
// Qt4 	Frage->setFrameShape( QLabel::MShape );
// Qt4 	Frage->setFrameShadow( QLabel::MShadow );
	Frage->adjustSize();
	Layout2->addWidget( Frage );
	Layout2->addWidget( Answer );
	QueryLayout->addLayout( Layout2 );
	if (doc->currentPageLayout != singlePage)
	{
		Layout3 = new QHBoxLayout;
		Layout3->setSpacing( 5 );
		Layout3->setMargin( 0 );
		Links = new QComboBox( this );
		QStringList::Iterator pNames;
		for(pNames = doc->pageSets[doc->currentPageLayout].pageNames.begin(); pNames != doc->pageSets[doc->currentPageLayout].pageNames.end(); ++pNames )
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
	Layout1->setSpacing( 5 );
	Layout1->setMargin( 0 );
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
