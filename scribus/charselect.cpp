#include <qtextcodec.h>
#include <qcursor.h>
#include <qtimer.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qtable.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qstringlist.h>
#include <qcombobox.h>
#include <qfont.h>
#include <qpopupmenu.h>

#include "scribus.h"
#include "fontcombo.h"
#include "charselect.h"
#include "charselect.moc"
#include "scpainter.h"
#include "pluginmanager.h"
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
extern QPixmap FontSample(QString da, int s, QString ts, QColor back, bool force = false);
extern int setBestEncoding(FT_Face face);

#ifdef QT_MAC
Zoom::Zoom(QWidget* parent, QPixmap pix, uint val) : QDialog( parent, "Edit", false, WStyle_Customize | WStyle_NoBorder | WType_Popup)
#else
Zoom::Zoom(QWidget* parent, QPixmap pix, uint val) : QDialog( parent, "Edit", false, WStyle_Customize | WStyle_NoBorder)
#endif
{
	int newwidth=pix.width()+2;
	int newheight=pix.height()+20;
	resize(newwidth,newheight);
	setMinimumSize(QSize(newwidth,newheight));
	setMaximumSize(QSize(newwidth,newheight));
	pixm = pix;
	QString tmp;
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
	watchTimer = new QTimer(this);
//	connect(watchTimer, SIGNAL(timeout()), this, SLOT(showAlternate()));
	mPressed = false;
	alternate = false;
	rowA = 0;
	colA = 0;
	ap = pl;
	par = parent;
	dia = 0;
	QToolTip::add(this, tr("You can see a thumbnail if you press\nand hold down the right mouse button\n\nThe Insert key inserts a Glyph into the Selection below\nand the Delete key removes the last inserted one"));
}

void ChTable::keyPressEvent(QKeyEvent *k)
{
	switch (k->key())
	{
		case Key_Backspace:
		case Key_Delete:
			emit delChar();
			break;
		case Key_Insert:
			emit selectChar(currentRow(), currentColumn());
			break;
	}
	QTable::keyPressEvent(k);
}

void ChTable::contentsMousePressEvent(QMouseEvent* e)
{
	e->accept();
	uint r = rowAt(e->pos().y());
	uint c = columnAt(e->pos().x());
	QString font;
	font = par->fontInUse;
	mPressed = true;
	alternate = false;
	if ((e->button() == RightButton) && ((r*16+c) < maxCount))
	{
		watchTimer->stop();
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
		dia->setModal(false);
		dia->show();
	}
/*	if (e->button() == LeftButton)
	{
		rowA = rowAt(e->pos().y());
		colA = columnAt(e->pos().x());
		watchTimer->start(3000, true);
	} */
	QTable::contentsMousePressEvent(e);
}

void ChTable::contentsMouseReleaseEvent(QMouseEvent* e)
{
	e->accept();
	watchTimer->stop();
	if ((e->button() == RightButton) && (mPressed))
	{
		if (dia)
		{
			dia->close();
			delete dia;
			dia = 0;
		}
	}
	if ((e->button() == LeftButton) && (!alternate))
		emit selectChar(rowAt(e->pos().y()), columnAt(e->pos().x()));
	mPressed = false;
	alternate = false;
	QTable::contentsMouseReleaseEvent(e);
}

void ChTable::showAlternate()
{
/*	watchTimer->stop();
	alternate = true;
	QString font;
	QString chToIns = "";
	font = par->fontInUse;
	uint baseChar = rowA*16+colA;
	if (baseChar < maxCount)
	{
		QPopupMenu *pmen = new QPopupMenu();
		chToIns = QChar(par->characters[baseChar]);
		pmen->insertItem(FontSample((*ap->doc->AllFonts)[font]->Datei, 20, chToIns, paletteBackgroundColor(), true));
		if ((*ap->doc->AllFonts)[font]->CharWidth.contains(par->characters[baseChar] + 0xF720))
		{
			chToIns = QChar(par->characters[baseChar] + 0xF720);
			pmen->insertItem(FontSample((*ap->doc->AllFonts)[font]->Datei, 20, chToIns, paletteBackgroundColor(), true));
		}
		int re = pmen->indexOf(pmen->exec(QCursor::pos()));
		delete pmen;
	} */
}

CharSelect::CharSelect( QWidget* parent, PageItem *item, ScribusApp *pl) : QDialog( parent, "CharSelect", true, 0 )
{
	QString font;
	if (pl->pluginManager->dllInput != "")
		font = pl->pluginManager->dllInput;
	else
		font = pl->doc->CurrFont;
	fontInUse = font;
	setCaption( tr( "Select Character:" )+" "+font );
	ite = item;
	ap = pl;
	setIcon(loadIcon("AppIcon.png"));
	zAuswahlLayout = new QVBoxLayout( this );
	zAuswahlLayout->setSpacing( 6 );
	zAuswahlLayout->setMargin( 11 );

	selectionsLayout = new QHBoxLayout();
	selectionsLayout->setSpacing( 6 );
	selectionsLayout->setMargin( 0 );
	fontLabel = new QLabel( this, "fontLabel" );
	fontLabel->setText( tr( "Font:" ) );
	selectionsLayout->addWidget( fontLabel );
	fontSelector = new FontCombo(this, &pl->Prefs);
	fontSelector->setMaximumSize(190, 30);
	fontSelector->setCurrentText(fontInUse);
	selectionsLayout->addWidget( fontSelector );
	if ((ap->doc->currentParaStyle > 4) ||  (ap->pluginManager->dllInput != ""))
		fontSelector->setEnabled(false);
	rangeLabel = new QLabel( this, "fontLabel" );
	rangeLabel->setText( tr( "Character Class:" ) );
	selectionsLayout->addWidget( rangeLabel );
	rangeSelector = new QComboBox( true, this, "rangeSelector" );
	rangeSelector->setEditable(false);
	characterClass = 0;
	selectionsLayout->addWidget( rangeSelector );
	QSpacerItem* spacer2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	selectionsLayout->addItem( spacer2 );
	zAuswahlLayout->addLayout(selectionsLayout);
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
	scanFont();
	generatePreview(0);
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

	insertButton = new QPushButton( tr( "&Insert" ), this, "insertButton" );
	layout1->addWidget( insertButton );
	deleteButton = new QPushButton( tr("C&lear"), this, "deleteButton" );
	layout1->addWidget( deleteButton );
	closeButton = new QPushButton( tr("&Close"), this, "closeButton" );
	layout1->addWidget( closeButton );
	zAuswahlLayout->addLayout( layout1 );
	sample->setMinimumHeight(52);
	delEdit();
//tooltips
	QToolTip::add( insertButton, tr( "Insert the characters at the cursor in the text" ) );
	QToolTip::add( deleteButton, tr( "Delete the current selection(s)." ) );
	QToolTip::add( closeButton, tr( "Close this dialog and return to text editing." ) );


	// signals and slots connections
	connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(delEdit()));
	connect(insertButton, SIGNAL(clicked()), this, SLOT(insChar()));
	connect(zTabelle, SIGNAL(selectChar(uint, uint)), this, SLOT(newChar(uint, uint)));
	connect(zTabelle, SIGNAL(delChar()), this, SLOT(delChar()));
	connect(fontSelector, SIGNAL(activated(int)), this, SLOT(newFont(int)));
	connect(rangeSelector, SIGNAL(activated(int)), this, SLOT(newCharClass(int)));
	setupRangeCombo();
}

void CharSelect::scanFont()
{
	FT_Face face;
	FT_ULong  charcode;
	FT_UInt   gindex;
	gindex = 0;
	allClasses.clear();
	charactersFull.clear();
	charactersLatin1.clear();
	charactersLatin1Supplement.clear();
	charactersLatinExtendedA.clear();
	charactersLatinExtendedB.clear();
	charactersGeneralPunctuation.clear();
	charactersSuperSubscripts.clear();
	charactersCurrencySymbols.clear();
	charactersLetterlikeSymbols.clear();
	charactersNumberForms.clear();
	charactersArrows.clear();
	charactersMathematicalOperators.clear();
	charactersBoxDrawing.clear();
	charactersBlockElements.clear();
	charactersGeometricShapes.clear();
	charactersMiscellaneousSymbols.clear();
	charactersDingbats.clear();
	charactersSmallFormVariants.clear();
	charactersAlphabeticPresentationForms.clear();
	charactersSpecial.clear();
	charactersGreek.clear();
	charactersGreekExtended.clear();
	charactersCyrillic.clear();
	charactersCyrillicSupplement.clear();
	charactersArabic.clear();
	charactersArabicPresentationFormsA.clear();
	charactersArabicPresentationFormsB.clear();
	charactersHebrew.clear();
	face = ap->doc->FFonts[fontInUse];
	if (!face) {
		return;
	}
	setBestEncoding(face);
	charcode = FT_Get_First_Char(face, &gindex );
	while (gindex != 0)
	{
		charactersFull.append(charcode);
		if ((charcode >= 0x0020 ) && (charcode <= 0x007F))
			charactersLatin1.append(charcode);
		else if ((charcode >= 0x0080 ) && (charcode <= 0x00FF))
			charactersLatin1Supplement.append(charcode);
		else if ((charcode >= 0x0100 ) && (charcode <= 0x017F))
			charactersLatinExtendedA.append(charcode);
		else if ((charcode >= 0x0180 ) && (charcode <= 0x024F))
			charactersLatinExtendedB.append(charcode);
		else if ((charcode >= 0x0370 ) && (charcode <= 0x03FF))
			charactersGreek.append(charcode);
		else if ((charcode >= 0x0400 ) && (charcode <= 0x04FF))
			charactersCyrillic.append(charcode);
		else if ((charcode >= 0x0500 ) && (charcode <= 0x052F))
			charactersCyrillicSupplement.append(charcode);
		else if ((charcode >= 0x0590 ) && (charcode <= 0x05FF))
			charactersHebrew.append(charcode);
		else if ((charcode >= 0x0600 ) && (charcode <= 0x06FF))
			charactersArabic.append(charcode);
		else if ((charcode >= 0x1F00 ) && (charcode <= 0x1FFF))
			charactersGreekExtended.append(charcode);
		else if ((charcode >= 0x2000 ) && (charcode <= 0x206F))
			charactersGeneralPunctuation.append(charcode);
		else if ((charcode >= 0x2070 ) && (charcode <= 0x209F))
			charactersSuperSubscripts.append(charcode);
		else if ((charcode >= 0x20A0 ) && (charcode <= 0x20CF))
			charactersCurrencySymbols.append(charcode);
		else if ((charcode >= 0x2100 ) && (charcode <= 0x214F))
			charactersLetterlikeSymbols.append(charcode);
		else if ((charcode >= 0x2150 ) && (charcode <= 0x218F))
			charactersNumberForms.append(charcode);
		else if ((charcode >= 0x2190 ) && (charcode <= 0x2190))
			charactersArrows.append(charcode);
		else if ((charcode >= 0x2200 ) && (charcode <= 0x22FF))
			charactersMathematicalOperators.append(charcode);
		else if ((charcode >= 0x2500 ) && (charcode <= 0x257F))
			charactersBoxDrawing.append(charcode);
		else if ((charcode >= 0x2580 ) && (charcode <= 0x259F))
			charactersBlockElements.append(charcode);
		else if ((charcode >= 0x25A0 ) && (charcode <= 0x25FF))
			charactersGeometricShapes.append(charcode);
		else if ((charcode >= 0x2600 ) && (charcode <= 0x26FF))
			charactersMiscellaneousSymbols.append(charcode);
		else if ((charcode >= 0x2700 ) && (charcode <= 0x27BF))
			charactersDingbats.append(charcode);
		else if ((charcode >= 0xFE50 ) && (charcode <= 0xFE6F))
			charactersSmallFormVariants.append(charcode);
		else if ((charcode >= 0xFB00 ) && (charcode <= 0xFB4F))
			charactersAlphabeticPresentationForms.append(charcode);
		else if ((charcode >= 0xFB50 ) && (charcode <= 0xFDFF))
			charactersArabicPresentationFormsA.append(charcode);
		else if ((charcode >= 0xFE70 ) && (charcode <= 0xFEFF))
			charactersArabicPresentationFormsB.append(charcode);
		else if ((charcode >= 0xFFF0 ) && (charcode <= 0xFFFF))
			charactersSpecial.append(charcode);
		charcode = FT_Get_Next_Char(face, charcode, &gindex );
	}
	allClasses.append(charactersFull);
	allClasses.append(charactersLatin1);
	allClasses.append(charactersLatin1Supplement);
	allClasses.append(charactersLatinExtendedA);
	allClasses.append(charactersLatinExtendedB);
	allClasses.append(charactersGeneralPunctuation);
	allClasses.append(charactersSuperSubscripts);
	allClasses.append(charactersCurrencySymbols);
	allClasses.append(charactersLetterlikeSymbols);
	allClasses.append(charactersNumberForms);
	allClasses.append(charactersArrows);
	allClasses.append(charactersMathematicalOperators);
	allClasses.append(charactersBoxDrawing);
	allClasses.append(charactersBlockElements);
	allClasses.append(charactersGeometricShapes);
	allClasses.append(charactersMiscellaneousSymbols);
	allClasses.append(charactersDingbats);
	allClasses.append(charactersSmallFormVariants);
	allClasses.append(charactersAlphabeticPresentationForms);
	allClasses.append(charactersSpecial);
	allClasses.append(charactersGreek);
	allClasses.append(charactersGreekExtended);
	allClasses.append(charactersCyrillic);
	allClasses.append(charactersCyrillicSupplement);
	allClasses.append(charactersArabic);
	allClasses.append(charactersArabicPresentationFormsA);
	allClasses.append(charactersArabicPresentationFormsB);
	allClasses.append(charactersHebrew);
}

void CharSelect::setupRangeCombo()
{
	disconnect(rangeSelector, SIGNAL(activated(int)), this, SLOT(newCharClass(int)));
	int counter = 0;
	rangeSelector->clear();
	rangeSelector->insertItem( tr("Full Character Set"));
	usedCharClasses.insert(counter, 0);
	counter++;
	if (charactersLatin1.count() != 0)
	{
		rangeSelector->insertItem( tr("Basic Latin"));
		usedCharClasses.insert(counter, 1);
		counter++;
	}
	if (charactersLatin1Supplement.count() != 0)
	{
		rangeSelector->insertItem( tr("Latin-1 Supplement"));
		usedCharClasses.insert(counter, 2);
		counter++;
	}
	if (charactersLatinExtendedA.count() != 0)
	{
		rangeSelector->insertItem( tr("Latin Extended-A"));
		usedCharClasses.insert(counter, 3);
		counter++;
	}
	if (charactersLatinExtendedB.count() != 0)
	{
		rangeSelector->insertItem( tr("Latin Extended-B"));
		usedCharClasses.insert(counter, 4);
		counter++;
	}
	if (charactersGeneralPunctuation.count() != 0)
	{
		rangeSelector->insertItem( tr("General Punctuation"));
		usedCharClasses.insert(counter, 5);
		counter++;
	}
	if (charactersSuperSubscripts.count() != 0)
	{
		rangeSelector->insertItem( tr("Super- and Subscripts"));
		usedCharClasses.insert(counter, 6);
		counter++;
	}
	if (charactersCurrencySymbols.count() != 0)
	{
		rangeSelector->insertItem( tr("Currency Symbols"));
		usedCharClasses.insert(counter, 7);
		counter++;
	}
	if (charactersLetterlikeSymbols.count() != 0)
	{
		rangeSelector->insertItem( tr("Letterlike Symbols"));
		usedCharClasses.insert(counter, 8);
		counter++;
	}
	if (charactersNumberForms.count() != 0)
	{
		rangeSelector->insertItem( tr("Number Forms"));
		usedCharClasses.insert(counter, 9);
		counter++;
	}
	if (charactersArrows.count() != 0)
	{
		rangeSelector->insertItem( tr("Arrows"));
		usedCharClasses.insert(counter, 10);
		counter++;
	}
	if (charactersMathematicalOperators.count() != 0)
	{
		rangeSelector->insertItem( tr("Mathematical Operators"));
		usedCharClasses.insert(counter, 11);
		counter++;
	}
	if (charactersBoxDrawing.count() != 0)
	{
		rangeSelector->insertItem( tr("Box Drawing"));
		usedCharClasses.insert(counter, 12);
		counter++;
	}
	if (charactersBlockElements.count() != 0)
	{
		rangeSelector->insertItem( tr("Block Elements"));
		usedCharClasses.insert(counter, 13);
		counter++;
	}
	if (charactersGeometricShapes.count() != 0)
	{
		rangeSelector->insertItem( tr("Geometric Shapes"));
		usedCharClasses.insert(counter, 14);
		counter++;
	}
	if (charactersMiscellaneousSymbols.count() != 0)
	{
		rangeSelector->insertItem( tr("Miscellaneous Symbols"));
		usedCharClasses.insert(counter, 15);
		counter++;
	}
	if (charactersDingbats.count() != 0)
	{
		rangeSelector->insertItem( tr("Dingbats"));
		usedCharClasses.insert(counter, 16);
		counter++;
	}
	if (charactersSmallFormVariants.count() != 0)
	{
		rangeSelector->insertItem( tr("Small Form Variants"));
		usedCharClasses.insert(counter, 17);
		counter++;
	}
	if (charactersAlphabeticPresentationForms.count() != 0)
	{
		rangeSelector->insertItem( tr("Ligatures"));
		usedCharClasses.insert(counter, 18);
		counter++;
	}
	if (charactersSpecial.count() != 0)
	{
		rangeSelector->insertItem( tr("Specials"));
		usedCharClasses.insert(counter, 19);
		counter++;
	}
	if (charactersGreek.count() != 0)
	{
		rangeSelector->insertItem( tr("Greek"));
		usedCharClasses.insert(counter, 20);
		counter++;
	}
	if (charactersGreekExtended.count() != 0)
	{
		rangeSelector->insertItem( tr("Greek Extended"));
		usedCharClasses.insert(counter, 21);
		counter++;
	}
	if (charactersCyrillic.count() != 0)
	{
		rangeSelector->insertItem( tr("Cyrillic"));
		usedCharClasses.insert(counter, 22);
		counter++;
	}
	if (charactersCyrillicSupplement.count() != 0)
	{
		rangeSelector->insertItem( tr("Cyrillic Supplement"));
		usedCharClasses.insert(counter, 23);
		counter++;
	}
	if (charactersArabic.count() != 0)
	{
		rangeSelector->insertItem( tr("Arabic"));
		usedCharClasses.insert(counter, 24);
		counter++;
	}
	if (charactersArabicPresentationFormsA.count() != 0)
	{
		rangeSelector->insertItem( tr("Arabic Extended A"));
		usedCharClasses.insert(counter, 25);
		counter++;
	}
	if (charactersArabicPresentationFormsB.count() != 0)
	{
		rangeSelector->insertItem( tr("Arabic Extended B"));
		usedCharClasses.insert(counter, 26);
		counter++;
	}
	if (charactersHebrew.count() != 0)
	{
		rangeSelector->insertItem( tr("Hebrew"));
		usedCharClasses.insert(counter, 27);
		counter++;
	}
	rangeSelector->setCurrentItem(0);
	connect(rangeSelector, SIGNAL(activated(int)), this, SLOT(newCharClass(int)));
}

void CharSelect::generatePreview(int charClass)
{
	FT_Face face;
	characters.clear();
	zTabelle->setNumRows( 0 );
	characters = allClasses[charClass];
	face = ap->doc->FFonts[fontInUse];
	if (!face) {
		maxCount = 0;
		zTabelle->maxCount = 0;
		return;
	}
	setBestEncoding(face);
	maxCount = characters.count();
	zTabelle->maxCount = maxCount;
	int ab = maxCount / 16;
	int ac = maxCount % 16;
	uint cc = 0;
	if (ac != 0)
		ab++;
	zTabelle->setNumRows( ab );
	int bh = QMAX(16 + qRound(-(*ap->doc->AllFonts)[fontInUse]->numDescender * 16) + 3, 16);
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
			FPointArray gly = (*ap->doc->AllFonts)[fontInUse]->GlyphArray[characters[cc]].Outlines.copy();
			if (gly.size() > 4)
			{
				gly.map(chma);
				double ww = bh - (*ap->doc->AllFonts)[fontInUse]->CharWidth[characters[cc]]*16;
				p->translate(ww / 2, 1);
				p->setBrush(black);
				p->setFillMode(1);
				p->setupPolygon(&gly);
				p->fillPath();
				p->end();
				QTableItem *it = new QTableItem(zTabelle, QTableItem::Never, "", pixm);
				zTabelle->setItem(a, b, it);
			}
			cc++;
			delete p;
			if (cc >= maxCount)
				break;
		}
	}
	for (int d = 0; d < 16; ++d)
		zTabelle->adjustColumn(d);
	for (int d = 0; d < zTabelle->numRows(); ++d)
		zTabelle->adjustRow(d);
	zTabelle->setCurrentCell(0, 0);
}

void CharSelect::newCharClass(int c)
{
	characterClass = usedCharClasses[c];
	generatePreview(characterClass);
}

void CharSelect::newFont(int font)
{
	fontInUse = fontSelector->text(font);
	delEdit();
	setCaption( tr( "Select Character:" )+" "+fontInUse );
	ap->SetNewFont(fontInUse);
	scanFont();
	generatePreview(0);
	characterClass = 0;
	setupRangeCombo();
}

void CharSelect::newChar(uint r, uint c) // , int b, const QPoint &pp)
{
	if ((r*16+c) < maxCount)
	{
		chToIns += QChar(characters[r*16+c]);
		sample->setPixmap(FontSample((*ap->doc->AllFonts)[fontInUse]->Datei, 28, chToIns, paletteBackgroundColor(), true));
		insertButton->setEnabled(true);
	}
}

void CharSelect::delChar()
{
	if (chToIns.length() == 0)
		return;
	if (chToIns.length() == 1)
	{
		delEdit();
		return;
	}
	chToIns.truncate(chToIns.length() - 1);
	sample->setPixmap(FontSample((*ap->doc->AllFonts)[fontInUse]->Datei, 28, chToIns, paletteBackgroundColor(), true));
	insertButton->setEnabled(true);
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
	if (ap->pluginManager->dllInput != "")
	{
		ap->pluginManager->dllReturn += chToIns;
		delEdit();
		return;
	}
	struct ScText *hg;
	for (uint a=0; a<chToIns.length(); ++a)
	{
		hg = new ScText;
		hg->ch = chToIns.at(a);
		if (hg->ch == QChar(10))
			hg->ch = QChar(13);
		if (hg->ch == QChar(9))
			hg->ch = " ";
		hg->cfont = (*ap->doc->AllFonts)[fontInUse];
		hg->csize = ap->doc->CurrFontSize;
		hg->ccolor = ap->doc->CurrTextFill;
		hg->cshade = ap->doc->CurrTextFillSh;
		hg->cstroke = ap->doc->CurrTextStroke;
		hg->cshade2 = ap->doc->CurrTextStrokeSh;
		hg->cscale = ap->doc->CurrTextScale;
		hg->cselect = false;
		hg->cstyle = ap->doc->CurrentStyle;
		hg->cab = ap->doc->currentParaStyle;
		if (ap->doc->docParagraphStyles[ap->doc->currentParaStyle].Font != "")
		{
			hg->cfont = (*ap->doc->AllFonts)[ap->doc->docParagraphStyles[ap->doc->currentParaStyle].Font];
			hg->csize = ap->doc->docParagraphStyles[ap->doc->currentParaStyle].FontSize;
		}
		hg->cextra = 0;
		hg->cselect = false;
		hg->xp = 0;
		hg->yp = 0;
		hg->PRot = 0;
		hg->PtransX = 0;
		hg->PtransY = 0;
		ite->itemText.insert(ite->CPos, hg);
		ite->CPos += 1;
	}
	ap->view->DrawNew();
	ap->slotDocCh();
}
