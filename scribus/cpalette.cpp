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
	GradCombo->insertItem( tr("Free linear Gradient"));
//	GradCombo->insertItem( tr("Free radial Gradient"));
	GradCombo->setCurrentItem(0);
	GradLayout->addWidget( GradCombo );
	GradEdit = new GradientPreview(this);
	GradLayout->addWidget(GradEdit, Qt::AlignHCenter);
	frame8 = new QFrame( this, "frame8" );
	frame8->setFrameShape( QFrame::NoFrame );
	frame8->setFrameShadow( QFrame::Plain );
	frame8Layout = new QGridLayout( frame8, 1, 1, 5, 5, "frame8Layout");
	GTextX1 = new QLabel( frame8, "GTextX1" );
	GTextX1->setText( tr( "X1:" ) );
	frame8Layout->addWidget( GTextX1, 0, 0 );
	GTextY1 = new QLabel( frame8, "GTextY1" );
	GTextY1->setText( tr( "Y1:" ) );
	frame8Layout->addWidget( GTextY1, 1, 0 );
	gX1 = new MSpinBox( frame8, 2);
	gX1->setDecimals(100);
	gX1->setSuffix( tr( " pt" ) );
	gX1->setMaxValue(3000);
	frame8Layout->addWidget( gX1, 0, 1 );
	gY1 = new MSpinBox( frame8, 2 );
	gY1->setSuffix( tr( " pt" ) );
	gY1->setDecimals(100);
	gY1->setMaxValue(3000);
	frame8Layout->addWidget( gY1, 1, 1 );
	GTextX2 = new QLabel( frame8, "GTextX2" );
	GTextX2->setText( tr( "X2:" ) );
	frame8Layout->addWidget( GTextX2, 0, 2 );
	GTextY2 = new QLabel( frame8, "GTextY2" );
	GTextY2->setText( tr( "Y2:" ) );
	frame8Layout->addWidget( GTextY2, 1, 2 );
	gX2 = new MSpinBox( frame8, 2 );
	gX2->setSuffix( tr( " pt" ) );
	gX2->setDecimals(100);
	gX2->setMaxValue(3000);
	frame8Layout->addWidget( gX2, 0, 3 );
	gY2 = new MSpinBox( frame8, 2 );
	gY2->setSuffix( tr( " pt" ) );
	gY2->setDecimals(100);
	gY2->setMaxValue(3000);
	frame8Layout->addWidget( gY2, 1, 3 );
	GradLayout->addWidget( frame8 );
	Form1Layout->addLayout(GradLayout);
	ListBox1 = new QListBox(this, "ListBox1");
	ListBox1->setMinimumSize( QSize( 150, 30 ) );
	Form1Layout->addWidget(ListBox1);
	setActGradient(0);
	GradientMode = false;
	QToolTip::add( Inhalt, tr( "Edit Line Color Properties" ) );
	QToolTip::add( Innen, tr( "Edit Fill Color Properties" ) );
	QToolTip::add( PM1, tr( "Saturation of color" ) );
	QToolTip::add( GradCombo, tr( "Normal or gradient fill method" ) );
	QToolTip::add( TransSpin, tr( "Set the transparency for the color selected" ) );
	QToolTip::add( ListBox1, tr( "Color of selected object" ) );
	connect(Inhalt, SIGNAL(clicked()), this, SLOT(InhaltButton()));
	connect(Innen, SIGNAL(clicked()), this, SLOT(InnenButton()));
	connect(ListBox1, SIGNAL(clicked(QListBoxItem*)), this, SLOT(selFarbe(QListBoxItem*)));
	connect(PM1, SIGNAL(valueChanged(int)), this, SLOT(setActShade()));
	connect(GradCombo, SIGNAL(activated(int)), this, SLOT(slotGrad(int)));
	connect(TransSpin, SIGNAL(valueChanged(int)), this, SLOT(slotTrans(int)));
	connect(gX1, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	connect(gX2, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	connect(gY1, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	connect(gY2, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	connect(GradEdit, SIGNAL(selectedColor(QString, int )), this, SLOT(slotColor(QString, int )));
}

void Cpalette::InhaltButton()
{
	int h = 0;
	if (Inhalt->isOn())
	{
		Mode = 1;
		Innen->setOn(false);
		h += GradCombo->height();
		h += GradEdit->height();
		if (GradCombo->currentItem() < 6)
			h -= frame8->height();
		frame8->hide();
		h += TransGroup->height();
		GradCombo->hide();
		GradEdit->hide();
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
	int h = 0;
	if (Innen->isOn())
	{
		Mode = 2;
		Inhalt->setOn(false);
		GradCombo->show();
		GradientMode = GradCombo->currentItem() != 0 ? true : false;
		h += GradCombo->height();
		h += TransGroup->height();
		if (GradientMode)
		{
			if (GradEdit->isHidden())
				GradEdit->show();
			if (GradCombo->currentItem() > 5)
				frame8->show();
			else
				frame8->hide();
			h += GradEdit->height();
		}
		ListBox1->resize(ListBox1->width(), ListBox1->height()-h);
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
			GradEdit->setActColor(SetFarbe(sFarbe, Shade), sFarbe, Shade);
			Color = sFarbe;
			emit gradientChanged();
		}
		break;
	}
}

QColor Cpalette::SetFarbe(QString farbe, int shad)
{
	int h, s, v, sneu;
	QColor tmp;
	Farbliste[farbe].getRGBColor().rgb(&h, &s, &v);
	if ((h == s) && (s == v))
	{
		Farbliste[farbe].getRGBColor().hsv(&h, &s, &v);
		sneu = 255 - ((255 - v) * shad / 100);
		tmp.setHsv(h, s, sneu);
	}
	else
	{
		Farbliste[farbe].getRGBColor().hsv(&h, &s, &v);
		sneu = s * shad / 100;
		tmp.setHsv(h, sneu, v);
	}
	return tmp;
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
	disconnect(PM1, SIGNAL(valueChanged(int)), this, SLOT(setActShade()));
	Color = n;
	Shade = s;
	PM1->setValue(Shade);
	updateBoxS(Color);
	connect(PM1, SIGNAL(valueChanged(int)), this, SLOT(setActShade()));
}

void Cpalette::slotGrad(int nr)
{
	ChooseGrad(nr);
	emit NewGradient(nr);
}

void Cpalette::ChooseGrad(int nr)
{
	/* PFJ - 29.02.04 - Removed GradGroup and Gradient mode from switch */
	bool test = nr == 0 ? false : true;
	GradientMode = test;
	if (nr != 0)
	{
		GradEdit->show();
		if (nr > 5)
			frame8->show();
		else
			frame8->hide();
	}
	else
	{
		GradEdit->hide();
		frame8->hide();
	}
	updateGeometry();
	repaint();
	disconnect(PM1, SIGNAL(valueChanged(int)), this, SLOT(setActShade()));
	switch (nr)
	{
	case 0:
		updateCList();
		PM1->setValue(Shade3);
		updateBoxS(Color3);
		break;
	default:
		updateCList();
		PM1->setValue(Shade);
		updateBoxS(Color);
		break;
	}
	setFocus();
	connect(PM1, SIGNAL(valueChanged(int)), this, SLOT(setActShade()));
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

void Cpalette::setActGradient(int typ)
{
	disconnect(GradCombo, SIGNAL(activated(int)), this, SLOT(slotGrad(int)));
	if (Mode == 2)
	{
		GradCombo->setCurrentItem(typ);
		ChooseGrad(typ);
	}
	connect(GradCombo, SIGNAL(activated(int)), this, SLOT(slotGrad(int)));
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
		if (GradCombo->currentItem() == 0)
		{
			Shade3 = b;
			emit NewBrushShade(b);
		}
		else
		{
			GradEdit->setActColor(SetFarbe(Color, b), Color, b);
			Shade = b;
			emit gradientChanged();
		}
		break;
	}
}

void Cpalette::UnitChange(double old, double neww, int ein)
{
	disconnect(gX1, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	disconnect(gX2, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	disconnect(gY1, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	disconnect(gY2, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	double oldX = gX1->value() / old;
	double oldXM = gX1->maxValue() / old;
	double oldY = gY1->value() / old;
	double oldYM = gY1->maxValue() / old;
	double oldW = gX2->value() / old;
	double oldWM = gX2->maxValue() / old;
	double oldH = gY2->value() / old;
	double oldHM = gY2->maxValue() / old;
	gX1->setDecimals(100);
	gY1->setDecimals(100);
	gX2->setDecimals(100);
	gY2->setDecimals(100);
	QString point[] = { tr(" pt"), tr(" mm"), tr(" in"), tr(" p")};
	QString einh = point[ein];
	int val = ein == 1 ? 1000 : 10000;
	if ((ein == 1) || (ein == 2))
	{
		gX1->setDecimals(val);
		gY1->setDecimals(val);
		gX2->setDecimals(val);
		gY2->setDecimals(val);
	}
	gX1->setSuffix( einh );
	gY1->setSuffix( einh );
	gX2->setSuffix( einh );
	gY2->setSuffix( einh );
	gX1->setMaxValue(oldXM * neww);
	gX1->setValue(oldX * neww);
	gY1->setMaxValue(oldYM * neww);
	gY1->setValue(oldY * neww);
	gX2->setMaxValue(oldWM * neww);
	gX2->setValue(oldW * neww);
	gY2->setMaxValue(oldHM * neww);
	gY2->setValue(oldH * neww);
	connect(gX1, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	connect(gX2, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	connect(gY1, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
	connect(gY2, SIGNAL(valueChanged(int)), this, SLOT(changeSpecial()));
}
