#include "effectsdialog.h"
#include "effectsdialog.moc"
#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
extern QPixmap loadIcon(QString nam);

EffectsDialog::EffectsDialog( QWidget* parent, QValueList<PageItem::imageEffect> eList ) : QDialog( parent, "EffectsDialog", true, 0 )
{
	EffectsDialogLayout = new QVBoxLayout( this, 10, 5, "EffectsDialogLayout");
	setCaption( tr( "Image Effects" ) );
	setIcon(loadIcon("AppIcon.png"));
	effectsList = eList;
	layout10 = new QGridLayout( 0, 1, 1, 0, 5, "layout10");

	layout2 = new QVBoxLayout( 0, 0, 5, "layout2");
	textLabel1 = new QLabel( this, "textLabel1" );
	textLabel1->setText( tr( "Available Efffects" ) );
	layout2->addWidget( textLabel1 );
	availableEffects = new QListBox( this, "availableEffects" );
	availableEffects->clear();
	availableEffects->insertItem( tr("Invert"));
	availableEffects->insertItem( tr("Grayscale"));
	layout2->addWidget( availableEffects );
	layout10->addLayout( layout2, 0, 0 );

	layout1 = new QVBoxLayout( 0, 0, 5, "layout1");
	QSpacerItem* spacer1 = new QSpacerItem( 21, 61, QSizePolicy::Minimum, QSizePolicy::Expanding );
	layout1->addItem( spacer1 );
	toEffects = new QPushButton( this, "toEffects" );
	toEffects->setText( tr( ">>" ) );
	toEffects->setEnabled(false);
	layout1->addWidget( toEffects );
	fromEffects = new QPushButton( this, "fromEffects" );
	fromEffects->setText( tr( "<<" ) );
	fromEffects->setEnabled(false);
	layout1->addWidget( fromEffects );
	QSpacerItem* spacer2 = new QSpacerItem( 31, 81, QSizePolicy::Minimum, QSizePolicy::Expanding );
	layout1->addItem( spacer2 );
	layout10->addLayout( layout1, 0, 1 );

	layout8 = new QVBoxLayout( 0, 0, 5, "layout8");
	textLabel2 = new QLabel( this, "textLabel2" );
	textLabel2->setText( tr( "Effects in use" ) );
	layout8->addWidget( textLabel2 );
	usedEffects = new QListBox( this, "usedEffects" );
	usedEffects->clear();
	for (uint a = 0; a < effectsList.count(); ++a)
	{
		if ((*effectsList.at(a)).effectCode == 0)
			usedEffects->insertItem( tr("Invert"));
		if ((*effectsList.at(a)).effectCode == 1)
			usedEffects->insertItem( tr("Grayscale"));
	}
	layout8->addWidget( usedEffects );
	layout7 = new QHBoxLayout( 0, 0, 5, "layout7");
	effectUp = new QPushButton( this, "effectUp" );
	effectUp->setText( "" );
	effectUp->setPixmap(loadIcon("Raiselayer.png"));
	effectUp->setEnabled(false);
	layout7->addWidget( effectUp );
	effectDown = new QPushButton( this, "effectDown" );
	effectDown->setText( "" );
	effectDown->setPixmap(loadIcon("Lowerlayer.png"));
	effectDown->setEnabled(false);
	layout7->addWidget( effectDown );
	effectOptions = new QPushButton( this, "effectOptions" );
	effectOptions->setText( tr( "Options" ) );
	effectOptions->setEnabled(false);
	layout7->addWidget( effectOptions );
	layout8->addLayout( layout7 );
	layout10->addLayout( layout8, 0, 2 );

	EffectsDialogLayout->addLayout( layout10 );

	layout9 = new QHBoxLayout( 0, 0, 5, "layout9");
	QSpacerItem* spacer3 = new QSpacerItem( 111, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout9->addItem( spacer3 );

	okButton = new QPushButton( this, "okButton" );
	okButton->setText( tr( "OK" ) );
	layout9->addWidget( okButton );

	cancelButton = new QPushButton( this, "cancelButton" );
	cancelButton->setText( tr( "Cancel" ) );
	layout9->addWidget( cancelButton );
	EffectsDialogLayout->addLayout( layout9 );
	resize( QSize(495, 278).expandedTo(minimumSizeHint()) );
	clearWState( WState_Polished );

	// signals and slots connections
	connect( okButton, SIGNAL( clicked() ), this, SLOT( saveValues() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( usedEffects, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( selectEffect(QListBoxItem*) ) );
	connect( availableEffects, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( selectAvailEffect(QListBoxItem*) ) );
	connect( toEffects, SIGNAL( clicked() ), this, SLOT( moveToEffects() ) );
	connect( fromEffects, SIGNAL( clicked() ), this, SLOT( moveFromEffects() ) );
	connect( effectUp, SIGNAL( clicked() ), this, SLOT( moveEffectUp() ) );
	connect( effectDown, SIGNAL( clicked() ), this, SLOT( moveEffectDown() ) );
	connect( effectOptions, SIGNAL( clicked() ), this, SLOT( editEffect() ) );
}

void EffectsDialog::saveValues()
{
	accept();
	effectsList.clear();
	struct PageItem::imageEffect ef;
	for (uint e = 0; e < usedEffects->count(); ++e)
	{
		if (usedEffects->item(e)->text() == tr("Invert"))
		{
			ef.effectCode = 0;
			ef.effectParameters = "";
			effectsList.append(ef);
		}
		if (usedEffects->item(e)->text() == tr("Grayscale"))
		{
			ef.effectCode = 1;
			ef.effectParameters = "";
			effectsList.append(ef);
		}
	}
}

void EffectsDialog::moveToEffects()
{
	usedEffects->insertItem(availableEffects->currentText());
}

void EffectsDialog::moveFromEffects()
{
	usedEffects->removeItem(usedEffects->currentItem());
	if (usedEffects->count() == 0)
	{
		fromEffects->setEnabled(false);
		effectUp->setEnabled(false);
		effectDown->setEnabled(false);
		effectOptions->setEnabled(false);
	}
}

void EffectsDialog::moveEffectUp()
{
	qWarning( "EffectsDialog::moveEffectUp(): Not implemented yet" );
}

void EffectsDialog::moveEffectDown()
{
	qWarning( "EffectsDialog::moveEffectDown(): Not implemented yet" );
}

void EffectsDialog::editEffect()
{
	qWarning( "EffectsDialog::editEffect(): Not implemented yet" );
}

void EffectsDialog::selectEffect(QListBoxItem* c)
{
	toEffects->setEnabled(false);
	if (c)
	{
		fromEffects->setEnabled(true);
		effectUp->setEnabled(true);
		effectDown->setEnabled(true);
		if (c->text() == tr("Grayscale"))
			effectOptions->setEnabled(false);
		else if (c->text() == tr("Invert"))
			effectOptions->setEnabled(false);
		else
			effectOptions->setEnabled(true);
	}
	disconnect( availableEffects, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( selectAvailEffect(QListBoxItem*) ) );
	availableEffects->clearSelection();
	connect( availableEffects, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( selectAvailEffect(QListBoxItem*) ) );
}

void EffectsDialog::selectAvailEffect(QListBoxItem* c)
{
	if (c)
		toEffects->setEnabled(true);
	fromEffects->setEnabled(false);
	effectUp->setEnabled(false);
	effectDown->setEnabled(false);
	effectOptions->setEnabled(false);
	disconnect( usedEffects, SIGNAL( selected(QListBoxItem*) ), this, SLOT( selectEffect(QListBoxItem*) ) );
	usedEffects->clearSelection();
	connect( usedEffects, SIGNAL( selected(QListBoxItem*) ), this, SLOT( selectEffect(QListBoxItem*) ) );
}

