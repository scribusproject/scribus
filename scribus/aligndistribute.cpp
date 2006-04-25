/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
	begin                : June 2005
	copyright            : (C) 2005 by Craig Bradney
	email                : cbradney@zip.com.au
***************************************************************************/

/***************************************************************************
*                                                                         *
*   ScMW program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "aligndistribute.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qlineedit.h>

#include "aligndistribute.moc"
#include "page.h"
#include "scmessagebox.h"
//#include "scribus.h"
#include "scribusdoc.h"
#include "selection.h"
#include "undomanager.h"
#include "mspinbox.h"
#include "units.h"
#include "commonstrings.h"

extern QPixmap loadIcon(QString nam);

//TODO Distribute with 
//TODO Handle locked items

/*
 *  Constructs a AlignDistributePalette as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
AlignDistributePalette::AlignDistributePalette( QWidget* parent, const char* name, bool modal, WFlags fl )
	: ScrPaletteBase( parent, name, modal, fl )
{
	if ( !name )
		setName( "AlignDistributePalette" );
	currDoc=NULL;
	AlignDistributePaletteLayout = new QVBoxLayout( this, 5, 6, "AlignDistributePaletteLayout"); 

	alignGroupBox = new QGroupBox( this, "alignGroupBox" );
	alignGroupBox->setColumnLayout(0, Qt::Vertical );
	alignGroupBox->layout()->setSpacing( 6 );
	alignGroupBox->layout()->setMargin( 11 );
	alignGroupBoxLayout = new QGridLayout( alignGroupBox->layout() );
	//alignGroupBoxLayout = new QVBoxLayout( alignGroupBox->layout() );
	alignGroupBoxLayout->setAlignment( Qt::AlignTop );

	layout11 = new QHBoxLayout( 0, 0, 6, "layout11"); 

	alignRelativeToLabel = new QLabel( alignGroupBox, "alignRelativeToLabel" );
	layout11->addWidget( alignRelativeToLabel );

	alignRelativeToCombo = new QComboBox( false, alignGroupBox, "alignRelativeToCombo" );
	layout11->addWidget( alignRelativeToCombo );
	alignGroupBoxLayout->addLayout( layout11, 0, 0 );

	layout14 = new QHBoxLayout( 0, 0, 6, "layout14"); 
	spacer15 = new QSpacerItem( 21, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout14->addItem( spacer15 );

	layout2 = new QGridLayout( 0, 1, 1, 0, 6, "layout2"); 

	alignLeftOutToolButton = new QToolButton( alignGroupBox, "alignLeftOutToolButton" );
	layout2->addWidget( alignLeftOutToolButton, 0, 0 );

	alignRightOutToolButton = new QToolButton( alignGroupBox, "alignRightOutToolButton" );
	layout2->addWidget( alignRightOutToolButton, 0, 4 );

	alignBottomInToolButton = new QToolButton( alignGroupBox, "alignBottomInToolButton" );
	layout2->addWidget( alignBottomInToolButton, 1, 3 );

	alignRightInToolButton = new QToolButton( alignGroupBox, "alignRightInToolButton" );
	layout2->addWidget( alignRightInToolButton, 0, 3 );

	alignBottomOutToolButton = new QToolButton( alignGroupBox, "alignBottomOutToolButton" );
	layout2->addWidget( alignBottomOutToolButton, 1, 4 );

	alignCenterHorToolButton = new QToolButton( alignGroupBox, "alignCenterHorToolButton" );
	layout2->addWidget( alignCenterHorToolButton, 0, 2 );

	alignLeftInToolButton = new QToolButton( alignGroupBox, "alignLeftInToolButton" );
	layout2->addWidget( alignLeftInToolButton, 0, 1 );

	alignCenterVerToolButton = new QToolButton( alignGroupBox, "alignCenterVerToolButton" );
	layout2->addWidget( alignCenterVerToolButton, 1, 2 );

	alignTopOutToolButton = new QToolButton( alignGroupBox, "alignTopOutToolButton" );
	layout2->addWidget( alignTopOutToolButton, 1, 0 );

	alignTopInToolButton = new QToolButton( alignGroupBox, "alignTopInToolButton" );
	layout2->addWidget( alignTopInToolButton, 1, 1 );
	layout14->addLayout( layout2 );
	spacer16 = new QSpacerItem( 21, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout14->addItem( spacer16 );
	alignGroupBoxLayout->addLayout( layout14, 1, 0 );
	
	alignGuideLayout = new QHBoxLayout( 0, 0, 6, "alignGuideLayout"); 
	alignGuideLeftSpacer = new QSpacerItem( 20, 10, QSizePolicy::Expanding, QSizePolicy::Minimum );
	alignGuideLayout->addItem( alignGuideLeftSpacer );
	alignGuideLineEdit = new QLineEdit( "", alignGroupBox, "alignGuideLineEdit");
	alignGuideLineEdit->setMinimumSize( QSize( 80, 20 ) );
	alignGuideLineEdit->setReadOnly(true);
	alignGuideLabel = new QLabel( alignGuideLineEdit, "", alignGroupBox, "alignGuideLabel");
	alignGuideLayout->addWidget( alignGuideLabel );

	alignGuideLayout->addWidget( alignGuideLineEdit );
	alignGuideRightSpacer = new QSpacerItem( 20, 10, QSizePolicy::Expanding, QSizePolicy::Minimum );
	alignGuideLayout->addItem( alignGuideRightSpacer);
	alignGroupBoxLayout->addLayout( alignGuideLayout, 2, 0 );
	
	AlignDistributePaletteLayout->addWidget( alignGroupBox );

	distributeGroupBox = new QGroupBox( this, "distributeGroupBox" );
	distributeGroupBox->setColumnLayout(0, Qt::Vertical );
	distributeGroupBox->layout()->setSpacing( 6 );
	distributeGroupBox->layout()->setMargin( 11 );
	distributeGroupBoxLayout = new QGridLayout( distributeGroupBox->layout() );
	distributeGroupBoxLayout->setAlignment( Qt::AlignTop );

	layout4 = new QHBoxLayout( 0, 0, 6, "layout4"); 
	distributeLeftSpacer = new QSpacerItem( 35, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout4->addItem( distributeLeftSpacer );

	layout1 = new QGridLayout( 0, 1, 1, 0, 6, "layout1"); 

	distributeDistHToolButton = new QToolButton( distributeGroupBox, "distributeDistHToolButton" );
	layout1->addWidget( distributeDistHToolButton, 0, 3 );
	
	distributeDistValueHToolButton = new QToolButton( distributeGroupBox, "distributeDistValueHToolButton" );
	layout1->addWidget( distributeDistValueHToolButton, 0, 4 );

	distributeRightToolButton = new QToolButton( distributeGroupBox, "distributeRightToolButton" );
	layout1->addWidget( distributeRightToolButton, 0, 2 );

	distributeBottomToolButton = new QToolButton( distributeGroupBox, "distributeBottomToolButton" );
	layout1->addWidget( distributeBottomToolButton, 1, 0 );

	distributeCenterHToolButton = new QToolButton( distributeGroupBox, "distributeCenterHToolButton" );
	layout1->addWidget( distributeCenterHToolButton, 0, 1 );

	distributeDistVToolButton = new QToolButton( distributeGroupBox, "distributeDistVToolButton" );
	layout1->addWidget( distributeDistVToolButton, 1, 3 );
	
	distributeDistValueVToolButton = new QToolButton( distributeGroupBox, "distributeDistValueVToolButton" );
	layout1->addWidget( distributeDistValueVToolButton, 1, 4 );

	distributeLeftToolButton = new QToolButton( distributeGroupBox, "distributeLeftToolButton" );
	layout1->addWidget( distributeLeftToolButton, 0, 0 );

	distributeCenterVToolButton = new QToolButton( distributeGroupBox, "distributeCenterVToolButton" );
	layout1->addWidget( distributeCenterVToolButton, 1, 1 );

	distributeTopToolButton = new QToolButton( distributeGroupBox, "distributeTopToolButton" );
	layout1->addWidget( distributeTopToolButton, 1, 2 );
	layout4->addLayout( layout1 );
	distributeRightSpacer = new QSpacerItem( 35, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout4->addItem( distributeRightSpacer );

	distributeGroupBoxLayout->addLayout( layout4, 0, 0 );
	
	distanceLayout = new QHBoxLayout( 0, 0, 6, "distanceLayout"); 
	distributeDistLeftSpacer = new QSpacerItem( 20, 10, QSizePolicy::Expanding, QSizePolicy::Minimum );
	distanceLayout->addItem( distributeDistLeftSpacer );
	distributeDistLabel = new QLabel( distributeGroupBox, "distributeDistLabel");
	distanceLayout->addWidget( distributeDistLabel );
	distributeDistMSpinBox = new MSpinBox( -1000, 1000, distributeGroupBox, 2);
	distributeDistMSpinBox->setMinimumSize( QSize( 80, 20 ) );
	distributeDistMSpinBox->setValue(0);
	distanceLayout->addWidget( distributeDistMSpinBox );
	distributeDistRightSpacer = new QSpacerItem( 20, 10, QSizePolicy::Expanding, QSizePolicy::Minimum );
	distanceLayout->addItem( distributeDistRightSpacer);
	distributeGroupBoxLayout->addLayout( distanceLayout, 1, 0 );
	
	AlignDistributePaletteLayout->addWidget( distributeGroupBox );
	languageChange();

	resize( QSize(222, 232).expandedTo(minimumSizeHint()) );
	clearWState( WState_Polished );

	// buddies
	alignRelativeToLabel->setBuddy( alignRelativeToCombo );
	distributeDistLabel->setBuddy( distributeDistMSpinBox );
	init();
	setDoc(NULL);
}

/*
*  Destroys the object and frees any allocated resources
*/
AlignDistributePalette::~AlignDistributePalette()
{
	// no need to delete child widgets, Qt does it all for us
}

/*
*  Sets the strings of the subwidgets using the current
*  language.
*/
void AlignDistributePalette::languageChange()
{
	setCaption( tr( "Align and Distribute" ) );
	alignGroupBox->setTitle( tr( "Align" ) );
	alignRelativeToLabel->setText( tr( "&Relative to:" ) );
	int alignComboValue=alignRelativeToCombo->currentItem();
	alignRelativeToCombo->clear();
	alignRelativeToCombo->insertItem( tr( "First Selected" ) );
	alignRelativeToCombo->insertItem( tr( "Last Selected" ) );
	alignRelativeToCombo->insertItem( tr( "Page" ) );
	alignRelativeToCombo->insertItem( tr( "Margins" ) );
	alignRelativeToCombo->insertItem( tr( "Guide" ) );		
	alignRelativeToCombo->insertItem( tr( "Selection" ) );
	alignRelativeToCombo->setCurrentItem(alignComboValue);
	alignToChanged(alignComboValue);
	alignLeftOutToolButton->setText( QString::null );
	QToolTip::add( alignLeftOutToolButton, tr( "Align right sides of objects to left side of anchor" ) );
	alignRightOutToolButton->setText( QString::null );
	QToolTip::add( alignRightOutToolButton, tr( "Align left sides of objects to right side of anchor" ) );
	alignBottomInToolButton->setText( QString::null );
	QToolTip::add( alignBottomInToolButton, tr( "Align bottoms" ) );
	alignRightInToolButton->setText( QString::null );
	QToolTip::add( alignRightInToolButton, tr( "Align right sides" ) );
	alignBottomOutToolButton->setText( QString::null );
	QToolTip::add( alignBottomOutToolButton, tr( "Align tops of objects to bottom of anchor" ) );
	alignCenterHorToolButton->setText( QString::null );
	QToolTip::add( alignCenterHorToolButton, tr( "Center on vertical axis" ) );
	alignLeftInToolButton->setText( QString::null );
	QToolTip::add( alignLeftInToolButton, tr( "Align left sides" ) );
	alignCenterVerToolButton->setText( QString::null );
	QToolTip::add( alignCenterVerToolButton, tr( "Center on horizontal axis" ) );
	alignTopOutToolButton->setText( QString::null );
	QToolTip::add( alignTopOutToolButton, tr( "Align bottoms of objects to top of anchor" ) );
	alignTopInToolButton->setText( QString::null );
	QToolTip::add( alignTopInToolButton, tr( "Align tops" ) );
	
	alignGuideLabel->setText( tr( "&Selected Guide:" ) );
	
	distributeGroupBox->setTitle( tr( "Distribute" ) );
	distributeDistHToolButton->setText( QString::null );
	QToolTip::add( distributeDistHToolButton, tr( "Make horizontal gaps between objects equal" ) );
	distributeDistValueHToolButton->setText( QString::null );
	QToolTip::add( distributeDistValueHToolButton, tr( "Make horizontal gaps between objects equal to the value specified" ) );
	distributeRightToolButton->setText( QString::null );
	distributeRightToolButton->setTextLabel( tr( "Distribute right sides equidistantly" ) );
	distributeBottomToolButton->setText( QString::null );
	QToolTip::add( distributeBottomToolButton, tr( "Distribute bottoms equidistantly" ) );
	distributeCenterHToolButton->setText( QString::null );
	QToolTip::add( distributeCenterHToolButton, tr( "Distribute centers equidistantly horizontally" ) );
	distributeDistVToolButton->setText( QString::null );
	QToolTip::add( distributeDistVToolButton, tr( "Make vertical gaps between objects equal" ) );
	distributeDistValueVToolButton->setText( QString::null );
	QToolTip::add( distributeDistValueVToolButton, tr( "Make vertical gaps between objects equal to the value specified" ) );
	distributeLeftToolButton->setText( QString::null );
	QToolTip::add( distributeLeftToolButton, tr( "Distribute left sides equidistantly" ) );
	distributeCenterVToolButton->setText( QString::null );
	QToolTip::add( distributeCenterVToolButton, tr( "Distribute centers equidistantly vertically" ) );
	distributeTopToolButton->setText( QString::null );
	QToolTip::add( distributeTopToolButton, tr( "Distribute tops equidistantly" ) );
	
	distributeDistLabel->setText( tr( "&Distance:" ) );
	QToolTip::add( distributeDistMSpinBox, tr( "Distribute the items with the distance specified" ) );
	
	guideInfoTextNone = tr("None Selected");
}

void AlignDistributePalette::init()
{
	undoManager = UndoManager::instance();

	alignLeftOutToolButton->setIconSet(QIconSet(loadIcon("al_left_out.png"), QIconSet::Small));
	alignLeftInToolButton->setIconSet(QIconSet(loadIcon("al_left_in.png"), QIconSet::Small));
	alignCenterHorToolButton->setIconSet(QIconSet(loadIcon("al_center_hor.png"), QIconSet::Small));
	alignRightInToolButton->setIconSet(QIconSet(loadIcon("al_right_in.png"), QIconSet::Small));
	alignRightOutToolButton->setIconSet(QIconSet(loadIcon("al_right_out.png"), QIconSet::Small));
	
	alignTopOutToolButton->setIconSet(QIconSet(loadIcon("al_top_out.png"), QIconSet::Small));
	alignTopInToolButton->setIconSet(QIconSet(loadIcon("al_top_in.png"), QIconSet::Small));
	alignCenterVerToolButton->setIconSet(QIconSet(loadIcon("al_center_ver.png"), QIconSet::Small));
	alignBottomInToolButton->setIconSet(QIconSet(loadIcon("al_bottom_in.png"), QIconSet::Small));
	alignBottomOutToolButton->setIconSet(QIconSet(loadIcon("al_bottom_out.png"), QIconSet::Small));
	
	distributeLeftToolButton->setIconSet(QIconSet(loadIcon("distribute_left.png"), QIconSet::Small));
	distributeCenterHToolButton->setIconSet(QIconSet(loadIcon("distribute_hcentre.png"), QIconSet::Small));
	distributeRightToolButton->setIconSet(QIconSet(loadIcon("distribute_right.png"), QIconSet::Small));
	distributeDistHToolButton->setIconSet(QIconSet(loadIcon("distribute_hdist.png"), QIconSet::Small));
	distributeDistValueHToolButton->setIconSet(QIconSet(loadIcon("distribute_hdist_val.png"), QIconSet::Small));
	
	distributeBottomToolButton->setIconSet(QIconSet(loadIcon("distribute_bottom.png"), QIconSet::Small));
	distributeCenterVToolButton->setIconSet(QIconSet(loadIcon("distribute_vcentre.png"), QIconSet::Small));
	distributeTopToolButton->setIconSet(QIconSet(loadIcon("distribute_top.png"), QIconSet::Small));
	distributeDistVToolButton->setIconSet(QIconSet(loadIcon("distribute_vdist.png"), QIconSet::Small));
	distributeDistValueVToolButton->setIconSet(QIconSet(loadIcon("distribute_vdist_val.png"), QIconSet::Small));
	
	connect(alignLeftOutToolButton, SIGNAL(clicked()), this, SLOT(alignLeftOut()));
	connect(alignRightOutToolButton, SIGNAL(clicked()), this, SLOT(alignRightOut()));
	connect(alignBottomInToolButton, SIGNAL(clicked()), this, SLOT(alignBottomIn()));
	connect(alignRightInToolButton, SIGNAL(clicked()), this, SLOT(alignRightIn()));
	connect(alignBottomOutToolButton, SIGNAL(clicked()), this, SLOT(alignBottomOut()));
	connect(alignCenterHorToolButton, SIGNAL(clicked()), this, SLOT(alignCenterHor()));
	connect(alignLeftInToolButton, SIGNAL(clicked()), this, SLOT(alignLeftIn()));
	connect(alignCenterVerToolButton, SIGNAL(clicked()), this, SLOT(alignCenterVer()));
	connect(alignTopOutToolButton, SIGNAL(clicked()), this, SLOT(alignTopOut()));
	connect(alignTopInToolButton, SIGNAL(clicked()), this, SLOT(alignTopIn()));
	connect(distributeDistHToolButton, SIGNAL(clicked()), this, SLOT(distributeDistH()));
	connect(distributeDistValueHToolButton, SIGNAL(clicked()), this, SLOT(distributeDistValH()));
	connect(distributeRightToolButton, SIGNAL(clicked()), this, SLOT(distributeRight()));
	connect(distributeBottomToolButton, SIGNAL(clicked()), this, SLOT(distributeBottom()));
	connect(distributeCenterHToolButton, SIGNAL(clicked()), this, SLOT(distributeCenterH()));
	connect(distributeDistVToolButton, SIGNAL(clicked()), this, SLOT(distributeDistV()));
	connect(distributeDistValueVToolButton, SIGNAL(clicked()), this, SLOT(distributeDistValV()));
	connect(distributeLeftToolButton, SIGNAL(clicked()), this, SLOT(distributeLeft()));
	connect(distributeCenterVToolButton, SIGNAL(clicked()), this, SLOT(distributeCenterV()));
	connect(distributeTopToolButton, SIGNAL(clicked()), this, SLOT(distributeTop()));
	
	connect(alignRelativeToCombo, SIGNAL(activated(int)), this, SLOT(alignToChanged(int)));
	
	unitRatio=1.0;
	guideDirection=-1;
	
	guideInfoText = guideInfoTextNone;
	alignGuideLineEdit->setText(guideInfoTextNone);
}

void AlignDistributePalette::unitChange()
{
	if (currDoc!=NULL)
	{
		double oldValue=distributeDistMSpinBox->value();
		double oldRatio=unitRatio;
		distributeDistMSpinBox->setDecimals(unitGetDecimalsFromIndex(currDoc->unitIndex()));
		distributeDistMSpinBox->setSuffix(unitGetSuffixFromIndex(currDoc->unitIndex()));
		unitRatio=unitGetRatioFromIndex(currDoc->unitIndex());
		double ratioDivisor =  unitRatio / oldRatio;
		distributeDistMSpinBox->setValue(oldValue*ratioDivisor);
		enableGuideButtons();
	}
}

void AlignDistributePalette::setDoc( ScribusDoc* newDoc )
{
	currDoc=newDoc;
	if (currDoc!=NULL)
		alignObjects=&(currDoc->AObjects);
	else
		alignObjects=NULL;
	unitChange();
}

void AlignDistributePalette::alignLeftOut()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_AlignLeftOut(currAlignTo, guidePosition);
}

void AlignDistributePalette::alignLeftIn()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_AlignLeftIn(currAlignTo, guidePosition);
}

void AlignDistributePalette::alignCenterHor()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_AlignCenterHor(currAlignTo, guidePosition);
}

void AlignDistributePalette::alignRightIn()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_AlignRightIn(currAlignTo, guidePosition);
}

void AlignDistributePalette::alignRightOut()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_AlignRightOut(currAlignTo, guidePosition);
}

void AlignDistributePalette::alignTopOut()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_AlignTopOut(currAlignTo, guidePosition);
}

void AlignDistributePalette::alignTopIn()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_AlignTopIn(currAlignTo, guidePosition);
}


void AlignDistributePalette::alignCenterVer()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_AlignCenterVer(currAlignTo, guidePosition);
}


void AlignDistributePalette::alignBottomIn()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_AlignBottomIn(currAlignTo, guidePosition);
}

void AlignDistributePalette::alignBottomOut()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_AlignBottomOut(currAlignTo, guidePosition);
}

void AlignDistributePalette::distributeLeft()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_DistributeLeft();
}

void AlignDistributePalette::distributeCenterH()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_DistributeCenterH();
}

void AlignDistributePalette::distributeRight()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_DistributeRight();
}

void AlignDistributePalette::distributeDistH(bool usingDistance)
{
	if (currDoc!=NULL)
		currDoc->itemSelection_DistributeDistH(usingDistance, distributeDistMSpinBox->value());
}

void AlignDistributePalette::distributeDistValH()
{
	if (currDoc!=NULL)
		distributeDistH(true);
}

void AlignDistributePalette::distributeBottom()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_DistributeBottom();
}

void AlignDistributePalette::distributeCenterV()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_DistributeCenterV();
}

void AlignDistributePalette::distributeTop()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_DistributeTop();
}

void AlignDistributePalette::distributeDistV(bool usingDistance)
{
	if (currDoc!=NULL)
		currDoc->itemSelection_DistributeDistV(usingDistance, distributeDistMSpinBox->value());
}

void AlignDistributePalette::distributeDistValV()
{
	if (currDoc!=NULL)
		distributeDistV(true);
}

void AlignDistributePalette::alignToChanged(int newAlignTo)
{
	currAlignTo=(ScribusDoc::AlignTo)newAlignTo;
	enableGuideButtons();
}

void AlignDistributePalette::setGuide(int direction, double position)
{
	//direction 0=H, 1=V
	guideDirection=direction;
	guidePosition=position;
	enableGuideButtons();
}

void AlignDistributePalette::enableGuideButtons()
{
	QString suffix="";
	double unitRatio=1.0;
	int precision=1;
	if (currDoc!=NULL)
	{
		suffix=unitGetSuffixFromIndex(currDoc->unitIndex());
		unitRatio=unitGetRatioFromIndex(currDoc->unitIndex());
		precision=unitGetPrecisionFromIndex(currDoc->unitIndex());
	}
	bool setterH=true, setterV=true;
	switch(guideDirection)
	{
		case -1:
			guideInfoText = guideInfoTextNone;
			if (currAlignTo==ScribusDoc::alignGuide)
				setterH=setterV=false;
			break;
		case 0:
			guideInfoText = tr("Y: %1%2").arg(guidePosition * unitRatio, 0, 'f', precision).arg(suffix);
			if (currAlignTo==ScribusDoc::alignGuide)
			{
				setterV=false;
				setterH=true;
			}
			break;
		case 1:
			guideInfoText = tr("X: %1%2").arg(guidePosition * unitRatio, 0, 'f', precision).arg(suffix);
			if (currAlignTo==ScribusDoc::alignGuide)
			{
				setterV=true;
				setterH=false;
			}
			break;
	}
	bool setterO=true;
	if (currAlignTo==ScribusDoc::alignGuide)
		setterO=false;
		
	alignLeftInToolButton->setEnabled(setterV);
	alignLeftOutToolButton->setEnabled(setterO);
	alignRightInToolButton->setEnabled(setterV);
	alignRightOutToolButton->setEnabled(setterO);	
	alignCenterHorToolButton->setEnabled(setterV);

	alignTopInToolButton->setEnabled(setterH);
	alignTopOutToolButton->setEnabled(setterO);
	alignBottomInToolButton->setEnabled(setterH);
	alignBottomOutToolButton->setEnabled(setterO);
	alignCenterVerToolButton->setEnabled(setterH);
	
	alignGuideLineEdit->setText(guideInfoText);
}
