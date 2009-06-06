/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QSpacerItem>
#include <QPushButton>

#include "commonstrings.h"
#include "scribusdoc.h"
#include "page.h"
#include "colorcombo.h"
#include "prefsmanager.h"

#include "dcolor.h"
#include "util_icon.h"

DelColor::DelColor( QWidget* parent, ColorList colorList, QString colorName, bool haveDoc) : QDialog( parent )
{
	setModal(true);
	setWindowTitle( tr( "Delete Color" ) );
	setWindowIcon(QIcon(loadIcon ( "AppIcon.png" )));
	dialogLayout = new QVBoxLayout( this );
	dialogLayout->setMargin(10);
	dialogLayout->setSpacing(5);
	delColorLayout = new QGridLayout;
	delColorLayout->setSpacing( 5 );
	delColorLayout->setMargin( 5 );
	deleteLabel = new QLabel( tr( "Delete Color:" ), this );
	delColorLayout->addWidget( deleteLabel, 0, 0 );
	colorToDelLabel = new QLabel( colorName, this );
	delColorLayout->addWidget( colorToDelLabel, 0, 1 );

	PrefsManager* prefsManager = PrefsManager::instance();
	bool isToolColor = prefsManager->isToolColor(colorName);
	if (haveDoc || isToolColor)
	{
		replaceLabel = new QLabel( tr( "Replace With:" ), this );
		delColorLayout->addWidget( replaceLabel, 1, 0 );
		replacementColData = new ColorCombo(false, this);
		colorList.remove(colorName);
		// 10/26/2004 pv - user can replace deleted color with "None"
		replacementColData->addItem(CommonStrings::tr_NoneColor);
		replacementColData->insertItems(colorList, ColorCombo::fancyPixmaps);
		delColorLayout->addWidget( replacementColData, 1, 1 );
		replacementColor = replacementColData->itemText(0);
	}
	dialogLayout->addLayout( delColorLayout );

	okCancelLayout = new QHBoxLayout;
	okCancelLayout->setSpacing( 5 );
	okCancelLayout->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
	okCancelLayout->addItem( spacer );
	okButton = new QPushButton( CommonStrings::tr_OK, this );
	okCancelLayout->addWidget( okButton );
	cancelButton = new QPushButton( CommonStrings::tr_Cancel, this );
	cancelButton->setDefault( true );
	okCancelLayout->addWidget( cancelButton );
	dialogLayout->addLayout( okCancelLayout );
	setMaximumSize(sizeHint());

	connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
	if (haveDoc || isToolColor)
		connect( replacementColData, SIGNAL(activated(int)), this, SLOT( ReplaceColor(int) ) );
}

void DelColor::ReplaceColor(int id)
{
	replacementColor = replacementColData->itemText(id);
}

const QString DelColor::getReplacementColor()
{
	return replacementColor;
}
