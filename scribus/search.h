#ifndef SEARCHREPLACE_H
#define SEARCHREPLACE_H

#include <qvariant.h>
#include <qdialog.h>

#include "scribusstructs.h"
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLineEdit;
class QPushButton;
class MSpinBox;
class FontCombo;
class StyleSelect;
class ShadeButton;
class PrefsContext;
class QLabel;

class ScribusDoc;
class PageItem;

class SearchReplace : public QDialog
{
	Q_OBJECT

public:
	SearchReplace( QWidget* parent, ScribusDoc *doc, preV *Prefs, PageItem* ite, bool mode = true );
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
	QCheckBox* SText;
	QCheckBox* SEffect;
	QCheckBox* REffect;
	QLineEdit* STextVal;
	QComboBox* SStyleVal;
	FontCombo* SFontVal;
	MSpinBox* SSizeVal;
	QComboBox* SFillVal;
	ShadeButton *SFillSVal;
	QComboBox* SStrokeVal;
	ShadeButton *SStrokeSVal;
	QGroupBox* Replace;
	QCheckBox* RStroke;
	QCheckBox* RStrokeS;
	QCheckBox* RFill;
	QCheckBox* RFillS;
	QCheckBox* RSize;
	QCheckBox* RFont;
	QCheckBox* RStyle;
	QCheckBox* RText;
	QLineEdit* RTextVal;
	QComboBox* RStyleVal;
	FontCombo* RFontVal;
	MSpinBox* RSizeVal;
	QComboBox* RFillVal;
	ShadeButton *RFillSVal;
	QComboBox* RStrokeVal;
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
	virtual void enableFontSearch();
	virtual void enableSizeSearch();
	virtual void enableEffSearch();
	virtual void enableFillSearch();
	virtual void enableFillSSearch();
	virtual void enableStrokeSearch();
	virtual void enableStrokeSSearch();
	virtual void enableTxReplace();
	virtual void enableStyleReplace();
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
	void NewFont(QString);
	void NewAbs(int);

protected:
	QVBoxLayout* SearchReplaceLayout;
	QHBoxLayout* SelLayout;
	QGridLayout* SearchLayout;
	QGridLayout* ReplaceLayout;
	QHBoxLayout* OptsLayout;
	QHBoxLayout* ButtonsLayout;
	virtual void readPrefs();

};

#endif // SEARCHREPLACE_H
