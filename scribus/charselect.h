/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CHARSELECT_H
#define CHARSELECT_H

#include "scribusapi.h"
#include "scrpalettebase.h"
#include "chartablemodel.h"
#include "ui_charselect.h"

class PageItem;


/*! \brief Character Palette for direct chars inserting. */
class SCRIBUS_API CharSelect : public ScrPaletteBase, public Ui::CharSelect
{
	Q_OBJECT

public:
	CharSelect(QWidget* parent);
	~CharSelect();

	void show();
	void setDoc(ScribusDoc* doc);
	void setEnabled(bool state, PageItem* item=0);

	const QString & fontInUse() const { return m_fontInUse; };
	void setFontInUse(const QString f) { m_fontInUse = f; };

	void setCharacterClass(int c) { m_characterClass = c; };

	const PageItem* item() const {return m_Item;};
	void setItem(PageItem* newItem) {m_Item=newItem;}
	const QString & getCharacters();
	void scanFont();
	void setupRangeCombo();
	void generatePreview(int charClass);

	CharTableModel * userTableModel() { return m_userTableModel; };

private:
	ScribusDoc* m_doc;
	//! \brief Current font name
	QString m_fontInUse;
	//! \brief Currently selected character category. See usedCharClasses.
	int m_characterClass;

	//! \brief A all font's character table widget.
// 	CharTableView* m_charTable;
	//! \brief m_charTable model
	CharTableModel * m_charTableModel;

	//! \brief User's defined char palette
// 	CharTableView* m_userTable;
	//! \brief m_userTable model
	CharTableModel * m_userTableModel;
// 	UnicodeChooseButton* unicodeButton;

	QList<CharClassDef> allClasses;
	CharClassDef characters;
	CharClassDef charactersFull;
	CharClassDef charactersLatin1;
	CharClassDef charactersLatin1Supplement;
	CharClassDef charactersLatinExtendedA;
	CharClassDef charactersLatinExtendedB;
	CharClassDef charactersGeneralPunctuation;
	CharClassDef charactersSuperSubscripts;
	CharClassDef charactersCurrencySymbols;
	CharClassDef charactersLetterlikeSymbols;
	CharClassDef charactersNumberForms;
	CharClassDef charactersArrows;
	CharClassDef charactersMathematicalOperators;
	CharClassDef charactersBoxDrawing;
	CharClassDef charactersBlockElements;
	CharClassDef charactersGeometricShapes;
	CharClassDef charactersMiscellaneousSymbols;
	CharClassDef charactersDingbats;
	CharClassDef charactersSmallFormVariants;
	CharClassDef charactersAlphabeticPresentationForms;
	CharClassDef charactersSpecial;
	CharClassDef charactersGreek;
	CharClassDef charactersGreekExtended;
	CharClassDef charactersCyrillic;
	CharClassDef charactersCyrillicSupplement;
	CharClassDef charactersArabic;
	CharClassDef charactersArabicPresentationFormsA;
	CharClassDef charactersArabicPresentationFormsB;
	CharClassDef charactersHebrew;
	QMap<int,int> usedCharClasses;
	QString chToIns;

public slots:
	void newChar(uint i);
	void userNewChar(uint i);
	void delChar();
	void newFont(int font);
	void newCharClass(int c);
	void delEdit();
	void insChar();

signals:
	/*! \brief A signall emitted when is the dialog modeless
	and user press the "Insert" button. */
	void insertSpecialChar();
	void insertUserSpecialChar(QChar);

protected:
	QString paletteFileMask;

	PageItem *m_Item;

	// GUI
// 	QGroupBox* m_bigPalette;
// 	QGroupBox* m_quickPalette;

// 	QLabel* insText;
// 	QLabel* sample;
// 	QLabel* fontLabel;
// 	QLabel* rangeLabel;

// 	FontCombo* fontSelector;
// 	QComboBox* rangeSelector;
// 	QPushButton* insertButton;
// 	QPushButton* deleteButton;
// 	QCheckBox* hideCheck;

// 	QPushButton *uniLoadButton;
// 	QPushButton *uniSaveButton;
// 	QPushButton *uniClearButton;

	void saveUserContent(QString f);
	void loadUserContent(QString f);

	protected slots:
		void hideCheck_clicked();
		void slot_insertSpecialChar();
		void slot_insertUserSpecialChar(QChar);
		void uniLoadButton_clicked();
		void uniSaveButton_clicked();
		void uniClearButton_clicked();
};

#endif
