/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

//#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

#include "charselect.h"
#include "chartableview.h"
#include "commonstrings.h"
#include "fontcombo.h"
#include "fonts/scfontmetrics.h"
#include "pageitem_textframe.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "sccombobox.h"
#include "scpaths.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "unicodesearch.h"
#include "util.h"
#include "util_icon.h"


CharSelect::CharSelect(QWidget* parent)
	: ScrPaletteBase(parent, "CharSelect"),
	m_doc(0),
	m_characterClass(0),
	m_Item(0)
{
	setupUi(this);

	paletteFileMask = tr("Scribus Char Palette (*.ucp);;All Files (*)");

	m_charTableModel = new CharTableModel(m_bigPalette, 26, m_doc,
										  PrefsManager::instance()->appPrefs.toolSettings.defFont);
	m_charTable->setModel(m_charTableModel);
	m_charTable->setDragEnabled(true);

	hideButton->setIcon(loadIcon("22/insert-table.png"));
	unicodeButton->setIcon(loadIcon("find.png"));
	uniLoadButton->setIcon(loadIcon("22/document-open.png"));
	uniSaveButton->setIcon(loadIcon("22/document-save.png"));
	uniClearButton->setIcon(loadIcon("22/document-new.png"));
	
	m_userTableModel = new CharTableModel(m_quickPalette, 6, m_doc,
										  PrefsManager::instance()->appPrefs.toolSettings.defFont);
	m_userTable->setModel(m_userTableModel);
	m_userTable->setAcceptDrops(true);

	// signals and slots connections
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(delEdit()));
	connect(insertButton, SIGNAL(clicked()), this, SLOT(insChar()));
	connect(m_charTable, SIGNAL(selectChar(uint)), this, SLOT(newChar(uint)));
	connect(m_charTable, SIGNAL(delChar()), this, SLOT(delChar()));
	connect(m_userTable, SIGNAL(selectChar(uint)), this, SLOT(userNewChar(uint)));
	connect(m_userTable, SIGNAL(delChar()), this, SLOT(delChar()));
	connect(m_userTableModel, SIGNAL(selectionChanged(QItemSelectionModel*)),
			m_userTable, SLOT(modelSelectionChanged(QItemSelectionModel*)));
	connect(unicodeButton, SIGNAL(chosenUnicode(QString)), m_userTableModel, SLOT(appendUnicode(QString)));
	connect(fontSelector, SIGNAL(activated(int)), this, SLOT(newFont(int)));
	connect(rangeSelector, SIGNAL(activated(int)), this, SLOT(newCharClass(int)));
	connect(hideButton, SIGNAL(toggled(bool)), this, SLOT(hideButton_toggled(bool)));
	connect(this, SIGNAL(insertSpecialChar()), this, SLOT(slot_insertSpecialChar()));
	connect(this, SIGNAL(insertUserSpecialChar(QChar)), this, SLOT(slot_insertUserSpecialChar(QChar)));
	connect(uniLoadButton, SIGNAL(clicked()), this, SLOT(uniLoadButton_clicked()));
	connect(uniSaveButton, SIGNAL(clicked()), this, SLOT(uniSaveButton_clicked()));
	connect(uniClearButton, SIGNAL(clicked()), this, SLOT(uniClearButton_clicked()));

	loadUserContent(ScPaths::getApplicationDataDir() + "charpalette.ucp");
	setupRangeCombo();
	newCharClass(0);
}

CharSelect::~CharSelect()
{
}

void CharSelect::setDoc(ScribusDoc* doc)
{
	if (m_doc != doc)
	{
		m_doc = doc;
		m_charTableModel->setDoc(m_doc);
		m_userTableModel->setDoc(m_doc);
		delEdit();
		setupRangeCombo();
		newCharClass(0);
	}

	if (!m_doc)
	{
		saveUserContent(ScPaths::getApplicationDataDir() + "charpalette.ucp");
		return;
	}

	QString oldFont(m_fontInUse);
	m_fontInUse = m_doc->currentStyle.charStyle().font().scName();
	if (oldFont != m_fontInUse && !m_fontInUse.isEmpty())
	{
		setCurrentComboItem(fontSelector, m_fontInUse);
		newFont(fontSelector->currentIndex());
		unicodeButton->setFont((*m_doc->AllFonts)[m_fontInUse]);
	}
}

const QString & CharSelect::getCharacters()
{
	return chToIns;
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
	(*m_doc->AllFonts)[m_fontInUse].glyphNames(glyphs);
	for (QMap<uint, std::pair<QChar, QString> >::iterator it=glyphs.begin();
		 it != glyphs.end(); ++it)
	{
		charcode = it.value().first.unicode();
		gindex = it.key();
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
		else if ((charcode >= 0x2190) && (charcode <= 0x2190))
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
}

void CharSelect::setupRangeCombo()
{
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
}

void CharSelect::generatePreview(int charClass)
{
	characters.clear();
	if (charClass>=0 && charClass<allClasses.count())
		characters = allClasses[charClass];
	m_charTableModel->setCharacters(characters);
}

void CharSelect::newCharClass(int c)
{
	m_characterClass = usedCharClasses[c];
	generatePreview(m_characterClass);
}

void CharSelect::newFont(int font)
{
	QString oldFont(m_fontInUse);
	m_fontInUse = fontSelector->itemText(font);
	if (!m_fontInUse.isEmpty())
	{
		m_charTableModel->setFontInUse(m_fontInUse);
		m_userTableModel->setFontInUse(m_fontInUse);
		unicodeButton->setFont((*m_doc->AllFonts)[m_fontInUse]);
		(*m_doc->AllFonts)[m_fontInUse].increaseUsage();
		if (!oldFont.isEmpty())
			(*m_doc->AllFonts)[oldFont].decreaseUsage();
		delEdit();
		scanFont();
		generatePreview(0);
		m_characterClass = 0;
		setupRangeCombo();
	}
}

void CharSelect::userNewChar(uint i)
{
	emit insertUserSpecialChar(QChar(i));
}

void CharSelect::newChar(uint i)
{
	chToIns += QChar(i);
	sample->setPixmap(FontSample((*m_doc->AllFonts)[m_fontInUse], 28, chToIns, palette().color(QPalette::Window), true));
	insertButton->setEnabled(true);
	QString tmp;
	tmp.sprintf("%04X", i);
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
	sample->setPixmap(FontSample((*m_doc->AllFonts)[m_fontInUse], 28, chToIns, palette().color(QPalette::Window), true));
	insertButton->setEnabled(true);
}

void CharSelect::delEdit()
{
	chToIns = "";
	QPixmap pm(1,28);
	pm.fill(palette().color(QPalette::Window));
	sample->setPixmap(pm);
	insertButton->setEnabled(false);
}

void CharSelect::insChar()
{
	emit insertSpecialChar();
}

void CharSelect::slot_insertSpecialChar()
{
	if (!m_Item)
		return;

	if (m_Item->HasSel)
		m_Item->asTextFrame()->deleteSelectedTextFromFrame();
	if (m_Item->asTextFrame())
		m_Item->asTextFrame()->invalidateLayout();
	//CB: Avox please make text->insertchar(char) so none of this happens in gui code, and item can tell doc its changed so the view and mainwindow slotdocch are not necessary
	QChar ch;
	for (int a=0; a<chToIns.length(); ++a)
	{
		ch = chToIns.at(a);
		if (ch == QChar(10))
			ch = QChar(13);
		if (ch == QChar(9))
			ch = QChar(32);
		m_Item->itemText.insertChars(m_Item->CPos, ch, true);
		m_Item->CPos += 1;
	}
	m_doc->updateFrameItems();
	m_doc->view()->DrawNew();
	m_doc->changed();
	delEdit();
}

void CharSelect::slot_insertUserSpecialChar(QChar ch)
{
	if (!m_Item)
		return;
	if (m_Item->HasSel)
		m_Item->asTextFrame()->deleteSelectedTextFromFrame();
	if (m_Item->asTextFrame())
		m_Item->asTextFrame()->invalidateLayout();
// 	//CB: Avox please make text->insertchar(char) so none of this happens in gui code, and item can tell doc its changed so the view and mainwindow slotdocch are not necessary
	if (ch == QChar(10))
		ch = QChar(13);
	if (ch == QChar(9))
		ch = QChar(32);
	m_Item->itemText.insertChars(m_Item->CPos, ch, true);
	m_doc->updateFrameItems();
	m_Item->CPos += 1;
	m_doc->view()->DrawNew();
	m_doc->changed();
}

void CharSelect::hideButton_toggled(bool state)
{
	// megahact #1 to keep user palette unchanged after resizing
	QSize sz(m_quickPalette->size());

	m_bigPalette->setShown(!state);
	if (state)
		resize(sz); // megahack #2 to keep palette small
	m_quickPalette->resize(sz);
	updateGeometry();
	adjustSize();
}

void CharSelect::show()
{
	ScrPaletteBase::show();
	PrefsContext* p = PrefsManager::instance()->prefsFile->getContext("charpalette");
	bool state = p->getBool("hideCheck", true);
	// always open big palette when there is no glyph in the custom one
	if (state && m_userTableModel->characters().count() == 0)
		state = false;
	hideButton->setChecked(state);
	hideButton_toggled(state);
}

void CharSelect::hide()
{
	ScrPaletteBase::hide();
	PrefsContext* p = PrefsManager::instance()->prefsFile->getContext("charpalette");
	p->set("hideCheck", hideButton->isChecked());
}

void CharSelect::setEnabled(bool state, PageItem* item)
{
	ScrPaletteBase::setEnabled(state);
	m_Item = item;
	if (state)
		setDoc(m_doc);
}

void CharSelect::uniLoadButton_clicked()
{
	QString f = QFileDialog::getOpenFileName(this, tr("Choose a filename to open"), QDir::currentPath(), paletteFileMask);
	if (!f.isNull())
		loadUserContent(f);
}

void CharSelect::loadUserContent(QString f)
{
	CharClassDef newChars;
	QFile file(f);
	if (!file.exists())
		return;
	if (file.open(QIODevice::ReadOnly))
	{
		QTextStream stream(&file);
		QString line;
		while (!stream.atEnd())
		{
			bool ok = false;
			line = stream.readLine();
			if (line.left(1) == "#")
				continue; // don't mess with a comment
			int val = line.toInt(&ok, 10);
			if (ok)
				newChars.append(val);
			else
			{
				QMessageBox::warning(this, tr("Error"),
									 "<qt>" + tr("Error reading file %1 - file is corrupted propably.").arg(f) + "</qt>",
									 QMessageBox::Ok, QMessageBox::NoButton);
				break;
			}
		}
		file.close();
		m_userTableModel->setCharacters(newChars);
	}
}

void CharSelect::uniSaveButton_clicked()
{
	if (m_userTableModel->characters().count() == 0)
		return;
	QString f = QFileDialog::getSaveFileName(this, tr("Choose a filename to save under"), QDir::currentPath(), paletteFileMask);
	if (f.isNull() || !overwrite(this, f))
		return;
	saveUserContent(f);
}

void CharSelect::saveUserContent(QString f)
{
	QFile file(f);
	if (file.open(QIODevice::WriteOnly))
	{
		QTextStream stream(&file);
		CharClassDef chars = m_userTableModel->characters();
		stream << "# This is a character palette file for Scribus\n";
		for (CharClassDef::Iterator it = chars.begin(); it != chars.end(); ++it)
			stream << (*it) << "\n";
		file.close();
	}
	else
		QMessageBox::warning(this, tr("Error"),
					 "<qt>" + tr("Cannot write file %1").arg(f) + "</qt>",
					 QMessageBox::Ok, QMessageBox::NoButton);
}

void CharSelect::uniClearButton_clicked()
{
	if (m_userTableModel->characters().count() > 0
		&&
		!QMessageBox::question(this, tr("Empty the Palette?"),
					 "<qt>" + tr("You will remove all characters from this palette. Are you sure?") + "</qt>",
					 CommonStrings::trYesKey, CommonStrings::trNoKey,
					 QString::null, 0, 1 )
	   )
	{
		m_userTableModel->setCharacters(CharClassDef());
	}
}

void CharSelect::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		Ui::CharSelect::retranslateUi(this);
		int i=rangeSelector->currentIndex();
		setupRangeCombo();
		disconnect(rangeSelector, SIGNAL(activated(int)), this, SLOT(newCharClass(int)));
		rangeSelector->setCurrentIndex(i);
		connect(rangeSelector, SIGNAL(activated(int)), this, SLOT(newCharClass(int)));
	}
	else
		QWidget::changeEvent(e);
}
