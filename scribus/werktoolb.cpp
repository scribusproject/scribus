/***************************************************************************
                          texttoolb.cpp  -  description
                             -------------------
    begin                : Sun Mar 10 2002
    copyright            : (C) 2002 by Franz Schmid
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

#include "werktoolb.h"
#include "werktoolb.moc"
#include "polyprops.h"

extern int PolyC;
extern int PolyFd;
extern float PolyF;
extern bool PolyS;
extern float PolyR;
extern QPixmap loadIcon(QString nam);

WerkToolB::WerkToolB(QMainWindow* parent) : QToolBar(tr("Tools"), parent)
{
	Select = new QToolButton(loadIcon("Kreuz.xpm"), tr("Select Items"), QString::null, this, SLOT(ModeFromTB()), this);
	Select->setToggleButton(true);
	Select->setOn(true);
	Rotiere = new QToolButton(loadIcon("Rotieren.xpm"), tr("Rotate Item"), QString::null, this, SLOT(ModeFromTB()), this);
	Rotiere->setToggleButton( true );
	Rotiere->setEnabled(false);
	Textedit = new QToolButton(loadIcon("Editm.xpm"),tr("Edit Contents of Frame"), QString::null, this, SLOT(ModeFromTB()), this);
	Textedit->setToggleButton( true );
	Textedit->setEnabled( FALSE );
	Zoom = new QToolButton(loadIcon("Lupe.xpm"),tr("Zooms in or out"), QString::null, this, SLOT(ModeFromTB()), this);
	Zoom->setToggleButton( true );
	Texte = new QToolButton(loadIcon("Text.xpm"),tr("Insert Textbox"), QString::null, this, SLOT(ModeFromTB()), this);
	Texte->setToggleButton( true );
	BildB = new QToolButton(loadIcon("Bild.xpm"),tr("Insert Picture"), QString::null, this, SLOT(ModeFromTB()), this);
	BildB->setToggleButton( true );
	Rechteck = new QToolButton(loadIcon("Rechtecke.xpm"),tr("Insert Rectangles"), QString::null, this, SLOT(ModeFromTB()), this);
	Rechteck->setToggleButton( true );
	Kreis = new QToolButton(loadIcon("Kreise.xpm"),tr("Insert Ovals"), QString::null, this, SLOT(ModeFromTB()), this);
	Kreis->setToggleButton( true );
	PolyM = new QPopupMenu();
	PolyM->insertItem(tr("Properties..."), this, SLOT(GetPolyProps()));
	Polygon = new QToolButton(loadIcon("spline.png"),tr("Insert Polygons"), QString::null, this, SLOT(ModeFromTB()), this);
	Polygon->setToggleButton( true );
	Polygon->setPopup(PolyM);
	Polygon->setPopupDelay(0);
	Linien = new QToolButton(loadIcon("Stift.xpm"),tr("Insert Lines"), QString::null, this, SLOT(ModeFromTB()), this);
	Linien->setToggleButton( true );
	PolyLin = new QToolButton(loadIcon("beziertool.png"),tr("Insert Bezier Curves"), QString::null, this, SLOT(ModeFromTB()), this);
	PolyLin->setToggleButton( true );
	KetteEin = new QToolButton(loadIcon("Lock.xpm"),tr("Create Textchains"), QString::null, this, SLOT(ModeFromTB()), this);
	KetteEin->setToggleButton( true );
	KetteEin->setEnabled(false);
	KetteAus = new QToolButton(loadIcon("Unlock.xpm"),tr("Break Textchains"), QString::null, this, SLOT(ModeFromTB()), this);
	KetteAus->setToggleButton( true );
	KetteAus->setEnabled(false);
	setCloseMode(QDockWindow::Undocked);
  connect(this, SIGNAL(placeChanged(QDockWindow::Place)), this, SLOT(Docken(QDockWindow::Place)));
  connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(Verbergen(bool)));
}

void WerkToolB::Docken(QDockWindow::Place p)
{
	if (p == InDock)
		setOrientation(Horizontal);
	else
		setOrientation(Vertical);
}

void WerkToolB::Verbergen(bool vis)
{
	if (!vis)
		emit Schliessen();
}

void WerkToolB::GetPolyProps()
{
	PolygonProps* dia = new PolygonProps(this);
  if (dia->exec())
  	{
		PolyC = dia->Ecken->value();
		PolyF = dia->PFactor;
		PolyS = dia->Konvex->isChecked();
		PolyFd = dia->Slider1->value();
		PolyR = dia->Faktor2->value();
		}
	delete dia;
}

void WerkToolB::ModeFromTB()
{
	Select->setOn(false);
	Rotiere->setOn(false);
	Textedit->setOn(false);
	Zoom->setOn(false);
	Texte->setOn(false);
	BildB->setOn(false);
	Rechteck->setOn(false);
	Kreis->setOn(false);
	Linien->setOn(false);
	KetteEin->setOn(false);
	KetteAus->setOn(false);
	Polygon->setOn(false);
	PolyLin->setOn(false);
	if (Select == sender())
		{
		Select->setOn(true);
		emit NewMode(1);
		}
	if (Rotiere == sender())
		{
		Rotiere->setOn(true);
		emit NewMode(9);
		}
	if (Textedit == sender())
		{
		Textedit->setOn(true);
		emit NewMode(7);
		}
	if (Zoom == sender())
		{
		Zoom->setOn(true);
		emit NewMode(6);
		}
	if (Texte == sender())
		{
		Texte->setOn(true);
		emit NewMode(5);
		}
	if (BildB == sender())
		{
		BildB->setOn(true);
		emit NewMode(4);
		}
	if (Rechteck == sender())
		{
		Rechteck->setOn(true);
		emit NewMode(2);
		}
	if (Kreis == sender())
		{
		Kreis->setOn(true);
		emit NewMode(3);
		}
	if (Linien == sender())
		{
		Linien->setOn(true);
		emit NewMode(8);
		}
	if (KetteEin == sender())
		{
		KetteEin->setOn(true);
		emit NewMode(10);
		}
	if (KetteAus == sender())
		{
		KetteAus->setOn(true);
		emit NewMode(11);
		}
	if (Polygon == sender())
		{
		Polygon->setOn(true);
		emit NewMode(12);
		}
	if (PolyLin == sender())
		{
		PolyLin->setOn(true);
		emit NewMode(13);
		}
}

WerkToolBP::WerkToolBP(QMainWindow* parent) : QToolBar(tr("PDF-Tools"), parent)
{
	PDFM = new QPopupMenu();
	PDFM->insertItem(loadIcon("pushbutton.png"), tr("Button"));
	PDFM->insertItem(loadIcon("textview.png"), tr("Text Field"));
	PDFM->insertItem(loadIcon("checkbox.png"), tr("Check Box"));
	PDFM->insertItem(loadIcon("combobox.png"), tr("Combo Box"));
	PDFM->insertItem(loadIcon("listbox.png"), tr("List Box"));
	PDFTool = new QToolButton(loadIcon("pushbutton.png"),tr("Insert PDF-Fields"), QString::null, this, SLOT(ModeFromTB()), this);
	PDFTool->setToggleButton(true);
	PDFTool->setPopup(PDFM);
	PDFTool->setPopupDelay(0);
	PDFwerkz = 0;
	PDFA = new QPopupMenu();
	PDFA->insertItem(loadIcon("charset.png"), tr("Text"));
	PDFA->insertItem(loadIcon("goto.png"), tr("Link"));
	PDFaTool = new QToolButton(loadIcon("charset.png"),tr("Insert PDF-Annotations"), QString::null, this, SLOT(ModeFromTB()), this);
	PDFaTool->setToggleButton(true);
	PDFaTool->setPopup(PDFA);
	PDFaTool->setPopupDelay(0);
	PDFnotiz = 0;
	setCloseMode(QDockWindow::Undocked);
  connect(this, SIGNAL(placeChanged(QDockWindow::Place)), this, SLOT(Docken(QDockWindow::Place)));
  connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(Verbergen(bool)));
	connect(PDFM, SIGNAL(activated(int)), this, SLOT(setPDFtool(int)));
	connect(PDFA, SIGNAL(activated(int)), this, SLOT(setPDFnotiz(int)));
}

void WerkToolBP::setPDFnotiz(int id)
{
	int c = PDFA->indexOf(id);
	PDFnotiz = c;
	switch (c)
		{
		case 0:
			PDFaTool->setPixmap(loadIcon("charset.png"));
			break;
		case 1:
			PDFaTool->setPixmap(loadIcon("goto.png"));
			break;
		}
	PDFTool->setOn(false);
	PDFaTool->setOn(true);
	emit NewMode(19+PDFnotiz);
}

void WerkToolBP::setPDFtool(int id)
{
	int c = PDFM->indexOf(id);
	PDFwerkz = c;
	switch (c)
		{
		case 0:
			PDFTool->setPixmap(loadIcon("pushbutton.png"));
			break;
		case 1:
			PDFTool->setPixmap(loadIcon("textview.png"));
			break;
		case 2:
			PDFTool->setPixmap(loadIcon("checkbox.png"));
			break;
		case 3:
			PDFTool->setPixmap(loadIcon("combobox.png"));
			break;
		case 4:
			PDFTool->setPixmap(loadIcon("listbox.png"));
			break;
		}
	PDFTool->setOn(true);
	PDFaTool->setOn(false);
	emit NewMode(14+PDFwerkz);
}

void WerkToolBP::Docken(QDockWindow::Place p)
{
	setOrientation(Horizontal);
}

void WerkToolBP::Verbergen(bool vis)
{
	if (!vis)
		emit Schliessen();
}

void WerkToolBP::ModeFromTB()
{
	PDFTool->setOn(false);
	PDFaTool->setOn(false);
	if (PDFTool == sender())
		{
		PDFTool->setOn(true);
		emit NewMode(14+PDFwerkz);
		}
	if (PDFaTool == sender())
		{
		PDFaTool->setOn(true);
		emit NewMode(19+PDFnotiz);
		}
}


