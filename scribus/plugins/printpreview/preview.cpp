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
#include <qcursor.h>
#include <qpainter.h>
#include <qcolor.h>
#include <qtooltip.h>
#include <qfile.h>
#include "libpostscript/pslib.h"

extern QPixmap loadIcon(QString nam);
extern void ReOrderText(ScribusDoc *doc, ScribusView *view);
ScribusApp* Carrier;
QWidget* par;

/*!
 \fn QString Name()
 \author Franz Schmid
 \date
 \brief Returns name of plugin
 \param None
 \retval QString containing name of plugin: Print Preview
 */
QString Name()
{
	return QObject::tr("Print Preview");
}

/*!
 \fn int Type()
 \author Franz Schmid
 \date
 \brief Returns type of plugin
 \param None
 \retval int containing type of plugin (1: Extra, 2: Import, 3: Export, 4: )
 */
int Type()
{
	return 5;
}

int ID()
{
	return 5;
}

void InitPlug(QWidget *d, ScribusApp *plug)
{
	Carrier = plug;
	par = d;
	Tes = new MenuPreview(d);
	int id = plug->fileMenu->insertItem(QObject::tr("Print Previe&w"), -1, plug->fileMenu->indexOf(plug->M_Print)+1);
	plug->fileMenu->setAccel(Qt::CTRL+Qt::SHIFT+Qt::Key_P, id);
	plug->fileMenu->connectItem(id, Tes, SLOT(RunPreview()));
	plug->fileMenu->setItemEnabled(id, 0);
	plug->MenuItemsFile.append(id);
	plug->SetKeyEntry(18, QObject::tr("Print Preview"), id, Qt::CTRL+Qt::SHIFT+Qt::Key_P);
}

void CleanUpPlug()
{}

/*!
 \fn void Run(QWidget *d, ScribusApp *plug)
 \author Franz Schmid
 \date
 \brief Run the print preview
 \param d QWidget *
 \param plug ScribusApp *
 \retval None
 */
 /*
void Run(QWidget *d, ScribusApp *plug)
{} */

void MenuPreview::RunPreview()
{
	if (Carrier->HaveDoc)
	{
		PPreview *dia = new PPreview(par, Carrier);
		dia->exec();
		Carrier->Prefs.PrPr_Mode = dia->EnableCMYK->isChecked();
		Carrier->Prefs.PrPr_AlphaText = dia->AliasText->isChecked();
		Carrier->Prefs.PrPr_AlphaGraphics = dia->AliasGr->isChecked();
		Carrier->Prefs.PrPr_Transparency = dia->AliasTr->isChecked();
		Carrier->Prefs.PrPr_C = dia->EnableCMYK_C->isChecked();
		Carrier->Prefs.PrPr_M = dia->EnableCMYK_M->isChecked();
		Carrier->Prefs.PrPr_Y = dia->EnableCMYK_Y->isChecked();
		Carrier->Prefs.PrPr_K = dia->EnableCMYK_K->isChecked();
		Carrier->Prefs.Gcr_Mode = dia->EnableGCR->isChecked();
		delete dia;
		system("rm -f "+Carrier->PrefsPfad+"/tmp.ps");
		system("rm -f "+Carrier->PrefsPfad+"/sc.png");
	}
}

/*!
 \fn PPreview::PPreview( QWidget* parent, ScribusApp *pl)
 \author Franz Schmid
 \date
 \brief Create the Print Preview window
 \param parent QWidget *
 \param pl ScribusApp *
 \retval PPreview window
 */
PPreview::PPreview( QWidget* parent, ScribusApp *pl) : QDialog( parent, "Preview", true, 0 )
{
	QString tmp;
	setCaption( tr("Print Preview"));
	app = pl;
	APage = -1;
	MPage = app->doc->PageC;
	CMode = false;
	TxtAl = false;
	GrAl = false;
	Trans = false;
	GMode = true;
	setIcon(loadIcon("AppIcon.png"));
	PLayout = new QVBoxLayout(this, 0, 0, "PLayout");

	Layout1 = new QHBoxLayout;
	Layout1->setSpacing(5);
	Layout1->setMargin(5);
	PGSel = new PageSelector(this, MPage);
	Layout1->addWidget(PGSel);

	Layout2 = new QVBoxLayout();
	Layout2->setSpacing(0);
	Layout2->setMargin(0);
	AliasText = new QCheckBox(this, "TextAntiAlias");
	AliasText->setText( tr("Anti-alias &Text"));
	AliasText->setChecked(app->Prefs.PrPr_AlphaText);
	Layout2->addWidget(AliasText);
	AliasGr = new QCheckBox(this, "GraphicsAntiAlias");
	AliasGr->setText( tr("Anti-alias &Graphics"));
	AliasGr->setChecked(app->Prefs.PrPr_AlphaGraphics);
	Layout2->addWidget(AliasGr);
	Layout1->addLayout(Layout2);

	Layout3 = new QVBoxLayout();
	Layout3->setSpacing(0);
	Layout3->setMargin(0);
	AliasTr = new QCheckBox(this, "DisplayTransparency");
	AliasTr->setText( tr("Display Trans&parency"));
	AliasTr->setChecked(app->Prefs.PrPr_Transparency);
	Layout3->addWidget(AliasTr);
	EnableGCR = new QCheckBox(this, "DisplayGCR");
	EnableGCR->setText( tr("&Under Color Removal"));
	EnableGCR->setChecked(app->Prefs.Gcr_Mode);
	Layout3->addWidget(EnableGCR);
	Layout1->addLayout(Layout3);

	Layout4 = new QVBoxLayout();
	Layout4->setSpacing(0);
	Layout4->setMargin(0);
	EnableCMYK = new QCheckBox(this, "DisplayCMYK");
	EnableCMYK->setText( tr("&Display CMYK"));
	EnableCMYK->setChecked(app->Prefs.PrPr_Mode);
	Layout4->addWidget(EnableCMYK);
	Layout5 = new QHBoxLayout();
	Layout5->setSpacing(0);
	Layout5->setMargin(0);
	EnableCMYK_C = new QCheckBox(this, "DisplayCMYK_C");
	EnableCMYK_C->setText( tr("&C"));
	EnableCMYK_C->setChecked(app->Prefs.PrPr_C);
	Layout5->addWidget(EnableCMYK_C);
	EnableCMYK_M = new QCheckBox(this, "DisplayCMYK_M");
	EnableCMYK_M->setText( tr("&M"));
	EnableCMYK_M->setChecked(app->Prefs.PrPr_M);
	Layout5->addWidget(EnableCMYK_M);
	EnableCMYK_Y = new QCheckBox(this, "DisplayCMYK_Y");
	EnableCMYK_Y->setText( tr("&Y"));
	EnableCMYK_Y->setChecked(app->Prefs.PrPr_Y);
	Layout5->addWidget(EnableCMYK_Y);
	EnableCMYK_K = new QCheckBox(this, "DisplayCMYK_K");
	EnableCMYK_K->setText( tr("&K"));
	EnableCMYK_K->setChecked(app->Prefs.PrPr_K);
	Layout5->addWidget(EnableCMYK_K);
	Layout4->addLayout(Layout5);
	Layout1->addLayout(Layout4);

	PLayout->addLayout(Layout1);

	Anzeige = new QScrollView(this);
	Anz = new QLabel(Anzeige->viewport());
	Anz->setPixmap(CreatePreview(0, 72));
	Anzeige->addChild(Anz, 0, 0);
	PLayout->addWidget(Anzeige);
	int w = Anz->width() + 20;
	resize(QMIN(QApplication::desktop()->width(),w), 500);
	if (!app->Prefs.PrPr_Mode)
	{
		EnableCMYK_C->setEnabled(false);
		EnableCMYK_M->setEnabled(false);
		EnableCMYK_Y->setEnabled(false);
		EnableCMYK_K->setEnabled(false);
	}
	//tooltips
	QToolTip::add( AliasText, tr( "Provides a more pleasant view of text items in the viewer, at the expense\n"
                                              "of a slight slowdown in previewing. This only affects Type 1 fonts" ) );
	QToolTip::add( AliasGr, tr( "Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and\n"
	                                       "vector graphics in the preview, at the expense of a slight slowdown in previewing" ) );
	QToolTip::add( AliasTr, tr( "Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later" ) );
	QToolTip::add( EnableCMYK, tr( "Gives a print preview using simulations of generic CMYK inks, instead of RGB colors" ) );
	QToolTip::add( EnableCMYK_C, tr( "Enable/disable the C (Cyan) ink plate" ) );
	QToolTip::add( EnableCMYK_M, tr( "Enable/disable the M (Magenta) ink plate" ) );
	QToolTip::add( EnableCMYK_Y, tr( "Enable/disable the Y (Yellow) ink plate" ) );
	QToolTip::add( EnableCMYK_K, tr( "Enable/disable the K (Black) ink plate" ) );
	QToolTip::add( EnableGCR, tr( "A way of switching off some of the gray shades which are composed\n"
	                                             "of cyan, yellow and magenta and using black instead.\n"
                                                 "UCR most affects parts of images which are neutral and/or dark tones\n"
                                                 "which are close to the gray. Use of this may improve printing some images\n"
                                                 "and some experimentation and testing is need on a case by case basis.\n"
                                                 "UCR reduces the possibility of over saturation with CMY inks." ) );
	//signals and slots
	connect(AliasText, SIGNAL(clicked()), this, SLOT(ToggleTextAA()));
	connect(AliasGr, SIGNAL(clicked()), this, SLOT(ToggleGr()));
	connect(AliasTr, SIGNAL(clicked()), this, SLOT(ToggleTr()));
	connect(EnableCMYK, SIGNAL(clicked()), this, SLOT(ToggleCMYK()));
	connect(EnableGCR, SIGNAL(clicked()), this, SLOT(ToggleGCR()));
	connect(EnableCMYK_C, SIGNAL(clicked()), this, SLOT(ToggleCMYK_Colour()));
	connect(EnableCMYK_M, SIGNAL(clicked()), this, SLOT(ToggleCMYK_Colour()));
	connect(EnableCMYK_Y, SIGNAL(clicked()), this, SLOT(ToggleCMYK_Colour()));
	connect(EnableCMYK_K, SIGNAL(clicked()), this, SLOT(ToggleCMYK_Colour()));
	connect(PGSel, SIGNAL(GotoPage(int)), this, SLOT(ToSeite(int)));
}

/*!
 \fn void PPreview::ToSeite(int num)
 \author Franz Schmid
 \date
 \brief Jump to newly selected page and create the new preview
 \param num int Page Number
 \retval None
 */
void PPreview::ToSeite(int num)
{
	int n = num-1;
	if (n == APage)
		return;
	Anz->setPixmap(CreatePreview(n, 72));
}

/*!
 \fn void PPreview::ToggleTextAA()
 \author Franz Schmid
 \date
 \brief Toggle Text anti-aliasing and create the new preview
 \param None
 \retval None
 */
void PPreview::ToggleTextAA()
{
	Anz->setPixmap(CreatePreview(APage, 72));
}

/*!
 \fn void PPreview::ToggleGr()
 \author Franz Schmid
 \date
 \brief Toggle Graphics anti-aliasing and create the new preview
 \param None
 \retval None
 */
void PPreview::ToggleGr()
{
	Anz->setPixmap(CreatePreview(APage, 72));
}

/*!
 \fn void PPreview::ToggleTr()
 \author Franz Schmid
 \date
 \brief Toggle transparency and create the new preview
 \param None
 \retval None
 */
void PPreview::ToggleTr()
{
	Anz->setPixmap(CreatePreview(APage, 72));
}

/*!
 \fn void PPreview::ToggleCMYK()
 \author Craig Bradney
 \date
 \brief When CMYK preview is toggled, (dis)enable the CMYK controls and create the new preview
 \param None
 \retval None
 */
void PPreview::ToggleCMYK()
{
	bool c = EnableCMYK->isChecked() ? true : false;
	EnableCMYK_C->setEnabled(c);
	EnableCMYK_M->setEnabled(c);
	EnableCMYK_Y->setEnabled(c);
	EnableCMYK_K->setEnabled(c);
	Anz->setPixmap(CreatePreview(APage, 72));
}

void PPreview::ToggleGCR()
{
	Anz->setPixmap(CreatePreview(APage, 72));
}
/*!
 \fn void PPreview::ToggleCMYK_Colour()
 \author Craig Bradney
 \date
 \brief If CMYK preview is enabled, create a new preview with the new CMYK plate settings
 \param None
 \retval None
 */
void PPreview::ToggleCMYK_Colour()
{
	if (EnableCMYK->isChecked())
		Anz->setPixmap(CreatePreview(APage, 72));
}

/*!
 \fn void PPreview::RenderPreview(int Seite, int Res)
 \author Franz Schmid
 \date
 \brief Renders the Preview to a file on Disk
 \param Seite int page number
 \param Res int
 \retval bool Flag indicating succsess
 */
int PPreview::RenderPreview(int Seite, int Res)
{
	bool ret = -1;
	QString cmd1, cmd2, tmp, tmp2, tmp3;
	QMap<QString,QFont> ReallyUsed;
	// Recreate Postscript-File only when the actual Page has changed
	if ((Seite != APage)  || (EnableGCR->isChecked() != GMode))
	{
		ReallyUsed.clear();
		app->GetUsedFonts(&ReallyUsed);
		PSLib *dd = app->getPSDriver(true, app->Prefs.AvailFonts, ReallyUsed, app->doc->PageColors, false);
		if (dd != NULL)
		{
			dd->PS_set_file(app->PrefsPfad+"/tmp.ps");
			std::vector<int> pageNs;
			pageNs.push_back(Seite+1);
			dd->CreatePS(app->doc, app->view, pageNs, false, tr("All"), true, false, false, false, EnableGCR->isChecked());
			delete dd;
			app->closePSDriver();
		}
		else
			return ret;
	}
	double b = app->doc->PageB * Res / 72;
	double h = app->doc->PageH * Res / 72;
	cmd1 = app->Prefs.gs_exe;
	cmd1 += " -q -dNOPAUSE -r"+tmp.setNum(Res)+" -g"+tmp2.setNum(qRound(b))+"x"+tmp3.setNum(qRound(h));
	if (EnableCMYK->isChecked())
		cmd1 += " -sDEVICE=bitcmyk -dGrayValues=256";
	else
	{
		if ((!AliasTr->isChecked()) || (app->HavePngAlpha != 0))
			cmd1 += " -sDEVICE=png16m";
		else
			cmd1 += " -sDEVICE=pngalpha";
	}
	if (AliasText->isChecked())
		cmd1 += " -dTextAlphaBits=4";
	if (AliasGr->isChecked())
		cmd1 += " -dGraphicsAlphaBits=4";
	cmd1 += " -sOutputFile="+app->PrefsPfad+"/sc.png ";
	cmd2 = " -c showpage -c quit";
	ret = system(cmd1 + app->PrefsPfad+"/tmp.ps" + cmd2);
	return ret;
}

/*!
 \fn QPixmap PPreview::CreatePreview(int Seite, int Res)
 \author Franz Schmid
 \date
 \brief Creates the Preview of the Actual Page
 \param Seite int page number
 \param Res int
 \retval Bild QPixmap print preview
 */
QPixmap PPreview::CreatePreview(int Seite, int Res)
{
	int ret = -1;
	QPixmap Bild;
	double b = app->doc->PageB * Res / 72;
	double h = app->doc->PageH * Res / 72;
	qApp->setOverrideCursor(QCursor(waitCursor), true);
	if ((Seite != APage) || (EnableCMYK->isChecked() != CMode)
	        || (AliasText->isChecked() != TxtAl) || (AliasGr->isChecked() != GrAl) || (EnableGCR->isChecked() != GMode)
	        || ((AliasTr->isChecked() != Trans) && (!EnableCMYK->isChecked())))
	{
		ret = RenderPreview(Seite, Res);
		if (ret != 0)
		{
			Bild.resize(1,1);
			qApp->setOverrideCursor(QCursor(arrowCursor), true);
			return Bild;
		}
	}
	APage = Seite;
	CMode = EnableCMYK->isChecked();
	TxtAl = AliasText->isChecked();
	GrAl = AliasGr->isChecked();
	Trans = AliasTr->isChecked();
	GMode = EnableGCR->isChecked();
	QImage image;
	if (EnableCMYK->isChecked())
	{
		int w = qRound(b);
		int w2 = 4*w;
		int h2 = qRound(h);
		int cyan, magenta, yellow, black, alpha;
		uint *p;
		QByteArray imgc(w2);
		image = QImage(w, h2, 32);
		QFile f(app->PrefsPfad+"/sc.png");
		if (f.open(IO_ReadOnly))
		{
			for (int y=0; y < h2; ++y )
			{
				p = (uint *)image.scanLine( y );
				f.readBlock(imgc.data(), w2);
				for (int x=0; x < w2; x += 4 )
				{
					cyan = uchar(imgc[x]);
					magenta = uchar(imgc[x + 1]);
					yellow = uchar(imgc[x + 2]);
					black = uchar(imgc[x + 3]);
					if (!EnableCMYK_C->isChecked())
						cyan = 0;
					if (!EnableCMYK_M->isChecked())
						magenta = 0;
					if (!EnableCMYK_Y->isChecked())
						yellow = 0;
					if (!EnableCMYK_K->isChecked())
						black = 0;
					if (AliasTr->isChecked() && ((cyan == 0) && (magenta == 0) && (yellow == 0 ) && (black == 0)))
						alpha = 0;
					else
						alpha = 255;
					*p = qRgba(255-QMIN(255, cyan+black), 255-QMIN(255,magenta+black), 255-QMIN(255,yellow+black), alpha);
					p++;
				}
			}
			f.close();
		}
	}
	else
	{
		image.load(app->PrefsPfad+"/sc.png");
		image = image.convertDepth(32);
		if ((AliasTr->isChecked()) && (app->HavePngAlpha == 0))
		{
			int wi = image.width();
			int hi = image.height();
			for( int yi=0; yi < hi; ++yi )
			{
				QRgb *s = (QRgb*)(image.scanLine( yi ));
				for(int xi=0; xi < wi; ++xi )
				{
					if((*s) == 0xffffffff)
						(*s) &= 0x00ffffff;
					s++;
				}
			}
		}
	}
	image.setAlphaBuffer(true);
	if (AliasTr->isChecked())
	{
		Bild = QPixmap(image.width(), image.height());
		QPainter p;
		QBrush b(QColor(205,205,205), loadIcon("testfill.png"));
		p.begin(&Bild);
		p.fillRect(0, 0, image.width(), image.height(), b);
		p.drawImage(0, 0, image);
		p.end();
	}
	else
		Bild.convertFromImage(image);
	qApp->setOverrideCursor(QCursor(arrowCursor), true);
	return Bild;
}

