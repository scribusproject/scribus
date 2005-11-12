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

#include <qvariant.h>
#include <qmainwindow.h>
#include <qptrlist.h>
#include <qtable.h>
#include <qtextedit.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qstatusbar.h>
#include <qlabel.h>
#include <qbuttongroup.h>
#include <qspinbox.h>
#include <qtoolbutton.h>
#include <qtoolbar.h>
#include <qlayout.h>
#include <qsplitter.h>
#include <qcheckbox.h>

#include "scribusapi.h"
#include "pageitem.h"

class MSpinBox;
class Spalette;
class StyleSelect;
class AlignSelect;
class FontCombo;
class ShadeButton;
class PrefsManager;
class PrefsContext;


struct PtiSmall {
		    int csize;
		    short cshade;
		    short cshade2;
		    short cstyle;
		    short cab;
			short cscale;
			short cscalev;
			short cbase;
			short cshadowx;
			short cshadowy;
			short coutline;
			short cunderpos;
			short cunderwidth;
			short cstrikepos;
			short cstrikewidth;
		    int cextra;
			QString ch;
		    QString cfont;
		    QString ccolor;
			QString cstroke;
			PageItem* cembedded;
		   };

class SCRIBUS_API SEditor : public QTextEdit
{
	Q_OBJECT

public:
	SEditor (QWidget* parent, ScribusDoc *docc);
	~SEditor() {};
	void setCurrentDocument(ScribusDoc *docc);
	void setAlign(int style);
	void saveItemText(PageItem *currItem);
	void loadItemText(PageItem *currItem);
	void loadText(QString tx, PageItem *currItem);
	void updateAll();
	void updateFromChars(int p);
	void updateSel(int code, struct PtiSmall *hg);
	void deleteSel();
	void setStyle(int Csty);
	void setFarbe(bool marker);
	void insChars(QString t);
	void insStyledText();
	void copyStyledText();

	typedef QPtrList<PtiSmall> ChList;
	QPtrList<ChList> StyledText;
	QPtrList<PageItem> FrameItems;
	QValueList<int> ParagStyles;
	ScribusDoc* doc;
	bool unicodeTextEditMode;
	bool wasMod;
	bool ready;
	int unicodeInputCount;
	int CurrentStyle;
	int currentParaStyle;
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
	QString CurrFont;
	QString unicodeInputString;
	QString tBuffer;
	ChList cBuffer;
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
	QPopupMenu* createPopupMenu(const QPoint & pos);

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
	QPopupMenu *pmen;
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

class SCRIBUS_API SToolBColorF : public QToolBar
{
	Q_OBJECT

public:
	SToolBColorF(QMainWindow* parent, ScribusDoc *doc);
	~SToolBColorF() {};
	void setCurrentDocument(ScribusDoc *doc);
	QLabel* FillIcon;
	QComboBox* TxFill;
	ShadeButton *PM2;

public slots:
	void SetColor(int c);
	void SetShade(int s);
	void newShadeHandler();
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
	QLabel* StrokeIcon;
	QComboBox* TxStroke;
	ShadeButton *PM1;

public slots:
	void SetColor(int c);
	void SetShade(int s);
	void newShadeHandler();
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
	StyleSelect* SeStyle;
	QLabel* trackingLabel;
	MSpinBox* Extra;

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

class SCRIBUS_API SToolBAlign : public QToolBar
{
	Q_OBJECT

public:
	SToolBAlign(QMainWindow* parent);
	~SToolBAlign() {};
	AlignSelect* GroupAlign;
	Spalette *Spal;

public slots:
	void newStyleHandler(int s);
	void SetAlign(int s);
	void languageChange();

signals:
	void NewAlign(int);
	void newStyle(int);
};

class SCRIBUS_API SToolBFont : public QToolBar
{
	Q_OBJECT

public:
	SToolBFont(QMainWindow* parent);
	~SToolBFont() {};
	FontCombo* Fonts;
	MSpinBox* Size;
	MSpinBox* ChScale;
	MSpinBox* ChScaleV;
	QLabel* ScaleTxt;
	QLabel* ScaleTxtV;

public slots:
	void SetFont(QString f);
	void SetSize(double s);
	void SetScale(int s);
	void SetScaleV(int s);
	void newSizeHandler();
	void languageChange();

signals:
	void NewFont(const QString &);
	void NewSize(double);
	void NewScale(int);
	void NewScaleV(int);
};

class SCRIBUS_API StoryEditor : public QMainWindow
{
	Q_OBJECT

public:
	StoryEditor( QWidget* parent );
	StoryEditor( QWidget* parent, ScribusDoc *docc, PageItem* ite );
	~StoryEditor();
	void closeEvent(QCloseEvent *);
	void keyPressEvent (QKeyEvent * e);
	bool eventFilter( QObject* ob, QEvent* ev );
	//int exec();
	void changeAlign(int align);
	int result;
	void setCurrentDocumentAndItem(ScribusDoc *doc=NULL, PageItem *item=NULL);
	bool textDataChanged() const;
	PageItem* currentItem() const;
	ScribusDoc* currentDocument() const;

	QPopupMenu* fmenu;
	QPopupMenu* emenu;
	QPopupMenu* settingsMenu;
	QToolBar* FileTools;
	QToolButton* DatNeu;
	QToolButton* DatOpe;
	QToolButton* DatSav;
	QToolButton* DatFin;
	QToolButton* DatClo;
	QToolButton* DatCan;
	QToolButton* DatRel;
	QToolButton* DatUpdt;
	SToolBFont* FontTools;
	SToolBAlign* AlignTools;
	SToolBColorF* FillTools;
	SToolBColorS* StrokeTools;
	SToolBStyle* StyleTools;
	QSplitter* EdSplit;
	SideBar* EditorBar;
	SEditor* Editor;
	QButtonGroup* ButtonGroup1;
	QButtonGroup* ButtonGroup2;
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
	/*! Enables/disables the "smart" selection (#1203) - 10/16/2004 pv */
	bool smartSelection;
	int smartSel;

	ScribusDoc* currDoc;
	PageItem* currItem;
	bool textChanged;
	int Mcopy;
	int Mcut;
	int Mdel;
	int Mpaste;
	int Mupdt;
	int M_FileRevert;
	bool firstSet;
	bool activFromApp;
	bool blockUpdate;
	int CurrPara;
	int CurrChar;

public slots:
	void setBackPref();
	void setFontPref();
	void newTxFill(int c, int s);
	void newTxStroke(int c, int s);
	void newTxFont(const QString &f);
	void newTxSize(double s);
	void newTxScale(int s);
	void newTxScaleV(int s);
	void newTxStyle(int s);
	void newTxKern(int s);
	void newShadowOffs(int x, int y);
	void newTxtOutline(int o);
	void newTxtUnderline(int p, int w);
	void newTxtStrike(int p, int w);
	void updateProps(int p, int ch);
	void newAlign(int st);
	void changeAlignSB(int pa, int align);
	void updateStatus();
	void Do_leave();
	void Do_leave2();
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
	// 10/12/2004 - pv - #1203: wrong selection on double click
	void doubleClick(int para, int pos);
	void ToggleSmart();
	void languageChange();

signals:
	void DocChanged();
	void EditSt();

protected:
	void buildGUI();
	void connectSignals();
	void disconnectSignals();
	/*! \brief Loading the preferences (position).
	It's in separate method due the 2 constructors. */
	void loadPrefs();
	/*! \brief Saving the preferences (position). */
	void savePrefs();

    QHBoxLayout* StoryEd2Layout;
	QGridLayout* ButtonGroup1Layout;
	QGridLayout* ButtonGroup2Layout;

	PrefsManager* prefsManager;
	PrefsContext* prefs;
};

#endif

