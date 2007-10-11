/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef MPALETTE_H
#define MPALETTE_H

#include <QListWidgetItem>
#include <QLineEdit>

class QButtonGroup;
class QCheckBox;
class QCloseEvent;
class QComboBox;
class QEvent;
class QEvent;
class QFocusEvent;
class QFrame;
class QGridLayout;
class QGroupBox;
class QHBoxLayout;
class QLabel;
class QMenu;
class QPushButton;
class QRadioButton;
class QSpinBox;
class QStackedWidget;
class QStackedWidget;
class QToolBox;
class QVBoxLayout;
class QWidget;


#include "scribusapi.h"
#include "scrpalettebase.h"
#include "scrspinbox.h"
#include "pageitem.h"
#include "page.h"
#include "linkbutton.h"
#include "linecombo.h"
#include "spalette.h"
#include "fontcombo.h"
#include "colorcombo.h"
#include "alignselect.h"
#include "shadebutton.h"
#include "sclistboxpixmap.h"
#include "scguardedptr.h"

class StyleSelect;
class ScribusDoc;
class Cpalette;
class Autoforms;
class ArrowChooser;
class ScComboBox;
class ScribusMainWindow;
class UserActionSniffer;


struct SCRIBUS_API LineFormatValue
{
	multiLine m_Line;
	ScGuardedPtr<ScribusDoc> m_doc;
	QString m_name;
	
	LineFormatValue();
	LineFormatValue( const multiLine& line, ScribusDoc* doc, const QString name );
	LineFormatValue(const LineFormatValue& other);
	LineFormatValue& operator= (const LineFormatValue& other);
};


Q_DECLARE_METATYPE(LineFormatValue);


class SCRIBUS_API LineFormatItem : public QListWidgetItem
{	
	enum {
		LineFormatUserType = UserType + 2
	} usrtyp;
	
public:	
	LineFormatItem( ScribusDoc* doc, const multiLine& line, const QString& name ) : QListWidgetItem(NULL, LineFormatUserType)
	{		
		setText(name);
		setData(Qt::UserRole, QVariant::fromValue(LineFormatValue(line, doc, name))); 
	};
	LineFormatItem( ) : QListWidgetItem(NULL, LineFormatUserType)
	{		
		setText("");
		setData(Qt::UserRole, QVariant::fromValue(LineFormatValue())); 
	};
	LineFormatItem * clone () const { return new LineFormatItem(*this); }
};



class SCRIBUS_API LineFormatItemDelegate : public ScListBoxPixmap<37, 37>
{
public:
	LineFormatItemDelegate() : ScListBoxPixmap<37, 37>() {}
	virtual int rtti() const { return 148523874; }
	virtual void redraw(const QVariant&) const;
};



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

class SCRIBUS_API PropertiesPalette : public ScrPaletteBase
{
	Q_OBJECT

public:
	PropertiesPalette(QWidget* parent);
	~PropertiesPalette() {};

	virtual void changeEvent(QEvent *e);
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
	QButtonGroup* RotationGroup;
	QRadioButton* TopLeft;
	QRadioButton* TopRight;
	QRadioButton* Center;
	QRadioButton* BottomLeft;
	QRadioButton* BottomRight;
	QGroupBox* textFlowOptions;
	QGroupBox* textFlowOptions2;
	QButtonGroup* textFlowOptionsB;
	QButtonGroup* textFlowOptionsB2;
	
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
	void setLspMode(QAction *);
	void setLsp(double r);
	void setSize(double s);
	void setFontFace(const QString&);
	void setExtra(double e);
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
	void setShadowOffs(double x, double y);
	void setUnderline(double p, double w);
	void newUnderline();
	void setStrike(double p, double w);
	void newStrike();
	void setOutlineW(double x);
	void newOutlineW();
	void setTScale(double e);
	void NewTScale();
	void NewTScaleV();
	void NewTBase();
	void setTScaleV(double e);
	void setTBase(double e);
	void SetLineFormats(ScribusDoc *dd);
	void SetSTline(QListWidgetItem *c);
	void NewTFont(QString);
	void newTxtFill();
	void newTxtStroke();
	void setActShade();
	void setActFarben(QString p, QString b, double shp, double shb);
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
	void DoFlow();
	void MakeIrre(int f, int c, double *vals);
	void EditSh();
	void EditSh2();
	void NewTDist();
	void NewSpGradient(double x1, double y1, double x2, double y2);
	void toggleGradientEdit();
	void DoRevert();
	void doClearCStyle();
	void doClearPStyle();
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

	QVBoxLayout* MpalLayout;
	QVBoxLayout* pageLayout;
	QVBoxLayout* pageLayout_2;
	QVBoxLayout* pageLayout_2a;
	QVBoxLayout* pageLayout_2b;
	QVBoxLayout* pageLayout_2c;
	QVBoxLayout* pageLayout_3;
	QVBoxLayout* pageLayout_4;
	QVBoxLayout* pageLayout_5;
	QVBoxLayout* pageLayout_5a;
	QVBoxLayout* pageLayout_5b;
	QVBoxLayout* pageLayout_6;
	QVBoxLayout* OverPLayout;
	QVBoxLayout* TLineLayout;
	QHBoxLayout* layout60;
	QGridLayout* Layout44;
	QHBoxLayout* Layout13;
	QGridLayout* Layout12;
	QHBoxLayout* layout47;
	QVBoxLayout* layout46;
	QGridLayout* layout41;
	QHBoxLayout* Layout1;
	QHBoxLayout* Layout1AL;
	QGridLayout* layout43;
	QVBoxLayout* Layout24;
	QHBoxLayout* Layout18;
	QGridLayout* Layout12_2;
	QHBoxLayout* NameGroupLayout;
	QGridLayout* GeoGroupLayout;
	QGridLayout* LayerGroupLayout;
	QHBoxLayout* ShapeGroupLayout;
	QGridLayout* DistanceLayout;
	QGridLayout* DistanceLayout2;
	QVBoxLayout* DistanceLayout3;
	QGridLayout* GroupBox3aLayout;
	QVBoxLayout* GroupBoxCMLayout;
	QVBoxLayout* textFlowOptionsLayout;
	QVBoxLayout* textFlowOptionsLayout2;
	QHBoxLayout* layout23;
	QHBoxLayout* layout24;
	QVBoxLayout* page_group_layout;
	QHBoxLayout* ShapeGroupLayout2;
	QGridLayout* Layout1t;
	QHBoxLayout* wordTrackingHLayout;
	QHBoxLayout* glyphExtensionHLayout;
	
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


	QGroupBox* ShapeGroup;
	QGroupBox* ShapeGroup2;
	QGroupBox* Distance3;
	QGroupBox* OverP;

	QToolButton* TabsButton;

	QToolButton* Zup;
	QToolButton* ZDown;
	QToolButton* ZTop;
	QToolButton* ZBottom;

	QToolBox* TabStack;
	QStackedWidget* TabStack2;
	QStackedWidget* TabStack3;


	QGroupBox* NameGroup;
	QGroupBox* GeoGroup;
	QGroupBox* LayerGroup;
	QGroupBox* Distance;
	QGroupBox* Distance2;
	QFrame* GroupBoxCM;
	QGroupBox* TLines;
	QGroupBox* GroupBox3a;
	QGroupBox* TransGroup;

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

	QListWidget* StyledLine;

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

	QRadioButton* FreeScale;
	QRadioButton* FrameScale;
	QRadioButton* EvenOdd;
	QRadioButton* NonZero;
	QRadioButton* KnockOut;
	QRadioButton* Overprint;

	QFrame* Line1;
	QFrame* Line2;
	QFrame* Line4;
	QFrame* Line5;
	QFrame* Frame4;

	QMenu* lineSpacingPop;

	QToolButton* linespacingButton;
	QToolButton* DoGroup;
	QToolButton* DoUnGroup;
	QToolButton* FlipH;
	QToolButton* FlipV;
	QToolButton* Locked;
	QToolButton* NoPrint;
	QToolButton* NoResize;
	QToolButton* Revert;
	QToolButton* charStyleClear;
	QToolButton* paraStyleClear;
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
	UserActionSniffer( QObject* parent );
protected:
	bool eventFilter( QObject *o, QEvent *e );
signals:
	void actionStart();
	void actionEnd();
};

#endif
