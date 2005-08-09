/***************************************************************************
	begin                : June 2005
	copyright            : (C) 2005 by Craig Bradney
	email                : cbradney@zip.com.au
***************************************************************************/

/***************************************************************************
*                                                                         *
*   ScApp program is free software; you can redistribute it and/or modify  *
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

#include "scribus.h"
#include "scribusdoc.h"
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
	ScApp = (ScribusApp *)parent;
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
	setView(NULL);
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
	usingDistance=false;
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

void AlignDistributePalette::setView( ScribusView * newView )
{
	currView=newView;
	if (currView!=NULL)
	{
		alignObjects=&(currView->AObjects);
		currDoc=currView->Doc;
	}
	else
	{
		alignObjects=NULL;
		currDoc=NULL;
	}
	unitChange();
}

bool AlignDistributePalette::startAlign()
{
	currView->BuildAObj();
	alignObjectsCount=alignObjects->count();
	if (alignObjectsCount==0)
		return false;
		
	bool oneLocked=false;
	for (uint i = 0; i < alignObjectsCount; ++i)
			for (uint j = 0; j < (*alignObjects)[i].Objects.count(); ++j)
				if ((*alignObjects)[i].Objects.at(j)->locked())
					oneLocked=true;
	if (oneLocked)
	{
		int t = QMessageBox::warning(ScApp, tr("Warning"),
											tr("Some objects are locked."),
											CommonStrings::tr_Cancel,
											tr("&Unlock All"), 0, 0);
		if (t == 0)
			return false;
		for (uint i = 0; i < alignObjectsCount; ++i)
			for (uint j = 0; j < (*alignObjects)[i].Objects.count(); ++j)
				if ((*alignObjects)[i].Objects.at(j)->locked())
					(*alignObjects)[i].Objects.at(j)->setLocked(false);
	}
	
	QString targetTooltip = Um::ItemsInvolved + "\n";
	for (uint i = 0; i < currView->SelItem.count(); ++i)
		targetTooltip += "\t" + currView->SelItem.at(i)->getUName() + "\n";
		// Make the align action a single action in Action History
	undoManager->beginTransaction(Um::Selection, 0, Um::AlignDistribute, targetTooltip, Um::IAlignDistribute);
	return true;
}

void AlignDistributePalette::endAlign()
{
	emit documentChanged();
	ScApp->HaveNewSel(currView->SelItem.at(0)->itemType());
	for (uint i = 0; i < currView->SelItem.count(); ++i)
	{
		currView->SelItem.at(i)->checkChanges(true); // force aligned items to check their changes
		currView->setRedrawBounding(currView->SelItem.at(i));
	}
	undoManager->commit(); // commit and send the action to the UndoManager
	currView->updateContents();
}


void AlignDistributePalette::alignLeftOut()
{
	if (currView!=NULL)
	{
		if (!startAlign())
			return;
		uint loopStart=0, loopEnd=alignObjectsCount;
		double newX = 99999.9;
		switch ( currAlignTo ) 
		{
			case First:
				newX = (*alignObjects)[0].x1;
				loopStart=1;
				break;
			case Last:
				newX = (*alignObjects)[alignObjectsCount-1].x1;
				loopEnd=alignObjectsCount-2;
				break;
			case Page:
				newX = currDoc->ScratchLeft;
				if ((currDoc->PageFP == doublePage) && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newX += currDoc->currentPage->Width;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newX += currDoc->currentPage->Width;
				}
				break;
			case Margins:
				newX = currDoc->ScratchLeft;
				if ((currDoc->PageFP == doublePage) && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newX += currDoc->currentPage->Width;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newX += currDoc->currentPage->Width;
				}
				newX += currDoc->currentPage->Margins.Left;
				break;
			case Guide:
				newX=currDoc->ScratchLeft + guidePosition;
				break;
			case Selection:
				for (uint a = 0; a < alignObjectsCount; ++a)
					newX = QMIN((*alignObjects)[a].x1, newX);
				break;
		}
		for (uint i = loopStart; i <= loopEnd; ++i)
		{
			double diff=newX-(*alignObjects)[i].x2;
			for (uint j = 0; j < (*alignObjects)[i].Objects.count(); ++j)
				(*alignObjects)[i].Objects.at(j)->Xpos+=diff;
		}
		endAlign();
	}
}

void AlignDistributePalette::alignLeftIn()
{
	if (currView!=NULL)
	{
		if (!startAlign())
			return;
		uint loopStart=0, loopEnd=alignObjectsCount;
		double newX = 99999.9;
		switch ( currAlignTo ) 
		{
			case First:
				newX = (*alignObjects)[0].x1;
				loopStart=1;
				break;
			case Last:
				newX = (*alignObjects)[alignObjectsCount-1].x1;
				loopEnd=alignObjectsCount-2;
				break;
			case Page:
				newX = currDoc->ScratchLeft;
				if ((currDoc->PageFP == doublePage) && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newX += currDoc->currentPage->Width;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newX += currDoc->currentPage->Width;
				}
				break;
			case Margins:
				newX = currDoc->ScratchLeft;
				if ((currDoc->PageFP == doublePage ) && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newX += currDoc->currentPage->Width;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newX += currDoc->currentPage->Width;
				}
				newX += currDoc->currentPage->Margins.Left;
				break;
			case Guide:
				newX=currDoc->ScratchLeft + guidePosition;
				break;
			case Selection:
				for (uint a = 0; a < alignObjectsCount; ++a)
					newX = QMIN((*alignObjects)[a].x1, newX);
				break;
		}
		for (uint i = loopStart; i <= loopEnd; ++i)
		{
			double diff=newX-(*alignObjects)[i].x1;
			for (uint j = 0; j < (*alignObjects)[i].Objects.count(); ++j)
				(*alignObjects)[i].Objects.at(j)->Xpos+=diff;
		}
		endAlign();
	}
}

void AlignDistributePalette::alignCenterHor()
{
	if (currView!=NULL)
	{
		if (!startAlign())
			return;
		uint loopStart=0, loopEnd=alignObjectsCount;
		double newX;
		switch ( currAlignTo ) 
		{
			case First:
				newX = (*alignObjects)[0].x1 + ((*alignObjects)[0].width)/2;
				loopStart=1;
				break;
			case Last:
				{
					int objindex=alignObjectsCount-1;
					newX = (*alignObjects)[objindex].x1 + ((*alignObjects)[objindex].width)/2;
					loopEnd=alignObjectsCount-2;
				}
				break;
			case Page:
				newX = currDoc->ScratchLeft;
				if ((currDoc->PageFP == doublePage) && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newX += currDoc->currentPage->Width;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newX += currDoc->currentPage->Width;
				}
				newX += currDoc->currentPage->Width/2;
				break;
			case Margins:
				newX = currDoc->ScratchLeft;
				if ((currDoc->PageFP == doublePage ) && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newX += currDoc->currentPage->Width;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newX += currDoc->currentPage->Width;
				}
				newX += currDoc->currentPage->Margins.Left;
				newX += (currDoc->currentPage->Width - currDoc->currentPage->Margins.Right - currDoc->currentPage->Margins.Left)/2;
				break;
			case Guide:
				newX=currDoc->ScratchLeft + guidePosition;
				break;
			case Selection:
				double minX=99999.9, maxX=-99999.9;
				for (uint a = 0; a < alignObjectsCount; ++a)
				{
					minX = QMIN((*alignObjects)[a].x1, minX);
					maxX = QMAX((*alignObjects)[a].x2, maxX);
				}
				newX = minX + (maxX-minX)/2;
				break;
		}
		for (uint i = loopStart; i <= loopEnd; ++i)
		{
			double diff=newX-(*alignObjects)[i].x1-((*alignObjects)[i].width)/2;
			for (uint j = 0; j < (*alignObjects)[i].Objects.count(); ++j)
				(*alignObjects)[i].Objects.at(j)->Xpos+=diff;
		}
		endAlign();
	}
}

void AlignDistributePalette::alignRightIn()
{
	if (currView!=NULL)
	{
		if (!startAlign())
			return;
		uint loopStart=0, loopEnd=alignObjectsCount;
		double newX = -99999.9;
		switch ( currAlignTo ) 
		{
			case First:
				newX = (*alignObjects)[0].x2;
				loopStart=1;
				break;
			case Last:
				newX = (*alignObjects)[alignObjectsCount-1].x2;
				loopEnd=alignObjectsCount-2;
				break;
			case Page:
				newX = currDoc->ScratchLeft;
				if ((currDoc->PageFP == doublePage ) && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newX += currDoc->currentPage->Width;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newX += currDoc->currentPage->Width;
				}
				newX += currDoc->currentPage->Width;
				break;
			case Margins:
				newX = currDoc->ScratchLeft;
				if ((currDoc->PageFP == doublePage ) && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newX += currDoc->currentPage->Width;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newX += currDoc->currentPage->Width;
				}
				newX += currDoc->currentPage->Width;
				newX -= currDoc->currentPage->Margins.Right;
				break;
			case Guide:
				newX=currDoc->ScratchLeft + guidePosition;
				break;
			case Selection:
				for (uint a = 0; a < alignObjectsCount; ++a)
					newX = QMAX((*alignObjects)[a].x2, newX);
				break;
		}
		for (uint i = loopStart; i <= loopEnd; ++i)
		{
			double diff=newX-(*alignObjects)[i].x2;
			for (uint j = 0; j < (*alignObjects)[i].Objects.count(); ++j)
				(*alignObjects)[i].Objects.at(j)->Xpos+=diff;
		}
		endAlign();
	}
}

void AlignDistributePalette::alignRightOut()
{
	if (currView!=NULL)
	{
		if (!startAlign())
			return;
		uint loopStart=0, loopEnd=alignObjectsCount;
		double newX = -99999.9;
		switch ( currAlignTo ) 
		{
			case First:
				newX = (*alignObjects)[0].x2;
				loopStart=1;
				break;
			case Last:
				newX = (*alignObjects)[alignObjectsCount-1].x2;
				loopEnd=alignObjectsCount-2;
				break;
			case Page:
				newX = currDoc->ScratchLeft;
				if ((currDoc->PageFP == doublePage ) && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newX += currDoc->currentPage->Width;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newX += currDoc->currentPage->Width;
				}
				newX += currDoc->currentPage->Width;
				break;
			case Margins:
				newX = currDoc->ScratchLeft;
				if ((currDoc->PageFP == doublePage ) && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newX += currDoc->currentPage->Width;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newX += currDoc->currentPage->Width;
				}
				newX += currDoc->currentPage->Width;
				newX -= currDoc->currentPage->Margins.Right;
				break;
			case Guide:
				newX=currDoc->ScratchLeft + guidePosition;
				break;
			case Selection:
				for (uint a = 0; a < alignObjectsCount; ++a)
					newX = QMAX((*alignObjects)[a].x2, newX);
				break;
		}
		for (uint i = loopStart; i <= loopEnd; ++i)
		{
			double diff=newX-(*alignObjects)[i].x1;
			for (uint j = 0; j < (*alignObjects)[i].Objects.count(); ++j)
				(*alignObjects)[i].Objects.at(j)->Xpos+=diff;
		}
		endAlign();
	}
}

void AlignDistributePalette::alignTopOut()
{
	if (currView!=NULL)
	{
		if (!startAlign())
			return;
		uint loopStart=0, loopEnd=alignObjectsCount, multiplier;
		double newY = 99999.9;
		switch ( currAlignTo ) 
		{
			case First:
				newY = (*alignObjects)[0].y1;
				loopStart=1;
				break;
			case Last:
				newY = (*alignObjects)[alignObjectsCount-1].y1;
				loopEnd=alignObjectsCount-2;
				break;
			case Page:
				newY = currDoc->ScratchTop;
				if (!currDoc->MasterP)
				{
					if (!(currDoc->PageFP == doublePage ))
						multiplier=currDoc->currentPage->PageNr;
					else
					{
						multiplier=currDoc->currentPage->PageNr/2;
						if (!currDoc->FirstPageLeft && currDoc->currentPage->PageNr % 2 == 1)
							multiplier++;
					}
					newY += (currDoc->ScratchBottom + currDoc->ScratchTop + currDoc->currentPage->Height) * static_cast<double>(multiplier);
				}
				break;
			case Margins:
				newY = currDoc->ScratchTop;
				if (!currDoc->MasterP)
				{
					if (!(currDoc->PageFP == doublePage ))
						multiplier=currDoc->currentPage->PageNr;
					else
					{
						multiplier=currDoc->currentPage->PageNr/2;
						if (!currDoc->FirstPageLeft && currDoc->currentPage->PageNr % 2 == 1)
							multiplier++;
					}
					newY += (currDoc->ScratchBottom + currDoc->ScratchTop + currDoc->currentPage->Height) * static_cast<double>(multiplier);
				}
				newY += currDoc->currentPage->Margins.Top;
				break;
			case Guide:
				newY=currDoc->ScratchTop + guidePosition;
				break;
			case Selection:
				for (uint a = 0; a < alignObjectsCount; ++a)
					newY = QMIN((*alignObjects)[a].y1, newY);
				break;
		}
		for (uint i = loopStart; i <= loopEnd; ++i)
		{
			double diff=newY-(*alignObjects)[i].y2;
			for (uint j = 0; j < (*alignObjects)[i].Objects.count(); ++j)
				(*alignObjects)[i].Objects.at(j)->Ypos+=diff;
		}
		endAlign();
	}
}

void AlignDistributePalette::alignTopIn()
{
	if (currView!=NULL)
	{
		if (!startAlign())
			return;
		uint loopStart=0, loopEnd=alignObjectsCount, multiplier;
		double newY = 99999.9;
		switch ( currAlignTo ) 
		{
			case First:
				newY = (*alignObjects)[0].y1;
				loopStart=1;
				break;
			case Last:
				newY = (*alignObjects)[alignObjectsCount-1].y1;
				loopEnd=alignObjectsCount-2;
				break;
			case Page:
				newY = currDoc->ScratchTop;
				if (!currDoc->MasterP)
				{
					if (!(currDoc->PageFP == doublePage ))
						multiplier=currDoc->currentPage->PageNr;
					else
					{
						multiplier=currDoc->currentPage->PageNr/2;
						if (!currDoc->FirstPageLeft && currDoc->currentPage->PageNr % 2 == 1)
							multiplier++;
					}
					newY += (currDoc->ScratchBottom + currDoc->ScratchTop + currDoc->currentPage->Height) * static_cast<double>(multiplier);
				}
				break;
			case Margins:
				newY = currDoc->ScratchTop;
				if (!currDoc->MasterP)
				{
					if (!(currDoc->PageFP == doublePage ))
						multiplier=currDoc->currentPage->PageNr;
					else
					{
						multiplier=currDoc->currentPage->PageNr/2;
						if (!currDoc->FirstPageLeft && currDoc->currentPage->PageNr % 2 == 1)
							multiplier++;
					}
					newY += (currDoc->ScratchBottom + currDoc->ScratchTop + currDoc->currentPage->Height) * static_cast<double>(multiplier);
				}
				newY += currDoc->currentPage->Margins.Top;
				break;
			case Guide:
				newY=currDoc->ScratchTop + guidePosition;
				break;
			case Selection:
				for (uint a = 0; a < alignObjectsCount; ++a)
					newY = QMIN((*alignObjects)[a].y1, newY);
				break;
		}
		for (uint i = loopStart; i <= loopEnd; ++i)
		{
			double diff=newY-(*alignObjects)[i].y1;
			for (uint j = 0; j < (*alignObjects)[i].Objects.count(); ++j)
				(*alignObjects)[i].Objects.at(j)->Ypos+=diff;
		}
		endAlign();
	}
}


void AlignDistributePalette::alignCenterVer()
{
	if (currView!=NULL)
	{
		if (!startAlign())
			return;
		uint loopStart=0, loopEnd=alignObjectsCount, multiplier;
		double newY;
		switch ( currAlignTo ) 
		{
			case First:
				newY = (*alignObjects)[0].y1 + ((*alignObjects)[0].height)/2;
				loopStart=1;
				break;
			case Last:
				{
					int objindex=alignObjectsCount-1;
					newY = (*alignObjects)[objindex].y1 + ((*alignObjects)[objindex].height)/2;
					loopEnd=alignObjectsCount-2;
				}
				break;
			case Page:
				newY = currDoc->ScratchTop;
				if (!currDoc->MasterP)
				{
					if (!(currDoc->PageFP == doublePage ))
						multiplier=currDoc->currentPage->PageNr;
					else
					{
						multiplier=currDoc->currentPage->PageNr/2;
						if (!currDoc->FirstPageLeft && currDoc->currentPage->PageNr % 2 == 1)
							multiplier++;
					}
					newY += (currDoc->ScratchBottom + currDoc->ScratchTop + currDoc->currentPage->Height) * static_cast<double>(multiplier);
				}
				newY += currDoc->currentPage->Height/2;
				break;
			case Margins:
				newY = currDoc->ScratchTop;
				if (!currDoc->MasterP)
				{
					if (!(currDoc->PageFP == doublePage ))
						multiplier=currDoc->currentPage->PageNr;
					else
					{
						multiplier=currDoc->currentPage->PageNr/2;
						if (!currDoc->FirstPageLeft && currDoc->currentPage->PageNr % 2 == 1)
							multiplier++;
					}
					newY += (currDoc->ScratchBottom + currDoc->ScratchTop + currDoc->currentPage->Height) * static_cast<double>(multiplier);
				}
				newY += currDoc->currentPage->Margins.Top;
				newY += (currDoc->currentPage->Height - currDoc->currentPage->Margins.Bottom - currDoc->currentPage->Margins.Top)/2;
				break;
			case Guide:
				newY=currDoc->ScratchTop + guidePosition;
				break;
			case Selection:
				double minY=99999.9, maxY=-99999.9;
				for (uint a = 0; a < alignObjectsCount; ++a)
				{
					minY = QMIN((*alignObjects)[a].y1, minY);
					maxY = QMAX((*alignObjects)[a].y2, maxY);
				}
				newY = minY + (maxY-minY)/2;
				break;
		}
		for (uint i = loopStart; i <= loopEnd; ++i)
		{
			double diff=newY-(*alignObjects)[i].y1-((*alignObjects)[i].height)/2;
			for (uint j = 0; j < (*alignObjects)[i].Objects.count(); ++j)
				(*alignObjects)[i].Objects.at(j)->Ypos+=diff;
		}
		endAlign();
	}
}


void AlignDistributePalette::alignBottomIn()
{
	if (currView!=NULL)
	{
		if (!startAlign())
			return;
		uint loopStart=0, loopEnd=alignObjectsCount, multiplier;
		double newY = -99999.9;
		switch ( currAlignTo ) 
		{
			case First:
				newY = (*alignObjects)[0].y2;
				loopStart=1;
				break;
			case Last:
				newY = (*alignObjects)[alignObjectsCount-1].y2;
				loopEnd=alignObjectsCount-2;
				break;
			case Page:
				newY = currDoc->ScratchTop;
				if (!currDoc->MasterP)
				{
					if (!(currDoc->PageFP == doublePage ))
						multiplier=currDoc->currentPage->PageNr;
					else
					{
						multiplier=currDoc->currentPage->PageNr/2;
						if (!currDoc->FirstPageLeft && currDoc->currentPage->PageNr % 2 == 1)
							multiplier++;
					}
					newY += (currDoc->ScratchBottom + currDoc->ScratchTop + currDoc->currentPage->Height) * static_cast<double>(multiplier);
				}
				newY += currDoc->currentPage->Height;
				break;
			case Margins:
				newY = currDoc->ScratchTop;
				if (!currDoc->MasterP)
				{
					if (!(currDoc->PageFP == doublePage ))
						multiplier=currDoc->currentPage->PageNr;
					else
					{
						multiplier=currDoc->currentPage->PageNr/2;
						if (!currDoc->FirstPageLeft && currDoc->currentPage->PageNr % 2 == 1)
							multiplier++;
					}
					newY += (currDoc->ScratchBottom + currDoc->ScratchTop + currDoc->currentPage->Height) * static_cast<double>(multiplier);
				}
				newY += currDoc->currentPage->Height;
				newY -= currDoc->currentPage->Margins.Bottom;
				break;
			case Guide:
				newY=currDoc->ScratchTop + guidePosition;
				break;
			case Selection:
				for (uint a = 0; a < alignObjectsCount; ++a)
					newY = QMAX((*alignObjects)[a].y2, newY);
				break;
		}
		for (uint i = loopStart; i <= loopEnd; ++i)
		{
			double diff=newY-(*alignObjects)[i].y2;
			for (uint j = 0; j < (*alignObjects)[i].Objects.count(); ++j)
				(*alignObjects)[i].Objects.at(j)->Ypos+=diff;
		}
		endAlign();
	}
}

void AlignDistributePalette::alignBottomOut()
{
	if (currView!=NULL)
	{
		if (!startAlign())
			return;
		uint loopStart=0, loopEnd=alignObjectsCount, multiplier;
		double newY = -99999.9;
		switch ( currAlignTo ) 
		{
			case First:
				newY = (*alignObjects)[0].y2;
				loopStart=1;
				break;
			case Last:
				newY = (*alignObjects)[alignObjectsCount-1].y2;
				loopEnd=alignObjectsCount-2;
				break;
			case Page:
				newY = currDoc->ScratchTop;
				if (!currDoc->MasterP)
				{
					if (!(currDoc->PageFP == doublePage ))
						multiplier=currDoc->currentPage->PageNr;
					else
					{
						multiplier=currDoc->currentPage->PageNr/2;
						if (!currDoc->FirstPageLeft && currDoc->currentPage->PageNr % 2 == 1)
							multiplier++;
					}
					newY += (currDoc->ScratchBottom + currDoc->ScratchTop + currDoc->currentPage->Height) * static_cast<double>(multiplier);
				}
				newY += currDoc->currentPage->Height;
				break;
			case Margins:
				newY = currDoc->ScratchTop;
				if (!currDoc->MasterP)
				{
					if (!(currDoc->PageFP == doublePage ))
						multiplier=currDoc->currentPage->PageNr;
					else
					{
						multiplier=currDoc->currentPage->PageNr/2;
						if (!currDoc->FirstPageLeft && currDoc->currentPage->PageNr % 2 == 1)
							multiplier++;
					}
					newY += (currDoc->ScratchBottom + currDoc->ScratchTop + currDoc->currentPage->Height) * static_cast<double>(multiplier);
				}
				newY += currDoc->currentPage->Height;
				newY -= currDoc->currentPage->Margins.Bottom;
				break;
			case Guide:
				newY=currDoc->ScratchTop + guidePosition;
				break;
			case Selection:
				for (uint a = 0; a < alignObjectsCount; ++a)
					newY = QMAX((*alignObjects)[a].y2, newY);
				break;
		}
		for (uint i = loopStart; i <= loopEnd; ++i)
		{
			double diff=newY-(*alignObjects)[i].y1;
			for (uint j = 0; j < (*alignObjects)[i].Objects.count(); ++j)
				(*alignObjects)[i].Objects.at(j)->Ypos+=diff;
		}
		endAlign();
	}
}


void AlignDistributePalette::distributeLeft()
{
	if (currView!=NULL)
	{
		if (!startAlign())
			return;
		if (alignObjectsCount<=1)
			return;
		QMap<double,uint> Xsorted;
		for (uint a = 0; a < alignObjectsCount; ++a)
			Xsorted.insert((*alignObjects)[a].x1, a, false);
			
		double minX;
		double maxX;
		for ( QMap<double,uint>::Iterator it = Xsorted.begin(); it != Xsorted.end(); ++it )
		{
			if (it == Xsorted.begin())
			{
				minX=it.key();
				maxX=it.key();
			}
			else
			{
				if (minX>it.key())
					minX=it.key();
				if (maxX<it.key())
					maxX=it.key();
			}
		}
			
		double separation=(maxX-minX)/static_cast<double>(alignObjectsCount-1);
		int i=0;
		for ( QMap<double,uint>::Iterator it = Xsorted.begin(); it != Xsorted.end(); ++it )
		{
			double diff=minX + i*separation-(*alignObjects)[it.data()].x1;
			for (uint j = 0; j < (*alignObjects)[it.data()].Objects.count(); ++j)
				(*alignObjects)[it.data()].Objects.at(j)->Xpos+=diff;
			i++;
		}
		endAlign();
	}
}

void AlignDistributePalette::distributeCenterH()
{
	if (currView!=NULL)
	{
		if (!startAlign())
			return;
		if (alignObjectsCount<=1)
			return;
		QMap<double,uint> Xsorted;
		for (uint a = 0; a < alignObjectsCount; ++a)
			Xsorted.insert((*alignObjects)[a].x1+((*alignObjects)[a].width)/2, a, false);
			
		double minX;
		double maxX;
		for ( QMap<double,uint>::Iterator it = Xsorted.begin(); it != Xsorted.end(); ++it )
		{
			if (it == Xsorted.begin())
			{
				minX=it.key();
				maxX=it.key();
			}
			else
			{
				if (minX>it.key())
					minX=it.key();
				if (maxX<it.key())
					maxX=it.key();
			}
		}
			
		double separation=(maxX-minX)/static_cast<double>(alignObjectsCount-1);
		int i=0;
		for ( QMap<double,uint>::Iterator it = Xsorted.begin(); it != Xsorted.end(); ++it )
		{
			double diff=minX + i*separation-(*alignObjects)[it.data()].x1-((*alignObjects)[it.data()].width)/2;
			for (uint j = 0; j < (*alignObjects)[it.data()].Objects.count(); ++j)
				(*alignObjects)[it.data()].Objects.at(j)->Xpos+=diff;
			i++;
		}
		endAlign();
	}
}

void AlignDistributePalette::distributeRight()
{
	if (currView!=NULL)
	{
		if (!startAlign())
			return;
		if (alignObjectsCount<=1)
			return;
		QMap<double,uint> Xsorted;
		for (uint a = 0; a < alignObjectsCount; ++a)
			Xsorted.insert((*alignObjects)[a].x2, a, false);
			
		double minX;
		double maxX;
		for ( QMap<double,uint>::Iterator it = Xsorted.begin(); it != Xsorted.end(); ++it )
		{
			if (it == Xsorted.begin())
			{
				minX=it.key();
				maxX=it.key();
			}
			else
			{
				if (minX>it.key())
					minX=it.key();
				if (maxX<it.key())
					maxX=it.key();
			}
		}
			
		double separation=(maxX-minX)/static_cast<double>(alignObjectsCount-1);
		int i=0;
		for ( QMap<double,uint>::Iterator it = Xsorted.begin(); it != Xsorted.end(); ++it )
		{
			double diff=minX + i*separation-(*alignObjects)[it.data()].x2;
			for (uint j = 0; j < (*alignObjects)[it.data()].Objects.count(); ++j)
				(*alignObjects)[it.data()].Objects.at(j)->Xpos+=diff;
			i++;
		}
		endAlign();
	}
}

void AlignDistributePalette::distributeDistH()
{
	if (currView!=NULL)
	{
		if (!startAlign())
			return;
		if (alignObjectsCount<=1)
			return;
		QMap<double,uint> X1sorted, X2sorted;
		for (uint a = 0; a < alignObjectsCount; ++a)
		{
			X1sorted.insert((*alignObjects)[a].x1, a, false);
			X2sorted.insert((*alignObjects)[a].x2, a, false);
		}	
		uint left=X1sorted.begin().data();
		uint right=X2sorted[X2sorted.keys().back()];
		double minX=(*alignObjects)[left].x2;
		double separation;
		if (!usingDistance)
		{
			double maxX=(*alignObjects)[right].x1;
			double totalSpace=maxX-minX;
			double totalWidth=0;
			uint insideObjectCount=0;
			for (uint a = 0; a < alignObjectsCount; ++a)
			{
				if (a==left)
					continue;
				if (a==right)
					continue;
				totalWidth += (*alignObjects)[a].width;
				++insideObjectCount;
			}
			separation=(totalSpace-totalWidth)/(insideObjectCount+1);
		}
		else
			separation=value2pts(distributeDistMSpinBox->value(), currDoc->unitIndex());
		double currX=minX;
		for ( QMap<double,uint>::Iterator it = X1sorted.begin(); it != X1sorted.end(); ++it )
		{
			if (it.data()==left)
				continue;
			if (it.data()==right && !usingDistance)
				continue;
			currX+=separation;

			double diff=currX-(*alignObjects)[it.data()].x1;
			for (uint j = 0; j < (*alignObjects)[it.data()].Objects.count(); ++j)
				(*alignObjects)[it.data()].Objects.at(j)->Xpos+=diff;
			currX+=(*alignObjects)[it.data()].width;
		}
		endAlign();
	}
	usingDistance=false;
}

void AlignDistributePalette::distributeDistValH()
{
	usingDistance=true;
	distributeDistH();
}

void AlignDistributePalette::distributeBottom()
{
	if (currView!=NULL)
	{
		if (!startAlign())
			return;
		if (alignObjectsCount<=1)
			return;
		QMap<double,uint> Ysorted;
		for (uint a = 0; a < alignObjectsCount; ++a)
			Ysorted.insert((*alignObjects)[a].y2, a, false);
			
		double minY;
		double maxY;
		for ( QMap<double,uint>::Iterator it = Ysorted.begin(); it != Ysorted.end(); ++it )
		{
			if (it == Ysorted.begin())
			{
				minY=it.key();
				maxY=it.key();
			}
			else
			{
				if (minY>it.key())
					minY=it.key();
				if (maxY<it.key())
					maxY=it.key();
			}
		}
			
		double separation=(maxY-minY)/static_cast<double>(alignObjectsCount-1);
		int i=0;
		for ( QMap<double,uint>::Iterator it = Ysorted.begin(); it != Ysorted.end(); ++it )
		{
			double diff=minY + i*separation-(*alignObjects)[it.data()].y2;
			for (uint j = 0; j < (*alignObjects)[it.data()].Objects.count(); ++j)
				(*alignObjects)[it.data()].Objects.at(j)->Ypos+=diff;
			i++;
		}
		endAlign();
	}
}

void AlignDistributePalette::distributeCenterV()
{
	if (currView!=NULL)
	{
		if (!startAlign())
			return;
		if (alignObjectsCount<=1)
			return;
		QMap<double,uint> Ysorted;
		for (uint a = 0; a < alignObjectsCount; ++a)
			Ysorted.insert((*alignObjects)[a].y1+((*alignObjects)[a].height)/2, a, false);
			
		double minY;
		double maxY;
		for ( QMap<double,uint>::Iterator it = Ysorted.begin(); it != Ysorted.end(); ++it )
		{
			if (it == Ysorted.begin())
			{
				minY=it.key();
				maxY=it.key();
			}
			else
			{
				if (minY>it.key())
					minY=it.key();
				if (maxY<it.key())
					maxY=it.key();
			}
		}
			
		double separation=(maxY-minY)/static_cast<double>(alignObjectsCount-1);
		int i=0;
		for ( QMap<double,uint>::Iterator it = Ysorted.begin(); it != Ysorted.end(); ++it )
		{
			double diff=minY + i*separation-(*alignObjects)[it.data()].y1-((*alignObjects)[it.data()].height)/2;
			for (uint j = 0; j < (*alignObjects)[it.data()].Objects.count(); ++j)
				(*alignObjects)[it.data()].Objects.at(j)->Ypos+=diff;
			i++;
		}
		endAlign();
	}
}

void AlignDistributePalette::distributeTop()
{
	if (currView!=NULL)
	{
		if (!startAlign())
			return;
		if (alignObjectsCount<=1)
			return;
		QMap<double,uint> Ysorted;
		for (uint a = 0; a < alignObjectsCount; ++a)
			Ysorted.insert((*alignObjects)[a].y1, a, false);
			
		double minY;
		double maxY;
		for ( QMap<double,uint>::Iterator it = Ysorted.begin(); it != Ysorted.end(); ++it )
		{
			if (it == Ysorted.begin())
			{
				minY=it.key();
				maxY=it.key();
			}
			else
			{
				if (minY>it.key())
					minY=it.key();
				if (maxY<it.key())
					maxY=it.key();
			}
		}
			
		double separation=(maxY-minY)/static_cast<double>(alignObjectsCount-1);
		int i=0;
		for ( QMap<double,uint>::Iterator it = Ysorted.begin(); it != Ysorted.end(); ++it )
		{
			double diff=minY + i*separation-(*alignObjects)[it.data()].y1;
			for (uint j = 0; j < (*alignObjects)[it.data()].Objects.count(); ++j)
				(*alignObjects)[it.data()].Objects.at(j)->Ypos+=diff;
			i++;
		}
		endAlign();
	}
}

void AlignDistributePalette::distributeDistV()
{
	if (currView!=NULL)
	{
		if (!startAlign())
			return;
		if (alignObjectsCount<=1)
			return;
		QMap<double,uint> Y1sorted, Y2sorted;
		for (uint a = 0; a < alignObjectsCount; ++a)
		{
			Y1sorted.insert((*alignObjects)[a].y1, a, false);
			Y2sorted.insert((*alignObjects)[a].y2, a, false);
		}	
		uint top=Y1sorted.begin().data();
		uint bottom=Y2sorted[Y2sorted.keys().back()];
		double minY=(*alignObjects)[top].y2;
		double separation;
		if (!usingDistance)
		{
			double maxY=(*alignObjects)[bottom].y1;
			double totalSpace=maxY-minY;
			double totalHeight=0;
			uint insideObjectCount=0;
			for (uint a = 0; a < alignObjectsCount; ++a)
			{
				if (a==top)
					continue;
				if (a==bottom)
					continue;
				totalHeight += (*alignObjects)[a].height;
				++insideObjectCount;
			}
			separation=(totalSpace-totalHeight)/(insideObjectCount+1);
		}
		else
			separation=value2pts(distributeDistMSpinBox->value(), currDoc->unitIndex());
		double currY=minY;
		for ( QMap<double,uint>::Iterator it = Y1sorted.begin(); it != Y1sorted.end(); ++it )
		{
			if (it.data()==top)
				continue;
			if (it.data()==bottom && !usingDistance)
				continue;
			currY+=separation;

			double diff=currY-(*alignObjects)[it.data()].y1;
			for (uint j = 0; j < (*alignObjects)[it.data()].Objects.count(); ++j)
				(*alignObjects)[it.data()].Objects.at(j)->Ypos+=diff;
			currY+=(*alignObjects)[it.data()].height;
		}
		endAlign();
	}
	usingDistance=false;
}

void AlignDistributePalette::distributeDistValV()
{
	usingDistance=true;
	distributeDistV();
}

void AlignDistributePalette::alignToChanged(int newAlignTo)
{
	currAlignTo=(AlignTo)newAlignTo;
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
			if (currAlignTo==Guide)
				setterH=setterV=false;
			break;
		case 0:
			guideInfoText = tr("Y: %1%2").arg(guidePosition * unitRatio, 0, 'f', precision).arg(suffix);
			if (currAlignTo==Guide)
			{
				setterV=false;
				setterH=true;
			}
			break;
		case 1:
			guideInfoText = tr("X: %1%2").arg(guidePosition * unitRatio, 0, 'f', precision).arg(suffix);
			if (currAlignTo==Guide)
			{
				setterV=true;
				setterH=false;
			}
			break;
	}
	bool setterO=true;
	if (currAlignTo==Guide)
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
