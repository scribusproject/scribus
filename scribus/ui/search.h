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
	SearchReplace( QWidget* parent, ScribusDoc *doc, PageItem* ite, bool mode = true );
	~SearchReplace() {};
	virtual void slotDoSearch();
	virtual void slotDoReplace();

	QLabel* SText1;
	QLabel* RText1;
	QGroupBox* Search;
	QCheckBox* SStroke;
	QCheckBox* SFill;
	QCheckBox* SStrokeS;
	QCheckBox* SFillS;
	QCheckBox* SSize;
	QCheckBox* SFont;
	QCheckBox* SStyle;
	QCheckBox* SAlign;
	QCheckBox* SText;
	QCheckBox* SEffect;
	QCheckBox* REffect;
	QLineEdit* STextVal;
	QComboBox* SStyleVal;
	QComboBox* SAlignVal;
	FontCombo* SFontVal;
	ScrSpinBox* SSizeVal;
	ColorCombo* SFillVal;
	ShadeButton *SFillSVal;
	ColorCombo* SStrokeVal;
	ShadeButton *SStrokeSVal;
	QGroupBox* Replace;
	QCheckBox* RStroke;
	QCheckBox* RStrokeS;
	QCheckBox* RFill;
	QCheckBox* RFillS;
	QCheckBox* RSize;
	QCheckBox* RFont;
	QCheckBox* RStyle;
	QCheckBox* RAlign;
	QCheckBox* RText;
	QLineEdit* RTextVal;
	QComboBox* RStyleVal;
	QComboBox* RAlignVal;
	FontCombo* RFontVal;
	ScrSpinBox* RSizeVal;
	ColorCombo* RFillVal;
	ShadeButton *RFillSVal;
	ColorCombo* RStrokeVal;
	ShadeButton *RStrokeSVal;
	StyleSelect* SEffVal;
	StyleSelect* REffVal;
	QCheckBox* Word;
	QCheckBox* CaseIgnore;
	QPushButton* DoSearch;
	QPushButton* DoReplace;
	QPushButton* AllReplace;
	QPushButton* clearButton;
	QPushButton* Leave;
	PageItem* Item;
	ScribusDoc* Doc;
	uint ReplStart;
	PrefsContext* prefs;
	bool NotFound;
	bool SMode;

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
	virtual void writePrefs();
	virtual void clear();

signals:
	void NewFont(const QString&);
	void NewAbs(int);

protected:
	QVBoxLayout* SearchReplaceLayout;
	QHBoxLayout* SelLayout;
	QGridLayout* SearchLayout;
	QGridLayout* ReplaceLayout;
	QHBoxLayout* OptsLayout;
	QHBoxLayout* ButtonsLayout;
	virtual void readPrefs();

	/// Number of matches found thus far in a search
	int matchesFound;

};

#endif // SEARCHREPLACE_H
