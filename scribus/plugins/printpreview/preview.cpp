/***************************************************************************
                          preview.cpp  -  description
                             -------------------
    begin                : Sun Jun 1 08:00:00 CEST 2003
    copyright            : (C) 2003 by Franz Schmid
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
#include "preview.h"
#include "preview.moc"
#include <qimage.h>
#include <cstdlib>

extern QPixmap loadIcon(QString nam);
 
QString Name()
{
  return QObject::tr("Print Preview");
}

int Type()
{
	return 1;
}

void Run(QWidget *d, ScribusApp *plug)
{
	if (plug->HaveDoc)
		{
  	PPreview *dia = new PPreview(d, plug);
  	dia->exec();
  	delete dia;
  	}
}

PPreview::PPreview( QWidget* parent, ScribusApp *pl) : QDialog( parent, "Preview", true, 0 )
{
	QString tmp;
	setCaption( tr("Print Preview"));
	app = pl;
	APage = 0;
	MPage = app->doc->PageC;
	setIcon(loadIcon("AppIcon.png"));
	PLayout = new QVBoxLayout(this, 0, 0, "PLayout");
	Layout5 = new QHBoxLayout;
	Layout5->setSpacing(6);
	Layout5->setMargin(2);
	Layout4 = new QVBoxLayout;
	Layout4->setSpacing(0);
	Layout4->setMargin(0);
	Layout1 = new QHBoxLayout;
	Layout1->setSpacing(6);
	Layout1->setMargin(0);
	First = new QToolButton(this, "First");
	First->setMaximumSize(QSize(24,24));
	First->setText("");
	First->setPixmap(loadIcon("start.png"));
	Layout1->addWidget(First);
	Back = new QToolButton(this, "Back");
	Back->setMaximumSize(QSize(24,24));
	Back->setText("");
	Back->setPixmap(loadIcon("back.png"));
	Layout1->addWidget(Back);
	Text1 = new QLabel(this);
	Text1->setText( tr("Page:"));
	Layout1->addWidget(Text1);
	SeitenAusw = new QComboBox( true, this, "SeitenAusw" );
	SeitenAusw->setEditable(false);
	for (int a = 0; a < MPage; a++)
	{
		SeitenAusw->insertItem(tmp.setNum(a+1));
	}
	Layout1->addWidget(SeitenAusw);
	Forward = new QToolButton(this, "Forward");
	Forward->setMaximumSize(QSize(24,24));
	Forward->setText("");
	Forward->setPixmap(loadIcon("forward.png"));
	Layout1->addWidget(Forward);
	Last = new QToolButton(this, "Last");
	Last->setMaximumSize(QSize(24,24));
	Last->setText("");
	Last->setPixmap(loadIcon("finish.png"));
	Layout1->addWidget(Last);
	QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1->addItem(spacer);
	Layout4->addLayout(Layout1);
	QSpacerItem* spacer2 = new QSpacerItem( 0, 20, QSizePolicy::Minimum, QSizePolicy::Minimum );
	Layout4->addItem(spacer2);
	Layout5->addLayout(Layout4);
	Layout2 = new QVBoxLayout;
	Layout2->setSpacing(1);
	Layout2->setMargin(0);
	Layout3 = new QVBoxLayout;
	Layout3->setSpacing(1);
	Layout3->setMargin(0);
	AliasText = new QCheckBox(this, "TextAntiAlias");
	AliasText->setText( tr("Anti-alias Text"));
	AliasText->setChecked(true);
	Layout2->addWidget(AliasText);
	AliasGr = new QCheckBox(this, "GraphicsAntiAlias");
	AliasGr->setText( tr("Anti-alias Graphics"));
	AliasGr->setChecked(true);
	Layout2->addWidget(AliasGr);
	AliasTr = new QCheckBox(this, "DisplayTransparency");
	AliasTr->setText( tr("Display Transparency"));
	AliasTr->setChecked(false);
	Layout2->addWidget(AliasTr);
	EnableCMYK = new QCheckBox(this, "DisplayCMYK");
	EnableCMYK->setText( tr("Display CMYK"));
	EnableCMYK->setChecked(false);
	Layout2->addWidget(EnableCMYK);
	EnableCMYK_C = new QCheckBox(this, "DisplayCMYK_C");
	EnableCMYK_C->setText( tr("C"));
	EnableCMYK_C->setChecked(true);
	EnableCMYK_C->setEnabled(false);
	Layout3->addWidget(EnableCMYK_C);
	EnableCMYK_M = new QCheckBox(this, "DisplayCMYK_M");
	EnableCMYK_M->setText( tr("M"));
	EnableCMYK_M->setChecked(true);
	EnableCMYK_M->setEnabled(false);
	Layout3->addWidget(EnableCMYK_M);
	EnableCMYK_Y = new QCheckBox(this, "DisplayCMYK_Y");
	EnableCMYK_Y->setText( tr("Y"));
	EnableCMYK_Y->setChecked(true);
	EnableCMYK_Y->setEnabled(false);
	Layout3->addWidget(EnableCMYK_Y);
	EnableCMYK_K = new QCheckBox(this, "DisplayCMYK_K");
	EnableCMYK_K->setText( tr("K"));
	EnableCMYK_K->setChecked(true);
	EnableCMYK_K->setEnabled(false);
	Layout3->addWidget(EnableCMYK_K);
	Layout5->addLayout(Layout2);
	Layout5->addLayout(Layout3);
	PLayout->addLayout(Layout5);
	Anzeige = new QScrollView(this);
	Anz = new QLabel(Anzeige->viewport());
	Anz->setPixmap(CreatePreview(0, 72));
	Anzeige->addChild(Anz, 0, 0);
	PLayout->addWidget(Anzeige);
	int w = Anz->width() + 20;
	resize(QMIN(QApplication::desktop()->width(),w), 500);
	connect(First, SIGNAL(clicked()), this, SLOT(ToStart()));
	connect(Back, SIGNAL(clicked()), this, SLOT(BackW()));
	connect(Forward, SIGNAL(clicked()), this, SLOT(ForW()));
	connect(Last, SIGNAL(clicked()), this, SLOT(ToEnd()));
	connect(AliasText, SIGNAL(clicked()), this, SLOT(ToggleTextAA()));
	connect(AliasGr, SIGNAL(clicked()), this, SLOT(ToggleGr()));
	connect(AliasTr, SIGNAL(clicked()), this, SLOT(ToggleTr()));
	connect(EnableCMYK, SIGNAL(clicked()), this, SLOT(ToggleCMYK()));
	connect(EnableCMYK_C, SIGNAL(clicked()), this, SLOT(ToggleCMYK_Colour()));
	connect(EnableCMYK_M, SIGNAL(clicked()), this, SLOT(ToggleCMYK_Colour()));
	connect(EnableCMYK_Y, SIGNAL(clicked()), this, SLOT(ToggleCMYK_Colour()));
	connect(EnableCMYK_K, SIGNAL(clicked()), this, SLOT(ToggleCMYK_Colour()));
	connect(SeitenAusw, SIGNAL(activated(int)), this, SLOT(ToSeite(int)));
	setPageCombo(0);
}

PPreview::~PPreview()
{
}

void PPreview::ToStart()
{
	if (APage == 0)
		return;
	Anz->setPixmap(CreatePreview(0, 72));
	APage = 0;
	setPageCombo(APage);
}

void PPreview::BackW()
{
	if (APage == 0)
		return;
	APage--;
	Anz->setPixmap(CreatePreview(APage, 72));
	setPageCombo(APage);
}

void PPreview::ForW()
{
	if (APage == MPage-1)
		return;
	APage++;
	Anz->setPixmap(CreatePreview(APage, 72));
	setPageCombo(APage);
}

void PPreview::ToEnd()
{
	if (APage == MPage-1)
		return;
	Anz->setPixmap(CreatePreview(MPage-1, 72));
	APage = MPage-1;
	setPageCombo(APage);
}

void PPreview::ToSeite(int num)
{
	if (num == APage)
		return;
	APage = num;
	Anz->setPixmap(CreatePreview(num, 72));
}

void PPreview::ToggleTextAA()
{
	Anz->setPixmap(CreatePreview(APage, 72));
}

void PPreview::ToggleGr()
{
	Anz->setPixmap(CreatePreview(APage, 72));
}

void PPreview::ToggleTr()
{
	Anz->setPixmap(CreatePreview(APage, 72));
}

void PPreview::ToggleCMYK()
{
	bool c = EnableCMYK->isChecked() ? true : false;
	EnableCMYK_C->setEnabled(c);
	EnableCMYK_M->setEnabled(c);
	EnableCMYK_Y->setEnabled(c);
	EnableCMYK_K->setEnabled(c);
	AliasTr->setEnabled(!c);
	Anz->setPixmap(CreatePreview(APage, 72));
}

void PPreview::ToggleCMYK_Colour()
{
	if (EnableCMYK->isChecked())
		Anz->setPixmap(CreatePreview(APage, 72));
}

void PPreview::setPageCombo(int num)
{
	disconnect(SeitenAusw, SIGNAL(activated(int)), this, SLOT(ToSeite(int)));
	SeitenAusw->setCurrentItem(num);
	bool setter = true;
	if (num == 0)
		setter = false;
	First->setEnabled(setter);
	Back->setEnabled(setter);
	setter = true;
	if (num == MPage-1)
		setter = false;
	Forward->setEnabled(setter);
	Last->setEnabled(setter);
	connect(SeitenAusw, SIGNAL(activated(int)), this, SLOT(ToSeite(int)));
}

QPixmap PPreview::CreatePreview(int Seite, int Res)
{
	int ret = -1;
	QString cmd1, cmd2, tmp, tmp2, tmp3;
	QMap<QString,QFont> ReallyUsed;
	QPixmap Bild;
	ReallyUsed.clear();
	app->GetUsedFonts(&ReallyUsed);
	PSLib *dd = app->getPSDriver(true, app->Prefs.AvailFonts, ReallyUsed, app->doc->PageColors, false);
	if (dd != NULL)
	{
		dd->PS_set_file(app->PrefsPfad+"/tmp.ps");
		app->view->CreatePS(dd, Seite, Seite+1, 1, false, tr("All"), true, false, false, false);
		delete dd;
		app->closePSDriver();
	}
	else
		return Bild;
	double b = app->doc->PageB * Res / 72;
	double h = app->doc->PageH * Res / 72;
	cmd1 = "gs -q -dNOPAUSE -r"+tmp.setNum(Res)+" -g"+tmp2.setNum(qRound(b))+"x"+tmp3.setNum(qRound(h));
	if (EnableCMYK->isChecked())
		cmd1 += " -sDEVICE=bitcmyk -dGrayValues=256";
	else
		{
		cmd1 += AliasTr->isChecked() ? " -sDEVICE=pngalpha" : " -sDEVICE=png16m";
		}
	if (AliasText->isChecked())
		cmd1 += " -dTextAlphaBits=4";
	if (AliasGr->isChecked())
		cmd1 += " -dGraphicsAlphaBits=4";
	cmd1 += " -sOutputFile="+app->PrefsPfad+"/sc.png ";
	cmd2 = " -c showpage -c quit";
	ret = system(cmd1 + app->PrefsPfad+"/tmp.ps" + cmd2);
	if (ret == 0)
	{
		QImage image;
		if (EnableCMYK->isChecked())
		{
			int w = qRound(b);
			int w2 = 4*w;
			int h2 = qRound(h);
			int cyan, magenta, yellow, black;
			uint *p;
			QByteArray imgc(w2*h2);
			image = QImage(w, h2, 32);
			QFile f(app->PrefsPfad+"/sc.png");
			if (f.open(IO_ReadOnly))
			{
				f.readBlock(imgc.data(), w2*h2);
				f.close();
			}
		    for (int y=0; y < h2; ++y ) 
			{
				p = (uint *)image.scanLine( y );
				for (int x=0; x < w2; x += 4 ) 
				{
					cyan = uchar(imgc[(y * w2) + x]);
				magenta = uchar(imgc[(y * w2) + x + 1]);
				yellow = uchar(imgc[(y * w2) + x + 2]);
				black = uchar(imgc[(y * w2)+ x + 3]);
				if (!EnableCMYK_C->isChecked())
					cyan = 0;
				if (!EnableCMYK_M->isChecked())
					magenta = 0;
				if (!EnableCMYK_Y->isChecked())
					yellow = 0;
				if (!EnableCMYK_K->isChecked())
					black = 0;
				CMYKColor color = CMYKColor(cyan, magenta, yellow, black);
				*p = color.getRGBColor().rgb() | 0xff000000;
				p++;
				}
			}
		}
		else
		{
			image.load(app->PrefsPfad+"/sc.png");
 			image = image.convertDepth(32);
		}
		Bild.convertFromImage(image);
		system("rm -f "+app->PrefsPfad+"/sc.png");
	}
	system("rm -f "+app->PrefsPfad+"/tmp.ps");
	return Bild;
}

