#include "charselect.h"
#include "charselect.moc"
#include "scpainter.h"
#include <qtextcodec.h>
#include <qcursor.h>

#if (_MSC_VER >= 1200)
 #include "win-config.h"
#else
 #include "config.h"
#endif

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_GLYPH_H

extern QPixmap loadIcon(QString nam);
extern QPixmap FontSample(QString da, int s, QString ts, QColor back);

QString Name()
{
	return QObject::tr("&Insert Special");
}

int Type()
{
	return 1;
}

void Run(QWidget *d, ScribusApp *plug)
{
	if ((plug->HaveDoc) && (plug->doc->ActPage->SelItem.count() != 0))
	{
		PageItem *b = plug->doc->ActPage->SelItem.at(0);
		if ((b->PType == 4) && ((plug->doc->AppMode == 7) || (plug->DLLinput != "")))
		{
			ZAuswahl *dia = new ZAuswahl(d, &plug->Prefs, b, plug);
			dia->exec();
			delete dia;
		}
	}
}

Zoom::Zoom(QWidget* parent, QPixmap pix, uint val) : QDialog( parent, "Edit", false, WStyle_Customize | WStyle_NoBorderEx)
{
	QString tmp;
	resize(pix.width()+2,pix.height()+20);
	setMinimumSize(QSize(pix.width()+2,pix.height()+20));
	setMaximumSize(QSize(pix.width()+2,pix.height()+20));
	pixm = pix;
	tmp.sprintf("%04X", val);
	valu = "0x"+tmp;
}

void Zoom::paintEvent(QPaintEvent *)
{
	QPainter p;
	p.begin(this);
	p.setPen(black);
	p.setBrush(NoBrush);
	p.drawRect(0, 0, width(), height());
	p.drawPixmap(1, 1, pixm);
	p.drawText(5, height()-3, valu);
	p.end();
}

ChTable::ChTable(ZAuswahl* parent, ScribusApp *pl) : QTable(parent)
{
	Mpressed = false;
	setFocusPolicy(NoFocus);
	ap = pl;
	par = parent;
}

void ChTable::contentsMousePressEvent(QMouseEvent* e)
{
	e->accept();
	int r = rowAt(e->pos().y());
	int c = columnAt(e->pos().x());
	QString font;
	if (ap->DLLinput != "")
		font = ap->DLLinput;
	else
		font = ap->doc->CurrFont;
	if ((e->button() == RightButton) && ((r*32+c) < MaxCount))
	{
		Mpressed = true;
		int bh = 48 + qRound(-(*ap->doc->AllFonts)[font]->numDescender * 48) + 3;
		QPixmap pixm(bh,bh);
		ScPainter *p = new ScPainter(&pixm, bh, bh);
		p->clear();
		pixm.fill(white);
		QWMatrix chma;
		chma.scale(4.8, 4.8);
		FPointArray gly = (*ap->doc->AllFonts)[font]->GlyphArray[par->Zeich[r*32+c]].Outlines.copy();
		double ww = bh - (*ap->doc->AllFonts)[font]->CharWidth[par->Zeich[r*32+c]]*48;
		if (gly.size() > 4)
		{
			gly.map(chma);
			p->translate(ww / 2, 1);
			p->setBrush(black);
			p->setFillMode(1);
			p->setupPolygon(&gly);
			p->fillPath();
			p->end();
		}
		delete p;
		dia = new Zoom(this, pixm, par->Zeich[r*32+c]);
		QPoint ps = QCursor::pos();
		dia->move(ps.x()-2, ps.y()-2);
		dia->show();
	}
}

void ChTable::contentsMouseReleaseEvent(QMouseEvent* e)
{
	e->accept();
	if ((e->button() == RightButton) && (Mpressed))
	{
		Mpressed = false;
		dia->close();
		delete dia;
	}
	if (e->button() == LeftButton)
		emit SelectChar(rowAt(e->pos().y()), columnAt(e->pos().x()));
}

ZAuswahl::ZAuswahl( QWidget* parent, preV *Vor, PageItem *item, ScribusApp *pl)
		: QDialog( parent, "ZAuswahl", true, 0 )
{
	QString font;
	if (pl->DLLinput != "")
		font = pl->DLLinput;
	else
		font = pl->doc->CurrFont;
	setCaption( tr( "Select Character:" )+" "+font );
	ite = item;
	ap = pl;
	setIcon(loadIcon("AppIcon.png"));
	ZAuswahlLayout = new QVBoxLayout( this );
	ZAuswahlLayout->setSpacing( 6 );
	ZAuswahlLayout->setMargin( 11 );

	ZTabelle = new ChTable( this, pl);
	ZTabelle->setNumCols( 32 );
	ZTabelle->setLeftMargin(0);
	ZTabelle->verticalHeader()->hide();
	ZTabelle->setTopMargin(0);
	ZTabelle->horizontalHeader()->hide();
	ZTabelle->setSorting(false);
	ZTabelle->setSelectionMode(QTable::NoSelection);
	ZTabelle->setColumnMovingEnabled(false);
	ZTabelle->setRowMovingEnabled(false);
	int counter = 1;
	FT_Face face;
	FT_ULong  charcode;
	FT_UInt   gindex;
	face = pl->doc->FFonts[font];
	charcode = FT_Get_First_Char(face, &gindex );
	while (gindex != 0)
	{
		Zeich.append(charcode);
		counter++;
		charcode = FT_Get_Next_Char(face, charcode, &gindex );
	}
	int ab = counter / 32;
	int ac = counter % 32;
	int cc = 0;
	MaxCount = counter;
	if (ac != 0)
		ab++;
	ZTabelle->setNumRows( ab );
	int bh = 16 + qRound(-(*pl->doc->AllFonts)[font]->numDescender * 16) + 3;
	QPixmap pixm(bh,bh);
	for (int a = 0; a < ab; ++a)
	{
		for (int b = 0; b < 32; ++b)
		{
			ScPainter *p = new ScPainter(&pixm, bh, bh);
			p->clear();
			pixm.fill(white);
			QWMatrix chma;
			chma.scale(1.6, 1.6);
			FPointArray gly = (*pl->doc->AllFonts)[font]->GlyphArray[Zeich[cc]].Outlines.copy();
			cc++;
			if (gly.size() > 4)
			{
				gly.map(chma);
				double ww = bh - (*ap->doc->AllFonts)[font]->CharWidth[Zeich[cc]]*16;
				p->translate(ww / 2, 1);
				p->setBrush(black);
				p->setFillMode(1);
				p->setupPolygon(&gly);
				p->fillPath();
			}
			p->end();
			delete p;
			QTableItem *it = new QTableItem(ZTabelle, QTableItem::Never, "", pixm);
			ZTabelle->setItem(a, b, it);
			if (cc == counter)
				break;
		}
	}
	for (int d = 0; d < 32; ++d)
		ZTabelle->adjustColumn(d);
	for (int d = 0; d < ZTabelle->numRows(); ++d)
		ZTabelle->adjustRow(d);
	ZTabelle->setMinimumSize(QSize(ZTabelle->rowHeight(0)*33, ZTabelle->rowHeight(0)*7));
	ZAuswahlLayout->addWidget( ZTabelle );
	ZTabelle->MaxCount = MaxCount;

	Zeichen = new QLabel( this, "Zeichen" );
	ZAuswahlLayout->addWidget( Zeichen );

	Layout1 = new QHBoxLayout;
	Layout1->setSpacing( 6 );
	Layout1->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1->addItem( spacer );

	Einf = new QPushButton(tr( "&Insert" ), this, "Einf" );
	Layout1->addWidget( Einf );
	Delete = new QPushButton(tr("C&lear"), this, "Delete" );
	Layout1->addWidget( Delete );
	Close = new QPushButton(tr("&Close"), this, "Close" );
	Layout1->addWidget( Close );
	ZAuswahlLayout->addLayout( Layout1 );
	Zeichen->setMaximumSize(width(), 50);
	DelEdit();

//tooltips
	QToolTip::add( Einf, tr( "Insert the characters at the cursor in the text" ) );
	QToolTip::add( Delete, tr( "Delete the current selection(s)." ) );
	QToolTip::add( Close, tr( "Close this dialog and return to text editing." ) );


	// signals and slots connections
	connect(Close, SIGNAL(clicked()), this, SLOT(accept()));
	connect(Delete, SIGNAL(clicked()), this, SLOT(DelEdit()));
	connect(Einf, SIGNAL(clicked()), this, SLOT(InsChar()));
	connect(ZTabelle, SIGNAL(SelectChar(int, int)), this, SLOT(NeuesZeichen(int, int)));
}

void ZAuswahl::NeuesZeichen(int r, int c) // , int b, const QPoint &pp)
{
	QString font;
	if (ap->DLLinput != "")
		font = ap->DLLinput;
	else
		font = ap->doc->CurrFont;
	if ((r*32+c) < MaxCount)
	{
		ChToIns += QChar(Zeich[r*32+c]);
		QString da = (*ap->doc->AllFonts)[font]->Datei;
		Zeichen->setPixmap(FontSample(da, 28, ChToIns, paletteBackgroundColor()));
		Einf->setEnabled(true);
	}
}

void ZAuswahl::DelEdit()
{
	ChToIns = "";
	QPixmap pm(1,28);
	pm.fill(paletteBackgroundColor());
	Zeichen->setPixmap(pm);
	Einf->setEnabled(false);
}

void ZAuswahl::InsChar()
{
	if (ap->DLLinput != "")
	{
		ap->DLLReturn += ChToIns;
		DelEdit();
		return;
	}
	struct Pti *hg;
	for (uint a=0; a<ChToIns.length(); ++a)
	{
		hg = new Pti;
		hg->ch = ChToIns.at(a);
		if (hg->ch == QChar(10))
			hg->ch = QChar(13);
		if (hg->ch == QChar(9))
			hg->ch = " ";
		hg->cfont = ap->doc->CurrFont;
		hg->csize = ap->doc->CurrFontSize;
		hg->ccolor = ap->doc->CurrTextFill;
		hg->cshade = ap->doc->CurrTextFillSh;
		hg->cstroke = ap->doc->CurrTextStroke;
		hg->cshade2 = ap->doc->CurrTextStrokeSh;
		hg->cscale = ap->doc->CurrTextScale;
		hg->cselect = false;
		hg->cstyle = ap->doc->CurrentStyle;
		hg->cab = ap->doc->CurrentABStil;
		if (ap->doc->Vorlagen[ap->doc->CurrentABStil].Font != "")
		{
			hg->cfont = ap->doc->Vorlagen[ap->doc->CurrentABStil].Font;
			hg->csize = ap->doc->Vorlagen[ap->doc->CurrentABStil].FontSize;
		}
		hg->cextra = 0;
		hg->cselect = false;
		hg->xp = 0;
		hg->yp = 0;
		hg->PRot = 0;
		hg->PtransX = 0;
		hg->PtransY = 0;
		ite->Ptext.insert(ite->CPos, hg);
		ite->CPos += 1;
	}
	ap->doc->ActPage->update();
	ap->slotDocCh();
}
