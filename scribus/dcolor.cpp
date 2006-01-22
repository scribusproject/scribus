/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <qdialog.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qpixmap.h>
#include <qiconset.h>

#include "commonstrings.h"
#include "scribusdoc.h"
#include "page.h"
#include "sccombobox.h"

#include "dcolor.h"
#include "dcolor.moc"


extern QPixmap loadIcon(QString nam);

DelColor::DelColor( QWidget* parent, ColorList colorList, QString colorName, bool haveDoc)
    : QDialog( parent, "dd", true, 0 )
{
	setName( "DelColor" );
    setCaption( tr( "Delete Color" ) );
  	setIcon(loadIcon("AppIcon.png"));
    dialogLayout = new QVBoxLayout( this, 10, 5 );
    delColorLayout = new QGridLayout;
    delColorLayout->setSpacing( 6 );
    delColorLayout->setMargin( 5 );
    deleteLabel = new QLabel( tr( "Delete Color:" ), this, "deleteLabel" );
    delColorLayout->addWidget( deleteLabel, 0, 0 );
    colorToDelLabel = new QLabel( colorName, this, "colorToDelLabel" );
    delColorLayout->addWidget( colorToDelLabel, 0, 1 );

	if (haveDoc)
	{
    	replaceLabel = new QLabel( tr( "Replace With:" ), this, "replaceLabel" );
    	delColorLayout->addWidget( replaceLabel, 1, 0 );
    	replacementColData = new ScComboBox(false, this);
		ColorList::Iterator it;
		QPixmap pm = QPixmap(15, 15);
		colorList.remove(colorName);
		replacementColData->insertItem( tr("None")); /* 10/26/2004 pv - user can replace deleted color with "None"  */
		for (it = colorList.begin(); it != colorList.end(); ++it)
		{
			pm.fill(colorList[it.key()].getRawRGBColor());
			replacementColData->insertItem(pm, it.key());
		}
    	delColorLayout->addWidget( replacementColData, 1, 1 );
    	replacementColor = replacementColData->text(0);
	}
    dialogLayout->addLayout( delColorLayout );

    okCancelLayout = new QHBoxLayout;
    okCancelLayout->setSpacing( 6 );
    okCancelLayout->setMargin( 0 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    okCancelLayout->addItem( spacer );
    okButton = new QPushButton( CommonStrings::tr_OK, this, "okButton" );
    okCancelLayout->addWidget( okButton );
    cancelButton = new QPushButton( CommonStrings::tr_Cancel, this, "PushButton13" );
    cancelButton->setDefault( true );
    okCancelLayout->addWidget( cancelButton );
    dialogLayout->addLayout( okCancelLayout );
    setMaximumSize(sizeHint());

    connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
	if (haveDoc)
    	connect( replacementColData, SIGNAL(activated(int)), this, SLOT( ReplaceColor(int) ) );
}

void DelColor::ReplaceColor(int id)
{
    replacementColor = replacementColData->text(id);
}

const QString DelColor::getReplacementColor()
{
	return replacementColor;
}
