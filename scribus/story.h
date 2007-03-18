/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          story.h    -  description
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

#ifndef STORY_H
#define STORY_H

#include <qpointer.h>
#include <qvariant.h>
#include <q3mainwindow.h>
#include <q3ptrlist.h>
#include <q3table.h>
#include <q3textedit.h>
#include <q3popupmenu.h>
#include <qmap.h>
#include <qmenubar.h>
#include <qstatusbar.h>
#include <qlabel.h>
#include <q3buttongroup.h>
#include <qspinbox.h>
#include <qstringlist.h>
#include <qtoolbutton.h>
#include <q3toolbar.h>
#include <qlayout.h>
#include <qsplitter.h>
#include <qcheckbox.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <QKeyEvent>
#include <Q3GridLayout>
#include <QPixmap>
#include <QHideEvent>
#include <QFocusEvent>
#include <QMouseEvent>
#include <QEvent>
#include <QShowEvent>
#include <QPaintEvent>
#include <QCloseEvent>

#include "scribusapi.h"
#include "pageitem.h"
#include "text/storytext.h"

class MenuManager;
class ScrSpinBox;
class ParaStyleComboBox;
class StyleSelect;
class AlignSelect;
class FontCombo;
class ScrAction;
class ShadeButton;
class PrefsManager;
class PrefsContext;
class StoryEditor;
class ColorCombo;
class CharSelect;


class SCRIBUS_API SEditor : public Q3TextEdit
{
	Q_OBJECT

public:
	SEditor (QWidget* parent, ScribusDoc *docc, StoryEditor* parentSE);
	~SEditor() {};
	void setCurrentDocument(ScribusDoc *docc);
	void setAlign(int style);
	void saveItemText(PageItem *currItem);
	void loadItemText(PageItem *currItem);
	void loadText(QString tx, PageItem *currItem);
	void updateAll();
	void updateFromChars(int p);
	void updateSel(const CharStyle& style);
	void updateSel(const ParagraphStyle& style);
	void deleteSel();
	void setStyle(int Csty);
	void setFarbe(bool marker);
	void insChars(QString t);
	void insStyledText();
	void copyStyledText();

	StoryText StyledText;
	Q3PtrList<PageItem> FrameItems;
	ScribusDoc* doc;
	bool unicodeTextEditMode;
	bool wasMod;
	bool ready;
	int unicodeInputCount;
	StyleFlag CurrentStyle;
	QString currentParaStyle;
	int CurrAlign;
	int CurrFontSize;
	int CurrTextFillSh;
	int CurrTextStrokeSh;
	int CurrTextScale;
	int CurrTextScaleV;
	int CurrTextBase;
	int CurrTextShadowX;
	int CurrTextShadowY;
	int CurrTextOutline;
	int CurrTextUnderPos;
	int CurrTextUnderWidth;
	int CurrTextStrikePos;
	int CurrTextStrikeWidth;
	int CurrTextKern;
	QString CurrTextStroke;
	QString CurrTextFill;
	QString prevFont;
	QString CurrFont;
	QString unicodeInputString;
	QString tBuffer;
	StoryText cBuffer;
	int ClipData;
	bool StoredSel;
	int SelParaStart;
	int SelParaEnd;
	int SelCharStart;
	int SelCharEnd;

protected:
	void keyPressEvent(QKeyEvent *k);
	void imEndEvent(QIMEvent *e);
	void focusOutEvent(QFocusEvent *e);
	void focusInEvent(QFocusEvent *e);
	Q3PopupMenu* createPopupMenu(const QPoint & pos);
	StoryEditor* parentStoryEditor;

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
};

class SCRIBUS_API SideBar : public QLabel
{
	Q_OBJECT

public:
	SideBar(QWidget *pa);
	~SideBar() {};
	int offs;
	int CurrentPar;
	SEditor *editor;
	Q3PopupMenu *pmen;
	bool noUpdt;
	bool inRep;

protected:
	void paintEvent(QPaintEvent *e);
	void mouseReleaseEvent(QMouseEvent *m);

signals:
	void ChangeStyle(int, int);
	//! signal raised when user ask for style manager
	void sigEditStyles();

public slots:
	void setPStyle(int s);
	void doMove(int x, int y);
	void doRepaint();
	void setRepaint(bool r);
	//! slot raising style manager
	void editStyles();
};

class SCRIBUS_API SToolBColorF : public Q3ToolBar
{
	Q_OBJECT

public:
	SToolBColorF(Q3MainWindow* parent, ScribusDoc *doc);
	~SToolBColorF() {};
	void setCurrentDocument(ScribusDoc *doc);
	QLabel* FillIcon;
	ColorCombo* TxFill;
	ShadeButton *PM2;

public slots:
	void SetColor(int c);
	void SetShade(int s);
	void newShadeHandler();
	void languageChange();

signals:
	void NewColor(int, int);
};

class SCRIBUS_API SToolBColorS : public Q3ToolBar
{
	Q_OBJECT

public:
	SToolBColorS(Q3MainWindow* parent, ScribusDoc *doc);
	~SToolBColorS() {};
	void setCurrentDocument(ScribusDoc *doc);
	QLabel* StrokeIcon;
	ColorCombo* TxStroke;
	ShadeButton *PM1;

public slots:
	void SetColor(int c);
	void SetShade(int s);
	void newShadeHandler();
	void languageChange();

signals:
	void NewColor(int, int);
};

class SCRIBUS_API SToolBStyle : public Q3ToolBar
{
	Q_OBJECT

public:
	SToolBStyle(Q3MainWindow* parent);
	~SToolBStyle() {};
	StyleSelect* SeStyle;
	QLabel* trackingLabel;
	ScrSpinBox* Extra;

public slots:
	void newStrikeHandler();
	void newUnderlineHandler();
	void newOutlineHandler();
	void newShadowHandler();
	void newKernHandler();
	void SetShadow(int x, int y);
	void setStrike(int p, int w);
	void setUnderline(int p, int w);
	void setOutline(int o);
	void SetStyle(int s);
	void SetKern(int k);
	void languageChange();

signals:
	void NewKern(int);
	void NewShadow(int, int);
	void newOutline(int);
	void newStrike(int, int);
	void newUnderline(int, int);
	void newStyle(int);
};

class SCRIBUS_API SToolBAlign : public Q3ToolBar
{
	Q_OBJECT

public:
	SToolBAlign(Q3MainWindow* parent);
	~SToolBAlign() {};
	AlignSelect* GroupAlign;
	ParaStyleComboBox *paraStyleCombo;

public slots:
	void SetAlign(int s);
	void SetParaStyle(int s);
	void languageChange();

signals:
	void newAlign(int);
	void newParaStyle(int);
};

class SCRIBUS_API SToolBFont : public Q3ToolBar
{
	Q_OBJECT

public:
	SToolBFont(Q3MainWindow* parent);
	~SToolBFont() {};
	FontCombo* Fonts;
	ScrSpinBox* ChScale;
	ScrSpinBox* ChScaleV;

public slots:
	void SetFont(QString f);
	void SetSize(double s);
	void SetScale(int s);
	void SetScaleV(int s);
	void newSizeHandler();

signals:
	void NewFont(const QString &);
	void NewSize(double);
	void NewScale(int);
	void NewScaleV(int);

private:
	ScrSpinBox* Size;
	QLabel* ScaleTxt;
	QLabel* ScaleTxtV;

private slots:
	void languageChange();
};

class SCRIBUS_API StoryEditor : public Q3MainWindow
{
	Q_OBJECT
	friend class SEditor;

public:
	StoryEditor( QWidget* parent );
// 	StoryEditor( QWidget* parent, ScribusDoc *docc, PageItem* ite );
	~StoryEditor();

	void setCurrentDocumentAndItem(ScribusDoc *doc=NULL, PageItem *item=NULL);

	ScribusDoc* currentDocument() const;
	PageItem* currentItem() const;
	SEditor* Editor;
	bool activFromApp;

public slots:
	void newAlign(int st);
	void newTxFill(int c, int s);
	void newTxStroke(int c, int s);
	void newTxFont(const QString &f);
	void newTxSize(double s);
	void newTxStyle(int s);

signals:
	void DocChanged();
	void EditSt();

private:
	//int exec();
	void changeAlign(int align);
	int result;
	bool textDataChanged() const;

	/*! \brief Enables/disables the "smart" selection (#1203) - 10/16/2004 pv */
	bool smartSelection;
	int smartSel;

	ScribusDoc* currDoc;
	PageItem* currItem;

	bool textChanged;
	bool firstSet;

	bool blockUpdate;
	int CurrPara;
	int CurrChar;

protected slots:
	void setBackPref();
	void setFontPref();
	void newTxScale(int s);
	void newTxScaleV(int s);
	void newTxKern(int s);
	void newShadowOffs(int x, int y);
	void newTxtOutline(int o);
	void newTxtUnderline(int p, int w);
	void newTxtStrike(int p, int w);
	void updateProps(int p, int ch);

	void newStyle(int st);
	void changeStyleSB(int pa, int st);
	void changeStyle(int st);
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
	void Do_fontPrev();
	void CopyAvail(bool u);
	void PasteAvail();
	void updateTextFrame();
	void SearchText();
	void slotEditStyles();
	void modifiedText();
	void LoadTextFile();
	void SaveTextFile();
	void setSmart(bool);
	void languageChange();

	void specialActionKeyEvent(const QString& actionName, int unicodevalue);
	/*! \brief Slot to insert special characters from charSelect widget. */
	void slot_insertSpecialChar();
	void slot_insertUserSpecialChar(QChar);
	// 10/12/2004 - pv - #1203: wrong selection on double click
	void doubleClick(int para, int pos);

protected:
	void initActions();
	void updateUnicodeActions();
	void buildMenus();
	void buildGUI();
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
	CharSelect *charSelect;
	bool charSelectUsed;
	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

	void closeEvent(QCloseEvent *);
	void keyPressEvent (QKeyEvent * e);
	bool eventFilter( QObject* ob, QEvent* ev );

    Q3HBoxLayout* StoryEd2Layout;
	Q3GridLayout* ButtonGroup1Layout;
	Q3GridLayout* ButtonGroup2Layout;

	PrefsManager* prefsManager;
	PrefsContext* prefs;

	QMap<QString, QPointer<ScrAction> > seActions;
	QStringList unicodeCharActionNames;
	MenuManager* seMenuMgr;
	QPixmap noIcon;

	Q3ToolBar* FileTools;
	SToolBFont* FontTools;
	SToolBAlign* AlignTools;
	SToolBColorF* FillTools;
	SToolBColorS* StrokeTools;
	SToolBStyle* StyleTools;
	QSplitter* EdSplit;
	SideBar* EditorBar;
	Q3ButtonGroup* ButtonGroup1;
	Q3ButtonGroup* ButtonGroup2;
	QLabel* WordCT1;
	QLabel* WordCT3;
	QLabel* ParCT;
	QLabel* ParC;
	QLabel* WordCT;
	QLabel* WordC;
	QLabel* CharCT;
	QLabel* CharC;
	QLabel* WordCT2;
	QLabel* WordC2;
	QLabel* CharCT2;
	QLabel* CharC2;
};

#endif

