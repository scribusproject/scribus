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

#ifdef _MSC_VER
 #if (_MSC_VER >= 1200)
  #include "win-config.h"
 #endif
#else
 #include "config.h"
#endif

#include "pageback.h"
#include <qcolor.h>
#include <qfont.h>
#include <qfontmetrics.h>
#include <qpixmap.h>
#include <qpointarray.h>
#include <qstringlist.h>
#include <qimage.h>
#include <qcstring.h>
#include <qfileinfo.h>
#include <cmath>
#include "scribus.h"

using namespace std;

extern void Level2Layer(ScribusDoc *doc, struct Layer *ll, int Level);
extern double Cwidth(ScribusDoc *doc, QString name, QString ch, int Siz, QString ch2 = " ");
extern ScribusApp* ScApp;
extern void ReOrderText(ScribusDoc *doc, ScribusView *view);

ScribusView::ScribusView(QWidget *parent, ScribusDoc *doc, preV *prefs)
		: QScrollView(parent, "s", WRepaintNoErase | WNorthWestGravity)
{
	setName("mainView");
	Ready = false;
	doZooming = false;
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
	LE = new MSpinBox( 10, 3200, this, 2 );
	LE->setFont(fo);
	LE->setSuffix( tr( " %" ) );
	LE->setValue( 100 );
	LE->setFocusPolicy(QWidget::ClickFocus);
	SB1 = new QPushButton(this);
	SB1->setPixmap(loadIcon("Klein.xpm"));
	SB1->setFocusPolicy(QWidget::NoFocus);
	SB2 = new QPushButton(this);
	SB2->setFocusPolicy(QWidget::NoFocus);
	SB2->setPixmap(loadIcon("Gross.xpm"));
	PGS = new PageSelector(this, 1);
	PGS->setFont(fo);
	PGS->setFocusPolicy(QWidget::ClickFocus);
	LY = new QPushButton(this);
	LY->setFont(fo);
	Laymen = new QPopupMenu(this);
	Laymen->setFont(fo);
	LY->setText( tr("Layer")+" 0");
	LY->setPopup(Laymen);
	LY->setFocusPolicy(QWidget::NoFocus);
	HR = new Hruler(this, Doc);
	VR = new Vruler(this, Doc);
	UN = new QToolButton(this);
	Unitmen = new QPopupMenu(this);
	Unitmen->insertItem(tr("pt"));
	Unitmen->insertItem(tr("mm"));
	Unitmen->insertItem(tr("in"));
	Unitmen->insertItem(tr("p"));
	UN->setPopup(Unitmen);
	UN->setFocusPolicy(QWidget::NoFocus);
	UN->setPopupDelay(10);
	switch (doc->Einheit)
	{
	case 0:
		UN->setText( tr("pt"));
		break;
	case 1:
		UN->setText( tr("mm"));
		break;
	case 2:
		UN->setText( tr("in"));
		break;
	case 3:
		UN->setText( tr("p"));
		break;
	}
	Pages.clear();
	MasterPages.clear();
	DocPages.clear();
	Ready = true;
	// jjsa 27-03-2004 added 5 variables for zoom
	oldX = -1,
	       oldY = -1;
	oldScale=1;
	oldSby=0;
	oldSbx=0;

	connect(SB1, SIGNAL(clicked()), this, SLOT(slotZoomOut()));
	connect(SB2, SIGNAL(clicked()), this, SLOT(slotZoomIn()));
	connect(LE, SIGNAL(valueChanged(int)), this, SLOT(Zval()));
	connect(PGS, SIGNAL(GotoPage(int)), this, SLOT(GotoPa(int)));
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
			PGS->setFont(ff);
			LY->setFont(ff);
			HR->setFont(ff);
			VR->setFont(ff);
		}
		QRect forec = fom.boundingRect("3200.00 %");
		int sadj = forec.width() - LE->ed->width();
		LE->setGeometry(x, y, 60+sadj, h);
		SB1->setGeometry(x+60+sadj, y, 15, h);
		SB2->setGeometry(x+75+sadj, y, 15, h);
		PGS->setGeometry(x+90+sadj, y, 215, h);
		LY->setGeometry(x+305+sadj, y, 110, h);
		HR->setGeometry(25, 1, w-24, 25);
		bar.setGeometry(x+415+sadj, y, w-(415+sadj), h);
	}
}
// jjsa 27-03-2004 add for better settinf while zooming
void ScribusView::rememberPreviousSettings(int mx, int my)
{
	// remember scale
	oldScale = Doc->Scale;

	oldX = childX(Doc->ActPage->parentWidget()) + mx;
	oldY = childY(Doc->ActPage->parentWidget()) + my;
	oldX2 = mx;
	oldY2 = my;
	// scrollbar position
	oldSbx = horizontalScrollBar()->value();
	oldSby = verticalScrollBar()->value();

	oldX = childX(Doc->ActPage->parentWidget()) + mx + oldSbx;
	oldY = childY(Doc->ActPage->parentWidget()) + my + oldSby;
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
	if (ScApp->ScriptRunning)
		return;
	HR->offs = x-static_cast<int>(10*Doc->Scale)-2;
	HR->repX = false;
	HR->repaint();
	VR->offs = y-static_cast<int>(10*Doc->Scale)-2;
	VR->repaint();
}

void ScribusView::Zval()
{
	int x = qRound(QMAX(Doc->ActPage->ViewReg().boundingRect().x() / Doc->Scale, 0));
	int y = qRound(QMAX(Doc->ActPage->ViewReg().boundingRect().y() / Doc->Scale, 0));
	int w = qRound(QMIN(Doc->ActPage->ViewReg().boundingRect().width() / Doc->Scale, Doc->PageB));
	int h = qRound(QMIN(Doc->ActPage->ViewReg().boundingRect().height() / Doc->Scale, Doc->PageH));
	rememberPreviousSettings(w / 2 + x,h / 2 + y);
	Doc->Scale = LE->value() / 100.0 * Prefs->DisScale;
	slotDoZoom();
	ScApp->setFocus();
}

/** Adds a Page */
Page* ScribusView::addPage(int nr)
{
	int z;
	double s = Doc->Scale;
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
		z = fe->PaintText(fe->Margins.Left,
		                  fe->Margins.Top,
		                  Doc->PageB-fe->Margins.Right-fe->Margins.Left,
		                  Doc->PageH-fe->Margins.Bottom-fe->Margins.Top,
		                  1, Doc->Dpen);
		fe->Items.at(z)->isAutoText = true;
		fe->Items.at(z)->BackBox = Doc->LastAuto;
		fe->Items.at(z)->Cols = qRound(Doc->PageSp);
		fe->Items.at(z)->ColGap = Doc->PageSpa;
		if (Doc->LastAuto != 0)
			Doc->LastAuto->NextBox = fe->Items.at(z);
		else
			Doc->FirstAuto = fe->Items.at(z);
		Doc->LastAuto = fe->Items.at(z);
		fe->SetRectFrame(fe->Items.at(z));
		if (nr > 0)
		{
			bool savre = Doc->RePos;
			Doc->RePos = true;
			QPixmap pgPix(1, 1);
			ScPainter *painter = new ScPainter(&pgPix, 1, 1);
			painter->translate(0.5, 0.5);
			if (fe->Items.at(z)->BackBox != 0)
				fe->Items.at(z)->BackBox->DrawObj(painter, QRect(0, 0, 1, 1));
			painter->end();
			delete painter;
			Doc->RePos = savre;
		}
	}
	PGS->setMaxValue(Doc->PageC);
	if ((!ScApp->ScriptRunning) && (!Doc->loading) && (!Doc->MasterP))
		PGS->GotoPg(nr);
	fe->setMouseTracking(true);
	connect(fe, SIGNAL(Hrule(int)), HR, SLOT(Draw(int)));
	connect(fe, SIGNAL(Vrule(int)), VR, SLOT(Draw(int)));
	connect(fe, SIGNAL(PgCh(int)), this, SLOT(setMenTxt(int)));
	connect(fe, SIGNAL(ZoomIn(int, int)), this, SLOT(slotZoomIn2(int, int)));
	connect(fe, SIGNAL(ZoomOut(int, int)), this, SLOT(slotZoomOut2(int, int)));
	connect(fe, SIGNAL(ZoomAbs()), this, SLOT(slotDoZoom()));
	connect(fe, SIGNAL(AbsPosi(int, int)), this, SLOT(SetCPo(int, int)));
	connect(fe, SIGNAL(AbsPosi2(int, int)), this, SLOT(SetCCPo(int, int)));
	return fe;
}

/** Lscht eine Seite */
void ScribusView::delPage(int Nr)
{
	if (Doc->PageC == 1)
		return;
	if (Doc->PageC < Nr-1)
		return;
	removeChild(Pages.at(Nr)->parentWidget());
	delete Pages.at(Nr)->parentWidget();
	Pages.remove(Nr);
	Doc->UnDoValid = false;
	Doc->PageC -= 1;
	Doc->ActPage = Pages.at(0);
	if (Doc->TemplateMode)
		MasterPages = Pages;
	else
		DocPages = Pages;
	PGS->setMaxValue(Doc->PageC);
	PGS->GotoPg(0);
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
	if (Doc->TemplateMode)
		MasterPages = Pages;
	else
		DocPages = Pages;
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
		if (!ScApp->ScriptRunning)
			setContentsPos(childX(Doc->ActPage->parentWidget())-static_cast<int>(10*Doc->Scale),
			               childY(Doc->ActPage->parentWidget())-static_cast<int>(10*Doc->Scale));
	}
	else
	{
		resizeContents(static_cast<int>(PSeite->width()+30*Doc->Scale), static_cast<int>(Doc->PageC * (PSeite->height()+25*Doc->Scale)+30));
		if (!ScApp->ScriptRunning)
			setContentsPos(0, childY(Doc->ActPage->parentWidget())-static_cast<int>(10*Doc->Scale));
	}
	setRulerPos(contentsX(), contentsY());
	setMenTxt(Doc->ActPage->PageNr);
}

void ScribusView::setMenTxt(int Seite)
{
	if (ScApp->ScriptRunning)
		return;
	disconnect(PGS, SIGNAL(GotoPage(int)), this, SLOT(GotoPa(int)));
	PGS->setMaxValue(Doc->PageC);
	if ((!Doc->loading) && (!Doc->MasterP))
		PGS->GotoPg(Seite);
	connect(PGS, SIGNAL(GotoPage(int)), this, SLOT(GotoPa(int)));
}

void ScribusView::setLayMenTxt(int l)
{
	QValueList<Layer>::iterator it;
	QString lName;
	for (it = Doc->Layers.begin(); it != Doc->Layers.end(); ++it)
	{
		if ((*it).LNr == l)
			break;
	}
	lName=(*it).Name;
	int len = lName.length();
	while (LY->fontMetrics().width(lName) > LY->width()-25)
	{
		len--;
		if (len == 0)
			break;
		lName.truncate(len);
		lName += ".";
	}
	LY->setText(lName);
}

/** Fuehrt die Vergroesserung/Verkleinerung aus */
void ScribusView::slotDoZoom()
{
	uint a;
	Page* Seite;
	QWidget* PSeite = Doc->ActPage->parentWidget();
	if (Doc->Scale > 32*Prefs->DisScale)
	{
		Doc->Scale = 32*Prefs->DisScale;
		return;
	}
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
				resizeContents(static_cast<int>(PSeite->width()*2+60*Doc->Scale), static_cast<int>((Doc->PageC/2 + 1) * (PSeite->height()+25*Doc->Scale)+30));
		}
		else
			resizeContents(static_cast<int>(PSeite->width()+30*Doc->Scale), static_cast<int>(Doc->PageC * (PSeite->height()+25*Doc->Scale)+30));
		disconnect(LE, SIGNAL(valueChanged(int)), this, SLOT(Zval()));
		LE->setValue(Doc->Scale/Prefs->DisScale*100);
		connect(LE, SIGNAL(valueChanged(int)), this, SLOT(Zval()));
		setRulerPos(contentsX(), contentsY());
		if (Doc->ActPage->SelItem.count() != 0)
		{
			PageItem *b = Doc->ActPage->SelItem.at(0);
			SetCCPo(static_cast<int>(b->Xpos + b->Width/2), static_cast<int>(b->Ypos + b->Height/2));
		}
		else
			SetCCPo(oldX2, oldY2);
	}
}

void ScribusView::SetCCPo(int x, int y)
{
	if (ScApp->ScriptRunning)
		return;
	center(static_cast<int>(childX(Doc->ActPage->parentWidget())+x*Doc->Scale), static_cast<int>(childY(Doc->ActPage->parentWidget())+y*Doc->Scale));
	setRulerPos(contentsX(), contentsY());
}

void ScribusView::SetCPo(int x, int y)
{
	if (ScApp->ScriptRunning)
		return;
	setContentsPos(static_cast<int>(childX(Doc->ActPage->parentWidget())+x*Doc->Scale), static_cast<int>(childY(Doc->ActPage->parentWidget())+y*Doc->Scale));
	setRulerPos(contentsX(), contentsY());
}

void ScribusView::LaMenu()
{
	uint a;
	QValueList<Layer>::iterator it;
 	Laymen->clear();
 	if (Doc->Layers.count() != 0)
 	{
		for (a=0; a < Doc->Layers.count(); a++)
 		{
			int curr,n=0;
			for (it = Doc->Layers.begin(); it != Doc->Layers.end(); ++it,n++)
			{
				if (Doc->Layers.count()-(*it).Level-1 == a)
				{
					Laymen->insertItem((*it).Name);
					if (Doc->ActiveLayer == n)
						curr=Doc->Layers.count()-(*it).Level-1;
				}
			}
			Laymen->setItemChecked(Laymen->idAt(curr), true);
 		}
 	}
}

void ScribusView::GotoLa(int l)
{
	int a=0;
	int d = Doc->Layers.count()-Laymen->indexOf(l)-1;
	QValueList<Layer>::iterator it;
	for (it = Doc->Layers.begin(); it != Doc->Layers.end(); ++it,a++)
	{
		if ( d == (*it).Level )
		{
			Doc->ActiveLayer = a;
			LY->setText(Doc->Layers[a].Name);
			emit changeLA(a);
			break;
		}
	}
}

void ScribusView::GotoPa(int Seite)
{
	GotoPage(Seite-1);
	ScApp->setFocus();
}

void ScribusView::ChgUnit(int art)
{
	int d = Unitmen->indexOf(art);
	emit changeUN(d);
}

void ScribusView::GotoPage(int Seite)
{
	Doc->ActPage = Pages.at(Seite);
	if (ScApp->ScriptRunning)
		return;
	setContentsPos(static_cast<int>(childX(Doc->ActPage->parentWidget())-10*Doc->Scale), static_cast<int>(childY(Doc->ActPage->parentWidget())-10*Doc->Scale));
	PGS->setMaxValue(Doc->PageC);
	if ((!Doc->loading) && (!Doc->MasterP))
		PGS->GotoPg(Seite);
}

void ScribusView::slotZoomIn(int mx,int my)
{
	if ((mx == 0) && (my == 0))
	{
		int x = qRound(QMAX(Doc->ActPage->ViewReg().boundingRect().x() / Doc->Scale, 0));
		int y = qRound(QMAX(Doc->ActPage->ViewReg().boundingRect().y() / Doc->Scale, 0));
		int w = qRound(QMIN(Doc->ActPage->ViewReg().boundingRect().width() / Doc->Scale, Doc->PageB));
		int h = qRound(QMIN(Doc->ActPage->ViewReg().boundingRect().height() / Doc->Scale, Doc->PageH));
		rememberPreviousSettings(w / 2 + x,h / 2 + y);
	}
	else
		rememberPreviousSettings(mx,my);
	Doc->Scale *= 2;
	if (Doc->Scale > 32*Prefs->DisScale)
		Doc->Scale = 32*Prefs->DisScale;
	slotDoZoom();
}

/** Verkleinert die Ansicht */
void ScribusView::slotZoomOut(int mx,int my)
{
	if ((mx == 0) && (my == 0))
	{
		int x = qRound(QMAX(Doc->ActPage->ViewReg().boundingRect().x() / Doc->Scale, 0));
		int y = qRound(QMAX(Doc->ActPage->ViewReg().boundingRect().y() / Doc->Scale, 0));
		int w = qRound(QMIN(Doc->ActPage->ViewReg().boundingRect().width() / Doc->Scale, Doc->PageB));
		int h = qRound(QMIN(Doc->ActPage->ViewReg().boundingRect().height() / Doc->Scale, Doc->PageH));
		rememberPreviousSettings(w / 2 + x,h / 2 + y);
	}
	else
		rememberPreviousSettings(mx,my);
	Doc->Scale /= 2;
	slotDoZoom();
}

/** Vergr
rt die Ansicht */
void ScribusView::slotZoomIn2(int mx,int my)
{
	doZooming = true;
	rememberPreviousSettings(mx,my);
	Doc->Scale += static_cast<double>(Doc->MagStep*Prefs->DisScale)/100.0;
	if (Doc->Scale > static_cast<double>(Doc->MagMax*Prefs->DisScale)/100.0)
	{
		Doc->Scale = static_cast<double>(Doc->MagMax*Prefs->DisScale)/100.0;
		return;
	}
	slotDoZoom();
}

/** Verkleinert die Ansicht */
void ScribusView::slotZoomOut2(int mx,int my)
{
	rememberPreviousSettings(mx,my);
	Doc->Scale -= static_cast<double>(Doc->MagStep*Prefs->DisScale)/100.0;
	if (Doc->Scale < static_cast<double>(Doc->MagMin*Prefs->DisScale)/100.0)
		Doc->Scale = static_cast<double>(Doc->MagMin*Prefs->DisScale)/100.0;
	slotDoZoom();
}

void ScribusView::DrawNew()
{
	if (ScApp->ScriptRunning)
		return;
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
	setMenTxt(Doc->ActPage->PageNr);
	disconnect(LE, SIGNAL(valueChanged(int)), this, SLOT(Zval()));
	LE->setValue(Doc->Scale/Prefs->DisScale*100);
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
	// jjsa 27-03-2004
	rememberPreviousSettings();
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
	PGS->setEnabled(false);
	reformPages();
	slotDoZoom();
}

void ScribusView::HideTemplate()
{
	// jjsa 27-03-2004
	rememberPreviousSettings();
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
	PGS->setEnabled(true);
	reformPages();
	GotoPage(0);
	slotDoZoom();
}

QPixmap ScribusView::MPageToPixmap(QString name, int maxGr)
{
	QPixmap pm = QPixmap(static_cast<int>(Doc->PageB), static_cast<int>(Doc->PageH));
	ScPainter *painter = new ScPainter(&pm, pm.width(), pm.height());
	double sca = Doc->Scale;
	bool frs = Prefs->FramesShown;
	int Lnr;
	struct Layer ll;
	ll.Drucken = false;
	ll.LNr = 0;
	Prefs->FramesShown = false;
	Doc->Scale = 1;
	PageItem *b;
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
					b->DrawObj(painter, QRect(0, 0, static_cast<int>(Doc->PageB), static_cast<int>(Doc->PageH)));
					b->Parent = Opa;
					b->OwnPage = Opa2;
				}
			}
			Lnr++;
		}
	}
	painter->end();
	Prefs->FramesShown = frs;
	Doc->Scale = sca;
	QImage im2;
	QImage im = pm.convertToImage();
	double sx = im.width() / static_cast<double>(maxGr);
	double sy = im.height() / static_cast<double>(maxGr);
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
	double sca = Doc->Scale;
	bool frs = Prefs->FramesShown;
	int Lnr;
	struct Layer ll;
	ll.Drucken = false;
	ll.LNr = 0;
	Prefs->FramesShown = false;
	Doc->Scale = 1;
	PageItem *b;
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
						b->DrawObj(painter, QRect(0, 0, static_cast<int>(Doc->PageB), static_cast<int>(Doc->PageH)));
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
					b->DrawObj(painter, QRect(0, 0, static_cast<int>(Doc->PageB), static_cast<int>(Doc->PageH)));
				}
			}
		}
		Lnr++;
	}
	painter->end();
	Prefs->FramesShown = frs;
	Doc->Scale = sca;
	QImage im2;
	QImage im = pm.convertToImage();
	double sx = im.width() / static_cast<double>(maxGr);
	double sy = im.height() / static_cast<double>(maxGr);
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

void ScribusView::CreatePS(PSLib *p, std::vector<int> &pageNs, bool sep, QString SepNam, bool farb, bool Hm, bool Vm, bool Ic, bool gcr)
{
	uint a;
	int sepac;
	double wideR;
	QValueList<double> dum;
	double gx, gy, gw, gh;
	dum.clear();
	ReOrderText(Doc, this);
	p->PS_set_Info("Author", Doc->DocAutor);
	p->PS_set_Info("Title", Doc->DocTitel);
	if (!farb)
		p->PS_setGray();
	if ((!p->Art) && (Doc->ActPage->SelItem.count() != 0))
	{
		Doc->ActPage->setGroupRect();
		Doc->ActPage->getGroupRect(&gx, &gy, &gw, &gh);
		p->PS_begin_doc(Doc->PageOri, gx, Doc->PageH - (gy+gh), gx + gw, Doc->PageH - gy, pageNs.size());
	}
	else
		p->PS_begin_doc(Doc->PageOri, 0.0, 0.0, Doc->PageB, Doc->PageH, pageNs.size());
	for (uint ap = 0; ap < MasterPages.count(); ++ap)
	{
		int Lnr = 0;
		struct Layer ll;
		ll.Drucken = false;
		ll.LNr = 0;
		if (MasterPages.at(ap)->Items.count() != 0)
		{
			for (uint lam = 0; lam < Doc->Layers.count(); ++lam)
			{
				Level2Layer(Doc, &ll, Lnr);
				if (ll.Drucken)
				{
					for (uint api = 0; api < MasterPages.at(ap)->Items.count(); ++api)
					{
						int h, s, v, k, tsz;
						double wideR;
						struct Pti *hl;
						QString tmps, chx;
						PageItem *c = MasterPages.at(ap)->Items.at(api);
						if ((c->LayerNr != ll.LNr) || (!c->isPrintable))
							continue;
						if ((c->PType == 2) && (c->PicAvail) && (c->Pfile != "") && (c->isPrintable) && (!sep) && (farb))
							p->PS_ImageData(c->InvPict, c->Pfile, c->AnName, c->IProfile, c->UseEmbedded, Ic);
						p->PS_TemplateStart(MasterPages.at(ap)->PageNam + tmps.setNum(c->ItemNr), Doc->PageB, Doc->PageH);
						if (c->isPrintable)
						{
							p->PS_save();
							if (c->Pcolor != "None")
							{
								SetFarbe(c->Pcolor, c->Shade, &h, &s, &v, &k, gcr);
								p->PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
							}
							if (c->Pcolor2 != "None")
							{
								SetFarbe(c->Pcolor2, c->Shade2, &h, &s, &v, &k, gcr);
								p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
							}
							p->PS_setlinewidth(c->Pwidth);
							p->PS_setcapjoin(c->PLineEnd, c->PLineJoin);
							p->PS_setdash(c->PLineArt, c->DashOffset, c->DashValues);
							p->PS_translate(c->Xpos, Doc->PageH - c->Ypos);
							if (c->Rot != 0)
								p->PS_rotate(-c->Rot);
							switch (c->PType)
							{
							case 2:
								break;
							case 4:
								break;
							case 5:
								if ((c->NamedLStyle == "") && (c->Pwidth != 0.0))
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
										SetFarbe(ml[it].Color, ml[it].Shade, &h, &s, &v, &k, gcr);
										p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
										p->PS_setlinewidth(ml[it].Width);
										p->PS_setcapjoin(static_cast<PenCapStyle>(ml[it].LineEnd), static_cast<PenJoinStyle>(ml[it].LineJoin));
										p->PS_setdash(static_cast<PenStyle>(ml[it].Dash), 0, dum);
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
										HandleGradient(p, c, c->Width, c->Height, gcr);
									else
										p->PS_fill();
								}
								if ((c->Pcolor2 != "None") || (c->NamedLStyle != ""))
								{
									if ((c->NamedLStyle == "") && (c->Pwidth != 0.0))
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
											SetFarbe(ml[it].Color, ml[it].Shade, &h, &s, &v, &k, gcr);
											p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
											p->PS_setlinewidth(ml[it].Width);
											p->PS_setcapjoin(static_cast<PenCapStyle>(ml[it].LineEnd), static_cast<PenJoinStyle>(ml[it].LineJoin));
											p->PS_setdash(static_cast<PenStyle>(ml[it].Dash), 0, dum);
											SetClipPath(p, &c->PoLine);
											p->PS_closepath();
											p->PS_stroke();
										}
									}
								}
								break;
							case 7:
								if ((c->Pcolor != "None") || (c->GrType != 0))
								{
									SetClipPath(p, &c->PoLine);
									p->PS_closepath();
									if (c->GrType != 0)
										HandleGradient(p, c, c->Width, c->Height, gcr);
									else
										p->PS_fill();
									p->PS_newpath();
								}
								if ((c->NamedLStyle == "") && (c->Pwidth != 0.0))
								{
									SetClipPath(p, &c->PoLine, false);
									p->PS_stroke();
								}
								else
								{
									multiLine ml = Doc->MLineStyles[c->NamedLStyle];
									for (int it = ml.size()-1; it > -1; it--)
									{
										SetFarbe(ml[it].Color, ml[it].Shade, &h, &s, &v, &k, gcr);
										p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
										p->PS_setlinewidth(ml[it].Width);
										p->PS_setcapjoin(static_cast<PenCapStyle>(ml[it].LineEnd), static_cast<PenJoinStyle>(ml[it].LineJoin));
										p->PS_setdash(static_cast<PenStyle>(ml[it].Dash), 0, dum);
										SetClipPath(p, &c->PoLine, false);
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
										if ((c->NamedLStyle == "") && (c->Pwidth != 0.0))
										{
											SetClipPath(p, &c->PoLine, false);
											p->PS_stroke();
										}
										else
										{
											multiLine ml = Doc->MLineStyles[c->NamedLStyle];
											for (int it = ml.size()-1; it > -1; it--)
											{
												SetFarbe(ml[it].Color, ml[it].Shade, &h, &s, &v, &k, gcr);
												p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
												p->PS_setlinewidth(ml[it].Width);
												p->PS_setcapjoin(static_cast<PenCapStyle>(ml[it].LineEnd), static_cast<PenJoinStyle>(ml[it].LineJoin));
												p->PS_setdash(static_cast<PenStyle>(ml[it].Dash), 0, dum);
												SetClipPath(p, &c->PoLine, false);
												p->PS_stroke();
											}
										}
										p->PS_restore();
									}
								}
								for (uint d = 0; d < c->MaxChars; ++d)
								{
									hl = c->Ptext.at(d);
									if ((hl->ch == QChar(13)) || (hl->ch == QChar(30)) || (hl->ch == QChar(9)) || (hl->ch == QChar(28)))
										continue;
									tsz = hl->csize;
									chx = hl->ch;
									if (hl->ch == QChar(29))
										chx = " ";
									if (hl->ch == QChar(0xA0))
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
										SetFarbe(hl->ccolor, hl->cshade, &h, &s, &v, &k, gcr);
										p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
									}
									/* Subset all TTF Fonts until the bug in the TTF-Embedding Code is fixed */
									QFileInfo fd = QFileInfo((*Doc->AllFonts)[hl->cfont]->Datei);
									QString fext = fd.extension(false).lower();
									if ((fext == "ttf") || ((*Doc->AllFonts)[hl->cfont]->isOTF) || ((*Doc->AllFonts)[hl->cfont]->Subset))
									{
										uint chr = chx[0].unicode();
										if (((*Doc->AllFonts)[hl->cfont]->CharWidth.contains(chr)) && (chr != 32))
										{
											p->PS_save();
/*											p->PS_translate(hl->PtransX, -hl->PtransY);
											p->PS_rotate(-hl->PRot);
											if (c->Reverse)
											{
												p->PS_translate(hl->xp, (hl->yp - (tsz / 10.0)) * -1);
												p->PS_scale(-1, 1);
												if (d < c->MaxChars-1)
												{
													QString ctx = c->Ptext.at(d+1)->ch;
													if (ctx == QChar(29))
														ctx = " ";
													if (ctx == QChar(0xA0))
														ctx = " ";
													wideR = -Cwidth(Doc, hl->cfont, chx, tsz, ctx) * (hl->cscale / 100.0);
												}
												else
													wideR = -Cwidth(Doc, hl->cfont, chx, tsz) * (hl->cscale / 100.0);
												p->PS_translate(wideR, 0);
											}
											else
												p->PS_translate(hl->xp, (hl->yp - (tsz / 10.0)) * -1); */
											if (hl->cscale != 100)
												p->PS_scale(hl->cscale / 100.0, 1);
											if (hl->ccolor != "None")
											{
												SetFarbe(hl->ccolor, hl->cshade, &h, &s, &v, &k, gcr);
												p->PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
												p->PutSeite("[ 1 0 0 -1 " + p->ToStr(-hl->PRot) + " 0 ]\n");
												p->PutSeite("["+p->ToStr(hl->PtransX) + " " + p->ToStr(-hl->PtransY) + " " + p->ToStr(-hl->PtransY) + " " + p->ToStr(-hl->PtransX) + " " + p->ToStr(hl->xp) + " " + p->ToStr(-hl->yp) + "]\n");
												p->PutSeite("[ 0 0 0 0 0 0 ] concatmatrix\nconcat\n");
												p->PS_translate(0, (tsz / 10.0));
												if (c->BaseOffs != 0)
													p->PS_translate(0, -c->BaseOffs);
												p->PS_showSub(chr, (*Doc->AllFonts)[hl->cfont]->RealName().simplifyWhiteSpace().replace( QRegExp("\\s"), "" ), tsz / 10.0, false);
											}
											p->PS_restore();
										}
									}
									else
									{
										p->PS_selectfont(hl->cfont, tsz / 10.0);
										p->PS_save();
										p->PutSeite("[ 1 0 0 -1 " + p->ToStr(-hl->PRot) + " 0 ]\n");
										p->PutSeite("["+p->ToStr(hl->PtransX) + " " + p->ToStr(-hl->PtransY) + " " + p->ToStr(-hl->PtransY) + " " + p->ToStr(-hl->PtransX) + " " + p->ToStr(hl->xp) + " " + p->ToStr(-hl->yp) + "]\n");
										p->PutSeite("[ 0 0 0 0 0 0 ] concatmatrix\nconcat\n");
										if (c->BaseOffs != 0)
											p->PS_translate(0, -c->BaseOffs);
										p->PS_show_xyG(hl->cfont, chx, 0, 0);
										p->PS_restore();
									}
								}
								break;
							}
							p->PS_restore();
						}
						p->PS_TemplateEnd();
					}
				}
				Lnr++;
			}
		}
	}
	sepac = 0;
	uint aa = 0;
	while (aa < pageNs.size())
	{
		a = pageNs[aa]-1;
		if ((!p->Art) && (Doc->ActPage->SelItem.count() != 0))
		{
			struct Margs Ma;
			Ma.Left = gx;
			Ma.Top = gy;
			Ma.Bottom = Doc->PageH - (gy + gh);
			Ma.Right = Doc->PageB - (gx + gw);
			p->PS_begin_page(Doc->PageB, Doc->PageH, &Ma, true);
		}
		else
			p->PS_begin_page(Doc->PageB, Doc->PageH, &Pages.at(a)->Margins, Prefs->ClipMargin);
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
				for (uint lam = 0; lam < Doc->Layers.count(); ++lam)
				{
					Level2Layer(Doc, &ll, Lnr);
					if (ll.Drucken)
					{
						for (uint am = 0; am < mPage->Items.count(); ++am)
						{
							QString tmps;
							PageItem *ite = mPage->Items.at(am);
							if ((ite->LayerNr != ll.LNr) || (!ite->isPrintable))
								continue;
							if ((ite->PType != 4) && (ite->PType != 2))
								p->PS_UseTemplate(Pages.at(a)->MPageNam + tmps.setNum(ite->ItemNr));
							else if (ite->PType == 2)
							{
								p->PS_save();
								p->PS_translate(ite->Xpos, Doc->PageH - ite->Ypos);
								if (ite->Rot != 0)
									p->PS_rotate(-ite->Rot);
								if (ite->Pcolor != "None")
								{
									SetFarbe(ite->Pcolor, ite->Shade, &h, &s, &v, &k, gcr);
									p->PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
									SetClipPath(p, &ite->PoLine);
									p->PS_closepath();
									p->PS_fill();
								}
								else
									p->PS_setcmykcolor_dummy();
								SetClipPath(p, &ite->PoLine);
								p->PS_closepath();
								p->PS_clip(false);
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
									if ((!sep) && (farb))
										p->PS_image(ite->InvPict, -ite->BBoxX+ite->LocalX, -ite->LocalY, ite->Pfile, ite->LocalScX, ite->LocalScY, ite->IProfile, ite->UseEmbedded, Ic, ite->AnName);
									else
										p->PS_image(ite->InvPict, -ite->BBoxX+ite->LocalX, -ite->LocalY, ite->Pfile, ite->LocalScX, ite->LocalScY, ite->IProfile, ite->UseEmbedded, Ic);
								}
								p->PS_restore();
								if (((ite->Pcolor2 != "None") || (ite->NamedLStyle != "")) && (!ite->isTableItem))
								{
									if ((ite->NamedLStyle == "") && (ite->Pwidth != 0.0))
									{
										SetFarbe(ite->Pcolor2, ite->Shade2, &h, &s, &v, &k, gcr);
										p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
										p->PS_setlinewidth(ite->Pwidth);
										p->PS_setcapjoin(ite->PLineEnd, ite->PLineJoin);
										p->PS_setdash(ite->PLineArt, ite->DashOffset, ite->DashValues);
										SetClipPath(p, &ite->PoLine);
										p->PS_closepath();
										p->PS_stroke();
									}
									else
									{
										multiLine ml = Doc->MLineStyles[ite->NamedLStyle];
										for (int it = ml.size()-1; it > -1; it--)
										{
											SetFarbe(ml[it].Color, ml[it].Shade, &h, &s, &v, &k, gcr);
											p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
											p->PS_setlinewidth(ml[it].Width);
											p->PS_setcapjoin(static_cast<PenCapStyle>(ml[it].LineEnd), static_cast<PenJoinStyle>(ml[it].LineJoin));
											p->PS_setdash(static_cast<PenStyle>(ml[it].Dash), 0, dum);
											SetClipPath(p, &ite->PoLine);
											p->PS_closepath();
											p->PS_stroke();
										}
									}
								}
								p->PS_restore();
							}
							else if (ite->PType == 4)
							{
								QWidget* Opa;
								Page* Opa2;
								Opa = ite->Parent;
								Opa2 = ite->OwnPage;
								ite->Parent = Pages.at(a);
								ite->OwnPage = Pages.at(a);
								double savScale = Doc->Scale;
								Doc->Scale = 1.0;
								Doc->RePos = true;
								QPixmap pgPix(10, 10);
								QRect rd = QRect(0,0,9,9);
								ScPainter *painter = new ScPainter(&pgPix, pgPix.width(), pgPix.height());
								ite->DrawObj(painter, rd);
								Doc->RePos = false;
								Doc->Scale = savScale;
								delete painter;
								ite->Parent = Opa;
								ite->OwnPage = Opa2;
								p->PS_save();
								if (ite->Pcolor != "None")
								{
									SetFarbe(ite->Pcolor, ite->Shade, &h, &s, &v, &k, gcr);
									p->PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
								}
								p->PS_translate(ite->Xpos, Doc->PageH - ite->Ypos);
								if (ite->Rot != 0)
									p->PS_rotate(-ite->Rot);
								if ((ite->Pcolor != "None") || (ite->GrType != 0))
								{
									SetClipPath(p, &ite->PoLine);
									p->PS_closepath();
									p->PS_fill();
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
									if ((hl->ch == QChar(13)) || (hl->ch == QChar(10)) || (hl->ch == QChar(9)) || (hl->ch == QChar(28)))
										continue;
									if (hl->cstyle & 256)
										continue;
									if (hl->yp == 0)
										continue;
									tsz = hl->csize;
									chx = hl->ch;
									if (hl->ch == QChar(29))
										chx = " ";
									if (hl->ch == QChar(0xA0))
										chx = " ";
									if (hl->ch == QChar(30))
									{
										if (Doc->MasterP)
											chx = "#";
										else
										{
											uint zae = 0;
											uint za2 = d;
											do
											{
												if (za2 == 0)
													break;
												za2--;
											}
											while (ite->Ptext.at(za2)->ch == QChar(30));
											if (ite->Ptext.at(za2)->ch != QChar(30))
												za2++;
											while (ite->Ptext.at(za2+zae)->ch == QChar(30))
											{
												zae++;
												if (za2+zae == ite->MaxChars)
													break;
											}
											QString out="%1";
											QString out2;
											out2 = out.arg(a+Doc->FirstPnum, -zae);
											chx = out2.mid(d-za2, 1);
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
									/* Subset all TTF Fonts until the bug in the TTF-Embedding Code is fixed */
									QFileInfo fd = QFileInfo((*Doc->AllFonts)[hl->cfont]->Datei);
									QString fext = fd.extension(false).lower();
									if ((fext == "ttf") || ((*Doc->AllFonts)[hl->cfont]->isOTF) || ((*Doc->AllFonts)[hl->cfont]->Subset))
									{
										uint chr = chx[0].unicode();
										if (((*Doc->AllFonts)[hl->cfont]->CharWidth.contains(chr)) && (chr != 32))
										{
											p->PS_save();
											if (ite->Reverse)
											{
												p->PS_translate(hl->xp, (hl->yp - (tsz / 10.0)) * -1);
												p->PS_scale(-1, 1);
												if (d < ite->MaxChars-1)
												{
													QString ctx = ite->Ptext.at(d+1)->ch;
													if (ctx == QChar(29))
														ctx = " ";
													if (ctx == QChar(0xA0))
														ctx = " ";
													wideR = -Cwidth(Doc, hl->cfont, chx, tsz, ctx) * (hl->cscale / 100.0);
												}
												else
													wideR = -Cwidth(Doc, hl->cfont, chx, tsz) * (hl->cscale / 100.0);
												p->PS_translate(wideR, 0);
											}
											else
												p->PS_translate(hl->xp, (hl->yp - (tsz / 10.0)) * -1);
											if (hl->cscale != 100)
												p->PS_scale(hl->cscale / 100.0, 1);
											if (hl->ccolor != "None")
											{
												SetFarbe(hl->ccolor, hl->cshade, &h, &s, &v, &k, gcr);
												p->PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
												p->PS_showSub(chr, (*Doc->AllFonts)[hl->cfont]->RealName().simplifyWhiteSpace().replace( QRegExp("\\s"), "" ), tsz / 10.0, false);
											}
											p->PS_restore();
										}
									}
									else
									{
										p->PS_selectfont(hl->cfont, tsz / 10.0);
										if (hl->ccolor != "None")
										{
											SetFarbe(hl->ccolor, hl->cshade, &h, &s, &v, &k, gcr);
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
												if (ctx == QChar(0xA0))
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
											chma.scale(tsz / 100.0, tsz / 100.0);
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
												p->PS_setlinewidth(QMAX((*Doc->AllFonts)[hl->cfont]->strokeWidth / 2 * (tsz / 10.0), 1));
												p->PS_setcapjoin(FlatCap, MiterJoin);
												p->PS_setdash(SolidLine, 0, dum);
												p->PS_translate(hl->xp, (hl->yp - tsz) * -1);
												SetFarbe(hl->cstroke, hl->cshade2, &h, &s, &v, &k, gcr);
												p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
												SetClipPath(p, &gly);
												p->PS_closepath();
												p->PS_stroke();
												p->PS_restore();
											}
										}
									}
									if ((hl->cstyle & 16) && (chx != QChar(13)))
									{
										double Ulen = Cwidth(Doc, hl->cfont, chx, hl->csize) * (hl->cscale / 100.0);
										double Upos = (*Doc->AllFonts)[hl->cfont]->strikeout_pos * (tsz / 10.0);
										if (hl->ccolor != "None")
										{
											p->PS_setcapjoin(FlatCap, MiterJoin);
											p->PS_setdash(SolidLine, 0, dum);
											SetFarbe(hl->ccolor, hl->cshade, &h, &s, &v, &k, gcr);
											p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
										}
										p->PS_setlinewidth((*Doc->AllFonts)[hl->cfont]->strokeWidth * (tsz / 10.0));
										p->PS_moveto(hl->xp, -hl->yp+Upos);
										p->PS_lineto(hl->xp+Ulen, -hl->yp+Upos);
										p->PS_stroke();
									}
									if ((hl->cstyle & 8) && (chx != QChar(13)))
									{
										double Ulen = Cwidth(Doc, hl->cfont, chx, hl->csize) * (hl->cscale / 100.0);
										double Upos = (*Doc->AllFonts)[hl->cfont]->underline_pos * (tsz / 10.0);
										if (hl->ccolor != "None")
										{
											p->PS_setcapjoin(FlatCap, MiterJoin);
											p->PS_setdash(SolidLine, 0, dum);
											SetFarbe(hl->ccolor, hl->cshade, &h, &s, &v, &k, gcr);
											p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
										}
										p->PS_setlinewidth((*Doc->AllFonts)[hl->cfont]->strokeWidth * (tsz / 10.0));
										p->PS_moveto(hl->xp, -hl->yp+Upos);
										p->PS_lineto(hl->xp+Ulen, -hl->yp+Upos);
										p->PS_stroke();
									}
									if (hl->cstyle & 512)
									{
										int chs = hl->csize;
										ite->SetZeichAttr(hl, &chs, &chx);
										double wide = Cwidth(Doc, hl->cfont, chx, chs);
										chx = "-";
										uint chr = chx[0].unicode();
										if ((*Doc->AllFonts)[hl->cfont]->CharWidth.contains(chr))
										{
											FPointArray gly = (*Doc->AllFonts)[hl->cfont]->GlyphArray[chr].Outlines.copy();
											QWMatrix chma;
											chma.scale(tsz / 100.0, tsz / 100.0);
											gly.map(chma);
											chma = QWMatrix();
											chma.scale(hl->cscale / 100.0, 1);
											gly.map(chma);
											if (hl->ccolor != "None")
											{
												p->PS_save();
												p->PS_newpath();
												p->PS_translate(hl->xp+wide, (hl->yp - (tsz / 10.0)) * -1);
												SetFarbe(hl->ccolor, hl->cshade, &h, &s, &v, &k, gcr);
												p->PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
												SetClipPath(p, &gly);
												p->PS_closepath();
												p->PS_fill();
												p->PS_restore();
											}
										}
									}
								}
								if (((ite->Pcolor2 != "None") || (ite->NamedLStyle != "")) && (!ite->isTableItem))
								{
									if ((ite->NamedLStyle == "") && (ite->Pwidth != 0.0))
									{
										SetFarbe(ite->Pcolor2, ite->Shade2, &h, &s, &v, &k, gcr);
										p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
										p->PS_setlinewidth(ite->Pwidth);
										p->PS_setcapjoin(ite->PLineEnd, ite->PLineJoin);
										p->PS_setdash(ite->PLineArt, ite->DashOffset, ite->DashValues);
										SetClipPath(p, &ite->PoLine);
										p->PS_closepath();
										p->PS_stroke();
									}
									else
									{
										multiLine ml = Doc->MLineStyles[ite->NamedLStyle];
										for (int it = ml.size()-1; it > -1; it--)
										{
											SetFarbe(ml[it].Color, ml[it].Shade, &h, &s, &v, &k, gcr);
											p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
											p->PS_setlinewidth(ml[it].Width);
											p->PS_setcapjoin(static_cast<PenCapStyle>(ml[it].LineEnd), static_cast<PenJoinStyle>(ml[it].LineJoin));
											p->PS_setdash(static_cast<PenStyle>(ml[it].Dash), 0, dum);
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
					for (uint am = 0; am < mPage->Items.count(); ++am)
					{
						PageItem *ite = mPage->Items.at(am);
						if ((ite->LayerNr != ll.LNr) || (!ll.Drucken))
							continue;
						if (ite->isPrintable)
						{
							if (ite->isTableItem)
							{
								p->PS_save();
								if (ite->Pcolor2 != "None")
								{
									SetFarbe(ite->Pcolor2, ite->Shade2, &h, &s, &v, &k, gcr);
									p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
								}
								p->PS_setlinewidth(ite->Pwidth);
								p->PS_setcapjoin(ite->PLineEnd, ite->PLineJoin);
								p->PS_setdash(ite->PLineArt, ite->DashOffset, ite->DashValues);
								p->PS_translate(ite->Xpos, Doc->PageH - ite->Ypos);
								if (ite->Rot != 0)
								p->PS_rotate(-ite->Rot);
								if ((ite->TopLine) || (ite->RightLine) || (ite->BottomLine) || (ite->LeftLine))
								{
									if (ite->TopLine)
									{
										p->PS_moveto(0, 0);
										p->PS_lineto(ite->Width, 0);
									}
									if (ite->RightLine)
									{
										p->PS_moveto(ite->Width, 0);
										p->PS_lineto(ite->Width, -ite->Height);
									}
									if (ite->BottomLine)
									{
										p->PS_moveto(0, -ite->Height);
										p->PS_lineto(ite->Width, -ite->Height);
									}
									if (ite->LeftLine)
									{
										p->PS_moveto(0, 0);
										p->PS_lineto(0, -ite->Height);
									}
									p->PS_stroke();
								}
								p->PS_restore();
							}
						}
					}
					Lnr++;
				}
			}
		}
		ProcessPage(p, Pages.at(a), a+1, sep, farb, Ic, gcr);
		p->PS_end_page();
		if (sep)
		{
			if (SepNam != tr("All"))
				aa++;
			else
			{
				if (sepac == 3)
				{
					aa++;
					sepac = 0;
				}
				else
					sepac++;
			}
		}
		else
			aa++;
	}
	p->PS_close();
}

void ScribusView::ProcessPage(PSLib *p, Page* a, uint PNr, bool sep, bool farb, bool ic, bool gcr)
{
	uint b, d;
	int h, s, v, k, tsz;
	double wideR;
	QValueList<double> dum;
	dum.clear();
	QCString chxc;
	QString chx, chglyph, tmp;
	PageItem *c;
	struct Pti *hl;
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
				if (c->LayerNr != ll.LNr)
					continue;
				if ((a->PageNam != "") && (c->PType == 4))
					continue;
				if ((a->PageNam != "") && (c->PType == 2) && ((sep) || (!farb)))
					continue;
				if ((!p->Art) && (Doc->ActPage->SelItem.count() != 0) && (!c->Select))
					continue;
				if (c->isPrintable)
				{
					p->PS_save();
					if (c->Pcolor != "None")
					{
						SetFarbe(c->Pcolor, c->Shade, &h, &s, &v, &k, gcr);
						p->PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
					}
					if (c->Pcolor2 != "None")
					{
						SetFarbe(c->Pcolor2, c->Shade2, &h, &s, &v, &k, gcr);
						p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
					}
					p->PS_setlinewidth(c->Pwidth);
					p->PS_setcapjoin(c->PLineEnd, c->PLineJoin);
					p->PS_setdash(c->PLineArt, c->DashOffset, c->DashValues);
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
								HandleGradient(p, c, c->Width, c->Height, gcr);
							else
								p->PS_fill();
							p->PS_newpath();
						}
						p->PS_save();
						SetClipPath(p, &c->PoLine);
						p->PS_closepath();
						p->PS_clip(true);
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
						if (((c->Pcolor2 != "None") || (c->NamedLStyle != "")) && (!c->isTableItem))
						{
							if ((c->NamedLStyle == "") && (c->Pwidth != 0.0))
							{
								SetFarbe(c->Pcolor2, c->Shade2, &h, &s, &v, &k, gcr);
								p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
								p->PS_setlinewidth(c->Pwidth);
								p->PS_setcapjoin(c->PLineEnd, c->PLineJoin);
								p->PS_setdash(c->PLineArt, c->DashOffset, c->DashValues);
								SetClipPath(p, &c->PoLine);
								p->PS_closepath();
								p->PS_stroke();
							}
							else
							{
								multiLine ml = Doc->MLineStyles[c->NamedLStyle];
								for (int it = ml.size()-1; it > -1; it--)
								{
									SetFarbe(ml[it].Color, ml[it].Shade, &h, &s, &v, &k, gcr);
									p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
									p->PS_setlinewidth(ml[it].Width);
									p->PS_setcapjoin(static_cast<PenCapStyle>(ml[it].LineEnd), static_cast<PenJoinStyle>(ml[it].LineJoin));
									p->PS_setdash(static_cast<PenStyle>(ml[it].Dash), 0, dum);
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
								if ((c->Ptext.at(d)->ch == QChar(13)) || (c->Ptext.at(d)->ch == QChar(10)) || (c->Ptext.at(d)->ch == QChar(28)))
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
								HandleGradient(p, c, c->Width, c->Height, gcr);
							else
								p->PS_fill();
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
							if ((hl->ch == QChar(13)) || (hl->ch == QChar(10)) || (hl->ch == QChar(9)) || (hl->ch == QChar(28)))
								continue;
							if (hl->cstyle & 256)
								continue;
							if (hl->yp == 0)
								continue;
							tsz = hl->csize;
							chx = hl->ch;
							if (hl->ch == QChar(29))
								chx = " ";
							if (hl->ch == QChar(0xA0))
								chx = " ";
							if (hl->ch == QChar(30))
							{
								if (Doc->MasterP)
									chx = "#";
								else
								{
									uint zae = 0;
									uint za2 = d;
									do
									{
										if (za2 == 0)
											break;
										za2--;
									}
									while (c->Ptext.at(za2)->ch == QChar(30));
									if (c->Ptext.at(za2)->ch != QChar(30))
										za2++;
									while (c->Ptext.at(za2+zae)->ch == QChar(30))
									{
										zae++;
										if (za2+zae == c->MaxChars)
											break;
									}
									QString out="%1";
									QString out2;
									out2 = out.arg(PNr-1+Doc->FirstPnum, -zae);
									chx = out2.mid(d-za2, 1);
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
							/* Subset all TTF Fonts until the bug in the TTF-Embedding Code is fixed */
							QFileInfo fd = QFileInfo((*Doc->AllFonts)[hl->cfont]->Datei);
							QString fext = fd.extension(false).lower();
							if ((fext == "ttf") || ((*Doc->AllFonts)[hl->cfont]->isOTF) || ((*Doc->AllFonts)[hl->cfont]->Subset))
							{
								uint chr = chx[0].unicode();
								if (((*Doc->AllFonts)[hl->cfont]->CharWidth.contains(chr)) && (chr != 32))
								{
									p->PS_save();
									if (c->Reverse)
									{
										p->PS_translate(hl->xp, (hl->yp - (tsz / 10.0)) * -1);
										p->PS_scale(-1, 1);
										if (d < c->MaxChars-1)
										{
											QString ctx = c->Ptext.at(d+1)->ch;
											if (ctx == QChar(29))
												ctx = " ";
											if (ctx == QChar(0xA0))
												ctx = " ";
											wideR = -Cwidth(Doc, hl->cfont, chx, tsz, ctx) * (hl->cscale / 100.0);
										}
										else
											wideR = -Cwidth(Doc, hl->cfont, chx, tsz) * (hl->cscale / 100.0);
										p->PS_translate(wideR, 0);
									}
									else
										p->PS_translate(hl->xp, (hl->yp - (tsz / 10.0)) * -1);
									if (hl->cscale != 100)
										p->PS_scale(hl->cscale / 100.0, 1);
									if (hl->ccolor != "None")
									{
										SetFarbe(hl->ccolor, hl->cshade, &h, &s, &v, &k, gcr);
										p->PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
										p->PS_showSub(chr, (*Doc->AllFonts)[hl->cfont]->RealName().simplifyWhiteSpace().replace( QRegExp("\\s"), "" ), tsz / 10.0, false);
									}
									p->PS_restore();
								}
							}
							else
							{
								p->PS_selectfont(hl->cfont, tsz / 10.0);
								if (hl->ccolor != "None")
								{
									SetFarbe(hl->ccolor, hl->cshade, &h, &s, &v, &k, gcr);
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
										if (ctx == QChar(0xA0))
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
									chma.scale(tsz / 100.0, tsz / 100.0);
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
										p->PS_setlinewidth(QMAX((*Doc->AllFonts)[hl->cfont]->strokeWidth / 2 * (tsz / 10.0), 1));
										p->PS_setcapjoin(FlatCap, MiterJoin);
										p->PS_setdash(SolidLine, 0, dum);
										p->PS_translate(hl->xp, (hl->yp - (tsz / 10.0)) * -1);
										SetFarbe(hl->cstroke, hl->cshade2, &h, &s, &v, &k, gcr);
										p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
										SetClipPath(p, &gly);
										p->PS_closepath();
										p->PS_stroke();
										p->PS_restore();
									}
								}
							}
							if ((hl->cstyle & 16) && (chx != QChar(13)))
							{
								double Ulen = Cwidth(Doc, hl->cfont, chx, hl->csize) * (hl->cscale / 100.0);
								double Upos = (*Doc->AllFonts)[hl->cfont]->strikeout_pos * (tsz / 10.0);
								if (hl->ccolor != "None")
								{
									p->PS_setcapjoin(FlatCap, MiterJoin);
									p->PS_setdash(SolidLine, 0, dum);
									SetFarbe(hl->ccolor, hl->cshade, &h, &s, &v, &k, gcr);
									p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
								}
								p->PS_setlinewidth((*Doc->AllFonts)[hl->cfont]->strokeWidth * (tsz / 10.0));
								p->PS_moveto(hl->xp, -hl->yp+Upos);
								p->PS_lineto(hl->xp+Ulen, -hl->yp+Upos);
								p->PS_stroke();
							}
							if ((hl->cstyle & 8) && (chx != QChar(13)))
							{
								double Ulen = Cwidth(Doc, hl->cfont, chx, hl->csize) * (hl->cscale / 100.0);
								double Upos = (*Doc->AllFonts)[hl->cfont]->underline_pos * (tsz / 10.0);
								if (hl->ccolor != "None")
								{
									p->PS_setcapjoin(FlatCap, MiterJoin);
									p->PS_setdash(SolidLine, 0, dum);
									SetFarbe(hl->ccolor, hl->cshade, &h, &s, &v, &k, gcr);
									p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
								}
								p->PS_setlinewidth((*Doc->AllFonts)[hl->cfont]->strokeWidth * (tsz / 10.0));
								p->PS_moveto(hl->xp, -hl->yp+Upos);
								p->PS_lineto(hl->xp+Ulen, -hl->yp+Upos);
								p->PS_stroke();
							}
							if (hl->cstyle & 512)
							{
								int chs = hl->csize;
								c->SetZeichAttr(hl, &chs, &chx);
								double wide = Cwidth(Doc, hl->cfont, chx, chs);
								chx = "-";
								uint chr = chx[0].unicode();
								if ((*Doc->AllFonts)[hl->cfont]->CharWidth.contains(chr))
								{
									FPointArray gly = (*Doc->AllFonts)[hl->cfont]->GlyphArray[chr].Outlines.copy();
									QWMatrix chma;
									chma.scale(tsz / 100.0, tsz / 100.0);
									gly.map(chma);
									chma = QWMatrix();
									chma.scale(hl->cscale / 100.0, 1);
									gly.map(chma);
									if (hl->ccolor != "None")
									{
										p->PS_save();
										p->PS_newpath();
										p->PS_translate(hl->xp+wide, (hl->yp - (tsz / 10.0)) * -1);
										SetFarbe(hl->ccolor, hl->cshade, &h, &s, &v, &k, gcr);
										p->PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
										SetClipPath(p, &gly);
										p->PS_closepath();
										p->PS_fill();
										p->PS_restore();
									}
								}
							}
						}
						if (((c->Pcolor2 != "None") || (c->NamedLStyle != "")) && (!c->isTableItem))
						{
							SetFarbe(c->Pcolor2, c->Shade2, &h, &s, &v, &k, gcr);
							p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
							p->PS_setlinewidth(c->Pwidth);
							p->PS_setcapjoin(c->PLineEnd, c->PLineJoin);
							p->PS_setdash(c->PLineArt, c->DashOffset, c->DashValues);
							if ((c->NamedLStyle == "") && (c->Pwidth != 0.0))
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
									SetFarbe(ml[it].Color, ml[it].Shade, &h, &s, &v, &k, gcr);
									p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
									p->PS_setlinewidth(ml[it].Width);
									p->PS_setcapjoin(static_cast<PenCapStyle>(ml[it].LineEnd), static_cast<PenJoinStyle>(ml[it].LineJoin));
									p->PS_setdash(static_cast<PenStyle>(ml[it].Dash), 0, dum);
									SetClipPath(p, &c->PoLine);
									p->PS_closepath();
									p->PS_stroke();
								}
							}
						}
						break;
					case 5:
						if ((c->NamedLStyle == "") && (c->Pwidth != 0.0))
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
								SetFarbe(ml[it].Color, ml[it].Shade, &h, &s, &v, &k, gcr);
								p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
								p->PS_setlinewidth(ml[it].Width);
								p->PS_setcapjoin(static_cast<PenCapStyle>(ml[it].LineEnd), static_cast<PenJoinStyle>(ml[it].LineJoin));
								p->PS_setdash(static_cast<PenStyle>(ml[it].Dash), 0, dum);
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
								HandleGradient(p, c, c->Width, c->Height, gcr);
							else
								p->PS_fill();
						}
						if ((c->Pcolor2 != "None") || (c->NamedLStyle != ""))
						{
							if ((c->NamedLStyle == "") && (c->Pwidth != 0.0))
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
									SetFarbe(ml[it].Color, ml[it].Shade, &h, &s, &v, &k, gcr);
									p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
									p->PS_setlinewidth(ml[it].Width);
									p->PS_setcapjoin(static_cast<PenCapStyle>(ml[it].LineEnd), static_cast<PenJoinStyle>(ml[it].LineJoin));
									p->PS_setdash(static_cast<PenStyle>(ml[it].Dash), 0, dum);
									SetClipPath(p, &c->PoLine);
									p->PS_closepath();
									p->PS_stroke();
								}
							}
						}
						break;
					case 7:
						if ((c->Pcolor != "None") || (c->GrType != 0))
						{
							SetClipPath(p, &c->PoLine);
							p->PS_closepath();
							if (c->GrType != 0)
								HandleGradient(p, c, c->Width, c->Height, gcr);
							else
								p->PS_fill();
							p->PS_newpath();
						}
						if ((c->NamedLStyle == "") && (c->Pwidth != 0.0))
						{
							SetClipPath(p, &c->PoLine, false);
							p->PS_stroke();
						}
						else
						{
							multiLine ml = Doc->MLineStyles[c->NamedLStyle];
							for (int it = ml.size()-1; it > -1; it--)
							{
								SetFarbe(ml[it].Color, ml[it].Shade, &h, &s, &v, &k, gcr);
								p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
								p->PS_setlinewidth(ml[it].Width);
								p->PS_setcapjoin(static_cast<PenCapStyle>(ml[it].LineEnd), static_cast<PenJoinStyle>(ml[it].LineJoin));
								p->PS_setdash(static_cast<PenStyle>(ml[it].Dash), 0, dum);
								SetClipPath(p, &c->PoLine, false);
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
								if ((c->NamedLStyle == "") && (c->Pwidth != 0.0))
								{
									SetClipPath(p, &c->PoLine, false);
									p->PS_stroke();
								}
								else
								{
									multiLine ml = Doc->MLineStyles[c->NamedLStyle];
									for (int it = ml.size()-1; it > -1; it--)
									{
										SetFarbe(ml[it].Color, ml[it].Shade, &h, &s, &v, &k, gcr);
										p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
										p->PS_setlinewidth(ml[it].Width);
										p->PS_setcapjoin(static_cast<PenCapStyle>(ml[it].LineEnd), static_cast<PenJoinStyle>(ml[it].LineJoin));
										p->PS_setdash(static_cast<PenStyle>(ml[it].Dash), 0, dum);
										SetClipPath(p, &c->PoLine, false);
										p->PS_stroke();
									}
								}
								p->PS_restore();
							}
						}
						for (d = 0; d < c->MaxChars; ++d)
						{
							hl = c->Ptext.at(d);
							if ((hl->ch == QChar(13)) || (hl->ch == QChar(30)) || (hl->ch == QChar(9)) || (hl->ch == QChar(28)))
								continue;
							tsz = hl->csize;
							chx = hl->ch;
							if (hl->ch == QChar(29))
								chx = " ";
							if (hl->ch == QChar(0xA0))
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
								SetFarbe(hl->ccolor, hl->cshade, &h, &s, &v, &k, gcr);
								p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
							}
							/* Subset all TTF Fonts until the bug in the TTF-Embedding Code is fixed */
							QFileInfo fd = QFileInfo((*Doc->AllFonts)[hl->cfont]->Datei);
							QString fext = fd.extension(false).lower();
							if ((fext == "ttf") || ((*Doc->AllFonts)[hl->cfont]->isOTF) || ((*Doc->AllFonts)[hl->cfont]->Subset))
							{
								uint chr = chx[0].unicode();
								if (((*Doc->AllFonts)[hl->cfont]->CharWidth.contains(chr)) && (chr != 32))
								{
									p->PS_save();
/*									p->PS_translate(hl->PtransX, -hl->PtransY);
									p->PS_rotate(-hl->PRot);
									if (c->Reverse)
									{
										p->PS_translate(hl->xp, (hl->yp - (tsz / 10.0)) * -1);
										p->PS_scale(-1, 1);
										if (d < c->MaxChars-1)
										{
											QString ctx = c->Ptext.at(d+1)->ch;
											if (ctx == QChar(29))
												ctx = " ";
											if (ctx == QChar(0xA0))
												ctx = " ";
											wideR = -Cwidth(Doc, hl->cfont, chx, tsz, ctx) * (hl->cscale / 100.0);
										}
										else
											wideR = -Cwidth(Doc, hl->cfont, chx, tsz) * (hl->cscale / 100.0);
										p->PS_translate(wideR, 0);
									}
									else
										p->PS_translate(hl->xp, (hl->yp - (tsz / 10.0)) * -1); */
									if (hl->cscale != 100)
										p->PS_scale(hl->cscale / 100.0, 1);
									if (hl->ccolor != "None")
									{
										SetFarbe(hl->ccolor, hl->cshade, &h, &s, &v, &k, gcr);
										p->PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
										p->PutSeite("[ 1 0 0 -1 " + p->ToStr(-hl->PRot) + " 0 ]\n");
										p->PutSeite("["+p->ToStr(hl->PtransX) + " " + p->ToStr(-hl->PtransY) + " " + p->ToStr(-hl->PtransY) + " " + p->ToStr(-hl->PtransX) + " " + p->ToStr(hl->xp) + " " + p->ToStr(-hl->yp) + "]\n");
										p->PutSeite("[ 0 0 0 0 0 0 ] concatmatrix\nconcat\n");
										p->PS_translate(0, (tsz / 10.0));
										if (c->BaseOffs != 0)
											p->PS_translate(0, -c->BaseOffs);
										p->PS_showSub(chr, (*Doc->AllFonts)[hl->cfont]->RealName().simplifyWhiteSpace().replace( QRegExp("\\s"), "" ), tsz / 10.0, false);
									}
									p->PS_restore();
								}
							}
							else
							{
								p->PS_selectfont(hl->cfont, tsz / 10.0);
								p->PS_save();
/*								p->PS_translate(hl->PtransX, -hl->PtransY);
								p->PS_rotate(-hl->PRot); */
								p->PutSeite("[ 1 0 0 -1 " + p->ToStr(-hl->PRot) + " 0 ]\n");
								p->PutSeite("["+p->ToStr(hl->PtransX) + " " + p->ToStr(-hl->PtransY) + " " + p->ToStr(-hl->PtransY) + " " + p->ToStr(-hl->PtransX) + " " + p->ToStr(hl->xp) + " " + p->ToStr(-hl->yp) + "]\n");
								p->PutSeite("[ 0 0 0 0 0 0 ] concatmatrix\nconcat\n");
								if (c->BaseOffs != 0)
									p->PS_translate(0, -c->BaseOffs);
								p->PS_show_xyG(hl->cfont, chx, 0, 0);
								p->PS_restore();
							}
						}
						break;
					}
					p->PS_restore();
				}
			}
		}
		for (b = 0; b < a->Items.count(); ++b)
		{
			c = a->Items.at(b);
			if ((c->LayerNr != ll.LNr) || (!ll.Drucken))
				continue;
			if ((a->PageNam != "") && (c->PType == 4))
				continue;
			if ((a->PageNam != "") && (c->PType == 2) && ((sep) || (!farb)))
				continue;
			if (c->isPrintable)
			{
				if (c->isTableItem)
				{
					p->PS_save();
					if (c->Pcolor2 != "None")
					{
						SetFarbe(c->Pcolor2, c->Shade2, &h, &s, &v, &k, gcr);
						p->PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
					}
					p->PS_setlinewidth(c->Pwidth);
					p->PS_setcapjoin(c->PLineEnd, c->PLineJoin);
					p->PS_setdash(c->PLineArt, c->DashOffset, c->DashValues);
					p->PS_translate(c->Xpos, Doc->PageH - c->Ypos);
					if (c->Rot != 0)
						p->PS_rotate(-c->Rot);
					if ((c->TopLine) || (c->RightLine) || (c->BottomLine) || (c->LeftLine))
					{
						if (c->TopLine)
						{
							p->PS_moveto(0, 0);
							p->PS_lineto(c->Width, 0);
						}
						if (c->RightLine)
						{
							p->PS_moveto(c->Width, 0);
							p->PS_lineto(c->Width, -c->Height);
						}
						if (c->BottomLine)
						{
							p->PS_moveto(0, -c->Height);
							p->PS_lineto(c->Width, -c->Height);
						}
						if (c->LeftLine)
						{
							p->PS_moveto(0, 0);
							p->PS_lineto(0, -c->Height);
						}
						p->PS_stroke();
					}
					p->PS_restore();
				}
			}
		}
		Lnr++;
	}
}

void ScribusView::HandleGradient(PSLib *p, PageItem *c, double w, double h, bool gcr)
{
	int ch,cs,cv,ck;
	double StartX, StartY, EndX, EndY;
	QPtrVector<VColorStop> cstops = c->fill_gradient.colorStops();
	switch (c->GrType)
	{
		case 1:
			StartX = 0;
			StartY = h / 2.0;
			EndX = w;
			EndY = h / 2.0;
			break;
		case 2:
			StartX = w / 2.0;
			StartY = 0;
			EndX = w / 2.0;
			EndY = h;
			break;
		case 3:
			StartX = 0;
			StartY = 0;
			EndX = w;
			EndY = h;
			break;
		case 4:
			StartX = 0;
			StartY = h;
			EndX = w;
			EndY = 0;
			break;
		case 5:
			StartX = w / 2.0;
			StartY = h / 2.0;
			if (w >= h)
			{
				EndX = w;
				EndY = h / 2.0;
			}
			else
			{
				EndX = w / 2.0;
				EndY = h;
			}
			break;
		case 6:
		case 7:
			StartX = c->GrStartX;
			StartY = c->GrStartY;
			EndX = c->GrEndX;
			EndY = c->GrEndY;
			break;
	}
	QValueList<double> StopVec;
	QStringList Gcolors;
	QString hs,ss,vs,ks;
	if ((c->GrType == 5) || (c->GrType == 7))
	{
		StopVec.clear();
		for (uint cst = 0; cst < c->fill_gradient.Stops(); ++cst)
		{
			StopVec.prepend(sqrt(pow(EndX - StartX, 2) + pow(EndY - StartY,2))*cstops.at(cst)->rampPoint);
			SetFarbe(cstops.at(cst)->name, cstops.at(cst)->shade, &ch, &cs, &cv, &ck, gcr);
			QString GCol = hs.setNum(ch / 255.0)+" "+ss.setNum(cs / 255.0)+" "+vs.setNum(cv / 255.0)+" "+ks.setNum(ck / 255.0);
			Gcolors.prepend(GCol);
		}
		p->PS_MultiRadGradient(w, -h, StartX, -StartY, StopVec, Gcolors);
	}
	else
	{
		StopVec.clear();
		for (uint cst = 0; cst < c->fill_gradient.Stops(); ++cst)
		{
			QWMatrix ma;
			ma.translate(StartX, StartY);
			ma.rotate(atan2(EndY - StartY, EndX - StartX)*(180.0/3.1415927));
			double w2 = sqrt(pow(EndX - StartX, 2) + pow(EndY - StartY,2))*cstops.at(cst)->rampPoint;
			double x = fabs(ma.m11() * w2 + ma.dx());
			double y = fabs(ma.m12() * w2 + ma.dy());
			StopVec.append(x);
			StopVec.append(-y);
			SetFarbe(cstops.at(cst)->name, cstops.at(cst)->shade, &ch, &cs, &cv, &ck, gcr);
			QString GCol = hs.setNum(ch / 255.0)+" "+ss.setNum(cs / 255.0)+" "+vs.setNum(cv / 255.0)+" "+ks.setNum(ck / 255.0);
			Gcolors.append(GCol);
		}
		p->PS_MultiLinGradient(w, -h, StopVec, Gcolors);
	}
}

void ScribusView::SetFarbe(QString farb, int shade, int *h, int *s, int *v, int *k, bool gcr)
{
	int h1, s1, v1, k1;
	h1 = *h;
	s1 = *s;
	v1 = *v;
	k1 = *k;
	CMYKColor tmp = Doc->PageColors[farb];
	if (gcr)
		tmp.applyGCR();
	tmp.getCMYK(&h1, &s1, &v1, &k1);
	*h = h1 * shade / 100;
	*s = s1 * shade / 100;
	*v = v1 * shade / 100;
	*k = k1 * shade / 100;
}

void ScribusView::SetClipPath(PSLib *p, FPointArray *c, bool poly)
{
	FPoint np, np1, np2;
	bool nPath = true;
	if (c->size() > 3)
	{
		for (uint poi=0; poi<c->size()-3; poi += 4)
		{
			if (c->point(poi).x() > 900000)
			{
				if (poly)
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
		if ((w->orientation() != Qt::Vertical) || ( w->state() & ShiftButton ))
		{
			if (w->delta() < 0)
				scrollBy(Prefs->Wheelval, 0);
			else
				scrollBy(-Prefs->Wheelval, 0);
		}
		else
		{
			if (w->delta() < 0)
				scrollBy(0, Prefs->Wheelval);
			else
				scrollBy(0, -Prefs->Wheelval);
		}
	}
	w->accept();
}
