#include "hysettings.h"
#include "hysettings.moc"
#include "sccombobox.h"

HySettings::HySettings( QWidget* parent, QMap<QString,QString>* langs ) : QWidget( parent, "Settings" )
{
	QStringList lada;
	layout3 = new QGridLayout( this, 1, 1, 0, 5, "Layout3");
	layout3->setAlignment( Qt::AlignTop );
	verbose = new QCheckBox( tr("&Hyphenation Suggestions"), this, "Verbose");
	layout3->addMultiCellWidget(verbose, 0, 0, 0, 1);
	
	input = new QCheckBox( tr("Hyphenate Text Automatically &During Typing"), this, "inp");
	layout3->addMultiCellWidget(input, 1, 1, 0, 1);
	
	language = new ScComboBox( true, this, "Language" );
	QMap<QString,QString>::Iterator it;
	for (it = langs->begin(); it != langs->end(); ++it)
		lada.append(it.data());
	lada.sort();
	language->insertStringList(lada);
	language->setEditable(false);
	text1 = new QLabel( language, tr( "&Language:" ), this, "Text1" );
	layout3->addWidget( text1, 2, 0 );
	layout3->addWidget( language, 2, 1, Qt::AlignLeft );
	
	wordLen = new QSpinBox( this, "WordLen" );
	wordLen->setMinValue( 3 );
	text2 = new QLabel( wordLen, tr( "&Smallest Word:" ), this, "Text2" );
	layout3->addWidget( text2, 3, 0 );
	layout3->addWidget( wordLen, 3, 1, Qt::AlignLeft );
	
	maxCount = new QSpinBox( this, "MaxCount" );
	maxCount->setMinValue( 0 );
	text3 = new QLabel(maxCount, tr("Consecutive Hyphenations &Allowed:"), this, "Text3");
	layout3->addWidget( text3, 4, 0 );
	layout3->addWidget( maxCount, 4, 1, Qt::AlignLeft);
	
	QToolTip::add( verbose, tr( "A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option." ) );
	QToolTip::add( input, tr("Enables automatic hyphenation of your text while typing."));
	QToolTip::add( wordLen, tr( "Length of the smallest word to be hyphenated." ) );
	QToolTip::add( maxCount, tr( "Maximum number of Hyphenations following each other.\nA value of 0 means unlimited hyphenations." ) );
}

