/****************************************************************************
** Form implementation generated from reading ui file 'InsPage.ui'
**
** Created: Tue Jun 12 21:21:13 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include <qdialog.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include "scribusdoc.h"

#include "inspage.h"
#include "inspage.moc"

extern QPixmap loadIcon(QString nam);

InsPage::InsPage( QWidget* parent, ScribusDoc* currentDoc, int currentPage, int maxPages, bool facingPages)
		: QDialog( parent, "InsPage", true, 0 )
{
	setCaption( tr( "Insert Page" ) );
	setIcon(loadIcon("AppIcon.png"));
	dialogLayout = new QVBoxLayout( this, 10, 5 );
	whereLayout = new QGridLayout;
	whereLayout->setSpacing( 6 );
	whereLayout->setMargin( 5 );
	insCountData = new QSpinBox( this, "insCountData" );
	insCountData->setMinValue( 1 );
	insCountData->setValue( 1 );
	insCountLabel = new QLabel( insCountData, tr( "&Insert" ), this, "insCountLabel" );
	whereLayout->addWidget( insCountLabel, 0, 0 );
	whereLayout->addWidget( insCountData, 0, 1 );
	pagesLabel = new QLabel( tr( "Page(s)" ), this, "pagesLabel" );
	whereLayout->addWidget( pagesLabel, 0, 2 );

	insWhereData = new QComboBox( false, this, "insWhereData" );
	insWhereData->insertItem( tr("before Page"));
	insWhereData->insertItem( tr("after Page"));
	insWhereData->insertItem( tr("at End"));
	insWhereData->setCurrentItem(2);
	whereLayout->addMultiCellWidget( insWhereData, 1, 1, 0, 1 );

	insWherePageData = new QSpinBox( 1, maxPages, 1, this, "insWherePageData" );
	insWherePageData->setValue( currentPage+1 );
	insWherePageData->setDisabled( true );

	whereLayout->addWidget( insWherePageData, 1, 2 );

	whereLayout->addColSpacing(0, insCountLabel->fontMetrics().width( tr( "&Insert" )));
	dialogLayout->addLayout( whereLayout );
	templLayout = new QHBoxLayout;
	templLayout->setSpacing( 6 );
	templLayout->setMargin( 0 );

	templData = new QComboBox(true, this, "templData");
	templData->setEditable(false);
	for (QMap<QString,int>::Iterator it = currentDoc->MasterNames.begin(); it != currentDoc->MasterNames.end(); ++it)
		templData->insertItem(it.key() == "Normal" ? tr("Normal") : it.key());

	templLabel = new QLabel(templData, (facingPages ? tr("Template (&Left Page):") : tr("&Template:")), this, "text");
	QSpacerItem* spacer3 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	templLayout->addWidget( templLabel );
	templLayout->addItem( spacer3 );
	templLayout->addWidget(templData);
	dialogLayout->addLayout(templLayout);

	if (facingPages)
	{
		templ2Layout = new QHBoxLayout;
		templ2Layout->setSpacing( 6 );
		templ2Layout->setMargin( 0 );
		templ2Data = new QComboBox(false, this, "templ2Data");
		for (QMap<QString,int>::Iterator it2 = currentDoc->MasterNames.begin(); it2 != currentDoc->MasterNames.end(); ++it2)
			templ2Data->insertItem(it2.key() == "Normal" ? tr("Normal") : it2.key());
		templ2Label = new QLabel( templ2Data, tr("Template (&Right Page):"), this, "texta");
		templ2Layout->addWidget( templ2Label );
		QSpacerItem* spacer2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
		templ2Layout->addItem( spacer2 );
		templ2Layout->addWidget(templ2Data);
		dialogLayout->addLayout(templ2Layout);
	}

	okCancelLayout = new QHBoxLayout;
	okCancelLayout->setSpacing( 6 );
	okCancelLayout->setMargin( 5 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	okCancelLayout->addItem( spacer );

	okButton = new QPushButton( tr( "&OK" ), this, "okButton" );
	okButton->setDefault( true );
	okCancelLayout->addWidget( okButton );

	cancelButton = new QPushButton( tr( "&Cancel" ), this, "cancelButton" );
	okCancelLayout->addWidget( cancelButton );
	dialogLayout->addLayout( okCancelLayout );
	setMaximumSize(sizeHint());

	// signals and slots connections
	connect( insWhereData, SIGNAL( activated(int) ), this, SLOT( insWherePageDataDisable(int) ) );
	connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
}


const QString InsPage::getTemplate()
{
	return templData->currentText();
}

const QString InsPage::getTemplate2()
{
	return templ2Data->currentText();
}

const int InsPage::getWhere()
{
	return insWhereData->currentItem();
}

const int InsPage::getWherePage()
{
	return insWherePageData->value();
}

const int InsPage::getCount()
{
	return insCountData->value();
}

void InsPage::insWherePageDataDisable(int index)
{
	insWherePageData->setDisabled((index==2));
}
