#ifndef CHARSELECT_H
#define CHARSELECT_H

#include <qdialog.h>
//Added by qt3to4:
#include <QPixmap>
#include <QPaintEvent>
#include <QEvent>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <Q3ValueList>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <Q3Table>
#include "scribusapi.h"

class QTimer;
class QLabel;
class QPushbutton;
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
	typedef Q3ValueList<uint> charClassDef;
	Q3ValueList<charClassDef> allClasses;
	Q3ValueList<uint> characters;
	Q3ValueList<uint> charactersFull;
	Q3ValueList<uint> charactersLatin1;
	Q3ValueList<uint> charactersLatin1Supplement;
	Q3ValueList<uint> charactersLatinExtendedA;
	Q3ValueList<uint> charactersLatinExtendedB;
	Q3ValueList<uint> charactersGeneralPunctuation;
	Q3ValueList<uint> charactersSuperSubscripts;
	Q3ValueList<uint> charactersCurrencySymbols;
	Q3ValueList<uint> charactersLetterlikeSymbols;
	Q3ValueList<uint> charactersNumberForms;
	Q3ValueList<uint> charactersArrows;
	Q3ValueList<uint> charactersMathematicalOperators;
	Q3ValueList<uint> charactersBoxDrawing;
	Q3ValueList<uint> charactersBlockElements;
	Q3ValueList<uint> charactersGeometricShapes;
	Q3ValueList<uint> charactersMiscellaneousSymbols;
	Q3ValueList<uint> charactersDingbats;
	Q3ValueList<uint> charactersSmallFormVariants;
	Q3ValueList<uint> charactersAlphabeticPresentationForms;
	Q3ValueList<uint> charactersSpecial;
	Q3ValueList<uint> charactersGreek;
	Q3ValueList<uint> charactersGreekExtended;
	Q3ValueList<uint> charactersCyrillic;
	Q3ValueList<uint> charactersCyrillicSupplement;
	Q3ValueList<uint> charactersArabic;
	Q3ValueList<uint> charactersArabicPresentationFormsA;
	Q3ValueList<uint> charactersArabicPresentationFormsB;
	Q3ValueList<uint> charactersHebrew;
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

class SCRIBUS_API ChTable : public Q3Table
{
    Q_OBJECT

public:
	ChTable(CharSelect* parent, ScribusApp *pl);
	~ChTable() {};
	void resizeData( int ) {};
	QRect cellGeometry ( int row, int col ) const;
	void paintCell( QPainter * p, int row, int col, const QRect & cr, bool selected, const QColorGroup & cg );
	void paintFocus( QPainter * p, const QRect & cr )
	{
		Q_UNUSED( p ); Q_UNUSED( cr );
	}

	bool mPressed;
	bool alternate;
	Zoom* dia;
	ScribusApp *ap;
	CharSelect* par;
	QTimer* watchTimer;
	int maxCount;
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
