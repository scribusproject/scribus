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
#include "autoform.h"
#include "scribus.h"

extern QPixmap loadIcon(QString nam);
extern ScribusApp* ScApp;

WerkToolB::WerkToolB(QMainWindow* parent) : QToolBar( tr("Tools"), parent)
{
	SubMode = 0;
	ValCount = 32;
	static double AutoShapes0[] = {0.0, 0.0, 0.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0,
									100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 0.0, 100.0, 0.0, 100.0,
									0.0, 100.0, 0.0, 100.0, 0.0, 0.0, 0.0, 0.0};
	ShapeVals = AutoShapes0;
	Select = new QToolButton(loadIcon("Kreuz.xpm"), tr("Select Items"), QString::null, this, SLOT(ModeFromTB()), this);
	Select->setToggleButton(true);
	Select->setOn(true);
	Texte = new QToolButton(loadIcon("Text.xpm"), tr("Insert Text Frame"), QString::null, this, SLOT(ModeFromTB()), this);
	Texte->setToggleButton( true );
	BildB = new QToolButton(loadIcon("Bild.xpm"), tr("Insert Picture"), QString::null, this, SLOT(ModeFromTB()), this);
	BildB->setToggleButton( true );
	TableB = new QToolButton(loadIcon("frame_table.png"), tr("Insert Table"), QString::null, this, SLOT(ModeFromTB()), this);
	TableB->setToggleButton( true );
	Rechteck = new Autoforms( this );
	Rechteck->setPopupDelay(0);
	Rechteck->setToggleButton( true );
	PolyM = new QPopupMenu();
	PolyM->insertItem( tr("Properties..."), this, SLOT(GetPolyProps()));
	Polygon = new QToolButton(loadIcon("spline.png"), tr("Insert Polygons"), QString::null, this, SLOT(ModeFromTB()), this);
	Polygon->setToggleButton( true );
	Polygon->setPopup(PolyM);
	Polygon->setPopupDelay(0);
	LinM = new QPopupMenu();
	int id;
	id = LinM->insertItem(loadIcon("Stift.xpm"));
	LinM->setWhatsThis(id, tr("Insert Lines"));
	id = LinM->insertItem(loadIcon("beziertool.png"));
	LinM->setWhatsThis(id, tr("Insert Bezier Curves"));
	id = LinM->insertItem(loadIcon("Stiftalt.xpm"));
	LinM->setWhatsThis(id, tr("Insert Freehand Line"));
	Linien = new QToolButton(loadIcon("Stift.xpm"), tr("Insert Lines"), QString::null, this, SLOT(ModeFromTB()), this);
	Linien->setToggleButton( true );
	Linien->setPopup(LinM);
	Linien->setPopupDelay(0);
	LMode = 8;
	Rotiere = new QToolButton(loadIcon("Rotieren.xpm"), tr("Rotate Item"), QString::null, this, SLOT(ModeFromTB()), this);
	Rotiere->setToggleButton( true );
	Rotiere->setEnabled(false);
	Zoom = new QToolButton(loadIcon("Lupe.xpm"), tr("Zoom in or out"), QString::null, this, SLOT(ModeFromTB()), this);
	Zoom->setToggleButton( true );
	Textedit = new QToolButton(loadIcon("Editm.xpm"), tr("Edit Contents of Frame"), QString::null, this, SLOT(ModeFromTB()), this);
	Textedit->setToggleButton( true );
	Textedit->setEnabled( false );
	Textedit2 = new QToolButton(loadIcon("signature.png"), tr("Edit the text with the Story Editor"), QString::null, this, SLOT(ModeFromTB()), this);
	Textedit2->setToggleButton( true );
	Textedit2->setEnabled( false );
	KetteEin = new QToolButton(loadIcon("Lock.xpm"), tr("Link Text Frames"), QString::null, this, SLOT(ModeFromTB()), this);
	KetteEin->setToggleButton( true );
	KetteEin->setEnabled(false);
	KetteAus = new QToolButton(loadIcon("Unlock.xpm"), tr("Unlink Text Frames"), QString::null, this, SLOT(ModeFromTB()), this);
	KetteAus->setToggleButton( true );
	KetteAus->setEnabled(false);
	Measure = new QToolButton(loadIcon("dist.png"), tr("Do measurements"), QString::null, this, SLOT(ModeFromTB()), this);
	Measure->setToggleButton( true );
	setCloseMode(QDockWindow::Undocked);
	connect(this, SIGNAL(placeChanged(QDockWindow::Place)), this, SLOT(Docken(QDockWindow::Place)));
	connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(Verbergen(bool)));
	connect(Rechteck, SIGNAL(FormSel(int, int, double *)), this, SLOT(SelShape(int, int, double *)));
	connect(Rechteck, SIGNAL(clicked()), this, SLOT(SelShape2()));
	connect(LinM, SIGNAL(activated(int)), this, SLOT(setLinMode(int)));
	QToolTip::add( Rechteck, tr( "Draw various Shapes" ) );
}

void WerkToolB::Docken(QDockWindow::Place p)
{
	setOrientation(p == InDock ? Horizontal : Vertical);
}

void WerkToolB::Verbergen(bool vis)
{
	if (!vis)
		emit Schliessen();
}

void WerkToolB::GetPolyProps()
{
	PolygonProps* dia = new PolygonProps(this, ScApp->doc->PolyC, ScApp->doc->PolyFd, ScApp->doc->PolyF, ScApp->doc->PolyS, ScApp->doc->PolyR);
	if (dia->exec())
		dia->getValues(&ScApp->doc->PolyC, &ScApp->doc->PolyFd, &ScApp->doc->PolyF, &ScApp->doc->PolyS, &ScApp->doc->PolyR);
	delete dia;
}

void WerkToolB::SelShape(int s, int c, double *vals)
{
	Select->setOn(false);
	Rotiere->setOn(false);
	Textedit->setOn(false);
	Textedit2->setOn(false);
	Zoom->setOn(false);
	Texte->setOn(false);
	BildB->setOn(false);
	TableB->setOn(false);
	Linien->setOn(false);
	KetteEin->setOn(false);
	KetteAus->setOn(false);
	Polygon->setOn(false);
	Measure->setOn(false);
	Rechteck->setOn(true);
	SubMode = s;
	ValCount = c;
	ShapeVals = vals;
	emit NewMode(2);
}

void WerkToolB::SelShape2()
{
	Select->setOn(false);
	Rotiere->setOn(false);
	Textedit->setOn(false);
	Textedit2->setOn(false);
	Zoom->setOn(false);
	Texte->setOn(false);
	BildB->setOn(false);
	TableB->setOn(false);
	Linien->setOn(false);
	KetteEin->setOn(false);
	KetteAus->setOn(false);
	Polygon->setOn(false);
	Measure->setOn(false);
	Rechteck->setOn(true);
	emit NewMode(2);
}

void WerkToolB::ModeFromTB()
{
	Select->setOn(false);
	Rotiere->setOn(false);
	Textedit->setOn(false);
	Textedit2->setOn(false);
	Zoom->setOn(false);
	Texte->setOn(false);
	BildB->setOn(false);
	TableB->setOn(false);
	Rechteck->setOn(false);
	Linien->setOn(false);
	KetteEin->setOn(false);
	KetteAus->setOn(false);
	Polygon->setOn(false);
	Measure->setOn(false);
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
	if (Textedit2 == sender())
	{
		Textedit2->setOn(true);
		emit NewMode(3);
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
	if (TableB == sender())
	{
		TableB->setOn(true);
		emit NewMode(22);
	}
	if (Linien == sender())
	{
		Linien->setOn(true);
		emit NewMode(LMode);
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
	if (Measure == sender())
	{
		Measure->setOn(true);
		emit NewMode(24);
	}
}

void WerkToolB::setLinMode(int id)
{
	Select->setOn(false);
	Rotiere->setOn(false);
	Textedit->setOn(false);
	Textedit2->setOn(false);
	Zoom->setOn(false);
	Texte->setOn(false);
	BildB->setOn(false);
	TableB->setOn(false);
	Rechteck->setOn(false);
	Linien->setOn(false);
	KetteEin->setOn(false);
	KetteAus->setOn(false);
	Polygon->setOn(false);
	Measure->setOn(false);
	int c = LinM->indexOf(id);
	QString icn[] = {"Stift.xpm", "beziertool.png", "Stiftalt.xpm"};
	int lm[] = {8, 13, 21};
	Linien->setPixmap(loadIcon(icn[c]));
	LMode = lm[c];
	emit NewMode(LMode);
	Linien->setOn(true);
}

WerkToolBP::WerkToolBP(QMainWindow* parent) : QToolBar( tr("PDF Tools"), parent)
{
	PDFM = new QPopupMenu();
	QString tmp_icn[] = {"pushbutton.png", "textview.png", "checkbox.png", "combobox.png", "listbox.png"};
	QString tmp_txt[] = { tr("Button"), tr("Text Field"), tr("Check Box"), tr("Combo Box"), tr("List Box")};
	size_t ar_tmp = sizeof(tmp_icn) / sizeof(*tmp_icn);
	for (uint a = 0; a < ar_tmp; ++a)
		PDFM->insertItem(loadIcon(tmp_icn[a]), tmp_txt[a]);
	PDFTool = new QToolButton(loadIcon("pushbutton.png"), tr("Insert PDF Fields"), QString::null, this, SLOT(ModeFromTB()), this);
	PDFTool->setToggleButton(true);
	PDFTool->setPopup(PDFM);
	PDFTool->setPopupDelay(0);
	PDFwerkz = 0;
	PDFA = new QPopupMenu();
	PDFA->insertItem(loadIcon("charset.png"), tr("Text"));
	PDFA->insertItem(loadIcon("goto.png"), tr("Link"));
	PDFaTool = new QToolButton(loadIcon("charset.png"), tr("Insert PDF Annotations"), QString::null, this, SLOT(ModeFromTB()), this);
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
	QString tmp[] = {"pushbutton.png", "textview.png", "checkbox.png", "combobox.png", "listbox.png"};
	PDFTool->setPixmap(loadIcon(tmp[c]));
	PDFTool->setOn(true);
	PDFaTool->setOn(false);
	emit NewMode(14+PDFwerkz);
}

void WerkToolBP::Docken(QDockWindow::Place )
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
