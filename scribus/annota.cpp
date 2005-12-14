/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
 
#include <qstringlist.h>

#include "scfonts.h"
#include "annota.h"
#include "annota.moc"
#include "customfdialog.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "commonstrings.h"
#include "pageitem.h"
#include "navigator.h"
#include "scribusview.h"

extern QPixmap loadIcon(QString nam);

Annota::Annota(QWidget* parent, PageItem *it, int Seite, int b, int h, ScribusView* vie)
		: QDialog( parent, "AN", true, 0 )
{
	setCaption( tr( "Annotation Properties" ) );
	setIcon(loadIcon("AppIcon.png"));
	item = it;
	Breite = b;
	Hoehe = h;
	OriBreite = b;
	OriHoehe = h;
	view = vie;
	MaxSeite = Seite;
	QStringList tl;
	if ((item->annotation().ActionType() == 2) || (item->annotation().ActionType() == 7))
	{
		QString tm = item->annotation().Action();
		tl = tl.split(" ", tm);
	}
	else
	{
		tl.append("0");
		tl.append("0");
	}

	AnnotLayout = new QVBoxLayout( this );
	AnnotLayout->setSpacing( 6 );
	AnnotLayout->setMargin( 11 );

	Layout1 = new QHBoxLayout;
	Layout1->setSpacing( 6 );
	Layout1->setMargin( 0 );

	ComboBox1 = new QComboBox( true, this, "ComboBox1" );
	/* PFJ - 28/02/04 - Changed to QString/size_t/for style */
	QString combo[] = { tr("Text"), tr("Link"), tr("External Link"),
	                    tr("External Web-Link")};
	size_t comboArray = sizeof(combo)/sizeof(*combo);
	for (uint prop = 0; prop < comboArray; ++prop)
		ComboBox1->insertItem(combo[prop]);
	ComboBox1->setEditable(false);
	TextLabel1 = new QLabel( ComboBox1, tr("&Type:"), this, "TextLabel1" );
	Layout1->addWidget( TextLabel1 );
	Layout1->addWidget( ComboBox1 );
	AnnotLayout->addLayout( Layout1 );
	item->annotation().Type() < 2 ? ComboBox1->setCurrentItem(item->annotation().Type()):
	ComboBox1->setCurrentItem(item->annotation().Type()-10);
	if ((item->annotation().ActionType() == 7) || (item->annotation().ActionType() == 8))
		ComboBox1->setCurrentItem(item->annotation().ActionType() - 5);
	Fram = new QWidgetStack(this);
	AnnotLayout->addWidget( Fram );

	GroupBox1 = new QGroupBox( this, "GroupBox1" );
	GroupBox1->setTitle( tr( "Destination" ) );
	GroupBox1->setColumnLayout(0, Qt::Vertical );
	GroupBox1->layout()->setSpacing( 0 );
	GroupBox1->layout()->setMargin( 0 );
	GroupBox1Layout = new QGridLayout( GroupBox1->layout() );
	GroupBox1Layout->setAlignment( Qt::AlignTop );
	GroupBox1Layout->setSpacing( 6 );
	GroupBox1Layout->setMargin( 11 );

	Destfile = new QLineEdit(GroupBox1, "File");
	Destfile->setText(item->annotation().Extern());
	Destfile->setReadOnly(true);
	GroupBox1Layout->addMultiCellWidget( Destfile, 0, 0, 0, 1 );
	ChFile = new QPushButton(GroupBox1, "Change");
	ChFile->setText( tr("C&hange..."));
	GroupBox1Layout->addWidget( ChFile, 0, 2 );
	if ((item->annotation().ActionType() != 7) && (item->annotation().ActionType() != 8))
	{
		Destfile->hide();
		ChFile->hide();
	}

	SpinBox1 = new QSpinBox( GroupBox1, "SpinBox1" );
	SpinBox1->setMinValue(1);
	SpinBox1->setMaxValue(item->annotation().ActionType() == 7 ? 1000 : Seite);
	SpinBox1->setValue(item->annotation().Ziel()+1);
	TextLabel3 = new QLabel( SpinBox1, tr("&Page:"), GroupBox1, "TextLabel3" );
	GroupBox1Layout->addWidget( TextLabel3, 1, 0 );
	GroupBox1Layout->addWidget( SpinBox1, 1, 1 );
	if ((!Destfile->text().isEmpty()) && (item->annotation().ActionType() == 7))
		Pg = new Navigator( GroupBox1, 100, item->annotation().Ziel()+1, view, item->annotation().Extern());
	else
		Pg = new Navigator( GroupBox1, 100, item->annotation().Ziel(), view);
	Pg->setMinimumSize(QSize(Pg->pmx.width(), Pg->pmx.height()));
	GroupBox1Layout->addMultiCellWidget(Pg, 1, 3, 2, 2);

	SpinBox2 = new QSpinBox( GroupBox1, "SpinBox2" );
	SpinBox2->setSuffix( tr( " pt" ) );
	SpinBox2->setMaxValue(Breite);
	SpinBox2->setValue(tl[0].toInt());
	TextLabel4 = new QLabel( SpinBox2, tr("&X-Pos"), GroupBox1, "TextLabel4" );
	GroupBox1Layout->addWidget( TextLabel4, 2, 0 );
	GroupBox1Layout->addWidget( SpinBox2, 2, 1 );
	SpinBox3 = new QSpinBox( GroupBox1, "SpinBox3" );
	SpinBox3->setMaxValue(Hoehe);
	SpinBox3->setSuffix( tr( " pt" ) );
	SpinBox3->setValue(Hoehe-tl[1].toInt());
	TextLabel5 = new QLabel( SpinBox3, tr("&Y-Pos:"), GroupBox1, "TextLabel5" );
	GroupBox1Layout->addWidget( TextLabel5, 3, 0 );
	GroupBox1Layout->addWidget( SpinBox3, 3, 1 );
	Fram->addWidget(GroupBox1, 1);

	Frame9 = new QFrame( this, "Frame7" );
	Frame9->setFrameShape( QFrame::NoFrame );
	Frame9->setFrameShadow( QFrame::Plain );
	Fram->addWidget(Frame9, 2);

	Layout1_2 = new QHBoxLayout;
	Layout1_2->setSpacing( 6 );
	Layout1_2->setMargin( 0 );

	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1_2->addItem( spacer );
	PushButton1 = new QPushButton( CommonStrings::tr_OK, this, "PushButton1" );
	PushButton1->setDefault( true );
	Layout1_2->addWidget( PushButton1 );
	PushButton2 = new QPushButton( CommonStrings::tr_Cancel, this, "PushButton2" );
	Layout1_2->addWidget( PushButton2 );
	AnnotLayout->addLayout( Layout1_2 );

	connect(PushButton1, SIGNAL(clicked()), this, SLOT(SetVals()));
	connect(PushButton2, SIGNAL(clicked()), this, SLOT(reject()));
	connect(ComboBox1, SIGNAL(activated(int)), this, SLOT(SetZiel(int)));
	connect(SpinBox1, SIGNAL(valueChanged(int)), this, SLOT(SetPg(int)));
	connect(Pg, SIGNAL(Coords(double, double)), this, SLOT(SetCo(double, double)));
	connect(SpinBox2, SIGNAL(valueChanged(int)), this, SLOT(SetCross()));
	connect(SpinBox3, SIGNAL(valueChanged(int)), this, SLOT(SetCross()));
	connect(ChFile, SIGNAL(clicked()), this, SLOT(GetFile()));
	SetZiel(item->annotation().Type());
	SetCross();
}

void Annota::SetCo(double x, double y)
{
	SpinBox2->setValue(static_cast<int>(x*Breite));
	SpinBox3->setValue(static_cast<int>(y*Hoehe));
}

void Annota::SetPg(int v)
{
	disconnect(SpinBox1, SIGNAL(valueChanged(int)), this, SLOT(SetPg(int)));
	if (ComboBox1->currentItem() == 2)
	{
		if (!Pg->SetSeite(v, 100, Destfile->text()))
		{
			SpinBox1->setValue(1);
			Pg->SetSeite(1, 100, Destfile->text());
		}
		Breite = Pg->Breite;
		Hoehe = Pg->Hoehe;
	}
	else
	{
		Pg->SetSeite(v-1, 100);
		SpinBox1->setValue(v);
		Breite = OriBreite;
		Hoehe = OriHoehe;
	}
	SpinBox2->setMaxValue(Breite);
	SpinBox3->setMaxValue(Hoehe);
	connect(SpinBox1, SIGNAL(valueChanged(int)), this, SLOT(SetPg(int)));
}

void Annota::SetCross()
{
	int x,y;
	disconnect(Pg, SIGNAL(Coords(double, double)), this, SLOT(SetCo(double, double)));
	x = static_cast<int>(static_cast<double>(SpinBox2->value())/static_cast<double>(Breite)*Pg->pmx.width());
	y = static_cast<int>(static_cast<double>(SpinBox3->value())/static_cast<double>(Hoehe)*Pg->pmx.height());
	Pg->drawMark(x, y);
	connect(Pg, SIGNAL(Coords(double, double)), this, SLOT(SetCo(double, double)));
}

void Annota::SetVals()
{
	QString tmp, tmp2;
	item->annotation().setZiel(SpinBox1->value()-1);
	item->annotation().setType(ComboBox1->currentItem()+10);
	switch (item->annotation().Type())
	{
	case 10:
		item->annotation().setActionType(0);
		break;
	case 11:
		item->annotation().setAction(tmp.setNum(SpinBox2->value())+" "+ tmp2.setNum(Hoehe-SpinBox3->value())+" 0");
		item->annotation().setExtern("");
		item->annotation().setActionType(2);
		break;
	case 12:
		item->annotation().setAction(tmp.setNum(SpinBox2->value())+" "+ tmp2.setNum(Hoehe-SpinBox3->value())+" 0");
		if (!Destfile->text().isEmpty())
		{
			item->annotation().setExtern(Destfile->text());
			item->annotation().setActionType(7);
		}
		item->annotation().setType(11);
		break;
	case 13:
		item->annotation().setAction("");
		if (!Destfile->text().isEmpty())
		{
			item->annotation().setExtern(Destfile->text());
			item->annotation().setActionType(8);
		}
		item->annotation().setType(11);
		break;
	}
	accept();
}

void Annota::SetZiel(int it)
{
	disconnect(ComboBox1, SIGNAL(activated(int)), this, SLOT(SetZiel(int)));
	Pg->show();
	TextLabel3->show();
	TextLabel4->show();
	TextLabel5->show();
	SpinBox1->show();
	SpinBox2->show();
	SpinBox3->show();
	switch (it)
	{
	case 1:
		Fram->raiseWidget(1);
		Destfile->setText("");
		Destfile->hide();
		ChFile->hide();
		item->annotation().setActionType(2);
		SetPg(QMIN(SpinBox1->value(), MaxSeite));
		break;
	case 2:
		Fram->raiseWidget(1);
		Destfile->show();
		ChFile->show();
		Destfile->setReadOnly(true);
		if ((Destfile->text().isEmpty())  || (item->annotation().ActionType() == 8))
		{
			Destfile->setText("");
			GetFile();
		}
		if (Destfile->text().isEmpty())
		{
			item->annotation().setActionType(2);
			Destfile->setText("");
			Destfile->hide();
			ChFile->hide();
			ComboBox1->setCurrentItem(1);
		}
		else
			item->annotation().setActionType(7);
		SetPg(QMIN(SpinBox1->value(), MaxSeite));
		break;
	case 3:
		Fram->raiseWidget(1);
		Destfile->show();
		Destfile->setReadOnly(false);
		ChFile->hide();
		Pg->hide();
		TextLabel3->hide();
		TextLabel4->hide();
		TextLabel5->hide();
		SpinBox1->hide();
		SpinBox2->hide();
		SpinBox3->hide();
		item->annotation().setActionType(8);
		break;
	case 11:
		Fram->raiseWidget(1);
		if (item->annotation().ActionType() == 7)
		{
			Destfile->show();
			ChFile->show();
			Destfile->setReadOnly(true);
		}
		if (item->annotation().ActionType() == 8)
		{
			Destfile->show();
			Destfile->setReadOnly(false);
			ChFile->hide();
			Pg->hide();
			TextLabel3->hide();
			TextLabel4->hide();
			TextLabel5->hide();
			SpinBox1->hide();
			SpinBox2->hide();
			SpinBox3->hide();
		}
		if (Pg->isVisible())
			SetPg(QMIN(SpinBox1->value(), MaxSeite));
		break;
	default:
		Fram->raiseWidget(2);
		break;
	}
	connect(ComboBox1, SIGNAL(activated(int)), this, SLOT(SetZiel(int)));
}

void Annota::GetFile()
{
	QString fn;
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString wdir = dirs->get("annot_getfile", ".");
	CustomFDialog dia(this, wdir, tr("Open"), tr("PDF-Documents (*.pdf);;All Files (*)"));
	if (!Destfile->text().isEmpty())
		dia.setSelection(Destfile->text());
	if (dia.exec() == QDialog::Accepted)
	{
		fn = dia.selectedFile();
		if (!fn.isEmpty())
		{
			dirs->set("annot_getfile", fn.left(fn.findRev("/")));
			Destfile->setText(fn);
			SpinBox1->setValue(1);
			SpinBox1->setMaxValue(1000);
			SetPg(1);
		}
	}
}
