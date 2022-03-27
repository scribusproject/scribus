/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SEARCHREPLACE_H
#define SEARCHREPLACE_H

#include <QDialog>
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLineEdit;
class QPushButton;
class QLabel;

#include "scribusapi.h"
class ScrSpinBox;
class FontCombo;
class StyleSelect;
class ShadeButton;
class PrefsContext;
class ColorCombo;
class ScribusDoc;
class PageItem;

class SCRIBUS_API SearchReplace : public QDialog
{
	Q_OBJECT

public:
	SearchReplace(QWidget* parent, ScribusDoc *doc, PageItem* item, bool mode = true );
	~SearchReplace() {};

	int firstMatchCursorPosition() const;
	void setSearchedText(const QString& text);

	ColorCombo* replaceFillValue;
	ColorCombo* replaceStrokeValue;
	ColorCombo* searchFillValue;
	ColorCombo* searchStrokeValue;
	FontCombo* replaceFontValue;
	FontCombo* searchFontValue;
	QCheckBox* ignoreCaseCheckBox;
	QCheckBox* replaceAlignCheckBox;
	QCheckBox* replaceEffectCheckBox;
	QCheckBox* replaceFillCheckBox;
	QCheckBox* replaceFillShadeCheckBox;
	QCheckBox* replaceFontCheckBox;
	QCheckBox* replaceSizeCheckBox;
	QCheckBox* replaceStrokeCheckBox;
	QCheckBox* replaceStrokeShadeCheckBox;
	QCheckBox* replaceStyleCheckBox;
	QCheckBox* replaceTextCheckBox;
	QCheckBox* searchAlignCheckBox;
	QCheckBox* searchEffectCheckBox;
	QCheckBox* searchFillCheckBox;
	QCheckBox* searchFillShadeCheckBox;
	QCheckBox* searchFontCheckBox;
	QCheckBox* searchSizeCheckBox;
	QCheckBox* searchStrokeCheckBox;
	QCheckBox* searchStrokeShadeCheckBox;
	QCheckBox* searchStyleCheckBox;
	QCheckBox* searchTextCheckBox;
	QCheckBox* wholeWordCheckBox;
	QComboBox* replaceAlignValue;
	QComboBox* replaceStyleValue;
	QComboBox* searchAlignValue;
	QComboBox* searchStyleValue;
	QGroupBox* replaceGroupBox;
	QGroupBox* searchGroupBox;
	QLabel* replaceTextLabel;
	QLabel* searchTextLabel;
	QLineEdit* replaceTextLineEdit;
	QLineEdit* searchTextLineEdit;
	QPushButton* clearButton;
	QPushButton* closeButton;
	QPushButton* replaceAllButton;
	QPushButton* replaceButton;
	QPushButton* searchButton;
	ScrSpinBox* replaceSizeSpinBox;
	ScrSpinBox* searchSizeSpinBox;
	ShadeButton *replaceFillShadeValue;
	ShadeButton *replaceStrokeShadeValue;
	ShadeButton *searchFillShadeValue;
	ShadeButton *searchStrokeShadeValue;
	StyleSelect* replaceStyleEffectsValue;
	StyleSelect* searchStyleEffectsValue;

public slots:
	virtual void slotSearch();
	virtual void slotReplace();
	virtual void slotReplaceAll();
	virtual void enableTxSearch();
	virtual void enableStyleSearch();
	virtual void enableAlignSearch();
	virtual void enableFontSearch();
	virtual void enableSizeSearch();
	virtual void enableEffSearch();
	virtual void enableFillSearch();
	virtual void enableFillSSearch();
	virtual void enableStrokeSearch();
	virtual void enableStrokeSSearch();
	virtual void enableTxReplace();
	virtual void enableStyleReplace();
	virtual void enableAlignReplace();
	virtual void enableFontReplace();
	virtual void enableSizeReplace();
	virtual void enableEffReplace();
	virtual void enableFillReplace();
	virtual void enableFillSReplace();
	virtual void enableStrokeReplace();
	virtual void enableStrokeSReplace();
	virtual void updateReplaceButtonsState();
	virtual void updateSearchButtonState();
	virtual void writePrefs();
	virtual void clear();

protected:
	PageItem* m_item;
	ScribusDoc* m_doc;

	uint m_replStart;
	PrefsContext* m_prefs;
	bool m_found { false };
	bool m_itemMode;
	bool m_replacingAll { false };

	QVBoxLayout* SearchReplaceLayout;
	QHBoxLayout* SelLayout;
	QGridLayout* SearchLayout;
	QGridLayout* ReplaceLayout;
	QHBoxLayout* OptsLayout;
	QHBoxLayout* ButtonsLayout;

	virtual void doSearch();
	virtual void doReplace();
	virtual void showNotFoundMessage();
	virtual void readPrefs();

	/// Number of matches found thus far in a search
	int m_matchesFound { 0 };
	int m_firstMatchPosition { -1 };

};

#endif // SEARCHREPLACE_H
