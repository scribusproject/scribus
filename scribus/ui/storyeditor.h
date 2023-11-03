/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          storyeditor.h    -  description
                             -------------------
    begin                : Tue Nov 11 2003
    copyright            : (C) 2003 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef STORYEDITOR_H
#define STORYEDITOR_H

#include <tuple>

#include <QTextEdit>
#include <QAction>
#include <QCheckBox>
#include <QCloseEvent>
#include <QDialog>
#include <QEvent>
#include <QFocusEvent>
#include <QHideEvent>
#include <QKeyEvent>
#include <QLabel>
#include <QLayout>
#include <QList>
#include <QMainWindow>
#include <QMap>
#include <QMenu>
#include <QMenuBar>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPair>
#include <QPixmap>
#include <QPointer>
#include <QShowEvent>
#include <QSpinBox>
#include <QSplitter>
#include <QStack>
#include <QStatusBar>
#include <QStringList>
#include <QToolBar>
#include <QToolButton>
#include <QVariant>
#include <QWidgetAction>

class QEvent;
class QGridLayout;
class QHBoxLayout;
class QFrame;

#include "scribusapi.h"
#include "text/storytext.h"

class PageItem;
class MenuManager;
class ScrSpinBox;
class ParaStyleComboBox;
class StyleSelect;
class AlignSelect;
class DirectionSelect;
class FontCombo;
class ScrAction;
class ShadeButton;
class PrefsManager;
class PrefsContext;
class StoryEditor;
class ColorCombo;
class CharSelect;

class SCRIBUS_API SEditor : public QTextEdit
{
	Q_OBJECT

	friend class StoryEditor;
	friend class SideBar;

public:
	SEditor (QWidget* parent, ScribusDoc *docc, StoryEditor* parentSE);
	~SEditor() {}

	void setCurrentDocument(ScribusDoc *docc);
	void setAlign(int align);
	void setDirection(int align);
	void saveItemText(PageItem *currItem);
	void loadItemText(PageItem *currItem);
	void loadText(const QString& tx, PageItem *currItem);
	void updateAll();
	void updateFromChars(int p);
	void updateSel(const CharStyle& style);
	void updateSel(const ParagraphStyle& style);
	void deleteSel();
	void setEffects(int effects);
	void setColor(bool marker);

	void insertChars(const QString& text);
	void insertChars(const QString& styledText, const QString& editText);

	StoryText StyledText;

protected:
	void insertCharsInternal(const QString& t);
	void insertCharsInternal(const QString& t, int position);

	void insertStyledText(const StoryText& styledText);
	void insertStyledText(const StoryText& styledText, int position);

	void insertUpdate(int position, int len);

	void setAlign(QTextCursor& tCursor, int style);
	void setDirection(QTextCursor& tCursor, int style);
	void setEffects(QTextCursor& tCursor, int effects);

	void keyPressEvent(QKeyEvent *k) override;
	void inputMethodEvent(QInputMethodEvent *event) override;
	void focusOutEvent(QFocusEvent *e) override;
	void focusInEvent(QFocusEvent *e) override;
	void scrollContentsBy(int dx, int dy) override;

	bool canInsertFromMimeData(const QMimeData* source) const override;
	QMimeData* createMimeDataFromSelection() const override;
	void insertFromMimeData(const QMimeData* source) override;

	QList<PageItem*> FrameItems;
	ScribusDoc* doc { nullptr };
	StoryEditor* parentStoryEditor { nullptr };
	int  blockContentsChangeHook { 0 };
	bool unicodeTextEditMode { false };
	bool wasMod { false };
	bool ready { false };
	int unicodeInputCount { 0 };
	StyleFlag CurrentEffects;
	QString currentParaStyle;
	int CurrAlign { 0 };
	int CurrDirection { 0 };
	double CurrFontSize { 0.0 };
	double CurrTextFillSh { 0.0 };
	double CurrTextStrokeSh { 0.0 };
	double CurrTextScaleH { 0.0 };
	double CurrTextScaleV { 0.0 };
	double CurrTextBase { 0.0 };
	double CurrTextShadowX { 0.0 };
	double CurrTextShadowY { 0.0 };
	double CurrTextOutline { 0.0 };
	double CurrTextUnderPos { 0.0 };
	double CurrTextUnderWidth { 0.0 };
	double CurrTextStrikePos { 0.0 };
	double CurrTextStrikeWidth { 0.0 };
	double CurrTextKern { 0.0 };
	QString CurrTextStroke;
	QString CurrTextFill;
	QString prevFont;
	QString CurrFont;
	QString unicodeInputString;

	QStack< std::tuple<int, int, int> > SelStack;

	int SelCharStart { 0 };
	int SelCharEnd { 0 };
	int SuspendContentsChange { 0 };	// input method

protected slots:
	void handleContentsChange(int position, int charsRemoved, int charsAdded); 

public slots:
	void cut();
	void copy();
	void paste();
	void ClipChange();
	void SelClipChange();

signals:
	void setProps(int, int);
	void SideBarUp(bool);
	void SideBarUpdate();
	void PasteAvail();
	void contentsMoving(int, int);
};

class SCRIBUS_API SideBar : public QLabel
{
	Q_OBJECT

public:
	SideBar(QWidget *pa);
	~SideBar() {};

	void setEditor(SEditor* editor);

	int offs { 0 };
	int currentPar { 0 };
	QMenu *pmen { nullptr };
	QWidgetAction* paraStyleAct { nullptr };
	bool noUpdt { true };
	bool inRep { false };

protected:
	void paintEvent(QPaintEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *m) override;

private:
	SEditor *m_editor { nullptr };

signals:
	void ChangeStyle(int, const QString&);
	//! signal raised when user ask for style manager
	//void sigEditStyles();

public slots:
	void setPStyle(const QString&);
	void doMove(int x, int y);
	void doRepaint();
	void setRepaint(bool r);
	//! slot raising style manager
	//void editStyles();
};

class SCRIBUS_API SToolBColorF : public QToolBar
{
	Q_OBJECT

public:
	SToolBColorF(QMainWindow* parent, ScribusDoc *doc);
	~SToolBColorF() {};
	
	void setCurrentDocument(ScribusDoc *doc);

	QLabel* FillIcon { nullptr };
	ColorCombo* TxFill { nullptr };
	ShadeButton *PM2 { nullptr };
	QAction* pm2Action { nullptr };
	QAction* txFillAction { nullptr };
	QAction* fillIconAction { nullptr };

protected:
	void changeEvent(QEvent *e) override;

public slots:
	void SetColor(int c);
	void SetShade(double s);
	void newShadeHandler();
	void iconSetChange();
	void languageChange();

signals:
	void NewColor(int, int);
};

class SCRIBUS_API SToolBColorS : public QToolBar
{
	Q_OBJECT

public:
	SToolBColorS(QMainWindow* parent, ScribusDoc *doc);
	~SToolBColorS() {};
	
	void setCurrentDocument(ScribusDoc *doc);

	QLabel* StrokeIcon { nullptr };
	ColorCombo* TxStroke { nullptr };
	ShadeButton *PM1 { nullptr };
	QAction* strokeIconAction { nullptr };
	QAction* txStrokeAction { nullptr };
	QAction* pm1Action { nullptr };

protected:
	void changeEvent(QEvent *e) override;

public slots:
	void SetColor(int c);
	void SetShade(double s);
	void newShadeHandler();
	void iconSetChange();
	void languageChange();

signals:
	void NewColor(int, int);
};

class SCRIBUS_API SToolBStyle : public QToolBar
{
	Q_OBJECT

public:
	SToolBStyle(QMainWindow* parent);
	~SToolBStyle() {};
	
	StyleSelect* SeStyle { nullptr };
	QLabel* trackingLabel { nullptr };
	ScrSpinBox* Extra { nullptr };
	QAction* seStyleAction { nullptr };
	QAction* trackingLabelAction { nullptr };
	QAction* extraAction { nullptr };

protected:
	void changeEvent(QEvent *e) override;

public slots:
	void newStrikeHandler();
	void newUnderlineHandler();
	void newOutlineHandler();
	void newShadowHandler();
	void newKernHandler();
	void SetShadow(double x, double y);
	void setStrike(double p, double w);
	void setUnderline(double p, double w);
	void setOutline(double o);
	void SetStyle(int s);
	void SetKern(double k);
	void iconSetChange();
	void languageChange();

signals:
	void NewKern(double);
	void NewShadow(double, double);
	void newOutline(double);
	void newStrike(double, double);
	void newUnderline(double, double);
	void newStyle(int);
};

class SCRIBUS_API SToolBAlign : public QToolBar
{
	Q_OBJECT

public:
	SToolBAlign(QMainWindow* parent);
	~SToolBAlign() {};
	
	AlignSelect* GroupAlign { nullptr };
	DirectionSelect* GroupDirection { nullptr };
	ParaStyleComboBox *paraStyleCombo { nullptr };
	QAction* groupAlignAction { nullptr };
	QAction* groupDirectionAction { nullptr };
	QAction* paraStyleComboAction { nullptr };

protected:
	void changeEvent(QEvent *e) override;

public slots:
	void SetAlign(int s);
	void SetDirection(int s);
	void SetParaStyle(const QString& s);
	void languageChange();

signals:
	void newAlign(int);
	void newDirection(int);
	void newParaStyle(const QString&);
};

class SCRIBUS_API SToolBFont : public QToolBar
{
	Q_OBJECT

public:
	SToolBFont(QMainWindow* parent);
	~SToolBFont() {};
	
	FontCombo* Fonts { nullptr };
	ScrSpinBox* charScaleH { nullptr };
	ScrSpinBox* charScaleV { nullptr };
	QAction* fontsAction { nullptr };
	QAction* chScaleHAction { nullptr };
	QAction* chScaleVAction { nullptr };

protected:
	void changeEvent(QEvent *e) override;

public slots:
	void SetFont(const QString& f);
	void SetSize(double s);
	void SetScaleH(double s);
	void SetScaleV(double s);

signals:
	void newFont(const QString &);
	void newSize(double);
	void newScaleH(double);
	void newScaleV(double);

private:
	ScrSpinBox* Size { nullptr };
	QLabel*  lblScaleTxtH { nullptr };
	QLabel*  lblScaleTxtV { nullptr };
	QAction* sizeAction { nullptr };
	QAction* scaleTxtHAction { nullptr };
	QAction* scaleTxtVAction { nullptr };

private slots:
	void iconSetChange();
	void languageChange();
};

class SCRIBUS_API StoryEditor : public QMainWindow
{
	Q_OBJECT
	friend class SEditor;

public:
	StoryEditor( QWidget* parent );
// 	StoryEditor( QWidget* parent, ScribusDoc *docc, PageItem* ite );
	~StoryEditor();

	void setCurrentDocumentAndItem(ScribusDoc* doc = nullptr, PageItem* item = nullptr);
	void setSpellActive(bool ssa);

	ScribusDoc* currentDocument() const;
	PageItem* currentItem() const;

	SEditor* Editor { nullptr };
	bool activFromApp { true };
	MenuManager* seMenuMgr { nullptr };
	QMap<QString, QPointer<ScrAction> > seActions;

public slots:
	void newStyle(const QString&);
	void newAlign(int st);
	void newDirection(int dir);
	void newTxFill(int c, int s);
	void newTxStroke(int c, int s);
	void newTxFont(const QString &f);
	void newTxSize(double s);
	void newTxStyle(int s);

signals:
	void DocChanged();

private:
	//int exec();
	void changeAlign(int align);
	void changeDirection(int align);
	bool textDataChanged() const;

	/*! \brief Enables/disables the "smart" selection (#1203) - 10/16/2004 pv */
	bool m_smartSelection { false };

	ScribusDoc* m_doc { nullptr };
	PageItem* m_item { nullptr };

	bool m_textChanged { false };
	bool m_firstSet { false };
	bool m_blockUpdate { false };
	
	int m_result { QDialog::Rejected };

protected slots:
	void setFontPref();
	void newTxScale();
	void newTxScaleV();
	void newTxKern(double s);
	void newShadowOffs(double x, double y);
	void newTxtOutline(double o);
	void newTxtUnderline(double p, double w);
	void newTxtStrike(double p, double w);
	void updateProps();
	void updateProps(QTextCursor &cur);
	void updateProps(int p, int ch);

	void changeStyleSB(int pa, const QString&);
	void changeStyle();
	void updateStatus();
	void Do_leave();
	void Do_leave2();
	//! \brief Saves the document with editation continued. Signal called from menu.
	void Do_saveDocument();
	bool Do_new();
	void slotFileRevert();

	void Do_selectAll();
	void Do_copy();
	void Do_paste();
	void Do_cut();
	void Do_del();
	void Do_insSp();
	/*! \brief Insert a Lorem Ipsum (from dialog) into
	current position in the SE.
	*/
	void insertSampleText();
	void Do_fontPrev();
	void CopyAvail(bool u);
	void PasteAvail();
	void updateTextFrame();
	void SearchText();
	//void slotEditStyles();
	void modifiedText();
	void LoadTextFile();
	void SaveTextFile();
	void setSmart(bool);

	void iconSetChange();
	void languageChange();

	void specialActionKeyEvent(int unicodevalue);
	/*! \brief Slot to insert special characters from charSelect widget. */
	void slot_insertSpecialChar();
	void slot_insertUserSpecialChar(QString, const QString&);
	// 10/12/2004 - pv - #1203: wrong selection on double click
	void doubleClick(int para, int pos);

protected:
	void initActions();
	void updateUnicodeActions();
	void buildMenus();
	void buildGUI();
	void setupEditorGUI();
	void connectSignals();
	void disconnectSignals();
	/*! \brief Loading the preferences (position).
	It's in separate method due the 2 constructors. */
	void loadPrefs();
	/*! \brief Saving the preferences (position). */
	void savePrefs();

	/*! \brief Special Characters dialog.
	It uses a little bit ugly method to operate with.
	It's a duplication of the main window's charPalette due
	the lack of setParent() in qt3 and a ugly behaviour of
	the reparent().
	charSelect is created as a copy of the charPalette.
	\author Petr Vanek <petr@scribus.info>
	*/
	CharSelect *charSelect { nullptr };
    //! True when there were some glyphs inserted via charSelect
	bool charSelectUsed { false };

	void changeEvent(QEvent *e) override;

	void showEvent(QShowEvent *) override;
	void hideEvent(QHideEvent *) override;

	void closeEvent(QCloseEvent *) override;
	void keyPressEvent(QKeyEvent *e) override;
	bool eventFilter(QObject *ob, QEvent* ev) override;

	QHBoxLayout* StoryEd2Layout { nullptr };
	QGridLayout* ButtonGroup1Layout { nullptr };
	QGridLayout* ButtonGroup2Layout { nullptr };

	PrefsManager& prefsManager;
	PrefsContext* prefs { nullptr };

	QStringList unicodeCharActionNames;
	QPixmap noIcon;

	QToolBar* FileTools { nullptr };
	SToolBFont* FontTools { nullptr };
	SToolBAlign* AlignTools { nullptr };
	SToolBColorF* FillTools { nullptr };
	SToolBColorS* StrokeTools { nullptr };
	SToolBStyle* StyleTools { nullptr };
	QSplitter* EdSplit { nullptr };
	SideBar* EditorBar { nullptr };
	QFrame* ButtonGroup1 { nullptr };
	QFrame* ButtonGroup2 { nullptr };
	QLabel* WordCT1 { nullptr };
	QLabel* WordCT3 { nullptr };
	QLabel* ParCT { nullptr };
	QLabel* ParC { nullptr };
	QLabel* WordCT { nullptr };
	QLabel* WordC { nullptr };
	QLabel* CharCT { nullptr };
	QLabel* CharC { nullptr };
	QLabel* WordCT2 { nullptr };
	QLabel* WordC2 { nullptr };
	QLabel* CharCT2 { nullptr };
	QLabel* CharC2 { nullptr };
	
	bool m_spellActive { false };
};

#endif


