#include "charselect.h"
#include "charselect.moc"
#include <qtextcodec.h>
#include <qbitmap.h>
#include <qpaintdevicemetrics.h>
#include "config.h"
#ifdef HAVE_FREETYPE
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_GLYPH_H
#endif

extern QPixmap loadIcon(QString nam);
 
QString Name()
{
  return "Insert Special";
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
		if ((b->PType == 4) && (plug->doc->AppMode == 7))
			{
  		ZAuswahl *dia = new ZAuswahl(d, &plug->Prefs, b, plug);
  		dia->exec();
  		delete dia;
  		}
  	}
}
 
ZAuswahl::ZAuswahl( QWidget* parent, preV *Vor, PageItem *item, ScribusApp *pl)
    : QDialog( parent, "ZAuswahl", true, 0 )
{
    setCaption( tr( "Select Character" ) );
    ite = item;
    ap = pl;
  	setIcon(loadIcon("AppIcon.xpm"));
    ZAuswahlLayout = new QVBoxLayout( this );
    ZAuswahlLayout->setSpacing( 6 );
    ZAuswahlLayout->setMargin( 11 );

    ZTabelle = new QTable( this, "ZTabelle" );
    ZTabelle->setNumCols( 32 );
    ZTabelle->setLeftMargin(0);
    ZTabelle->verticalHeader()->hide();
    ZTabelle->setTopMargin(0);
    ZTabelle->horizontalHeader()->hide();
    ZTabelle->setSorting(false);
    ZTabelle->setSelectionMode(QTable::NoSelection);
    ZTabelle->setColumnMovingEnabled(false);
    ZTabelle->setRowMovingEnabled(false);
    ZTabelle->setFont(pl->doc->UsedFonts[ite->IFont]);
    QPaintDeviceMetrics pm(this);
    DevResX = pm.logicalDpiX();
    DevResY = pm.logicalDpiY();
#ifdef HAVE_FREETYPE
		int counter = 1;
		FT_Face face;
		FT_ULong  charcode;
		FT_UInt   gindex;
		face = pl->doc->FFonts[ite->IFont];
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
    for (int a = 0; a < ab; a++)
      {
      for (int b = 0; b < 32; b++)
        {
				FT_Set_Char_Size(	face, 0, 14*64, DevResX, DevResY );
				FT_Load_Char( face, Zeich[cc], FT_LOAD_RENDER | FT_LOAD_NO_BITMAP | FT_LOAD_MONOCHROME );
				QByteArray bd(face->glyph->bitmap.rows * face->glyph->bitmap.pitch);
				uint yy = 0;
				uint adv;
				if ((face->glyph->bitmap.width % 8) == 0)
					adv = face->glyph->bitmap.width / 8;
				else
					adv = face->glyph->bitmap.width / 8 + 1;
				for (int y = 0; y < face->glyph->bitmap.rows; ++y)
					{
					memcpy(bd.data()+yy, face->glyph->bitmap.buffer+(y * face->glyph->bitmap.pitch), adv);
					yy += adv;
					}
				QBitmap bb(face->glyph->bitmap.width, face->glyph->bitmap.rows, (uchar*)bd.data(), false);
				QPixmap pixm(face->glyph->bitmap.width, face->glyph->bitmap.rows);
				if (!pixm.isNull())
					{
					pixm.fill(black);
					pixm.setMask(bb);
					QPixmap pixm2(face->glyph->bitmap.width+4, face->glyph->bitmap.rows);
					pixm2.fill();
					QPainter p;
					p.begin(&pixm2);
					p.drawPixmap(4, 0, pixm);
					p.end();
					QTableItem *it = new QTableItem(ZTabelle, QTableItem::Never, "", pixm2);
					ZTabelle->setItem(a, b, it);
					}
        cc++;
        if (cc == counter)
					break;
        }
      }
    for (int d = 0; d < 32; d++)
      {
			ZTabelle->setColumnWidth(d, ZTabelle->rowHeight(0));
      }
    ZTabelle->setMinimumSize(QSize(ZTabelle->rowHeight(0)*32, ZTabelle->rowHeight(0)*7));
#else
    ZTabelle->setNumRows( 7 );
    QByteArray c = QByteArray(1);
    c[0] = 32;
    uint cc = 32;
    for (int a = 0; a < 7; a++)
      {
      for (int b = 0; b < 32; b++)
        {
				QTableItem *it = new QTableItem(ZTabelle, QTableItem::Never, QString(QChar(cc)));
				ZTabelle->setItem(a, b, it);
        cc++;
        c[0] = cc;
        }
      }
    for (int d = 0; d < 32; d++)
      {
			ZTabelle->setColumnWidth(d, ZTabelle->rowHeight(0));
      }
    ZTabelle->setMinimumSize(QSize(ZTabelle->rowHeight(0)*32, ZTabelle->rowHeight(0)*7));
#endif
    ZAuswahlLayout->addWidget( ZTabelle );

    Zeichen = new QLineEdit( this, "Zeichen" );
    Zeichen->setFont(pl->doc->UsedFonts[ite->IFont]);
    ZAuswahlLayout->addWidget( Zeichen );

    Layout1 = new QHBoxLayout;
    Layout1->setSpacing( 6 );
    Layout1->setMargin( 0 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( spacer );

    Einf = new QPushButton( this, "Einf" );
    Einf->setText( tr( "Insert" ) );
    Layout1->addWidget( Einf );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( spacer_2 );

    Delete = new QPushButton( this, "Delete" );
    Delete->setText( tr( "Clear" ) );
    Layout1->addWidget( Delete );
    QSpacerItem* spacer_3 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( spacer_3 );

    Close = new QPushButton( this, "Close" );
    Close->setText( tr( "Close" ) );
    Layout1->addWidget( Close );
    QSpacerItem* spacer_4 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( spacer_4 );
    ZAuswahlLayout->addLayout( Layout1 );

    // signals and slots connections
    connect(Close, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Delete, SIGNAL(clicked()), this, SLOT(DelEdit()));
    connect(Einf, SIGNAL(clicked()), this, SLOT(InsChar()));
    connect(ZTabelle, SIGNAL(pressed(int, int, int, const QPoint&)), this, SLOT(NeuesZeichen(int, int)));
}

ZAuswahl::~ZAuswahl()
{
}

void ZAuswahl::NeuesZeichen(int r, int c)
{
#ifdef HAVE_FREETYPE
	if ((r*32+c) < MaxCount)
		Zeichen->insert(QChar(Zeich[r*32+c]));
#else
  Zeichen->insert(ZTabelle->text(r, c));
#endif
}

void ZAuswahl::DelEdit()
{
	Zeichen->clear();
}

void ZAuswahl::InsChar()
{
	struct Pti *hg;
	QString Tex = Zeichen->text();
	for (uint a=0; a<Tex.length(); a++)
		{
		hg = new Pti;
		hg->ch = Tex.at(a);
		if (hg->ch == QChar(10)) { hg->ch = QChar(13); }
		if (hg->ch == QChar(9)) { hg->ch = " "; }
		hg->cfont = ite->IFont;
		hg->csize = ite->ISize;
		hg->ccolor = ite->Pcolor2;
		hg->cextra = 0;
		hg->cshade = ite->Shade2;
		hg->cselect = false;
		hg->cstyle = 0;
		hg->cab = 0;
		hg->xp = 0;
		hg->yp = 0;
		hg->PRot = 0;
		hg->PtransX = 0;
		hg->PtransY = 0;
		ite->Ptext.insert(ite->CPos, hg);
 		ite->CPos += 1;
		}
	ite->Dirty = true;
	ite->paintObj();
	ap->slotDocCh();
}
