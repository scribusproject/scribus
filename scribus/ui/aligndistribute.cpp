/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
	begin                : June 2005
	copyright            : (C) 2005 by Craig Bradney
	email                : cbradney@scribus.info
***************************************************************************/

/***************************************************************************
*                                                                         *
*   Scribus program is free software; you can redistribute it and/or modify  *
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
#include <QLocale>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QTabWidget>
#include <QToolButton>
#include <QToolTip>
#include <QVariant>

#include "iconmanager.h"
#include "localemgr.h"
#include "scribusapp.h"
#include "scribusdoc.h"
#include "scribusstructs.h"
#include "ui/scrspinbox.h"
#include "undomanager.h"
#include "units.h"

//TODO Distribute with 

AlignDistribute::AlignDistribute(QWidget* parent) : QWidget(parent)
{
	setupUi(this);
}


// =============================


AlignDistributePalette::AlignDistributePalette(QWidget* parent) : DockPanelBase("AlignDistributePalette", parent)
{
	setSizePolicy( QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
	setObjectName("AlignDistributePalette");

	ad = new AlignDistribute(this);
	setWidget(ad);
	//set up scrspinboxes
	ad->distributeDistSpinBox->setValues(-10000.0, 10000.0, 2, 0.0);

	resize( QSize(100, 100).expandedTo(minimumSizeHint()) );
	languageChange();
	init();
	setDoc(nullptr);

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(ScQApp, SIGNAL(localeChanged()), this, SLOT(localeChange()));
}

void AlignDistributePalette::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	DockPanelBase::changeEvent(e);
}

void AlignDistributePalette::languageChange()
{
	ad->retranslateUi(this);

	int alignComboValue = ad->alignRelativeToCombo->currentIndex();
	ad->alignRelativeToCombo->clear();
	ad->alignRelativeToCombo->addItem( tr( "First Selected" ) );
	ad->alignRelativeToCombo->addItem( tr( "Last Selected" ) );
	ad->alignRelativeToCombo->addItem( tr( "Page" ) );
	ad->alignRelativeToCombo->addItem( tr( "Margins" ) );
	ad->alignRelativeToCombo->addItem( tr( "Guide" ) );
	ad->alignRelativeToCombo->addItem( tr( "Selection" ) );
	ad->alignRelativeToCombo->setCurrentIndex(alignComboValue);
	ad->alignRelativeToCombo->setToolTip( tr( "<qt>Align relative to the:<ul><li>First selected item</li><li>Second Selected Item</li><li>The current page</li><li>The margins of the current page</li><li>A Guide</li><li>The selection</ul></qt>" ) );
	alignToChanged(alignComboValue);

	int alignMethodValue = ad->alignMoveOrResizeCombo->currentIndex();
	ad->alignMoveOrResizeCombo->clear();
	ad->alignMoveOrResizeCombo->addItem( tr("Move") );
	ad->alignMoveOrResizeCombo->addItem( tr("Resize") );
	ad->alignMoveOrResizeCombo->setToolTip( tr( "<qt>When aligning one side of an item do one of the following:<ul><li>Always move the other side too (preserve existing width and height)</li><li>Keep the other side fixed (resize the item instead of moving it) whenever possible</li></ul></qt>" ));
	alignMethodChanged(alignMethodValue);

	ad->alignGuideLineEdit->setToolTip( tr( "The location of the selected guide to align to" ) );
	ad->alignLeftOutToolButton->setToolTip( tr( "Align right sides of items to left side of anchor" ) );
	ad->alignRightOutToolButton->setToolTip( tr( "Align left sides of items to right side of anchor" ) );
	ad->alignBottomInToolButton->setToolTip( tr( "Align bottoms" ) );
	ad->alignRightInToolButton->setToolTip( tr( "Align right sides" ) );
	ad->alignBottomOutToolButton->setToolTip( tr( "Align tops of items to bottom of anchor" ) );
	ad->alignCenterHorToolButton->setToolTip( tr( "Center on vertical axis" ) );
	ad->alignLeftInToolButton->setToolTip( tr( "Align left sides" ) );
	ad->alignCenterVerToolButton->setToolTip( tr( "Center on horizontal axis" ) );
	ad->alignTopOutToolButton->setToolTip( tr( "Align bottoms of items to top of anchor" ) );
	ad->alignTopInToolButton->setToolTip( tr( "Align tops" ) );

	ad->distributeDistHToolButton->setToolTip( tr( "Make horizontal gaps between items equal" ) );
	ad->distributeDistValueHToolButton->setToolTip( tr( "Make horizontal gaps between items equal to the value specified" ) );

	ad->distributeRightToolButton->setToolTip( tr( "Distribute right sides equidistantly" ) );
	ad->distributeBottomToolButton->setToolTip( tr( "Distribute bottoms equidistantly" ) );
	ad->distributeCenterHToolButton->setToolTip( tr( "Distribute centers equidistantly horizontally" ) );
	ad->distributeDistVToolButton->setToolTip( tr( "Make vertical gaps between items equal" ) );
	ad->distributeDistValueVToolButton->setToolTip( tr( "Make vertical gaps between items equal to the value specified" ) );
	ad->distributeLeftToolButton->setToolTip( tr( "Distribute left sides equidistantly" ) );
	ad->distributeCenterVToolButton->setToolTip( tr( "Distribute centers equidistantly vertically" ) );
	ad->distributeTopToolButton->setToolTip( tr( "Distribute tops equidistantly" ) );
	ad->distributeAcrossPageToolButton->setToolTip( tr( "Make horizontal gaps between items and sides of page equal" ) );
	ad->distributeDownPageToolButton->setToolTip( tr( "Make vertical gaps between items and the top and bottom of page equal" ) );
	ad->distributeAcrossMarginsToolButton->setToolTip( tr( "Make horizontal gaps between items and sides of page margins equal" ) );
	ad->distributeDownMarginsToolButton->setToolTip( tr( "Make vertical gaps between items and the top and bottom of page margins equal" ) );

	ad->distributeDistSpinBox->setToolTip( tr( "Distribute the items with the distance specified" ) );
	ad->reverseDistributionCheckBox->setToolTip( tr("When distributing by a set distance, reverse the direction of the distribution of items") );

	guideInfoTextNone = tr("None Selected");

	ad->swapLeftToolButton->setToolTip( tr( "Swap items to the left" ) );
	ad->swapRightToolButton->setToolTip( tr( "Swap items to the right" ) );
}

void AlignDistributePalette::init()
{
	undoManager = UndoManager::instance();

	iconSetChange();

	connect(ad->alignLeftOutToolButton, SIGNAL(clicked()), this, SLOT(alignLeftOut()));
	connect(ad->alignRightOutToolButton, SIGNAL(clicked()), this, SLOT(alignRightOut()));
	connect(ad->alignBottomInToolButton, SIGNAL(clicked()), this, SLOT(alignBottomIn()));
	connect(ad->alignRightInToolButton, SIGNAL(clicked()), this, SLOT(alignRightIn()));
	connect(ad->alignBottomOutToolButton, SIGNAL(clicked()), this, SLOT(alignBottomOut()));
	connect(ad->alignCenterHorToolButton, SIGNAL(clicked()), this, SLOT(alignCenterHor()));
	connect(ad->alignLeftInToolButton, SIGNAL(clicked()), this, SLOT(alignLeftIn()));
	connect(ad->alignCenterVerToolButton, SIGNAL(clicked()), this, SLOT(alignCenterVer()));
	connect(ad->alignTopOutToolButton, SIGNAL(clicked()), this, SLOT(alignTopOut()));
	connect(ad->alignTopInToolButton, SIGNAL(clicked()), this, SLOT(alignTopIn()));
	connect(ad->distributeDistHToolButton, SIGNAL(clicked()), this, SLOT(distributeDistH()));
	connect(ad->distributeDistValueHToolButton, SIGNAL(clicked()), this, SLOT(distributeDistValH()));
	connect(ad->distributeRightToolButton, SIGNAL(clicked()), this, SLOT(distributeRight()));
	connect(ad->distributeBottomToolButton, SIGNAL(clicked()), this, SLOT(distributeBottom()));
	connect(ad->distributeCenterHToolButton, SIGNAL(clicked()), this, SLOT(distributeCenterH()));
	connect(ad->distributeDistVToolButton, SIGNAL(clicked()), this, SLOT(distributeDistV()));
	connect(ad->distributeDistValueVToolButton, SIGNAL(clicked()), this, SLOT(distributeDistValV()));
	connect(ad->distributeLeftToolButton, SIGNAL(clicked()), this, SLOT(distributeLeft()));
	connect(ad->distributeCenterVToolButton, SIGNAL(clicked()), this, SLOT(distributeCenterV()));
	connect(ad->distributeTopToolButton, SIGNAL(clicked()), this, SLOT(distributeTop()));
	connect(ad->distributeAcrossPageToolButton, SIGNAL(clicked()), this, SLOT(distributeDistAcrossPage()));
	connect(ad->distributeDownPageToolButton, SIGNAL(clicked()), this, SLOT(distributeDistDownPage()));
	connect(ad->distributeAcrossMarginsToolButton, SIGNAL(clicked()), this, SLOT(distributeDistAcrossMargins()));
	connect(ad->distributeDownMarginsToolButton, SIGNAL(clicked()), this, SLOT(distributeDistDownMargins()));
	connect(ad->swapLeftToolButton, SIGNAL(clicked()), this, SLOT(swapLeft()));
	connect(ad->swapRightToolButton, SIGNAL(clicked()), this, SLOT(swapRight()));

	ad->alignRelativeToCombo->setCurrentIndex(0);
	alignToChanged(0);
	alignMethodChanged(0);
	connect(ad->alignRelativeToCombo, SIGNAL(activated(int)), this, SLOT(alignToChanged(int)));
	connect(ad->alignMoveOrResizeCombo, SIGNAL(activated(int)), this, SLOT(alignMethodChanged(int)));

	unitRatio = 1.0;
	guideDirection = -1;

	guideInfoText = guideInfoTextNone;
	ad->alignGuideLineEdit->setText(guideInfoTextNone);
}

void AlignDistributePalette::iconSetChange()
{
	IconManager& im = IconManager::instance();

	ad->alignLeftOutToolButton->setIcon(im.loadIcon("22/align-horizontal-left-out.png"));
	ad->alignLeftInToolButton->setIcon(im.loadIcon("22/align-horizontal-left.png"));
	ad->alignCenterHorToolButton->setIcon(im.loadIcon("22/align-horizontal-center.png"));
	ad->alignRightInToolButton->setIcon(im.loadIcon("22/align-horizontal-right.png"));
	ad->alignRightOutToolButton->setIcon(im.loadIcon("22/align-horizontal-right-out.png"));

	ad->alignTopOutToolButton->setIcon(im.loadIcon("22/align-vertical-top-out.png"));
	ad->alignTopInToolButton->setIcon(im.loadIcon("22/align-vertical-top.png"));
	ad->alignCenterVerToolButton->setIcon(im.loadIcon("22/align-vertical-center.png"));
	ad->alignBottomInToolButton->setIcon(im.loadIcon("22/align-vertical-bottom.png"));
	ad->alignBottomOutToolButton->setIcon(im.loadIcon("22/align-vertical-bottom-out.png"));

	ad->distributeLeftToolButton->setIcon(im.loadIcon("22/distribute-horizontal-left.png"));
	ad->distributeCenterHToolButton->setIcon(im.loadIcon("22/distribute-horizontal-center.png"));
	ad->distributeRightToolButton->setIcon(im.loadIcon("22/distribute-horizontal-right.png"));
	ad->distributeDistHToolButton->setIcon(im.loadIcon("22/distribute-horizontal-equal.png"));

	ad->distributeBottomToolButton->setIcon(im.loadIcon("22/distribute-vertical-bottom.png"));
	ad->distributeCenterVToolButton->setIcon(im.loadIcon("22/distribute-vertical-center.png"));
	ad->distributeTopToolButton->setIcon(im.loadIcon("22/distribute-vertical-top.png"));
	ad->distributeDistVToolButton->setIcon(im.loadIcon("22/distribute-vertical-equal.png"));

	ad->distributeAcrossPageToolButton->setIcon(im.loadIcon("22/distribute-horizontal-page.png"));
	ad->distributeDownPageToolButton->setIcon(im.loadIcon("22/distribute-vertical-page.png"));
	ad->distributeAcrossMarginsToolButton->setIcon(im.loadIcon("22/distribute-horizontal-margin.png"));
	ad->distributeDownMarginsToolButton->setIcon(im.loadIcon("22/distribute-vertical-margin.png"));
	ad->distributeDistValueHToolButton->setIcon(im.loadIcon("22/distribute-horizontal-x.png"));
	ad->distributeDistValueVToolButton->setIcon(im.loadIcon("22/distribute-vertical-y.png"));

	ad->swapLeftToolButton->setIcon(im.loadIcon("22/swap-left.png"));
	ad->swapRightToolButton->setIcon(im.loadIcon("22/swap-right.png"));
}

void AlignDistributePalette::unitChange()
{
	if (currDoc == nullptr)
		return;
	unitRatio = unitGetRatioFromIndex(currDoc->unitIndex());
	ad->distributeDistSpinBox->setNewUnit(currDoc->unitIndex());
	enableGuideButtons();
}

void AlignDistributePalette::setDoc(ScribusDoc* newDoc)
{
	currDoc = newDoc;
	if (currDoc != nullptr)
		alignObjects = &(currDoc->AObjects);
	else
		alignObjects = nullptr;
	unitChange();
}

void AlignDistributePalette::alignLeftOut()
{
	if (currDoc != nullptr)
		currDoc->itemSelection_AlignLeftOut(currAlignTo, currAlignMethod, guidePosition);
}

void AlignDistributePalette::alignLeftIn()
{
	if (currDoc != nullptr)
		currDoc->itemSelection_AlignLeftIn(currAlignTo, currAlignMethod, guidePosition);
}

void AlignDistributePalette::alignCenterHor()
{
	if (currDoc != nullptr)
		currDoc->itemSelection_AlignCenterHor(currAlignTo, currAlignMethod, guidePosition);
}

void AlignDistributePalette::alignRightIn()
{
	if (currDoc != nullptr)
		currDoc->itemSelection_AlignRightIn(currAlignTo, currAlignMethod, guidePosition);
}

void AlignDistributePalette::alignRightOut()
{
	if (currDoc != nullptr)
		currDoc->itemSelection_AlignRightOut(currAlignTo, currAlignMethod, guidePosition);
}

void AlignDistributePalette::alignTopOut()
{
	if (currDoc != nullptr)
		currDoc->itemSelection_AlignTopOut(currAlignTo, currAlignMethod, guidePosition);
}

void AlignDistributePalette::alignTopIn()
{
	if (currDoc != nullptr)
		currDoc->itemSelection_AlignTopIn(currAlignTo, currAlignMethod, guidePosition);
}


void AlignDistributePalette::alignCenterVer()
{
	if (currDoc != nullptr)
		currDoc->itemSelection_AlignCenterVer(currAlignTo, currAlignMethod, guidePosition);
}


void AlignDistributePalette::alignBottomIn()
{
	if (currDoc != nullptr)
		currDoc->itemSelection_AlignBottomIn(currAlignTo, currAlignMethod, guidePosition);
}

void AlignDistributePalette::alignBottomOut()
{
	if (currDoc != nullptr)
		currDoc->itemSelection_AlignBottomOut(currAlignTo, currAlignMethod, guidePosition);
}

void AlignDistributePalette::distributeLeft()
{
	if (currDoc != nullptr)
		currDoc->itemSelection_DistributeLeft();
}

void AlignDistributePalette::distributeCenterH()
{
	if (currDoc != nullptr)
		currDoc->itemSelection_DistributeCenterH();
}

void AlignDistributePalette::distributeRight()
{
	if (currDoc != nullptr)
		currDoc->itemSelection_DistributeRight();
}

void AlignDistributePalette::distributeDistH(bool usingDistance)
{
	if (currDoc != nullptr)
		currDoc->itemSelection_DistributeDistH(usingDistance, ad->distributeDistSpinBox->value(), ad->reverseDistributionCheckBox->isChecked());
}

void AlignDistributePalette::distributeDistValH()
{
	ad->distributeDistSpinBox->interpretText();
	if (currDoc != nullptr)
		distributeDistH(true);
}

void AlignDistributePalette::distributeDistAcrossPage()
{
	if (currDoc != nullptr)
		currDoc->itemSelection_DistributeAcrossPage();
}

void AlignDistributePalette::distributeDistDownPage()
{
	if (currDoc != nullptr)
		currDoc->itemSelection_DistributeDownPage();
}

void AlignDistributePalette::distributeDistAcrossMargins()
{
	if (currDoc != nullptr)
		currDoc->itemSelection_DistributeAcrossPage(true);
}

void AlignDistributePalette::distributeDistDownMargins()
{
	if (currDoc != nullptr)
		currDoc->itemSelection_DistributeDownPage(true);
}

void AlignDistributePalette::distributeBottom()
{
	if (currDoc != nullptr)
		currDoc->itemSelection_DistributeBottom();
}

void AlignDistributePalette::distributeCenterV()
{
	if (currDoc != nullptr)
		currDoc->itemSelection_DistributeCenterV();
}

void AlignDistributePalette::distributeTop()
{
	if (currDoc != nullptr)
		currDoc->itemSelection_DistributeTop();
}

void AlignDistributePalette::distributeDistV(bool usingDistance)
{
	if (currDoc != nullptr)
		currDoc->itemSelection_DistributeDistV(usingDistance, ad->distributeDistSpinBox->value(), ad->reverseDistributionCheckBox->isChecked());
}

void AlignDistributePalette::distributeDistValV()
{
	ad->distributeDistSpinBox->interpretText();
	if (currDoc != nullptr)
		distributeDistV(true);
}

void AlignDistributePalette::swapLeft()
{
	if (currDoc != nullptr)
		currDoc->itemSelection_SwapLeft();
}

void AlignDistributePalette::swapRight()
{
	if (currDoc != nullptr)
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
	guideDirection = direction;
	guidePosition = position;
	enableGuideButtons();
}


void AlignDistributePalette::localeChange()
{
	const QLocale& l(LocaleManager::instance().userPreferredLocale());
	ad->distributeDistSpinBox->setLocale(l);
}

void AlignDistributePalette::enableGuideButtons()
{
	QString suffix;
	double unitRatio = 1.0;
	int precision = 1;
	if (currDoc != nullptr)
	{
		suffix = unitGetSuffixFromIndex(currDoc->unitIndex());
		unitRatio = unitGetRatioFromIndex(currDoc->unitIndex());
		precision = unitGetPrecisionFromIndex(currDoc->unitIndex());
	}
	bool setterH = true;
	bool setterV = true;
	switch (guideDirection)
	{
		case -1:
			guideInfoText = guideInfoTextNone;
			if (currAlignTo == ScribusDoc::alignGuide)
				setterH = setterV = false;
			break;
		case 0:
			guideInfoText = tr("Y: %1%2").arg(guidePosition * unitRatio, 0, 'f', precision).arg(suffix);
			if (currAlignTo == ScribusDoc::alignGuide)
				setterV = false;
			break;
		case 1:
			guideInfoText = tr("X: %1%2").arg(guidePosition * unitRatio, 0, 'f', precision).arg(suffix);
			if (currAlignTo == ScribusDoc::alignGuide)
				setterH = false;
			break;
	}
	bool setterO = true;
	if (currAlignTo == ScribusDoc::alignGuide)
		setterO = false;

	ad->alignLeftInToolButton->setEnabled(setterV);
	ad->alignLeftOutToolButton->setEnabled(setterO);
	ad->alignRightInToolButton->setEnabled(setterV);
	ad->alignRightOutToolButton->setEnabled(setterO);
	ad->alignCenterHorToolButton->setEnabled(setterV);

	ad->alignTopInToolButton->setEnabled(setterH);
	ad->alignTopOutToolButton->setEnabled(setterO);
	ad->alignBottomInToolButton->setEnabled(setterH);
	ad->alignBottomOutToolButton->setEnabled(setterO);
	ad->alignCenterVerToolButton->setEnabled(setterH);

	ad->alignGuideLineEdit->setText(guideInfoText);
}

