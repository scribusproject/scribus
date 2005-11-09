#ifndef CHARSELECT_H
#define CHARSELECT_H

#include <qdialog.h>
#include "scribusapi.h"

class QTimer;
class QLabel;
class QPushbutton;
class QTable;
class QLayout;
class QToolTip;
class QStringList;
class QComboBox;
class QFont;
class QWidget;

class FontCombo;
class PageItem;
class ScribusApp;

class SCRIBUS_API Zoom : public QDialog
{
Q_OBJECT

public:
	Zoom( QWidget* parent, QPixmap pix, uint val);
	~Zoom() {};
	void paintEvent(QPaintEvent *);
	QPixmap pixm;
	QString valu;
};

class ChTable;

class SCRIBUS_API CharSelect : public QDialog
{
	Q_OBJECT

public:
	CharSelect(QWidget* parent, PageItem *item);
	CharSelect(QWidget* parent, PageItem *item, QString font);
	~CharSelect() {};

	const QString & getCharacters();

	bool needReturn;
	QString m_characters;
	void scanFont();
	void setupRangeCombo();
	void generatePreview(int charClass);
	ChTable* zTabelle;
	QLabel* sample;
	QLabel* fontLabel;
	QLabel* rangeLabel;
	FontCombo* fontSelector;
	QComboBox* rangeSelector;
	QPushButton* insertButton;
	QPushButton* deleteButton;
	QPushButton* closeButton;
	PageItem *ite;
	ScribusApp *ap;
	typedef QValueList<uint> charClassDef;
	QValueList<charClassDef> allClasses;
	QValueList<uint> characters;
	QValueList<uint> charactersFull;
	QValueList<uint> charactersLatin1;
	QValueList<uint> charactersLatin1Supplement;
	QValueList<uint> charactersLatinExtendedA;
	QValueList<uint> charactersLatinExtendedB;
	QValueList<uint> charactersGeneralPunctuation;
	QValueList<uint> charactersSuperSubscripts;
	QValueList<uint> charactersCurrencySymbols;
	QValueList<uint> charactersLetterlikeSymbols;
	QValueList<uint> charactersNumberForms;
	QValueList<uint> charactersArrows;
	QValueList<uint> charactersMathematicalOperators;
	QValueList<uint> charactersBoxDrawing;
	QValueList<uint> charactersBlockElements;
	QValueList<uint> charactersGeometricShapes;
	QValueList<uint> charactersMiscellaneousSymbols;
	QValueList<uint> charactersDingbats;
	QValueList<uint> charactersSmallFormVariants;
	QValueList<uint> charactersAlphabeticPresentationForms;
	QValueList<uint> charactersSpecial;
	QValueList<uint> charactersGreek;
	QValueList<uint> charactersGreekExtended;
	QValueList<uint> charactersCyrillic;
	QValueList<uint> charactersCyrillicSupplement;
	QValueList<uint> charactersArabic;
	QValueList<uint> charactersArabicPresentationFormsA;
	QValueList<uint> charactersArabicPresentationFormsB;
	QValueList<uint> charactersHebrew;
	QMap<int,int> usedCharClasses;
	QString chToIns;
	QString fontInUse;
	uint maxCount;
	int characterClass;

public slots:
	void newChar(uint r, uint c);
	void delChar();
	void newFont(int font);
	void newCharClass(int c);
	void delEdit();
	void insChar();

protected:
	void run(QWidget* parent, PageItem* item, ScribusApp* pl);
	bool eventFilter( QObject *obj, QEvent *ev );
	void recalcCellSizes();
	QVBoxLayout* zAuswahlLayout;
	QHBoxLayout* selectionsLayout;
	QHBoxLayout* layout1;
};

class SCRIBUS_API ChTable : public QTable
{
    Q_OBJECT

public:
	ChTable(CharSelect* parent, ScribusApp *pl);
	~ChTable() {};
	void resizeData( int ) {};
	QRect cellGeometry ( int row, int col ) const;
	void paintCell( QPainter * p, int row, int col, const QRect & cr, bool selected, const QColorGroup & cg );
	void paintFocus( QPainter * /*p*/, const QRect & /*cr*/ ) {};

	bool mPressed;
	bool alternate;
	Zoom* dia;
	ScribusApp *ap;
	CharSelect* par;
	QTimer* watchTimer;
	uint maxCount;
	uint rowA;
	uint colA;

public slots:
	void showAlternate();

signals:
	void selectChar(uint, uint);
	void delChar();

protected:
	virtual void keyPressEvent(QKeyEvent *k);
	virtual void contentsMouseReleaseEvent(QMouseEvent *m);
	virtual void contentsMousePressEvent(QMouseEvent* e);
};

#endif // QUERY_H
