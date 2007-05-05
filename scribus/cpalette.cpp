/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          cpalette.cpp  -  description
                             -------------------
    begin                : Wed Apr 25 2001
    copyright            : (C) 2001 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "cpalette.h"
//#include "cpalette.moc"

#include <qtooltip.h>
#include <q3listbox.h>
#include <qpixmap.h>
#include <qrect.h>
#include <q3popupmenu.h>
#include <qfont.h>
#include <qlayout.h>
#include <qtoolbutton.h>
#include <q3buttongroup.h>
#include <q3groupbox.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <q3iconview.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3GridLayout>
#include <Q3Frame>
#include <Q3VBoxLayout>
#include "colorlistbox.h"
#include "sccombobox.h"
#include "scribusdoc.h"
#include "scrspinbox.h"
#include "gradienteditor.h"
#include "units.h"
#include "page.h"
#include "pageitem.h"
#include "util.h"
#include "dynamictip.h"
#include "commonstrings.h"
#include "linkbutton.h"
#include "sccolorengine.h"


Cpalette::Cpalette(QWidget* parent) : QWidget(parent, "Cdouble")
{
	Color = "";
	Color3 = "";
	Shade = 100;
	Shade3 = 100;
	currentGradient = 0;
	currentItem = 0;
	Form1Layout = new Q3VBoxLayout( this, 0, 0, "Form1Layout");
	Layout1 = new Q3HBoxLayout;
	Layout1->setSpacing( 4 );
	Layout1->setMargin( 1 );
	Inhalt = new QToolButton(this, "t1");
	Inhalt->setPixmap(loadIcon("16/color-stroke.png"));
	Inhalt->setToggleButton(true);
// 	Inhalt->setAutoRaise(true);
	Inhalt->setBackgroundMode(Qt::PaletteBackground);
	Layout1->addWidget(Inhalt);
	Innen = new QToolButton(this, "t2");
	Innen->setPixmap(loadIcon("16/color-fill.png"));
	Innen->setToggleButton(true);
// 	Innen->setAutoRaise(true);
	Innen->setBackgroundMode(Qt::PaletteBackground);
	Innen->setOn(true);
	Layout1->addWidget(Innen);
	selectorQSpacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1->addItem( selectorQSpacer );
	Mode = 2;
	ShadeTxt = new QLabel( this, "ShadeTxt" );
	Layout1->addWidget( ShadeTxt);
	PM1 = new QSpinBox( this, "shspin" );
	PM1->setMinValue(0);
	PM1->setMaxValue(100);
	PM1->setLineStep(10);
	PM1->setValue(100);
	Layout1->addWidget(PM1);
	Form1Layout->addLayout(Layout1);
	GradLayout = new Q3VBoxLayout( 0, 0, 6, "GradLayout");
	QFont fo = QFont(font());
	fo.setPointSize(fo.pointSize()-1);
	gradientQCombo = new ScComboBox( false, this, "gradientQCombo" );
	gradientQCombo->setFont(fo);
	GradLayout->addWidget( gradientQCombo );
	gradEdit = new GradientEditor(this);
	gradEdit->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding));
	GradLayout->addWidget(gradEdit, Qt::AlignHCenter);
	freeGradientQFrame = new Q3Frame( this, "freeGradientQFrame" );
	freeGradientQFrame->setFrameShape( Q3Frame::NoFrame );
	freeGradientQFrame->setFrameShadow( Q3Frame::Plain );
	freeGradientLayout = new Q3GridLayout( freeGradientQFrame, 1, 1, 5, 5, "freeGradientLayout");
	GTextX1 = new QLabel("X1:", freeGradientQFrame, "GTextX1" );
	freeGradientLayout->addWidget( GTextX1, 0, 0 );
	GTextY1 = new QLabel("Y1:", freeGradientQFrame, "GTextY1" );
	freeGradientLayout->addWidget( GTextY1, 1, 0 );
	gX1 = new ScrSpinBox( -3000, 3000, freeGradientQFrame, 0);
	freeGradientLayout->addWidget( gX1, 0, 1 );
	gY1 = new ScrSpinBox( -3000, 3000, freeGradientQFrame, 0);
	freeGradientLayout->addWidget( gY1, 1, 1 );
	GTextX2 = new QLabel("X2:", freeGradientQFrame, "GTextX2" );
	freeGradientLayout->addWidget( GTextX2, 0, 2 );
	GTextY2 = new QLabel("Y2:", freeGradientQFrame, "GTextY2" );
	freeGradientLayout->addWidget( GTextY2, 1, 2 );
	gX2 = new ScrSpinBox( -3000, 3000, freeGradientQFrame, 0);
	freeGradientLayout->addWidget( gX2, 0, 3 );
	gY2 = new ScrSpinBox( -3000, 3000, freeGradientQFrame, 0);
	freeGradientLayout->addWidget( gY2, 1, 3 );
	gradEditButton = new QToolButton(freeGradientQFrame, "t1");
	gradEditButton->setToggleButton(true);
	freeGradientLayout->addMultiCellWidget(gradEditButton, 2, 2, 0, 3);
	GradLayout->addWidget( freeGradientQFrame );
	Form1Layout->addLayout(GradLayout);
	colorListQLBox = new ColorListBox(this, "colorListQLBox");
	colorListQLBox->setMinimumSize( QSize( 150, 30 ) );
	colorListQLBox->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	Form1Layout->addWidget(colorListQLBox);
// 	dynTip = new DynamicTip(colorListQLBox, &colorList);

	patternFrame = new Q3Frame( this, "frame3" );
	patternFrame->setFrameShape( Q3Frame::NoFrame );
	frame3Layout = new Q3VBoxLayout( patternFrame, 0, 2, "frame3Layout");
	patternBox = new Q3IconView(patternFrame, "patternBox");
	patternBox->setMinimumSize( QSize( 150, 30 ) );
	patternBox->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	patternBox->setResizeMode( Q3IconView::Adjust );
	patternBox->setItemsMovable( false );
	frame3Layout->addWidget( patternBox );

	groupOffset = new Q3GroupBox( patternFrame, "groupOffset" );
	groupOffset->setColumnLayout(0, Qt::Vertical );
	groupOffset->layout()->setSpacing( 2 );
	groupOffset->layout()->setMargin( 3 );
	groupOffsetLayout = new Q3HBoxLayout( groupOffset->layout() );
	groupOffsetLayout->setAlignment( Qt::AlignTop );
	textLabel1 = new QLabel( groupOffset, "textLabel1" );
	groupOffsetLayout->addWidget( textLabel1 );
	spinXoffset = new ScrSpinBox( -3000, 3000, groupOffset, 0);
	groupOffsetLayout->addWidget( spinXoffset );
	textLabel2 = new QLabel( groupOffset, "textLabel2" );
	groupOffsetLayout->addWidget( textLabel2 );
	spinYoffset = new ScrSpinBox( -3000, 3000, groupOffset, 0);
	groupOffsetLayout->addWidget( spinYoffset );
	frame3Layout->addWidget( groupOffset );

	groupScale = new Q3GroupBox( patternFrame, "groupScale" );
	groupScale->setColumnLayout(0, Qt::Vertical );
	groupScale->layout()->setSpacing( 2 );
	groupScale->layout()->setMargin( 3 );
	groupScaleLayout = new Q3GridLayout( groupScale->layout() );
	groupScaleLayout->setAlignment( Qt::AlignTop );
	textLabel5 = new QLabel( groupScale, "textLabel5" );
	groupScaleLayout->addWidget( textLabel5, 0, 0 );
	spinXscaling = new ScrSpinBox( 1, 500, groupScale, 0);
	spinXscaling->setValue( 100 );
	groupScaleLayout->addWidget( spinXscaling, 0, 1 );
	textLabel6 = new QLabel( groupScale, "textLabel6" );
	groupScaleLayout->addWidget( textLabel6, 1, 0 );
	spinYscaling = new ScrSpinBox( 1, 500, groupScale, 0 );
	groupScaleLayout->addWidget( spinYscaling, 1, 1 );
	keepScaleRatio = new LinkButton( groupScale );
	keepScaleRatio->setToggleButton( true );
	keepScaleRatio->setAutoRaise( true );
	keepScaleRatio->setMaximumSize( QSize( 15, 32767 ) );
	groupScaleLayout->addMultiCellWidget( keepScaleRatio, 0, 1, 2, 2 );
	frame3Layout->addWidget( groupScale );

	groupRotation = new Q3GroupBox( patternFrame, "groupRotation" );
	groupRotation->setColumnLayout(0, Qt::Vertical );
	groupRotation->layout()->setSpacing( 2 );
	groupRotation->layout()->setMargin( 3 );
	groupRotationLayout = new Q3HBoxLayout( groupRotation->layout() );
	groupRotationLayout->setAlignment( Qt::AlignTop );
	textLabel7 = new QLabel( groupRotation, "textLabel7" );
	groupRotationLayout->addWidget( textLabel7 );
	spinAngle = new ScrSpinBox( -100, 180, groupRotation, 0 );
	groupRotationLayout->addWidget( spinAngle );
	frame3Layout->addWidget( groupRotation );
	Form1Layout->addWidget(patternFrame);
	patternFrame->hide();

	TransGroup = new Q3GroupBox( tr( "Transparency Settings" ), this, "TransGroup" );
	TransGroup->setColumnLayout(0, Qt::Vertical );
	TransGroup->layout()->setSpacing( 0 );
	TransGroup->layout()->setMargin( 0 );
	Layout1t = new Q3GridLayout( TransGroup->layout() );
	Layout1t->setAlignment( Qt::AlignTop );
	Layout1t->setSpacing( 5 );
	Layout1t->setMargin( 5 );
	TransTxt = new QLabel( TransGroup, "Transtxt" );
	Layout1t->addWidget( TransTxt, 0, 0 );
	TransSpin = new QSpinBox( TransGroup, "traspin" );
	TransSpin->setMinValue(0);
	TransSpin->setMaxValue(100);
	TransSpin->setLineStep(10);
	TransSpin->setValue(100);
	Layout1t->addWidget(TransSpin, 0, 1);
	TransTxt2 = new QLabel( TransGroup, "textLabel1" );
	Layout1t->addWidget( TransTxt2, 1, 0 );
	blendMode = new ScComboBox( false, TransGroup, "blendMode" );
	Layout1t->addWidget( blendMode, 1, 1 );
	Form1Layout->addWidget(TransGroup);

	Inhalt->setOn(true);
	InnenButton();
	GradientMode = false;

	languageChange();
	setActGradient(0);

	connect(Inhalt, SIGNAL(clicked()), this, SLOT(InhaltButton()));
	connect(Innen, SIGNAL(clicked()), this, SLOT(InnenButton()));
	connect(colorListQLBox, SIGNAL(clicked(Q3ListBoxItem*)), this, SLOT(selectColor(Q3ListBoxItem*)));
	connect(colorListQLBox, SIGNAL(selected(Q3ListBoxItem*)), this, SLOT(selectColor(Q3ListBoxItem*)));
	connect(PM1, SIGNAL(valueChanged(int)), this, SLOT(setActShade()));
	connect(gradientQCombo, SIGNAL(activated(int)), this, SLOT(slotGrad(int)));
	connect(TransSpin, SIGNAL(valueChanged(int)), this, SLOT(slotTrans(int)));
	connect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
	connect(gX1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gX2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gY1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gY2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(spinXoffset, SIGNAL(valueChanged(double)), this, SLOT(changePatternProps()));
	connect(spinYoffset, SIGNAL(valueChanged(double)), this, SLOT(changePatternProps()));
	connect(spinXscaling, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	connect(spinYscaling, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
	connect(keepScaleRatio, SIGNAL(clicked()), this, SLOT(ToggleKette()));
	connect(spinAngle, SIGNAL(valueChanged(double)), this, SLOT(changePatternProps()));
	connect(gradEdit->Preview, SIGNAL(selectedColor(QString, int )), this, SLOT(slotColor(QString, int )));
	connect(gradEdit->Preview, SIGNAL(currTrans(double )), this, SLOT(setGradTrans(double )));
	connect(gradEdit, SIGNAL(gradientChanged()), this, SIGNAL(gradientChanged()));
	connect(gradEdit->Preview, SIGNAL(gradientChanged()), this, SIGNAL(gradientChanged()));
	connect(gradEditButton, SIGNAL(clicked()), this, SIGNAL(editGradient()));
}

void Cpalette::setCurrentItem(PageItem* item)
{
	currentItem = item;
}

void Cpalette::updateFromItem()
{
	if (currentItem == NULL)
		return;
	if (!currentDoc)
		return;
	Color = currentItem->lineColor();
	Shade = currentItem->lineShade();
	Color3 = currentItem->fillColor();
	Shade3 = currentItem->fillShade();
	setActTrans(currentItem->fillTransparency(), currentItem->lineTransparency());
	setActBlend(currentItem->fillBlendmode(), currentItem->lineBlendmode());
	if (Mode == 1)
	{
		updateCList();
		setActFarben(Color, Color3, Shade, Shade3);
	}
	else
	{
		ChooseGrad(currentItem->GrType);
		gradEdit->Preview->fill_gradient = currentItem->fill_gradient;
		gradientQCombo->setCurrentItem(currentItem->GrType);
		gradEdit->Preview->updateDisplay();
		double dur = currentDoc->unitRatio();
		setSpecialGradient(currentItem->GrStartX * dur, currentItem->GrStartY * dur, currentItem->GrEndX * dur, currentItem->GrEndY * dur);
	}
}

void Cpalette::InhaltButton()
{
	if (Inhalt->isOn())
	{
		Mode = 1;
		Innen->setOn(false);
		freeGradientQFrame->hide();
		freeGradientQFrame->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));
		gradientQCombo->hide();
		gradientQCombo->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));
		gradEdit->hide();
		gradEdit->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));
		patternFrame->hide();
		patternFrame->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));
		colorListQLBox->show();
		colorListQLBox->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
		GradientMode = false;
		layout()->activate();
//		updateCList();
//		repaint();
	}
	updateFromItem();
	emit modeChanged();
}

void Cpalette::InnenButton()
{
	if (Innen->isOn())
	{
		Mode = 2;
		Inhalt->setOn(false);
		gradientQCombo->show();
		gradientQCombo->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
		GradientMode = gradientQCombo->currentItem() != 0 ? true : false;
		if (GradientMode)
		{
			if (gradEdit->isHidden())
			{
				gradEdit->show();
				gradEdit->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
			}
			if (gradientQCombo->currentItem() > 5)
			{
				freeGradientQFrame->show();
				freeGradientQFrame->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
			}
			else
			{
				freeGradientQFrame->hide();
				freeGradientQFrame->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));
			}
		}
		layout()->activate();
//		updateCList();
//		repaint();
	}
	updateFromItem();
	emit modeChanged();
}
#include <QStandardItem>
#include <QAbstractItemView>
void Cpalette::updatePatternList()
{
	disconnect(patternBox, SIGNAL(clicked(Q3IconViewItem*)), this, SLOT(selectPattern(Q3IconViewItem*)));
	patternBox->clear();
	QMap<QString, ScPattern>::Iterator it;
	for (it = patternList->begin(); it != patternList->end(); ++it)
	{
		QPixmap pm;
		if (it.data().getPattern()->width() > it.data().getPattern()->height())
			pm.convertFromImage(it.data().getPattern()->scaleWidth(48));
		else
			pm.convertFromImage(it.data().getPattern()->scaleHeight(48));
		(void) new Q3IconViewItem(patternBox, it.key(), pm);
	}
	// Qt4 gradientQCombo->listBox()->item(8)->setSelectable(patternList->count() != 0);
	/* qt4 FIXME
	QStandardItem* si=dynamic_cast<QStandardItem*>(gradientQCombo->view()->children().at(8));
	if (si)
		si->setSelectable(patternList->count() != 0);
	*/
	patternBox->setSelected(patternBox->currentItem(), false);
	connect(patternBox, SIGNAL(clicked(Q3IconViewItem*)), this, SLOT(selectPattern(Q3IconViewItem*)));
}

void Cpalette::SetPatterns(QMap<QString, ScPattern> *docPatterns)
{
	patternList = docPatterns;
	updatePatternList();
}

void Cpalette::selectPattern(Q3IconViewItem *c)
{
	if (c == NULL)
		return;
	emit NewPattern(c->text());
}

void Cpalette::changePatternProps()
{
	emit NewPatternProps(spinXscaling->value(), spinYscaling->value(), spinXoffset->value(), spinYoffset->value(), spinAngle->value());
}

void Cpalette::ToggleKette()
{
	disconnect(spinXscaling, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	disconnect(spinYscaling, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
	if (keepScaleRatio->isOn())
	{
		spinYscaling->setValue(spinXscaling->value());
		changePatternProps();
		keepScaleRatio->setOn(true);
	}
	else
		keepScaleRatio->setOn(false);
	connect(spinXscaling, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	connect(spinYscaling, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
}

void Cpalette::HChange()
{
	disconnect(spinXscaling, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	disconnect(spinYscaling, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
	if (keepScaleRatio->isOn())
		spinYscaling->setValue(spinXscaling->value());
	changePatternProps();
	connect(spinXscaling, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	connect(spinYscaling, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
}

void Cpalette::VChange()
{
	disconnect(spinXscaling, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	disconnect(spinYscaling, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
	if (keepScaleRatio->isOn())
		spinXscaling->setValue(spinYscaling->value());
	changePatternProps();
	connect(spinXscaling, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	connect(spinYscaling, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
}

void Cpalette::SetColors(ColorList newColorList)
{
	colorList.clear();
	colorList = newColorList;
	updateCList();
}

void Cpalette::updateCList()
{
	disconnect(colorListQLBox, SIGNAL(clicked(Q3ListBoxItem*)), this, SLOT(selectColor(Q3ListBoxItem*)));
	disconnect(colorListQLBox, SIGNAL(selected(Q3ListBoxItem*)), this, SLOT(selectColor(Q3ListBoxItem*)));
	colorListQLBox->clear();
	if ((!GradientMode) || (Mode == 1))
		colorListQLBox->insertItem(CommonStrings::tr_NoneColor);
	colorListQLBox->insertItems(colorList, ColorListBox::fancyPixmap);
	colorListQLBox->setSelected(colorListQLBox->currentItem(), false);
	connect(colorListQLBox, SIGNAL(clicked(Q3ListBoxItem*)), this, SLOT(selectColor(Q3ListBoxItem*)));
	connect(colorListQLBox, SIGNAL(selected(Q3ListBoxItem*)), this, SLOT(selectColor(Q3ListBoxItem*)));
}

void Cpalette::selectColor(Q3ListBoxItem *c)
{
	if (c == NULL) { return; }
	sFarbe = c->text();
	switch (Mode)
	{
	case 1:
		emit NewPen(sFarbe);
		break;
	case 2:
		if (gradientQCombo->currentItem() == 0)
		{
			Color3 = sFarbe;
			emit NewBrush(sFarbe);
		}
		else if (gradientQCombo->currentItem() < 8)
		{
			gradEdit->Preview->setActColor(setColor(sFarbe, Shade), sFarbe, Shade);
			Color = sFarbe;
			emit gradientChanged();
		}
		break;
	}
}

QColor Cpalette::setColor(QString colorName, int shad)
{
	const ScColor& color = colorList[colorName];
	return ScColorEngine::getShadeColorProof(color, currentDoc, shad);
}

void Cpalette::updateBoxS(QString colorName)
{
	disconnect(colorListQLBox, SIGNAL(clicked(Q3ListBoxItem*)), this, SLOT(selectColor(Q3ListBoxItem*)));
	disconnect(colorListQLBox, SIGNAL(selected(Q3ListBoxItem*)), this, SLOT(selectColor(Q3ListBoxItem*)));
	int c = 0;
	if ((colorName != CommonStrings::None) && (!colorName.isEmpty()))
	{
		if (!GradientMode)
			++c;
		ColorList::Iterator itend=colorList.end();
		for (ColorList::Iterator it = colorList.begin(); it != itend; ++it)
		{
			if (it.key() == colorName)
				break;
			++c;
		}
	}
	colorListQLBox->setCurrentItem(c);
	connect(colorListQLBox, SIGNAL(clicked(Q3ListBoxItem*)), this, SLOT(selectColor(Q3ListBoxItem*)));
	connect(colorListQLBox, SIGNAL(selected(Q3ListBoxItem*)), this, SLOT(selectColor(Q3ListBoxItem*)));
}

void Cpalette::setActFarben(QString p, QString b, int shp, int shb)
{
	disconnect(PM1, SIGNAL(valueChanged(int)), this, SLOT(setActShade()));
	switch (Mode)
	{
	case 1:
		PM1->setValue(shp);
		updateBoxS(p);
		break;
	case 2:
		Color3 = b;
		Shade3 = shb;
		PM1->setValue(shb);
		updateBoxS(b);
		break;
	}
	connect(PM1, SIGNAL(valueChanged(int)), this, SLOT(setActShade()));
}

void Cpalette::slotColor(QString n, int s)
{
	if ((GradientMode) && (Mode ==2))
	{
		disconnect(PM1, SIGNAL(valueChanged(int)), this, SLOT(setActShade()));
		Color = n;
		Shade = s;
		PM1->setValue(Shade);
		updateBoxS(Color);
		connect(PM1, SIGNAL(valueChanged(int)), this, SLOT(setActShade()));
	}
}

void Cpalette::slotGrad(int number)
{
	if ((number == 8) && (patternList->count() == 0))
	{
		gradientQCombo->setCurrentItem(currentGradient);
		return;
	}
	int oldgrad = currentGradient;
	ChooseGrad(number);
	if ((currentGradient != 0) && (oldgrad != currentGradient))
	{
		gradEdit->Preview->fill_gradient = currentItem->fill_gradient;
		gradEdit->Preview->updateDisplay();
	}
	emit NewGradient(number);
}

void Cpalette::ChooseGrad(int number)
{
	if (number==-1)
	{
		gradientQCombo->setCurrentItem(0);
		currentGradient = 0;
	}
	//no need to disconnect as qcombobox only emits from user action
	/* PFJ - 29.02.04 - Removed GradGroup and Gradient mode from switch */
	GradientMode = number == 0 ? false : number == 8 ? false : true;

	if (number != 0)
	{
		if (number == 8)
		{
			PM1->setEnabled(false);
			freeGradientQFrame->hide();
			gradEdit->hide();
			freeGradientQFrame->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));
			gradEdit->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));
			colorListQLBox->hide();
			colorListQLBox->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));
			patternFrame->show();
			patternFrame->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
		}
		else
		{
			PM1->setEnabled(true);
			patternFrame->hide();
			patternFrame->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));
			colorListQLBox->show();
			colorListQLBox->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
			gradEdit->show();
			gradEdit->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
			if (gradientQCombo->currentItem() > 5)
			{
				freeGradientQFrame->show();
				freeGradientQFrame->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
			}
			else
			{
				freeGradientQFrame->hide();
				freeGradientQFrame->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));
			}
		}
	}
	else
	{
		PM1->setEnabled(true);
		patternFrame->hide();
		patternFrame->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));
		freeGradientQFrame->hide();
		gradEdit->hide();
		freeGradientQFrame->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));
		gradEdit->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));
		colorListQLBox->show();
		colorListQLBox->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	}
	layout()->activate();
	disconnect(PM1, SIGNAL(valueChanged(int)), this, SLOT(setActShade()));
	// JG probably not needed at all and should probably not be here
	updateCList();
	switch (number)
	{
	case 0:
		PM1->setValue(Shade3);
		updateBoxS(Color3);
		break;
	default:
		PM1->setValue(Shade);
		updateBoxS(Color);
		break;
	}
	setFocus();
	repaint();
	connect(PM1, SIGNAL(valueChanged(int)), this, SLOT(setActShade()));
}

void Cpalette::setActTrans(double val, double val2)
{
	disconnect(TransSpin, SIGNAL(valueChanged(int)), this, SLOT(slotTrans(int)));
	TransSpin->setValue(qRound(100 - (Mode == 1 ? val2 * 100 : val * 100)));
	connect(TransSpin, SIGNAL(valueChanged(int)), this, SLOT(slotTrans(int)));
}

void Cpalette::setActBlend(int val, int val2)
{
	disconnect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
	blendMode->setCurrentItem(Mode == 1 ? val2 : val);
	connect(blendMode, SIGNAL(activated(int)), this, SLOT(changeBlendMode(int)));
}

void Cpalette::setGradTrans(double val)
{
	if ((GradientMode) && (Mode ==2))
	{
		disconnect(TransSpin, SIGNAL(valueChanged(int)), this, SLOT(slotTrans(int)));
		TransSpin->setValue(qRound(val * 100));
		connect(TransSpin, SIGNAL(valueChanged(int)), this, SLOT(slotTrans(int)));
	}
}

void Cpalette::changeBlendMode(int blend)
{
	if (Mode == 1)
		emit NewBlendS(blend);
	else
	{
		if ((gradientQCombo->currentItem() == 0) || (gradientQCombo->currentItem() == 8))
			emit NewBlend(blend);
	}
}

void Cpalette::slotTrans(int val)
{
	if (Mode == 1)
		emit NewTransS(static_cast<double>(100 - val) / 100.0);
	else
	{
		if ((gradientQCombo->currentItem() == 0) || (gradientQCombo->currentItem() == 8))
			emit NewTrans(static_cast<double>(100 - val) / 100.0);
		else
		{
			gradEdit->Preview->setActTrans(static_cast<double>(val) / 100.0);
			emit gradientChanged();
		}
	}
	setFocus();
}

void Cpalette::setActGradient(int typ)
{
	disconnect(gradientQCombo, SIGNAL(activated(int)), this, SLOT(slotGrad(int)));
	if (Mode == 2)
	{
		currentGradient = typ;
		gradientQCombo->setCurrentItem(typ);
		ChooseGrad(typ);
	}
	connect(gradientQCombo, SIGNAL(activated(int)), this, SLOT(slotGrad(int)));
}

void Cpalette::setSpecialGradient(double x1, double y1, double x2, double y2)
{
	disconnect(gX1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	disconnect(gX2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	disconnect(gY1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	disconnect(gY2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	gX1->setValue(x1);
	gX2->setValue(x2);
	gY1->setValue(y1);
	gY2->setValue(y2);
	connect(gX1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gX2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gY1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gY2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
}

void Cpalette::changeSpecial()
{
	emit NewSpecial(gX1->value(), gY1->value(), gX2->value(), gY2->value());
}

void Cpalette::setActShade()
{
	int b = PM1->value();
	switch (Mode)
	{
	case 1:
		emit NewPenShade(b);
		break;
	case 2:
		if (gradientQCombo->currentItem() == 0)
		{
			Shade3 = b;
			emit NewBrushShade(b);
		}
		else
		{
			gradEdit->Preview->setActColor(setColor(Color, b), Color, b);
			Shade = b;
			emit gradientChanged();
		}
		break;
	}
}


void Cpalette::setActPattern(QString pattern, double scaleX, double scaleY, double offsetX, double offsetY, double rotation)
{
	disconnect(patternBox, SIGNAL(clicked(Q3IconViewItem*)), this, SLOT(selectPattern(Q3IconViewItem*)));
	disconnect(spinXoffset, SIGNAL(valueChanged(double)), this, SLOT(changePatternProps()));
	disconnect(spinYoffset, SIGNAL(valueChanged(double)), this, SLOT(changePatternProps()));
	disconnect(spinXscaling, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	disconnect(spinYscaling, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
	disconnect(spinAngle, SIGNAL(valueChanged(double)), this, SLOT(changePatternProps()));
	Q3IconViewItem *it = patternBox->findItem(pattern);
	if (it)
		patternBox->setCurrentItem(it);
	spinXoffset->setValue(offsetX);
	spinYoffset->setValue(offsetY);
	spinXscaling->setValue(scaleX);
	spinYscaling->setValue(scaleY);
	spinAngle->setValue(rotation);
	if (scaleX == scaleY)
		keepScaleRatio->setOn(true);
	else
		keepScaleRatio->setOn(false);
	connect(patternBox, SIGNAL(clicked(Q3IconViewItem*)), this, SLOT(selectPattern(Q3IconViewItem*)));
	connect(spinXoffset, SIGNAL(valueChanged(double)), this, SLOT(changePatternProps()));
	connect(spinYoffset, SIGNAL(valueChanged(double)), this, SLOT(changePatternProps()));
	connect(spinXscaling, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	connect(spinYscaling, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
	connect(spinAngle, SIGNAL(valueChanged(double)), this, SLOT(changePatternProps()));
}

void Cpalette::unitChange(double oldUnitRatio, double newUnitRatio, int unitIndex)
{
	disconnect(gX1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	disconnect(gX2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	disconnect(gY1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	disconnect(gY2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	disconnect(spinXoffset, SIGNAL(valueChanged(double)), this, SLOT(changePatternProps()));
	disconnect(spinYoffset, SIGNAL(valueChanged(double)), this, SLOT(changePatternProps()));
	gX1->setNewUnit(unitIndex);
	gY1->setNewUnit(unitIndex);
	gX2->setNewUnit(unitIndex);
	gY2->setNewUnit(unitIndex);
	spinXoffset->setNewUnit(unitIndex);
	spinYoffset->setNewUnit(unitIndex);
	connect(gX1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gX2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gY1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gY2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(spinXoffset, SIGNAL(valueChanged(double)), this, SLOT(changePatternProps()));
	connect(spinYoffset, SIGNAL(valueChanged(double)), this, SLOT(changePatternProps()));
}

void Cpalette::languageChange()
{
	QString ptSuffix=tr(" pt");
	gX1->setSuffix(ptSuffix);
	gY1->setSuffix(ptSuffix);
	gX2->setSuffix(ptSuffix);
	gY2->setSuffix(ptSuffix);
	QString pctSuffix=tr(" %");
	PM1->setSuffix(pctSuffix);
	TransSpin->setSuffix(pctSuffix);
	groupOffset->setTitle( tr( "Offsets" ) );
	textLabel1->setText( tr( "X:" ) );
	spinXoffset->setSuffix( ptSuffix );
	textLabel2->setText( tr( "Y:" ) );
	spinYoffset->setSuffix( ptSuffix );
	groupScale->setTitle( tr( "Scaling" ) );
	textLabel5->setText( tr( "X-Scale:" ) );
	spinXscaling->setSuffix( pctSuffix );
	textLabel6->setText( tr( "Y-Scale:" ) );
	spinYscaling->setSuffix( pctSuffix );
	groupRotation->setTitle( tr( "Rotation" ) );
	textLabel7->setText( tr( "Angle" ) );

	ShadeTxt->setText( tr( "Shade:" ) );
	TransTxt->setText( tr( "Opacity:" ) );
	GTextX1->setText( tr( "X1:"));
	GTextY1->setText( tr( "Y1:" ));
	GTextX2->setText( tr( "X2:" ));
	GTextY2->setText( tr( "Y2:" ));
	gradEditButton->setText( tr("Move Vector"));

	int oldGradient=gradientQCombo->currentItem();
	gradientQCombo->clear();
	gradientQCombo->insertItem( tr("Normal"));
	gradientQCombo->insertItem( tr("Horizontal Gradient"));
	gradientQCombo->insertItem( tr("Vertical Gradient"));
	gradientQCombo->insertItem( tr("Diagonal Gradient"));
	gradientQCombo->insertItem( tr("Cross Diagonal Gradient"));
	gradientQCombo->insertItem( tr("Radial Gradient"));
	gradientQCombo->insertItem( tr("Free linear Gradient"));
	gradientQCombo->insertItem( tr("Free radial Gradient"));
	gradientQCombo->insertItem( tr("Pattern"));
	gradientQCombo->setCurrentItem(oldGradient);
	TransGroup->setTitle( tr( "Transparency Settings" ));
	TransTxt2->setText( tr( "Blend Mode:" ) );
	blendMode->clear();
	blendMode->insertItem( tr("Normal"));
	blendMode->insertItem( tr("Darken"));
	blendMode->insertItem( tr("Lighten"));
	blendMode->insertItem( tr("Multiply"));
	blendMode->insertItem( tr("Screen"));
	blendMode->insertItem( tr("Overlay"));
	blendMode->insertItem( tr("Hard Light"));
	blendMode->insertItem( tr("Soft Light"));
	blendMode->insertItem( tr("Difference"));
	blendMode->insertItem( tr("Exclusion"));
	blendMode->insertItem( tr("Color Dodge"));
	blendMode->insertItem( tr("Color Burn"));
	blendMode->insertItem( tr("Hue"));
	blendMode->insertItem( tr("Saturation"));
	blendMode->insertItem( tr("Color"));
	blendMode->insertItem( tr("Luminosity"));

	QToolTip::add( Inhalt, tr( "Edit Line Color Properties" ) );
	QToolTip::add( Innen, tr( "Edit Fill Color Properties" ) );
	QToolTip::add( PM1, tr( "Saturation of color" ) );
	QToolTip::add( gradientQCombo, tr( "Normal or gradient fill method" ) );
	QToolTip::add( TransSpin, tr( "Set the transparency for the color selected" ) );
	QToolTip::add( gradEditButton, "<qt>" + tr( "Move the start of the gradient vector with the left mouse button pressed and move the end of the gradient vector with the right mouse button pressed" ) + "</qt>");
}
