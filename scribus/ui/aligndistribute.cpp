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

#include <QComboBox>
#include <QEvent>
#include <QImage>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QTabWidget>
#include <QToolButton>
#include <QToolTip>
#include <QVariant>

#include "commonstrings.h"
#include "iconmanager.h"
#include "scpage.h"
#include "scribusdoc.h"
#include "scribusstructs.h"
#include "selection.h"
#include "ui/scrspinbox.h"
#include "undomanager.h"
#include "units.h"

//TODO Distribute with 

AlignDistributePalette::AlignDistributePalette( QWidget* parent, const char* name) : ScDockPalette( parent, name, 0 )
{
	setupUi(this);
	setSizePolicy( QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
	currDoc=NULL;
	guideDirection=-1;
	setObjectName(name);
	
	//hide spare controls 
	toolButtonDummy1->hide();
	toolButtonDummy2->hide();

	//set up scrspinboxes
	distributeDistSpinBox->setValues(-1000.0, 1000.0, 2, 0.0);

	// buddies
	alignRelativeToLabel->setBuddy( alignRelativeToCombo );
	alignGuideLabel->setBuddy( alignGuideLineEdit );
	distributeDistLabel->setBuddy( distributeDistSpinBox );
	
	resize( QSize(100, 100).expandedTo(minimumSizeHint()) );
	languageChange();
	init();
	setDoc(NULL);
}

AlignDistributePalette::~AlignDistributePalette()
{
}

void AlignDistributePalette::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		ScDockPalette::changeEvent(e);
}

void AlignDistributePalette::languageChange()
{
	retranslateUi(this);

	int alignComboValue=alignRelativeToCombo->currentIndex();
	alignRelativeToCombo->clear();
	alignRelativeToCombo->addItem( tr( "First Selected" ) );
	alignRelativeToCombo->addItem( tr( "Last Selected" ) );
	alignRelativeToCombo->addItem( tr( "Page" ) );
	alignRelativeToCombo->addItem( tr( "Margins" ) );
	alignRelativeToCombo->addItem( tr( "Guide" ) );		
	alignRelativeToCombo->addItem( tr( "Selection" ) );
	alignRelativeToCombo->setCurrentIndex(alignComboValue);
	alignRelativeToCombo->setToolTip( tr( "<qt>Align relative to the:<ul><li>First selected item</li><li>Second Selected Item</li><li>The current page</li><li>The margins of the current page</li><li>A Guide</li><li>The selection</ul></qt>" ) );
	alignToChanged(alignComboValue);

	int alignMethodValue=alignMoveOrResizeCombo->currentIndex();
	alignMoveOrResizeCombo->clear();
	alignMoveOrResizeCombo->addItem( tr("Move") );
	alignMoveOrResizeCombo->addItem( tr("Resize") );
	alignMoveOrResizeCombo->setToolTip( tr( "<qt>When aligning one side of an item do one of the following:<ul><li>Always move the other side too (preserve existing width and height)</li><li>Keep the other side fixed (resize the item instead of moving it) whenever possible</li></ul></qt>" ));
	alignMethodChanged(alignMethodValue);

	alignGuideLineEdit->setToolTip( tr( "The location of the selected guide to align to" ) );
	alignLeftOutToolButton->setToolTip( tr( "Align right sides of items to left side of anchor" ) );
	alignRightOutToolButton->setToolTip( tr( "Align left sides of items to right side of anchor" ) );
	alignBottomInToolButton->setToolTip( tr( "Align bottoms" ) );
	alignRightInToolButton->setToolTip( tr( "Align right sides" ) );
	alignBottomOutToolButton->setToolTip( tr( "Align tops of items to bottom of anchor" ) );
	alignCenterHorToolButton->setToolTip( tr( "Center on vertical axis" ) );
	alignLeftInToolButton->setToolTip( tr( "Align left sides" ) );
	alignCenterVerToolButton->setToolTip( tr( "Center on horizontal axis" ) );
	alignTopOutToolButton->setToolTip( tr( "Align bottoms of items to top of anchor" ) );
	alignTopInToolButton->setToolTip( tr( "Align tops" ) );
		
	distributeDistHToolButton->setToolTip( tr( "Make horizontal gaps between items equal" ) );
	distributeDistValueHToolButton->setToolTip( tr( "Make horizontal gaps between items equal to the value specified" ) );
	
	distributeRightToolButton->setToolTip( tr( "Distribute right sides equidistantly" ) );
	distributeBottomToolButton->setToolTip( tr( "Distribute bottoms equidistantly" ) );
	distributeCenterHToolButton->setToolTip( tr( "Distribute centers equidistantly horizontally" ) );
	distributeDistVToolButton->setToolTip( tr( "Make vertical gaps between items equal" ) );
	distributeDistValueVToolButton->setToolTip( tr( "Make vertical gaps between items equal to the value specified" ) );
	distributeLeftToolButton->setToolTip( tr( "Distribute left sides equidistantly" ) );
	distributeCenterVToolButton->setToolTip( tr( "Distribute centers equidistantly vertically" ) );
	distributeTopToolButton->setToolTip( tr( "Distribute tops equidistantly" ) );
	distributeAcrossPageToolButton->setToolTip( tr( "Make horizontal gaps between items and sides of page equal" ) );
	distributeDownPageToolButton->setToolTip( tr( "Make vertical gaps between items and the top and bottom of page equal" ) );
	distributeAcrossMarginsToolButton->setToolTip( tr( "Make horizontal gaps between items and sides of page margins equal" ) );
	distributeDownMarginsToolButton->setToolTip( tr( "Make vertical gaps between items and the top and bottom of page margins equal" ) );

	distributeDistSpinBox->setToolTip( tr( "Distribute the items with the distance specified" ) );
	reverseDistributionCheckBox->setToolTip( tr("When distributing by a set distance, reverse the direction of the distribution of items") );
	
	guideInfoTextNone = tr("None Selected");

	swapLeftToolButton->setToolTip( tr( "Swap items to the left" ) );
	swapRightToolButton->setToolTip( tr( "Swap items to the right" ) );
}

void AlignDistributePalette::init()
{
	undoManager = UndoManager::instance();
	IconManager* im = IconManager::instance();
	alignLeftOutToolButton->setIcon(im->loadIcon("22/align-horizontal-left-out.png"));
	alignLeftInToolButton->setIcon(im->loadIcon("22/align-horizontal-left.png"));
	alignCenterHorToolButton->setIcon(im->loadIcon("22/align-horizontal-center.png"));
	alignRightInToolButton->setIcon(im->loadIcon("22/align-horizontal-right.png"));
	alignRightOutToolButton->setIcon(im->loadIcon("22/align-horizontal-right-out.png"));

	alignTopOutToolButton->setIcon(im->loadIcon("22/align-vertical-top-out.png"));
	alignTopInToolButton->setIcon(im->loadIcon("22/align-vertical-top.png"));
	alignCenterVerToolButton->setIcon(im->loadIcon("22/align-vertical-center.png"));
	alignBottomInToolButton->setIcon(im->loadIcon("22/align-vertical-bottom.png"));
	alignBottomOutToolButton->setIcon(im->loadIcon("22/align-vertical-bottom-out.png"));

	distributeLeftToolButton->setIcon(im->loadIcon("22/distribute-horizontal-left.png"));
	distributeCenterHToolButton->setIcon(im->loadIcon("22/distribute-horizontal-center.png"));
	distributeRightToolButton->setIcon(im->loadIcon("22/distribute-horizontal-right.png"));
	distributeDistHToolButton->setIcon(im->loadIcon("22/distribute-horizontal-equal.png"));

	distributeBottomToolButton->setIcon(im->loadIcon("22/distribute-vertical-bottom.png"));
	distributeCenterVToolButton->setIcon(im->loadIcon("22/distribute-vertical-center.png"));
	distributeTopToolButton->setIcon(im->loadIcon("22/distribute-vertical-top.png"));
	distributeDistVToolButton->setIcon(im->loadIcon("22/distribute-vertical-equal.png"));

	distributeAcrossPageToolButton->setIcon(im->loadIcon("22/distribute-horizontal-page.png"));
	distributeDownPageToolButton->setIcon(im->loadIcon("22/distribute-vertical-page.png"));
	distributeAcrossMarginsToolButton->setIcon(im->loadIcon("22/distribute-horizontal-margin.png"));
	distributeDownMarginsToolButton->setIcon(im->loadIcon("22/distribute-vertical-margin.png"));
	distributeDistValueHToolButton->setIcon(im->loadIcon("22/distribute-horizontal-x.png"));
	distributeDistValueVToolButton->setIcon(im->loadIcon("22/distribute-vertical-y.png"));

	swapLeftToolButton->setIcon(im->loadIcon("22/swap-left.png"));
	swapRightToolButton->setIcon(im->loadIcon("22/swap-right.png"));

	QSize s(24,24);
	alignLeftOutToolButton->setMinimumSize(s);
	alignLeftInToolButton->setMinimumSize(s);
	alignCenterHorToolButton->setMinimumSize(s);
	alignRightInToolButton->setMinimumSize(s);
	alignRightOutToolButton->setMinimumSize(s);

	alignTopOutToolButton->setMinimumSize(s);
	alignTopInToolButton->setMinimumSize(s);
	alignCenterVerToolButton->setMinimumSize(s);
	alignBottomInToolButton->setMinimumSize(s);
	alignBottomOutToolButton->setMinimumSize(s);

	distributeLeftToolButton->setMinimumSize(s);
	distributeCenterHToolButton->setMinimumSize(s);
	distributeRightToolButton->setMinimumSize(s);
	distributeDistHToolButton->setMinimumSize(s);

	distributeBottomToolButton->setMinimumSize(s);
	distributeCenterVToolButton->setMinimumSize(s);
	distributeTopToolButton->setMinimumSize(s);
	distributeDistVToolButton->setMinimumSize(s);

	distributeAcrossPageToolButton->setMinimumSize(s);
	distributeDownPageToolButton->setMinimumSize(s);
	distributeAcrossMarginsToolButton->setMinimumSize(s);
	distributeDownMarginsToolButton->setMinimumSize(s);
	distributeDistValueHToolButton->setMinimumSize(s);
	distributeDistValueVToolButton->setMinimumSize(s);

	swapLeftToolButton->setMinimumSize(s);
	swapRightToolButton->setMinimumSize(s);

	alignLeftOutToolButton->setMaximumSize(s);
	alignLeftInToolButton->setMaximumSize(s);
	alignCenterHorToolButton->setMaximumSize(s);
	alignRightInToolButton->setMaximumSize(s);
	alignRightOutToolButton->setMaximumSize(s);

	alignTopOutToolButton->setMaximumSize(s);
	alignTopInToolButton->setMaximumSize(s);
	alignCenterVerToolButton->setMaximumSize(s);
	alignBottomInToolButton->setMaximumSize(s);
	alignBottomOutToolButton->setMaximumSize(s);

	distributeLeftToolButton->setMaximumSize(s);
	distributeCenterHToolButton->setMaximumSize(s);
	distributeRightToolButton->setMaximumSize(s);
	distributeDistHToolButton->setMaximumSize(s);

	distributeBottomToolButton->setMaximumSize(s);
	distributeCenterVToolButton->setMaximumSize(s);
	distributeTopToolButton->setMaximumSize(s);
	distributeDistVToolButton->setMaximumSize(s);

	distributeAcrossPageToolButton->setMaximumSize(s);
	distributeDownPageToolButton->setMaximumSize(s);
	distributeAcrossMarginsToolButton->setMaximumSize(s);
	distributeDownMarginsToolButton->setMaximumSize(s);
	distributeDistValueHToolButton->setMaximumSize(s);
	distributeDistValueVToolButton->setMaximumSize(s);

	swapLeftToolButton->setMaximumSize(s);
	swapRightToolButton->setMaximumSize(s);


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
	connect(swapLeftToolButton, SIGNAL(clicked()), this, SLOT(swapLeft()));
	connect(swapRightToolButton, SIGNAL(clicked()), this, SLOT(swapRight()));
	
	alignRelativeToCombo->setCurrentIndex(0);
	alignToChanged(0);
	alignMethodChanged(0);
	connect(alignRelativeToCombo, SIGNAL(activated(int)), this, SLOT(alignToChanged(int)));
	connect(alignMoveOrResizeCombo, SIGNAL(activated(int)), this, SLOT(alignMethodChanged(int)));
	
	unitRatio=1.0;
	guideDirection=-1;
	
	guideInfoText = guideInfoTextNone;
	alignGuideLineEdit->setText(guideInfoTextNone);
}

void AlignDistributePalette::unitChange()
{
	if (currDoc!=NULL)
	{
		unitRatio=unitGetRatioFromIndex(currDoc->unitIndex());
		distributeDistSpinBox->setNewUnit(currDoc->unitIndex());
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
		currDoc->itemSelection_AlignLeftOut(currAlignTo, currAlignMethod, guidePosition);
}

void AlignDistributePalette::alignLeftIn()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_AlignLeftIn(currAlignTo, currAlignMethod, guidePosition);
}

void AlignDistributePalette::alignCenterHor()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_AlignCenterHor(currAlignTo, currAlignMethod, guidePosition);
}

void AlignDistributePalette::alignRightIn()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_AlignRightIn(currAlignTo, currAlignMethod, guidePosition);
}

void AlignDistributePalette::alignRightOut()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_AlignRightOut(currAlignTo, currAlignMethod, guidePosition);
}

void AlignDistributePalette::alignTopOut()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_AlignTopOut(currAlignTo, currAlignMethod, guidePosition);
}

void AlignDistributePalette::alignTopIn()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_AlignTopIn(currAlignTo, currAlignMethod, guidePosition);
}


void AlignDistributePalette::alignCenterVer()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_AlignCenterVer(currAlignTo, currAlignMethod, guidePosition);
}


void AlignDistributePalette::alignBottomIn()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_AlignBottomIn(currAlignTo, currAlignMethod, guidePosition);
}

void AlignDistributePalette::alignBottomOut()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_AlignBottomOut(currAlignTo, currAlignMethod, guidePosition);
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
		currDoc->itemSelection_DistributeDistH(usingDistance, distributeDistSpinBox->value(), reverseDistributionCheckBox->isChecked());
}

void AlignDistributePalette::distributeDistValH()
{
	distributeDistSpinBox->interpretText();
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
		currDoc->itemSelection_DistributeDistV(usingDistance, distributeDistSpinBox->value(), reverseDistributionCheckBox->isChecked());
}

void AlignDistributePalette::distributeDistValV()
{
	distributeDistSpinBox->interpretText();
	if (currDoc!=NULL)
		distributeDistV(true);
}

void AlignDistributePalette::swapLeft()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_SwapLeft();
}

void AlignDistributePalette::swapRight()
{
	if (currDoc!=NULL)
		currDoc->itemSelection_SwapRight();
}

void AlignDistributePalette::alignToChanged(int newAlignTo)
{
	currAlignTo = ScribusDoc::AlignTo(newAlignTo);
	enableGuideButtons();
}
void AlignDistributePalette::alignMethodChanged(int newAlignMethod)
{
	currAlignMethod = ScribusDoc::AlignMethod(newAlignMethod);
	enableGuideButtons();
}

void AlignDistributePalette::setGuide(int direction, qreal position)
{
	//direction 0=H, 1=V
// 	qDebug()<<"AlignDistributePalette::setGuide("<<direction<<""<<position<<")";
	guideDirection=direction;
	guidePosition=position;
	enableGuideButtons();
}

void AlignDistributePalette::enableGuideButtons()
{
	QString suffix;
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
