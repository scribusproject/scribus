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
extern QPixmap loadIcon(QString nam);

Cpalette::Cpalette(QWidget* parent) : QWidget(parent, "Cdouble")
{
	Color = "";
	Color2 = "";
	Color3 = "";
	Shade = 100;
	Shade2 = 100;
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
	ShadeTxt->setText( tr( "Shade:" ) );
	Layout1->addWidget( ShadeTxt, 0, 2 );
	PM1 = new QSpinBox( this, "shspin" );
	PM1->setMinValue(0);
	PM1->setMaxValue(100);
	PM1->setLineStep(10);
	PM1->setSuffix( tr(" %"));
	PM1->setValue(100);
	Layout1->addWidget(PM1, 0, 3);
	TransGroup = new QButtonGroup( this, "GradGroup" );
	TransGroup->setFrameShape( QButtonGroup::NoFrame );
	TransGroup->setFrameShadow( QButtonGroup::Plain );
	TransGroup->setTitle("");
	TransGroup->setColumnLayout(0, Qt::Vertical );
	TransGroup->layout()->setSpacing( 4 );
	TransGroup->layout()->setMargin( 0 );
	TransGroupLayout = new QHBoxLayout( TransGroup->layout() );
	TransGroupLayout->setAlignment( Qt::AlignTop );
	TransTxt = new QLabel( TransGroup, "Transtxt" );
	TransTxt->setText( tr( "Opacity:" ) );
	TransGroupLayout->addWidget( TransTxt );
	TransSpin = new QSpinBox( TransGroup, "traspin" );
	TransSpin->setMinValue(0);
	TransSpin->setMaxValue(100);
	TransSpin->setLineStep(10);
	TransSpin->setSuffix( tr(" %"));
	TransSpin->setValue(100);
	TransGroupLayout->addWidget( TransSpin );
	Layout1->addMultiCellWidget( TransGroup, 1, 1, 2, 3 );
	Form1Layout->addLayout(Layout1);

	GradLayout = new QVBoxLayout( 0, 0, 6, "GradLayout");

	QFont fo = QFont(font());
	fo.setPointSize(fo.pointSize()-1);
	GradCombo = new QComboBox( true, this, "GradCombo" );
	GradCombo->setEditable(false);
	GradCombo->setFont(fo);
	GradCombo->insertItem( tr("Normal"));
	GradCombo->insertItem( tr("Horizontal Gradient"));
	GradCombo->insertItem( tr("Vertical Gradient"));
	GradCombo->insertItem( tr("Diagonal Gradient"));
	GradCombo->insertItem( tr("Cross Diagonal Gradient"));
	GradCombo->insertItem( tr("Radial Gradient"));
	GradCombo->setCurrentItem(0);
	GradLayout->addWidget( GradCombo );
	GradGroup = new QButtonGroup( this, "GradGroup" );
	GradGroup->setFrameShape( QButtonGroup::NoFrame );
	GradGroup->setFrameShadow( QButtonGroup::Plain );
	GradGroup->setTitle("");
	GradGroup->setColumnLayout(0, Qt::Vertical );
	GradGroup->layout()->setSpacing( 5 );
	GradGroup->layout()->setMargin( 0 );
	GradGroupLayout = new QHBoxLayout( GradGroup->layout() );
	GradGroupLayout->setAlignment( Qt::AlignTop );
	GrColor1 = new QRadioButton( GradGroup, "GrColor1" );
	GrColor1->setText("#1");
	GrColor1->setChecked( true );
	GradGroupLayout->addWidget( GrColor1 );
	GrColor2 = new QRadioButton( GradGroup, "GrColor2" );
	GrColor2->setText("#2");
	GradGroupLayout->addWidget( GrColor2 );
	GradLayout->addWidget( GradGroup );
	Form1Layout->addLayout(GradLayout);

	ListBox1 = new QListBox(this, "ListBox1");
	ListBox1->setMinimumSize( QSize( 150, 210 ) );
	Form1Layout->addWidget(ListBox1);
	setActGradient("", "", 100, 100, 0);
	GradientMode = false;
	QToolTip::add( Inhalt, tr( "Edit Line Color Properties" ) );
	QToolTip::add( Innen, tr( "Edit Fill Color Properties" ) );
	QToolTip::add( PM1, tr( "Saturation of color" ) );
	QToolTip::add( GradCombo, tr( "Normal or gradient fill method" ) );
	QToolTip::add( GrColor1, tr( "Edit the first color of object" ) );
	QToolTip::add( GrColor2, tr( "Edit the second color of object" ) );
	QToolTip::add( TransSpin, tr( "Set the transparency for the color selected" ) );
	QToolTip::add( ListBox1, tr( "Color of selected object" ) );
	connect(Inhalt, SIGNAL(clicked()), this, SLOT(InhaltButton()));
	connect(Innen, SIGNAL(clicked()), this, SLOT(InnenButton()));
	connect(ListBox1, SIGNAL(clicked(QListBoxItem*)), this, SLOT(selFarbe(QListBoxItem*)));
	connect(PM1, SIGNAL(valueChanged(int)), this, SLOT(setActShade()));
	connect(GradCombo, SIGNAL(activated(int)), this, SLOT(slotGrad(int)));
	connect(GrColor1, SIGNAL(clicked()), this, SLOT(slotColor()));
	connect(GrColor2, SIGNAL(clicked()), this, SLOT(slotColor()));
	connect(TransSpin, SIGNAL(valueChanged(int)), this, SLOT(slotTrans(int)));
}

void Cpalette::InhaltButton()
{
	int h = 0;
	if (Inhalt->isOn())
	{
		Mode = 1;
		Innen->setOn(false);
		h += GradCombo->height();
		h += GradGroup->height();
		h += TransGroup->height();
		GradCombo->hide();
		GradGroup->hide();
		GradientMode = false;
		ListBox1->resize(ListBox1->width(), ListBox1->height()+h);
		updateCList();
		updateGeometry();
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
		GradCombo->show();
		GradGroup->show();
		GradientMode = GradCombo->currentItem() != 0 ? true : false;
		updateCList();
		updateGeometry();
		repaint();
	}
	emit QueryItem();
}

void Cpalette::SetColors(CListe farben)
{
	Farbliste.clear();
	Farbliste = farben;
	updateCList();
}

void Cpalette::updateCList()
{
	ListBox1->clear();
	CListe::Iterator it;
	QPixmap pm = QPixmap(30, 15);
	if ((!GradientMode) || (Mode == 1))
		ListBox1->insertItem( tr("None"));
	for (it = Farbliste.begin(); it != Farbliste.end(); ++it)
	{
		pm.fill(Farbliste[it.key()].getRGBColor());
		ListBox1->insertItem(pm, it.key());
	}
	ListBox1->setSelected(ListBox1->currentItem(), false);
}

void Cpalette::selFarbe(QListBoxItem *c)
{
	if (c == NULL) { return; }
	sFarbe = c->text();
	switch (Mode)
	{
	case 1:
		emit NewPen(sFarbe);
		break;
	case 2:
		if (GradCombo->currentItem() == 0)
		{
			Color3 = sFarbe;
			emit NewBrush(sFarbe);
		}
		else
		{
			if (GrColor1->isChecked())
				Color = sFarbe;
			else
				Color2 = sFarbe;
			emit NewGradient(GradCombo->currentItem(), Color, Shade, Color2, Shade2);
		}
		break;
	}
}

void Cpalette::updateBoxS(QString Farbe)
{
	CListe::Iterator it;
	int c = 0;
	if ((Farbe != "None") && (Farbe != ""))
	{
		if (!GradientMode)
			c++;
		for (it = Farbliste.begin(); it != Farbliste.end(); ++it)
		{
			if (it.key() == Farbe)
				break;
			c++;
		}
	}
	ListBox1->setCurrentItem(c);
}

void Cpalette::setActFarben(QString p, QString b, int shp, int shb)
{
	switch (Mode)
	{
	case 1:
		PM1->setValue(shp);
		updateBoxS(p);
		break;
	case 2:
		PM1->setValue(shb);
		updateBoxS(b);
		Color3 = b;
		Shade3 = shb;
		break;
	}
}

void Cpalette::slotColor()
{
	if (GrColor1->isChecked())
	{
		PM1->setValue(Shade);
		updateBoxS(Color);
	}
	if (GrColor2->isChecked())
	{
		PM1->setValue(Shade2);
		updateBoxS(Color2);
	}
}

void Cpalette::slotGrad(int nr)
{
	ChooseGrad(nr);
	emit NewGradient(nr, Color, Shade, Color2, Shade2);
}

void Cpalette::ChooseGrad(int nr)
{
	/* PFJ - 29.02.04 - Removed GradGroup and Gradient mode from switch */
	bool test = nr == 0 ? false : true;
	GradGroup->setEnabled(test);
	GradientMode = test;
	switch (nr)
	{
	case 0:
		updateCList();
		updateGeometry();
		repaint();
		PM1->setValue(Shade3);
		updateBoxS(Color3);
		break;
	default:
		updateCList();
		if (GrColor1->isChecked())
		{
			PM1->setValue(Shade);
			updateBoxS(Color);
		}
		if (GrColor2->isChecked())
		{
			PM1->setValue(Shade2);
			updateBoxS(Color2);
		}
		break;
	}
	setFocus();
}

void Cpalette::setActTrans(double val, double val2)
{
	disconnect(TransSpin, SIGNAL(valueChanged(int)), this, SLOT(slotTrans(int)));
	TransSpin->setValue(qRound(100 - (Mode == 1 ? val2 * 100 : val * 100)));
	connect(TransSpin, SIGNAL(valueChanged(int)), this, SLOT(slotTrans(int)));
}

void Cpalette::slotTrans(int val)
{
	if (Mode == 1)
		emit NewTransS(static_cast<double>(100 - val) / 100.0);
	else
		emit NewTrans(static_cast<double>(100 - val) / 100.0);
	setFocus();
}

void Cpalette::UseTrans(bool b)
{
	int h = 0;
	if (b)
		TransGroup->show();
	else
	{
		if (!TransGroup->isHidden())
		{
			h += TransGroup->height();
			TransGroup->hide();
			ListBox1->resize(ListBox1->width(), ListBox1->height()+h);
		}
	}
	updateGeometry();
	repaint();
	UseTransFeature = b;
}

void Cpalette::setActGradient(QString p, QString b, int shp, int shb, int typ)
{
	disconnect(GradCombo, SIGNAL(activated(int)), this, SLOT(slotGrad(int)));
	Color2 = b;
	Color = p;
	Shade = shp;
	Shade2 = shb;
	if (Mode == 2)
	{
		GradCombo->setCurrentItem(typ);
		ChooseGrad(typ);
	}
	connect(GradCombo, SIGNAL(activated(int)), this, SLOT(slotGrad(int)));
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
		if (GradCombo->currentItem() == 0)
		{
			emit NewBrushShade(b);
			Shade3 = b;
		}
		else
		{
			if (GrColor1->isChecked())
				Shade = b;
			else
				Shade2 = b;
			emit NewGradient(GradCombo->currentItem(), Color, Shade, Color2, Shade2);
		}
		break;
	}
}
