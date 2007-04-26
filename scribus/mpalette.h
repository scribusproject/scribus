/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef MPALETTE_H
#define MPALETTE_H

#include <q3buttongroup.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <q3frame.h>
#include <q3groupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qspinbox.h>
#include <qwidget.h>
#include <q3widgetstack.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qimage.h>
#include <qpixmap.h>
#include <q3listbox.h>
#include <qlineedit.h>
#include <qtimer.h>
#include <qtoolbox.h>
#include <q3popupmenu.h>
//Added by qt3to4:
#include <QFocusEvent>
#include <QCloseEvent>
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <QEvent>
#include <Q3VBoxLayout>
#include <QToolButton>

#include "scribusapi.h"
#include "scrpalettebase.h"
#include "scrspinbox.h"
#include "pageitem.h"
#include "page.h"
class ScribusDoc;
#include "linkbutton.h"
#include "linecombo.h"
#include "spalette.h"
#include "fontcombo.h"
#include "colorcombo.h"
class StyleSelect;
#include "alignselect.h"
#include "shadebutton.h"
class Cpalette;
class Autoforms;
class ArrowChooser;
class ScComboBox;
class ScribusMainWindow;
class UserActionSniffer;
/*
class SCRIBUS_API LabelButton : public QLabel
{
	Q_OBJECT

public:
	LabelButton(QWidget* parent, QString text1, QString text2);
	~LabelButton() {};
	void setTexts(QString text1, QString text2);
	bool getState();

private:
	bool state;
	QString TextA;
	QString TextB;

signals:
	void clicked();

protected:
	virtual void enterEvent(QEvent*);
	virtual void leaveEvent(QEvent*);
	virtual void mousePressEvent(QMouseEvent*);
	virtual void mouseReleaseEvent(QMouseEvent *);
};
*/
class SCRIBUS_API NameWidget : public QLineEdit
{
	Q_OBJECT

public:
	NameWidget(QWidget* parent);
	~NameWidget() {};

signals:
	void Leaved();

protected:
	virtual void focusOutEvent(QFocusEvent *);
};

class SCRIBUS_API Mpalette : public ScrPaletteBase
{
	Q_OBJECT

public:
	Mpalette(QWidget* parent);
	~Mpalette() {};

	virtual void closeEvent(QCloseEvent *closeEvent);
	void updateColorSpecialGradient();
	const VGradient getFillGradient();
	void updateColorList();
	void setGradientEditMode(bool);
	void updateCmsList();
	void setTextFlowMode(PageItem::TextFlowMode mode);
	void ShowCMS();
	/*! \brief fills the langs combobox in language specific order
	\author 10/07/2004 - Petr Vanek - rewritten to fix #1185.
	Uses sortQStringList from utils.cpp - STL!
	\param langMap a structure with languages/hyphs*/
	void fillLangCombo(QMap<QString,QString> langMap);
	/** @brief Returns true if there is a user action going on at the moment of call. */
	bool userActionOn(); // not yet implemented!!! This is needed badly.
                         // When user releases the mouse button or arrow key, changes must be checked
                         // and if in ScribusView a groupTransaction has been started it must be also
                         // commmited

	Cpalette *Cpal;
	Autoforms* SCustom;
	Autoforms* SCustom2;
	ParaStyleComboBox *paraStyleCombo;
	CharStyleComboBox *charStyleCombo;
	FontComboH* Fonts;
	ArrowChooser* startArrow;
	ArrowChooser* endArrow;
	Q3ButtonGroup* RotationGroup;
	Q3ButtonGroup* textFlowOptions;
	Q3ButtonGroup* textFlowOptions2;
	
public slots:
	void setMainWindow(ScribusMainWindow *mw);
	void languageChange();
	void setDoc(ScribusDoc *d);
	void unsetDoc();
	void unsetItem();
//	void setCurrentItem(PageItem *);
	void setMultipleSelection(bool);
	void NewSel(int nr);
	void SetCurItem(PageItem *i);
	void unitChange();
	void setLevel(uint l);
	void setXY(double x, double y);
	void setBH(double x, double y);
	void setR(double r);
	void setRR(double r);
	void setCols(int r, double g);
	void setLspMode(int id);
	void setLsp(double r);
	void setSize(int s);
	void setFontFace(const QString&);
	void setExtra(int e);
	void setDvals(double left, double top, double bottom, double right);
	void ChangeScaling();
	void setLvalue(double scx, double scy, double x, double y);
	void setSvalue(double s);
	void setLIvalue(Qt::PenStyle p, Qt::PenCapStyle pc, Qt::PenJoinStyle pj);
	/// update TB values:
	void updateStyle(const ParagraphStyle& newCurrent);
	void setStil(int s);
	void setAli(int e);
	void setParStyle(const QString& name);
	void setCharStyle(const QString& name);
	void setOpticalMargins(int);
	void setMinWordTracking();
	void setNormWordTracking();
	void setMinGlyphExtension();
	void setMaxGlyphExtension();
	void setShadowOffs(int x, int y);
	void setUnderline(int p, int w);
	void newUnderline();
	void setStrike(int p, int w);
	void newStrike();
	void setOutlineW(int x);
	void newOutlineW();
	void setTScale(int e);
	void NewTScale();
	void NewTScaleV();
	void NewTBase();
	void setTScaleV(int e);
	void setTBase(int e);
	void SetLineFormats(ScribusDoc *dd);
	void SetSTline(Q3ListBoxItem *c);
	void NewTFont(QString);
	void newTxtFill();
	void newTxtStroke();
	void setActShade();
	void setActFarben(QString p, QString b, int shp, int shb);
	void ManageTabs();
	void setLocked(bool);
	void setSizeLocked(bool);
	void setPrintingEnabled(bool);
	void setFlippedH(bool);
	void setFlippedV(bool);

private slots:
	void SelTab(int t);
	void NewX();
	void NewY();
	void NewW();
	void NewH();
	void NewR();
	void NewRR();
	void NewLsp();
	void HandleGapSwitch();
	void NewCols();
	void NewGap();
	void NewSize();
	void NewExtra();
	void handleFlipH();
	void handleFlipV();
	void ToggleKette();
	void HChange();
	void VChange();
	void ToggleKetteD();
	void HChangeD();
	void VChangeD();
	void NewLocalXY();
	void NewLocalSC();
	void NewLocalDpi();
	void EditEff();
	void EditPSD();
	void NewLS();
	void NewLSty();
	void NewLJoin();
	void NewLEnd();
	void NewAli(int a);
	void setTypeStyle(int s);
	void newShadowOffs();
	void NewLMode();
	void DoLower();
	void DoRaise();
	void DoFront();
	void DoBack();
	void NewRotMode(int m);
	void DoFlow(int id);
	void MakeIrre(int f, int c, double *vals);
	void EditSh();
	void EditSh2();
	void NewTDist();
	void NewSpGradient(double x1, double y1, double x2, double y2);
	void toggleGradientEdit();
	void DoRevert();
	void handleLock();
	void handleLockSize();
	void handlePrint();
	void handlePathType();
	void handlePathFlip();
	void handlePathLine();
	void handlePathDist();
	void handlePathOffs();
	void handleFillRule();
	void handleOverprint();
	void ChProf(const QString& prn);
	void ChIntent();
	void NewName();
	void NewLanguage();
	void HandleTLines();
	void setStartArrow(int id);
	void setEndArrow(int id);
	void setGroupTransparency(int trans);
	void setGroupBlending(int blend);
	void doGrouping();

protected slots:
	//virtual void reject();
	void spinboxStartUserAction();
	void spinboxFinishUserAction();
	void updateSpinBoxConstants();

signals:
	void DocChanged();
	void NewParStyle(int);
	void NewAlignment(int);
	void NewEffects(int);
	void ShapeEdit();
	void NewFont(const QString&);
	void UpdtGui(int);

protected:
	ScribusMainWindow *m_ScMW;
	
	Q3VBoxLayout* MpalLayout;
	Q3VBoxLayout* pageLayout;
	Q3VBoxLayout* pageLayout_2;
	Q3VBoxLayout* pageLayout_2a;
	Q3VBoxLayout* pageLayout_2b;
	Q3VBoxLayout* pageLayout_2c;
	Q3VBoxLayout* pageLayout_3;
	Q3VBoxLayout* pageLayout_4;
	Q3VBoxLayout* pageLayout_5;
	Q3VBoxLayout* pageLayout_5a;
	Q3VBoxLayout* pageLayout_5b;
	Q3VBoxLayout* pageLayout_6;
	Q3VBoxLayout* OverPLayout;
	Q3VBoxLayout* TLineLayout;
	Q3HBoxLayout* layout60;
	Q3GridLayout* Layout44;
	Q3HBoxLayout* Layout13;
	Q3GridLayout* Layout12;
	Q3HBoxLayout* layout47;
	Q3VBoxLayout* layout46;
	Q3GridLayout* layout41;
	Q3HBoxLayout* Layout1;
	Q3HBoxLayout* Layout1AL;
	Q3GridLayout* layout43;
	Q3VBoxLayout* Layout24;
	Q3HBoxLayout* Layout18;
	Q3HBoxLayout* Layout13_2;
	Q3GridLayout* Layout12_2;
	Q3HBoxLayout* NameGroupLayout;
	Q3GridLayout* GeoGroupLayout;
	Q3GridLayout* LayerGroupLayout;
	Q3HBoxLayout* RotationGroupLayout;
	Q3HBoxLayout* ShapeGroupLayout;
	Q3GridLayout* DistanceLayout;
	Q3GridLayout* DistanceLayout2;
	Q3VBoxLayout* DistanceLayout3;
	Q3GridLayout* GroupBox3aLayout;
	Q3VBoxLayout* GroupBoxCMLayout;
	Q3VBoxLayout* textFlowOptionsLayout;
	Q3VBoxLayout* textFlowOptionsLayout2;
	Q3HBoxLayout* layout23;
	Q3HBoxLayout* layout24;
	Q3VBoxLayout* page_group_layout;
	Q3HBoxLayout* ShapeGroupLayout2;
	Q3GridLayout* Layout1t;
	Q3HBoxLayout* wordTrackingHLayout;
	Q3HBoxLayout* glyphExtensionHLayout;
	
	NameWidget* NameEdit;
	
	QWidget* page;
	QWidget* page_2;
	QWidget* page_2a;
	QWidget* page_2b;
	QWidget* page_2c;
	QWidget* page_3;
	QWidget* page_4;
	QWidget* page_5;
	QWidget* page_5a;
	QWidget* page_5b;
	QWidget* page_6;
	QWidget* page_group;
	
	QLabel* xposLabel;
	QLabel* widthLabel;
	QLabel* yposLabel;
	QLabel* heightLabel;
	QLabel* rotationLabel;
	QLabel* basepointLabel;
	QLabel* LevelTxt;
	QLabel* SRect;
	QLabel* SRect2;
	QLabel* rndcornersLabel;
	QLabel* startoffsetLabel;
	QLabel* distfromcurveLabel;
	QLabel* pathTextTypeLabel;
	QLabel* topLabel;
	QLabel* columnsLabel;
	QLabel* bottomLabel;
	QLabel* leftLabel;
	QLabel* rightLabel;
	QLabel* trackingLabel;
	QLabel* fontsizeLabel;
	QLabel* yscaleLabel;
	QLabel* xscaleLabel;
	QLabel* xposImgLabel;
	QLabel* yposImgLabel;
	QLabel* linewidthLabel;
	QLabel* endingsLabel;
	QLabel* linetypeLabel;
	QLabel* StrokeIcon;
	QLabel* ShadeTxt2;
	QLabel* ShadeTxt1;
	QLabel* FillIcon;
	QLabel* paraStyleLabel;
	QLabel* charStyleLabel;
	QLabel* langLabel;
	QLabel* LineModeT;
	QLabel* TextCms1;
	QLabel* TextCms2;
	QLabel* edgesLabel;
	QLabel* ChBaseTxt;
	QLabel* ScaleTxt;
	QLabel* ScaleTxtV;
	QLabel* imgDPIXLabel;
	QLabel* imgDPIYLabel;
	QLabel* startArrowText;
	QLabel* endArrowText;
	QLabel* TransTxt;
	QLabel* TransTxt2;
	QLabel* optMarginLabel;
	QLabel* wordTrackingLabel;
	QLabel* minWordTrackingLabel;
	QLabel* normWordTrackingLabel;
	QLabel* glyphExtensionLabel;
	QLabel* minGlyphExtensionLabel;
	QLabel* maxGlyphExtensionLabel;

//	LabelButton* colgapLabel;
	ScComboBox* colgapLabel;
	StyleSelect* SeStyle;
	AlignSelect* GroupAlign;

	LinkButton* keepImageWHRatioButton;
	LinkButton* keepFrameWHRatioButton;
	LinkButton* keepImageDPIRatioButton;
	LineCombo* LStyle;

	ShadeButton *PM1;
	ShadeButton *PM2;
	PageItem *CurItem;
	bool HaveDoc;
	bool HaveItem;
	ScribusDoc *doc;
	double m_unitRatio;
	int m_unitIndex;
	bool LMode;
	double RoVal;


	Q3ButtonGroup* ShapeGroup;
	Q3ButtonGroup* ShapeGroup2;
	Q3ButtonGroup* Distance3;
	Q3ButtonGroup* OverP;

	QToolButton* TabsButton;

	QToolButton* Zup;
	QToolButton* ZDown;
	QToolButton* ZTop;
	QToolButton* ZBottom;

	QToolBox* TabStack;
	Q3WidgetStack* TabStack2;
	Q3WidgetStack* TabStack3;


	Q3GroupBox* NameGroup;
	Q3GroupBox* GeoGroup;
	Q3GroupBox* LayerGroup;
	Q3GroupBox* Distance;
	Q3GroupBox* Distance2;
	Q3GroupBox* GroupBoxCM;
	Q3GroupBox* TLines;
	Q3GroupBox* GroupBox3a;
	Q3GroupBox* TransGroup;

	QToolButton* textFlowDisabled;
	QToolButton* textFlowUsesFrameShape;
	QToolButton* textFlowUsesBoundingBox;
	QToolButton* textFlowUsesContourLine;
	QToolButton* textFlowUsesImageClipping;
	QToolButton* textFlowDisabled2;
	QToolButton* textFlowUsesFrameShape2;
	QToolButton* textFlowUsesBoundingBox2;
	QToolButton* textFlowUsesContourLine2;
	QToolButton* textFlowUsesImageClipping2;
	QCheckBox* Aspect;
	QCheckBox* flippedPathText;
	QCheckBox* showcurveCheckBox;
	QCheckBox* TopLine;
	QCheckBox* LeftLine;
	QCheckBox* RightLine;
	QCheckBox* BottomLine;

	ScComboBox* pathTextType;
	QComboBox* langCombo;
	QComboBox* InputP;
	QComboBox* MonitorI;
	QComboBox* LineMode;
	ColorCombo* TxStroke;
	QComboBox* LJoinStyle;
	QComboBox* LEndStyle;
	ColorCombo* TxFill;
	ScComboBox* blendMode;
	QComboBox *optMarginCombo;

	Q3ListBox* StyledLine;

	ScrSpinBox* Width;
	ScrSpinBox* Xpos;
	ScrSpinBox* Ypos;
	ScrSpinBox* Height;
	ScrSpinBox* Rot;
	ScrSpinBox* RoundRect;
	ScrSpinBox* dGap;
	ScrSpinBox* DTop;
	ScrSpinBox* DBottom;
	ScrSpinBox* DLeft;
	ScrSpinBox* DRight;
	ScrSpinBox* Dist;
	ScrSpinBox* LineW;
	ScrSpinBox* Size;
	ScrSpinBox* LineSp;
	ScrSpinBox* Extra;
	ScrSpinBox* imageYOffsetSpinBox;
	ScrSpinBox* imageXOffsetSpinBox;
	ScrSpinBox* imageYScaleSpinBox;
	ScrSpinBox* imageXScaleSpinBox;
	ScrSpinBox* imgDpiX;
	ScrSpinBox* imgDpiY;
	ScrSpinBox* LSize;
	ScrSpinBox* ChScale;
	ScrSpinBox* ChScaleV;
	ScrSpinBox* ChBase;
	ScrSpinBox* minWordTrackingSpinBox;
	ScrSpinBox* normWordTrackingSpinBox;
	ScrSpinBox* minGlyphExtSpinBox;
	ScrSpinBox* maxGlyphExtSpinBox;
	QSpinBox* DCol;
	QSpinBox* TransSpin;

	QRadioButton* TopLeft;
	QRadioButton* TopRight;
	QRadioButton* Center;
	QRadioButton* BottomLeft;
	QRadioButton* BottomRight;
	QRadioButton* FreeScale;
	QRadioButton* FrameScale;
	QRadioButton* EvenOdd;
	QRadioButton* NonZero;
	QRadioButton* KnockOut;
	QRadioButton* Overprint;

	Q3Frame* Line1;
	Q3Frame* Line2;
	Q3Frame* Line4;
	Q3Frame* Line5;
	Q3Frame* Frame4;

	Q3PopupMenu* lineSpacingPop;

	QToolButton* linespacingButton;
	QToolButton* DoGroup;
	QToolButton* DoUnGroup;
	QToolButton* FlipH;
	QToolButton* FlipV;
	QToolButton* Locked;
	QToolButton* NoPrint;
	QToolButton* NoResize;
	QToolButton* Revert;
	QToolButton* EditShape;
	QToolButton* EditShape2;
	QToolButton* EditEffects;
	QToolButton* EditPSDProps;

	
	bool _userActionOn;
	UserActionSniffer *userActionSniffer;
	void installSniffer(ScrSpinBox *spinBox);
	
	int idXYZItem;
	int idShapeItem;
	int idTextItem;
	int idImageItem;
	int idLineItem;
	int idColorsItem;
	int idGroupItem;
};

class SCRIBUS_API UserActionSniffer : public QObject
{
	Q_OBJECT

public:
	UserActionSniffer();
protected:
	bool eventFilter( QObject *o, QEvent *e );
signals:
	void actionStart();
	void actionEnd();
};

#endif
