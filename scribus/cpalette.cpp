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
#include "cpalette.moc"

#include <qtooltip.h>
#include <qlistbox.h>
#include <qpixmap.h>
#include <qrect.h>
#include <qpopupmenu.h>
#include <qfont.h>
#include <qlayout.h>
#include <qtoolbutton.h>
#include <qbuttongroup.h>
#include <qlabel.h>
#include <qspinbox.h>
#include "sccombobox.h"
#include "scribusdoc.h"
#include "mspinbox.h"
#include "gradienteditor.h"
#include "units.h"
#include "page.h"
#include "util.h"
#include "dynamictip.h"

Cpalette::Cpalette(QWidget* parent) : QWidget(parent, "Cdouble")
{
	Color = "";
	Color3 = "";
	Shade = 100;
	Shade3 = 100;
	UseTransFeature = false;
	Form1Layout = new QVBoxLayout( this, 0, 0, "Form1Layout");
	Layout1 = new QGridLayout;
	Layout1->setSpacing( 4 );
	Layout1->setMargin( 1 );
	Inhalt = new QToolButton(this, "t1");
	Inhalt->setPixmap(loadIcon("Stiftalt.xpm"));
	Inhalt->setToggleButton(true);
	Inhalt->setAutoRaise(true);
	Inhalt->setBackgroundMode(PaletteBackground);
	Layout1->addWidget(Inhalt, 0, 0);
	Innen = new QToolButton(this, "t2");
	Innen->setPixmap(loadIcon("fill.png"));
	Innen->setToggleButton(true);
	Innen->setAutoRaise(true);
	Innen->setBackgroundMode(PaletteBackground);
	Innen->setOn(true);
	Layout1->addWidget(Innen, 0, 1);
	Mode = 2;
	ShadeTxt = new QLabel( this, "ShadeTxt" );
	Layout1->addWidget( ShadeTxt, 0, 2 );
	PM1 = new QSpinBox( this, "shspin" );
	PM1->setMinValue(0);
	PM1->setMaxValue(100);
	PM1->setLineStep(10);
	PM1->setValue(100);
	Layout1->addWidget(PM1, 0, 3);
	TransTxt = new QLabel( this, "Transtxt" );
	Layout1->addWidget( TransTxt, 1, 2 );
	TransSpin = new QSpinBox( this, "traspin" );
	TransSpin->setMinValue(0);
	TransSpin->setMaxValue(100);
	TransSpin->setLineStep(10);
	TransSpin->setValue(100);
	Layout1->addWidget(TransSpin, 1, 3);
	Form1Layout->addLayout(Layout1);
	GradLayout = new QVBoxLayout( 0, 0, 6, "GradLayout");
	QFont fo = QFont(font());
	fo.setPointSize(fo.pointSize()-1);
	gradientQCombo = new ScComboBox( false, this, "gradientQCombo" );
	gradientQCombo->setFont(fo);
	GradLayout->addWidget( gradientQCombo );
	gradEdit = new GradientEditor(this);
	gradEdit->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding));
	GradLayout->addWidget(gradEdit, Qt::AlignHCenter);
	freeGradientQFrame = new QFrame( this, "freeGradientQFrame" );
	freeGradientQFrame->setFrameShape( QFrame::NoFrame );
	freeGradientQFrame->setFrameShadow( QFrame::Plain );
	freeGradientLayout = new QGridLayout( freeGradientQFrame, 1, 1, 5, 5, "freeGradientLayout");
	GTextX1 = new QLabel("X1:", freeGradientQFrame, "GTextX1" );
	freeGradientLayout->addWidget( GTextX1, 0, 0 );
	GTextY1 = new QLabel("Y1:", freeGradientQFrame, "GTextY1" );
	freeGradientLayout->addWidget( GTextY1, 1, 0 );
	gX1 = new MSpinBox( freeGradientQFrame, 2);
	gX1->setDecimals(100);
	gX1->setMinValue(0);
	gX1->setMaxValue(3000);
	freeGradientLayout->addWidget( gX1, 0, 1 );
	gY1 = new MSpinBox( freeGradientQFrame, 2 );
	gY1->setDecimals(100);
	gY1->setMinValue(0);
	gY1->setMaxValue(3000);
	freeGradientLayout->addWidget( gY1, 1, 1 );
	GTextX2 = new QLabel("X2:", freeGradientQFrame, "GTextX2" );
	freeGradientLayout->addWidget( GTextX2, 0, 2 );
	GTextY2 = new QLabel("Y2:", freeGradientQFrame, "GTextY2" );
	freeGradientLayout->addWidget( GTextY2, 1, 2 );
	gX2 = new MSpinBox( freeGradientQFrame, 2 );
	gX2->setDecimals(100);
	gX2->setMinValue(0);
	gX2->setMaxValue(3000);
	freeGradientLayout->addWidget( gX2, 0, 3 );
	gY2 = new MSpinBox( freeGradientQFrame, 2 );
	gY2->setDecimals(100);
	gY2->setMinValue(0);
	gY2->setMaxValue(3000);
	freeGradientLayout->addWidget( gY2, 1, 3 );
	gradEditButton = new QToolButton(freeGradientQFrame, "t1");
	gradEditButton->setToggleButton(true);
	freeGradientLayout->addMultiCellWidget(gradEditButton, 2, 2, 0, 3);
	GradLayout->addWidget( freeGradientQFrame );
	Form1Layout->addLayout(GradLayout);
	colorListQLBox = new QListBox(this, "colorListQLBox");
	colorListQLBox->setMinimumSize( QSize( 150, 30 ) );
	colorListQLBox->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	Form1Layout->addWidget(colorListQLBox);
	dynTip = new DynamicTip(colorListQLBox, &colorList);
	Inhalt->setOn(true);
	InnenButton();
	GradientMode = false;
	
	languageChange();
	setActGradient(0);
	
	connect(Inhalt, SIGNAL(clicked()), this, SLOT(InhaltButton()));
	connect(Innen, SIGNAL(clicked()), this, SLOT(InnenButton()));
	connect(colorListQLBox, SIGNAL(clicked(QListBoxItem*)), this, SLOT(selectColor(QListBoxItem*)));
	connect(colorListQLBox, SIGNAL(selected(QListBoxItem*)), this, SLOT(selectColor(QListBoxItem*)));
	connect(PM1, SIGNAL(valueChanged(int)), this, SLOT(setActShade()));
	connect(gradientQCombo, SIGNAL(activated(int)), this, SLOT(slotGrad(int)));
	connect(TransSpin, SIGNAL(valueChanged(int)), this, SLOT(slotTrans(int)));
	connect(gX1, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	connect(gX2, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	connect(gY1, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	connect(gY2, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	connect(gradEdit->Preview, SIGNAL(selectedColor(QString, int )), this, SLOT(slotColor(QString, int )));
	connect(gradEdit->Preview, SIGNAL(currTrans(double )), this, SLOT(setGradTrans(double )));
	connect(gradEdit, SIGNAL(gradientChanged()), this, SIGNAL(gradientChanged()));
	connect(gradEdit->Preview, SIGNAL(gradientChanged()), this, SIGNAL(gradientChanged()));
	connect(gradEditButton, SIGNAL(clicked()), this, SIGNAL(editGradient()));
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
		GradientMode = false;
		layout()->activate();
		updateCList();
		repaint();
	}
	emit QueryItem();
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
		updateCList();
		repaint();
	}
	emit QueryItem();
}

void Cpalette::SetColors(ColorList newColorList)
{
	colorList.clear();
	colorList = newColorList;
	updateCList();
}

void Cpalette::updateCList()
{
	disconnect(colorListQLBox, SIGNAL(clicked(QListBoxItem*)), this, SLOT(selectColor(QListBoxItem*)));
	disconnect(colorListQLBox, SIGNAL(selected(QListBoxItem*)), this, SLOT(selectColor(QListBoxItem*)));
	colorListQLBox->clear();
	if ((!GradientMode) || (Mode == 1))
		colorListQLBox->insertItem( tr("None"));
	ColorList::Iterator itend=colorList.end();
	for (ColorList::Iterator it = colorList.begin(); it != itend; ++it)
	{
		ScColor col = colorList[it.key()];
		QPixmap * pm = getFancyPixmap(col);
		colorListQLBox->insertItem(*pm, it.key());
	}
	colorListQLBox->setSelected(colorListQLBox->currentItem(), false);
	connect(colorListQLBox, SIGNAL(clicked(QListBoxItem*)), this, SLOT(selectColor(QListBoxItem*)));
	connect(colorListQLBox, SIGNAL(selected(QListBoxItem*)), this, SLOT(selectColor(QListBoxItem*)));
}

void Cpalette::selectColor(QListBoxItem *c)
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
		else
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
	return colorList[colorName].getShadeColorProof(shad);
}

void Cpalette::updateBoxS(QString colorName)
{
	disconnect(colorListQLBox, SIGNAL(clicked(QListBoxItem*)), this, SLOT(selectColor(QListBoxItem*)));
	disconnect(colorListQLBox, SIGNAL(selected(QListBoxItem*)), this, SLOT(selectColor(QListBoxItem*)));
	int c = 0;
	if ((colorName != "None") && (!colorName.isEmpty()))
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
	connect(colorListQLBox, SIGNAL(clicked(QListBoxItem*)), this, SLOT(selectColor(QListBoxItem*)));
	connect(colorListQLBox, SIGNAL(selected(QListBoxItem*)), this, SLOT(selectColor(QListBoxItem*)));
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
	ChooseGrad(number);
	emit NewGradient(number);
}

void Cpalette::ChooseGrad(int number)
{
	if (number==-1)
		gradientQCombo->setCurrentItem(0); //no need to disconnect as qcombobox only emits from user action
	/* PFJ - 29.02.04 - Removed GradGroup and Gradient mode from switch */
	GradientMode = number == 0 ? false : true;

	if (number != 0)
	{
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
	else
	{
		freeGradientQFrame->hide();
		gradEdit->hide();
		freeGradientQFrame->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));
		gradEdit->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));
	}
	layout()->activate();
	disconnect(PM1, SIGNAL(valueChanged(int)), this, SLOT(setActShade()));
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

void Cpalette::setGradTrans(double val)
{
	if ((GradientMode) && (Mode ==2))
	{
		disconnect(TransSpin, SIGNAL(valueChanged(int)), this, SLOT(slotTrans(int)));
		TransSpin->setValue(qRound(val * 100));
		connect(TransSpin, SIGNAL(valueChanged(int)), this, SLOT(slotTrans(int)));
	}
}

void Cpalette::slotTrans(int val)
{
	if (Mode == 1)
		emit NewTransS(static_cast<double>(100 - val) / 100.0);
	else
	{
		if (gradientQCombo->currentItem() == 0)
			emit NewTrans(static_cast<double>(100 - val) / 100.0);
		else
		{
			gradEdit->Preview->setActTrans(static_cast<double>(val) / 100.0);
			emit gradientChanged();
		}
	}
	setFocus();
}

void Cpalette::UseTrans(bool useTrans)
{
	if (useTrans)
	{
		TransTxt->show();
		TransSpin->show();
		TransTxt->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
		TransSpin->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	}
	else
	{
		if (!TransTxt->isHidden())
		{
			TransTxt->hide();
			TransSpin->hide();
			TransTxt->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));
			TransSpin->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));
		}
	}
	layout()->activate();
	repaint();
	UseTransFeature = useTrans;
}

void Cpalette::setActGradient(int typ)
{
	disconnect(gradientQCombo, SIGNAL(activated(int)), this, SLOT(slotGrad(int)));
	if (Mode == 2)
	{
		gradientQCombo->setCurrentItem(typ);
		ChooseGrad(typ);
	}
	connect(gradientQCombo, SIGNAL(activated(int)), this, SLOT(slotGrad(int)));
}

void Cpalette::setSpecialGradient(double x1, double y1, double x2, double y2, double w, double h)
{
	disconnect(gX1, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	disconnect(gX2, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	disconnect(gY1, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	disconnect(gY2, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	gX1->setMaxValue(w);
	gX2->setMaxValue(w);
	gY1->setMaxValue(h);
	gY2->setMaxValue(h);
	gX1->setMinValue(0);
	gX2->setMinValue(0);
	gY1->setMinValue(0);
	gY2->setMinValue(0);
	gX1->setValue(x1);
	gX2->setValue(x2);
	gY1->setValue(y1);
	gY2->setValue(y2);
	connect(gX1, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	connect(gX2, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	connect(gY1, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	connect(gY2, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
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

void Cpalette::UnitChange(double oldUnitRatio, double newUnitRatio, int unitIndex)
{
	disconnect(gX1, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	disconnect(gX2, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	disconnect(gY1, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	disconnect(gY2, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));

	double oldX = gX1->value() / oldUnitRatio;
	double oldXM = gX1->maxValue() / oldUnitRatio;
	double oldY = gY1->value() / oldUnitRatio;
	double oldYM = gY1->maxValue() / oldUnitRatio;
	double oldW = gX2->value() / oldUnitRatio;
	double oldWM = gX2->maxValue() / oldUnitRatio;
	double oldH = gY2->value() / oldUnitRatio;
	double oldHM = gY2->maxValue() / oldUnitRatio;

	QString unitSuffix = unitGetSuffixFromIndex(unitIndex);
	int unitDecimals = unitGetDecimalsFromIndex(unitIndex);
	gX1->setDecimals( unitDecimals );
	gY1->setDecimals( unitDecimals );
	gX2->setDecimals( unitDecimals );
	gY2->setDecimals( unitDecimals );
	gX1->setSuffix( unitSuffix );
	gY1->setSuffix( unitSuffix );
	gX2->setSuffix( unitSuffix );
	gY2->setSuffix( unitSuffix );

	gX1->setMaxValue(oldXM * newUnitRatio);
	gX1->setValue(oldX * newUnitRatio);
	gY1->setMaxValue(oldYM * newUnitRatio);
	gY1->setValue(oldY * newUnitRatio);
	gX2->setMaxValue(oldWM * newUnitRatio);
	gX2->setValue(oldW * newUnitRatio);
	gY2->setMaxValue(oldHM * newUnitRatio);
	gY2->setValue(oldH * newUnitRatio);
	gX1->setMinValue(0);
	gX2->setMinValue(0);
	gY1->setMinValue(0);
	gY2->setMinValue(0);

	connect(gX1, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	connect(gX2, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	connect(gY1, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	connect(gY2, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
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
	gradientQCombo->setCurrentItem(oldGradient);

	QToolTip::add( Inhalt, tr( "Edit Line Color Properties" ) );
	QToolTip::add( Innen, tr( "Edit Fill Color Properties" ) );
	QToolTip::add( PM1, tr( "Saturation of color" ) );
	QToolTip::add( gradientQCombo, tr( "Normal or gradient fill method" ) );
	QToolTip::add( TransSpin, tr( "Set the transparency for the color selected" ) );
	QToolTip::add( gradEditButton, "<qt>" + tr( "Move the start of the gradient vector with the left mouse button pressed and move the end of the gradient vector with the right mouse button pressed" ) + "</qt>");
}
