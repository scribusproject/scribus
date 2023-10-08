/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CHARSELECTENHANCED_H
#define CHARSELECTENHANCED_H

#include "scribusapi.h"
#include "scrpalettebase.h"
#include "chartablemodel.h"
#include "ui_charselectenhanced.h"


/*! \brief Character Palette for batch glyphs inserting.
Enhanced instance is created/destroyed on every call from CharSelect
to improve internal performance.
*/
class SCRIBUS_API CharSelectEnhanced : public ScrPaletteBase, public Ui::CharSelectEnhanced
{
	Q_OBJECT

public:
	CharSelectEnhanced(QWidget* parent);
	~CharSelectEnhanced() = default;

	void setDoc(ScribusDoc* doc);
	QString getUsedFont() const;

signals:
	/*! \brief A signal emitted when press the "Insert" button.
	Glyphs are handled in CharSelect parent.
	*/
	void insertSpecialChars(const QVector<uint> & chars);

public slots:
	void newChar(uint i, const QString&);
	void delChar();
	void newFont(int font);
	void newCharClass(int c);
	void delEdit();
	void insChar();

protected:
	void changeEvent(QEvent *e) override;

private:
	ScribusDoc* m_doc { nullptr };
	//! \brief Current font name
	QString m_fontInUse;
	//! \brief Currently selected character category. See usedCharClasses.
	int m_characterClass { 0 };

	//! \brief m_charTable model
	CharTableModel* m_charTableModel { nullptr };

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
	QVector<uint> chToIns;

	void scanFont();
	void setupRangeCombo();
	void generatePreview(int charClass);

private slots:
	void hexLineEdit_returnPressed();
};

#endif
