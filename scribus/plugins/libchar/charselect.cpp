#include <qtextcodec.h>
#include <qcursor.h>

#include <qlabel.h>
#include <qpushbutton.h>
#include <qtable.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qstringlist.h>
#include <qcombobox.h>
#include <qfont.h>

#include "scribus.h"
#include "charselect.h"
#include "charselect.moc"
#include "scpainter.h"
#ifdef _MSC_VER
 #if (_MSC_VER >= 1200)
  #include "win-config.h"
 #endif
#else
 #include "config.h"
#endif
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_GLYPH_H

extern QPixmap loadIcon(QString nam);
extern QPixmap FontSample(QString da, int s, QString ts, QColor back);
extern void setBestEncoding(FT_Face face);

QString Name()
{
	return QObject::tr("&Insert Special");
}

int Type()
{
	return 1;
}

int ID()
{
	return 1;
}

void Run(QWidget *d, ScribusApp *plug)
{
	if ((plug->HaveDoc) && (plug->view->SelItem.count() != 0))
	{
		PageItem *b = plug->view->SelItem.at(0);
		if ((b->PType == 4) && ((plug->doc->AppMode == 7) || (plug->DLLinput != "")))
		{
			CharSelect *dia = new CharSelect(d, b, plug);
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

ChTable::ChTable(CharSelect* parent, ScribusApp *pl) : QTable(parent)
{
	mPressed = false;
	setFocusPolicy(NoFocus);
	ap = pl;
	par = parent;
	QToolTip::add(this, tr("You can see a thumbnail if you press\nand hold down the right mouse button"));
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
	if ((e->button() == RightButton) && ((r*16+c) < maxCount))
	{
		mPressed = true;
		int bh = 48 + qRound(-(*ap->doc->AllFonts)[font]->numDescender * 48) + 3;
		QPixmap pixm(bh,bh);
		ScPainter *p = new ScPainter(&pixm, bh, bh);
		p->clear();
		pixm.fill(white);
		QWMatrix chma;
		chma.scale(4.8, 4.8);
		FPointArray gly = (*ap->doc->AllFonts)[font]->GlyphArray[par->characters[r*16+c]].Outlines.copy();
		double ww = bh - (*ap->doc->AllFonts)[font]->CharWidth[par->characters[r*16+c]]*48;
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
		dia = new Zoom(this, pixm, par->characters[r*16+c]);
		QPoint ps = QCursor::pos();
		dia->move(ps.x()-2, ps.y()-2);
		dia->show();
	}
}

void ChTable::contentsMouseReleaseEvent(QMouseEvent* e)
{
	e->accept();
	if ((e->button() == RightButton) && (mPressed))
	{
		mPressed = false;
		dia->close();
		delete dia;
	}
	if (e->button() == LeftButton)
		emit selectChar(rowAt(e->pos().y()), columnAt(e->pos().x()));
}

CharSelect::CharSelect( QWidget* parent, PageItem *item, ScribusApp *pl) : QDialog( parent, "CharSelect", true, 0 )
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
	zAuswahlLayout = new QVBoxLayout( this );
	zAuswahlLayout->setSpacing( 6 );
	zAuswahlLayout->setMargin( 11 );

	zTabelle = new ChTable( this, pl);
	zTabelle->setNumCols( 16 );
	zTabelle->setLeftMargin(0);
	zTabelle->verticalHeader()->hide();
	zTabelle->setTopMargin(0);
	zTabelle->horizontalHeader()->hide();
	zTabelle->setSorting(false);
	zTabelle->setSelectionMode(QTable::NoSelection);
	zTabelle->setColumnMovingEnabled(false);
	zTabelle->setRowMovingEnabled(false);
	int counter = 1;
	FT_Face face;
	FT_ULong  charcode;
	FT_UInt   gindex;
	face = pl->doc->FFonts[font];
	setBestEncoding(face);
	gindex = 0;
	charcode = FT_Get_First_Char(face, &gindex );
	while (gindex != 0)
	{
		characters.append(charcode);
		counter++;
		charcode = FT_Get_Next_Char(face, charcode, &gindex );
	}
	int ab = counter / 16;
	int ac = counter % 16;
	int cc = 0;
	maxCount = counter;
	if (ac != 0)
		ab++;
	zTabelle->setNumRows( ab );
	int bh = 16 + qRound(-(*pl->doc->AllFonts)[font]->numDescender * 16) + 3;
	QPixmap pixm(bh,bh);
	for (int a = 0; a < ab; ++a)
	{
		for (int b = 0; b < 16; ++b)
		{
			ScPainter *p = new ScPainter(&pixm, bh, bh);
			p->clear();
			pixm.fill(white);
			QWMatrix chma;
			chma.scale(1.6, 1.6);
			FPointArray gly = (*pl->doc->AllFonts)[font]->GlyphArray[characters[cc]].Outlines.copy();
			cc++;
			if (gly.size() > 4)
			{
				gly.map(chma);
				double ww = bh - (*ap->doc->AllFonts)[font]->CharWidth[characters[cc]]*16;
				p->translate(ww / 2, 1);
				p->setBrush(black);
				p->setFillMode(1);
				p->setupPolygon(&gly);
				p->fillPath();
			}
			p->end();
			delete p;
			QTableItem *it = new QTableItem(zTabelle, QTableItem::Never, "", pixm);
			zTabelle->setItem(a, b, it);
			if (cc == counter)
				break;
		}
	}
	for (int d = 0; d < 16; ++d)
		zTabelle->adjustColumn(d);
	for (int d = 0; d < zTabelle->numRows(); ++d)
		zTabelle->adjustRow(d);
	zTabelle->setMinimumSize(QSize(zTabelle->rowHeight(0)*18, zTabelle->rowHeight(0)*7));
	zAuswahlLayout->addWidget( zTabelle );
	zTabelle->maxCount = maxCount;

	sample = new QLabel( this, "Zeichen" );
	sample->setFrameShape(QFrame::Box);
	sample->setPaletteBackgroundColor(paletteBackgroundColor());
	zAuswahlLayout->addWidget( sample );

	layout1 = new QHBoxLayout;
	layout1->setSpacing( 6 );
	layout1->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout1->addItem( spacer );

	insertButton = new QPushButton(tr( "&Insert" ), this, "insertButton" );
	layout1->addWidget( insertButton );
	deleteButton = new QPushButton(tr("C&lear"), this, "deleteButton" );
	layout1->addWidget( deleteButton );
	closeButton = new QPushButton(tr("&Close"), this, "closeButton" );
	layout1->addWidget( closeButton );
	zAuswahlLayout->addLayout( layout1 );
	sample->setMaximumSize(width(), 52);
	sample->setMinimumSize(width(), 52);
	delEdit();

//tooltips
	QToolTip::add( insertButton, tr( "Insert the characters at the cursor in the text" ) );
	QToolTip::add( deleteButton, tr( "Delete the current selection(s)." ) );
	QToolTip::add( closeButton, tr( "Close this dialog and return to text editing." ) );


	// signals and slots connections
	connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(delEdit()));
	connect(insertButton, SIGNAL(clicked()), this, SLOT(insChar()));
	connect(zTabelle, SIGNAL(selectChar(int, int)), this, SLOT(newChar(int, int)));
}

void CharSelect::newChar(int r, int c) // , int b, const QPoint &pp)
{
	QString font;
	if (ap->DLLinput != "")
		font = ap->DLLinput;
	else
		font = ap->doc->CurrFont;
	if ((r*16+c) < maxCount)
	{
		chToIns += QChar(characters[r*16+c]);
		QString da = (*ap->doc->AllFonts)[font]->Datei;
		sample->setPixmap(FontSample(da, 28, chToIns, paletteBackgroundColor()));
		insertButton->setEnabled(true);
	}
}

void CharSelect::delEdit()
{
	chToIns = "";
	QPixmap pm(1,28);
	pm.fill(paletteBackgroundColor());
	sample->setPixmap(pm);
	insertButton->setEnabled(false);
}

void CharSelect::insChar()
{
	if (ap->DLLinput != "")
	{
		ap->DLLReturn += chToIns;
		delEdit();
		return;
	}
	struct Pti *hg;
	for (uint a=0; a<chToIns.length(); ++a)
	{
		hg = new Pti;
		hg->ch = chToIns.at(a);
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
	ap->view->DrawNew();
	ap->slotDocCh();
}
