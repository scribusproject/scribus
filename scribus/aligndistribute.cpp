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
//#include "aligndistribute.moc"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qlineedit.h>
#include <qtabwidget.h>

#include "page.h"
#include "scmessagebox.h"
#include "scribusdoc.h"
#include "selection.h"
#include "undomanager.h"
#include "scrspinbox.h"
#include "units.h"
#include "commonstrings.h"

extern QPixmap loadIcon(QString nam);

//TODO Distribute with 

AlignDistributePalette::AlignDistributePalette( QWidget* parent, const char* name, bool /*modal*/, Qt::WFlags /*fl*/ )
	: ScrPaletteBase( parent, name )
{
	setupUi(this);
	if ( !name )
		setName( "AlignDistributePalette" );
	currDoc=NULL;
	
	//hide spare controls 
	toolButtonDummy1->hide();
	toolButtonDummy2->hide();

	//set up mspinboxes
	distributeDistScrSpinBox->setValues(-1000.0, 1000.0, 2, 0.0);

	// buddies
	alignRelativeToLabel->setBuddy( alignRelativeToCombo );
	alignGuideLabel->setBuddy( alignGuideLineEdit );
	distributeDistLabel->setBuddy( distributeDistScrSpinBox );
	
	resize( QSize(100, 100).expandedTo(minimumSizeHint()) );
	init();
	languageChange();
	setDoc(NULL);
}

AlignDistributePalette::~AlignDistributePalette()
{
}

void AlignDistributePalette::languageChange()
{
	setCaption( tr( "Align and Distribute" ) );
	tabWidget->changeTab(tabWidget->page(0), tr( "Align"));
	tabWidget->changeTab(tabWidget->page(1), tr( "Distribute"));
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
	QToolTip::add( alignLeftOutToolButton, tr( "Align right sides of items to left side of anchor" ) );
	alignRightOutToolButton->setText( QString::null );
	QToolTip::add( alignRightOutToolButton, tr( "Align left sides of items to right side of anchor" ) );
	alignBottomInToolButton->setText( QString::null );
	QToolTip::add( alignBottomInToolButton, tr( "Align bottoms" ) );
	alignRightInToolButton->setText( QString::null );
	QToolTip::add( alignRightInToolButton, tr( "Align right sides" ) );
	alignBottomOutToolButton->setText( QString::null );
	QToolTip::add( alignBottomOutToolButton, tr( "Align tops of items to bottom of anchor" ) );
	alignCenterHorToolButton->setText( QString::null );
	QToolTip::add( alignCenterHorToolButton, tr( "Center on vertical axis" ) );
	alignLeftInToolButton->setText( QString::null );
	QToolTip::add( alignLeftInToolButton, tr( "Align left sides" ) );
	alignCenterVerToolButton->setText( QString::null );
	QToolTip::add( alignCenterVerToolButton, tr( "Center on horizontal axis" ) );
	alignTopOutToolButton->setText( QString::null );
	QToolTip::add( alignTopOutToolButton, tr( "Align bottoms of items to top of anchor" ) );
	alignTopInToolButton->setText( QString::null );
	QToolTip::add( alignTopInToolButton, tr( "Align tops" ) );
	
	alignGuideLabel->setText( tr( "&Selected Guide:" ) );
	
	distributeDistHToolButton->setText( QString::null );
	QToolTip::add( distributeDistHToolButton, tr( "Make horizontal gaps between items equal" ) );
	distributeDistValueHToolButton->setText( QString::null );
	QToolTip::add( distributeDistValueHToolButton, tr( "Make horizontal gaps between items equal to the value specified" ) );
	
	distributeRightToolButton->setText( QString::null );
	distributeRightToolButton->setTextLabel( tr( "Distribute right sides equidistantly" ) );
	distributeBottomToolButton->setText( QString::null );
	QToolTip::add( distributeBottomToolButton, tr( "Distribute bottoms equidistantly" ) );
	distributeCenterHToolButton->setText( QString::null );
	QToolTip::add( distributeCenterHToolButton, tr( "Distribute centers equidistantly horizontally" ) );
	distributeDistVToolButton->setText( QString::null );
	QToolTip::add( distributeDistVToolButton, tr( "Make vertical gaps between items equal" ) );
	distributeDistValueVToolButton->setText( QString::null );
	QToolTip::add( distributeDistValueVToolButton, tr( "Make vertical gaps between items equal to the value specified" ) );
	distributeLeftToolButton->setText( QString::null );
	QToolTip::add( distributeLeftToolButton, tr( "Distribute left sides equidistantly" ) );
	distributeCenterVToolButton->setText( QString::null );
	QToolTip::add( distributeCenterVToolButton, tr( "Distribute centers equidistantly vertically" ) );
	distributeTopToolButton->setText( QString::null );
	QToolTip::add( distributeTopToolButton, tr( "Distribute tops equidistantly" ) );
	distributeAcrossPageToolButton->setText( QString::null );
	QToolTip::add( distributeAcrossPageToolButton, tr( "Make horizontal gaps between items and sides of page equal" ) );
	distributeDownPageToolButton->setText( QString::null );
	QToolTip::add( distributeDownPageToolButton, tr( "Make vertical gaps between items and the top and bottom of page equal" ) );
	distributeAcrossMarginsToolButton->setText( QString::null );
	QToolTip::add( distributeAcrossMarginsToolButton, tr( "Make horizontal gaps between items and sides of page margins equal" ) );
	distributeDownMarginsToolButton->setText( QString::null );
	QToolTip::add( distributeDownMarginsToolButton, tr( "Make vertical gaps between items and the top and bottom of page margins equal" ) );

	distributeDistLabel->setText( tr( "&Distance:" ) );
	QToolTip::add( distributeDistScrSpinBox, tr( "Distribute the items with the distance specified" ) );
	
	guideInfoTextNone = tr("None Selected");
}

void AlignDistributePalette::init()
{
	undoManager = UndoManager::instance();

// GUI3-4 compile PV
// 	alignLeftOutToolButton->setIconSet(QIcon(loadIcon("22/align-horizontal-left-out.png"), QIcon::Small));
// 	alignLeftInToolButton->setIconSet(QIcon(loadIcon("22/align-horizontal-left.png"), QIcon::Small));
// 	alignCenterHorToolButton->setIconSet(QIcon(loadIcon("22/align-horizontal-center.png"), QIcon::Small));
// 	alignRightInToolButton->setIconSet(QIcon(loadIcon("22/align-horizontal-right.png"), QIcon::Small));
// 	alignRightOutToolButton->setIconSet(QIcon(loadIcon("22/align-horizontal-right-out.png"), QIcon::Small));
// 	
// 	alignTopOutToolButton->setIconSet(QIcon(loadIcon("22/align-vertical-top-out.png"), QIcon::Small));
// 	alignTopInToolButton->setIconSet(QIcon(loadIcon("22/align-vertical-top.png"), QIcon::Small));
// 	alignCenterVerToolButton->setIconSet(QIcon(loadIcon("22/align-vertical-center.png"), QIcon::Small));
// 	alignBottomInToolButton->setIconSet(QIcon(loadIcon("22/align-vertical-bottom.png"), QIcon::Small));
// 	alignBottomOutToolButton->setIconSet(QIcon(loadIcon("22/align-vertical-bottom-out.png"), QIcon::Small));
// 	
// 	distributeLeftToolButton->setIconSet(QIcon(loadIcon("22/distribute-horizontal-left.png"), QIcon::Small));
// 	distributeCenterHToolButton->setIconSet(QIcon(loadIcon("22/distribute-horizontal-center.png"), QIcon::Small));
// 	distributeRightToolButton->setIconSet(QIcon(loadIcon("22/distribute-horizontal-right.png"), QIcon::Small));
// 	distributeDistHToolButton->setIconSet(QIcon(loadIcon("22/distribute-horizontal-equal.png"), QIcon::Small));
// 	
// 	distributeBottomToolButton->setIconSet(QIcon(loadIcon("22/distribute-vertical-bottom.png"), QIcon::Small));
// 	distributeCenterVToolButton->setIconSet(QIcon(loadIcon("22/distribute-vertical-center.png"), QIcon::Small));
// 	distributeTopToolButton->setIconSet(QIcon(loadIcon("22/distribute-vertical-top.png"), QIcon::Small));
// 	distributeDistVToolButton->setIconSet(QIcon(loadIcon("22/distribute-vertical-equal.png"), QIcon::Small));
// 	
// 	distributeAcrossPageToolButton->setIconSet(QIcon(loadIcon("distribute_acrosspage.png"), QIcon::Small));
// 	distributeDownPageToolButton->setIconSet(QIcon(loadIcon("distribute_downpage.png"), QIcon::Small));
// 	distributeAcrossMarginsToolButton->setIconSet(QIcon(loadIcon("distribute_acrossmargins.png"), QIcon::Small));
// 	distributeDownMarginsToolButton->setIconSet(QIcon(loadIcon("distribute_downmargins.png"), QIcon::Small));
// 	distributeDistValueHToolButton->setIconSet(QIcon(loadIcon("distribute_hdist_val.png"), QIcon::Small));
// 	distributeDistValueVToolButton->setIconSet(QIcon(loadIcon("distribute_vdist_val.png"), QIcon::Small));
	
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
	connect(distributeAcrossPageToolButton, SIGNAL(clicked()), this, SLOT(distributeDistAcrossPage()));
	connect(distributeDownPageToolButton, SIGNAL(clicked()), this, SLOT(distributeDistDownPage()));
	connect(distributeAcrossMarginsToolButton, SIGNAL(clicked()), this, SLOT(distributeDistAcrossMargins()));
	connect(distributeDownMarginsToolButton, SIGNAL(clicked()), this, SLOT(distributeDistDownMargins()));
	
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
		double oldValue=distributeDistScrSpinBox->value();
		double oldRatio=unitRatio;
		distributeDistScrSpinBox->setDecimals(unitGetDecimalsFromIndex(currDoc->unitIndex()));
		distributeDistScrSpinBox->setSuffix(unitGetSuffixFromIndex(currDoc->unitIndex()));
		unitRatio=unitGetRatioFromIndex(currDoc->unitIndex());
		double ratioDivisor =  unitRatio / oldRatio;
		distributeDistScrSpinBox->setValue(oldValue*ratioDivisor);
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
		currDoc->itemSelection_DistributeDistH(usingDistance, distributeDistScrSpinBox->value());
}

void AlignDistributePalette::distributeDistValH()
{
	if (currDoc!=NULL)
		distributeDistH(true);
}

void AlignDistributePalette::distributeDistAcrossPage()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_DistributeAcrossPage();
}

void AlignDistributePalette::distributeDistDownPage()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_DistributeDownPage();
}

void AlignDistributePalette::distributeDistAcrossMargins()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_DistributeAcrossPage(true);
}

void AlignDistributePalette::distributeDistDownMargins()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_DistributeDownPage(true);
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
		currDoc->itemSelection_DistributeDistV(usingDistance, distributeDistScrSpinBox->value());
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
