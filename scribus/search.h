#ifndef SEARCHREPLACE_H
#define SEARCHREPLACE_H

#include <qvariant.h>
#include <qdialog.h>
#include "scribusdoc.h"
#include "pageitem.h"

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

class SearchReplace : public QDialog
{
    Q_OBJECT

public:
    SearchReplace( QWidget* parent, ScribusDoc *doc, preV *Prefs, PageItem* ite );
    ~SearchReplace() {};

    QGroupBox* Search;
    QCheckBox* SStroke;
    QCheckBox* SFill;
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
    QComboBox* SStrokeVal;
    QGroupBox* Replace;
    QCheckBox* RStroke;
    QCheckBox* RFill;
    QCheckBox* RSize;
    QCheckBox* RFont;
    QCheckBox* RStyle;
    QCheckBox* RText;
    QLineEdit* RTextVal;
    QComboBox* RStyleVal;
    FontCombo* RFontVal;
    MSpinBox* RSizeVal;
    QComboBox* RFillVal;
    QComboBox* RStrokeVal;
    StyleSelect* SEffVal;
    StyleSelect* REffVal;
    QCheckBox* Word;
    QCheckBox* CaseIgnore;
    QPushButton* DoSearch;
    QPushButton* DoReplace;
    QPushButton* AllReplace;
    QPushButton* Leave;
	PageItem* Item;
	ScribusDoc* Doc;
	uint ReplStart;
	bool NotFound;

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
    virtual void enableStrokeSearch();
    virtual void enableTxReplace();
    virtual void enableStyleReplace();
    virtual void enableFontReplace();
    virtual void enableSizeReplace();
	virtual void enableEffReplace();
    virtual void enableFillReplace();
    virtual void enableStrokeReplace();

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

};

#endif // SEARCHREPLACE_H
