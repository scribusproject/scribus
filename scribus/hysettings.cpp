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
	Verbose = new QCheckBox(tr("&Hyphenation Suggestions"), this, "Verbose");
	Layout3->addMultiCellWidget(Verbose, 0, 0, 0, 1);
	
	Input = new QCheckBox(tr("Hyphenate Text Automatically &During Typing"), this, "inp");
	Layout3->addMultiCellWidget(Input, 1, 1, 0, 1);
	
	Language = new QComboBox( true, this, "Language" );
	QMap<QString,QString>::Iterator it;
	for (it = langs->begin(); it != langs->end(); ++it)
		lada.append(it.data());
	lada.sort();
	Language->insertStringList(lada);
	Language->setEditable(false);
	Text1 = new QLabel( Language, tr( "&Language:" ), this, "Text1" );
	Layout3->addWidget( Text1, 2, 0 );
	Layout3->addWidget( Language, 2, 1 );
	
	WordLen = new QSpinBox( this, "WordLen" );
	WordLen->setMinValue( 3 );
	Text2 = new QLabel( WordLen, tr( "&Smallest Word:" ), this, "Text2" );
	Layout3->addWidget( Text2, 3, 0 );
	Layout3->addWidget( WordLen, 3, 1 );
	
	MaxCount = new QSpinBox( this, "MaxCount" );
	MaxCount->setMinValue( 0 );
	Text3 = new QLabel(MaxCount, tr("Consecutive Hyphenations &Allowed:"), this, "Text3");
	Layout3->addWidget( Text3, 4, 0 );
	Layout3->addWidget( MaxCount, 4, 1);
	
	HySettingsLayout->addLayout( Layout3 );
	Layout1 = new QHBoxLayout( 0, 0, 6, "Layout1");
	QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1->addItem( spacer );
	OK = new QPushButton( tr( "&OK" ), this, "OK" );
	OK->setDefault( true );
	Layout1->addWidget( OK );
	Cancel = new QPushButton( tr( "&Cancel" ), this, "Cancel" );
	Layout1->addWidget( Cancel );
	HySettingsLayout->addLayout( Layout1 );
	
	QToolTip::add( Verbose, tr( "A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option." ) );
	QToolTip::add( Input, tr("Enables automatic hyphenation of your text while typing."));
	QToolTip::add( WordLen, tr( "Length of the smallest word to be hyphenated." ) );
	QToolTip::add( MaxCount, tr( "Maximum number of Hyphenations following each other.\nA value of 0 means unlimited hyphenations." ) );
	connect(OK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(Cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

