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
#include "pageitem.h"
#include "mspinbox.h"
#include "spalette.h"
#include "fontcombo.h"
#include "styleselect.h"
#include "alignselect.h"
#include "shadebutton.h"

struct PtiSmall { 
		    int csize;
		    int cshade;
		    int cshade2;
		    int cstyle;
		    int cab;
			int cscale;
		    double cextra;
			QString ch;
		    QString cfont;
		    QString ccolor;
			QString cstroke;
		   };

class SEditor : public QTextEdit
{
	Q_OBJECT

public:
	SEditor (QWidget* parent, ScribusDoc *docc);
	~SEditor() {};
	void setAlign(int style);
	void saveItemText(PageItem* b);
	void loadItemText(PageItem* b);
	void loadText(QString tx, PageItem* b);
	void updateAll();
	void updateFromChars(int p);
	void updateSel(int code, struct PtiSmall *hg);
	void deleteSel();
	void setStyle(int Csty);
	void setFarbe(QString farbe, int shad);
	void insChars(QString t);
	void insStyledText();
	void copyStyledText();

	typedef QPtrList<PtiSmall> ChList;
	QPtrList<ChList> StyledText;
	QValueList<int> ParagStyles;
	ScribusDoc* doc;
	bool UniCinp;
	bool wasMod;
	bool ready;
	int UniCinC;
	int CurrentStyle;
	int currentParaStyle;
	int CurrFontSize;
	int CurrTextFillSh;
	int CurrTextStrokeSh;
	int CurrTextScale;
	double CurrTextKern;
	QString CurrTextStroke;
	QString CurrTextFill;
	QString CurrFont;
	QString UniCinS;
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

class SideBar : public QLabel  
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

public slots:
	void setPStyle(int s);
	void doMove(int x, int y);
	void doRepaint();
	void setRepaint(bool r);
};

class SToolBColorF : public QToolBar  
{
	Q_OBJECT

public: 
	SToolBColorF(QMainWindow* parent, ScribusDoc *doc);
	~SToolBColorF() {};
	QLabel* FillIcon;
	QComboBox* TxFill;
	ShadeButton *PM2;

public slots:
	void SetColor(int c);
	void SetShade(int s);
	void newShadeHandler();

signals:
	void NewColor(int, int);
};

class SToolBColorS : public QToolBar  
{
	Q_OBJECT

public: 
	SToolBColorS(QMainWindow* parent, ScribusDoc *doc);
	~SToolBColorS() {};
	QLabel* StrokeIcon;
	QComboBox* TxStroke;
	ShadeButton *PM1;

public slots:
	void SetColor(int c);
	void SetShade(int s);
	void newShadeHandler();

signals:
	void NewColor(int, int);
};

class SToolBStyle : public QToolBar  
{
	Q_OBJECT

public: 
	SToolBStyle(QMainWindow* parent);
	~SToolBStyle() {};
	StyleSelect* SeStyle;
	QLabel* kerningLabel;
	MSpinBox* Extra;

public slots:
	void newKernHandler();
	void SetStyle(int s);
	void SetKern(double k);

signals:
	void NewKern(double);
	void newStyle(int);
};

class SToolBAlign : public QToolBar  
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

signals:
	void NewAlign(int);
	void newStyle(int);
};

class SToolBFont : public QToolBar  
{
	Q_OBJECT

public: 
	SToolBFont(QMainWindow* parent);
	~SToolBFont() {};
	FontCombo* Fonts;
	MSpinBox* Size;
	QSpinBox* ChScale;

public slots:
	void SetFont(QString f);
	void SetSize(double s);
	void SetScale(int s);
	void newSizeHandler();

signals:
	void NewFont(const QString &);
	void NewSize(double);
	void NewScale(int);
};

class StoryEditor : public QMainWindow
{
	Q_OBJECT

public:
	StoryEditor( QWidget* parent, ScribusDoc *docc, PageItem* ite );
	~StoryEditor() {};
	void closeEvent(QCloseEvent *);
	int exec();
	void changeAlign(int align);
	int result;

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

	ScribusDoc* doc;
	PageItem* CurrItem;
	bool TextChanged;
	int Mcopy;
	int Mcut;
	int Mdel;
	int Mpaste;
	int Mupdt;
	int fid52;
	bool firstSet;

public slots:
	void setBackPref();
	void setFontPref();
	void newTxFill(int c, int s);
	void newTxStroke(int c, int s);
	void newTxFont(const QString &f);
	void newTxSize(double s);
	void newTxScale(int s);
	void newTxStyle(int s);
	void newTxKern(double s);
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

signals:
	void DocChanged();
	void EditSt();

protected:
    QHBoxLayout* StoryEd2Layout;
	QGridLayout* ButtonGroup1Layout;
	QGridLayout* ButtonGroup2Layout;

};

#endif

