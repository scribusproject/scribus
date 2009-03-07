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
#include <QPixmap>
#include <QPushButton>
#include <QTabWidget>
#include <QToolButton>
#include <QToolTip>
#include <QVariant>

#include "commonstrings.h"
#include "page.h"
#include "scmessagebox.h"
#include "scribusdoc.h"
#include "scrspinbox.h"
#include "selection.h"
#include "undomanager.h"
#include "units.h"
#include "util_icon.h"
#include "scribusstructs.h"

//TODO Distribute with 

AlignDistributePalette::AlignDistributePalette( QWidget* parent, const char* name, bool /*modal*/, Qt::WFlags /*fl*/ )
	: ScrPaletteBase( parent, name )
{
	setupUi(this);
	currDoc=NULL;
	
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
		QWidget::changeEvent(e);
}

void AlignDistributePalette::languageChange()
{
	setWindowTitle( tr( "Align and Distribute" ) );
	tabWidget->setTabText(0, tr( "Align"));
	tabWidget->setTabText(1, tr( "Distribute"));
	alignRelativeToLabel->setText( tr( "&Relative to:" ) );
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
	alignGuideLineEdit->setToolTip( tr( "The location of the selected guide to align to" ) );
	alignLeftOutToolButton->setText( QString::null );
	alignLeftOutToolButton->setToolTip( tr( "Align right sides of items to left side of anchor" ) );
	alignRightOutToolButton->setText( QString::null );
	alignRightOutToolButton->setToolTip( tr( "Align left sides of items to right side of anchor" ) );
	alignBottomInToolButton->setText( QString::null );
	alignBottomInToolButton->setToolTip( tr( "Align bottoms" ) );
	alignRightInToolButton->setText( QString::null );
	alignRightInToolButton->setToolTip( tr( "Align right sides" ) );
	alignBottomOutToolButton->setText( QString::null );
	alignBottomOutToolButton->setToolTip( tr( "Align tops of items to bottom of anchor" ) );
	alignCenterHorToolButton->setText( QString::null );
	alignCenterHorToolButton->setToolTip( tr( "Center on vertical axis" ) );
	alignLeftInToolButton->setText( QString::null );
	alignLeftInToolButton->setToolTip( tr( "Align left sides" ) );
	alignCenterVerToolButton->setText( QString::null );
	alignCenterVerToolButton->setToolTip( tr( "Center on horizontal axis" ) );
	alignTopOutToolButton->setText( QString::null );
	alignTopOutToolButton->setToolTip( tr( "Align bottoms of items to top of anchor" ) );
	alignTopInToolButton->setText( QString::null );
	alignTopInToolButton->setToolTip( tr( "Align tops" ) );
	
	alignGuideLabel->setText( tr( "&Selected Guide:" ) );
	
	distributeDistHToolButton->setText( QString::null );
	distributeDistHToolButton->setToolTip( tr( "Make horizontal gaps between items equal" ) );
	distributeDistValueHToolButton->setText( QString::null );
	distributeDistValueHToolButton->setToolTip( tr( "Make horizontal gaps between items equal to the value specified" ) );
	
	distributeRightToolButton->setText( QString::null );
	distributeRightToolButton->setToolTip( tr( "Distribute right sides equidistantly" ) );
	distributeBottomToolButton->setText( QString::null );
	distributeBottomToolButton->setToolTip( tr( "Distribute bottoms equidistantly" ) );
	distributeCenterHToolButton->setText( QString::null );
	distributeCenterHToolButton->setToolTip( tr( "Distribute centers equidistantly horizontally" ) );
	distributeDistVToolButton->setText( QString::null );
	distributeDistVToolButton->setToolTip( tr( "Make vertical gaps between items equal" ) );
	distributeDistValueVToolButton->setText( QString::null );
	distributeDistValueVToolButton->setToolTip( tr( "Make vertical gaps between items equal to the value specified" ) );
	distributeLeftToolButton->setText( QString::null );
	distributeLeftToolButton->setToolTip( tr( "Distribute left sides equidistantly" ) );
	distributeCenterVToolButton->setText( QString::null );
	distributeCenterVToolButton->setToolTip( tr( "Distribute centers equidistantly vertically" ) );
	distributeTopToolButton->setText( QString::null );
	distributeTopToolButton->setToolTip( tr( "Distribute tops equidistantly" ) );
	distributeAcrossPageToolButton->setText( QString::null );
	distributeAcrossPageToolButton->setToolTip( tr( "Make horizontal gaps between items and sides of page equal" ) );
	distributeDownPageToolButton->setText( QString::null );
	distributeDownPageToolButton->setToolTip( tr( "Make vertical gaps between items and the top and bottom of page equal" ) );
	distributeAcrossMarginsToolButton->setText( QString::null );
	distributeAcrossMarginsToolButton->setToolTip( tr( "Make horizontal gaps between items and sides of page margins equal" ) );
	distributeDownMarginsToolButton->setText( QString::null );
	distributeDownMarginsToolButton->setToolTip( tr( "Make vertical gaps between items and the top and bottom of page margins equal" ) );

	distributeDistLabel->setText( tr( "&Distance:" ) );
	distributeDistSpinBox->setToolTip( tr( "Distribute the items with the distance specified" ) );
	
	guideInfoTextNone = tr("None Selected");
}

void AlignDistributePalette::init()
{
	undoManager = UndoManager::instance();

// GUI3-4 compile PV
	alignLeftOutToolButton->setIcon(QIcon(loadIcon("22/align-horizontal-left-out.png")));
	alignLeftInToolButton->setIcon(QIcon(loadIcon("22/align-horizontal-left.png")));
	alignCenterHorToolButton->setIcon(QIcon(loadIcon("22/align-horizontal-center.png")));
	alignRightInToolButton->setIcon(QIcon(loadIcon("22/align-horizontal-right.png")));
	alignRightOutToolButton->setIcon(QIcon(loadIcon("22/align-horizontal-right-out.png")));
 	
	alignTopOutToolButton->setIcon(QIcon(loadIcon("22/align-vertical-top-out.png")));
	alignTopInToolButton->setIcon(QIcon(loadIcon("22/align-vertical-top.png")));
	alignCenterVerToolButton->setIcon(QIcon(loadIcon("22/align-vertical-center.png")));
	alignBottomInToolButton->setIcon(QIcon(loadIcon("22/align-vertical-bottom.png")));
	alignBottomOutToolButton->setIcon(QIcon(loadIcon("22/align-vertical-bottom-out.png")));
 	
	distributeLeftToolButton->setIcon(QIcon(loadIcon("22/distribute-horizontal-left.png")));
	distributeCenterHToolButton->setIcon(QIcon(loadIcon("22/distribute-horizontal-center.png")));
	distributeRightToolButton->setIcon(QIcon(loadIcon("22/distribute-horizontal-right.png")));
	distributeDistHToolButton->setIcon(QIcon(loadIcon("22/distribute-horizontal-equal.png")));
 	
	distributeBottomToolButton->setIcon(QIcon(loadIcon("22/distribute-vertical-bottom.png")));
	distributeCenterVToolButton->setIcon(QIcon(loadIcon("22/distribute-vertical-center.png")));
	distributeTopToolButton->setIcon(QIcon(loadIcon("22/distribute-vertical-top.png")));
	distributeDistVToolButton->setIcon(QIcon(loadIcon("22/distribute-vertical-equal.png")));
 	
	
	distributeAcrossPageToolButton->setIcon(QIcon(loadIcon("22/distribute-horizontal-page.png")));
	distributeDownPageToolButton->setIcon(QIcon(loadIcon("22/distribute-vertical-page.png")));
	distributeAcrossMarginsToolButton->setIcon(QIcon(loadIcon("22/distribute-horizontal-margin.png")));
	distributeDownMarginsToolButton->setIcon(QIcon(loadIcon("22/distribute-vertical-margin.png")));
	distributeDistValueHToolButton->setIcon(QIcon(loadIcon("22/distribute-horizontal-x.png")));
	distributeDistValueVToolButton->setIcon(QIcon(loadIcon("22/distribute-vertical-y.png")));

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
	
	alignRelativeToCombo->setCurrentIndex(0);
	alignToChanged(0);
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
		currDoc->itemSelection_DistributeDistH(usingDistance, distributeDistSpinBox->value());
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
		currDoc->itemSelection_DistributeDistV(usingDistance, distributeDistSpinBox->value());
}

void AlignDistributePalette::distributeDistValV()
{
	if (currDoc!=NULL)
		distributeDistV(true);
}

void AlignDistributePalette::alignToChanged(int newAlignTo)
{
	currAlignTo = ScribusDoc::AlignTo(newAlignTo);
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
