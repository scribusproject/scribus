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
	insCountQSBox = new QSpinBox( this, "insCountQSBox" );
	insCountQSBox->setMinValue( 1 );
	insCountQSBox->setValue( 1 );
	insCountQLabel = new QLabel( insCountQSBox, tr( "&Insert" ), this, "insCountQLabel" );
	whereLayout->addWidget( insCountQLabel, 0, 0 );
	whereLayout->addWidget( insCountQSBox, 0, 1 );
	pagesQLabel = new QLabel( tr( "Page(s)" ), this, "pagesQLabel" );
	whereLayout->addWidget( pagesQLabel, 0, 2 );

	insWhereQCBox = new QComboBox( false, this, "insWhereQCBox" );
	insWhereQCBox->insertItem( tr("before Page"));
	insWhereQCBox->insertItem( tr("after Page"));
	insWhereQCBox->insertItem( tr("at End"));
	insWhereQCBox->setCurrentItem(2);
	whereLayout->addMultiCellWidget( insWhereQCBox, 1, 1, 0, 1 );

	insWherePageQSBox = new QSpinBox( 1, maxPages, 1, this, "insWherePageQSBox" );
	insWherePageQSBox->setValue( currentPage+1 );
	insWherePageQSBox->setDisabled( true );

	whereLayout->addWidget( insWherePageQSBox, 1, 2 );

	whereLayout->addColSpacing(0, insCountQLabel->fontMetrics().width( tr( "&Insert" )));
	dialogLayout->addLayout( whereLayout );
	templLayout = new QHBoxLayout;
	templLayout->setSpacing( 6 );
	templLayout->setMargin( 0 );

	templQCBox = new QComboBox(true, this, "templQCBox");
	templQCBox->setEditable(false);
	for (QMap<QString,int>::Iterator it = currentDoc->MasterNames.begin(); it != currentDoc->MasterNames.end(); ++it)
		templQCBox->insertItem(it.key() == "Normal" ? tr("Normal") : it.key());

	templQLabel = new QLabel(templQCBox, "", this, "text");
	templQLabel->setText( facingPages ? tr("Template (&Left Page):") : tr("&Template:"));
	QSpacerItem* spacer3 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	templLayout->addWidget( templQLabel );
	templLayout->addItem( spacer3 );
	templLayout->addWidget(templQCBox);
	dialogLayout->addLayout(templLayout);

	if (facingPages)
	{
		templ2Layout = new QHBoxLayout;
		templ2Layout->setSpacing( 6 );
		templ2Layout->setMargin( 0 );
		templ2QCBox = new QComboBox(false, this, "templ2QCBox");
		for (QMap<QString,int>::Iterator it2 = currentDoc->MasterNames.begin(); it2 != currentDoc->MasterNames.end(); ++it2)
			templ2QCBox->insertItem(it2.key() == "Normal" ? tr("Normal") : it2.key());
		templ2QLabel = new QLabel( templ2QCBox, tr("Template (&Right Page):"), this, "texta");
		templ2Layout->addWidget( templ2QLabel );
		QSpacerItem* spacer2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
		templ2Layout->addItem( spacer2 );
		templ2Layout->addWidget(templ2QCBox);
		dialogLayout->addLayout(templ2Layout);
	}

	okCancelLayout = new QHBoxLayout;
	okCancelLayout->setSpacing( 6 );
	okCancelLayout->setMargin( 5 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	okCancelLayout->addItem( spacer );

	okQPButton = new QPushButton( tr( "&OK" ), this, "okQPButton" );
	okQPButton->setDefault( true );
	okCancelLayout->addWidget( okQPButton );

	cancelQPButton = new QPushButton( tr( "&Cancel" ), this, "cancelQPButton" );
	okCancelLayout->addWidget( cancelQPButton );
	dialogLayout->addLayout( okCancelLayout );
	setMaximumSize(sizeHint());

	// signals and slots connections
	connect( insWhereQCBox, SIGNAL( activated(int) ), this, SLOT( insWherePageQSBoxDisable(int) ) );
	connect( okQPButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( cancelQPButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
}


const QString InsPage::getTemplate()
{
	return templQCBox->currentText();
}

const QString InsPage::getTemplate2()
{
	return templ2QCBox->currentText();
}

const int InsPage::getWhere()
{
	return insWhereQCBox->currentItem();
}

const int InsPage::getWherePage()
{
	return insWherePageQSBox->value();
}

const int InsPage::getCount()
{
	return insCountQSBox->value();
}

void InsPage::insWherePageQSBoxDisable(int index)
{
	insWherePageQSBox->setDisabled((index==2));
}
