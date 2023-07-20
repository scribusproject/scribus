/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SEARCHREPLACE_H
#define SEARCHREPLACE_H

#include <QDialog>

class QCheckBox;
class QComboBox;
class QGridLayout;
class QGroupBox;
class QHBoxLayout;
class QLabel;
class QLineEdit;
class QPushButton;
class QVBoxLayout;

#include "scribusapi.h"

class ColorCombo;
class FontCombo;
class PageItem;
class PrefsContext;
class ScribusDoc;
class ScrSpinBox;
class ShadeButton;
class StoryText;
class StyleSelect;

class SCRIBUS_API SearchReplace : public QDialog
{
	Q_OBJECT

public:
	SearchReplace(QWidget* parent, ScribusDoc *doc, PageItem* item, bool mode = true );
	~SearchReplace() {};

	int firstMatchCursorPosition();
	void setSearchedText(const QString& text);

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
	PageItem* m_item { nullptr };
	ScribusDoc* m_doc { nullptr };

	PrefsContext* m_prefs { nullptr };
	bool m_found { false };
	bool m_itemMode;
	bool m_replacingAll { false };

	QVBoxLayout* SearchReplaceLayout { nullptr };
	QHBoxLayout* SelLayout { nullptr };
	QGridLayout* SearchLayout { nullptr };
	QGridLayout* ReplaceLayout { nullptr };
	QHBoxLayout* OptsLayout { nullptr };
	QHBoxLayout* ButtonsLayout { nullptr };

	ColorCombo* replaceFillValue { nullptr };
	ColorCombo* replaceStrokeValue { nullptr };
	ColorCombo* searchFillValue { nullptr };
	ColorCombo* searchStrokeValue { nullptr };
	FontCombo* replaceFontValue { nullptr };
	FontCombo* searchFontValue { nullptr };
	QCheckBox* ignoreCaseCheckBox { nullptr };
	QCheckBox* replaceAlignCheckBox { nullptr };
	QCheckBox* replaceEffectCheckBox { nullptr };
	QCheckBox* replaceFillCheckBox { nullptr };
	QCheckBox* replaceFillShadeCheckBox { nullptr };
	QCheckBox* replaceFontCheckBox { nullptr };
	QCheckBox* replaceSizeCheckBox { nullptr };
	QCheckBox* replaceStrokeCheckBox { nullptr };
	QCheckBox* replaceStrokeShadeCheckBox { nullptr };
	QCheckBox* replaceStyleCheckBox { nullptr };
	QCheckBox* replaceTextCheckBox { nullptr };
	QCheckBox* searchAlignCheckBox { nullptr };
	QCheckBox* searchEffectCheckBox { nullptr };
	QCheckBox* searchFillCheckBox { nullptr };
	QCheckBox* searchFillShadeCheckBox { nullptr };
	QCheckBox* searchFontCheckBox { nullptr };
	QCheckBox* searchSizeCheckBox { nullptr };
	QCheckBox* searchStrokeCheckBox { nullptr };
	QCheckBox* searchStrokeShadeCheckBox { nullptr };
	QCheckBox* searchStyleCheckBox { nullptr };
	QCheckBox* searchTextCheckBox { nullptr };
	QCheckBox* wholeWordCheckBox { nullptr };
	QComboBox* replaceAlignValue { nullptr };
	QComboBox* replaceStyleValue { nullptr };
	QComboBox* searchAlignValue { nullptr };
	QComboBox* searchStyleValue { nullptr };
	QGroupBox* replaceGroupBox { nullptr };
	QGroupBox* searchGroupBox { nullptr };
	QLabel* replaceTextLabel { nullptr };
	QLabel* searchTextLabel { nullptr };
	QLineEdit* replaceTextLineEdit { nullptr };
	QLineEdit* searchTextLineEdit { nullptr };
	QPushButton* clearButton { nullptr };
	QPushButton* closeButton { nullptr };
	QPushButton* replaceAllButton { nullptr };
	QPushButton* replaceButton { nullptr };
	QPushButton* searchButton { nullptr };
	ScrSpinBox* replaceSizeSpinBox { nullptr };
	ScrSpinBox* searchSizeSpinBox { nullptr };
	ShadeButton* replaceFillShadeValue { nullptr };
	ShadeButton* replaceStrokeShadeValue { nullptr };
	ShadeButton* searchFillShadeValue { nullptr };
	ShadeButton* searchStrokeShadeValue { nullptr };
	StyleSelect* replaceStyleEffectsValue { nullptr };
	StyleSelect* searchStyleEffectsValue { nullptr };

	virtual void doSearch();
	virtual void doSearch_itemMode();
	virtual void doSearch_storyEdMode();
	virtual int  doSearch_storyText(const StoryText& storyText, int start, int& textLen);
	virtual void doReplace();
	virtual void showNotFoundMessage();
	virtual void readPrefs();

	/// Number of matches found thus far in a search
	int m_matchesFound { 0 };
	int m_firstMatchPosition { -1 };

};

#endif // SEARCHREPLACE_H
