/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QFileDialog>
#include <QMessageBox>

#include "charselectenhanced.h"
#include "fonts/scfontmetrics.h"
#include "scribusdoc.h"
#include "util.h"


CharSelectEnhanced::CharSelectEnhanced(QWidget* parent)
		: ScrPaletteBase(parent, "CharSelectEnhanced"),
		m_doc(0),
		m_characterClass(0)
{
	setupUi(this);

	m_charTableModel = new CharTableModel(this, 26, m_doc);//,
//                                           PrefsManager::instance()->appPrefs.toolSettings.defFont);
	m_charTable->setModel(m_charTableModel);
	m_charTable->setDragEnabled(true);

	// insert hex codes directly
	QValidator* insValidator = new QRegExpValidator(QRegExp("[A-F,a-f,0-9]{4}"), this);
	hexLineEdit->setValidator(insValidator);

	// signals and slots connections
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(delEdit()));
	connect(insertButton, SIGNAL(clicked()), this, SLOT(insChar()));
	connect(m_charTable, SIGNAL(selectChar(uint, QString)), this, SLOT(newChar(uint, QString)));
	connect(fontSelector, SIGNAL(activated(int)), this, SLOT(newFont(int)));
	connect(rangeSelector, SIGNAL(activated(int)), this, SLOT(newCharClass(int)));
	connect(hexLineEdit, SIGNAL(returnPressed()),
	        this, SLOT(hexLineEdit_returnPressed()));
}

CharSelectEnhanced::~CharSelectEnhanced()
{
}

void CharSelectEnhanced::setDoc(ScribusDoc* doc)
{
//     tDebug("CharSelectEnhanced setDoc start");
	if (m_doc != doc)
	{
		m_doc = doc;
		m_charTableModel->setDoc(m_doc);
		delEdit();
		setupRangeCombo();
		newCharClass(1);
	}

	QString oldFont(m_fontInUse);
	m_fontInUse = m_doc->currentStyle.charStyle().font().scName();
	if (oldFont != m_fontInUse && !m_fontInUse.isEmpty())
	{
		setCurrentComboItem(fontSelector, m_fontInUse);
		newFont(fontSelector->currentIndex());
	}
//     tDebug("CharSelectEnhanced setDoc end");
}

QString CharSelectEnhanced::getUsedFont()
{
	return m_fontInUse;
}

void CharSelectEnhanced::scanFont()
{
//     tDebug("scanFont start");
	uint  charcode;
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
	(*m_doc->AllFonts)[m_fontInUse].glyphNames(glyphs);
	for (QMap<uint, std::pair<QChar, QString> >::iterator it=glyphs.begin();
	        it != glyphs.end(); ++it)
	{
		charcode = it.value().first.unicode();
		gname = it.value().second;
		charactersFull.append(charcode);
		if ((charcode >= 0x0020) && (charcode <= 0x007F))
			charactersLatin1.append(charcode);
		else if ((charcode >= 0x0080) && (charcode <= 0x00FF))
			charactersLatin1Supplement.append(charcode);
		else if ((charcode >= 0x0100) && (charcode <= 0x017F))
			charactersLatinExtendedA.append(charcode);
		else if ((charcode >= 0x0180) && (charcode <= 0x024F))
			charactersLatinExtendedB.append(charcode);
		else if ((charcode >= 0x0370) && (charcode <= 0x03FF))
			charactersGreek.append(charcode);
		else if ((charcode >= 0x0400) && (charcode <= 0x04FF))
			charactersCyrillic.append(charcode);
		else if ((charcode >= 0x0500) && (charcode <= 0x052F))
			charactersCyrillicSupplement.append(charcode);
		else if ((charcode >= 0x0590) && (charcode <= 0x05FF))
			charactersHebrew.append(charcode);
		else if ((charcode >= 0x0600) && (charcode <= 0x06FF))
			charactersArabic.append(charcode);
		else if ((charcode >= 0x1F00) && (charcode <= 0x1FFF))
			charactersGreekExtended.append(charcode);
		else if ((charcode >= 0x2000) && (charcode <= 0x206F))
			charactersGeneralPunctuation.append(charcode);
		else if ((charcode >= 0x2070) && (charcode <= 0x209F))
			charactersSuperSubscripts.append(charcode);
		else if ((charcode >= 0x20A0) && (charcode <= 0x20CF))
			charactersCurrencySymbols.append(charcode);
		else if ((charcode >= 0x2100) && (charcode <= 0x214F))
			charactersLetterlikeSymbols.append(charcode);
		else if ((charcode >= 0x2150) && (charcode <= 0x218F))
			charactersNumberForms.append(charcode);
		else if ((charcode >= 0x2190) && (charcode <= 0x21FF))
			charactersArrows.append(charcode);
		else if ((charcode >= 0x2200) && (charcode <= 0x22FF))
			charactersMathematicalOperators.append(charcode);
		else if ((charcode >= 0x2500) && (charcode <= 0x257F))
			charactersBoxDrawing.append(charcode);
		else if ((charcode >= 0x2580) && (charcode <= 0x259F))
			charactersBlockElements.append(charcode);
		else if ((charcode >= 0x25A0) && (charcode <= 0x25FF))
			charactersGeometricShapes.append(charcode);
		else if ((charcode >= 0x2600) && (charcode <= 0x26FF))
			charactersMiscellaneousSymbols.append(charcode);
		else if ((charcode >= 0x2700) && (charcode <= 0x27BF))
			charactersDingbats.append(charcode);
		else if ((charcode >= 0xFE50) && (charcode <= 0xFE6F))
			charactersSmallFormVariants.append(charcode);
		else if ((charcode >= 0xFB00) && (charcode <= 0xFB4F))
			charactersAlphabeticPresentationForms.append(charcode);
		else if ((charcode >= 0xFB50) && (charcode <= 0xFDFF))
			charactersArabicPresentationFormsA.append(charcode);
		else if ((charcode >= 0xFE70) && (charcode <= 0xFEFF))
			charactersArabicPresentationFormsB.append(charcode);
		else if ((charcode >= 0xFFF0) && (charcode <= 0xFFFF))
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
//     tDebug("scanFont end");
}

void CharSelectEnhanced::setupRangeCombo()
{
//     tDebug("setupRangeCombo start");
	disconnect(rangeSelector, SIGNAL(activated(int)), this, SLOT(newCharClass(int)));
	int counter = 0;
	rangeSelector->clear();
	rangeSelector->addItem( tr("Full Character Set"));
	usedCharClasses.insert(counter, 0);
	counter++;
	if (charactersLatin1.count() != 0)
	{
		rangeSelector->addItem( tr("Basic Latin"));
		usedCharClasses.insert(counter, 1);
		counter++;
	}
	if (charactersLatin1Supplement.count() != 0)
	{
		rangeSelector->addItem( tr("Latin-1 Supplement"));
		usedCharClasses.insert(counter, 2);
		counter++;
	}
	if (charactersLatinExtendedA.count() != 0)
	{
		rangeSelector->addItem( tr("Latin Extended-A"));
		usedCharClasses.insert(counter, 3);
		counter++;
	}
	if (charactersLatinExtendedB.count() != 0)
	{
		rangeSelector->addItem( tr("Latin Extended-B"));
		usedCharClasses.insert(counter, 4);
		counter++;
	}
	if (charactersGeneralPunctuation.count() != 0)
	{
		rangeSelector->addItem( tr("General Punctuation"));
		usedCharClasses.insert(counter, 5);
		counter++;
	}
	if (charactersSuperSubscripts.count() != 0)
	{
		rangeSelector->addItem( tr("Super- and Subscripts"));
		usedCharClasses.insert(counter, 6);
		counter++;
	}
	if (charactersCurrencySymbols.count() != 0)
	{
		rangeSelector->addItem( tr("Currency Symbols"));
		usedCharClasses.insert(counter, 7);
		counter++;
	}
	if (charactersLetterlikeSymbols.count() != 0)
	{
		rangeSelector->addItem( tr("Letterlike Symbols"));
		usedCharClasses.insert(counter, 8);
		counter++;
	}
	if (charactersNumberForms.count() != 0)
	{
		rangeSelector->addItem( tr("Number Forms"));
		usedCharClasses.insert(counter, 9);
		counter++;
	}
	if (charactersArrows.count() != 0)
	{
		rangeSelector->addItem( tr("Arrows"));
		usedCharClasses.insert(counter, 10);
		counter++;
	}
	if (charactersMathematicalOperators.count() != 0)
	{
		rangeSelector->addItem( tr("Mathematical Operators"));
		usedCharClasses.insert(counter, 11);
		counter++;
	}
	if (charactersBoxDrawing.count() != 0)
	{
		rangeSelector->addItem( tr("Box Drawing"));
		usedCharClasses.insert(counter, 12);
		counter++;
	}
	if (charactersBlockElements.count() != 0)
	{
		rangeSelector->addItem( tr("Block Elements"));
		usedCharClasses.insert(counter, 13);
		counter++;
	}
	if (charactersGeometricShapes.count() != 0)
	{
		rangeSelector->addItem( tr("Geometric Shapes"));
		usedCharClasses.insert(counter, 14);
		counter++;
	}
	if (charactersMiscellaneousSymbols.count() != 0)
	{
		rangeSelector->addItem( tr("Miscellaneous Symbols"));
		usedCharClasses.insert(counter, 15);
		counter++;
	}
	if (charactersDingbats.count() != 0)
	{
		rangeSelector->addItem( tr("Dingbats"));
		usedCharClasses.insert(counter, 16);
		counter++;
	}
	if (charactersSmallFormVariants.count() != 0)
	{
		rangeSelector->addItem( tr("Small Form Variants"));
		usedCharClasses.insert(counter, 17);
		counter++;
	}
	if (charactersAlphabeticPresentationForms.count() != 0)
	{
		rangeSelector->addItem( tr("Ligatures"));
		usedCharClasses.insert(counter, 18);
		counter++;
	}
	if (charactersSpecial.count() != 0)
	{
		rangeSelector->addItem( tr("Specials"));
		usedCharClasses.insert(counter, 19);
		counter++;
	}
	if (charactersGreek.count() != 0)
	{
		rangeSelector->addItem( tr("Greek"));
		usedCharClasses.insert(counter, 20);
		counter++;
	}
	if (charactersGreekExtended.count() != 0)
	{
		rangeSelector->addItem( tr("Greek Extended"));
		usedCharClasses.insert(counter, 21);
		counter++;
	}
	if (charactersCyrillic.count() != 0)
	{
		rangeSelector->addItem( tr("Cyrillic"));
		usedCharClasses.insert(counter, 22);
		counter++;
	}
	if (charactersCyrillicSupplement.count() != 0)
	{
		rangeSelector->addItem( tr("Cyrillic Supplement"));
		usedCharClasses.insert(counter, 23);
		counter++;
	}
	if (charactersArabic.count() != 0)
	{
		rangeSelector->addItem( tr("Arabic"));
		usedCharClasses.insert(counter, 24);
		counter++;
	}
	if (charactersArabicPresentationFormsA.count() != 0)
	{
		rangeSelector->addItem( tr("Arabic Extended A"));
		usedCharClasses.insert(counter, 25);
		counter++;
	}
	if (charactersArabicPresentationFormsB.count() != 0)
	{
		rangeSelector->addItem( tr("Arabic Extended B"));
		usedCharClasses.insert(counter, 26);
		counter++;
	}
	if (charactersHebrew.count() != 0)
	{
		rangeSelector->addItem( tr("Hebrew"));
		usedCharClasses.insert(counter, 27);
		counter++;
	}
	rangeSelector->setCurrentIndex(0);
	connect(rangeSelector, SIGNAL(activated(int)), this, SLOT(newCharClass(int)));
//     tDebug("setupRangeCombo end");
}

void CharSelectEnhanced::generatePreview(int charClass)
{
//     tDebug("generatePreview start");
	characters.clear();
	if (charClass>=0 && charClass<allClasses.count())
		characters = allClasses[charClass];
	m_charTableModel->setCharacters(characters);
//     tDebug("generatePreview end");
}

void CharSelectEnhanced::newCharClass(int c)
{
//     tDebug("newCharClass start");
	m_characterClass = usedCharClasses[c];
	generatePreview(m_characterClass);
//     tDebug("newCharClass end");
}

void CharSelectEnhanced::newFont(int font)
{
//     tDebug("newFont start");
	QString oldFont(m_fontInUse);
	m_fontInUse = fontSelector->itemText(font);
	if (!m_fontInUse.isEmpty())
	{
		m_charTableModel->setFontInUse(m_fontInUse);
		(*m_doc->AllFonts)[m_fontInUse].increaseUsage();
		if (!oldFont.isEmpty())
			(*m_doc->AllFonts)[oldFont].decreaseUsage();
		delEdit();
		scanFont();
		generatePreview(0);
		m_characterClass = 0;
		setupRangeCombo();
	}
//     tDebug("newFont end");
}

void CharSelectEnhanced::newChar(uint i, QString)
{
	chToIns += QChar(i);
	sample->setPixmap(FontSample((*m_doc->AllFonts)[m_fontInUse], 28, chToIns, palette().color(QPalette::Window), true));
	insertButton->setEnabled(true);
}

void CharSelectEnhanced::delChar()
{
	if (chToIns.length() == 0)
		return;
	if (chToIns.length() == 1)
	{
		delEdit();
		return;
	}
	chToIns.truncate(chToIns.length() - 1);
	sample->setPixmap(FontSample((*m_doc->AllFonts)[m_fontInUse], 28, chToIns, palette().color(QPalette::Window), true));
	insertButton->setEnabled(chToIns.length() > 0);
}

void CharSelectEnhanced::delEdit()
{
	chToIns = "";
	QPixmap pm(1,28);
	pm.fill(palette().color(QPalette::Window));
	sample->setPixmap(pm);
	insertButton->setEnabled(false);
}

void CharSelectEnhanced::insChar()
{
	// #7810 : won't work as insertButton->hasFocus()
	// always return false on OSX
	/*if (!insertButton->hasFocus())
		return;*/
	emit insertSpecialChars(chToIns);
	delEdit();
}

void CharSelectEnhanced::hexLineEdit_returnPressed()
{
	QString tx("0x%1");;
	bool ok = false;
	uint code = tx.arg(hexLineEdit->text()).toUInt(&ok, 16);
	if ((ok) && (code > 31))
		newChar(code, m_fontInUse);
}

void CharSelectEnhanced::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		Ui::CharSelectEnhanced::retranslateUi(this);
		int i=rangeSelector->currentIndex();
		setupRangeCombo();
		disconnect(rangeSelector, SIGNAL(activated(int)), this, SLOT(newCharClass(int)));
		rangeSelector->setCurrentIndex(i);
		connect(rangeSelector, SIGNAL(activated(int)), this, SLOT(newCharClass(int)));
	}
	else
		QWidget::changeEvent(e);
}
