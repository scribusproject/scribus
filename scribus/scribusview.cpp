/***************************************************************************
                          scribusview.cpp  -  description
                             -------------------
    begin                : Fre Apr  6 21:47:55 CEST 2001
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

#include "scribusview.h"
#include "scribusview.moc"
#include "config.h"
#include "pageback.h"
#include <qcolor.h>
#include <qfont.h>
#include <qfontmetrics.h>
#include <qpixmap.h>
#include <qpointarray.h>
#include <qstringlist.h>
#include <qimage.h>
#include <qcstring.h>

extern void Level2Layer(ScribusDoc *doc, struct Layer *ll, int Level);
extern float Cwidth(ScribusDoc *doc, QString name, QString ch, int Siz, QString ch2 = " ");

ScribusView::ScribusView(QWidget *parent, ScribusDoc *doc, preV *prefs)
 						: QScrollView(parent, "s", WRepaintNoErase | WNorthWestGravity)
{
	Ready = false;
	Doc = doc;
	Doc->PageC = 0;
  Prefs = prefs;
	setHScrollBarMode(QScrollView::AlwaysOn);
	setVScrollBarMode(QScrollView::AlwaysOn);
	setMargins(25, 25, 0, 0);
	setResizePolicy(Manual);
	enableClipper(true);
  viewport()->setBackgroundMode(PaletteBackground);
	QFont fo = QFont(font());
	fo.setPointSize(10);
  LE = new MSpinBox( this, 2 );
	LE->setFont(fo);
  LE->setSuffix( tr( " %" ) );
	LE->setMaxValue( 320000 );
	LE->setMinValue( 1000 );
	LE->setValue( 10000 );
	LE->setDecimals( 1000 );
	LE->setFocusPolicy(QWidget::ClickFocus);
	SB1 = new QPushButton(this);
	SB1->setPixmap(loadIcon("Klein.xpm"));
	SB1->setFocusPolicy(QWidget::NoFocus);
	SB2 = new QPushButton(this);
	SB2->setFocusPolicy(QWidget::NoFocus);
	SB2->setPixmap(loadIcon("Gross.xpm"));
	LA = new QPushButton(this);
	LA->setFont(fo);
	Seitmen = new QPopupMenu(this);
	Seitmen->setFont(fo);
	LA->setText(tr("Page")+" 1");
	LA->setPopup(Seitmen);
	LA->setFocusPolicy(QWidget::NoFocus);
	LY = new QPushButton(this);
	LY->setFont(fo);
	Laymen = new QPopupMenu(this);
	Laymen->setFont(fo);
	LY->setText(tr("Layer")+" 0");
	LY->setPopup(Laymen);
	LY->setFocusPolicy(QWidget::NoFocus);
	HR = new Hruler(this, Doc);
	VR = new Vruler(this, Doc);
	UN = new QToolButton(this);
	Unitmen = new QPopupMenu(this);
	Unitmen->insertItem("pt");
	Unitmen->insertItem("mm");
	Unitmen->insertItem("in");
	Unitmen->insertItem("p");
	UN->setPopup(Unitmen);
	UN->setFocusPolicy(QWidget::NoFocus);
	UN->setPopupDelay(10);
	switch (doc->Einheit)
		{
		case 0:
			UN->setText("pt");
			break;
		case 1:
			UN->setText("mm");
			break;
		case 2:
			UN->setText("in");
			break;
		case 3:
			UN->setText("p");
			break;
		}
	Pages.clear();
	MasterPages.clear();
	DocPages.clear();
	Ready = true;
	connect(SB1, SIGNAL(clicked()), this, SLOT(slotZoomOut()));
	connect(SB2, SIGNAL(clicked()), this, SLOT(slotZoomIn()));
	connect(LE, SIGNAL(valueChanged(int)), this, SLOT(Zval()));
	connect(Seitmen, SIGNAL(activated(int)), this, SLOT(GotoPa(int)));
	connect(Laymen, SIGNAL(activated(int)), this, SLOT(GotoLa(int)));
	connect(Unitmen, SIGNAL(activated(int)), this, SLOT(ChgUnit(int)));
	connect(this, SIGNAL(contentsMoving(int, int)), this, SLOT(setRulerPos(int, int)));
}

/** Waagrechter Scrollbalken */
void ScribusView::setHBarGeometry(QScrollBar &bar, int x, int y, int w, int h)
{
	bar.setGeometry(x+270, y, w-270, h);
	if (Ready)
		{
		QFontMetrics fom(LE->font());
		LE->setGeometry(x, y, 60, h);
		if (fom.height() > LE->ed->height())
			{
			QFont ff = LE->font();
			do
				{
				int si = LE->font().pointSize();
				ff.setPointSize(si-1);
				LE->setFont(ff);
				fom = QFontMetrics(LE->font());
				}
			while (fom.height() > LE->ed->height());
			LA->setFont(ff);
			LY->setFont(ff);
			}
		QRect forec = fom.boundingRect("3200.00 %");
		int sadj = forec.width() - LE->ed->width();
		LE->setGeometry(x, y, 60+sadj, h);
		SB1->setGeometry(x+60+sadj, y, 15, h);
		SB2->setGeometry(x+75+sadj, y, 15, h);
		LA->setGeometry(x+90+sadj, y, 70, h);
		LY->setGeometry(x+160+sadj, y, 110, h);
		HR->setGeometry(25, 1, w-24, 25);
		}
}

/** Senkrechter Scrollbalken */
void ScribusView::setVBarGeometry(QScrollBar &bar, int x, int y, int w, int h)
{
	bar.setGeometry(x, y, w, h);
	if (Ready)
		{
		VR->setGeometry(1, 25, 25, h-24);
		UN->setGeometry(1, 1, 25, 25);
		}
}

void ScribusView::setRulerPos(int x, int y)
{
	HR->offs = x-static_cast<int>(10*Doc->Scale)-2;
	HR->repX = false;
	HR->repaint();
	VR->offs = y-static_cast<int>(10*Doc->Scale)-2;
	VR->repaint();
}

void ScribusView::Zval()
{
	Doc->Scale = static_cast<float>(LE->value()) / 10000.0 * Prefs->DisScale;
	slotDoZoom();
	setFocus();
}

/** Fügt eine Seite hinzu */
Page* ScribusView::addPage(int nr)
{
	int z;
	float s = Doc->Scale;
	QWidget* feh = new PageBack(viewport());
	feh->resize(static_cast<int>((Doc->PageB+5)*s), static_cast<int>((Doc->PageH+5)*s));
	Page* fe = new Page(feh, 0, 0, static_cast<int>(Doc->PageB*s), static_cast<int>(Doc->PageH*s), Doc, this);
	Doc->ActPage = fe;
	fe->Margins.Top = Doc->PageM.Top;
	fe->Margins.Bottom = Doc->PageM.Bottom;
	addChild(feh, static_cast<int>(10*s), static_cast<int>(Doc->PageC*((Doc->PageH+30)*s)+10*s));
	feh->show();
	Doc->PageC++;
	Pages.insert(nr, fe);
	reformPages();
	if ((Doc->PageAT) && (!Doc->loading))
		{
		if (Doc->PageSp > 1)
			{
			int a;
			float bsp = (Doc->PageB-fe->Margins.Right-fe->Margins.Left-(Doc->PageSp-1)*Doc->PageSpa)/Doc->PageSp;
			float anf = fe->Margins.Left;
			for (a = 0; a < Doc->PageSp; a++)
				{
				z = fe->PaintText(anf,
											fe->Margins.Top,
											bsp,
											Doc->PageH-fe->Margins.Bottom-fe->Margins.Top,
											1, Doc->Dpen);
				anf += bsp + Doc->PageSpa;
				fe->Items.at(z)->isAutoText = true;
				fe->Items.at(z)->BackBox = Doc->LastAuto;
				if (Doc->LastAuto != 0)
					Doc->LastAuto->NextBox = fe->Items.at(z);
				else
					Doc->FirstAuto = fe->Items.at(z);
				Doc->LastAuto = fe->Items.at(z);
				fe->SetRectFrame(fe->Items.at(z));
				}
			}
		else
			{
			z = fe->PaintText(fe->Margins.Left,
										fe->Margins.Top,
										Doc->PageB-fe->Margins.Right-fe->Margins.Left,
										Doc->PageH-fe->Margins.Bottom-fe->Margins.Top,
										1, Doc->Dpen);
			fe->Items.at(z)->isAutoText = true;
			fe->Items.at(z)->BackBox = Doc->LastAuto;
			if (Doc->LastAuto != 0)
				Doc->LastAuto->NextBox = fe->Items.at(z);
			else
				Doc->FirstAuto = fe->Items.at(z);
			Doc->LastAuto = fe->Items.at(z);
			fe->SetRectFrame(fe->Items.at(z));
			}
		Doc->FirstAuto->Dirty = true;
		}
	LA->setText(tr("Page")+" "+QString::number(nr+1));
	fe->setMouseTracking(true);
	connect(fe, SIGNAL(Hrule(int)), HR, SLOT(Draw(int)));
	connect(fe, SIGNAL(Vrule(int)), VR, SLOT(Draw(int)));
	connect(fe, SIGNAL(PgCh(int)), this, SLOT(setMenTxt(int)));
	connect(fe, SIGNAL(ZoomIn()), this, SLOT(slotZoomIn2()));
	connect(fe, SIGNAL(ZoomOut()), this, SLOT(slotZoomOut2()));
	connect(fe, SIGNAL(ZoomAbs()), this, SLOT(slotDoZoom()));
	connect(fe, SIGNAL(AbsPosi(int, int)), this, SLOT(SetCPo(int, int)));
	connect(fe, SIGNAL(AbsPosi2(int, int)), this, SLOT(SetCCPo(int, int)));
	return fe;	
}

/** Löscht eine Seite */
void ScribusView::delPage(int Nr)
{
	if (Doc->PageC == 1)
		{ return; }
	if (Doc->PageC < Nr-1)
		{ return; }
	removeChild(Pages.at(Nr)->parentWidget());
	delete Pages.at(Nr)->parentWidget();
	Pages.remove(Nr);
	Doc->PageC -= 1;
	Doc->ActPage = Pages.at(0);
	LA->setText(tr("Page")+" 1");
}

void ScribusView::movePage(int from, int to, int ziel, int art)
{
	QPtrList<Page> Buf;
	int a;
	uint b;
	int zz = ziel;
	Buf.clear();
	for (a = from; a < to; a++)
		{
		Buf.append(Pages.at(from));
		Pages.remove(from);
		if (a <= zz)
			zz--;			
		}
	switch (art)
		{
		case 0:
			for (b = 0; b < Buf.count(); b++)
				{
				Pages.insert(zz, Buf.at(b));
				zz++;
				}
			break;
		case 1:
			for (b = 0; b < Buf.count(); b++)
				{
				zz++;
				Pages.insert(zz, Buf.at(b));
				}
			break;
		case 2:
			for (b = 0; b < Buf.count(); b++)
				{
				Pages.append(Buf.at(b));
				}
			break;
		}
	reformPages();
}

void ScribusView::reformPages()
{	
	uint a;
	Page* Seite;
	QWidget* PSeite = Doc->ActPage->parentWidget();
 	for (a=0; a < Pages.count(); a++)
 		{
 		Seite = Pages.at(a);
		Seite->PageNr = a;
		PSeite = Seite->parentWidget();
		if (Doc->PageFP)
			{
			if (Doc->MasterP)
				{
				if (Seite->LeftPg)
 					{
 					Seite->Margins.Left = Doc->PageM.Right;
 					Seite->Margins.Right = Doc->PageM.Left;
 					}
 				else
 					{
 					Seite->Margins.Right = Doc->PageM.Right;
 					Seite->Margins.Left = Doc->PageM.Left;
 					}
				}
			else
				{	
 				if (a % 2 == 0)
 					{
 					if (Doc->FirstPageLeft)
 						{
 						Seite->Margins.Left = Doc->PageM.Right;
 						Seite->Margins.Right = Doc->PageM.Left;
 						}
 					else
 						{
 						Seite->Margins.Right = Doc->PageM.Right;
 						Seite->Margins.Left = Doc->PageM.Left;
 						}
 					}
 				else
 					{
 					if (Doc->FirstPageLeft)
 						{
 						Seite->Margins.Right = Doc->PageM.Right;
 						Seite->Margins.Left = Doc->PageM.Left;
 						}
 					else
 						{
 						Seite->Margins.Left = Doc->PageM.Right;
 						Seite->Margins.Right = Doc->PageM.Left;
 						}
 					}
 				}
 			}
		else
			{
			Seite->Margins.Right = Doc->PageM.Right;
			Seite->Margins.Left = Doc->PageM.Left;
			}
		Seite->Margins.Top = Doc->PageM.Top;
		Seite->Margins.Bottom = Doc->PageM.Bottom;
		if (Doc->MasterP)
			addChild(PSeite, static_cast<int>(10*Doc->Scale), static_cast<int>((Doc->PageC-1)*(PSeite->height()+25*Doc->Scale)+10*Doc->Scale));
		else
			{
			if ((Doc->PageFP) && (Doc->PagesSbS))
				{
 				if (a % 2 == 0)
 					{
 					if (Doc->FirstPageLeft)
						addChild(PSeite, static_cast<int>(10*Doc->Scale),
				        	 	static_cast<int>(a/2 * (PSeite->height()+25*Doc->Scale)+10*Doc->Scale));
 					else
						addChild(PSeite, static_cast<int>(PSeite->width()+35*Doc->Scale),
				        	 	static_cast<int>((a+1)/2 * (PSeite->height()+25*Doc->Scale)+10*Doc->Scale));
 					}
 				else
 					{
 					if (Doc->FirstPageLeft)
						addChild(PSeite, static_cast<int>(PSeite->width()+35*Doc->Scale),
									 	static_cast<int>(a/2 * (PSeite->height()+25*Doc->Scale)+10*Doc->Scale));
 					else
						addChild(PSeite, static_cast<int>(10*Doc->Scale),
									 	static_cast<int>((a+1)/2 * (PSeite->height()+25*Doc->Scale)+10*Doc->Scale));
 					}
				}
			else
				addChild(PSeite, static_cast<int>(10*Doc->Scale), static_cast<int>(a*(PSeite->height()+25*Doc->Scale)+10*Doc->Scale));
			}
		}
	PSeite = Doc->ActPage->parentWidget();
	if ((Doc->PageFP) && (Doc->PagesSbS))
		{
		if (Doc->FirstPageLeft)
			resizeContents(static_cast<int>(PSeite->width()*2+60*Doc->Scale),
							 			static_cast<int>(((Doc->PageC-1)/2 + 1) * (PSeite->height()+25*Doc->Scale)+30));
		else
			resizeContents(static_cast<int>(PSeite->width()*2+60*Doc->Scale),
										 static_cast<int>((Doc->PageC/2 + 1) * (PSeite->height()+25*Doc->Scale)+30));
		setContentsPos(childX(Doc->ActPage->parentWidget())-static_cast<int>(10*Doc->Scale),
									 childY(Doc->ActPage->parentWidget())-static_cast<int>(10*Doc->Scale));
		}
	else
		{
		resizeContents(static_cast<int>(PSeite->width()+30*Doc->Scale), static_cast<int>(Doc->PageC * (PSeite->height()+25*Doc->Scale)+30));
		setContentsPos(0, childY(Doc->ActPage->parentWidget())-static_cast<int>(10*Doc->Scale));
		}
	setRulerPos(contentsX(), contentsY());
 	PaMenu();
	setMenTxt(Doc->ActPage->PageNr);
}	

void ScribusView::setMenTxt(int Seite)
{
	LA->setText(tr("Page")+" "+QString::number(Seite+1));
}

void ScribusView::setLayMenTxt(int l)
{
	LY->setText(Doc->Layers[l].Name);
}

/** Fuehrt die Vergroesserung/Verkleinerung aus */
void ScribusView::slotDoZoom()
{ 	
	uint a;
	Page* Seite;
	QWidget* PSeite = Doc->ActPage->parentWidget();
	if (Doc->Scale > 32)
		Doc->Scale = 32;
	if (Pages.count() != 0)
		{
		for (a=0; a < Pages.count(); a++)
			{
			Seite = Pages.at(a);
			PSeite = Seite->parentWidget();
			Seite->resize(static_cast<int>(Doc->PageB*Doc->Scale), static_cast<int>(Doc->PageH*Doc->Scale));
			PSeite->resize(static_cast<int>((Doc->PageB+5)*Doc->Scale), static_cast<int>((Doc->PageH+5)*Doc->Scale));
			if (Doc->MasterP)
				addChild(PSeite, static_cast<int>(10*Doc->Scale), static_cast<int>((Doc->PageC-1)*(PSeite->height()+25*Doc->Scale)+10*Doc->Scale));
			else
				{
				if ((Doc->PageFP) && (Doc->PagesSbS))
					{
 					if (a % 2 == 0)
 						{
 						if (Doc->FirstPageLeft)
							addChild(PSeite, static_cast<int>(10*Doc->Scale),
				        	 		static_cast<int>(a/2 * (PSeite->height()+25*Doc->Scale)+10*Doc->Scale));
 						else
							addChild(PSeite, static_cast<int>(PSeite->width()+35*Doc->Scale),
				        	 		static_cast<int>((a+1)/2 * (PSeite->height()+25*Doc->Scale)+10*Doc->Scale));
 						}
 					else
 						{
 						if (Doc->FirstPageLeft)
							addChild(PSeite, static_cast<int>(PSeite->width()+35*Doc->Scale),
									 		static_cast<int>(a/2 * (PSeite->height()+25*Doc->Scale)+10*Doc->Scale));
 						else
							addChild(PSeite, static_cast<int>(10*Doc->Scale),
									 		static_cast<int>((a+1)/2 * (PSeite->height()+25*Doc->Scale)+10*Doc->Scale));
 						}
					}
				else
					addChild(PSeite, static_cast<int>(10*Doc->Scale), static_cast<int>(a*(PSeite->height()+25*Doc->Scale)+10*Doc->Scale));
				}
			}
		if ((Doc->PageFP) && (Doc->PagesSbS))
			{
			if (Doc->FirstPageLeft)
					resizeContents(static_cast<int>(PSeite->width()*2+60*Doc->Scale),
									 			static_cast<int>(((Doc->PageC-1)/2 + 1) * (PSeite->height()+25*Doc->Scale)+30));
			else
				resizeContents(static_cast<int>(PSeite->width()*2+60*Doc->Scale),
									 		static_cast<int>((Doc->PageC/2 + 1) * (PSeite->height()+25*Doc->Scale)+30));
			setContentsPos(childX(Doc->ActPage->parentWidget())-static_cast<int>(10*Doc->Scale),
									 	childY(Doc->ActPage->parentWidget())-static_cast<int>(10*Doc->Scale));
			}
		else
			{
			resizeContents(static_cast<int>(PSeite->width()+30*Doc->Scale), static_cast<int>(Doc->PageC * (PSeite->height()+25*Doc->Scale)+30));
			setContentsPos(0, childY(Doc->ActPage->parentWidget())-static_cast<int>(10*Doc->Scale));
			}
		disconnect(LE, SIGNAL(valueChanged(int)), this, SLOT(Zval()));
		LE->setValue(qRound(Doc->Scale/Prefs->DisScale*10000));
		connect(LE, SIGNAL(valueChanged(int)), this, SLOT(Zval()));
		setRulerPos(contentsX(), contentsY());
		if (Doc->ActPage->SelItem.count() != 0)
			{
			PageItem *b = Doc->ActPage->SelItem.at(0);
			SetCCPo(static_cast<int>(b->Xpos + b->Width/2), static_cast<int>(b->Ypos + b->Height/2));
			}
		}
}

void ScribusView::SetCCPo(int x, int y)
{
	center(static_cast<int>(childX(Doc->ActPage->parentWidget())+x*Doc->Scale), static_cast<int>(childY(Doc->ActPage->parentWidget())+y*Doc->Scale));
	setRulerPos(contentsX(), contentsY());	
}

void ScribusView::SetCPo(int x, int y)
{
	setContentsPos(static_cast<int>(childX(Doc->ActPage->parentWidget())+x*Doc->Scale), static_cast<int>(childY(Doc->ActPage->parentWidget())+y*Doc->Scale));
	setRulerPos(contentsX(), contentsY());	
}

void ScribusView::PaMenu()
{
	uint a;
	Seitmen->clear();
	if (Pages.count() != 0)
		{
		for (a=0; a < Pages.count(); a++)
			{
			Seitmen->insertItem(tr("Page")+" "+QString::number(a+1));
			}
		}
}

void ScribusView::LaMenu()
{
	uint a;
	Laymen->clear();
	if (Doc->Layers.count() != 0)
		{
		for (a=0; a < Doc->Layers.count(); a++)
			{
			Laymen->insertItem(Doc->Layers[a].Name);
			}
		}
}

void ScribusView::GotoLa(int l)
{
	int d = Laymen->indexOf(l);
	Doc->ActiveLayer = d;
	LY->setText(Doc->Layers[d].Name);
	emit changeLA(d);
}

void ScribusView::GotoPa(int Seite)
{
	int d = Seitmen->indexOf(Seite);
	GotoPage(d);
}

void ScribusView::ChgUnit(int art)
{
	int d = Unitmen->indexOf(art);
	emit changeUN(d);
}

void ScribusView::GotoPage(int Seite)
{
	Doc->ActPage = Pages.at(Seite);
	setContentsPos(static_cast<int>(childX(Doc->ActPage->parentWidget())-10*Doc->Scale), static_cast<int>(childY(Doc->ActPage->parentWidget())-10*Doc->Scale));
	LA->setText(tr("Page")+" "+QString::number(Seite+1));
}

/** Vergrößert die Ansicht */
void ScribusView::slotZoomIn()
{
	Doc->Scale *= 2;
	if (Doc->Scale > 32)
		Doc->Scale = 32;
	slotDoZoom();
}

/** Verkleinert die Ansicht */
void ScribusView::slotZoomOut()
{
	Doc->Scale /= 2;
	slotDoZoom();
}

/** Vergrößert die Ansicht */
void ScribusView::slotZoomIn2()
{
	Doc->Scale += static_cast<float>(Doc->MagStep)/100;
	if (Doc->Scale > static_cast<float>(Doc->MagMax)/100)
		Doc->Scale = static_cast<float>(Doc->MagMax)/100;
	slotDoZoom();
}

/** Verkleinert die Ansicht */
void ScribusView::slotZoomOut2()
{
	Doc->Scale -= static_cast<float>(Doc->MagStep)/100;
	if (Doc->Scale < static_cast<float>(Doc->MagMin)/100)
		Doc->Scale = static_cast<float>(Doc->MagMin)/100;
	slotDoZoom();
}

void ScribusView::DrawNew()
{ 	
	uint a;
	Page *b = Doc->ActPage;
	if (Pages.count() != 0)
		{
		for (a=0; a < Pages.count(); a++)
			{
			Doc->ActPage = Pages.at(a);
			Pages.at(a)->setEraseColor(Doc->papColor);
			Pages.at(a)->update();
			}
		}
	HR->repX = false;
	HR->repaint();
	VR->repaint();
	Doc->ActPage = b;
	disconnect(LE, SIGNAL(valueChanged(int)), this, SLOT(Zval()));
	LE->setValue(qRound(Doc->Scale/Prefs->DisScale*10000));
	connect(LE, SIGNAL(valueChanged(int)), this, SLOT(Zval()));
}

int ScribusView::CountElements()
{
	int cc = 0;
	for (uint a=0; a < Pages.count(); a++)
		cc += Pages.at(a)->Items.count();
	return cc;
}

void ScribusView::RecalcPictures(ProfilesL *Pr, QProgressBar *dia)
{ 	
	uint a, i;
	Page *b = Doc->ActPage;
	PageItem* it;
	int counter;
	if (dia != NULL)
		counter = dia->progress();
	else
		counter = 0;
	if (Pages.count() != 0)
		{
		for (a=0; a < Pages.count(); a++)
			{
			Doc->ActPage = Pages.at(a);
			for (i=0; i < Pages.at(a)->Items.count(); i++)
				{
				it = Pages.at(a)->Items.at(i);
/*				if (it->GrType != 0)
					Pages.at(a)->UpdateGradient(it);   */
				if ((it->PType == 2) && (it->PicAvail))
					{
					if (Pr->contains(it->IProfile))
						Pages.at(a)->LoadPict(it->Pfile, i);
					else
						{
						it->IProfile = Doc->CMSSettings.DefaultInputProfile;
						Pages.at(a)->LoadPict(it->Pfile, i);
						}
					it->Sizing = true;
					Pages.at(a)->AdjustPreview(it);
					it->Sizing = false;
					}
				counter++;
				if (dia != NULL)
					dia->setProgress(counter);
				}
			}
		}
	Doc->ActPage = b;
}

void ScribusView::ShowTemplate(int nr)
{
	for (uint a=0; a<Pages.count(); a++)
		{
		Pages.at(a)->parentWidget()->hide();
		}
	if (!Doc->MasterP)
		{
		DocPages = Pages;
		Pages = MasterPages;
		}
	Doc->PageC = 1;
	Doc->MasterP = true;
	Doc->ActPage = Pages.at(nr);
	Pages.at(nr)->parentWidget()->show();
	LA->setEnabled(false);
	reformPages();
	slotDoZoom();
}

void ScribusView::HideTemplate()
{
	for (uint a=0; a<Pages.count(); a++)
		{
		Pages.at(a)->Deselect(true);
		Pages.at(a)->parentWidget()->hide();
		}
	MasterPages = Pages;
	Doc->PageC = DocPages.count();
	Pages = DocPages;
	Doc->MasterP = false;
	Doc->ActPage = Pages.at(0);
	for (uint a=0; a<Pages.count(); a++)
		{
		Pages.at(a)->parentWidget()->show();
		}
	LA->setEnabled(true);
	reformPages();
	GotoPage(0);
	slotDoZoom();
}

QPixmap ScribusView::MPageToPixmap(QString name, int maxGr)
{
	QPixmap pm = QPixmap(static_cast<int>(Doc->PageB), static_cast<int>(Doc->PageH));
	ScPainter *painter = new ScPainter(&pm, pm.width(), pm.height());
	float sca = Doc->Scale;
	bool frs = Doc->ShFrames;
	int Lnr;
	struct Layer ll;
	ll.Drucken = false;
	ll.LNr = 0;
	Doc->ShFrames = false;
	Doc->Scale = 1;
	PageItem *b;
	PageItem *bb;
	PageItem *nb;
	Lnr = 0;
	Page* Mp = MasterPages.at(MasterNames[name]);
	QWidget* Opa;
	Page* Opa2;
	if (Mp->Items.count() != 0)
		{
		Lnr = 0;
		for (uint la = 0; la < Doc->Layers.count(); la++)
			{
			Level2Layer(Doc, &ll, Lnr);
			if (ll.Drucken)
				{
				for (uint a = 0; a < Mp->Items.count(); a++)
					{
					b = Mp->Items.at(a);
					if (b->LayerNr != ll.LNr)
						continue;
					Opa = b->Parent;
					Opa2 = b->OwnPage;
					b->Parent = Mp;
					b->OwnPage = Mp;
					nb = b->NextBox;
					bb = b->BackBox;
					b->NextBox = 0;
					b->BackBox = 0;
					b->DrawObj(painter, QRect(0, 0, static_cast<int>(Doc->PageB), static_cast<int>(Doc->PageH)));
					b->NextBox = nb;
					b->BackBox = bb;
					b->Parent = Opa;
					b->OwnPage = Opa2;
					}
				}
			Lnr++;
			}
		}
	painter->end();
	Doc->ShFrames = frs;
	Doc->Scale = sca;
	QImage im2;
	QImage im = pm.convertToImage();
	float sx = im.width() / static_cast<float>(maxGr);
	float sy = im.height() / static_cast<float>(maxGr);
	if (sy < sx)
		im2 = im.smoothScale(static_cast<int>(im.width() / sx), static_cast<int>(im.height() / sx));
	else
		im2 = im.smoothScale(static_cast<int>(im.width() / sy), static_cast<int>(im.height() / sy));
	pm.convertFromImage(im2);
	QPainter p;
	p.begin(&pm);
	p.setBrush(NoBrush);
	p.setPen(black);
	p.drawRect(0, 0, pm.width(), pm.height());
	p.end();
	im2.detach();
	delete painter;
	return pm;
}

QPixmap ScribusView::PageToPixmap(int Nr, int maxGr)
{
	QPixmap pm = QPixmap(static_cast<int>(Doc->PageB), static_cast<int>(Doc->PageH));
	ScPainter *painter = new ScPainter(&pm, pm.width(), pm.height());
	float sca = Doc->Scale;
	bool frs = Doc->ShFrames;
	int Lnr;
	struct Layer ll;
	ll.Drucken = false;
	ll.LNr = 0;
	Doc->ShFrames = false;
	Doc->Scale = 1;
	PageItem *b;
	PageItem *bb;
	PageItem *nb;
	Lnr = 0;
	if (Pages.at(Nr)->MPageNam != "")
		{
		Page* Mp = MasterPages.at(MasterNames[Pages.at(Nr)->MPageNam]);
		QWidget* Opa;
		Page* Opa2;
		if (Mp->Items.count() != 0)
			{
			Lnr = 0;
			for (uint la = 0; la < Doc->Layers.count(); la++)
				{
				Level2Layer(Doc, &ll, Lnr);
				if (ll.Drucken)
					{
					for (uint a = 0; a < Mp->Items.count(); a++)
						{
						b = Mp->Items.at(a);
						if (b->LayerNr != ll.LNr)
							continue;
						Opa = b->Parent;
						Opa2 = b->OwnPage;
						b->Parent = Pages.at(Nr);
						b->OwnPage = Pages.at(Nr);
						nb = b->NextBox;
						bb = b->BackBox;
						b->NextBox = 0;
						b->BackBox = 0;
						b->DrawObj(painter, QRect(0, 0, static_cast<int>(Doc->PageB), static_cast<int>(Doc->PageH)));
						b->NextBox = nb;
						b->BackBox = bb;
						b->Parent = Opa;
						b->OwnPage = Opa2;
						}
					}
				Lnr++;
				}
			}
		}
	Lnr = 0;
	ll.Drucken = false;
	ll.LNr = 0;
	for (uint la2 = 0; la2 < Doc->Layers.count(); la2++)
		{
		Level2Layer(Doc, &ll, Lnr);
		if (ll.Drucken)
			{
			for (uint a = 0; a < Pages.at(Nr)->Items.count(); a++)
				{
				b = Pages.at(Nr)->Items.at(a);
				if (b->LayerNr != ll.LNr)
					continue;
				if (b->isPrintable)
					{
					if ((b->isAnnotation) && (b->AnType != 11))
						continue;
					nb = b->NextBox;
					bb = b->BackBox;
					b->NextBox = 0;
					b->BackBox = 0;
					b->DrawObj(painter, QRect(0, 0, static_cast<int>(Doc->PageB), static_cast<int>(Doc->PageH)));
					b->NextBox = nb;
					b->BackBox = bb;
					}
				}
			}
		Lnr++;
		}
	painter->end();
	Doc->ShFrames = frs;
	Doc->Scale = sca;
	QImage im2;
	QImage im = pm.convertToImage();
	float sx = im.width() / static_cast<float>(maxGr);
	float sy = im.height() / static_cast<float>(maxGr);
	if (sy < sx)
		im2 = im.smoothScale(static_cast<int>(im.width() / sx), static_cast<int>(im.height() / sx));
	else
		im2 = im.smoothScale(static_cast<int>(im.width() / sy), static_cast<int>(im.height() / sy));
	pm.convertFromImage(im2);
	QPainter p;
	p.begin(&pm);
	p.setBrush(NoBrush);
	p.setPen(black);
	p.drawRect(0, 0, pm.width(), pm.height());
	p.end();
	im2.detach();
	delete painter;
	return pm;
}

void ScribusView::CreatePS(PSLib *p, uint von, uint bis, int step, bool sep, QString SepNam, bool farb, bool Hm, bool Vm, bool Ic)
{
	uint a;
	int sepac;
	float wideR;
	bool multiPath = false;
	p->PS_set_Info("Author", Doc->DocAutor);
	p->PS_set_Info("Title", Doc->DocTitel);
	if (!farb)
		p->PS_setGray();
	p->PS_begin_doc(Doc->PageOri, Doc->PageB, Doc->PageH, abs(bis-von));
	a = von;
	sepac = 0;
	for (uint ap = 0; ap < MasterPages.count(); ++ap)
		{
		if (MasterPages.at(ap)->Items.count() != 0)
			{
			if ((!sep) && (farb))
				{
				for (uint api = 0; api < MasterPages.at(ap)->Items.count(); ++api)
					{
					PageItem *it = MasterPages.at(ap)->Items.at(api);
					if ((it->PType == 2) && (it->PicAvail) && (it->Pfile != "") && (it->isPrintable))
						p->PS_ImageData(it->InvPict, it->Pfile, it->AnName, it->IProfile, it->UseEmbedded, Ic);
					}
				} 
			p->PS_TemplateStart(MasterPages.at(ap)->PageNam, Doc->PageB, Doc->PageH);
			ProcessPage(p, MasterPages.at(ap), ap+1, sep, farb, Ic);
			p->PS_TemplateEnd();
			}
		}
	while (a != bis)
		{
		p->PS_begin_page(Doc->PageB, Doc->PageH, &Pages.at(a)->Margins);
		if (Hm)
			{
			p->PS_translate(Doc->PageB, 0);
			p->PS_scale(-1, 1);
			}
		if (Vm)
			{
			p->PS_translate(0, Doc->PageH);
			p->PS_scale(1, -1);
			}
		if (sep)
			{
			if (SepNam == "Black")
				p->PS_plate(0);
			if (SepNam == "Cyan")
				p->PS_plate(1);
			if (SepNam == "Magenta")
				p->PS_plate(2);
			if (SepNam == "Yellow")
				p->PS_plate(3);
			if (SepNam == tr("All"))
				p->PS_plate(sepac);
			}
		if (Pages.at(a)->MPageNam != "")
			{
			int h, s, v, k, tsz;
			QCString chxc;
			QString chx;
			struct Pti *hl;
			int Lnr = 0;
			struct Layer ll;
			ll.Drucken = false;
			ll.LNr = 0;
			Page* mPage = MasterPages.at(MasterNames[Pages.at(a)->MPageNam]);
			if (mPage->Items.count() != 0)
				{
				p->PS_UseTemplate(Pages.at(a)->MPageNam);
				for (uint lam = 0; lam < Doc->Layers.count(); ++lam)
					{
					Level2Layer(Doc, &ll, Lnr);
					if (ll.Drucken)
						{
						for (uint am = 0; am < mPage->Items.count(); ++am)
							{
							PageItem *ite = mPage->Items.at(am);
							if (ite->Segments.count() != 0)
								multiPath = true;
							else
								multiPath = false;
							if ((ite->LayerNr != ll.LNr) || (!ite->isPrintable))
								continue;
							if ((ite->PType == 2) && ((sep) || (!farb)))
								{
								p->PS_save();
								p->PS_translate(ite->Xpos, Doc->PageH - ite->Ypos);
								if (ite->Rot != 0)
									p->PS_rotate(-ite->Rot);
								if (ite->Pcolor != "None")
									{
									SetFarbe(ite->Pcolor, ite->Shade, &h, &s, &v, &k);
									p->PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
									SetClipPath(p, &ite->PoLine);
									p->PS_closepath();
									p->PS_fill(multiPath);
									}
								else
									p->PS_setcmykcolor_dummy();
								SetClipPath(p, &ite->PoLine);
								p->PS_closepath();
								p->PS_clip(multiPath);
								p->PS_save();
								if ((ite->flippedH % 2) != 0)
									{
									p->PS_translate(ite->Width, 0);
									p->PS_scale(-1, 1);
									}
								if ((ite->flippedV % 2) != 0)
									{
									p->PS_translate(0, -ite->Height);
									p->PS_scale(1, -1);
									}
								if ((ite->PicAvail) && (ite->Pfile != ""))
									{
									p->PS_translate(0, -ite->BBoxH*ite->LocalScY);
									p->PS_image(ite->InvPict, -ite->BBoxX+ite->LocalX, -ite->LocalY, ite->Pfile, ite->LocalScX, ite->LocalScY, ite->IProfile, ite->UseEmbedded, Ic);
									}
								p->PS_restore();
								if ((ite->Pcolor2 != "None") || (ite->NamedLStyle != ""))
									{
									if (ite->NamedLStyle == "")
										{
										SetClipPath(p, &ite->PoLine);
										p->PS_closepath();
										p->PS_stroke();
										}
									else
										{
										multiLine ml = Doc->MLineStyles[ite->NamedLStyle];
										for (int it = ml.size()-1; it > -1; it--)
											{
											SetFarbe(ml[it].Color, ml[it].Shade, &h, &s, &v, &k);
											p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
											p->PS_setlinewidth(ml[it].Width);
											p->PS_setdash(static_cast<PenStyle>(ml[it].Dash), static_cast<PenCapStyle>(ml[it].LineEnd), static_cast<PenJoinStyle>(ml[it].LineJoin));
											SetClipPath(p, &ite->PoLine);
											p->PS_closepath();
											p->PS_stroke();
											}
										}
									}
								p->PS_restore();
								}
							if (ite->PType == 4)
								{
								p->PS_save();
								if (ite->Pcolor != "None")
									{
									SetFarbe(ite->Pcolor, ite->Shade, &h, &s, &v, &k);
									p->PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
									}
								p->PS_translate(ite->Xpos, Doc->PageH - ite->Ypos);
								if (ite->Rot != 0) 
									p->PS_rotate(-ite->Rot);
								if ((ite->Pcolor != "None") || (ite->GrType != 0))
									{
									SetClipPath(p, &ite->PoLine);
									p->PS_closepath();
									p->PS_fill(multiPath);
									}
								if ((ite->flippedH % 2) != 0)
									{
									p->PS_translate(ite->Width, 0);
									p->PS_scale(-1, 1);
									}
								if ((ite->flippedV % 2) != 0)
									{
									p->PS_translate(0, -ite->Height);
									p->PS_scale(1, -1);
									}
								for (uint d = 0; d < ite->MaxChars; ++d)
									{
									hl = ite->Ptext.at(d);
									if ((hl->ch == QChar(13)) || (hl->ch == QChar(10))) // || (hl->ch == QChar(32)))
										continue;
									if (hl->yp == 0)
										break;
									tsz = hl->csize;
									chx = hl->ch;
									if (hl->ch == QChar(29))
										chx = " ";
									if (hl->ch == QChar(30))
										{
										if (Doc->MasterP)
											chx = "#";
										else
											{
											uint zae = 0;
											while (ite->Ptext.at(d+zae)->ch == QChar(30))
												{
												zae++;
												if (d+zae == ite->MaxChars)
													break;
												}
											QString out="%1";
											chx = out.arg(a+Doc->FirstPnum, zae).right(zae).left(1);
											}
										}
									if (hl->cstyle & 64)
										{
										if (chx.upper() != chx)
											{
											tsz = hl->csize * Doc->VKapit / 100;
											chx = chx.upper();
											}
										}
									if (hl->cstyle & 1)
										tsz = hl->csize * Doc->VHochSc / 100;
									if (hl->cstyle & 2)
										tsz = hl->csize * Doc->VHochSc / 100;
									if (((*Doc->AllFonts)[hl->cfont]->isOTF) || ((*Doc->AllFonts)[hl->cfont]->Subset))
										{
										uint chr = chx[0].unicode();
										if (((*Doc->AllFonts)[hl->cfont]->CharWidth.contains(chr)) && (chr != 32))
											{
											p->PS_save();
											if (ite->Reverse)
												{
												p->PS_translate(hl->xp, (hl->yp - tsz) * -1);
												p->PS_scale(-1, 1);
												if (d < ite->MaxChars-1)
													{
													QString ctx = ite->Ptext.at(d+1)->ch;
													if (ctx == QChar(29))
														ctx = " ";
													wideR = -Cwidth(Doc, hl->cfont, chx, tsz, ctx) * (hl->cscale / 100.0);
													}
												else
													wideR = -Cwidth(Doc, hl->cfont, chx, tsz) * (hl->cscale / 100.0);
												p->PS_translate(wideR, 0);
												}
											else
												p->PS_translate(hl->xp, (hl->yp - tsz) * -1);
											if (hl->cscale != 100)
												p->PS_scale(hl->cscale / 100.0, 1);
											if (hl->ccolor != "None")
												{
												SetFarbe(hl->ccolor, hl->cshade, &h, &s, &v, &k);
												p->PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
												p->PS_showSub(chr, (*Doc->AllFonts)[hl->cfont]->RealName(), tsz, false);
												}
											p->PS_restore();
											}
										}
									else
										{
										p->PS_selectfont(hl->cfont, tsz);
										if (hl->ccolor != "None")
											{
											SetFarbe(hl->ccolor, hl->cshade, &h, &s, &v, &k);
											p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
											}
										p->PS_save();
										if (ite->Reverse)
											{
											int chs = hl->csize;
											ite->SetZeichAttr(hl, &chs, &chx);
											p->PS_translate(hl->xp, -hl->yp);
											p->PS_scale(-1, 1);
											if (d < ite->MaxChars-1)
												{
												QString ctx = ite->Ptext.at(d+1)->ch;
												if (ctx == QChar(29))
													ctx = " ";
												wideR = -Cwidth(Doc, hl->cfont, chx, chs, ctx) * (hl->cscale / 100.0);
												p->PS_translate(wideR, 0);
												}
											else
												{
												wideR = -Cwidth(Doc, hl->cfont, chx, chs) * (hl->cscale / 100.0);
												p->PS_translate(wideR, 0);
												}
											if (hl->cscale != 100)
												p->PS_scale(hl->cscale / 100.0, 1);
											p->PS_show_xyG(hl->cfont, chx, 0, 0);
											}
										else
											{
											p->PS_translate(hl->xp, -hl->yp);
											if (hl->cscale != 100)
												p->PS_scale(hl->cscale / 100.0, 1);
											p->PS_show_xyG(hl->cfont, chx, 0, 0);
											}
										p->PS_restore();
										}
									if ((hl->cstyle & 4) && (chx != QChar(13)))
										{
										uint chr = chx[0].unicode();
										if ((*Doc->AllFonts)[hl->cfont]->CharWidth.contains(chr))
											{
											FPointArray gly = (*Doc->AllFonts)[hl->cfont]->GlyphArray[chr].Outlines.copy();
											QWMatrix chma;
											chma.scale(tsz / 10.0, tsz / 10.0);
											gly.map(chma);
											chma = QWMatrix();
											chma.scale(hl->cscale / 100.0, 1);
											gly.map(chma);
											if (ite->Reverse)
												{
												chma = QWMatrix();
												chma.scale(-1, 1);
												chma.translate(wideR, 0);
												gly.map(chma);
												}
											if (hl->cstroke != "None")
												{
												p->PS_save();
												p->PS_setlinewidth(QMAX((*Doc->AllFonts)[hl->cfont]->strokeWidth / 2 * tsz, 1));
												p->PS_setdash(SolidLine, FlatCap, MiterJoin);
												p->PS_translate(hl->xp, (hl->yp - tsz) * -1);
												SetFarbe(hl->cstroke, hl->cshade2, &h, &s, &v, &k);
												p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
												SetClipPath(p, &gly);
												p->PS_closepath();
												p->PS_stroke();
												p->PS_restore();
												}
											}
										}
									if (hl->cstyle & 16)
										{
										float Ulen = Cwidth(Doc, hl->cfont, chx, hl->csize) * (hl->cscale / 100.0);
										float Upos = (*Doc->AllFonts)[hl->cfont]->strikeout_pos * tsz;
										if (hl->ccolor != "None")
											{
											p->PS_setdash(SolidLine, FlatCap, MiterJoin);
											SetFarbe(hl->ccolor, hl->cshade, &h, &s, &v, &k);
											p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
											}
										p->PS_setlinewidth((*Doc->AllFonts)[hl->cfont]->strokeWidth * tsz);
										p->PS_moveto(hl->xp, -hl->yp+Upos);
										p->PS_lineto(hl->xp+Ulen, -hl->yp+Upos);
										p->PS_stroke();
										}
									if ((hl->cstyle & 128) && (((ite->Ptext.at(QMIN(d+1, ite->Ptext.count()-1))->yp != hl->yp) && (ite->Ptext.at(QMIN(d+1, ite->Ptext.count()-1))->ch != QChar(13))) || ((ite->NextBox != 0) && (d == ite->Ptext.count()-1))))
										{
										int chs = hl->csize;
										ite->SetZeichAttr(hl, &chs, &chx);
										float wide = Cwidth(Doc, hl->cfont, chx, chs);
										chx = "-";
										uint chr = chx[0].unicode();
										if ((*Doc->AllFonts)[hl->cfont]->CharWidth.contains(chr))
											{
											FPointArray gly = (*Doc->AllFonts)[hl->cfont]->GlyphArray[chr].Outlines.copy();
											QWMatrix chma;
											chma.scale(tsz / 10.0, tsz / 10.0);
											gly.map(chma);
											chma = QWMatrix();
											chma.scale(hl->cscale / 100.0, 1);
											gly.map(chma);
											if (hl->ccolor != "None")
												{
												p->PS_save();
												p->PS_translate(hl->xp+wide, (hl->yp - tsz) * -1);
												SetFarbe(hl->ccolor, hl->cshade, &h, &s, &v, &k);
												p->PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
												SetClipPath(p, &gly);
												p->PS_closepath();
												p->PS_fill(true);
												p->PS_restore();
												}
											}
										}
									if ((hl->cstyle & 8) && (chx != QChar(13)))
										{
										int chs = hl->csize;
										ite->SetZeichAttr(hl, &chs, &chx);
										float csi = chs / 10.0;
										float wid = Cwidth(Doc, hl->cfont, chx, chs) * (hl->cscale / 100.0);
										QPixmap pgPix(static_cast<int>(wid), hl->csize);
										ScPainter *painter = new ScPainter(&pgPix, static_cast<int>(wid), hl->csize);
										QString psSt = "1 -1 scale\n";
										uint chr = chx[0].unicode();
										if ((*Doc->AllFonts)[hl->cfont]->CharWidth.contains(chr))
											{
											FPointArray gly = (*Doc->AllFonts)[hl->cfont]->GlyphArray[chr].Outlines.copy();
											float st = (*Doc->AllFonts)[hl->cfont]->underline_pos * chs;
											painter->setLineWidth(QMAX((*Doc->AllFonts)[hl->cfont]->strokeWidth * chs, 1));
											if (gly.size() < 4)
												{
												gly.resize(0);
												gly.addPoint(FPoint(0,0));
												gly.addPoint(FPoint(0,0));
												gly.addPoint(FPoint(1,0));
												gly.addPoint(FPoint(1,0));
												}
											QWMatrix chma;
											chma.scale(csi, csi);
											gly.map(chma);
											chma = QWMatrix();
											chma.scale(hl->cscale / 100.0, 1);
											gly.map(chma);
											painter->setupPolygon(&gly);
											painter->drawUnderline(FPoint(0-hl->cextra, chs-st), FPoint(wid, chs-st), true, &psSt);
											p->PS_save();
											if (ite->Reverse)
												{
												p->PS_translate(hl->xp, (hl->yp - st) * -1);
												p->PS_scale(-1, 1);
												p->PS_translate(-wid, 0);
												}
											else
												p->PS_translate(hl->xp, (hl->yp - st) * -1);
											if (hl->ccolor != "None")
												{
												SetFarbe(hl->ccolor, hl->cshade, &h, &s, &v, &k);
												p->PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
												}
											p->PS_insert(psSt+"closepath\n");
											p->PS_fill(true);
											p->PS_restore();
											painter->end();
											delete painter;
											}
										}
									}
								if ((ite->Pcolor2 != "None") || (ite->NamedLStyle != ""))
									{
									SetFarbe(ite->Pcolor2, ite->Shade2, &h, &s, &v, &k);
									p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
									p->PS_setlinewidth(ite->Pwidth);
									p->PS_setdash(ite->PLineArt, ite->PLineEnd, ite->PLineJoin);
									if (ite->NamedLStyle == "")
										{
										SetClipPath(p, &ite->PoLine);
										p->PS_closepath();
										p->PS_stroke();
										}
									else
										{
										multiLine ml = Doc->MLineStyles[ite->NamedLStyle];
										for (int it = ml.size()-1; it > -1; it--)
											{
											SetFarbe(ml[it].Color, ml[it].Shade, &h, &s, &v, &k);
											p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
											p->PS_setlinewidth(ml[it].Width);
											p->PS_setdash(static_cast<PenStyle>(ml[it].Dash), static_cast<PenCapStyle>(ml[it].LineEnd), static_cast<PenJoinStyle>(ml[it].LineJoin));
											SetClipPath(p, &ite->PoLine);
											p->PS_closepath();
											p->PS_stroke();
											}
										}
									}
								p->PS_restore();
								}
							}
						}
					Lnr++;
					}
				}
			}
		ProcessPage(p, Pages.at(a), a+1, sep, farb, Ic);
		p->PS_end_page();
		if (sep)
			{
			if (SepNam != tr("All"))
				a += step;
			else
				{
				if (sepac == 3)
					{
					a += step;
					sepac = 0;
					}
				else
					sepac++;
				}
			}
		else
			a += step;
		}
	p->PS_close();
}

void ScribusView::ProcessPage(PSLib *p, Page* a, uint PNr, bool sep, bool farb, bool ic)
{
	uint b, d;
	int h, s, v, k, tsz;
	float wideR;
	QCString chxc;
	QString chx, chglyph, tmp;
	PageItem *c;
	struct Pti *hl;
	bool multiPath = false;
	int Lnr = 0;
	struct Layer ll;
	ll.Drucken = false;
	ll.LNr = 0;
	for (uint la = 0; la < Doc->Layers.count(); ++la)
		{
		Level2Layer(Doc, &ll, Lnr);
		if (ll.Drucken)
			{
			for (b = 0; b < a->Items.count(); ++b)
				{
				c = a->Items.at(b);
				if (c->Segments.count() != 0)
					multiPath = true;
				else
					multiPath = false;
				if (c->LayerNr != ll.LNr)
					continue;
				if ((a->PageNam != "") && (c->PType == 4))
					continue;
				if ((a->PageNam != "") && (c->PType == 2) && ((sep) || (!farb)))
					continue;
				if (c->isPrintable)
					{
					p->PS_save();
					if (c->Pcolor != "None")
						{
						SetFarbe(c->Pcolor, c->Shade, &h, &s, &v, &k);
						p->PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
						}
					if (c->Pcolor2 != "None")
						{
						SetFarbe(c->Pcolor2, c->Shade2, &h, &s, &v, &k);
						p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
						}
					p->PS_setlinewidth(c->Pwidth);
					p->PS_setdash(c->PLineArt, c->PLineEnd, c->PLineJoin);
					p->PS_translate(c->Xpos, Doc->PageH - c->Ypos);
					if (c->Rot != 0)
						p->PS_rotate(-c->Rot);
					switch (c->PType)
						{
						case 2:
							if ((c->Pcolor != "None") || (c->GrType != 0))
								{
								SetClipPath(p, &c->PoLine);
								p->PS_closepath();
								if ((c->GrType != 0) && (a->PageNam == ""))
									{
									SetFarbe(c->GrColor2, c->GrShade2, &h, &s, &v, &k);
									p->PS_GradientCol1(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
									SetFarbe(c->GrColor, c->GrShade, &h, &s, &v, &k);
									p->PS_GradientCol2(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
									switch (c->GrType)
										{
										case 1:
										case 2:
										case 3:
										case 4:
											p->PS_LinGradient(c->Width, -c->Height, c->PType, c->GrType, multiPath);
											break;
										case 5:
											p->PS_RadGradient(c->Width, -c->Height, c->PType, multiPath);
											break;
										default:
											break;
										}
									}
								else
									p->PS_fill(multiPath);
								}
							p->PS_save();
							SetClipPath(p, &c->PoLine);
							p->PS_closepath();
							p->PS_clip(multiPath);
							if ((c->flippedH % 2) != 0)
								{
								p->PS_translate(c->Width, 0);
								p->PS_scale(-1, 1);
								}
							if ((c->flippedV % 2) != 0)
								{
								p->PS_translate(0, -c->Height);
								p->PS_scale(1, -1);
								}
							if ((c->PicAvail) && (c->Pfile != ""))
								{
								p->PS_translate(0, -c->BBoxH*c->LocalScY);
								if (a->PageNam != "")
									p->PS_image(c->InvPict, -c->BBoxX+c->LocalX, -c->LocalY, c->Pfile, c->LocalScX, c->LocalScY, c->IProfile, c->UseEmbedded, ic, c->AnName);
								else
									p->PS_image(c->InvPict, -c->BBoxX+c->LocalX, -c->LocalY, c->Pfile, c->LocalScX, c->LocalScY, c->IProfile, c->UseEmbedded, ic);
								}
							p->PS_restore();
							if ((c->Pcolor2 != "None") || (c->NamedLStyle != ""))
								{
								if (c->NamedLStyle == "")
									{
									SetClipPath(p, &c->PoLine);
									p->PS_closepath();
									p->PS_stroke();
									}
								else
									{
									multiLine ml = Doc->MLineStyles[c->NamedLStyle];
									for (int it = ml.size()-1; it > -1; it--)
										{
										SetFarbe(ml[it].Color, ml[it].Shade, &h, &s, &v, &k);
										p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
										p->PS_setlinewidth(ml[it].Width);
										p->PS_setdash(static_cast<PenStyle>(ml[it].Dash), static_cast<PenCapStyle>(ml[it].LineEnd), static_cast<PenJoinStyle>(ml[it].LineJoin));
										SetClipPath(p, &c->PoLine);
										p->PS_closepath();
										p->PS_stroke();
										}
									}
								}						
							break;
						case 4:
							if (c->isBookmark)
								{
								QString bm = "";
								QString cc;
								for (d = 0; d < c->Ptext.count(); ++d)
									{
									if ((c->Ptext.at(d)->ch == QChar(13)) || (c->Ptext.at(d)->ch == QChar(10)))
										break;
									bm += "\\"+cc.setNum(QMAX(c->Ptext.at(d)->ch.at(0).unicode(), 32), 8);
									}
								p->PDF_Bookmark(bm, a->PageNr+1);
								}
							if (c->isAnnotation)
								{
								QString bm = "";
								QString cc;
								for (d = 0; d < c->Ptext.count(); ++d)
									{
									bm += "\\"+cc.setNum(QMAX(c->Ptext.at(d)->ch.at(0).unicode(), 32), 8);
									}
								p->PDF_Annotation(bm, 0, 0, c->Width, -c->Height);
								break;
								}
							if ((c->Pcolor != "None") || (c->GrType != 0))
								{
								SetClipPath(p, &c->PoLine);
								p->PS_closepath();
								if ((c->GrType != 0) && (a->PageNam == ""))
									{
									SetFarbe(c->GrColor2, c->GrShade2, &h, &s, &v, &k);
									p->PS_GradientCol1(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
									SetFarbe(c->GrColor, c->GrShade, &h, &s, &v, &k);
									p->PS_GradientCol2(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
									switch (c->GrType)
										{
										case 1:
										case 2:
										case 3:
										case 4:
											p->PS_LinGradient(c->Width, -c->Height, c->PType, c->GrType, multiPath);
											break;
										case 5:
											p->PS_RadGradient(c->Width, -c->Height, c->PType, multiPath);
											break;
										default:
											break;
										}
									}
								else
									p->PS_fill(multiPath);
								}
							if ((c->flippedH % 2) != 0)
								{
								p->PS_translate(c->Width, 0);
								p->PS_scale(-1, 1);
								}
							if ((c->flippedV % 2) != 0)
								{
								p->PS_translate(0, -c->Height);
								p->PS_scale(1, -1);
								}
							for (d = 0; d < c->MaxChars; ++d)
								{
								hl = c->Ptext.at(d);
								if ((hl->ch == QChar(13)) || (hl->ch == QChar(10)))
									continue;
								if (hl->yp == 0)
									break;
								tsz = hl->csize;
								chx = hl->ch;
								if (hl->ch == QChar(29))
									chx = " ";
								if (hl->ch == QChar(30))
									{
									if (Doc->MasterP)
										chx = "#";
									else
										{
										uint zae = 0;
										while (c->Ptext.at(d+zae)->ch == QChar(30))
											{
											zae++;
											if (d+zae == c->MaxChars)
												break;
											}
										QString out="%1";
										chx = out.arg(PNr-1+Doc->FirstPnum, zae).right(zae).left(1);
										}
									}
								if (hl->cstyle & 64)
									{
									if (chx.upper() != chx)
										{
										tsz = hl->csize * Doc->VKapit / 100;
										chx = chx.upper();
										}
									}
								if (hl->cstyle & 1)
									tsz = hl->csize * Doc->VHochSc / 100;
								if (hl->cstyle & 2)
									tsz = hl->csize * Doc->VHochSc / 100;
								if (((*Doc->AllFonts)[hl->cfont]->isOTF)  || ((*Doc->AllFonts)[hl->cfont]->Subset))
									{
									uint chr = chx[0].unicode();
									if (((*Doc->AllFonts)[hl->cfont]->CharWidth.contains(chr)) && (chr != 32))
										{
										p->PS_save();
										if (c->Reverse)
											{
											p->PS_translate(hl->xp, (hl->yp - tsz) * -1);
											p->PS_scale(-1, 1);
											if (d < c->MaxChars-1)
												{
												QString ctx = c->Ptext.at(d+1)->ch;
												if (ctx == QChar(29))
													ctx = " ";
												wideR = -Cwidth(Doc, hl->cfont, chx, tsz, ctx) * (hl->cscale / 100.0);
												}
											else
												wideR = -Cwidth(Doc, hl->cfont, chx, tsz) * (hl->cscale / 100.0);
											p->PS_translate(wideR, 0);
											}
										else
											p->PS_translate(hl->xp, (hl->yp - tsz) * -1);
										if (hl->cscale != 100)
											p->PS_scale(hl->cscale / 100.0, 1);
										if (hl->ccolor != "None")
											{
											SetFarbe(hl->ccolor, hl->cshade, &h, &s, &v, &k);
											p->PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
											p->PS_showSub(chr, (*Doc->AllFonts)[hl->cfont]->RealName(), tsz, false);
											}
										p->PS_restore();
										}
									}
								else
									{
									p->PS_selectfont(hl->cfont, tsz);
									if (hl->ccolor != "None")
										{
										SetFarbe(hl->ccolor, hl->cshade, &h, &s, &v, &k);
										p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
										}
									p->PS_save();
									if (c->Reverse)
										{
										int chs = hl->csize;
										c->SetZeichAttr(hl, &chs, &chx);
										p->PS_translate(hl->xp, -hl->yp);
										p->PS_scale(-1, 1);
										if (d < c->MaxChars-1)
											{
											QString ctx = c->Ptext.at(d+1)->ch;
											if (ctx == QChar(29))
												ctx = " ";
											wideR = -Cwidth(Doc, hl->cfont, chx, chs, ctx) * (hl->cscale / 100.0);
											p->PS_translate(wideR, 0);
											}
										else
											{
											wideR = -Cwidth(Doc, hl->cfont, chx, chs) * (hl->cscale / 100.0);
											p->PS_translate(wideR, 0);
											}
										if (hl->cscale != 100)
											p->PS_scale(hl->cscale / 100.0, 1);
										p->PS_show_xyG(hl->cfont, chx, 0, 0);
										}
									else
										{
										p->PS_translate(hl->xp, -hl->yp);
										if (hl->cscale != 100)
											p->PS_scale(hl->cscale / 100.0, 1);
										p->PS_show_xyG(hl->cfont, chx, 0, 0);
										}
									p->PS_restore();
									}
								if ((hl->cstyle & 4) && (chx != QChar(13)))
									{
									uint chr = chx[0].unicode();
									if ((*Doc->AllFonts)[hl->cfont]->CharWidth.contains(chr))
										{
										FPointArray gly = (*Doc->AllFonts)[hl->cfont]->GlyphArray[chr].Outlines.copy();
										QWMatrix chma;
										chma.scale(tsz / 10.0, tsz / 10.0);
										gly.map(chma);
										chma = QWMatrix();
										chma.scale(hl->cscale / 100.0, 1);
										gly.map(chma);
										if (c->Reverse)
											{
											chma = QWMatrix();
											chma.scale(-1, 1);
											chma.translate(wideR, 0);
											gly.map(chma);
											}
										if (hl->cstroke != "None")
											{
											p->PS_save();
											p->PS_setlinewidth(QMAX((*Doc->AllFonts)[hl->cfont]->strokeWidth / 2 * tsz, 1));
											p->PS_setdash(SolidLine, FlatCap, MiterJoin);
											p->PS_translate(hl->xp, (hl->yp - tsz) * -1);
											SetFarbe(hl->cstroke, hl->cshade2, &h, &s, &v, &k);
											p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
											SetClipPath(p, &gly);
											p->PS_closepath();
											p->PS_stroke();
											p->PS_restore();
											}
										}
									}
								if ((hl->cstyle & 8) && (chx != QChar(13)))
									{
									int chs = hl->csize;
									c->SetZeichAttr(hl, &chs, &chx);
									float csi = chs / 10.0;
									float wid = Cwidth(Doc, hl->cfont, chx, chs) * (hl->cscale / 100.0);
									QPixmap pgPix(hl->csize, static_cast<int>(wid));
									ScPainter *painter = new ScPainter(&pgPix, hl->csize, static_cast<int>(wid));
									QString psSt = "1 -1 scale\n";
									uint chr = chx[0].unicode();
									if ((*Doc->AllFonts)[hl->cfont]->CharWidth.contains(chr))
										{
										FPointArray gly = (*Doc->AllFonts)[hl->cfont]->GlyphArray[chr].Outlines.copy();
										float st = (*Doc->AllFonts)[hl->cfont]->underline_pos * chs;
										painter->setLineWidth(QMAX((*Doc->AllFonts)[hl->cfont]->strokeWidth * chs, 1));
										if (gly.size() < 4)
											{
											gly.resize(0);
											gly.addPoint(FPoint(0,0));
											gly.addPoint(FPoint(0,0));
											gly.addPoint(FPoint(1,0));
											gly.addPoint(FPoint(1,0));
											}
										QWMatrix chma;
										chma.scale(csi, csi);
										gly.map(chma);
										chma = QWMatrix();
										chma.scale(hl->cscale / 100.0, 1);
										gly.map(chma);
										painter->setupPolygon(&gly);
										painter->drawUnderline(FPoint(0-hl->cextra, chs-st), FPoint(wid, chs-st), true, &psSt);
										p->PS_save();
										if (c->Reverse)
											{
											p->PS_translate(hl->xp, (hl->yp - st) * -1);
											p->PS_scale(-1, 1);
											p->PS_translate(-wid, 0);
											}
										else
											p->PS_translate(hl->xp, (hl->yp - st) * -1);
										if (hl->ccolor != "None")
											{
											SetFarbe(hl->ccolor, hl->cshade, &h, &s, &v, &k);
											p->PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
											}
										p->PS_insert(psSt+"closepath\n");
										p->PS_fill(true);
										p->PS_restore();
										painter->end();
										delete painter;
										}
									}
								if (hl->cstyle & 16)
									{
									float Ulen = Cwidth(Doc, hl->cfont, chx, hl->csize) * (hl->cscale / 100.0);
									float Upos = (*Doc->AllFonts)[hl->cfont]->strikeout_pos * tsz;
									if (hl->ccolor != "None")
										{
										p->PS_setdash(SolidLine, FlatCap, MiterJoin);
										SetFarbe(hl->ccolor, hl->cshade, &h, &s, &v, &k);
										p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
										}
									p->PS_setlinewidth((*Doc->AllFonts)[hl->cfont]->strokeWidth * tsz);
									p->PS_moveto(hl->xp, -hl->yp+Upos);
									p->PS_lineto(hl->xp+Ulen, -hl->yp+Upos);
									p->PS_stroke();
									}
								if ((hl->cstyle & 128) && (((c->Ptext.at(QMIN(d+1, c->Ptext.count()-1))->yp != hl->yp) && (c->Ptext.at(QMIN(d+1, c->Ptext.count()-1))->ch != QChar(13))) || ((c->NextBox != 0) && (d == c->Ptext.count()-1))))
									{
									int chs = hl->csize;
									c->SetZeichAttr(hl, &chs, &chx);
									float wide = Cwidth(Doc, hl->cfont, chx, chs);
									chx = "-";
									uint chr = chx[0].unicode();
									if ((*Doc->AllFonts)[hl->cfont]->CharWidth.contains(chr))
										{
										FPointArray gly = (*Doc->AllFonts)[hl->cfont]->GlyphArray[chr].Outlines.copy();
										QWMatrix chma;
										chma.scale(tsz / 10.0, tsz / 10.0);
										gly.map(chma);
										chma = QWMatrix();
										chma.scale(hl->cscale / 100.0, 1);
										gly.map(chma);
										if (hl->ccolor != "None")
											{
											p->PS_save();
											p->PS_translate(hl->xp+wide, (hl->yp - tsz) * -1);
											SetFarbe(hl->ccolor, hl->cshade, &h, &s, &v, &k);
											p->PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
											SetClipPath(p, &gly);
											p->PS_closepath();
											p->PS_fill(true);
											p->PS_restore();
											}
										}
									}
								}
							if ((c->Pcolor2 != "None") || (c->NamedLStyle != ""))
								{
								SetFarbe(c->Pcolor2, c->Shade2, &h, &s, &v, &k);
								p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
								p->PS_setlinewidth(c->Pwidth);
								p->PS_setdash(c->PLineArt, c->PLineEnd, c->PLineJoin);
								if (c->NamedLStyle == "")
									{
									SetClipPath(p, &c->PoLine);
									p->PS_closepath();
									p->PS_stroke();
									}
								else
									{
									multiLine ml = Doc->MLineStyles[c->NamedLStyle];
									for (int it = ml.size()-1; it > -1; it--)
										{
										SetFarbe(ml[it].Color, ml[it].Shade, &h, &s, &v, &k);
										p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
										p->PS_setlinewidth(ml[it].Width);
										p->PS_setdash(static_cast<PenStyle>(ml[it].Dash), static_cast<PenCapStyle>(ml[it].LineEnd), static_cast<PenJoinStyle>(ml[it].LineJoin));
										SetClipPath(p, &c->PoLine);
										p->PS_closepath();
										p->PS_stroke();
										}
									}
								}
							break;
						case 5:
							if (c->NamedLStyle == "")
								{
								p->PS_moveto(0, 0);
								p->PS_lineto(c->Width, -c->Height);
								p->PS_stroke();
								}
							else
								{
								multiLine ml = Doc->MLineStyles[c->NamedLStyle];
								for (int it = ml.size()-1; it > -1; it--)
									{
									SetFarbe(ml[it].Color, ml[it].Shade, &h, &s, &v, &k);
									p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
									p->PS_setlinewidth(ml[it].Width);
									p->PS_setdash(static_cast<PenStyle>(ml[it].Dash), static_cast<PenCapStyle>(ml[it].LineEnd), static_cast<PenJoinStyle>(ml[it].LineJoin));
									p->PS_moveto(0, 0);
									p->PS_lineto(c->Width, -c->Height);
									p->PS_stroke();
									}
								}
							break;
						case 1:
						case 3:
						case 6:
							if ((c->Pcolor != "None") || (c->GrType != 0))
								{
								SetClipPath(p, &c->PoLine);
								p->PS_closepath();
								if (c->GrType != 0)
									{
									SetFarbe(c->GrColor2, c->GrShade2, &h, &s, &v, &k);
									p->PS_GradientCol1(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
									SetFarbe(c->GrColor, c->GrShade, &h, &s, &v, &k);
									p->PS_GradientCol2(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
									switch (c->GrType)
										{
										case 1:
										case 2:
										case 3:
										case 4:
											p->PS_LinGradient(c->Width, -c->Height, c->PType, c->GrType, multiPath);
											break;
										case 5:
											p->PS_RadGradient(c->Width, -c->Height, c->PType, multiPath);
											break;
										default:
											break;
										}
									}
								else
									p->PS_fill(multiPath);
								}
							if ((c->Pcolor2 != "None") || (c->NamedLStyle != ""))
								{
								if (c->NamedLStyle == "")
									{
									SetClipPath(p, &c->PoLine);
									p->PS_closepath();
									p->PS_stroke();
									}
								else
									{
									multiLine ml = Doc->MLineStyles[c->NamedLStyle];
									for (int it = ml.size()-1; it > -1; it--)
										{
										SetFarbe(ml[it].Color, ml[it].Shade, &h, &s, &v, &k);
										p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
										p->PS_setlinewidth(ml[it].Width);
										p->PS_setdash(static_cast<PenStyle>(ml[it].Dash), static_cast<PenCapStyle>(ml[it].LineEnd), static_cast<PenJoinStyle>(ml[it].LineJoin));
										SetClipPath(p, &c->PoLine);
										p->PS_closepath();
										p->PS_stroke();
										}
									}
								}
							break;
						case 7:
							if (c->NamedLStyle == "")
								{						
								SetClipPath(p, &c->PoLine);
								p->PS_stroke();
								}
							else
								{
								multiLine ml = Doc->MLineStyles[c->NamedLStyle];
								for (int it = ml.size()-1; it > -1; it--)
									{
									SetFarbe(ml[it].Color, ml[it].Shade, &h, &s, &v, &k);
									p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
									p->PS_setlinewidth(ml[it].Width);
									p->PS_setdash(static_cast<PenStyle>(ml[it].Dash), static_cast<PenCapStyle>(ml[it].LineEnd), static_cast<PenJoinStyle>(ml[it].LineJoin));
									SetClipPath(p, &c->PoLine);
									p->PS_stroke();
									}
								}
							break;
						case 8:
							if (c->PoShow)
								{
								if (c->PoLine.size() > 3)
									{
									p->PS_save();
									if (c->NamedLStyle == "")
										{
										SetClipPath(p, &c->PoLine);
										p->PS_stroke();
										}
									else
										{
										multiLine ml = Doc->MLineStyles[c->NamedLStyle];
										for (int it = ml.size()-1; it > -1; it--)
											{
											SetFarbe(ml[it].Color, ml[it].Shade, &h, &s, &v, &k);
											p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
											p->PS_setlinewidth(ml[it].Width);
											p->PS_setdash(static_cast<PenStyle>(ml[it].Dash), static_cast<PenCapStyle>(ml[it].LineEnd), static_cast<PenJoinStyle>(ml[it].LineJoin));
											SetClipPath(p, &c->PoLine);
											p->PS_stroke();
											}
										}
									p->PS_restore();
									}
								}
							for (d = 0; d < c->MaxChars; ++d)
								{
								hl = c->Ptext.at(d);
								if ((hl->ch == QChar(13)) || (hl->ch == QChar(30))) // || (hl->ch == QChar(32)))
									continue;
								tsz = hl->csize;
								chx = hl->ch;
								if (hl->ch == QChar(29))
									chx = " ";
								if (hl->cstyle & 64)
									{
									if (chx.upper() != chx)
										{
										tsz = hl->csize * Doc->VKapit / 100;
										chx = chx.upper();
										}
									}
								if (hl->cstyle & 1)
									tsz = hl->csize * Doc->VHochSc / 100;
								if (hl->cstyle & 2)
									tsz = hl->csize * Doc->VHochSc / 100;
								if (hl->ccolor != "None")
									{
									SetFarbe(hl->ccolor, hl->cshade, &h, &s, &v, &k);
									p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
									}
								if (((*Doc->AllFonts)[hl->cfont]->isOTF) || ((*Doc->AllFonts)[hl->cfont]->Subset))
									{
									uint chr = chx[0].unicode();
									if (((*Doc->AllFonts)[hl->cfont]->CharWidth.contains(chr)) && (chr != 32))
										{
										p->PS_save();
										p->PS_translate(hl->PtransX, -hl->PtransY);
										p->PS_rotate(-hl->PRot);
										if (c->Reverse)
											{
											p->PS_translate(hl->xp, (hl->yp - tsz) * -1);
											p->PS_scale(-1, 1);
											if (d < c->MaxChars-1)
												{
												QString ctx = c->Ptext.at(d+1)->ch;
												if (ctx == QChar(29))
													ctx = " ";
												wideR = -Cwidth(Doc, hl->cfont, chx, tsz, ctx) * (hl->cscale / 100.0);
												}
											else
												wideR = -Cwidth(Doc, hl->cfont, chx, tsz) * (hl->cscale / 100.0);
											p->PS_translate(wideR, 0);
											}
										else
											p->PS_translate(hl->xp, (hl->yp - tsz) * -1);
										if (hl->cscale != 100)
											p->PS_scale(hl->cscale / 100.0, 1);
										if (hl->ccolor != "None")
											{
											SetFarbe(hl->ccolor, hl->cshade, &h, &s, &v, &k);
											p->PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
											p->PS_showSub(chr, (*Doc->AllFonts)[hl->cfont]->RealName(), tsz, false);
											}
										p->PS_restore();
										}
									}
								else
									{
									p->PS_selectfont(hl->cfont, tsz);
									p->PS_save();
									p->PS_translate(hl->PtransX, -hl->PtransY);
									p->PS_rotate(-hl->PRot);
									p->PS_show_xyG(hl->cfont, chx, hl->xp, -hl->yp);
									p->PS_restore();
									}
								}
							break;
						}
					p->PS_restore();
					}
				}
			}
		Lnr++;
		}
}

void ScribusView::SetFarbe(QString farb, int shade, int *h, int *s, int *v, int *k)
{
	int h1, s1, v1, k1;
	h1 = *h;
	s1 = *s;
	v1 = *v;
	k1 = *k;
	CMYKColor	tmp = Doc->PageColors[farb];
	tmp.applyGCR();
	tmp.getCMYK(&h1, &s1, &v1, &k1);
	*h = h1 * shade / 100;
	*s = s1 * shade / 100;
	*v = v1 * shade / 100;
	*k = k1 * shade / 100;
}

void ScribusView::SetClipPath(PSLib *p, FPointArray *c)
{
	FPoint np, np1, np2;
	bool nPath = true;
	if (c->size() > 3)
		{
		for (uint poi=0; poi<c->size()-3; poi += 4)
			{
			if (c->point(poi).x() > 900000)
				{
				p->PS_closepath();
				nPath = true;
				continue;
				}
			if (nPath)
				{
				np = c->point(poi);
				p->PS_moveto(np.x(), -np.y());
				nPath = false;
				}
			np = c->point(poi+1);
			np1 = c->point(poi+3);
			np2 = c->point(poi+2);
			p->PS_curve(np.x(), -np.y(), np1.x(), -np1.y(), np2.x(), -np2.y());
			}
		}
}

void ScribusView::contentsWheelEvent(QWheelEvent *w)
{
	if ((Doc->ActPage->Mpressed) && (Doc->ActPage->MidButt))
		{
      w->delta() > 0 ? slotZoomIn2() : slotZoomOut2();
		}
	else
		{
      w->delta() < 0 ? scrollBy(0, Prefs->Wheelval) : scrollBy(0, -Prefs->Wheelval);
		}
	w->accept();
}
