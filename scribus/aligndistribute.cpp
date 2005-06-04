/****************************************************************************
** Form implementation generated from reading ui file './aligndistribute.ui'
**
** Created: Thu Jun 2 15:19:58 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

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

#include "aligndistribute.moc"

#include "scribus.h"
#include "scribusdoc.h"
#include "undomanager.h"

extern QPixmap loadIcon(QString nam);

/*
 *  Constructs a AlignDistributePalette as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
AlignDistributePalette::AlignDistributePalette( QWidget* parent, const char* name, bool modal, WFlags fl )
	: ScrPaletteBase( parent, name, modal, fl )
{
	if ( !name )
		setName( "AlignDistributePalette" );
	ScApp = (ScribusApp *)parent;
	AlignDistributePaletteLayout = new QVBoxLayout( this, 11, 6, "AlignDistributePaletteLayout"); 

	alignGroupBox = new QGroupBox( this, "alignGroupBox" );
	alignGroupBox->setColumnLayout(0, Qt::Vertical );
	alignGroupBox->layout()->setSpacing( 6 );
	alignGroupBox->layout()->setMargin( 11 );
	alignGroupBoxLayout = new QVBoxLayout( alignGroupBox->layout() );
	alignGroupBoxLayout->setAlignment( Qt::AlignTop );

	layout11 = new QHBoxLayout( 0, 0, 6, "layout11"); 

	alignRelativeToLabel = new QLabel( alignGroupBox, "alignRelativeToLabel" );
	layout11->addWidget( alignRelativeToLabel );

	alignRelativeToCombo = new QComboBox( FALSE, alignGroupBox, "alignRelativeToCombo" );
	layout11->addWidget( alignRelativeToCombo );
	alignGroupBoxLayout->addLayout( layout11 );

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
	alignGroupBoxLayout->addLayout( layout14 );
	AlignDistributePaletteLayout->addWidget( alignGroupBox );

	distributeGroupBox = new QGroupBox( this, "distributeGroupBox" );
	distributeGroupBox->setColumnLayout(0, Qt::Vertical );
	distributeGroupBox->layout()->setSpacing( 6 );
	distributeGroupBox->layout()->setMargin( 11 );
	distributeGroupBoxLayout = new QGridLayout( distributeGroupBox->layout() );
	distributeGroupBoxLayout->setAlignment( Qt::AlignTop );

	layout4 = new QHBoxLayout( 0, 0, 6, "layout4"); 
	dsitributeLeftSpacer = new QSpacerItem( 35, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout4->addItem( dsitributeLeftSpacer );

	layout1 = new QGridLayout( 0, 1, 1, 0, 6, "layout1"); 

	distributeDistHToolButton = new QToolButton( distributeGroupBox, "distributeDistHToolButton" );
	layout1->addWidget( distributeDistHToolButton, 0, 3 );

	distributeRightToolButton = new QToolButton( distributeGroupBox, "distributeRightToolButton" );
	layout1->addWidget( distributeRightToolButton, 0, 2 );

	distributeBottomToolButton = new QToolButton( distributeGroupBox, "distributeBottomToolButton" );
	layout1->addWidget( distributeBottomToolButton, 1, 0 );

	distributeCenterHToolButton = new QToolButton( distributeGroupBox, "distributeCenterHToolButton" );
	layout1->addWidget( distributeCenterHToolButton, 0, 1 );

	distributeDistVToolButton = new QToolButton( distributeGroupBox, "distributeDistVToolButton" );
	layout1->addWidget( distributeDistVToolButton, 1, 3 );

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
	AlignDistributePaletteLayout->addWidget( distributeGroupBox );
	languageChange();
	resize( QSize(222, 232).expandedTo(minimumSizeHint()) );
	clearWState( WState_Polished );

	// buddies
	alignRelativeToLabel->setBuddy( alignRelativeToCombo );
	init();
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
	distributeGroupBox->setTitle( tr( "Distribute" ) );
	distributeDistHToolButton->setText( QString::null );
	QToolTip::add( distributeDistHToolButton, tr( "Make vertical gaps between objects equal" ) );
	distributeRightToolButton->setText( QString::null );
	distributeRightToolButton->setTextLabel( tr( "Distribute right sides equidistantly" ) );
	distributeBottomToolButton->setText( QString::null );
	QToolTip::add( distributeBottomToolButton, tr( "Distribute bottoms equidistantly" ) );
	distributeCenterHToolButton->setText( QString::null );
	QToolTip::add( distributeCenterHToolButton, tr( "Distribute centers equidistantly horizontally" ) );
	distributeDistVToolButton->setText( QString::null );
	QToolTip::add( distributeDistVToolButton, tr( "Make vertical gaps between objects equal" ) );
	distributeLeftToolButton->setText( QString::null );
	QToolTip::add( distributeLeftToolButton, tr( "Distribute left sides equidistantly" ) );
	distributeCenterVToolButton->setText( QString::null );
	QToolTip::add( distributeCenterVToolButton, tr( "Distribute centers equidistantly vertically" ) );
	distributeTopToolButton->setText( QString::null );
	QToolTip::add( distributeTopToolButton, tr( "Distribute tops equidistantly" ) );
}


void AlignDistributePalette::init()
{
	undoManager = UndoManager::instance();

	alignLeftOutToolButton->setIconSet(loadIcon("al_left_out.png"));
	alignLeftInToolButton->setIconSet(loadIcon("al_left_in.png"));
	alignCenterHorToolButton->setIconSet(loadIcon("al_center_hor.png"));
	alignRightInToolButton->setIconSet(loadIcon("al_right_in.png"));
	alignRightOutToolButton->setIconSet(loadIcon("al_right_out.png"));
	
	alignTopOutToolButton->setIconSet(loadIcon("al_top_out.png"));
	alignTopInToolButton->setIconSet(loadIcon("al_top_in.png"));
	alignCenterVerToolButton->setIconSet(loadIcon("al_center_ver.png"));
	alignBottomInToolButton->setIconSet(loadIcon("al_bottom_in.png"));
	alignBottomOutToolButton->setIconSet(loadIcon("al_bottom_out.png"));
	
	distributeLeftToolButton->setIconSet(loadIcon("distribute_left.png"));
	distributeCenterHToolButton->setIconSet(loadIcon("distribute_hcentre.png"));
	distributeRightToolButton->setIconSet(loadIcon("distribute_right.png"));
	distributeDistHToolButton->setIconSet(loadIcon("distribute_hdist.png"));
	
	distributeBottomToolButton->setIconSet(loadIcon("distribute_bottom.png"));
	distributeCenterVToolButton->setIconSet(loadIcon("distribute_vcentre.png"));
	distributeTopToolButton->setIconSet(loadIcon("distribute_top.png"));
	distributeDistVToolButton->setIconSet(loadIcon("distribute_vdist.png"));
	
	
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
	connect(distributeRightToolButton, SIGNAL(clicked()), this, SLOT(distributeRight()));
	connect(distributeBottomToolButton, SIGNAL(clicked()), this, SLOT(distributeBottom()));
	connect(distributeCenterHToolButton, SIGNAL(clicked()), this, SLOT(distributeCenterH()));
	connect(distributeDistVToolButton, SIGNAL(clicked()), this, SLOT(distributeDistV()));
	connect(distributeLeftToolButton, SIGNAL(clicked()), this, SLOT(distributeLeft()));
	connect(distributeCenterVToolButton, SIGNAL(clicked()), this, SLOT(distributeCenterV()));
	connect(distributeTopToolButton, SIGNAL(clicked()), this, SLOT(distributeTop()));
	
	connect(alignRelativeToCombo, SIGNAL(activated(int)), this, SLOT(alignToChanged(int)));

	//Temporarily disable the uncoded actions
	distributeLeftToolButton->setEnabled(false);
	distributeCenterHToolButton->setEnabled(false);
	distributeRightToolButton->setEnabled(false);
	distributeDistHToolButton->setEnabled(false);
	
	distributeBottomToolButton->setEnabled(false);
	distributeCenterVToolButton->setEnabled(false);
	distributeTopToolButton->setEnabled(false);
	distributeDistVToolButton->setEnabled(false);

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
}

void AlignDistributePalette::startAlign()
{
	currView->BuildAObj();
	alignObjectsCount=alignObjects->count();
	QString targetTooltip = Um::ItemsInvolved + "\n";
	for (uint i = 0; i < currView->SelItem.count(); ++i)
		targetTooltip += "\t" + currView->SelItem.at(i)->getUName() + "\n";
		// Make the align action a single action in Action History
	undoManager->beginTransaction(Um::Selection, 0, Um::AlignDistribute, targetTooltip, Um::IAlignDistribute);

}

void AlignDistributePalette::endAlign()
{
	emit documentChanged();
	ScApp->HaveNewSel(currView->SelItem.at(0)->itemType());
	for (uint i = 0; i < currView->SelItem.count(); ++i)
		currView->SelItem.at(i)->checkChanges(true); // force aligned items to check their changes
	undoManager->commit(); // commit and send the action to the UndoManager
	currView->updateContents();
}


void AlignDistributePalette::alignLeftOut()
{
	if (currView!=NULL)
	{
		double newX = 99999.9;
		
		startAlign();
		uint loopStart=0, loopEnd=alignObjectsCount;
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
				if (currDoc->PageFP && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newX += currDoc->pageWidth;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newX += currDoc->pageWidth;
				}
				break;
			case Margins:
				newX = currDoc->ScratchLeft;
				if (currDoc->PageFP && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newX += currDoc->pageWidth;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newX += currDoc->pageWidth;
				}
				newX += currDoc->pageMargins.Left;
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
				for (uint i = loopStart; i <= loopEnd; ++i)

		endAlign();
	}
}

void AlignDistributePalette::alignLeftIn()
{
	if (currView!=NULL)
	{
		double newX = 99999.9;
		
		startAlign();
		uint loopStart=0, loopEnd=alignObjectsCount;
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
				if (currDoc->PageFP && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newX += currDoc->pageWidth;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newX += currDoc->pageWidth;
				}
				break;
			case Margins:
				newX = currDoc->ScratchLeft;
				if (currDoc->PageFP && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newX += currDoc->pageWidth;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newX += currDoc->pageWidth;
				}
				newX += currDoc->pageMargins.Left;
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
		double newX = 99999.9;
		
		startAlign();
		uint loopStart=0, loopEnd=alignObjectsCount;
		switch ( currAlignTo ) 
		{
			case First:
				newX = (*alignObjects)[0].x1 + ((*alignObjects)[0].x2-(*alignObjects)[0].x1)/2;
				loopStart=1;
				break;
			case Last:
				{
					int objindex=alignObjectsCount-1;
					newX = (*alignObjects)[objindex].x1 + ((*alignObjects)[objindex].x2-(*alignObjects)[objindex].x1)/2;
					loopEnd=alignObjectsCount-2;
				}
				break;
			case Page:
				newX = currDoc->ScratchLeft;
				if (currDoc->PageFP && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newX += currDoc->pageWidth;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newX += currDoc->pageWidth;
				}
				newX += currDoc->pageWidth/2;
				break;
			case Margins:
				newX = currDoc->ScratchLeft;
				if (currDoc->PageFP && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newX += currDoc->pageWidth;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newX += currDoc->pageWidth;
				}
				newX += currDoc->pageMargins.Left;
				newX += (currDoc->pageWidth - currDoc->pageMargins.Right - currDoc->pageMargins.Left)/2;
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
			double diff=newX-(*alignObjects)[i].x1-((*alignObjects)[i].x2-(*alignObjects)[i].x1)/2;
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
		double newX = -99999.9;
		
		startAlign();
		uint loopStart=0, loopEnd=alignObjectsCount;
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
				if (currDoc->PageFP && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newX += currDoc->pageWidth;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newX += currDoc->pageWidth;
				}
				newX += currDoc->pageWidth;
				break;
			case Margins:
				newX = currDoc->ScratchLeft;
				if (currDoc->PageFP && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newX += currDoc->pageWidth;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newX += currDoc->pageWidth;
				}
				newX += currDoc->pageWidth;
				newX -= currDoc->pageMargins.Right;
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
		double newX = -99999.9;
		
		startAlign();
		uint loopStart=0, loopEnd=alignObjectsCount;
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
				if (currDoc->PageFP && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newX += currDoc->pageWidth;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newX += currDoc->pageWidth;
				}
				newX += currDoc->pageWidth;
				break;
			case Margins:
				newX = currDoc->ScratchLeft;
				if (currDoc->PageFP && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newX += currDoc->pageWidth;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newX += currDoc->pageWidth;
				}
				newX += currDoc->pageWidth;
				newX -= currDoc->pageMargins.Right;
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
		double newY = 99999.9;
		
		startAlign();
		uint loopStart=0, loopEnd=alignObjectsCount;
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
				if (currDoc->PageFP && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newY += currDoc->pageHeight;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newY += currDoc->pageHeight;
				}
				break;
			case Margins:
				newY = currDoc->ScratchTop;
				if (currDoc->PageFP && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newY += currDoc->pageHeight;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newY += currDoc->pageHeight;
				}
				newY += currDoc->pageMargins.Left;
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
				for (uint i = loopStart; i <= loopEnd; ++i)

		endAlign();
	}
}

void AlignDistributePalette::alignTopIn()
{
	if (currView!=NULL)
	{
		double newY = 99999.9;
		
		startAlign();
		uint loopStart=0, loopEnd=alignObjectsCount;
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
				if (currDoc->PageFP && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newY += currDoc->pageHeight;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newY += currDoc->pageHeight;
				}
				break;
			case Margins:
				newY = currDoc->ScratchTop;
				if (currDoc->PageFP && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newY += currDoc->pageHeight;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newY += currDoc->pageHeight;
				}
				newY += currDoc->pageMargins.Left;
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
		double newY = 99999.9;
		
		startAlign();
		uint loopStart=0, loopEnd=alignObjectsCount;
		switch ( currAlignTo ) 
		{
			case First:
				newY = (*alignObjects)[0].y1 + ((*alignObjects)[0].y2-(*alignObjects)[0].y1)/2;
				loopStart=1;
				break;
			case Last:
				{
					int objindex=alignObjectsCount-1;
					newY = (*alignObjects)[objindex].y1 + ((*alignObjects)[objindex].y2-(*alignObjects)[objindex].y1)/2;
					loopEnd=alignObjectsCount-2;
				}
				break;
			case Page:
				newY = currDoc->ScratchTop;
				if (currDoc->PageFP && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newY += currDoc->pageHeight;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newY += currDoc->pageHeight;
				}
				newY += currDoc->pageHeight/2;
				break;
			case Margins:
				newY = currDoc->ScratchTop;
				if (currDoc->PageFP && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newY += currDoc->pageHeight;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newY += currDoc->pageHeight;
				}
				newY += currDoc->pageMargins.Left;
				newY += (currDoc->pageHeight - currDoc->pageMargins.Right - currDoc->pageMargins.Left)/2;
				break;
			case Selection:
				double minX=99999.9, maxX=-99999.9;
				for (uint a = 0; a < alignObjectsCount; ++a)
				{
					minX = QMIN((*alignObjects)[a].y1, minX);
					maxX = QMAX((*alignObjects)[a].y2, maxX);
				}
				newY = minX + (maxX-minX)/2;
				break;
		}
		for (uint i = loopStart; i <= loopEnd; ++i)
		{
			double diff=newY-(*alignObjects)[i].y1-((*alignObjects)[i].y2-(*alignObjects)[i].y1)/2;
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
		double newY = -99999.9;
		
		startAlign();
		uint loopStart=0, loopEnd=alignObjectsCount;
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
				if (currDoc->PageFP && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newY += currDoc->pageHeight;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newY += currDoc->pageHeight;
				}
				newY += currDoc->pageHeight;
				break;
			case Margins:
				newY = currDoc->ScratchTop;
				if (currDoc->PageFP && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newY += currDoc->pageHeight;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newY += currDoc->pageHeight;
				}
				newY += currDoc->pageHeight;
				newY -= currDoc->pageMargins.Right;
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
		double newY = -99999.9;
		
		startAlign();
		uint loopStart=0, loopEnd=alignObjectsCount;
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
				if (currDoc->PageFP && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newY += currDoc->pageHeight;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newY += currDoc->pageHeight;
				}
				newY += currDoc->pageHeight;
				break;
			case Margins:
				newY = currDoc->ScratchTop;
				if (currDoc->PageFP && !currDoc->MasterP)
				{
					if ((currDoc->currentPage->PageNr % 2 == 1) && (currDoc->FirstPageLeft))
						newY += currDoc->pageHeight;
					if ((currDoc->currentPage->PageNr % 2 == 0) && (!currDoc->FirstPageLeft))
						newY += currDoc->pageHeight;
				}
				newY += currDoc->pageHeight;
				newY -= currDoc->pageMargins.Right;
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




void AlignDistributePalette::distributeDistH()
{
}

void AlignDistributePalette::distributeRight()
{
}

void AlignDistributePalette::distributeBottom()
{
}

void AlignDistributePalette::distributeCenterH()
{
}

void AlignDistributePalette::distributeDistV()
{
}

void AlignDistributePalette::distributeLeft()
{
}

void AlignDistributePalette::distributeCenterV()
{
}

void AlignDistributePalette::distributeTop()
{
}

void AlignDistributePalette::alignToChanged(int newAlignTo)
{
	currAlignTo=(AlignTo)newAlignTo;	
}
