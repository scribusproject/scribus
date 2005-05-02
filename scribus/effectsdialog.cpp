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
#include <qcombobox.h>
#include <qtextstream.h>
#include <qwidgetstack.h>
#include <qwidget.h>
#include "scribusdoc.h"
#include "shadebutton.h"

extern QPixmap loadIcon(QString nam);

EffectsDialog::EffectsDialog( QWidget* parent, PageItem* item, ScribusDoc* docc ) : QDialog( parent, "EffectsDialog", true, 0 )
{
	EffectsDialogLayout = new QHBoxLayout( this, 10, 5, "EffectsDialogLayout");
	setCaption( tr( "Image Effects" ) );
	setIcon(loadIcon("AppIcon.png"));
	currItem = item;
	effectsList = currItem->effectsInUse;
	doc = docc;
	currentOptions = 0;
	bool mode = false;
	image.LoadPicture(currItem->Pfile, "", 0, false, false, 1, 72, &mode);
	int ix = image.width();
	int iy = image.height();
	if ((ix > 220) || (iy > 220))
	{
		double sx = ix / 220.0;
		double sy = iy / 220.0;
		if (sy < sx)
			image.createLowRes(sx);
		else
			image.createLowRes(sy);
	}
	layout16 = new QVBoxLayout( 0, 0, 5, "layout16");
	pixmapLabel1 = new QLabel( this, "pixmapLabel1" );
	pixmapLabel1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, pixmapLabel1->sizePolicy().hasHeightForWidth() ) );
	pixmapLabel1->setMinimumSize( QSize( 220, 220 ) );
	pixmapLabel1->setMaximumSize( QSize( 220, 220 ) );
	pixmapLabel1->setFrameShape( QLabel::StyledPanel );
	pixmapLabel1->setFrameShadow( QLabel::Sunken );
	pixmapLabel1->setScaledContents( false );
	layout16->addWidget( pixmapLabel1 );
	textLabel5 = new QLabel( this, "textLabel5" );
	textLabel5->setText( tr( "Options:" ) );
	layout16->addWidget( textLabel5 );
	optionStack = new QWidgetStack( this, "optionStack" );
	optionStack->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, optionStack->sizePolicy().hasHeightForWidth() ) );
	optionStack->setMinimumSize( QSize( 220, 80 ) );
	optionStack->setFrameShape( QWidgetStack::GroupBoxPanel );
	WStackPage = new QWidget( optionStack, "WStackPage" );
	optionStack->addWidget( WStackPage, 0 );

	WStackPage_2 = new QWidget( optionStack, "WStackPage_2" );
	WStackPageLayout = new QVBoxLayout( WStackPage_2, 5, 5, "WStackPageLayout");
	WStackPageLayout->setAlignment( Qt::AlignTop );
	layout17 = new QHBoxLayout( 0, 0, 5, "layout7");
	textLabel3 = new QLabel( tr( "Color:" ), WStackPage_2, "textLabel3" );
	layout17->addWidget( textLabel3 );

	colData = new QComboBox(false, WStackPage_2, "colData");
	ColorList::Iterator it;
	QPixmap pm = QPixmap(15, 15);
	for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
	{
		pm.fill(doc->PageColors[it.key()].getRGBColor());
		colData->insertItem(pm, it.key());
	}
	layout17->addWidget( colData );
	WStackPageLayout->addLayout( layout17 );

	layout19 = new QHBoxLayout( 0, 0, 5, "layout7");
	textLabel4 = new QLabel( tr( "Shade:" ), WStackPage_2, "textLabel4" );
	layout19->addWidget( textLabel4 );
	shade = new ShadeButton(WStackPage_2);
	shade->setValue(100);
	layout19->addWidget( shade );
	WStackPageLayout->addLayout( layout19 );
	optionStack->addWidget( WStackPage_2, 1 );
	layout16->addWidget( optionStack );
	EffectsDialogLayout->addLayout( layout16 );

	layout18 = new QVBoxLayout( 0, 0, 0, "layout18");
	layout10 = new QGridLayout( 0, 1, 1, 0, 5, "layout10");
	layout2 = new QVBoxLayout( 0, 0, 5, "layout2");
	textLabel1 = new QLabel( this, "textLabel1" );
	textLabel1->setText( tr( "Available Effects" ) );
	layout2->addWidget( textLabel1 );
	availableEffects = new QListBox( this, "availableEffects" );
	availableEffects->clear();
	availableEffects->insertItem( tr("Invert"));
	availableEffects->insertItem( tr("Grayscale"));
	availableEffects->insertItem( tr("Colorize"));
	availableEffects->setMinimumSize(fontMetrics().width(tr( "Available Effects" ))+40, 180);
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
	usedEffects->setMinimumSize(fontMetrics().width(tr( "Available Effects" ))+40, 180);
	usedEffects->clear();
	effectValMap.clear();
	for (uint a = 0; a < effectsList.count(); ++a)
	{
		if ((*effectsList.at(a)).effectCode == 0)
		{
			usedEffects->insertItem( tr("Invert"));
			effectValMap.insert(usedEffects->item(usedEffects->count()-1), "");
		}
		if ((*effectsList.at(a)).effectCode == 1)
		{
			usedEffects->insertItem( tr("Grayscale"));
			effectValMap.insert(usedEffects->item(usedEffects->count()-1), "");
		}
		if ((*effectsList.at(a)).effectCode == 2)
		{
			usedEffects->insertItem( tr("Colorize"));
			effectValMap.insert(usedEffects->item(usedEffects->count()-1), (*effectsList.at(a)).effectParameters);
		}
	}
	layout8->addWidget( usedEffects );
	layout7 = new QHBoxLayout( 0, 0, 5, "layout7");
	QSpacerItem* spacer4 = new QSpacerItem( 10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout7->addItem( spacer4 );
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
	QSpacerItem* spacer5 = new QSpacerItem( 10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout7->addItem( spacer5 );
	layout8->addLayout( layout7 );
	layout10->addLayout( layout8, 0, 2 );
	layout18->addLayout( layout10 );

	layout9 = new QHBoxLayout( 0, 0, 5, "layout9");
	QSpacerItem* spacer3 = new QSpacerItem( 111, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout9->addItem( spacer3 );

	okButton = new QPushButton( this, "okButton" );
	okButton->setText( tr( "OK" ) );
	layout9->addWidget( okButton );

	cancelButton = new QPushButton( this, "cancelButton" );
	cancelButton->setText( tr( "Cancel" ) );
	layout9->addWidget( cancelButton );
	layout18->addLayout( layout9 );
	EffectsDialogLayout->addLayout( layout18 );
	optionStack->raiseWidget(0);
	createPreview();
	resize( minimumSizeHint() );
	clearWState( WState_Polished );

	// signals and slots connections
	connect( okButton, SIGNAL( clicked() ), this, SLOT( leaveOK() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( usedEffects, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( selectEffect(QListBoxItem*) ) );
	connect( availableEffects, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( selectAvailEffect(QListBoxItem*) ) );
	connect( toEffects, SIGNAL( clicked() ), this, SLOT( moveToEffects() ) );
	connect( fromEffects, SIGNAL( clicked() ), this, SLOT( moveFromEffects() ) );
	connect( effectUp, SIGNAL( clicked() ), this, SLOT( moveEffectUp() ) );
	connect( effectDown, SIGNAL( clicked() ), this, SLOT( moveEffectDown() ) );
	connect( colData, SIGNAL(activated(int)), this, SLOT( createPreview()));
	connect( shade, SIGNAL(clicked()), this, SLOT(createPreview()));
}

void EffectsDialog::leaveOK()
{
	saveValues();
	accept();
}

void EffectsDialog::createPreview()
{
	ScImage im = image.copy();
	saveValues();
	im.applyEffect(effectsList, doc->PageColors, false);
	pixmapLabel1->setPixmap( im );
}

void EffectsDialog::saveValues()
{
	if (currentOptions != 0)
	{
		if (currentOptions->text() == tr("Colorize"))
		{
			QString efval = "";
			efval = colData->currentText();
			QString tmp;
			tmp.setNum(shade->getValue());
			efval += " "+tmp;
			effectValMap[currentOptions] = efval;
		}
	}
	effectsList.clear();
	struct ScImage::imageEffect ef;
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
		if (usedEffects->item(e)->text() == tr("Colorize"))
		{
			ef.effectCode = 2;
			ef.effectParameters = effectValMap[usedEffects->item(e)];
			effectsList.append(ef);
		}
	}
}

void EffectsDialog::moveToEffects()
{
	usedEffects->insertItem(availableEffects->currentText());
	if (availableEffects->currentText() == tr("Invert"))
		effectValMap.insert(usedEffects->item(usedEffects->count()-1), "");
	if (availableEffects->currentText() == tr("Grayscale"))
		effectValMap.insert(usedEffects->item(usedEffects->count()-1), "");
	if (availableEffects->currentText() == tr("Colorize"))
	{
		ColorList::Iterator it;
		it = doc->PageColors.begin();
		QString efval = it.key()+" 100";
		effectValMap.insert(usedEffects->item(usedEffects->count()-1), efval);
	}
	disconnect( usedEffects, SIGNAL( selected(QListBoxItem*) ), this, SLOT( selectEffect(QListBoxItem*) ) );
	selectEffect(usedEffects->item(usedEffects->count()-1));
	usedEffects->setCurrentItem(usedEffects->item(usedEffects->count()-1));
	connect( usedEffects, SIGNAL( selected(QListBoxItem*) ), this, SLOT( selectEffect(QListBoxItem*) ) );
	createPreview();
}

void EffectsDialog::moveFromEffects()
{
	effectValMap.remove(usedEffects->item(usedEffects->currentItem()));
	usedEffects->removeItem(usedEffects->currentItem());
	currentOptions = 0;
	if (usedEffects->count() == 0)
		fromEffects->setEnabled(false);
	if (usedEffects->count() < 2)
	{
		effectUp->setEnabled(false);
		effectDown->setEnabled(false);
	}
	selectEffect(usedEffects->item(usedEffects->currentItem()));
	createPreview();
}

void EffectsDialog::moveEffectUp()
{
	int curr = usedEffects->currentItem();
	if (curr == 0)
		return;
	disconnect( usedEffects, SIGNAL( selected(QListBoxItem*) ), this, SLOT( selectEffect(QListBoxItem*) ) );
	usedEffects->clearSelection();
	QListBoxItem *it = usedEffects->item(curr);
	usedEffects->takeItem(it);
	usedEffects->insertItem(it, curr-1);
	usedEffects->setCurrentItem(it);
	selectEffect(usedEffects->item(usedEffects->currentItem()));
	createPreview();
	connect( usedEffects, SIGNAL( selected(QListBoxItem*) ), this, SLOT( selectEffect(QListBoxItem*) ) );
}

void EffectsDialog::moveEffectDown()
{
	int curr = usedEffects->currentItem();
	if (curr == static_cast<int>(usedEffects->count())-1)
		return;
	disconnect( usedEffects, SIGNAL( selected(QListBoxItem*) ), this, SLOT( selectEffect(QListBoxItem*) ) );
	usedEffects->clearSelection();
	QListBoxItem *it = usedEffects->item(curr);
	usedEffects->takeItem(it);
	usedEffects->insertItem(it, curr+1);
	usedEffects->setCurrentItem(it);
	selectEffect(usedEffects->item(usedEffects->currentItem()));
	createPreview();
	connect( usedEffects, SIGNAL( selected(QListBoxItem*) ), this, SLOT( selectEffect(QListBoxItem*) ) );
}

void EffectsDialog::selectEffect(QListBoxItem* c)
{
	toEffects->setEnabled(false);
	if (currentOptions != 0)
	{
		if (currentOptions->text() == tr("Colorize"))
		{
			QString efval = "";
			efval = colData->currentText();
			QString tmp;
			tmp.setNum(shade->getValue());
			efval += " "+tmp;
			effectValMap[currentOptions] = efval;
		}
	}
	if (c)
	{
		fromEffects->setEnabled(true);
		if (usedEffects->count() > 1)
		{
			effectUp->setEnabled(true);
			effectDown->setEnabled(true);
			if (usedEffects->currentItem() == 0)
				effectUp->setEnabled(false);
			if (usedEffects->currentItem() == static_cast<int>(usedEffects->count())-1)
				effectDown->setEnabled(false);
		}
		if (c->text() == tr("Grayscale"))
			optionStack->raiseWidget(0);
		else if (c->text() == tr("Invert"))
			optionStack->raiseWidget(0);
		else if (c->text() == tr("Colorize"))
		{
			disconnect( colData, SIGNAL(activated(int)), this, SLOT( createPreview()));
			disconnect( shade, SIGNAL(clicked()), this, SLOT(createPreview()));
			QString tmpstr = effectValMap[c];
			QString col;
			int shading;
			QTextStream fp(&tmpstr, IO_ReadOnly);
			fp >> col;
			fp >> shading;
			colData->setCurrentText(col);
			shade->setValue(shading);
			optionStack->raiseWidget(1);
			connect( colData, SIGNAL(activated(int)), this, SLOT( createPreview()));
			connect( shade, SIGNAL(clicked()), this, SLOT(createPreview()));
		}
		else
			optionStack->raiseWidget(0);
		currentOptions = c;
	}
	else
		optionStack->raiseWidget(0);
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
	disconnect( usedEffects, SIGNAL( selected(QListBoxItem*) ), this, SLOT( selectEffect(QListBoxItem*) ) );
	if (currentOptions != 0)
	{
		if (currentOptions->text() == tr("Colorize"))
		{
			QString efval = "";
			efval = colData->currentText();
			QString tmp;
			tmp.setNum(shade->getValue());
			efval += " "+tmp;
			effectValMap[currentOptions] = efval;
		}
	}
	currentOptions = 0;
	usedEffects->clearSelection();
	optionStack->raiseWidget(0);
	connect( usedEffects, SIGNAL( selected(QListBoxItem*) ), this, SLOT( selectEffect(QListBoxItem*) ) );
}

