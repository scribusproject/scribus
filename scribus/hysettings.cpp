#include "hysettings.h"
#include "hysettings.moc"
#include <qpixmap.h>

extern QPixmap loadIcon(QString nam);

HySettings::HySettings( QWidget* parent, QMap<QString,QString>* langs )
		: QDialog( parent, "Settings", true, 0 )
{
	QStringList lada;
	resize( 218, 103 );
	setCaption( tr( "Hyphenator Settings" ) );
	setIcon(loadIcon("AppIcon.png"));
	HySettingsLayout = new QVBoxLayout( this, 11, 6, "HySettingsLayout");
	Layout3 = new QGridLayout( 0, 1, 1, 0, 6, "Layout3");
	Verbose = new QCheckBox(this, "Verbose");
	Verbose->setText( tr("Fully Automatic"));
	Layout3->addMultiCellWidget(Verbose, 0, 0, 0, 1);
	QToolTip::add( Verbose, tr( "If you uncheck this you will get a dialog\neverytime a possible Hyphenation is found." ) );
	Input = new QCheckBox(this, "inp");
	Input->setText( tr("Check during Typing"));
	Layout3->addMultiCellWidget(Input, 1, 1, 0, 1);
	QToolTip::add(Input, tr("Enables automatic checking of your text while typing."));
	Language = new QComboBox( true, this, "Language" );
	QMap<QString,QString>::Iterator it;
	for (it = langs->begin(); it != langs->end(); ++it)
		lada.append(it.key());
	lada.sort();
	Language->insertStringList(lada);
	Language->setEditable(false);
	Layout3->addWidget( Language, 2, 1 );
	Text1 = new QLabel( this, "Text1" );
	Text1->setText( tr( "Language:" ) );
	Layout3->addWidget( Text1, 2, 0 );
	Text2 = new QLabel( this, "Text2" );
	Text2->setText( tr( "Smallest Word:" ) );
	Layout3->addWidget( Text2, 3, 0 );
	WordLen = new QSpinBox( this, "WordLen" );
	WordLen->setMinValue( 3 );
	QToolTip::add( WordLen, tr( "Length of the smallest word to be hyphenated." ) );
	Layout3->addWidget( WordLen, 3, 1 );
	Text3 = new QLabel(this, "Text3");
	Text3->setText( tr("Number of Hypenations allowed:"));
	Layout3->addWidget( Text3, 4, 0 );
	MaxCount = new QSpinBox( this, "MaxCount" );
	QToolTip::add( MaxCount, tr( "Maximum number of Hyphenations following each other." ) );
	Layout3->addWidget( MaxCount, 4, 1);
	MaxCount->setMinValue( 0 );
	HySettingsLayout->addLayout( Layout3 );
	Layout1 = new QHBoxLayout( 0, 0, 6, "Layout1");
	QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1->addItem( spacer );
	OK = new QPushButton( this, "OK" );
	OK->setText( tr( "OK" ) );
	OK->setDefault( true );
	Layout1->addWidget( OK );
	QSpacerItem* spacer_2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1->addItem( spacer_2 );
	Cancel = new QPushButton( this, "Cancel" );
	Cancel->setText( tr( "Cancel" ) );
	Layout1->addWidget( Cancel );
	QSpacerItem* spacer_3 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1->addItem( spacer_3 );
	HySettingsLayout->addLayout( Layout1 );
	connect(OK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(Cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

