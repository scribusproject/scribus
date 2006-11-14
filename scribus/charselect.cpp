/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <qtable.h>

#include "scconfig.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "fontcombo.h"
#include "charselect.h"
#include "charselect.moc"
#include "sccombobox.h"
#include "scpainter.h"
#include "unicodesearch.h"

#include "fonts/scfontmetrics.h"
#include "util.h"


CharSelect::CharSelect( QWidget* parent, PageItem *item) : QDialog( parent, "CharSelect", true, 0 )
{
	m_fontInUse = item->doc()->currentStyle.charStyle().font().scName();
	needReturn = false;
	installEventFilter(this);
	run(parent, item);
}

CharSelect::CharSelect( QWidget* parent, PageItem *item, QString font, bool modal)
	: QDialog( parent, "CharSelect", modal, 0 )
{
	m_fontInUse = font;
	needReturn = true;
	installEventFilter(this);
	run(parent, item);
}


const QString & CharSelect::getCharacters()
{
	return m_characters;
}

void CharSelect::run( QWidget* /*parent*/, PageItem *item)
{
	setCaption( tr( "Select Character:" )+" "+m_fontInUse );
	m_Item = item;
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
	fontSelector = new FontCombo(this);
	fontSelector->setMaximumSize(190, 30);
	fontSelector->setCurrentText(m_fontInUse);
	selectionsLayout->addWidget( fontSelector );
	if (needReturn)
		fontSelector->setEnabled(false);
	rangeLabel = new QLabel( this, "fontLabel" );
	rangeLabel->setText( tr( "Character Class:" ) );
	selectionsLayout->addWidget( rangeLabel );
	rangeSelector = new ScComboBox( false, this, "rangeSelector" );
	m_characterClass = 0;
	selectionsLayout->addWidget( rangeSelector );
	QSpacerItem* spacer2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	selectionsLayout->addItem( spacer2 );
	zAuswahlLayout->addLayout(selectionsLayout);

	charTable = new CharTable(this, 16, m_Item, m_fontInUse);
	charTable->enableDrops(false);
	scanFont();

	unicodeButton = new UnicodeChooseButton(this, "unicodeButton");
	unicodeButton->setFont((*m_Item->doc()->AllFonts)[m_fontInUse]);
	unicodeLayout = new QVBoxLayout();
	unicodeLayout->addWidget(unicodeButton);

	userTable = new CharTable(this, 2, m_Item, m_fontInUse);
	userTable->setMaximumWidth(100);
	userTable->setMinimumWidth(100);
	userTable->enableDrops(true);
	unicodeLayout->addWidget(userTable);

	charPalettesLayout = new QHBoxLayout();
	charPalettesLayout->addWidget(charTable);
	charPalettesLayout->addLayout(unicodeLayout);
	zAuswahlLayout->addLayout(charPalettesLayout);
	
	layout3 = new QHBoxLayout;
	layout3->setSpacing( 6 );
	layout3->setMargin( 0 );
	
	layout2 = new QVBoxLayout;
	layout2->setSpacing( 6 );
	layout2->setMargin( 0 );

	insCode = new QLineEdit( this, "insText" );
	insCode->setMaxLength(4);
	insCode->setInputMask(">NNNN");
	insCode->clear();
	insText = new QLabel( insCode, tr("&Insert Code:"), this, "insText" );
	insCode->setFixedWidth(insText->width());
	layout2->addWidget( insText );
	layout2->addWidget( insCode );

	layout3->addLayout(layout2, Qt::AlignLeft);

	sample = new QLabel( this, "sample" );
	sample->setFrameShape(QFrame::Box);
	sample->setPaletteBackgroundColor(paletteBackgroundColor());
	sample->setMinimumHeight(52);
	sample->setMinimumWidth(460);
	layout3->addWidget( sample );
	zAuswahlLayout->addLayout( layout3 );

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
	delEdit();
//tooltips
	QToolTip::add( insertButton, "<qt>" + tr( "Insert the characters at the cursor in the text") + "</qt>");
	QToolTip::add( deleteButton, "<qt>" + tr( "Delete the current selection(s).") + "</qt>");
	QToolTip::add( closeButton, "<qt>" + tr( "Close this dialog and return to text editing") + "</qt>");
	QToolTip::add( insCode, "<qt>" + tr( "Type in a four digit unicode value directly here") + "</qt>");
	QToolTip::add( charTable, "<qt>" + tr("You can see a thumbnail if you press and hold down the right mouse button. The Insert key inserts a Glyph into the Selection below and the Delete key removes the last inserted one") + "</qt>");
	// signals and slots connections
	connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(delEdit()));
	connect(insertButton, SIGNAL(clicked()), this, SLOT(insChar()));
	//connect(charTable, SIGNAL(selectChar(uint, uint)), this, SLOT(newChar(uint, uint)));
	connect(charTable, SIGNAL(selectChar(uint)), this, SLOT(newChar(uint)));
	connect(charTable, SIGNAL(delChar()), this, SLOT(delChar()));
	//connect(userTable, SIGNAL(selectChar(uint, uint)), this, SLOT(newChar(uint, uint)));
	connect(userTable, SIGNAL(selectChar(uint)), this, SLOT(newChar(uint)));
	connect(userTable, SIGNAL(delChar()), this, SLOT(delChar()));
	connect(unicodeButton, SIGNAL(chosenUnicode(QString)), userTable, SLOT(appendUnicode(QString)));
	connect(fontSelector, SIGNAL(activated(int)), this, SLOT(newFont(int)));
	connect(rangeSelector, SIGNAL(activated(int)), this, SLOT(newCharClass(int)));
	connect(insCode, SIGNAL(returnPressed()), this, SLOT(newChar()));
	connect(insCode, SIGNAL(lostFocus()), this, SLOT(newChar()));
	setupRangeCombo();
	newCharClass(0);
}

void CharSelect::scanFont()
{
	uint  charcode;
	uint   gindex;
	QString   gname;
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
	QMap<uint, std::pair<QChar, QString> > glyphs;
	(*m_Item->doc()->AllFonts)[m_fontInUse].glyphNames(glyphs);
	for (QMap<uint, std::pair<QChar, QString> >::iterator it=glyphs.begin();
		 it != glyphs.end(); ++it)
	{
		charcode = it.data().first.unicode();
		gindex = it.key();
		gname = it.data().second;
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
	characters.clear();
	characters = allClasses[charClass];
	charTable->setCharacters(characters);
}

void CharSelect::newCharClass(int c)
{
	m_characterClass = usedCharClasses[c];
	generatePreview(m_characterClass);
}

void CharSelect::newFont(int font)
{
	QString oldFont(m_fontInUse);
	m_fontInUse = fontSelector->text(font);
	charTable->setFontInUse(m_fontInUse);
	userTable->setFontInUse(m_fontInUse);
	unicodeButton->setFont((*m_Item->doc()->AllFonts)[m_fontInUse]);
	(*m_Item->doc()->AllFonts)[m_fontInUse].increaseUsage();
	(*m_Item->doc()->AllFonts)[oldFont].decreaseUsage();
	delEdit();
	setCaption( tr( "Select Character:" )+" "+m_fontInUse );
	ScCore->primaryMainWindow()->SetNewFont(m_fontInUse);
	if (m_Item->doc()->currentStyle.charStyle().font().scName() != m_fontInUse)
	{
		disconnect(fontSelector, SIGNAL(activated(int)), this, SLOT(newFont(int)));
		fontSelector->RebuildList(m_Item->doc());
		m_fontInUse = m_Item->doc()->currentStyle.charStyle().font().scName();
		setCaption( tr( "Select Character:" )+" "+m_fontInUse );
		fontSelector->setCurrentText(m_fontInUse);
		connect(fontSelector, SIGNAL(activated(int)), this, SLOT(newFont(int)));
	}
	scanFont();
	generatePreview(0);
	m_characterClass = 0;
	setupRangeCombo();
}

void CharSelect::newChar()
{
	QString tx = insCode->text();
	tx.prepend("0x");
	bool ok = false;
	uint code = tx.toUInt(&ok, 16);
	if ((ok) && (code > 31))
	{
		chToIns += QChar(code);
		sample->setPixmap(FontSample((*m_Item->doc()->AllFonts)[m_fontInUse], 28, chToIns, paletteBackgroundColor(), true));
		insertButton->setEnabled(true);
	}
}

void CharSelect::newChar(uint i)
{
	chToIns += QChar(i);
	sample->setPixmap(FontSample((*m_Item->doc()->AllFonts)[m_fontInUse], 28, chToIns, paletteBackgroundColor(), true));
	insertButton->setEnabled(true);
	QString tmp;
	tmp.sprintf("%04X", i);
	insCode->setText(tmp);
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
	sample->setPixmap(FontSample((*m_Item->doc()->AllFonts)[m_fontInUse], 28, chToIns, paletteBackgroundColor(), true));
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
	if (needReturn)
	{
		m_characters = chToIns;
		delEdit();
		emit insertSpecialChar();
		return;
	}
	if (m_Item->HasSel)
		m_Item->asTextFrame()->deleteSelectedTextFromFrame();
	//CB: Avox please make text->insertchar(char) so none of this happens in gui code, and item can tell doc its changed so the view and mainwindow slotdocch are not necessary
	for (uint a=0; a<chToIns.length(); ++a)
	{
		QChar ch = chToIns.at(a);
		if (ch == QChar(10))
			ch = QChar(13);
		if (ch == QChar(9))
			ch = QChar(32);
		m_Item->itemText.insertChars(m_Item->CPos, ch);
		m_Item->CPos += 1;
	}
	m_Item->doc()->view()->DrawNew();
	m_Item->doc()->changed();
	delEdit();
}

bool CharSelect::eventFilter( QObject */*obj*/, QEvent *ev )
{
	if ( ev->type() == QEvent::Show )
	{
		charTable->recalcCellSizes();
		userTable->recalcCellSizes();
		return true;
	}
	return false;
}
