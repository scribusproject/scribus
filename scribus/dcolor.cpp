/****************************************************************************
** Form implementation generated from reading ui file 'Cdel.ui'
**
** Created: Tue Apr 24 22:08:43 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/


#include <qdialog.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qpixmap.h>
#include <qiconset.h>

#include "scribusdoc.h"

#include "dcolor.h"
#include "dcolor.moc"


extern QPixmap loadIcon(QString nam);

DelColor::DelColor( QWidget* parent, CListe colorList, QString colorName, bool haveDoc)
    : QDialog( parent, "dd", true, 0 )
{
	setName( "DelColor" );
    setCaption( tr( "Delete Color" ) );
  	setIcon(loadIcon("AppIcon.png"));
    dialogLayout = new QVBoxLayout( this, 10, 5 );
    delColorLayout = new QGridLayout;
    delColorLayout->setSpacing( 6 );
    delColorLayout->setMargin( 5 );
    deleteQLabel = new QLabel( tr( "Delete Color:" ), this, "deleteQLabel" );
    delColorLayout->addWidget( deleteQLabel, 0, 0 );
    colorToDelQLabel = new QLabel( colorName, this, "colorToDelQLabel" );
    delColorLayout->addWidget( colorToDelQLabel, 0, 1 );

	if (haveDoc)
	{
    	replaceQLabel = new QLabel( tr( "Replace With:" ), this, "replaceQLabel" );
    	delColorLayout->addWidget( replaceQLabel, 1, 0 );
    	replacementColQCBox = new QComboBox(false, this);
		CListe::Iterator it;
		QPixmap pm = QPixmap(15, 15);
		colorList.remove(colorName);
		replacementColQCBox->insertItem(tr("None")); /* 10/26/2004 pv - user can replace deleted color with "None"  */
		for (it = colorList.begin(); it != colorList.end(); ++it)
		{
			pm.fill(colorList[it.key()].getRGBColor());
			replacementColQCBox->insertItem(pm, it.key());
		}
    	delColorLayout->addWidget( replacementColQCBox, 1, 1 );
    	replacementColor = replacementColQCBox->text(0);
	}
    dialogLayout->addLayout( delColorLayout );

    okCancelLayout = new QHBoxLayout;
    okCancelLayout->setSpacing( 6 );
    okCancelLayout->setMargin( 0 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    okCancelLayout->addItem( spacer );
    okQPButton = new QPushButton( tr( "&OK" ), this, "okQPButton" );
    okCancelLayout->addWidget( okQPButton );
    cancelQPButton = new QPushButton( tr( "&Cancel" ), this, "PushButton13" );
    cancelQPButton->setDefault( true );
    okCancelLayout->addWidget( cancelQPButton );
    dialogLayout->addLayout( okCancelLayout );
    setMaximumSize(sizeHint());

    connect( okQPButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( cancelQPButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
	if (haveDoc)
    	connect( replacementColQCBox, SIGNAL(activated(int)), this, SLOT( ReplaceColor(int) ) );
}

void DelColor::ReplaceColor(int id)
{
    replacementColor = replacementColQCBox->text(id);
}

const QString DelColor::getReplacementColor()
{
	return replacementColor;
}
